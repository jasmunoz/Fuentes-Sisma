/*********************************************************************
 conexhsm : conectar el socket, retorna el socket de conexion, y de entrada 
            tiene la entidad
            In : entidad, datos NPTDHSM opcionales
            return: int socket (<0 si es error)
                        
 desconexhsm : desconectar el socket, de entrada el socket que queremos 
               desconectar 
               retorna void
 claveshsm: recupera las claves necesarias para ejecutar un comando
            respecto a un subsistema (red, cajeros, estampadora, claves internas)
            In : entidad, comando, subsistema, bin
            Out: clave1, clave2 
            retorna 0 ok,-1 ko
 manejhsm: conexion con hsm y comandos 
          Parametros de entrada:
		  char * clavehsm, char* comandohsm, int tipcla, char * datoshsm, long NumByte, struct sfdssl * sockssl, char * prespuestahsm) 
               char * clavehsm      : clave o claves necesarias concatenadas (char de 150)
               char * comandohsm    : comando que hay que ejecutar (codigo de 2 posiciones)
               int tipcla						: indica si la clave es TDES o AES 00 = TDES sin PIN, 03 = TDES, 05 = AES //PRJ0258542
               char * datoshsm      : datos para la entrada del comando 
			   long NumByte			: longitud de los datos de entrada
			   struct sfdssl * sockssl : socket por el que estamos conectado
          Parametros de salida:    
               char * prespuestahsm : datos de respuesta
          Retorna la longitud de la respuesta ok >0
          Si hay error <0
             -2: error de validacion
             -1: error no esperado
             -145: error de comunicacion

// DMND0037144 CAMBIOS FICHERO ESTAMPACION OPCION 1 - BIMSF-3123
BIMSF-3123
	CPRCSO
		ABRIR-HSM                 VALUE '01'.
		OBTENER-CLAVES            VALUE '02'.
		OBTENER-VALOR             VALUE '03'.
		CERRAR-HSM                VALUE '04'.
		TODO                      VALUE '05'.
	COMANDO-TRAD
		CIFRADO-DATO-TRAD         VALUE '14'.
		DESCIFRADO-DATO-TRAD      VALUE '15'.
		CIFRADO-TXT-TRAD          VALUE '17'.
		DESCIFRADO-TXT-TRAD       VALUE '18'.
		HASHEADO-TRAD             VALUE '30'.
		RSA-TRAD                  VALUE '31'.
		CIFRADO-FICH-TRAD         VALUE '33'.
		DESCIFRADO-FICH-TRAD      VALUE '34'.
	CRTRNO 
		OK-SMPXHSM0               VALUE '01'.
		ERROR-IMPREVISTO          VALUE '02'.
		ERROR-PREVISTO            VALUE '03'.
		ERROR-PREVISTO-ROLLBACK   VALUE '04'.
	CRSPSTA
		CORRECTO                  VALUE '01'.
		DATOS-ENTRADA-ERRONEOS    VALUE '02'.
		ERROR-COMUNICACION        VALUE '03'.
		OTRO-ERROR-HSM            VALUE '04'.

			 
***********************************************************************/

#include <stdio.h>
#include <string.h> 
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/time.h> 
#include <time.h>
#include <errno.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>
#include "conexhsm.h"
#include "utilidades.h"


#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#define LONMSG_IN   4096
#define CAFILE "Certificado.pem"

typedef struct sfdssl {         /* estructura conjunta para socket y socket seguro */
 int sfd_ssl;                   
 SSL * ssl_ssl;                               
} sfdssl;
struct sfdssl * sockssl; //PRJ0258542 - Fase 3


/**************** prototipo de funciones ****************/
extern int tratar_fich(char* fichero, OPCION_FICH opcion, void *registro, void *clave);
extern int tratar_fich1(char* ficher, OPCION_FICH opcion, char * registro, long tamanoE); //PRJ0258542 - Fase 3
in_addr_t inet_addr(const char *cp);
long manejhsm(char * clavehsm, char* comandohsm, int tipcla, char * datoshsm, long NumByte,  struct sfdssl * sockssl , char * prespuestahsm); //PRJ0258542 - Se pone el tipo de clave para saber si es AES o TDES y cambia de long a int el retorno
int conexhsm(char* codent, NPSDHSM reg_hsm);

void desconexhsm(int sock);
int claveshsm(char * comando, char * subsistema, char * bin, char * clave1, char * clave2);

//******* llamada desde cobol, que deber ser todos char*
void manejhsm_C(char * clavehsm, char* comandohsm, char * datoshsm, char *NumByte, struct sfdssl * sockssl, char * prespuestahsm, char * codResp); 
void conexhsm_C(char* codent, char *comando, struct sfdssl * sockssl, char * codResp);
void desconexhsm_C(struct sfdssl * sockssl, char * codResp);
void claveshsm_C(char * comando, char * subsistema, char * bin, char * clave1, char * clave2, char * codResp);
void conexhsm_1(char * ipcon, char * puerto, char * tiempo, char * cabece, struct sfdssl * sockssl, char * codResp, int numHSM); //PRJ0258542 - Fase 3

void fEliminar_Blancos (char *, long);
int cliente();
int fIdentificaComando(int iFuncion); //Identifica el comando de entrada
int enviar_mensajes_hsm(SSL *ssl, char *, int);
int leer_linea (SSL *, int, char *, int, int);

int fcomponerPIN(char * cad_cadena, char * cad_claves, char * cab_comando);
int fcifrar_cadena(char * cod_CifDescif,SSL *ssl, char * datos_cif, long long_cif, char * clave_crip, int fd);//compone el mensaje de cifrado de datos
int fObtenerPIN(SSL *, char *, char *, char *, char *, int);
//int fHexa_binario(char * cadena, char *resultado);
void fHexa_binario(char * cadena, char *long_cadena, char *resultado); //Jira BIMSF-3123 - mando la longitud
void fOrExclusivo (char * , char * , char *);
void fbinario_Hexa(char * , char *);
int fHexa_ascii (char * cadena, char * resultado);
void fAscii_Hexa (int lonAscii, char * charAscii, char * charHexa);
int hex_to_int(char c);
int hex_to_ascii(char i, char d);
int recv_to(SSL *ssl, char *buffer, int to, int fd);
int getLenMsgToReceive(SSL *ssl,int fd, char *buffer, int to);
void finalConex();
int fConexIP(int iHSM);
int fConexHSM(char * ipcon, char * puerto, char * tiempo, char * cabece, int iHSM); //PRJ0258542 - Fase 3
int mandatest(); //PRJ0258542 - Fase 3
void fechahorasys(struct tm_dec *tmPtr);
//PRJ270893 - 03-2024
void fHex_bin (char * cadena, char * long_cadena, char * resultado);
//PRJ270893 - 03-2024


//añadido para HSM 901, 1103, 2000
int fHash(char * hashMec, SSL * ssl, char * datos_cif, long long_cif, char * status, int sfd); //HASH
int fRsa(SSL *ssl, char * datos_cif, long long_cif, char * status, int sfd); //RSA
int fcifrarPIN(char * cad_cadena, char * cad_claves, char * cab_comando);
char *b64_encode(const unsigned char *in, size_t len);
char* hex_2_base64(char *_hex);

//añadido para PRICE
int fcomponerMAC(char * cad_datos, char * comando_pet, int tipcla, char * clave_mac, long long_mac);  //MAC //PRJ0258542 - se anade el 3º parametro para saber el tipo de claves
int fcomponerCMAC(char * cad_datos, char * comando_pet, int tipcla, char * clave_mac, long long_mac);  //CMAC //PRJ0258542 - para calculo de MAC en mensajes de intercambio
int fcomponerImpor(char * cad_datos, char * comando_pet, char * modo, int tipcla, char * clave_imp, long long_imp);  //IMPORT //PRJ0258542 - para importar las claves TR-31 a AES
int fcomponerKCV(char * cad_datos, char * comando_pet, long long_kcv);  //KCV //PRJ0258542 - para calculo de KCV
int fcomponerOFF(char * cad_datos, char * comando_pet);  //OFFSET //PRJ0258542 - para calculo del OffSet desde el bloque de PIN
int fcomponerCRIPTOGRAMA(char * cad_datos, char * cad_clave, char * comando_pet, char * operacion); //ARQC - ARPC
int fcomponerALTA(char * cad_datos, char * comando_pet, int tipcla);  //PRJ0258542 - para alta de nuevas claves
int fcomponerBAJA(char * cad_datos, char * comando_pet, int tipcla, char * clave_baja, long long_baj);  //PRJ0258542 - para baja de claves en el HSM
int fcomponerClaveTR(char * cad_datos, char * comando_pet, int tipcla, char * clave_imp, long long_imp); //PRJ0258542 - exportar la clave solicitada a formato TR31 para enviar a Redsys
int fcomponerClaveTR(char * cad_datos, char * comando_pet, int tipcla, char * clave_imp, long long_imp); //PRJ0258542 - exportar la clave solicitada a formato TR31 para enviar a Redsys

//añadido para el 601 , 604, 611 y 903
int addPadding(char* in, long long_dat);
int addPadding32(char* in, long long_dat); 
int SubPadding(char* in, long long_dat);
int fcifrar_cadena_aes(char * cod_CifDescif,SSL *ssl, char * datos_cif, long long_cif, char * clave_crip,int sfd,char * vector_inicial_aes);//compone el mensaje de cifrado de datos
int foffset(SSL *ssl, char * datos_cif, long long_cif, char * clave_crip,int sfd);//compone el mensaje para el offset
void printf_pcadena ( char * p);

//****** variables del programa
int eftport; //Puerto pasado a entero
unsigned long net_addr;
struct sockaddr_in ser_addr;// <sys/socket.h>
char clave_H[5];
//char caddefe[8];
char cabecera[4]; 
char cadport[5];
char cadname[16];
//char aux_time[5];
char sentencia[100];
int timeout;
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
tm_dec tm;
const int ERROR_STATUS = -1;

// estnuctura para guardar el contenido del listado de claves internas del HSM    
typedef struct
{
    char identificador[6]; 
    char lmk[3];
} tficha;

// matriz de estructuras tficha 
tficha registros[99]; 

//certificados 
SSL_CTX *ctx;  

//********************************************************************************
///FUNCION CONEXHSM, que conecta con el socket
//   retorna el numero de socket que se usa, o un numero menor de 0
//********************************************************************************
int conexhsm(char* codent, NPSDHSM reg_hsm)  
{

    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Entrando rutina conexhsm  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  int retorno=0;
  int valor=0;
  int i=0;
  char aux_time[5];
  char indPuertoOK[2];
  NPSINCI reginci;
  int long_codent= 0;
  char* tiempo;
  
    //BIMSF-3123
	printf("** ** Datos conexion de entrada codent >%s< puerto >%.4s< ip1>%.15s< ip2>%.15s<\n"
		,codent
        ,&reg_hsm.dhpuerto
        ,&reg_hsm.dhnumip
        ,&reg_hsm.dhnumip2);
   
//************** LEEMOS DE LA TABLA NPTDHSM ***********
  if  (strncmp(reg_hsm.dhpuerto, "    ",4))
  //si no es espacio, es que viene relleno
  { 
     printf("Datos conexion de entrada puerto vacios >%.4s< ip1>%.15s< ip2>%.15s<\n"
        ,&reg_hsm.dhpuerto
         ,&reg_hsm.dhnumip
         ,&reg_hsm.dhnumip2);
  }
  else
  {
    strncpy(clave_H, codent, 4);
    clave_H[4]='\0';
    retorno = tratar_fich("NPTDHSM", IGUAL, &reg_hsm, clave_H);
    if(retorno)
    {
     printf ("HSM: error al leer NPTDHSM\n");
     return(-1);    
    }
  }
   
  ////// PUERTO
  sprintf(cadport, "%.04s", reg_hsm.dhpuerto);  
  eftport=atoi(cadport);
     
  // recupera los valores de la cabecera y timeout
  strncpy(aux_time, reg_hsm.dhtiempo, 4);
  aux_time[4]='\0';
  printf("tiempo >%s<\n", aux_time);  
  timeout = atoi(aux_time);
  printf("timeout >%d<\n", timeout); 
  strncpy(cabecera, reg_hsm.dhcabece, 3);
  cabecera[3]='\0'; // la cabecera la ponemos de 3 "CAB"
    
  ///// IP PRINCIPAL
  strncpy(cadname, reg_hsm.dhnumip, sizeof(reg_hsm.dhnumip));    
  cadname[sizeof(reg_hsm.dhnumip)]='\0';
  
  //conectar el socket, debe estar en la conexion para no hacerlo siempre
  valor = cliente(); //retorna el identificador del socket
  
  //BIMSF-3123
  printf("** ** Socket conectado en el cliente valor %i\n", valor);
   
  if (valor < 0)
  {
     printf("HSM: HOSTNAME 1 es :>%s< puerto :>%d<\n",cadname,eftport);
     printf("HSM: retorno de funcion cliente con IP principal >%d<\n", valor);
     ////// IP SECUNDARIA
     strncpy(cadname, reg_hsm.dhnumip2, sizeof(reg_hsm.dhnumip2));    
     cadname[sizeof(reg_hsm.dhnumip2)]='\0';
     
     printf("HSM: conectar IP secundaria >%s< puerto :>%d<\n",cadname,eftport);
     
     valor = cliente(); 
     if (valor < 0)
     {  printf("HSM: HOSTNAME 2 es :>%s<puerto :>%d<\n",cadname, eftport);
        printf("HSM: retorno de funcion cliente con IP secundaria >%d<\n", valor);
     }
  }
  if (valor > 0)
  {
     printf("** ** Conectado HSM id socket >%d<\n", valor);
     printf("** ** HSM: conectado IP >%s< puerto :>%d<\n",cadname,eftport);
  }
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Saliendo rutina conexhsm  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf(" ** ** Valor de salida %i\n", valor);
    return(valor);   
}




//********************************************************************************
///FUNCION DESCONEXHSM, que desconecta con el socket
//********************************************************************************
void desconexhsm(int sock)
{
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Entrando rutina desconexhsm  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  int err;

  err=shutdown(sock,2);
  err=close(sock);
  //perror("He cerrado el socket");
  if(err)
    printf("HSM: He cerrado el socket. err = %d , errno = %d\n",err,errno);
  //exit (0);

    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Saliendo rutina desconexhsm  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
}

//********************************************************************************
///FUNCION fConexIP, conecta al hsm 1 principal o 2 secundario sin cambiar entre ellos
//********************************************************************************
int fConexIP(int iHSM) 
{
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Entrando rutina fConexIP  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  int retorno=0;
  int valor=0;
  int i=0;
  char aux_time[5];
  char indPuertoOK[2];
  int segundos;
  NPSDHSM reg_hsm;
  NPSINCI reginci;
  
  //coger fecha del sistema
  fechahorasys(&tm);
  
 // printf ("Entramos en fconexip\n");

//************** LEEMOS DE LA TABLA NPTDHSM ***********    
  retorno = tratar_fich("NPTDHSM", IGUAL, &reg_hsm, "0001");
  //printf ("retorno de tratar fichero %i\n",retorno);
  //fflush(stdout);
  //fflush(stderr);
  if(retorno)
  {
     printf ("HSM: error al leer NPTDHSM\n");
     return(-1);    
  }
  //printf ("retorno de tratar fichero %i\n",retorno);
  ////// PUERTO
  sprintf(cadport, "%.04s", reg_hsm.dhpuerto);  
  eftport=atoi(cadport);
    
  // recupera los valores de la cabecera y timeout
  strncpy(aux_time, reg_hsm.dhtiempo, 4);
  aux_time[4]='\0';
  timeout = atoi(aux_time);
  strncpy(cabecera, reg_hsm.dhcabece, 3);
  cabecera[3]='\0'; // la cabecera la ponemos de 3 "CAB"

  if (iHSM == 1)
  {      
    ///// IP PRINCIPAL
    //strncpy(cadname, "192.168.173.15", sizeof(reg_hsm.dhnumip));  
    strncpy(cadname, reg_hsm.dhnumip, sizeof(reg_hsm.dhnumip));    
    cadname[sizeof(reg_hsm.dhnumip)]='\0';
    printf("                IP PRINCIPAL \n");  
  }    
  else
  {
    if (iHSM == 2)
    {
       ////// IP SECUNDARIA
       strncpy(cadname, reg_hsm.dhnumip2, sizeof(reg_hsm.dhnumip2));    
       cadname[sizeof(reg_hsm.dhnumip2)]='\0';     
       printf("                IP SECUNDARIA \n");   
   }      
    else
    {
        printf ("HSM: informar num HSM\n");
          
    }
  }

  //conectar el socket, debe estar en la conexion para no hacerlo siempre
   valor = cliente(); //retorna el identificador del socket
   if (valor < 0)
  {
     ////// IP SECUNDARIA
     strncpy(cadname, reg_hsm.dhnumip2, sizeof(reg_hsm.dhnumip2));    
     cadname[sizeof(reg_hsm.dhnumip2)]='\0';
     printf("WARNING: Fallo hsm principal, cambiado a secundario %d:%d < ip>%.15s\n",tm.tm_hour,tm.tm_min,cadname);
     
     valor = cliente(); 
     if (valor < 0)
     { 
        printf("ERROR: Fallo total hsm %d:%d < ip1>%.15s\n",tm.tm_hour,tm.tm_min,cadname); 
            fflush(stdout);
    fflush(stderr);
     }
     else
     {
       printf("OK conexion correcta 2: %d:%d %s\n",tm.tm_hour,tm.tm_min,cadname);   
           fflush(stdout);
    fflush(stderr);
     }       
  }
  else
  {
    printf("OK conexion correcta 1: %d:%d %s\n",tm.tm_hour,tm.tm_min,cadname);     
    fflush(stdout);
    fflush(stderr);    
  }
   segundos =atoi(aux_time);
   printf("display sleep en segundos %d\n",segundos);
   sleep(segundos);
   
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Saliendo rutina fConexIP  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
  	printf(" ** ** Valor de salida valor %i\n", valor);
   
  return(valor);    
}

//PRJ0258542 - Fase 3 INI
//********************************************************************************
///FUNCION fConexHSM, conecta al hsm 1 principal o 2 secundario sin cambiar entre ellos
//********************************************************************************
int fConexHSM(char * ipcon, char * puerto, char * tiempo, char * cabece, int iHSM) 
{
	fechahorasys(&tm);
  printf(" ** ** ** Entrando rutina fConexHSM  20250117 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  long retorno=0;
  int valor=0;
  int i=0;
  char aux_time[5];
  char indPuertoOK[2];
  int segundos;
	char clave[4];
	char Entrada_dll[4];
	char Salida_dll[4];
	long longE = 0;
	int tipcla = 0;
  char ip_conex[15];
	char ip_tiem[4];
	char ip_puer[4];
	char ip_cabe[4];

  //NPSDHSM reg_hsm;
  //NPSINCI reginci;
  
  strncpy(ip_conex, ipcon, 15);
	strncpy(ip_tiem, tiempo, 4);
	strncpy(ip_puer, puerto, 4);
	strncpy(ip_cabe, cabece, 4);
	printf(" ** Parametros ip >%.15s< tiempo >%.4s< puerto >%.4s< cabecera >%.4s<\n", ip_conex, ip_tiem, ip_puer, ip_cabe);
  
  //coger fecha del sistema
  fechahorasys(&tm);
  
  ////// PUERTO
  sprintf(cadport, "%.04s", ip_puer);  
  eftport=atoi(cadport);
    
  // recupera los valores de la cabecera y timeout
  strncpy(aux_time, ip_tiem, 4);
  aux_time[4]='\0';
  timeout = atoi(aux_time);
  strncpy(cabecera, ip_cabe, 3);
  cabecera[3]='\0'; // la cabecera la ponemos de 3 "CAB"

 	if (iHSM == 1)
  {      
  	///// IP PRINCIPAL
    strncpy(cadname, ip_conex, sizeof(ip_conex));
    cadname[sizeof(ip_conex)]='\0';
    printf("  *****        IP PRINCIPAL >%s<\n",cadname);  
  }    
  else
  {
		if (iHSM == 2)
    {
       ////// IP SECUNDARIA
       strncpy(cadname, ip_conex, sizeof(ip_conex));
       cadname[sizeof(ip_conex)]='\0';
       printf("  *****        IP SECUNDARIA >%s<\n",cadname);   
  	}      
		else
   	{
   		printf ("HSM: informar num HSM\n");
   		return(-1);
    }
	}

  valor = cliente();
  if (valor < 0)
  {
  	printf("WARNING: Fallo HSM \n");
    return(-1);
	}
  else
  {
  	printf("OK conexion correcta >%i<\n", valor);   
	
  }       
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fConexHSM  20250117 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
   
  return(valor);
}

void conexhsm_1(char * ipcon, char * puerto, char * tiempo, char * cabece, struct sfdssl * sockssl, char * codResp, int numHSM)
{

	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina conexhsm_1  20250120 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf("** ** Datos conexion de entrada tiempo >%.4s< puerto >%.4s< ip>%.15s< cabecera>%.4s< NumHSM >%d<\n", tiempo, puerto, ipcon, cabece, numHSM);
	printf("Valor sockssl->sfd_ssl >%i<\n",sockssl->sfd_ssl);
	printf("**************************************************\n");  

	int retorno;
	NPSDHSM registro;
	const SSL_METHOD *method;
	int SSL_VERIFY_FAIL;

	if (sockssl->sfd_ssl > 0)
	{
		desconexhsm(sockssl->sfd_ssl);
	}

	retorno = fConexHSM(ipcon, puerto, tiempo, cabece, numHSM);
	if (retorno > 0)
  {
		sockssl->sfd_ssl=retorno;
		SSL_library_init();
		method = TLSv1_2_client_method();
		ctx = SSL_CTX_new(method);
		sockssl->ssl_ssl = SSL_new(ctx);


		if (sockssl->ssl_ssl == NULL)
		{
			fprintf(stderr, "SSL_new() failed\n");
			//printf("error retorno del conex 1 %d\n", retorno);
			strncpy(codResp,"02",2);
			codResp[2]='\0';
			exit(EXIT_FAILURE);
		}
		SSL_set_fd(sockssl->ssl_ssl, sockssl->sfd_ssl);  /* attach the socket descriptor */
		const int status = SSL_connect(sockssl->ssl_ssl);
		//printf ("status<%i>\n",status);  

		if (status != 1)
		{
			SSL_get_error(sockssl->ssl_ssl, status);
			ERR_print_errors_fp(stderr); //High probability this doesn't do anything
			fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
			//printf("error retorno del conex 2 %d\n", retorno);
			strncpy(codResp,"02",2);
			codResp[2]='\0';
			exit(EXIT_FAILURE);
		}

		strncpy(codResp,"01",2);
		codResp[2]='\0';
		//printf("SSL conectado\n");
        
  }
  else
  {
		//printf("error retorno del conex 3 %d\n", retorno);
		strncpy(codResp,"02",2);
		codResp[2]='\0';
  }
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina conexhsm_1  20250120 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

}


//PRJ0258542 - Fase 3 FIN

//********************************************************************************
///FUNCION fCambiaIP, Cambia las IPS del hsm del principal a secundaria y viceversa
//********************************************************************************
int fCambiaIP() 
{
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Entrando rutina fCambiaIP  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  int retorno=0;   
   
   retorno = tratar_fich ("HSMDRIP", REESCRIBIR, NULL, "0001");
   if(retorno)
     printf("HSM: error >%d< al modificar ip principal y secundaria\n", retorno);
   else
     printf("intercambiadas ips principal y secundaria\n");
 
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Saliendo rutina fCambiaIP  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
 
   return(retorno); 
    
}


//********************************************************************************
///FUNCION CLAVESHSM, que recupera las claves necesarias para el comando hsm y el subsistema
//********************************************************************************
int claveshsm(char * comando, char * subsistema, char * bin, char * clave1, char * clave2)

{
	//BIMSF-3123
  fechahorasys(&tm);
  printf(" ** ** ** Entrando rutina claveshsm  20241121 v2.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf(" ** Valores de entrada: comando >%s< subsistema >%s< bin >%.6s<\n", comando, subsistema, bin); 
	
  //in:  comando[2]: cifrar, descifrar,...
  //     subsistema[4]: 0000 interno = 30 + 02
  //     bin[6]: 000000
  //out: clave1 
  //     clave2 
  //return  0-ok, <0-error
  
  NPSIDEN regiden;
  char clave_iden[60];
  int retorno=0;
  char auxComando[50];
  char auxBin[7];
  

  /////// COMANDO //////////////////
	strncpy(auxComando, comando, 50);
	auxComando[50]='\0';

	////// CLAVE IDEN ////////
  strncpy(auxBin,bin,6);
	auxBin[6]='\0';
  strncpy(clave_iden, subsistema,4);   
  strncpy(clave_iden + 4, auxComando,54);
  strncpy(clave_iden + 54, auxBin, 6); 
  clave_iden[60]='\0';

 	retorno=tratar_fich("NPTIDEN",IGUAL,&regiden,clave_iden);
	if (retorno)
	{
		printf("HSM: Error >%d< en IGUAL Fichero IDEN\n", retorno);
  	return(-1); 
	}
	
 	strncpy(clave1,regiden.idclave1,sizeof(regiden.idclave1));
 	fEliminar_Blancos(clave1, sizeof(regiden.idclave1));
 	strncpy(clave2,regiden.idclave2, sizeof(regiden.idclave2));
 	fEliminar_Blancos(clave2, sizeof(regiden.idclave2));
 	
  //BIMSF-3123
  fechahorasys(&tm);
  printf(" ** ** ** Saliendo rutina claveshsm  20241121 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf(" ** Valores de salida: clave1 >%s< clave2 >%s< \n", clave1, clave2); //29122023 - BIMSF-3454
  return(0);      
}


//********************************************************************************
//* Progama que envia al servidor criptografico para que resuelva y responde.    *
//********************************************************************************
//FUNCION MANEJHSM
// PRJ0258542 - Se amplia un campo mas en la 3ª posición para poder meter el tipo de clave
//						- 00 = TDESsin PIN
//						- 03 = TDES
//						- 05 = AES
// Este valor es para trasladarlo a fcomponerMAC y fcomponerCMAC, ya que se debe saber que tipo de clave se le envía al HSM
//********************************************************************************
long manejhsm(char * clavehsm, char* comandohsm, int tipcla, char * datoshsm, long NumByte, struct sfdssl * sockssl, char * prespuestahsm) 
{
  //BIMSF-3123
  fechahorasys(&tm);
 	
	printf("**************************************************\n");  
 	printf("***   manejhsm.c version 10/24                 ***\n");  
 	printf("**************************************************\n");  
  printf(" ** ** ** Entrando rutina manejhsm  20241021 v2.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf("Valor clavehsm   >%s<\n",clavehsm);
	printf("Valor comandohsm >%*s<\n",strlen(comandohsm),comandohsm);
	printf("Valor tipcla     >%d<\n", tipcla);
	printf("Valor datoshsm   >%*s<\n",strlen(datoshsm),datoshsm);
	printf("Valor NumByte    >%i<\n",NumByte);
	printf("Valor sockssl->sfd_ssl >%i<\n",sockssl->sfd_ssl);
  printf("**************************************************\n");  
  //in : clave o claves concatenadas para utilizar en la preguntahsm
  //     comando en un codigo numerico de 2 posiciones
  //     datos para utilizar en el comando
  //     longitud de los datos
  //     socket
  //out: respuesta del hsm  
  // retorna negativo si es error, longitud de la respuesta si es ok, -145 = error comunicacion   

  long   retorno=0;
  int Num_devolver, err;
  int NumByte2 = 0;

  int i = 0;
  int suma_long = 0;
  char test_aux[25]; // Para componer el mensaje de test antes de enviar
  char pan[16];
  char clavedescifrado[50];
  char var_comand_PIN[10];
  char pinCorto[10];
  char ppreguntahsm[LONG_MSG]; 
  char cadHexa[LONG_MSG];
  char cadAscii[4096];
  char cadena[4096];
  char resultado[4096];
  int longCif=0;
  int NumByte_aux=0;
  int comprobacion=0;
 // char datosCif[17];
  char datosCif[LONG_MSG];
  int funcion=0; // Para identificar el comando que se usa
  char HhashMec[3];
  char Hstatus[3];
  char datoshsm_Aux[4096]; //BIMSF-3123 - antes de 3000, ahora 4096
  int fin_funcion;
  char *salida;
  const SSL_METHOD *method;
  struct hostent *host;
  int SSL_VERIFY_FAIL;
  const char *CADIR;
  char prespuestahsm2[LONG_MSG];
  char vector_aes[32];
  char claveiden[4]; //PRJ0258542 - Fase 3
	char cadenaGraba[4017]; //PRJ0258542 - Fase 3
	long tamano = 0; //PRJ0258542 - Fase 3

// Inicializacion de variables
   Num_devolver = 0;
         
   memset(test_aux,'\0',sizeof(test_aux));
   memset(pan,'\0',sizeof(pan));
   memset(clavedescifrado,'\0',sizeof(clavedescifrado));
   memset(pinCorto,'\0',sizeof(pinCorto));
   memset(var_comand_PIN, '\0', sizeof(var_comand_PIN));
   memset(ppreguntahsm, '\0', LONG_MSG);
   
   errno=0;
   retorno=0; 

  //si el identificador del socket no es mayor que 0 error de comunicacion
  if(sockssl->sfd_ssl < 0 || sockssl->sfd_ssl == 0) //PRJ0258542 - Fase 3 - Se anade valor 0 al IF
  {
     printf("HSM: id socket negativo o cero. Error de comunicacion\n");
     return(-145);
  }
  //datos obligatorios para todos los casos
  if(strlen(comandohsm)==0 || sockssl->sfd_ssl ==0)
  {
      printf("HSM: Faltan datos obligatorios\n");
      return(-2); 
  }


  // LLAMADA A LA RUTINA QUE IDENTIFICA EL COMANDO LLEGADO POR PARAMETRO 
  funcion=atoi(comandohsm);
  //BIMSF-3123
  printf(" ** ** Funcion devuelta por atoi %i\n",funcion);
 
  retorno = fIdentificaComando(funcion);
  if(retorno)
  { 
     return(retorno); //si no identifica comando retorna -2
  }
   
   if(NumByte <= 0)   
   {
      NumByte = LONG_MSG; //Si el parametro ha llegado <= 0, inicializa con 4096 (tamaño maximo)
   }
   datoshsm[NumByte]='\0';
   //printf("Valor datoshsm  2 >%*s<\n",NumByte,datoshsm); //PRJ0258542
   //memset(prespuestahsm, '\0', NumByte); //PRJ0258542
   //strncpy(prespuestahsm, " ", NumByte);
   //printf("Valor datoshsm  3 >%*s<\n",NumByte,datoshsm); //PRJ0258542

// Composicion del mensaje correspondiente para enviar al HSM
   switch(funcion)
   {
     case 0: //LISTADO 0301 
        strncpy(test_aux,sentencia,strlen(sentencia));
        suma_long = strlen(sentencia);
        sprintf(ppreguntahsm, "%06d", suma_long);
        strncpy(ppreguntahsm + 6,test_aux,suma_long);
        ppreguntahsm[suma_long + 6] = '\0';
        //printf("El mensaje completo  EN CASE 0 LISTADO es: >%*s<\n",strlen(ppreguntahsm),ppreguntahsm);
        break;

     case 1://TEST 1401 
        strncpy(test_aux,sentencia,strlen(sentencia));
        suma_long = strlen(sentencia);
        sprintf(ppreguntahsm, "%06d", suma_long);          
        strncpy(ppreguntahsm + 6,test_aux,suma_long);
        ppreguntahsm[suma_long + 6] = '\0';

        //printf("El mensaje completo es: >%*s<\n",strlen(ppreguntahsm),ppreguntahsm);
        break;
     
     case 2://cifrar PIN 611
      //datoshsm debera llevar: PAN(16) PIN(4)
      // se guarda el pan 
        
        strncpy (pan, datoshsm, 16);
        pan[16] = '\0'; // Cierre de cadena
      
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
        ppreguntahsm[strlen(datoshsm)]='\0';
        
        retorno = fcifrarPIN(ppreguntahsm,clavehsm,sentencia);

        break;

     case 3: //PIN EN CLARO 611
     case 10: //BLOQUE DE PIN EN ASCII 601
        //datoshsm debera llevar: PAN(16) OFFSET(4)
        //clavehsm tendra concatenado: clave de calculo de pin(15), clave de cifrado de pin lmk03(15), clave cifrado pin lmk07 (15)
        
        
        // se guarda el pan para utilizarlo en la segunda llamada si hay que obtener el pin en claro - comando 3
        strncpy (pan, datoshsm, 16);
        pan[16] = '\0'; // Cierre de cadena

        // se guarda para utilizarlo en la segunda llamada si hay que obtener el pin en claro - comando 3
        strncpy (clavedescifrado, clavehsm + 15 + 15, 15); //clave cifrado pin con lmk07
        clavedescifrado[15] = '\0';
        
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
        ppreguntahsm[strlen(datoshsm)]='\0';
        
        retorno = fcomponerPIN(ppreguntahsm,clavehsm,sentencia); 
        break;
        
     case 4: //MAC 801
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE PETICION DE MAC
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				retorno = fcomponerMAC(ppreguntahsm, sentencia, tipcla, clavehsm, NumByte);		        
        break;
         
     case 5: //ARPC 503
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE PETICION DE CALCULO DE ARPC
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';
        retorno = fcomponerCRIPTOGRAMA(ppreguntahsm, clavehsm, sentencia, GENERA_ARPC);
        break;
		
     case 6: //ARQC 503
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE PETICION DE VERIFICACION DE ARQC
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';
        retorno = fcomponerCRIPTOGRAMA(ppreguntahsm, clavehsm, sentencia, VERIF_ARQC);
        break;        
        
     case 7: //CMAC 802 - PRJ0258542 - CMAC //PRJ0258542
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE PETICION DE CMAC para intercambio claves
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				retorno = fcomponerCMAC(ppreguntahsm, sentencia, tipcla, clavehsm, NumByte);		        
        break;

     case 8: //Importar clave TR-31 206 - PRJ0258542 - //PRJ0258542 - llamada mejor al 206 interna
				//printf("Llega hasta la funcion 8\n");
				//return(-2);

        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE IMPORTACION DE CLAVE TR-31 para intercambio claves
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';
				//printf("1 Valor ppreguntahsm  >%s<\n", ppreguntahsm);
				//printf("1 Valor NumByte       >%d<\n", NumByte);				
				retorno = fcomponerImpor(ppreguntahsm, sentencia, "i", tipcla, clavehsm, NumByte);
				//printf("2 Valor ppreguntahsm  >%s<\n", ppreguntahsm);
				NumByte = strlen(ppreguntahsm);
				//printf("2 Valor NumByte       >%d<\n", NumByte);				
        break;
        
     case 9: //Calculo del KCV 404 - PRJ0258542 - //PRJ0258542
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE CALCULO DEL KCV DE LA CLAVE para intercambio claves
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				retorno = fcomponerKCV(ppreguntahsm, sentencia, NumByte);		        
        break;

     case 11: //Sacar Offset del bloque de PIN 618 - PRJ0258542 - //PRJ0258542
     		printf("Valor funcion = 11\n");
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE CALCULO DEL Offset desde el bloque de PIN
        //printf("Valor datoshsm  4 >%*s<\n",NumByte,datoshsm);
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				printf("Valor ppreguntahsm 1 >%*s<\n",strlen(ppreguntahsm),ppreguntahsm);
				retorno = fcomponerOFF(ppreguntahsm, sentencia);		        
        break;

     case 12: //Importar clave TR-31 206 - PRJ0258542 - //PRJ0258542 - llamada mejor al 206 externa
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE IMPORTACION DE CLAVE TR-31 para intercambio claves
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				retorno = fcomponerImpor(ppreguntahsm, sentencia, "e", tipcla, clavehsm, NumByte);		        
        break;
        
     case 14: //CIFRAR PAN 901
        //si la tarjeta es de mas de 16 caracteres se cifran los primeros 16 y se concatena con el resto.
        //si la tarjeta es de menos de 16 caracteres, se rellena con F para cifrarlo.        
        if(NumByte <= 16) // para tarjetas de hasta 16
        {
           strncpy(datosCif, datoshsm, NumByte+1);
           strncpy(datosCif + NumByte,"FFFFFFFFFFFFFFFF", (17-NumByte));
           longCif=17;
        }
        else
        {
           strncpy(datosCif, datoshsm, NumByte);
           if((NumByte-1) % 16 != 0) //tienen que ser bloques de 16
           {
              strncpy(datosCif + NumByte,"FFFFFFFFFFFFFFFF", (16*(((NumByte-1)/16) + 1)) - (NumByte-1));
               if (NumByte % 16 != 0)
               {    
                longCif=16*((NumByte/16) +1)+1;  
               }
			   else 
               {
			    longCif= NumByte+1; 
               }              
              comprobacion=(16*(((NumByte-1)/16) + 1)) - (NumByte-1);
           }
           else
           {
              longCif=NumByte;

           }
           datosCif[longCif]='\0';
        }

        retorno = fcifrar_cadena(CIFRADO,sockssl->ssl_ssl, datosCif, longCif,clavehsm, sockssl->sfd_ssl);
        
        if (retorno)
        {
           printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
           return(retorno);
        }
        else
        {
           memset(prespuestahsm, ' ', sizeof(prespuestahsm));
           strncpy(prespuestahsm, datosCif, longCif);
           prespuestahsm[longCif]= '\0';
           Num_devolver = longCif ;           
           //printf("Respuesta HSM manejhsm salida opcion 14:  >%s<\n", prespuestahsm);
        }
        break;

     case 15: //DESCIFRAR PAN 901
        //si la tarjeta es de mas de 16 caracteres se descifran los primeros 16 y se concatena con el resto.
        //si despues de descifrarlo tiene 'F' al final, se sustituyen por espacios pq eran de relleno.
      
            strncpy(datosCif, datoshsm, NumByte);
            longCif=NumByte;
            datosCif[longCif]='\0';                    
               
           retorno = fcifrar_cadena(DESCIFRADO,sockssl->ssl_ssl, datosCif, longCif,clavehsm,sockssl->sfd_ssl );

        if (retorno)
        {
           printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
           return(retorno);
        }
        else
        {
            memset(prespuestahsm, ' ', sizeof(prespuestahsm));

             for(i=(NumByte-2); i>0; i--) //si era una tarjeta de menos de 16, se relleno con FFF antes de cifrar
             {
                if(datosCif[i]=='F')
                {
                  datosCif[i] = ' '; //sustituyo por espacios
                }
                else
                { 
                   i=0;
                }                       
             }

           strncpy(prespuestahsm,datosCif, NumByte);
           prespuestahsm[NumByte]= '\0';           
           Num_devolver = NumByte;
        }
        break;
         
     case 17: //CIFRADO ASCII 901
        // datoshsm llegara en ascii y hay que convertirlo a hexadecimal para poder cifrarlo
        // ocupa el doble
       
        if(NumByte > LONG_MSG/2)
        {
           printf("HSM: No se pueden cifrar cadenas ascii de longitud superior a %d\n", LONG_MSG/2);
           return(-2);
        }
        if(NumByte <= 16) // para tarjetas de hasta 16
        {
           strncpy(datosCif, datoshsm, NumByte+1);
           strncpy(datosCif + NumByte,"FFFFFFFFFFFFFFFF", (17-NumByte));
           longCif=17;
        }
        else
        {
           strncpy(datosCif, datoshsm, NumByte);
           if((NumByte-1) % 16 != 0) //tienen que ser bloques de 16
           {
              strncpy(datosCif + NumByte,"FFFFFFFFFFFFFFFF", (16*(((NumByte-1)/16) + 1)) - (NumByte-1));
               if (NumByte % 16 != 0)
               {    
                longCif=16*((NumByte/16) +1)+1;  
               }
			   else 
               {
			    longCif= NumByte+1; 
               }              
              comprobacion=(16*(((NumByte-1)/16) + 1)) - (NumByte-1);
           }
           else
           {
              longCif=NumByte;

           }
           datosCif[longCif]='\0';
        }

        retorno = fcifrar_cadena(CIFRADO,sockssl->ssl_ssl, datosCif, longCif,clavehsm, sockssl->sfd_ssl );
        if (retorno)
        {
           printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
           return(retorno);
        }
        else
        {
           memset(prespuestahsm, ' ', sizeof(prespuestahsm));
           strncpy(prespuestahsm, datosCif, longCif);
           prespuestahsm[longCif]= '\0';
           Num_devolver = longCif ;           
         // printf("Respuesta HSM manejhsm salida opcion 17:  >%s<\n", prespuestahsm);
        }
        break;
        
     break;

     case 18: //DESCIFRADO ASCII 901
        // datoshsm llegara en ascii y hay que convertirlo a hexadecimal para poder descifrarlo
        // ocupara el doble
        if(NumByte > LONG_MSG/2)
        {
           printf("HSM: No se pueden descifrar cadenas ascii de longitud superior a %d\n", LONG_MSG/2);
           return(-2);
        }

        retorno = fcifrar_cadena(DESCIFRADO,sockssl->ssl_ssl, datoshsm, NumByte,clavehsm, sockssl->sfd_ssl );
        if (retorno)
        {
           printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
           return(retorno);
        }
        
        memset(prespuestahsm, ' ', sizeof(prespuestahsm));
        strncpy(prespuestahsm,datoshsm, NumByte);
        prespuestahsm[NumByte+1]= '\0';       
        Num_devolver = NumByte;
        break;        
        
     case 19: //CIFRADO DATOS HEXADECIMALES 901
        retorno = fcifrar_cadena(CIFRADO,sockssl->ssl_ssl, datoshsm, NumByte,clavehsm,sockssl->sfd_ssl );
        if (retorno)
        {
           printf("HSM: error retorno de fcifrar_cadena >%d<\n", retorno);
           return(retorno);           
        }
        else
        {
           memset(prespuestahsm, ' ', sizeof(prespuestahsm));
           strncpy(prespuestahsm,datoshsm, NumByte);
           prespuestahsm[NumByte]= '\0';
           Num_devolver = NumByte;
        }
        break;

     case 20: //DESCIFRADO DATOS HEXADECIMALES 901
        retorno = fcifrar_cadena(DESCIFRADO,sockssl->ssl_ssl, datoshsm, NumByte,clavehsm,sockssl->sfd_ssl );
        if (retorno)
        {
           printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
           return(retorno);
        }
        else
        {
           memset(prespuestahsm, ' ', sizeof(prespuestahsm));
           strncpy(prespuestahsm,datoshsm, NumByte);
           prespuestahsm[NumByte]= '\0';
           Num_devolver = NumByte;
        }
        
        break;
    
     case 24://BORRAR CLAVES, //PRJ0258542 - Se hace todo nuevo, comando 0102

        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				retorno = fcomponerBAJA(ppreguntahsm, sentencia, tipcla, clavehsm, NumByte);		        
        break;

     case 25://INSERTAR CLAVES, //PRJ0258542 - Se hace todo nuevo, comando 0101

        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				retorno = fcomponerALTA(ppreguntahsm, sentencia, tipcla);		        
        break;

     case 26://LANZA COMANDO, solo pruebas
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
        ppreguntahsm[strlen(datoshsm)] = '\0';
     
        break;
        
     case 28://LANZA COMANDO, funcion offset 604
     
   //     strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
   //     ppreguntahsm[strlen(datoshsm)]='\0';
        
         
        
      retorno = foffset(sockssl->ssl_ssl, datoshsm, NumByte,clavehsm,sockssl->sfd_ssl );
      
      if (retorno)
      {
         printf("HSM: error: retorno de offset >%d<\n", retorno);
         return(retorno);
      }
     
        memset(prespuestahsm, ' ', sizeof(prespuestahsm));
        strncpy(prespuestahsm,datoshsm, NumByte);
        prespuestahsm[NumByte+1]= '\0';       
        Num_devolver = NumByte;
        break;  
            
        break; 
        
     case 30://LANZA COMANDO, funcion HASH 2000

        strncpy(HhashMec, datoshsm, 2);
        HhashMec[2]='\0';
        strncpy(Hstatus, "01", 2);
        Hstatus[2]='\0';
        NumByte=NumByte-2;
        strncpy(datosCif, datoshsm+2, NumByte);
        datosCif[NumByte]='\0';
 
        if(NumByte > LONG_MSG/2)
        {
           printf("HSM: No se pueden cifrar cadenas ascii de longitud superior a %d\n", LONG_MSG/2);
           return(-2);
        }

        fAscii_Hexa (NumByte, datosCif, cadHexa); // utilizo una funcion para tener en cuenta los negativos
        //fprintf(stdout, "datos a hashear %s, %s, y tamaño %i\n", datosCif, cadHexa, NumByte);

        retorno = fHash(HhashMec,sockssl->ssl_ssl, cadHexa, NumByte*2,Hstatus, sockssl->sfd_ssl);
        if (retorno)
        {
           printf("HSM: error: retorno de hashear >%d<\n", retorno);
           return(retorno);
        }

        // hay que llamar una seguna vez
        strncpy(Hstatus, "03", 2);
        Hstatus[2]='\0';
        
        NumByte_aux = strlen(cadHexa);
        retorno = fHash(HhashMec,sockssl->ssl_ssl, cadHexa, NumByte_aux,Hstatus, sockssl->sfd_ssl);
        
        memset(prespuestahsm, ' ', sizeof(prespuestahsm));
        strncpy(prespuestahsm, cadHexa, strlen(cadHexa));

        NumByte=strlen(cadHexa);
        prespuestahsm[NumByte]= '\0';
        Num_devolver = NumByte;
        fprintf(stdout, "datos finales %s y %i\n", prespuestahsm, NumByte);

        break; 
        
     case 31://LANZA COMANDO, funcion RSA 1103
     
        strncpy(datosCif, datoshsm, NumByte);
        longCif=NumByte;
        datosCif[longCif]='\0';  
        retorno = fRsa(sockssl->ssl_ssl,datosCif,longCif,clavehsm, sockssl->sfd_ssl);
        fprintf(stdout, "datos salida RSA %s\n", datosCif); 
        if (retorno)
        {
           printf("HSM: error: retorno de RSA >%d<\n", retorno);
           return(retorno);
        }
        else
        {
           longCif=strlen(datosCif);
           salida=hex_2_base64(datosCif);
           memset(prespuestahsm, ' ', sizeof(prespuestahsm));
           longCif=strlen(salida);
           strncpy(prespuestahsm, salida, longCif);
           prespuestahsm[longCif]= '\0';
           Num_devolver = longCif ;   
        }
        fprintf(stdout, "datos finales %s y %i\n", prespuestahsm, longCif);
        break;

     case 33: //CIFRADO DATOS PSD2 903

			  //BIMSF-3123
			  printf(" ** ** Entrada en funcion case 33 \n");
        printf(" ** ** * Variable datoshsm antes >%s< y NumByte >%i< \n",datoshsm,NumByte);      
        strncpy(vector_aes,datoshsm, 32);
        //strncpy(datoshsm,datoshsm + 32, NumByte); //BIMSF-3123 - quitado 240215
        memset(datoshsm_Aux, '\0', NumByte);
        strncpy(datoshsm_Aux,datoshsm + 32, NumByte);
        //BIMSF-3123 -- Al quitar el vector, tenemos que restar 32 a NumByte
   	    NumByte = NumByte - 32;
        memset(datoshsm, '\0', NumByte);
        datoshsm[NumByte]='\0';
        strncpy(datoshsm, datoshsm_Aux, NumByte);
        //printf(" ** ** * Variable vector_aes >%s< \n",vector_aes);      
        //printf(" ** ** * Variable datoshsm despues >%s< \n",datoshsm);      
		
        // primero tenemos que utilizar el padding que nos solicitan para este cifrado
        //printf( "*** ** Entrada addPadding32 >%s< NumByte >%i< y NumByte2 >%i< \n", datoshsm, NumByte, NumByte2);
        NumByte2 = addPadding32(datoshsm,NumByte);
        //printf( "*** ** Salida addPadding32 >%s< NumByte >%i< y NumByte2 >%i< \n", datoshsm, NumByte, NumByte2);

        // datoshsm llegara en ascii y hay que convertirlo a hexadecimal para poder cifrarlo ocupa el doble
				if(NumByte2 > LONG_MSG/2)
				{
		   		printf("HSM: No se pueden cifrar cadenas ascii de longitud superior a %d\n", LONG_MSG/2);
		   		return(-2);
				}
			
				fAscii_Hexa (NumByte2, datoshsm, cadHexa); // utilizo una funcion para tener en cuenta los negativos
                
	  		retorno = fcifrar_cadena_aes(CIFRADO,sockssl->ssl_ssl, cadHexa, NumByte2*2,clavehsm,sockssl->sfd_ssl,vector_aes);//con paddin
     		//retorno = fcifrar_cadena_aes(CIFRADO,sockssl->ssl_ssl, datoshsm, NumByte,clavehsm, sockssl->sfd_ssl,vector_aes);//original

				if (retorno)
        {
		   		printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
		   		return(retorno);
        }
				else
				{
        	memset(prespuestahsm, ' ', sizeof(prespuestahsm));
		   		//para binario *4
          strncpy(prespuestahsm,cadHexa, NumByte2*2);
          Num_devolver = NumByte2*2;
	    	}
        break; 

     case 34: //DESCIFRADO DATOS PSD2 903
        
			  //BIMSF-3123
				printf(" ** ** Entrada en funcion case 34 \n");
     	 	printf(" ** ** * Variable datoshsm antes >%s< y NumByte >%i< \n",datoshsm,NumByte);      
        strncpy(vector_aes,datoshsm, 32);
        memset(datoshsm_Aux, '\0', NumByte);
        strncpy(datoshsm_Aux,datoshsm + 32, NumByte);
   	    NumByte = NumByte - 32;
        memset(datoshsm, '\0', NumByte);
        datoshsm[NumByte]='\0';
        strncpy(datoshsm, datoshsm_Aux, NumByte);
        //printf(" ** ** * Variable vector_aes >%s< \n",vector_aes);      
        //printf(" ** ** * Variable datoshsm despues >%s< \n",datoshsm);      
		
        //printf( "*** ** Entrada addPadding32 >%s< NumByte >%i< y NumByte2 >%i< \n", datoshsm, NumByte, NumByte2);
        NumByte2 = addPadding32(datoshsm,NumByte);
        //printf( "*** ** Salida addPadding32 >%s< NumByte >%i< y NumByte2 >%i< \n", datoshsm, NumByte, NumByte2);
        
        retorno = fcifrar_cadena_aes(DESCIFRADO,sockssl->ssl_ssl, datoshsm, NumByte,clavehsm, sockssl->sfd_ssl,vector_aes);
			  if (retorno)
        {
		   		printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
		   		return(retorno);
        }
		
	    	//printf("cadHexa cifrado %d>%.*s<\n",NumByte*2,NumByte*2, cadHexa);
        
				//de hexadecimal lo pasamos a ascii
				retorno=fHexa_ascii (datoshsm, cadAscii);
				//printf("Retorno y cadena cadAscii: %i, >%s<\n", retorno, cadAscii);
				memset(prespuestahsm, ' ', sizeof(prespuestahsm));
				strncpy(prespuestahsm,cadAscii,retorno);
				prespuestahsm[retorno+1]='\0';
				//strncpy(prespuestahsm, datoshsm, NumByte/2);
				Num_devolver = strlen(cadAscii);
				//Num_devolver = NumByte/2;
       
        break; 
        
     case 35: //Exportar clave TR-31 304 - //PRJ0258542
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE EXPORTACION DE CLAVE TR-31 para consulta de KCV
        strncpy(ppreguntahsm, clavehsm, strlen(clavehsm));
				ppreguntahsm[strlen(clavehsm)]='\0';		
				retorno = fcomponerClaveTR(ppreguntahsm, sentencia, tipcla, datoshsm, NumByte);		        
        break;
        
      case 36: //Traslacion de PIN 0603 - //PRJ0258542
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE TRASLACION DE PIN para moverlo de formato ISO4 a ISO2
        strncpy(ppreguntahsm, datoshsm, strlen(datoshsm));
				ppreguntahsm[strlen(datoshsm)]='\0';		
				retorno = fcompone_0603(ppreguntahsm, clavehsm, sentencia);		        
        break;
        
      case 37: //Exportar PIN 0605 - //PRJ0258542
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE EXPORTACION DE CLAVE TR-31 para consulta de KCV
        strncpy(ppreguntahsm, clavehsm, strlen(clavehsm));
				ppreguntahsm[strlen(clavehsm)]='\0';		
				retorno = fcomponerClaveTR(ppreguntahsm, sentencia, tipcla, datoshsm, NumByte);		        
        break;
        
      case 38: //Calcula bloque de PIN 0611 - //PRJ0258542
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE EXPORTACION DE CLAVE TR-31 para consulta de KCV
        strncpy(ppreguntahsm, clavehsm, strlen(clavehsm));
				ppreguntahsm[strlen(clavehsm)]='\0';		
				retorno = fcomponerClaveTR(ppreguntahsm, sentencia, tipcla, datoshsm, NumByte);		        
        break;
        
      case 39: //Generacion PIN 0619 - //PRJ0258542
        //LLAMADA A LA RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE EXPORTACION DE CLAVE TR-31 para consulta de KCV
        strncpy(ppreguntahsm, clavehsm, strlen(clavehsm));
				ppreguntahsm[strlen(clavehsm)]='\0';		
				retorno = fcomponerClaveTR(ppreguntahsm, sentencia, tipcla, datoshsm, NumByte);		        
        break;
        
     default:
        printf( "HSM: error en composicion del comando de peticion \n");
        return(-1);// Error generico 
   }
   /////////////////////////////////////////////////////////////////////////////////
   //------------ ENVIO DE LOS MENSAJES SALVO PARA CIFRAR QUE LO HACE YA SU FUNCION
   ////////////////////////////////////////////////////////////////////////////////
     
   if(funcion !=14 && funcion !=15 && funcion !=17 && funcion !=18 && funcion !=19 && funcion !=20 && funcion != 28 && funcion != 30 && funcion != 31 && funcion !=33 && funcion !=34)
  // if(funcion !=14 && funcion !=15 && funcion !=17 && funcion !=18 && funcion !=19 && funcion !=20  && funcion != 30 && funcion != 31 && funcion !=33 && funcion !=34)

   { 
       //-------ENVIAR MENSAJE 
      //printf("envimaos el mensaje  %s\n", ppreguntahsm);
			if (funcion == 8 || funcion == 24) //PRJ0258542 - Fase 3
			{
				printf("Se tiene que grabar registro\n");
				strncpy(claveiden, prespuestahsm, 4);
				strncpy(cadenaGraba, claveiden, 4);
				strncpy(cadenaGraba + 4, "0000000000", 10);
				strncpy(cadenaGraba + 14, "I", 1);
				strncpy(cadenaGraba + 15, "NE", 2);
				strncpy(prespuestahsm, "    ", 4);
				
				//strncpy(cadenaGraba + 17, ppreguntahsm, strlen(ppreguntahsm));
				strncpy(cadenaGraba + 17, ppreguntahsm, NumByte);
				//tamano = tamano + strlen(ppreguntahsm);
				tamano = NumByte + 17;
				retorno = tratar_fich1("NPTRHSM", ESCRIBIR, cadenaGraba, tamano);
			}
      
      retorno=enviar_mensajes_hsm(sockssl->ssl_ssl, ppreguntahsm, strlen(ppreguntahsm));
      if(retorno <=0)
      { 
         err=SSL_shutdown(sockssl->ssl_ssl);
         SSL_free(sockssl->ssl_ssl); 
         //printf("HSM: retorno de funcion enviar_mensajes_hsm >%d<\n", retorno);
         return(retorno);
      } 
         
      //---------LEER RESPUESTA
      Num_devolver = leer_linea(sockssl->ssl_ssl, timeout, prespuestahsm, LONG_MSG, sockssl->sfd_ssl);   
      if(Num_devolver <=0)
      {
      	if (funcion == 24 && Num_devolver != 0) //PRJ0258542 - Fase 3
      	{
         	printf("HSM: COMANDO INCORRECTO\n");
         	printf("HSM: COMANDO >%.13s<\n", ppreguntahsm);
				}
      }
          
    }
   
   ///////////////////////////////////////////////////////////////////////////// 
   /////////////////// TRATAMIENTO POSTERIOR A LA RESPUESTA
   /////////////////////////////////////////////////////////////////////////////
   switch(funcion)
   {
   case 3: // obtenido el bloque de pin, obtengo el pin en claro
     
     if (Num_devolver > 0) 
     {
        retorno = fObtenerPIN(sockssl->ssl_ssl, pan ,clavedescifrado ,prespuestahsm,pinCorto, sockssl->sfd_ssl);
       
        if(retorno)
        {
           printf("HSM: ERROR EN EL CALCULO DE PIN >%d<\n",retorno);
           return(retorno);
        }
        else
        {
           strncpy(prespuestahsm, pinCorto, strlen(pinCorto));
           prespuestahsm[strlen(pinCorto)] = '\0'; // Cierre de cadena
           Num_devolver = strlen(prespuestahsm);
           
        }
     }
   break;
 
   
  }
        
   /////////////////// TERMINA PARA TODOS ////////////////////////////////////////////
   
 // Rellena el parametro de vuelta a la aplicacion origen con los datos que espera cada parte
    //BIMSF-3123
  fechahorasys(&tm);
  printf(" ** ** ** Saliendo rutina manejhsm  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf(" ** ** Valor de salida: prespuestahsm >%*s<\n",strlen(prespuestahsm),prespuestahsm);
 
	return(Num_devolver);     
} //FIN manejhsm
  


/*********************************************************/
/* FUNCION: cliente.c                                    */
/* Solo crea el socket y conecta                         */
/*********************************************************/
int cliente()
{
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Entrando rutina cliente  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  int retorno, err;
  int sfd;
  int arg;
  int queda;
  struct hostent *host;
  

// PARTE PARA ABRIR LA CONEXION

  if ( (host = gethostbyname("localhost")) == NULL )
  {
      perror("localhost"); 
      abort();
  }


     sfd = socket(PF_INET, SOCK_STREAM, 0);  
    ser_addr.sin_port=htons((short)eftport); 
    ser_addr.sin_family=PF_INET; 
    ser_addr.sin_addr.s_addr = inet_addr(cadname);
    signal(SIGALRM, finalConex);

    alarm(1); //espera un segundos maximo para conectar

    printf ("Entramos a conectar con servidor funcion connect() con sfd [%d]\n",sfd);
    printf ("Entramos a conectar con servidor funcion connect() con ip [%s]\n",cadname);
    printf ("Entramos a conectar con servidor funcion connect() con longitud [%d]\n",sizeof(ser_addr));
    
    fechahorasys(&tm);
    printf("Antes connect   - %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
    retorno = connect(sfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    fechahorasys(&tm);
    printf("Despues connect - %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
    
    printf ("volvemos de retorno >%d<\n",retorno);
    queda = alarm(0);
    
    printf("Tiempo restante >%d<\n",queda);
    
    /////////// si se ha alcanzado el time out forzamos el error del connect (aunque no haria falta)
        ////////// pq el connect devuelve -1 si no consigue terminar.
    if(queda==0)
      retorno=-1;
  
    //printf("retorno >%d<\n",retorno);
  
    if (retorno != 0 )
    {
      close(sfd);
      return(-1);

    }

    //BIMSF-3123
    //fechahorasys(&tm);
    //printf(" ** ** ** Saliendo rutina cliente  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   return(sfd);
}


void finalConex()
{
  // printf("HSM: superado timeout conexion\n");
   //return;
  // exit(0); //en vez del return;
}


/* ******************************** */
/* FUNCION:  enviar_mensajes_hsm    */
/* ******************************** */
int enviar_mensajes_hsm(SSL *ssl, char * pmsg_env, int NumByte)
{
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Entrando rutina enviar_mensajes_hsm  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
		printf(" ** ** -- Cadena de envio a HSM >%s< de NumByte >%i<\n", pmsg_env, NumByte);
   	char auxLong[7];
   
   strncpy(auxLong,pmsg_env,6);
   auxLong[6]='\0';
   if(atoi(auxLong) != strlen(pmsg_env)- 6)
   {
      printf("HSM: Mensaje a enviar:>%s<\n",pmsg_env);
      printf("HSM: ERROR EN LONGITUD DEL MENSAJE %d\n", strlen(pmsg_env)-6);
      return(-1);
   }
   //printf("escribimos %s y %i\n", pmsg_env, strlen(pmsg_env)); 
   //if (send(sfd,pmsg_env, NumByte, 0) < 0) //retorna el num byte enviados

  if (SSL_write(ssl, pmsg_env, strlen(pmsg_env)) < 0)
   {   
      printf("HSM: Mensaje a enviar:>%s<\n",pmsg_env);    
      printf("HSM: ERROR EN send//%d\n",errno);
      if(errno == 32 || errno == 145 || errno == 9)
      {
         printf("HSM: error de conexion\n");
         return(-145);
      }
      else
      {
         return(-1);
      }
   }
  
    //BIMSF-3123
    fechahorasys(&tm);
    printf(" ** ** ** Saliendo rutina enviar_mensajes_hsm  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   return(1);
}

/* ************************************    */
/* FUNCIOn: leer_linea                    */
/* ************************************    */
int leer_linea (SSL *ssl, int timeout, char * pmsg_respu, int longitud, int sd)
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina leer_linea  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	int fin=0;
	int retorno=0;
	char comando[5];
	char resp_aux[LONG_MSG];
	char valor_retor[8];

	char long_respuesta[6];
	int total_respuesta = 0;
	int longitud_clave = 0;

// nnicializacion de variables
  memset(comando,'\0',sizeof(comando));
  memset(pmsg_respu,'\0',sizeof(pmsg_respu));
  memset(resp_aux,'\0',sizeof(resp_aux));
  memset(long_respuesta,'\0',sizeof(long_respuesta));
  memset(valor_retor,'\0',sizeof(valor_retor));
 
 
   retorno=recv_to(ssl,resp_aux,timeout, sd);  
  // printf("retonro %i y datos %s\n", retorno, resp_aux);

 	if (retorno<=0)
  {
  	printf("HSM: error en rcv errno>%d< retorno>%d<\n", errno, retorno);
    retorno = -145;
 	}
  else
  {
		// Se extrae primero la longitud del mensaje de respuesta
		strncpy(long_respuesta,resp_aux,6);                   //6 - longitud
		long_respuesta[6] = '\0';
		total_respuesta = retorno - 6;
		strncpy(comando,resp_aux + 6 + strlen(cabecera),4);  //4 - comando         
		// printf("comando %s\n", comando); 
		switch (atoi(comando))
    {
    	case 101: //Alta de claves
            
				//BIMSF-3123
				printf(" ** ** Valor comando 101\n");
            
				strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
        valor_retor[8] = '\0';
        if (strcmp(valor_retor,"00000000")== 0)   
        {       
        	strncpy(pmsg_respu,resp_aux+18+ strlen(cabecera) ,total_respuesta - 12 -strlen(cabecera)); //8 -resultado
          pmsg_respu[total_respuesta - 12 -strlen(cabecera)] = '\0'; // Cierre de cadena.
          break;
        }
        else
        {
        	printf("HSM: ERROR EN EL ALTA >%s<\n", valor_retor);
          return(-1);          
       	}
    	case 103: //LISTADO  NO SE USA EN INVERSIS Y HABRIA QUE HACER UN PROGRAMA PARA LANZARLO
            
				//BIMSF-3123
				printf(" ** ** Valor comando 103\n");
            
				strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
        valor_retor[8] = '\0';
        if (strcmp(valor_retor,"00000000")== 0)   
        {       
        	strncpy(pmsg_respu,resp_aux+18+ strlen(cabecera) ,total_respuesta - 12 -strlen(cabecera)); //8 -resultado
          pmsg_respu[total_respuesta - 12 -strlen(cabecera)] = '\0'; // Cierre de cadena.
          break;
        }
        else
        {
        	printf("HSM: ERROR EN LISTADO >%s<\n", valor_retor);
          return(-1);          
       	}
    	case 202: //IMPORTAR CLAVES  NO SE USA EN INVERSIS Y HABRIA QUE HACER UN PROGRAMA PARA LANZARLO
				//BIMSF-3123
				printf(" ** ** Valor comando 202\n");
            
				printf("resultado >%.8s<\n", resp_aux + 6 + strlen(cabecera) + 4);
				strncpy(pmsg_respu,resp_aux+18+ strlen(cabecera) ,total_respuesta - 12 -strlen(cabecera)); //8 -resultado
				pmsg_respu[total_respuesta - 12 -strlen(cabecera)] = '\0'; // Cierre de cadena.        
				break; 

     	case 102: //BORRAR CLAVES 
				
				printf(" ** ** Valor comando 102\n");
        printf(" ** ** total_respuesta >%s<\n",total_respuesta);
				strncpy(pmsg_respu,resp_aux+18+ strlen(cabecera) ,total_respuesta - 12 -strlen(cabecera)); //8 -resultado
				pmsg_respu[total_respuesta - 12 -strlen(cabecera)] = '\0'; // Cierre de cadena.
				printf(" ** ** pmsg_respu >%s<\n",pmsg_respu);
				break; 
       
     	case 1401:  //TEST  NO SE USA EN INVERSIS Y HABRIA QUE HACER UN PROGRAMA PARA LANZARLO
				//BIMSF-3123
				printf(" ** ** Valor comando 1401\n");
            
				strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
				valor_retor[8] = '\0';

				if (strcmp(valor_retor,"00000000")== 0)
				{       
					//Se copia desde el valor de retorno en adelante para quitar la cabecera
					// printf("dentro 1401\n");
					strncpy(pmsg_respu,resp_aux+ 10 + strlen(cabecera),total_respuesta - strlen(cabecera) - 4);
					pmsg_respu[total_respuesta - strlen(cabecera) - 4] = '\0'; // Cierre de cadena.
			   	// printf("dentro %s y salid: %s y %i\n", resp_aux, pmsg_respu, total_respuesta);
					strncpy(pmsg_respu,resp_aux+12+ strlen(cabecera) ,total_respuesta - 12 -strlen(cabecera));
					pmsg_respu[total_respuesta - 12 -strlen(cabecera)] = '\0'; // Cierre de cadena.
			   	// printf("dentro %s y salid: %s y %i\n", resp_aux, pmsg_respu, total_respuesta);
					break;
				}
				else
				{
					printf("HSM: ERROR EN TEST >%s<\n", valor_retor);
					return(-1);                    
				}

   		case 618:	//OFFSET SE USA EN SISMA para Bloque de PIN //PRJ0258542
				printf(" ** ** Valor comando 618\n");
            
			  // Se copia desde el valor de retorno en adelante para quitar la cabecera
			  strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
			  valor_retor[8] = '\0';

			  if (strcmp(valor_retor,"00000000")== 0)
			 	{
					strncpy(pmsg_respu,resp_aux+10 +strlen(cabecera)+8,total_respuesta - 12-strlen(cabecera));
					pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
			 	}
			 	else 
			 	{
					printf("HSM: ERROR EN CALCULO OFFSET >%s<\n", valor_retor);
					return(-1);
			 	}
			 	break;

   		case 801:	//MAC SE USA EN SISMA
				//BIMSF-3123
				printf(" ** ** Valor comando 801\n");
            
			  // Se copia desde el valor de retorno en adelante para quitar la cabecera
			  strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
			  valor_retor[8] = '\0';

			  if (strcmp(valor_retor,"00000000")== 0)
			 	{
					strncpy(pmsg_respu,resp_aux+10 +strlen(cabecera)+8,total_respuesta - 12-strlen(cabecera));
					pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
			 	}
			 	else 
			 	{
					printf("HSM: ERROR EN CALCULO MAC >%s<\n", valor_retor);
					return(-1);
			 	}
			  //printf("\npmsg_respu >%s<\n",pmsg_respu,strlen(pmsg_respu));				
			 	break;

   		case 802:	//CMAC SE USA EN SISMA para MAC en AES //PRJ0258542
				printf(" ** ** Valor comando 802\n");
            
			  // Se copia desde el valor de retorno en adelante para quitar la cabecera
			  strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
			  valor_retor[8] = '\0';

			  if (strcmp(valor_retor,"00000000")== 0)
			 	{
					strncpy(pmsg_respu,resp_aux+10 +strlen(cabecera)+8,total_respuesta - 12-strlen(cabecera));
					pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
			 	}
			 	else 
			 	{
					printf("HSM: ERROR EN CALCULO CMAC >%s<\n", valor_retor);
					return(-1);
			 	}
			 	break;

   		case 206:	//Importar Claves a TR-31 //PRJ0258542 - mejor que usar el 205, pero hace lo mismo
				printf(" ** ** Valor comando 206\n");
            
			  // Se copia desde el valor de retorno en adelante para quitar la cabecera
			  strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
			  valor_retor[8] = '\0';

			  if (strcmp(valor_retor,"00000000") == 0)
			 	{
					strncpy(pmsg_respu,resp_aux+10 +strlen(cabecera)+8,total_respuesta - 12-strlen(cabecera));
					pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
			 	}
			 	else 
			 	{
					printf("HSM: ERROR AL IMPORTAR CLAVE >%s<\n", valor_retor);
					if (strcmp(valor_retor, "00020000") == 0)
					{
						return(-2);
					}
					else
					{
						return(-1);
					}
			 	}
			 	break;

   		case 304:	//Exportar Claves a TR-31 //PRJ0258542
				printf(" ** ** Valor comando 304\n");
            
			  // Se copia desde el valor de retorno en adelante para quitar la cabecera
			  strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
			  valor_retor[8] = '\0';

			  if (strcmp(valor_retor,"00000000")== 0)
			 	{
					strncpy(pmsg_respu,resp_aux+10 +strlen(cabecera)+8,total_respuesta - 12-strlen(cabecera));
					pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
			 	}
			 	else 
			 	{
					printf("HSM: ERROR AL EXPORTAR CLAVE >%s<\n", valor_retor);
					return(-1);
			 	}
			 	break;

   		case 404:	//Calculo KCV para clave AES //PRJ0258542
				printf(" ** ** Valor comando 404\n");
            
			  // Se copia desde el valor de retorno en adelante para quitar la cabecera
			  strncpy(valor_retor,resp_aux+10 + strlen(cabecera),8);
			  valor_retor[8] = '\0';

			  if (strcmp(valor_retor,"00000000")== 0)
			 	{
					strncpy(pmsg_respu,resp_aux+10 +strlen(cabecera)+8,total_respuesta - 12-strlen(cabecera));
					pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
			 	}
			 	else 
			 	{
					printf("HSM: ERROR EN CALCULO DE KCV >%s<\n", valor_retor);
					return(-1);
			 	}
			 	break;

    	case 503:	//ARPC  NO SE USA EN INVERSIS PERO ESTA POR SI SE NECESITA YA DESARROLLADO
			//BIMSF-3123
					printf(" ** ** Valor comando 503\n");
            
          //Se copia desde el valor de retorno en adelante para quitar la cabecera
          strncpy(valor_retor,resp_aux+10+strlen(cabecera),8);
          valor_retor[8] = '\0';

          if (strcmp(valor_retor,"00000000")== 0)
          {
            strncpy(pmsg_respu,resp_aux+10+strlen(cabecera)+8,total_respuesta - 12-strlen(cabecera));
            pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
          }
          else 
          {
            printf("HSM: ERROR EN CALCULO ARPC >%s<\n", valor_retor);
            return(-1);
          }
          //printf("\npmsg_respu >%s<\n",pmsg_respu,strlen(pmsg_respu));				
          break;
          
		  // SE HIZO EL DESARROLLO PERO NO SE PUEDE PROBAR PORQUE HABRIA QUE ACTUALIZAR LOS SERVIDORES DE INVERSIS Y
		  // DECIDIERON NO HACERLO, SE DEJA EL DESARROLLO PARA FUTURO
         case 601:    //PIN Y PIN CIFRADO (por ahora)
         case 611:    //CIFRAR PIN
       //Se copia desde el valor de retorno en adelante para quitar la cabecera
         
         strncpy(valor_retor,resp_aux+10+strlen(cabecera),8);
         valor_retor[8] = '\0';

         if (strcmp(valor_retor,"00000000")== 0)
         {
           strncpy(pmsg_respu,resp_aux+10+strlen(cabecera)+8,total_respuesta - 12 -strlen(cabecera));
           pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
         }
         else 
         {
            printf("HSM: ERROR EN CALCULO PIN >%s<\n", valor_retor);
            return(-1);
         }
         printf("\npmsg_respu 601 >%s<\n",pmsg_respu,strlen(pmsg_respu));				

         break;
         
	  // SE HIZO EL DESARROLLO PERO NO SE PUEDE PROBAR PORQUE HABRIA QUE ACTUALIZAR LOS SERVIDORES DE INVERSIS Y
	  // DECIDIERON NO HACERLO, SE DEJA EL DESARROLLO PARA FUTURO
       case 604: //OFFSET
    //     strncpy(pmsg_respu, resp_aux, total_respuesta +6); //6 de la long que va delante
    //     pmsg_respu[total_respuesta+6] = '\0'; // Cierre de cadena.        
    //     break;  
    
   //Se copia desde el valor de retorno en adelante para quitar la cabecera
			//BIMSF-3123
			printf(" ** ** Valor comando 604\n");
            

         strncpy(valor_retor,resp_aux+10+strlen(cabecera),8);
         valor_retor[8] = '\0';
    
         if (strcmp(valor_retor,"00000000")== 0)
         {
           strncpy(pmsg_respu,resp_aux+10+strlen(cabecera)+8,total_respuesta - 12 -strlen(cabecera));
           pmsg_respu[total_respuesta - 12-strlen(cabecera)] = '\0'; // Cierre de cadena.
         }
         else 
         {
            printf("HSM: ERROR EN CALCULO PIN >%s<\n", valor_retor);
            return(-1);
         }
         printf("\npmsg_respu 604>%s<\n",pmsg_respu,strlen(pmsg_respu));				

         break;
         
       case 901: //CIFRADO DESCIFRADO
 			//BIMSF-3123
			printf(" ** ** Valor comando 901\n");
            
          strncpy(pmsg_respu, resp_aux, total_respuesta +6); //6 de la long que va delante
          pmsg_respu[total_respuesta+6] = '\0'; // Cierre de cadena.        
          break; 
 
       case 903: //CIFRADO DESCIFRADO  PSD2
			//BIMSF-3123
			printf(" ** ** Valor comando 903\n");
            
          strncpy(pmsg_respu, resp_aux, total_respuesta +6); //6 de la long que va delante
          pmsg_respu[total_respuesta+6] = '\0'; // Cierre de cadena.	    
          break;  

       case 2000: //HASH
			//BIMSF-3123
			printf(" ** ** Valor comando 2000\n");
            
          strncpy(pmsg_respu, resp_aux, total_respuesta +6); //6 de la long que va delante
          pmsg_respu[total_respuesta+6] = '\0'; // Cierre de cadena.
          break;
          
       case 1103: //RSA
 			//BIMSF-3123
			printf(" ** ** Valor comando 1103\n");
            
         strncpy(pmsg_respu, resp_aux, total_respuesta +6); //6 de la long que va delante
          pmsg_respu[total_respuesta+6] = '\0'; // Cierre de cadena.
          break;
          
       default:
          printf("mensaje de respuesta: %s\n", resp_aux);
          printf("HSM: ERROR COMANDO RECEPCION NO ESPERADO\n");
          return(-1); 
     }

     retorno = strlen(pmsg_respu);
     //printf("lon: %i\n", retorno);
   }

	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina leer_linea  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   return(retorno);
}



//*****************************************************************************************//
// RUTINA QUE IDENTIFICA EL COMANDO LLEGADO POR PARAMETRO
//       El parametro recibido lo convierte en la cabecera del comando a enviar    
//*****************************************************************************************//
int fIdentificaComando (int iFuncion)
{
	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fIdentificaComando  20241021 v2.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   int i = 0; // Contador
   int len=0;
   char cadcomando_B[20];
   char chComando[5];

   // ponemos la cabecera en la sentencia   
   strncpy(sentencia, cabecera, strlen(cabecera));
   
   switch(iFuncion)
   {
   case 0: //LISTADO 0301
      strncpy(chComando,LISTADO, strlen(LISTADO));
      strncpy(chComando+strlen(LISTADO),"S",1);
      chComando[strlen(LISTADO)+1]='\0';      
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';             
      break;
   case 1: //TEST 1401
      strncpy(chComando,TEST, strlen(TEST));
      chComando[strlen(TEST)]='\0';      
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';             
      break;
   case 2: //CIFRAR PIN 911
      strncpy(chComando,CIFRAR_PIN, strlen(CIFRAR_PIN));
      chComando[strlen(CIFRAR_PIN)]='\0';      
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';                   
      break;
   case 3: //CALCULA_PIN enclaro 0601   
   case 10://PIN_CIFRADO 0601
      strncpy(chComando,CALCULA_PIN, strlen(CALCULA_PIN));
      chComando[strlen(CALCULA_PIN)]='\0';      
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';                   
      break;
   case 4: //CALCULA_MAC 0801   
      strncpy(chComando,CALCULA_MAC, strlen(CALCULA_MAC));
	  	chComando[strlen(CALCULA_MAC)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;
      
   case 5: //CALCULA_ARPC 0503  
   case 6: //VERIFICA_ARQC  
      strncpy(chComando,CRIPTOGRAMA, strlen(CRIPTOGRAMA));
	  	chComando[strlen(CRIPTOGRAMA)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  	  
      break;

   case 7: //CALCULA_CMAC 0802 //PRJ0258542
      strncpy(chComando,CALCULA_CMAC, strlen(CALCULA_CMAC));
	  	chComando[strlen(CALCULA_CMAC)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;

   case 8: //IMPORTAR_TR31 0206 //PRJ0258542 - para internas
   case 12: //IMPORTAR_TR31 0206 //PRJ0258542 - para externas
      strncpy(chComando,IMPORTAR_TR31, strlen(IMPORTAR_TR31));
	  	chComando[strlen(IMPORTAR_TR31)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;

   case 9: //CALCULA_KCV 0404 //PRJ0258542
      strncpy(chComando,CALCULA_KCV, strlen(CALCULA_KCV));
	  	chComando[strlen(CALCULA_KCV)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;

   case 11: //CALCULA_OFF 0618 //PRJ0258542
      strncpy(chComando,CALCULA_OFF, strlen(CALCULA_OFF));
	  	chComando[strlen(CALCULA_OFF)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;

   //case 11: //PINTAR_CLAVES   //PRJ0258542 - comentado no se usa
   //    strncpy(sentencia, "PINTAR_CLAVES", 13);
   //    sentencia[13]='\0';
   //   break;
   case 14: //CIFRA PAN    0901
   case 15: //DESCIFRA PAN 0901
   case 17: //CIFRA DATO EN ASCII 0901
   case 18: //DESCIFRA DATO EN ASCII 0901
   case 19: //CIFRA PAN    0901
   case 20: //DESCIFRA PAN 0901
      strncpy(chComando,DESCIFRA_DATO, strlen(DESCIFRA_DATO));
      chComando[strlen(DESCIFRA_DATO)]='\0';      
      break;
   case 16: //IMPORTAR_CLAVE 0608   
      strncpy(chComando,IMPORTACION, strlen(IMPORTACION));
      chComando[strlen(IMPORTACION)]='\0';      
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';                   
      break;      
      
   case 24: //BAJA_CLAVE 0102  //PRJ0258542
      strncpy(chComando,BAJA_CLAVE, strlen(BAJA_CLAVE));
	  	chComando[strlen(BAJA_CLAVE)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;

   case 25: //ALTA_CLAVE 0101  //PRJ0258542
      strncpy(chComando,ALTA_CLAVE, strlen(ALTA_CLAVE));
	  	chComando[strlen(ALTA_CLAVE)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;

   case 26: //EJECUTA_COMANDO ------------------------- NO DESARROLLADO EN INVERSIS NO SE USA
      sentencia[0] = '\0';
      break;
   case 28://LANZA COMANDO, funcion offset 604
      strncpy(chComando,CALCULA_OFFSET, strlen(CALCULA_OFFSET));
 	  chComando[strlen(CALCULA_OFFSET)]='\0';
      //anadido RAQUEL
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';      
      break;
    case 30: //COMANDO HASH 2000
      strncpy(chComando,HASH_DATO, strlen(HASH_DATO));
      chComando[strlen(HASH_DATO)]='\0';
      break;
    case 31: // RSA 1103
      strncpy(chComando,RSA_DATO, strlen(RSA_DATO));
      chComando[strlen(RSA_DATO)]='\0';
      break;   
    case 33: //CIFRA CADENA PSD2  0903
    case 34: //DESCIFRA CADENA PSD2  0903 
       strncpy(chComando,DESCIFRA_DATO_AES, strlen(DESCIFRA_DATO_AES));
 	  	 chComando[strlen(DESCIFRA_DATO_AES)]='\0';	  
       break;      
   case 35: //EXPORTAR-TR31 0304 //PRJ0258542
      strncpy(chComando,EXPORTAR_TR31, strlen(EXPORTAR_TR31));
	  	chComando[strlen(EXPORTAR_TR31)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;
   case 36: //Traslacion de PIN 0603 //PRJ0258542
      strncpy(chComando,TRANSLATE_PIN, strlen(TRANSLATE_PIN));
	  	chComando[strlen(TRANSLATE_PIN)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;
   case 37: //Exportar PIN 0605 //PRJ0258542
      strncpy(chComando,EXPORT_PIN, strlen(EXPORT_PIN));
	  	chComando[strlen(EXPORT_PIN)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;
   case 38: //Calcula bloque de PIN 0611 //PRJ0258542
      strncpy(chComando,CIFRAR_PIN, strlen(CIFRAR_PIN));
	  	chComando[strlen(CIFRAR_PIN)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;
   case 39: //Generacion PIN 0619 //PRJ0258542
      strncpy(chComando,GENERA_PIN, strlen(GENERA_PIN));
	  	chComando[strlen(GENERA_PIN)]='\0';	  
      strncpy(sentencia + strlen(cabecera), chComando, strlen(chComando));
      sentencia[strlen(cabecera) + strlen(chComando)] = '\0';       	  
      break;


   default:
       printf("HSM: error en comando HSM\n");
       return(-2);   
    }
 	//BIMSF-3123
	fechahorasys(&tm);
	//printf(" ** ** ** Saliendo rutina fIdentificaComando  20241021 v2.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  return(0);
}



//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE PETICION DE PIN               //    
//*****************************************************************************************//
int fcomponerPIN(char * cad_datos, char * cad_claves, char * cab_comando)
{
 	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerPIN  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

//cad_datos tendra: pan(16) offset(4)
//cad_claves tendra: clavecalculo(15) clavecifrado(15)
 int suma_long_mensaje = 0;
 char long_total[6];

 char clave_cal_pin[50];
 char clave_cif_pin[50];

 char mensaje_completo[1024];

    //Inicializacion de las variables
   memset(clave_cal_pin,'\0',sizeof(clave_cal_pin));
   memset(clave_cif_pin,'\0',sizeof(clave_cif_pin));
   memset(mensaje_completo,'\0',sizeof(mensaje_completo));

   // Se guardan los identificadores para componer las claves
   strncpy(clave_cal_pin, cad_claves, 21);
   clave_cal_pin[21]='\0';
   strncpy(clave_cif_pin, cad_claves+21, 21);
   clave_cif_pin[21]='\0';
   

   strncpy(mensaje_completo, cab_comando,strlen(cab_comando));
   suma_long_mensaje = suma_long_mensaje + strlen(cab_comando);

   strncpy(mensaje_completo + suma_long_mensaje,clave_cal_pin, strlen(clave_cal_pin)); // Clave calculo pin
   suma_long_mensaje = suma_long_mensaje + strlen(clave_cal_pin);

   strncpy(mensaje_completo + suma_long_mensaje,clave_cif_pin, strlen(clave_cif_pin)); // clave cifrado pin
   suma_long_mensaje = suma_long_mensaje + strlen(clave_cif_pin);

   strncpy(mensaje_completo + suma_long_mensaje,tabla_decimal,16); //  tabla decimal constante
   suma_long_mensaje = suma_long_mensaje + 16;

   strncpy(mensaje_completo + suma_long_mensaje,"0",1); // formato bloque de pin 
   suma_long_mensaje = suma_long_mensaje + 1;

   strncpy(mensaje_completo + suma_long_mensaje,"1",1); // algoritmo generacion pin
   suma_long_mensaje = suma_long_mensaje + 1;

   //strncpy(mensaje_completo + suma_long_mensaje,"16",2); // longitud pan 
   strncpy(mensaje_completo + suma_long_mensaje,cad_datos,2); //longitud pan
   suma_long_mensaje = suma_long_mensaje + 2;

  // strncpy(mensaje_completo + suma_long_mensaje, cad_datos, 16); // pan
   strncpy(mensaje_completo + suma_long_mensaje, cad_datos+2, 16); //pan
   suma_long_mensaje = suma_long_mensaje + 16;
   
  // strncpy(mensaje_completo + suma_long_mensaje,"4",1);  // longitud offset
   strncpy(mensaje_completo + suma_long_mensaje,cad_datos+18,1);  // longitud offset
   suma_long_mensaje = suma_long_mensaje + 1;

  // strncpy(mensaje_completo + suma_long_mensaje, cad_datos + 16, 4);  //offset
   strncpy(mensaje_completo + suma_long_mensaje, cad_datos + 19, 4);  //offset 
   suma_long_mensaje = suma_long_mensaje + 4;

   mensaje_completo[suma_long_mensaje] = '\0'; // Cierre de la cadena

   sprintf(long_total, "%06d", suma_long_mensaje); 
   long_total[6] = '\0';
  
   strncpy(cad_datos,long_total,6);
   strncpy(cad_datos + 6, mensaje_completo,suma_long_mensaje);
   cad_datos[suma_long_mensaje + 6] = '\0';
   
 	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerPIN  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  
   return(0);
} 

//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE CIFRAR DE PIN               //    
// //*****************************************************************************************//
int fcifrarPIN(char * cad_cadena, char * cad_claves, char * cab_comando)
{
 	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcifrarPIN  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  int suma_long_mensaje = 0; 
  char long_total[6];
  char NPIN[4];

  char clave_cif_pin[50];
  char mensaje_completo[1024];
  char pin_binario[1024];
  char pin_hex[1024];

   //Inicializacion de las variables
  memset(clave_cif_pin,'\0',sizeof(clave_cif_pin));
  memset(mensaje_completo,'\0',sizeof(mensaje_completo));
  
  // Se guardan los identificadores para componer las claves
   // strncpy(clave_cif_pin, cad_claves, 21); //esto seria si usaramos un registro independiente en bbdd para esta clave pero usamos el mismo 601,611,604
   // clave_cif_pin[21]='\0';
    
   strncpy(clave_cif_pin, cad_claves, 14); 
   clave_cif_pin[14]='\0'; 
            
 // strncpy (NPIN, cad_cadena+16,4);
  strncpy (NPIN, cad_cadena+21,4);
  NPIN[4] = '\0'; // Cierre de cadena
  

  strncpy(mensaje_completo, cab_comando,strlen(cab_comando));
  suma_long_mensaje = suma_long_mensaje + strlen(cab_comando);
  

  strncpy(mensaje_completo + suma_long_mensaje,clave_cif_pin, strlen(clave_cif_pin)); // clave cifrado pin
  suma_long_mensaje = suma_long_mensaje + strlen(clave_cif_pin);
  
   
  strncpy(mensaje_completo + suma_long_mensaje,"0", 1); // clave cifrado pin
  suma_long_mensaje = suma_long_mensaje + 1;

  //strncpy(mensaje_completo + suma_long_mensaje,"2",1); // formato bloque de pin
  strncpy(mensaje_completo + suma_long_mensaje,cad_cadena,1); // formato bloque de pin
  suma_long_mensaje = suma_long_mensaje + 1;

  //strncpy(mensaje_completo + suma_long_mensaje,"16",2); // longitud pan
  strncpy(mensaje_completo + suma_long_mensaje,cad_cadena+1,2); // longitud pan
  suma_long_mensaje = suma_long_mensaje + 2;
  
  //strncpy(mensaje_completo + suma_long_mensaje, cad_cadena, 16); // pan
  strncpy(mensaje_completo + suma_long_mensaje, cad_cadena+3, 16); // pan
  suma_long_mensaje = suma_long_mensaje + 16;
  
      
  //strncpy(mensaje_completo + suma_long_mensaje,"0",1);  // presencia del PIN
  strncpy(mensaje_completo + suma_long_mensaje,cad_cadena+19,1);  // presencia del PIN
  suma_long_mensaje = suma_long_mensaje + 1;
  
 // strncpy(mensaje_completo + suma_long_mensaje, "8", 1);  //pin
  strncpy(mensaje_completo + suma_long_mensaje, cad_cadena+20, 1);  //longitud pin
  suma_long_mensaje = suma_long_mensaje + 1;
    
 // RAQUEL vamos a pasar el pin a hexadecimal antes de añadirlo al comando por que con el pin en claro da error HSM: ERROR EN CALCULO PIN >00011C00<   
 //   //strncpy(mensaje_completo + suma_long_mensaje, "8", 1);  //pin
  strncpy(mensaje_completo + suma_long_mensaje,cad_cadena+21, 4);  //pin
  suma_long_mensaje = suma_long_mensaje + 4;

  
  printf("mensaje %s\n", mensaje_completo);

  mensaje_completo[suma_long_mensaje] = '\0'; // Cierre de la cadena

  sprintf(long_total, "%06d", suma_long_mensaje); 
  long_total[6] = '\0';
 
  strncpy(cad_cadena,long_total,6);
  strncpy(cad_cadena+ 6, mensaje_completo,suma_long_mensaje);
  cad_cadena[suma_long_mensaje + 6] = '\0';
  
 	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcifrarPIN  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);


   return(0);
}

/* ************************************************************************* */
/* Funcion que elimina los espacios en blanco, colocando el fin de cadena    */
/*  hay que conocer el tamaño maximo de la cadena y maximo sera 4096         */ 
/*************************************************************************** */
void fEliminar_Blancos (char * cadena_entrada, long long_entrada)
{

  	//BIMSF-3123
	fechahorasys(&tm);
	//printf(" ** ** ** Entrando rutina fEliminar_Blancos  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   long tam_entrada = 0;
    char cadena_aux[LONG_MSG];

  memset(cadena_aux, '\0', sizeof(cadena_aux));

  while (cadena_entrada[tam_entrada]!=' ' && tam_entrada < long_entrada)
  {
     sprintf(cadena_aux + tam_entrada, "%c", cadena_entrada[tam_entrada]); 
     tam_entrada++;
  }
  strncpy(cadena_entrada, cadena_aux, tam_entrada);
  cadena_entrada[tam_entrada] = '\0';
  	//BIMSF-3123
	fechahorasys(&tm);
	//printf(" ** ** ** Saliendo rutina fEliminar_Blancos  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

}

/* ************************************************************************* */
/* Funcion que recibe el bloque de PIN cifrado para descifrarlo usando otro  */
/* comando del HSM (0901).                         */
/* Despues lo transforma en un pin 'en claro'                 */
/* ************************************************************************* */
int fObtenerPIN(SSL *ssl,char * cadena_pan, char * clavedescifrado, char * bloquepin, char * pinclaro, int sfd)
{
  	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fObtenerPIN  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	
 char mensaje[1024];
 char clave_descif[200];
 char cad_aux[1024];
 char octeto[10];
 char octetoBinario[10];
 char pinNatural [10];
 char suma [10]; // Para ir guardando los digitos resultantes de la suma por separado del offset + pin natural
 char caracter_natural[2];

 char mascara[50];
 char result_XOR[100];
 char cad_pin_binario[500]; 
 char cad_masc_binario[500]; 
 char result_binario[100];

 int s = 0;// Para ir sumando los digitos por separado del offset + pin natural
 int fin_funcion = 0;
 unsigned int p = 0; // Contador para el bucle que carga los octetos
 int retorno =0;

// Inicializacion de varibles.
  memset(mensaje,'\0',sizeof(mensaje));
  memset(clave_descif,'\0',sizeof(clave_descif));
  memset(cad_aux,'\0',sizeof(cad_aux));
  memset(octeto,'\0',sizeof(octeto));
  memset(octetoBinario,'\0',sizeof(octetoBinario));
  memset(pinNatural,'\0',sizeof(pinNatural));
  memset(suma,'\0',sizeof(suma));
  memset(caracter_natural,'\0',sizeof(caracter_natural));
  memset(mascara,'\0',sizeof(mascara));
  memset(result_XOR,'\0', sizeof(result_XOR));
  memset(result_binario,'\0', sizeof(result_binario));
  memset(cad_pin_binario,'\0', sizeof(cad_pin_binario));
  memset(cad_masc_binario,'\0', sizeof(cad_masc_binario));
   

   strncpy(cad_aux, bloquepin, 16);
   cad_aux[16] = '\0'; 
   
   retorno = fcifrar_cadena(DESCIFRADO,ssl, cad_aux, 16, clavedescifrado,sfd );
 
   if(retorno <0)
   {
      printf("HSM: error: retorno de fcifrar_cadena >%d<\n", retorno);
      return(retorno);     
   }

//Construccion de la mascara con la que se hace el XOR
   strcpy(mascara,"0000"); //Relleno con 0 por la izquierda para formar la mascara con el PAN
   strncpy (mascara + 4, cadena_pan + 3, 12);
   mascara[16] = '\0'; // Cierre de la cadena.


//Conversion a binario de los dos bloques para la operacion
//RAQUEL REVISAR ESTO PARA EL PIN CUANDO SE HAGA
 //  fin_funcion = fHexa_binario(cad_aux,cad_pin_binario);
 //  if (!fin_funcion)
 //     fin_funcion = fHexa_binario(mascara,cad_masc_binario);
 //  else
 //     return (-1);
 //
// Operacion de XOR: es una funcion porque el tamaño de los operandos en la funcion de C++
//      no permite su uso (deben ser enteros)
   fOrExclusivo(cad_masc_binario,cad_pin_binario,result_binario);


//Reconversion del bloque resultante a hexadecimal
   fbinario_Hexa(result_binario, result_XOR);


// Bloque reutilizado para extraer los primeros caracteres que son los que forman el pin en claro
   strncpy(pinNatural,result_XOR + 2, 4);
   pinNatural[4] = '\0';


   for(p = 0; pinNatural[p]; p++)
   {
     s=hex_to_int(pinNatural[p]);
     // Se trata de conseguir un digito
     if (s > 9)
       s = s - 10;

     sprintf(suma,"%d",s);
     strncpy(pinclaro + p, suma, strlen(suma));
   }

   pinclaro[p] = '\0';

  	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fObtenerPIN  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	
   return(0);

}


/* ************************************************************************* */
/* Funcion que recibe un caracter hexadecimal y devuelve su valor decimal    */
/* ************************************************************************* */
int hex_to_int(char c)
{
  int numhex;
   switch(c)
   {
    case'0':numhex=0;break;
    case'1':numhex=1;break;
    case'2':numhex=2;break;
    case'3':numhex=3;break;
    case'4':numhex=4;break;
    case'5':numhex=5;break;
    case'6':numhex=6;break;
    case'7':numhex=7;break;
    case'8':numhex=8;break;
    case'9':numhex=9;break;
    case'A':case'a':numhex=10;break;
    case'B':case'b':numhex=11;break;
    case'C':case'c':numhex=12;break;
    case'D':case'd':numhex=13;break;
    case'E':case'e':numhex=14;break;
    case'F':case'f':numhex=15;break;
    }
    return numhex;
}

/* ************************************************************************* */
/* Funcion que recibe una cadena en binario (ceros y unos), y la devuelve en */
/* formato hexadecimal                                                       */
/* ************************************************************************* */
void  fbinario_Hexa(char * cadena_binaria, char * cadena_hex)
    {
        unsigned int b= 0; // Contador para recorrer la cadena binaria
        unsigned int h= 0; // Contador para recorrer la cadena hexadecimal
        char cuatro[10];

        memset(cuatro, '\0', sizeof(cuatro));

        while(b < strlen(cadena_binaria))
            {
            strncpy(cuatro, cadena_binaria + b, 4);
            cuatro[4] = '\0'; // Cierre de cadena

            switch (atoi(cuatro))
                {
                case 0:
                    strncpy(cadena_hex + h,"0",1);
                    break;
                case 1:
                    strncpy(cadena_hex + h,"1",1);
                    break;
                case 10:
                    strncpy(cadena_hex + h,"2",1);
                    break;
                case 11:
                    strncpy(cadena_hex + h,"3",1);
                    break;
                case 100:
                    strncpy(cadena_hex + h,"4",1);
                    break;
                case 101:
                    strncpy(cadena_hex + h,"5",1);
                    break;
                case 110:
                    strncpy(cadena_hex + h,"6",1);
                    break;
                case 111:
                    strncpy(cadena_hex + h,"7",1);
                    break;
                case 1000:
                    strncpy(cadena_hex + h,"8",1);
                    break;
                case 1001:
                    strncpy(cadena_hex + h,"9",1);
                    break;
                case 1010:
                    strncpy(cadena_hex + h,"A",1);
                    break;
                case 1011:
                    strncpy(cadena_hex + h,"B",1);
                    break;
                case 1100:
                    strncpy(cadena_hex + h,"C",1);
                    break;
                case 1101:
                    strncpy(cadena_hex + h,"D",1);
                    break;
                case 1110:
                    strncpy(cadena_hex + h,"E",1);
                    break;
                case 1111:
                    strncpy(cadena_hex + h,"F",1);
                    break;
                }

                b = b + 4;
                h++;
                }
        cadena_hex[h] = '\0'; //cierre de cadena. 
    }

/* ************************************************************************* */
/* Funcion que hace la conversion de hexadecimal a binario.Recibe los        */
/* parametros:                                                               */
/*            cadena: string que solo puede contener valor del 0..9 y A..F   */
/*            long_cadena: 4 posiciones de tipo caractes (ej: "0040")        */
/*            resultado: los caracteres binarios concatenados                */
/* ************************************************************************* */

//int fHexa_binario (char * cadena, char * resultado)
//void fHexa_binario (char * cadena, char * resultado) //Jira BIMSF-3123 - vamos a pasar la longitud
void fHexa_binario (char * cadena, char * long_cadena, char * resultado)
{

 int c = 0; // Contador para recorrer cadena
 int s;
 int resto, cociente;
 int varA;
 char auxD[4];
 int longitud = 0;
 
  //printf("Prueba para ver version correcta 000 \n"); //JFL
  printf("Valor de longitud antes >%.4s< \n",long_cadena); //Jira BIMSF-3123
  //longitud=strlen(cadena); //Jira BIMSF-3123 -- quito y uso lo que llega, solo las 4 primeras posiciones
  //strncpy(long_cadena,long_cadena,4);   JFL
  //printf("Valor de longitud despues >%.4s< \n",long_cadena); //Jira BIMSF-3123
  long_cadena[4]='\0';
  printf("Valor de longitud despues >%s< \n",long_cadena); //Jira BIMSF-3123
  longitud=atoi(long_cadena);

    // while (cadena[c]!=' ' && c < strlen(cadena)) // Este comentario estaba antes de quitar lo de 512 
	// while (cadena[c]!=' ' && c < 512) //Jira BIMSF-3123 - se cambia a longitud
	printf("Cadena de entrada >%s<, de longitud de entrada >%i< \n",cadena,longitud); //Jira BIMSF-3123
	while (cadena[c]!=' ' && c < longitud)
    {
	 
     varA=3;
     strncpy(auxD,"0000",4);
     //recuperamos el valor del digito
     s=hex_to_int(cadena[c]);
     while (varA > 0)
     {
       resto=s%2;
       cociente=s/2;
       if (resto == 0)
         strncpy(auxD+varA,"0",1);
       else
          strncpy(auxD+varA,"1",1);
       s=cociente;
       varA=varA-1;
     }
     if (s == 0)  
       strncpy(auxD+varA,"0",1);
     else
       strncpy(auxD+varA,"1",1);
 
     strncpy(resultado + c * 4, auxD, 4); 
     c++;
   }
   resultado[c * 4] = '\0';
   
   //return(0);
    longitud=strlen(resultado); //Jira BIMSF-3123
	printf(" ** ** -- Cadena de salida >%s<, de longitud de salida >%i< \n",resultado,longitud); //Jira BIMSF-3123
    return;
}

/* ************************************************************************* */
/* Funcion que realiza el XOR de dos cadenas que recibe por como parametros     */
/*        - cadena1 y cadena2 son las usadas como operandos     */
/*        - resultado la cadena resultante de la conbinacion de las anteriores */
/* ************************************************************************* */
void fOrExclusivo (char * cadena1, char * cadena2, char * resultado)
{
 unsigned int k = 0; // Usado como contador para recorrido del bucle
 char car_cadena1[2]; 
 char car_cadena2[2]; 
 char car_resultado[5];
 int car_comparado = 0;

     memset(car_cadena1, '\0', sizeof(car_cadena1));
     memset(car_cadena2, '\0', sizeof(car_cadena2));
     memset(car_resultado, '\0', sizeof(car_resultado));

     for (k = 0; k < strlen(cadena1); k++)
     {
         strncpy(car_cadena1,cadena1 + k, 1);
         strncpy(car_cadena2,cadena2 + k, 1);
         car_cadena1[1] = '\0';
         car_cadena2[1] = '\0';
         car_comparado = atoi(car_cadena1)^atoi(car_cadena2);
         sprintf(car_resultado,"%d",car_comparado);
         strncpy(resultado + k, car_resultado, 1);
     }
}

//*****************************************************************************************//
// RUTINA QUE HACE EL PADING                                        					   //	
//*****************************************************************************************//    
   int addPadding(char * in, long long_dat) 
   {
//       int offset = 0M;
        int extra = 8 - (long_dat % 8);
        int newLength = long_dat + extra;

        char caden_sal[newLength];
       
        memset(caden_sal, '\0', sizeof(caden_sal));
        memcpy(caden_sal, in, long_dat);
        caden_sal[long_dat] = 0x80;
        long_dat++;
        
      
        while (long_dat < newLength)
       {
            caden_sal[long_dat] = 0x00;
            long_dat++;
        }
        memcpy (in, caden_sal, long_dat);
         
     return newLength;     
    }
    
//*****************************************************************************************//
// RUTINA QUE QUITA  EL PADING                                        					   //	
//*****************************************************************************************//  
  int SubPadding(char* in, long long_dat)
  {
    int i=long_dat-1;
    
     
    while (i != 0)
    {
     if (in[i] == '\x80' ) //Simbolo de euro €
        return i;
     else
       i--;
    }
   
        return i;
  }
  
 //*****************************************************************************************//
// RUTINA QUE HACE EL PADING de (16 ascii) 32 posiciones en hexadecimal          			   //	
//*****************************************************************************************//    
   int addPadding32(char * in, long long_dat) 
   {

    long newLength = 0; // JASM cambio de int a long 04/12/2023
		long longReci = long_dat; // JASM 04/12/2023
        //int extra = 32 - (long_dat % 32); // JASM 04/12/2023
		int extra = 32 - (longReci % 32);
		//printf(" * * Entra la cadena in >%s< 1 * * \n", in);
    printf( " ** Dentro de addPadding32 long_dat >%i< extra >%i< newLength >%i< longReci >%i< 1 ** \n", long_dat, extra, newLength, longReci);    
        /*
		if ((long_dat % 32) != 0) 
        {
           newLength = long_dat + extra;
        }
        else
        {
           newLength = long_dat;
        }
		*/ // JASM 04/12/2023
		if ((longReci % 32) != 0) 
        {
           newLength = longReci + extra;
        }
        else
        {
           newLength = longReci;
        }
        char caden_sal[newLength];
        
        /* JASM 04/12/2023
		memset(caden_sal, '\0', sizeof(caden_sal));
        memcpy(caden_sal, in, long_dat);
        caden_sal[long_dat] = 0x80;
		long_dat++;

        while (long_dat < newLength)
        {
            caden_sal[long_dat] = 0x00;
            long_dat++;
        }
		*/
		
        if ((longReci % 32) != 0) 
        {
			memset(caden_sal, '\0', sizeof(caden_sal));
			//printf(" * * Procesa la cadena caden_sal >%s< 1 * * \n", caden_sal);
			memcpy(caden_sal, in, longReci);
			//printf(" * * Procesa la cadena caden_sal >%s< 2 * * \n", caden_sal);
			caden_sal[longReci] = 0x80;
			//printf(" * * Procesa la cadena caden_sal >%s< 4 * * \n", caden_sal);
			printf( " ** Dentro de addPadding32 long_dat >%i< extra >%i< newLength >%i< longReci >%i< 3 ** \n", long_dat, extra, newLength, longReci);    
			longReci++;
			printf( " ** Dentro de addPadding32 long_dat >%i< extra >%i< newLength >%i< longReci >%i< 4 ** \n", long_dat, extra, newLength, longReci);    

			while (longReci < newLength)
			{
				caden_sal[longReci] = 0x00;
				longReci++;
			}
			printf( " ** Dentro de addPadding32 long_dat >%i< extra >%i< newLength >%i< longReci >%i< 5 ** \n", long_dat, extra, newLength, longReci);    
			//printf(" * * Procesa la cadena caden_sal >%s< 5 * * \n", caden_sal);
		}
		else
		{
			memcpy(caden_sal, in, longReci);
			//printf(" * * Procesa la cadena caden_sal >%s< 6 * * \n", caden_sal);
		}

    memcpy (in, caden_sal, newLength);
		//printf(" * * Sale la cadena in >%s< 1 * * \n", in);
      
        return newLength;  
    }

/****************************************************************************
 *  NOMBRE:      fcifrar_cadena        
 *  PARAMETROS:  socket, datos a cifrar, longitud de los datos, clave
 *		cod_CifDescif = CIFRADO / DESCIFRADO
 *		SSL = estructura de datos para las instrucciones al HSM
 *		datos_cif = entrada de datos a procesar, tanto para cifrar como para descifrar
 *		long_cif = longitud de los datos que se quieren procesar
 *		clave_crip = la clave que se va a usar para el cifrado
 *		sfd = cadena de salida cifrada o descifrada, lo contrario de lo que entro
 *  DESCRIPCION: compone y envia al hsm comando de cifrado con identif.
 *                 de la clave, devolviendo el
 *                 resultado en entrada.                
 *                                     
 ****************************************************************************/
int fcifrar_cadena(char * cod_CifDescif,SSL *ssl, char * datos_cif, long long_cif, char * clave_crip, int sfd)
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcifrar_cadena  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	
	char cadena_aux[LONG_MSG];
   char comando[LONG_MSG]; //contiene el comando completo para enviar al hsm
   char cadena_respuesta[LONG_MSG];
   int longitud = 0;
   char ch_long_aux[7];
   int longitud_aux = 0;
   int retorno = 0;   
   int n_sock = 0; //Alberto.

   memset(cadena_aux, '\0', sizeof(cadena_aux));
   memset(ch_long_aux, '\0', sizeof(ch_long_aux));
   memset(cadena_respuesta, '\0', sizeof(cadena_respuesta));
     
   //validacion datos obligatorios - si no pasa la validacion error -2
   if (long_cif == 0)
   {
      printf("HSM: longitud 0\n");
      return(-2);   
   }
   if(strlen(datos_cif) ==0)
   {
      printf("HSM: faltan datos\n");
      return(-2);
   }
   if(strlen(clave_crip) ==0)
   {
      printf("HSM: falta clave\n");
      return(-2);
   }    

   // Componer mensaje hsm     
   strncpy(cadena_aux, cabecera, strlen(cabecera));
   longitud = longitud + strlen(cabecera);
   strncpy(cadena_aux + longitud, DESCIFRA_DATO, strlen(DESCIFRA_DATO)); //0901     
   longitud = longitud + strlen(DESCIFRA_DATO); 
   strncpy(cadena_aux + longitud, clave_crip, strlen(clave_crip));
   longitud = longitud + strlen(clave_crip);
   strncpy(cadena_aux + longitud, cod_CifDescif, strlen(cod_CifDescif));
   longitud = longitud + strlen(cod_CifDescif); //1:Cifrar 0:Descifrar//
   //SGL  no pude estae a pelo, el campo debe viajar en el mismo sitio que el texto a cifrar, en el primero caracter
   //strncpy(cadena_aux + longitud, "0", 1); //  0:CBC --> no cambia
   strncpy(cadena_aux + longitud, datos_cif,1);
   longitud = longitud + 1; 
   sprintf(ch_long_aux, "%06d", long_cif-1);     //SGL deberia ser longitud-1
   strncpy(cadena_aux + longitud, ch_long_aux, strlen(ch_long_aux));
   longitud = longitud + strlen(ch_long_aux);
   strncpy(cadena_aux + longitud, datos_cif+1, long_cif-1); //SGL tener en cuenta que en la primera posicion va el algoritmo
   longitud = longitud + long_cif-1;
   cadena_aux[longitud] = '\0'; // Cierre de cadena.

   longitud_aux = strlen(cadena_aux);

   sprintf(ch_long_aux, "%06d", longitud_aux);
   strncpy(comando, ch_long_aux, strlen(ch_long_aux));
   longitud = longitud + strlen(ch_long_aux);
   strncpy(comando + strlen(ch_long_aux), cadena_aux, strlen(cadena_aux));
   comando[longitud] = '\0'; //Cierre de la cadena que tiene el comando hsm.
    
   
   // Envio del mensaje - Llamada a funcion. 
   retorno = enviar_mensajes_hsm(ssl, comando, strlen(comando));
   if(retorno <=0)
   {    
         printf("HSM: ERROR EN send CIFRADO %d\n",errno);
         return(retorno);     
   }
  // printf("recibimos.....\n");
   retorno=recv_to(ssl,cadena_respuesta,timeout, sfd); //devuelve los bytes leidos

   if(retorno<=0)
   {    
      printf("HSM: ERROR EN recv CIFRADO %d\n",errno);
      return(-145);
   }
   //printf("HSM: salida  >%s<\n",cadena_respuesta); 
   cadena_respuesta[retorno]='\0';
   //printf("cadena_respuesta en cifrado %s\n",cadena_respuesta );
   longitud = 0; // se inicializa de nuevo
   memset(datos_cif, '\0', long_cif);
   longitud = strlen(ch_long_aux) + strlen(cabecera) +strlen(DESCIFRA_DATO);
   //printf("longitud en cifrado %i" , longitud);

   if (strncmp(cadena_respuesta+longitud,"00000000", 8)== 0)
   {
      longitud = longitud + 8 + strlen(ch_long_aux);
      strncpy(datos_cif, cadena_respuesta + longitud, strlen(cadena_respuesta)-longitud);
      datos_cif[strlen(cadena_respuesta)-longitud]= '\0';
      //printf ("HSM: DATOS_CIF (SALIDA de la funcion fcifrar_cadena ) >%s<\n", datos_cif);
   }
   else
   {
      // printf("HSM: COMANDO >%s<\n", comando);
       printf("HSM: ERROR EN CIFRADO >%.8s<\n",cadena_respuesta+longitud );
       return(-1 );
   }

    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcifrar_cadena  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	
  return (0);
}


/****************************************************************************
 *  NOMBRE:      cifrar_cadena_aes		
 *  PARAMETROS:  socket, datos a cifrar, longitud de los datos, clave
 *  DESCRIPCION: compone y envia al hsm comando de cifrado con identif.
 *				 de la clave, devolviendo el
 *				 resultado en entrada.				
 *				 					
 ****************************************************************************/

int fcifrar_cadena_aes(char * cod_CifDescif, SSL *ssl, char * datos_cif, long long_cif, char * clave_crip,int sfd, char * vector_inicial_aes)
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcifrar_cadena_aes  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	
   char cadena_aux[LONG_MSG];
   char comando[LONG_MSG]; //contiene el comando completo para enviar al hsm
   char cadena_respuesta[LONG_MSG];
   int longitud = 0;
   char ch_long_aux[7];
   int longitud_aux = 0;
   int retorno = 0;   
   int n_sock = 0; 
   int fin_funcion = 0;
   
   
   //printf("fcifrar_cadena %s\n", cod_CifDescif); 
   memset(cadena_aux, '\0', sizeof(cadena_aux));
   memset(ch_long_aux, '\0', sizeof(ch_long_aux));
   memset(cadena_respuesta, '\0', sizeof(cadena_respuesta));
   
   //validacion datos obligatorios - si no pasa la validacion error -2
   if (long_cif == 0)
   {
      printf("HSM: longitud 0\n");
	  return(-2);   
   }
   if(strlen(datos_cif) ==0)
   {
      printf("HSM: faltan datos\n");
	  return(-2);
   }
   if(strlen(clave_crip) ==0)
   {
      printf("HSM: falta clave\n"); 
	  return(-2);
   }    
   if(long_cif % 16 != 0)
   {
      printf("HSM: debe ser multiplo de 16\n");
	  return(-2);
   }
   
   // Componer mensaje hsm 	
   strncpy(cadena_aux, cabecera, strlen(cabecera));
   longitud = longitud + strlen(cabecera);
   strncpy(cadena_aux + longitud, DESCIFRA_DATO_AES, strlen(DESCIFRA_DATO_AES)); //0903 carga_hmk.h
   longitud = longitud + strlen(DESCIFRA_DATO_AES);  
   strncpy(cadena_aux + longitud, clave_crip, strlen(clave_crip));
   longitud = longitud + strlen(clave_crip);
   strncpy(cadena_aux + longitud, cod_CifDescif, strlen(cod_CifDescif));
   longitud = longitud + strlen(cod_CifDescif); //1:Cifrar 0:Descifrar//   
   strncpy(cadena_aux + longitud, "0", 1); //  0:CBC --> no cambia
   longitud = longitud + 1;
   strncpy(cadena_aux + longitud,vector_inicial_aes, 32); //{"00000000000000000000000000000000"} en conexhsm.h
   longitud = longitud + 32;

   
   sprintf(ch_long_aux, "%06d", long_cif); 		 
   strncpy(cadena_aux + longitud, ch_long_aux, strlen(ch_long_aux));   
   longitud = longitud + strlen(ch_long_aux);
   
   strncpy(cadena_aux + longitud, datos_cif, long_cif);
   longitud = longitud + long_cif;
   cadena_aux[longitud] = '\0'; // Cierre de cadena.

   longitud_aux = strlen(cadena_aux);
   sprintf(ch_long_aux, "%06d", longitud_aux);
   strncpy(comando, ch_long_aux, strlen(ch_long_aux));
   longitud = longitud + strlen(ch_long_aux);
   strncpy(comando + strlen(ch_long_aux), cadena_aux, strlen(cadena_aux));
   comando[longitud] = '\0'; //Cierre de la cadena que tiene el comando hsm.

   // Envio del mensaje - Llamada a funcion. 
   retorno = enviar_mensajes_hsm(ssl, comando, strlen(comando));
   if(retorno <=0)
   {	
         printf("HSM: ERROR EN send CIFRADO %d\n",errno);
         return(retorno);     
   }

   retorno=recv_to(ssl,cadena_respuesta,timeout,sfd); //devuelve los bytes leidos
   if(retorno<=0)
   {	
      printf("HSM: ERROR EN recv CIFRADO %d\n",errno); 
	  	return(-145);
   }
   
   cadena_respuesta[retorno]='\0';
   printf("--- Cadena_respuesta HSM: %d >%.*s<\n",retorno, retorno, cadena_respuesta); 
   longitud = 0; // se inicializa de nuevo
   memset(datos_cif, '\0', long_cif);
   longitud = strlen(ch_long_aux) + strlen(cabecera) +strlen(DESCIFRA_DATO);
   //printf("cadena_respuesta+longitud >%.8s<\n",cadena_respuesta+longitud );

   if (strncmp(cadena_respuesta+longitud,"00000000", 8)== 0)
   {
      longitud = longitud + 8 + strlen(ch_long_aux);
	  //Conversion a binario de los dos bloques para la operacion
    //  fin_funcion = fHexa_binario(cadena_respuesta + longitud,datos_cif);
       strncpy(datos_cif, cadena_respuesta + longitud, strlen(cadena_respuesta)-longitud);
       datos_cif[strlen(cadena_respuesta)-longitud]= '\0';
	  //printf("datos_cif: >%.*s<\n",strlen(datos_cif),datos_cif); 
      //printf("respuesta OK\n");
   }
   else
   {
       printf("HSM: COMANDO >%s<\n", comando);
       printf("HSM: ERROR EN CIFRADO >%.8s<\n",cadena_respuesta+longitud );
       return(-1 );
   }
  
     //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcifrar_cadena_aes  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  return (0);
}

int fHash(char * hashMec,SSL *ssl, char * datos_cif, long long_cif, char * status, int sfd)    
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fHash  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   char cadena_aux[LONG_MSG];
   char comando[LONG_MSG]; //contiene el comando completo para enviar al hsm
   char cadena_respuesta[LONG_MSG];
   int longitud = 0;
   char ch_long_aux[7];
   int longitud_aux = 0;
   int retorno = 0;
   int n_sock = 0; //Alberto.

   memset(cadena_aux, '\0', sizeof(cadena_aux));
   memset(ch_long_aux, '\0', sizeof(ch_long_aux));
   memset(cadena_respuesta, '\0', sizeof(cadena_respuesta));

   strncpy(cadena_aux, cabecera, strlen(cabecera));
   longitud = longitud + strlen(cabecera);
   strncpy(cadena_aux + longitud, HASH_DATO, strlen(HASH_DATO)); //2000
   longitud = longitud + strlen(HASH_DATO);
   strncpy(cadena_aux + longitud, hashMec, strlen(hashMec)); //mecanismo
   longitud = longitud + strlen(hashMec);
   strncpy(cadena_aux + longitud, status, strlen(status)); // estado
   longitud = longitud + strlen(status);
   strncpy(cadena_aux + longitud, "01", 2);
   longitud = longitud + 2; //hexadecimal o binario

   sprintf(ch_long_aux, "%06d", long_cif);     
   strncpy(cadena_aux + longitud, ch_long_aux, strlen(ch_long_aux));
   longitud = longitud + strlen(ch_long_aux);

   strncpy(cadena_aux + longitud, datos_cif, long_cif); 
   longitud = longitud + long_cif;

   //compobar el estaus, si es 03, hay que rellenar a 000000 el segundo campo de longitud
   if (strncmp(status, "03", 2)==0)
   {//lo ponemos a 00 porque no viene nada mas
      strncpy(cadena_aux + longitud, "000000", 6);
         longitud = longitud + 6;
   }
   cadena_aux[longitud] = '\0'; // Cierre de cadena.

   longitud_aux = strlen(cadena_aux);
   sprintf(ch_long_aux, "%06d", longitud_aux);
   strncpy(comando, ch_long_aux, strlen(ch_long_aux));
   longitud = longitud + strlen(ch_long_aux);
   strncpy(comando + strlen(ch_long_aux), cadena_aux, strlen(cadena_aux));
   comando[longitud] = '\0'; //Cierre de la cadena que tiene el comando hsm.


   retorno = enviar_mensajes_hsm(ssl, comando, strlen(comando));
   if(retorno <=0)
   {
         printf("HSM: ERROR EN send %d\n",errno);
         return(retorno);
   }

   retorno=recv_to(ssl,cadena_respuesta,timeout, sfd); //devuelve los bytes leidos
   if(retorno<=0)
   {
      printf("HSM: ERROR EN recv %d\n",errno);
      return(-145);
   }

   cadena_respuesta[retorno]='\0';
   longitud = 0; // se inicializa de nuevo
   memset(datos_cif, '\0', long_cif);
   longitud = strlen(ch_long_aux) + strlen(cabecera) +strlen(HASH_DATO);
  // printf("HSM: respuesta: %s\n", cadena_respuesta);

   if (strncmp(cadena_respuesta+longitud,"00000000", 8)== 0)
   {
      longitud = longitud + 8 + 2 + strlen(ch_long_aux);
      strncpy(datos_cif, cadena_respuesta + longitud, strlen(cadena_respuesta)-longitud); // mandamos la longitus mas el tama�o
      datos_cif[strlen(cadena_respuesta)-longitud]= '\0';

   }
   else
   {
      // printf("HSM: COMANDO >%s<\n", comando);
       printf("HSM: ERROR EN HASHEAR  >%.8s<\n",cadena_respuesta+longitud );
       return(-1 );
   }

    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fHash  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   return (0);
}

int fRsa(SSL *ssl, char * datos_cif, long long_cif, char * clave_rsa, int sfd)    
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fRsa  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   char cadena_aux[LONG_MSG];
   char comando[LONG_MSG]; //contiene el comando completo para enviar al hsm
   char cadena_respuesta[LONG_MSG];
   int longitud = 0;
   char ch_long_aux[7];
   int longitud_aux = 0;
   int retorno = 0;
   int n_sock = 0; //Alberto.

   memset(cadena_aux, '\0', sizeof(cadena_aux));
   memset(ch_long_aux, '\0', sizeof(ch_long_aux));
   memset(cadena_respuesta, '\0', sizeof(cadena_respuesta));

   //validacion datos obligatorios - si no pasa la validacion error -2
   if (long_cif == 0)
   {
      printf("HSM: longitud 0\n");
      return(-2);   
   }
   if(strlen(datos_cif) ==0)
   {
      printf("HSM: faltan datos\n");
      return(-2);
   }   
   
   // Componer mensaje hsm 	
   strncpy(cadena_aux, cabecera, strlen(cabecera));
   longitud = longitud + strlen(cabecera);
   strncpy(cadena_aux + longitud, RSA_DATO, strlen(RSA_DATO)); //comando 1103
   longitud = longitud + strlen(RSA_DATO);
   strncpy(cadena_aux + longitud, clave_rsa, strlen(clave_rsa)); //clave
   longitud = longitud + strlen(clave_rsa);
   strncpy(cadena_aux + longitud,"12",2); // hash mechanism 12
   longitud = longitud + 2;
//SGL  no pude estae a pelo, el campo debe viajar en el mismo sitio que el texto a cifrar, en el primero caracter
   strncpy(cadena_aux + longitud, "0101",4); // algoritmo 01 + pad 01
   longitud = longitud + 4; 
   
   sprintf(ch_long_aux, "%06d", long_cif);   
   strncpy(cadena_aux + longitud, ch_long_aux, strlen(ch_long_aux));   
   longitud = longitud + strlen(ch_long_aux);
   
   strncpy(cadena_aux + longitud, datos_cif, long_cif); //SGL tener en cuenta que en la primera posicion va el algoritmo
   longitud = longitud + long_cif;
   cadena_aux[longitud] = '\0'; // Cierre de cadena

   longitud_aux = strlen(cadena_aux);
   sprintf(ch_long_aux, "%06d", longitud_aux);
   strncpy(comando, ch_long_aux, strlen(ch_long_aux));
   longitud = longitud + strlen(ch_long_aux);
   strncpy(comando + strlen(ch_long_aux), cadena_aux, strlen(cadena_aux));
   comando[longitud] = '\0'; //Cierre de la cadena que tiene el comando hsm.
  // printf("HSM: COMANDO >%s<\n", comando);
    
   // Envio del mensaje - Llamada a funcion. 
   retorno = enviar_mensajes_hsm(ssl, comando, strlen(comando));
   if(retorno <=0)
   {    
         printf("HSM: ERROR send RSA %d\n",errno);
         return(retorno);     
   }
   
   retorno=recv_to(ssl,cadena_respuesta,timeout, sfd); //devuelve los bytes leidos
   if(retorno<=0)
   {    
      printf("HSM: ERROR EN recv %d\n",errno);
      return(-145);
   }
   //printf("retorno fRsa %d\n", retorno);
   cadena_respuesta[retorno]='\0';
   longitud = 0; // se inicializa de nuevo
   memset(datos_cif, '\0', long_cif);
   longitud = strlen(ch_long_aux) + strlen(cabecera) +strlen(RSA_DATO);

   if (strncmp(cadena_respuesta+longitud,"00000000", 8)== 0)
   {
      longitud = longitud + 8 + strlen(ch_long_aux);
      strncpy(datos_cif, cadena_respuesta + longitud, strlen(cadena_respuesta)-longitud);
      datos_cif[strlen(cadena_respuesta)-longitud]= '\0';

   }
   else
   {
     //  printf("HSM: COMANDO >%s<\n", comando);
       printf("HSM: ERROR EN RSA_DATO >%.8s<\n",cadena_respuesta+longitud );
       return(-1 );
   }
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fRsa  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   return (0);
}


//compone el mensaje para el offset
 int foffset(SSL *ssl, char * datos_cif, long long_cif, char * clave_crip,int sfd)
  
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina foffset  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   char cadena_aux[LONG_MSG];
   char comando[LONG_MSG]; //contiene el comando completo para enviar al hsm
   char cadena_respuesta[LONG_MSG];
   int longitud = 0;
   char ch_long_aux[7];
   int longitud_aux = 0;
   int retorno = 0;   
   int n_sock = 0; //Alberto.
   char clave_gen_pin[50]; 
   char clave_trans_pin[50];

   
   
   //printf("fcifrar_cadena %s\n", cod_CifDescif); 
   memset(cadena_aux, '\0', sizeof(cadena_aux));
   memset(ch_long_aux, '\0', sizeof(ch_long_aux));
   memset(cadena_respuesta, '\0', sizeof(cadena_respuesta));
   memset(clave_gen_pin,'\0',sizeof(clave_gen_pin));
   memset(clave_trans_pin,'\0',sizeof(clave_trans_pin));
   
   //validacion datos obligatorios - si no pasa la validacion error -2
   if (long_cif == 0)
   {
      printf("HSM: longitud 0\n");
	  return(-2);   
   }
   if(strlen(datos_cif) ==0)
   {
      printf("HSM: faltan datos\n");
	  return(-2);
   }
   if(strlen(clave_crip) ==0)
   {
      printf("HSM: falta clave\n");
	  return(-2);
   }    
   
   // Se guardan los identificadores para componer las claves
   strncpy(clave_gen_pin, clave_crip, 21); 
   clave_gen_pin[21]='\0';
   strncpy(clave_trans_pin, clave_crip+21, 21); 
   clave_trans_pin[21]='\0';
   
//  //RAQUEL PRUEBA MISMAS CLAVES 
//  strncpy(clave_gen_pin, "S000500028N00065A8283", 21); 
//  clave_gen_pin[21]='\0';
// strncpy(clave_trans_pin, "S000500036N00061F0011", 21);
// clave_trans_pin[21]='\0'; 
   
   
   // Componer mensaje hsm 	RAQUEL !!! REVISAR LO DE LAS 2 CLAVES Y COMO RECUPEAR EL BLOQUE PIN, LONGITUD PAN, PAN , LONGITUD PIN
   strncpy(cadena_aux, cabecera, strlen(cabecera));
   longitud = longitud + strlen(cabecera);
   strncpy(cadena_aux + longitud, CALCULA_OFFSET, strlen(CALCULA_OFFSET)); //0604 carga_hmk.h
   longitud = longitud + strlen(CALCULA_OFFSET); 
   strncpy(cadena_aux + longitud,clave_gen_pin, strlen(clave_gen_pin)); // Clave generacion pin
   longitud = longitud + strlen(clave_gen_pin);
   strncpy(cadena_aux + longitud,clave_trans_pin, strlen(clave_trans_pin)); // clave transporte pin
   longitud = longitud + strlen(clave_trans_pin);
   strncpy(cadena_aux + longitud,tabla_decimal, 16); //{"0123456789012345"} en conexhsm.h cambiada a 849F79EE1F2945A3
   longitud = longitud + 16;
   strncpy(cadena_aux + longitud, "0", 1); //  0:formato ISO0. --> no cambia
   longitud = longitud + 1;
   strncpy(cadena_aux + longitud, "1", 1); //  1:IBM3624-PIN Offset. --> no cambia
   longitud = longitud + 1;
   strncpy(cadena_aux + longitud,datos_cif,16); //  Bloque pin cifrado
   longitud = longitud + 16;
   strncpy(cadena_aux + longitud,datos_cif +16,2); // Longitud PAN
   longitud = longitud + 2;
   strncpy(cadena_aux + longitud,datos_cif +18,16); //PAN
   longitud = longitud + 16;
   strncpy(cadena_aux + longitud,datos_cif+34,1); //longitud PIN 
   longitud = longitud + 1; 
   
   
   sprintf(ch_long_aux, "%06d", long_cif); 		 
  // strncpy(cadena_aux + longitud, ch_long_aux, strlen(ch_long_aux));   
  // longitud = longitud + strlen(ch_long_aux); 
  //
  // strncpy(cadena_aux + longitud, datos_cif, long_cif);
  // longitud = longitud + long_cif; 
   cadena_aux[longitud] = '\0'; // Cierre de cadena.
   


   sprintf(ch_long_aux, "%06d", longitud);
   strncpy(comando, ch_long_aux, strlen(ch_long_aux));
   longitud = longitud + strlen(ch_long_aux);
   strncpy(comando + strlen(ch_long_aux), cadena_aux, strlen(cadena_aux));
   comando[longitud] = '\0'; //Cierre de la cadena que tiene el comando hsm.

   // Envio del mensaje - Llamada a funcion. 
 retorno = enviar_mensajes_hsm(ssl, comando, strlen(comando));

 if(retorno <=0)
 {	
       printf("HSM: ERROR EN send CIFRADO %d\n",errno);
       return(retorno);     
 }

 retorno=recv_to(ssl,cadena_respuesta,timeout,sfd); //devuelve los bytes leidos

 if(retorno<=0)
 {	
    printf("HSM: ERROR EN recv CIFRADO %d\n",errno); 
   return(-145);
 }
 
 cadena_respuesta[retorno]='\0';

 longitud = 0; // se inicializa de nuevo
 memset(datos_cif, '\0', long_cif); 
 longitud = strlen(ch_long_aux) + strlen(cabecera) +strlen(CALCULA_OFFSET);

 
 if (strncmp(cadena_respuesta+longitud,"00000000", 8)== 0)
 {
  
    longitud = longitud + 8;

    strncpy(datos_cif, cadena_respuesta + longitud, strlen(cadena_respuesta)-longitud);
    datos_cif[strlen(cadena_respuesta)-longitud]= '\0';

    //printf("respuesta OK\n");
 }
 else
 {
     printf("HSM: COMANDO >%s<\n", comando);
     printf("HSM: ERROR EN OFFSET >%.8s<\n",cadena_respuesta+longitud );
     return(-1 );
 }
  
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina foffset  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   return (0);    
    
}  


//////////////// CONVERSOR de ASCII A HEXADECIMAL ///////////////////////////
void fAscii_Hexa (int lonAscii, char * charAscii, char * charHexa)
{
 int lonHexa = 0;
 int i = 0; 
 int numCodAsci=0;
 char auxHexa8[8];
 
  for(i = 0; i<lonAscii; i++)
  {          
     numCodAsci=charAscii[i];
     if(numCodAsci<0)
     {
         sprintf(auxHexa8, "%02X", charAscii[i]);  //1 caracter ascii lo convierte en 8 cifras hexadecimales
                                                   //de las que las 2 ultimas son las significativas
         sprintf(charHexa+i*2, "%.2s", auxHexa8+6);  //convierte 1 caracter ascii en 2 hexadecimales
     }                                                     
     else
     {
         sprintf(charHexa+i*2, "%02X", charAscii[i]);  //convierte 1 caracter ascii en 2 hexadecimales
     }
  }
  lonHexa = 2 * lonAscii;
  charHexa[lonHexa]='\0';
}

//////////////// CONVERSOR HEXADECIMAL a ASCII //////////////////
int fHexa_ascii (char * cadena, char * resultado)
{

 int lonCadena = 0;
 int i = 0; // Contador para recorrer cadena hexadecimal
 int j = 0; // Contador para recorrer cadena ascii
 char cIz = 0;
 char cDe = 0;
 char auxC =0;

   lonCadena = strlen(cadena);
   
   if(lonCadena==0)
   {
      return(0);
   }

   for(i = 0; i < lonCadena; i++)
   {
       if(i % 2 != 0) /// en posicion impar tengo la pareja pq empieza en 0
       { 
          cDe=cadena[i];
          auxC = hex_to_ascii(cIz, cDe);                    
          resultado[j]=auxC;
          
          j++;
       }else // si es par guardo el caracter de la izquierda
       {
          cIz = cadena[i];
       }
   }
   resultado[lonCadena/2]='\0';

   return(lonCadena/2);
}


int hex_to_ascii(char i, char d){
        int high = hex_to_int(i) * 16;
        int low = hex_to_int(d);
        return high+low;
}


////////////////////////  PARA COBOL ////////////////////////////////////
//funcion para obtener claves para el cobol, que luego llama a la nuestra
void claveshsm_C(char * comando, char * subsistema, char * bin, char * clave1, char * clave2, char * codResp)
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina claveshsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	int retorno=0; 
	int errValida=0;

    if (strlen(comando) < 2)
    {
       printf("HSM: longitud del comando menor de la esperada >%s<\n", comando);
       errValida = -2;
    }
    if (!errValida && strlen(subsistema) < 4)
    {
       printf("HSM: longitud del subsistema menor de la esperada >%s<\n", subsistema);
       errValida = -2;
    }

    if (!errValida && strlen(bin) < 6)
    {
       printf("HSM: longitud del bin menor de la esperada >%s<\n", bin);
       errValida = -2;
    }
    
    if(!errValida)
    {
       retorno=claveshsm(comando, subsistema, bin, clave1, clave2);
       if (retorno == 0)
       {
          strncpy(codResp,"01",2);
          codResp[2]='\0';
       }
       else
       {
         printf("HSM: retorno claveshsm >%d< \n", retorno);
         strncpy(codResp,"02",2);
         codResp[2]='\0';
       }
    }
    else
    {
       strncpy(codResp,"02",2);
       codResp[2]='\0';    
    }
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina claveshsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

}
//funcion de desconexion para el cobol, que luego llama a la nuestra
void desconexhsm_C(struct sfdssl * sockssl, char * codResp) 
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina desconexhsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

    int retorno;
    
    desconexhsm(sockssl->sfd_ssl);
    SSL_free(sockssl->ssl_ssl);
    SSL_CTX_free(ctx);
    printf ("liberados ssl <%i> y ctx <%i>\n",sockssl->ssl_ssl,ctx); 
    strncpy(codResp,"01",2);
    codResp[2]='\0';
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina desconexhsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

}

//funcion de manejador para el cobol, que luego llama a la nuestra
void manejhsm_C(char * clavehsm, char* comandohsm, char * datoshsm, char *NumByte, struct sfdssl * sockssl, char * prespuestahsm, char * codResp) 
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina manejhsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf("Valor clavehsm   >%s<\n",clavehsm);
	printf("Valor comandohsm >%*s<\n",strlen(comandohsm),comandohsm);
	printf("Valor datoshsm   >%*s<\n",strlen(datoshsm),datoshsm);
	printf("Valor NumByte    >%s<\n",NumByte);

 int retorno=0;
 int errValida=0;
 char auxClave[151];
 char auxComando[3];
 char auxDatos[1501];
 char auxNumByte[5];
 char auxSock[9];
 int longiDato=0;
 
    if (sizeof(sfdssl) < 8)
    {
       printf("HSM: longitud del socket menor de la esperada\n");
       errValida = -2;
    }
    if (!errValida && strlen(comandohsm) < 2)
    {
       printf("HSM: longitud del comando menor de la esperada\n");
       errValida = -2;
    }
    if (!errValida && strlen(NumByte) < 4)
    {
       printf("HSM: longitud del dato numByte menor de la esperada\n");
       errValida = -2;
    }
    
    if(strlen(clavehsm) < 150)
    {
       strncpy(auxClave, clavehsm, strlen(clavehsm));
       fEliminar_Blancos(auxClave, strlen(clavehsm));
    }
    else
    {
       strncpy(auxClave, clavehsm, 150);
       fEliminar_Blancos(auxClave, 150);
    }
       
    strncpy(auxComando, comandohsm,2);
    auxComando[2]='\0';
    strncpy(auxNumByte, NumByte,4);
    auxNumByte[4]='\0';
    longiDato=atoi(auxNumByte);
    
    if(!errValida)
    {
      memcpy(auxDatos, datoshsm, longiDato);
      auxDatos[longiDato]='\0';
     // strncpy(auxSock, sockssl.sfd_ssl,8);
     // auxSock[8]='\0';
      //EL VALOR DEL 3 CAMPO SE TIENE TOCAR CON LOS NUEVOS COMANDOS, PORQUE YA SI VAN A SER CLAVES AES
      retorno=manejhsm(auxClave, auxComando, 0, auxDatos, longiDato, sockssl , prespuestahsm); //PRJ0258542 - valor fijo 0
    
      if (retorno > 0)
      {
         strncpy(codResp,"01",2); //correcto
         codResp[2]='\0';
      }
      else
      {
         if(retorno == -145)
         {
            strncpy(codResp,"03",2); //error de comunicacion
            codResp[2]='\0';
         }
         else
         {
            if(retorno == -2)
            {
               strncpy(codResp,"02",2); //datos de entrada erroneos
               codResp[2]='\0';         
            }
            else
            {
               strncpy(codResp,"04",2); //otros errores
               codResp[2]='\0';
            }         
        }
      //  printf("HSM: retorno manejhsm >%d< codResp >%s<\n", retorno, codResp);
      }
    }
    else
    {
         strncpy(codResp,"02",2);
         codResp[2]='\0';
    }
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina manejhsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

}

//funciona para conectar desde cobol
void conexhsm_C(char* codent, char *comando, struct sfdssl * sockssl, char * codResp)
{

    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina conexhsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	printf("Valor codent   >%s<\n",codent);
	printf("Valor comando  >%.*s<\n",strlen(comando),comando);
	printf("Valor sockssl->sfd_ssl >%i<\n",sockssl->sfd_ssl);
  printf("**************************************************\n");  

  //comando por si hay una ip de hsm por cada tipo de operaciones en un futuro
  //desde cobol no informamos los datos de NPTDHSM.
  int retorno;
  NPSDHSM registro;
  const SSL_METHOD *method;
  int SSL_VERIFY_FAIL;
  
  memset(registro.dhcodent, ' ', sizeof(NPSDHSM));
  // retorna el valor del socket
  retorno = conexhsm(codent,registro);
  if (retorno > 0)
  {
      sockssl->sfd_ssl=retorno;
   //  sprintf(sockssl->sfd_ssl, "%08d", retorno);   
  // PARTE INICAL PARA HACER LA CONEXION SEGURA
       SSL_library_init();

       method = TLSv1_2_client_method();

       ctx = SSL_CTX_new(method);


   //SE UTILIZAN PARA INTERCAMBIO DE CERTIFICADOS
   // SSL_CTX_set_verify(ctx,SSL_VERIFY_FAIL, NULL);
   // SSL_CTX_load_verify_locations(ctx, CAFILE, CADIR) ;
   
       sockssl->ssl_ssl = SSL_new(ctx);

      
       if (sockssl->ssl_ssl == NULL)
       {
            fprintf(stderr, "SSL_new() failed\n");
            //printf("error retorno del conex \n");
            strncpy(codResp,"02",2);
            codResp[2]='\0';
            exit(EXIT_FAILURE);
       }
       
        SSL_set_fd(sockssl->ssl_ssl, sockssl->sfd_ssl);  /* attach the socket descriptor */
        const int status = SSL_connect(sockssl->ssl_ssl);
        printf ("status<%i>\n",status);  
    
       if (status != 1)
       {
           SSL_get_error(sockssl->ssl_ssl, status);
           ERR_print_errors_fp(stderr); //High probability this doesn't do anything
           fprintf(stderr, "SSL_connect failed with SSL_get_error code %d\n", status);
           //printf("error retorno del conex \n");
           strncpy(codResp,"02",2);
           codResp[2]='\0';
           exit(EXIT_FAILURE);
       }
       
        strncpy(codResp,"01",2);
        codResp[2]='\0';
        printf("SSL conectado\n");
        
  }
  else
  {
      //printf("error retorno del conex %d\n", retorno);
      strncpy(codResp,"02",2);
      codResp[2]='\0';
  }
     //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina conexhsm_C  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

}
/*****************************************************************************/
/*                                                                           */
/*   Params:                                                                 */
/*     IN:  fd     - (int) socket file descriptor                            */
/*     OUT: buffer - (char*) buffer to hold data                             */
/*     IN: to     - (int) timeout in milliseconds                            */
/*   Results:                                                                */
/*      int    - (int) resultado:                                            */
/*                  -1 = Error                                               */
/*                   0 = Timeout                                             */
/*                  >0 = OK                                                  */
/*   Notes:                                                                  */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
int recv_to(SSL *ssl, char *buffer, int to, int fd) {

    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina recv_to  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
  
   fd_set readset;
   int result, iof = -1;
   struct timeval tv;
   char plen[7];
   //char *bufferAux;
   int bytesLeidos=0;
   int indx=0;
   int nBytesMsg=0;

   
   memset(buffer,'\0',LONG_MSG);  // para que imprima correctamente.
   //Se saca a otra funcion para mejor legibilidad  
   nBytesMsg=getLenMsgToReceive(ssl,fd,buffer,to);

  // printf("recibioms %i\n", nBytesMsg);
   if (nBytesMsg<=0)
   {
       fprintf(stdout,"Error de getLenMsgToReceive:%d\n", nBytesMsg);
       fflush(stdout);
       desconexhsm(fd);
       return nBytesMsg;
   }    
   
   indx=6;

   do{
       
       bytesLeidos = SSL_read(ssl, buffer+indx, (nBytesMsg-(indx-6)));
       indx += bytesLeidos;   
 
         
   }while((indx-6) < nBytesMsg);     

   result = indx;   
   
    //BIMSF-3123
	fechahorasys(&tm);
  printf(" ** ** -- Cadena de salida directamente del HSM: >%s<\n", buffer); //PRJ0258452
	printf(" ** ** ** Saliendo rutina recv_to  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

    return result; 

}

/*****************************************************************************/
/*                                                                           */
/*   Params:                                                                 */
/*     IN:  fd     - (int) socket file descriptor                            */
/*     OUT: buffer - (char*) buffer to hold data                             */
/*     IN: to     - (int) timeout in milliseconds                            */
/*   Results:                                                                */
/*      int    - (int) resultado:                                            */
/*                  -1 = Error                                               */
/*                   0 = Timeout                                             */
/*                  >0 = OK                                                  */
/*   Notes:                                                                  */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/
int getLenMsgToReceive(SSL *ssl,int fd, char *buffer, int to) {
    
     fd_set readset;
   int result, iof = -1;
   struct timeval tv;
   char plen[7];
   int bytesLeidos=0;
   int nBytesMsg=0;
    
   
   //Obtenemos la longitud del mensaje a recibir

   memset(plen,'\0',7); 
   
   FD_ZERO(&readset);
   FD_SET(fd, &readset);
   
   // Initialize time out struct
   tv.tv_sec = to;
   tv.tv_usec = 0; //to * 1000
 
   // select()
   result = select(fd+1, &readset, NULL, NULL, &tv); 

   if (result<0)  //error
   {
       fprintf(stdout,"HSM: Error de RECV:%d\n", result);
      fflush(stdout);
      desconexhsm(fd);
      return result;
   }
      
   if(result==0)  //timeout 
   {
       fprintf(stdout,"HSM: Alcanzado el timeout de:%d seg.\n", to);
      fflush(stdout);
      desconexhsm(fd);
      return result;
   }
          
   if (result > 0 && FD_ISSET(fd, &readset))
    { //todo ok
         // Set non-blocking mode
      if ((iof = fcntl(fd, F_GETFL, 0)) != -1)
         fcntl(fd, F_SETFL, iof | O_NONBLOCK);    
             
      //bytesLeidos=recv(fd, plen, 6, 0);

     bytesLeidos=SSL_read(ssl,plen, 6);

          // set as before
      if (iof != -1)
        fcntl(fd, F_SETFL, iof); 
       
   }else{
           fprintf(stdout,"HSM: Error en FD_ISSET");
          fflush(stdout);
         return -1;
    } 
   
   if(bytesLeidos!=6)
   {
      fprintf(stdout,"HSM: Error. Se esperaba recibir la longitud del mensaje\n");   
      fflush(stdout);      
      if (bytesLeidos > 0)
        return bytesLeidos *(-1);  //ojo revisar si tengo que cerrar o no el socket.       
      else
        return bytesLeidos;
   }

   nBytesMsg=atoi(plen);
   memcpy(buffer, plen,6);
   
   return nBytesMsg;
    
}

char* hex_2_base64(char *_hex)
{
  char *hex_2_bin[16] = { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111" };
  char *dec_2_base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i, comproba, cont;

 //allocating memory for binary string
  int bin_size = strlen(_hex) * 4;
  while (bin_size % 6 != 0) //add space for zero padding
    bin_size += 8;
  char *bin = malloc(bin_size + 1);
  memset(bin, 0, bin_size + 1); 

  //these are for strtol, its arguments need the zero terminator
  char buf[2] = { 0 };
  char b64buf[6 + 1] = { 0 };

 //converting hex input to binary
 char *bin_end = bin;
  for (i = 0; i < strlen(_hex); i++)
  {
    buf[0] = _hex[i];
    memcpy(bin_end, hex_2_bin[strtol(buf, NULL, 16)], 4);
    bin_end += 4;
  }

  //pad binary string w/ zeroes
 comproba = 0;
 int total = strlen(bin);
 while (strlen(bin) < bin_size)
 {
    comproba = 1;
    strcat(bin, "00000000");
 }

 //allocating memory for b64 output
   int b64size = (strlen(bin) / 6) + 1;
  char *out = malloc(b64size);
  memset(out, 0, b64size);

 //walk through binary string, converting chunks of 6 bytes into base64 chars
 char *bin_ptr = bin;
  char *out_end = out;
  int index_b64;
  cont=0;
  while (*bin_ptr) 
  {
    strncpy(b64buf, bin_ptr, 6);
    index_b64 = strtol(b64buf, NULL, 2);
    if ((index_b64 == 0) && (comproba == 1) && (cont > total))
      buf[0] = '=';
    else
      buf[0] = dec_2_base64[index_b64];
    memcpy(out_end, buf, 1);
    out_end += 1;
    bin_ptr += 6;
    cont += 6;
  }

  free(bin);
  return out;
}

void fechahorasys(struct tm_dec *tmPtr) { 

    time_t tt;
    struct tm *tm11;
    struct timeval tim;

    tt = time(NULL);
    tm11=localtime(&tt);

    tmPtr->tm_mon= 0;
    tmPtr->tm_mon= tm11->tm_mon+1;
    tmPtr->tm_mday= tm11->tm_mday;
    tmPtr->tm_hour= tm11->tm_hour;
    tmPtr->tm_min= tm11->tm_min;
    tmPtr->tm_sec= tm11->tm_sec;
    tmPtr->tm_year= tm11->tm_year+1900;

    //mmilisegundos
    gettimeofday(&tim, NULL);
    tmPtr->tm_mls = tim.tv_usec/1000;
}

//añadido para PRICE
//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE PETICION DE MAC                           //
//PRJ0258542 - se anade el 3º parametro para saber si la clave es TDES o AES
//*****************************************************************************************//
// DMND0037316 se modificara esta funcion para poder usar claves AES en el comando 801
int fcomponerMAC(char * cad_datos, char * comando_pet, int tipcla, char * clave_mac, long long_mac)
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerMAC  20240927 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  char mensaje_completo[1024]; 
	int suma_long = 0;
	char long_aux[6];
	//char vector_inicial_aes[32] = '00000000000000000000000000000000'; //PRJ0258452 - crear variable para usar en caso de AES
	//printf(">>>>>>>>>>>>>>>INICIO COMPONER MAC\n");
	memset(mensaje_completo, '\0', sizeof(mensaje_completo));
	memset(long_aux, '\0', sizeof(long_aux));  
	//printf("\n>>>>>>>>>>>>>>>COMPONER MAC\n");
	strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
	suma_long = strlen(comando_pet);
	//printf(" Mensaje completo 1:%s< \n", mensaje_completo);  //PRJ0258542

	strncpy(mensaje_completo + suma_long, clave_mac, strlen(clave_mac));
	//printf(" Mensaje completo 2:%s< \n", mensaje_completo);  //PRJ0258542
	suma_long = suma_long + strlen(clave_mac);
	//DMND0037316 para que el comando funcione con clave AES, segun indicaciones de
	//realsec bastaria con sustituir en la formacion del comando el indicador de algoritmo
	//que ahora mismo es ANSI 9.19  y pasaria a ser 3 - CBC MAC (AES only).
	//PRJ0258542 - INI - pregunta por el valor de tipcla, para saber si es AES o TDES 0 y 3 es TDES, 5 es AES
	if (tipcla == 5)
	{
		strncpy(mensaje_completo + suma_long,"3", 1); //Indicador de algoritmo:CBC MAC (AES only)
	}
	else
	{
		strncpy(mensaje_completo + suma_long,"1", 1); //Indicador de algoritmo: ANSI 9.19
	}
	//PRJ0258542 - FIN
	
	//printf(" Mensaje completo 3:%s< \n", mensaje_completo);  //PRJ0258542
	suma_long = suma_long + 1;
	//DMND0037316 me cabe la duda si esta linea 3131 habria que sustituir ese "0" por un "2"
	//llegado el momento podeis consultarlo con Alfonso de Realsec
	strncpy(mensaje_completo + suma_long,"0", 1); //Indicador de relleno: metodo 1
	suma_long = suma_long + 1;
	//printf(" Mensaje completo 4:%s< \n", mensaje_completo);  //PRJ0258542

	strncpy(mensaje_completo + suma_long,"0", 1); //Indicador de hash: No hash
	suma_long = suma_long + 1;
	//printf(" Mensaje completo 5:%s< \n", mensaje_completo);  //PRJ0258542

	//PRJ0258542 - INI - pregunta por el valor de tipcla, para saber si es AES o TDES 0 y 3 es TDES, 5 es AES
	if (tipcla == 5)
	{
		strncpy(mensaje_completo + suma_long,vector_inicial_aes, 32); //Vector de inicialización de 32 para AES
  	suma_long = suma_long + 32;
	}
	else
	{
		strncpy(mensaje_completo + suma_long,vector_inicial, 16); //Vector de inicialización de 16 para TDES
  	suma_long = suma_long + 16;
	}
	//PRJ0258542 - FIN
	//printf(" Mensaje completo 6:%s< \n", mensaje_completo);  //PRJ0258542

	sprintf(long_aux, "%06d", long_mac); // Se convierte en una cadena de 6
	strncpy(mensaje_completo + suma_long, long_aux, 6);
	suma_long = suma_long + 6;
	//printf(" Mensaje completo 7:%s< \n", mensaje_completo);  //PRJ0258542

	strncpy(mensaje_completo + suma_long, cad_datos, long_mac);
	suma_long = suma_long + long_mac;
	//printf(" Mensaje completo 8:%s< \n", mensaje_completo);  //PRJ0258542

	mensaje_completo[suma_long] = '\0'; 
	//printf(" Mensaje completo 9:%s< \n", mensaje_completo);  //PRJ0258542

	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';

	//BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerMAC  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	return(0);
}

//añadido para PRICE
//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE CALCULO DE ARPC						   //	
//*****************************************************************************************//
int fcomponerCRIPTOGRAMA(char * cad_datos, char * cad_clave, char * comando_pet, char * operacion)
{
    //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerCRIPTOGRAMA  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

 int long_mensaje = 0;
 int long_datos_calculo = 0;

 char long_aux[6];
 char mensaje_completo[1024];
 char datos_diversif[50];
 char orden_clave[7];


    //Inicializacion de las variables
   memset(mensaje_completo,'\0',sizeof(mensaje_completo));
   memset(long_aux,'\0',sizeof(long_aux));
   memset(datos_diversif,'\0',sizeof(datos_diversif));
   memset(orden_clave, '\0', sizeof(orden_clave));

   //printf("COMPONER/VERIFICAR CRIPTOGRAMA\n");
   //printf("cad_datos es >%s<\n",cad_datos);
   //printf("comando_pet es >%s<\n",comando_pet);

// Identifica los datos diversificadores PAN || PSN
   strncpy(datos_diversif, cad_datos, 16);   
   datos_diversif[16] = '\0'; // cierre de cadena

   strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
   long_mensaje = long_mensaje + strlen(comando_pet);

   strncpy(mensaje_completo + long_mensaje,operacion, 1); // Opcion de 'Generar ARPC' o 'Verificar ARQC'
   long_mensaje = long_mensaje + 1;
   
   strncpy(mensaje_completo + long_mensaje,ESQUEMA_VISA, 1); // Opcion de 'Esquema VISA'
   long_mensaje = long_mensaje + 1;

   strncpy(mensaje_completo + long_mensaje,cad_clave, strlen(cad_clave));  // clave lmk 06
   long_mensaje = long_mensaje + strlen(cad_clave);

   strncpy(mensaje_completo + long_mensaje,datos_diversif, strlen(datos_diversif));
   long_mensaje = long_mensaje + strlen(datos_diversif);

// Este bloque es solo para la opcion de verificacion de ARQC
   if (strcmp(operacion,VERIF_ARQC)== 0) 
   {
// Longitud de los datos para la verificacion del ARQC
      strncpy(mensaje_completo + long_mensaje, cad_datos + 16, 3);
      long_mensaje = long_mensaje + 3;

/////// SE DEBE OBTENER EL NUMERICO DE LA LONGITUD PARA INCLUIRLO EN LA SIGUIENTE SENTENCIA
      strncpy(long_aux ,cad_datos + 16, 3);
      long_aux[3] = '\0';
      long_datos_calculo = atoi(long_aux);
// Datos para el calculo del ARQC -- TEMPORAL: Tamaño variable ??
      strncpy(mensaje_completo + long_mensaje, cad_datos + 16 + 3, long_datos_calculo);
      long_mensaje = long_mensaje + long_datos_calculo;

// DATOS DEL CRIPTOGRAMA PARA VERIFICACION DE ARQC
      strncpy(mensaje_completo + long_mensaje,cad_datos + 16 + 3 + long_datos_calculo , 16); 
      long_mensaje = long_mensaje + 16;
   }
   else
   {
// DATOS DEL CRIPTOGRAMA PARA CALCULO DE ARPC
        strncpy(mensaje_completo + long_mensaje,cad_datos + 16, 16); //criptograma de 'pregunta'
        long_mensaje = long_mensaje + 16;

//CODIGO DE RESPUESTA USADO PARA ARPC: el que responda a la operacion en BCD
        strncpy(mensaje_completo + long_mensaje,cad_datos + 16 + 16, 4);
        long_mensaje = long_mensaje + 4;
        strncpy(mensaje_completo + long_mensaje,"000000000000", 12); //Rellena el cod.respuesta con 0 binario
        long_mensaje = long_mensaje + 12;
   }
   mensaje_completo[long_mensaje] = '\0'; // Cierre de la cadena
   sprintf(long_aux, "%06d", long_mensaje); 
   long_aux[6] = '\0';

   strncpy(cad_datos,long_aux,6);
   strncpy(cad_datos + 6, mensaje_completo,long_mensaje);
   cad_datos[long_mensaje + 6] = '\0'; // Cierre de la cadena
 
 //BIMSF-3123
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerCRIPTOGRAMA  20240215 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

   return(0);
 }
 
//PRJ270893 - 03-2024
void fHex_bin (char * cadena, char * long_cadena, char * resultado)
{

	int c = 0; // Contador para recorrer cadena
 	int s;
 	int resto, cociente;
 	int varA;
 	char auxD[4];
 	int longitud = 0;
 
  printf("\n");
  printf("*********************************************\n"); 
  printf("***  conexhsm.c version 03/24 - fHex_bin  ***\n"); 
  printf("*********************************************\n"); 

  long_cadena[4]='\0';
  longitud=atoi(long_cadena);

	printf("Longitud <%i> cadena entrada <%s>\n",longitud,cadena);
	while (cadena[c]!=' ' && c < longitud)
  {
	 
  	varA=3;
    strncpy(auxD,"0000",4);
    s=hex_to_int(cadena[c]);
    while (varA > 0)
    {
    	resto=s%2;
      cociente=s/2;
      if (resto == 0)
      	strncpy(auxD+varA,"0",1);
      else
      	strncpy(auxD+varA,"1",1);
      	
      s=cociente;
      varA=varA-1;
   	}
    if (s == 0)  
    	strncpy(auxD+varA,"0",1);
    else
    	strncpy(auxD+varA,"1",1);
 
   	strncpy(resultado + c * 4, auxD, 4); 
    c++;
 	}
  resultado[c * 4] = '\0';
   
  longitud=strlen(resultado);
  //printf("Cadena de salida >%s<, de longitud de salida >%i< \n",resultado,longitud);
  printf("Longitud <%i> cadena salida <%s>\n", longitud, resultado);
  printf("**********************************\n"); 
  printf("\n");
  return;
}

void fBin_hex (char * cadena, char * long_cadena, char * resultado)
{

	int c = 0; // Contador para recorrer cadena
 	int s = 0;
 	int resto, cociente;
 	int varA;
 	char auxD[4];
 	int longitud = 0;
 
  printf("\n");
  printf("*********************************************\n"); 
  printf("***  conexhsm.c version 03/24 - fBin_hex  ***\n"); 
  printf("*********************************************\n"); 

  long_cadena[4]='\0';
  longitud=atoi(long_cadena);

	printf("Longitud <%i> cadena entrada <%s>\n",longitud,cadena);
	while (c < longitud)
  {
  	strncpy(auxD,cadena + c,4);
  	//printf("Trozo >%s< valor origen %i y valor destino %i\n",auxD,c,s);
  	switch (atoi(auxD))
  	{
  		case 0:
  			strncpy(resultado + s,"0",1);
  			break;
     	case 1:
        strncpy(resultado + s,"1",1);
        break;
    	case 10:
        strncpy(resultado + s,"2",1);
        break;
    	case 11:
        strncpy(resultado + s,"3",1);
        break;
    	case 100:
        strncpy(resultado + s,"4",1);
        break;
    	case 101:
        strncpy(resultado + s,"5",1);
        break;
    	case 110:
        strncpy(resultado + s,"6",1);
        break;
    	case 111:
        strncpy(resultado + s,"7",1);
        break;
    	case 1000:
        strncpy(resultado + s,"8",1);
        break;
    	case 1001:
        strncpy(resultado + s,"9",1);
        break;
    	case 1010:
        strncpy(resultado + s,"A",1);
        break;
    	case 1011:
        strncpy(resultado + s,"B",1);
        break;
    	case 1100:
        strncpy(resultado + s,"C",1);
        break;
    	case 1101:
        strncpy(resultado + s,"D",1);
        break;
    	case 1110:
        strncpy(resultado + s,"E",1);
        break;
    	case 1111:
        strncpy(resultado + s,"F",1);
        break;
    }
  	s++;
  	c+=4;
 	}
  resultado[s] = '\0';
   
  longitud=strlen(resultado);
  //printf("Cadena de salida >%s<, de longitud de salida >%i< \n",resultado,longitud);
  printf("Longitud <%i> cadena salida <%s>\n", longitud, resultado);
  printf("**********************************\n"); 
  printf("\n");
  return;
}
//PRJ270893 - 03-2024

//PRJ0258542 - INI
//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE PETICION DE CMAC                           //
//PRJ0258542 - se anade el 3º parametro para saber si la clave es TDES o AES
//*****************************************************************************************//
int fcomponerCMAC(char * cad_datos, char * comando_pet, int tipcla, char * clave_mac, long long_mac)
{
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerCMAC  20241021 v2.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  char mensaje_completo[1024]; 
	int suma_long = 0;
	char long_aux[6];
	char separa1[1];
	char separa2[2];
	
	//separa1 = '0x3B';
	//separa2 = '0x23';
	
	//printf(">>>>>>>>>>>>>>>INICIO COMPONER CMAC\n");
	memset(mensaje_completo, '\0', sizeof(mensaje_completo));
	memset(long_aux, '\0', sizeof(long_aux));  
	
	strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
	suma_long = strlen(comando_pet);
	strncpy(mensaje_completo + suma_long, clave_mac, strlen(clave_mac));
	suma_long = suma_long + strlen(clave_mac);
	strncpy(mensaje_completo + suma_long,";", 1); //Separador
	suma_long = suma_long + 1;
	strncpy(mensaje_completo + suma_long,"3", 1); //Indicador de relleno: metodo 3 de NO Padding
	suma_long = suma_long + 1;
	strncpy(mensaje_completo + suma_long,"#", 1); //Separador
	suma_long = suma_long + 1;
	strncpy(mensaje_completo + suma_long,"0", 1); //Indicador de hash: No hash
	suma_long = suma_long + 1;
	strncpy(mensaje_completo + suma_long,vector_inicial_aes, 32); //Vector de inicialización de 32 para AES
  suma_long = suma_long + 32;	
	sprintf(long_aux, "%06d", long_mac); // Se convierte en una cadena de 6
	strncpy(mensaje_completo + suma_long, long_aux, 6);
	suma_long = suma_long + 6;
	strncpy(mensaje_completo + suma_long, cad_datos, long_mac);
	suma_long = suma_long + long_mac;
	mensaje_completo[suma_long] = '\0'; 
	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';

	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerCMAC  20241021 v2.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	return(0);
} //FIN de fcomponerCMAC

//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE PARA ALTA DE NUEVA CLAVE                     //
//PRJ0258542 - se anade el 3º parametro para saber si la clave es TDES o AES
// Habría que afinar para dar de alta claves de otro tipo, pero de momento solo se quiere 
//		dar de alta una clave de MAC AES para poder probar el resto de cosas
//*****************************************************************************************//
int fcomponerALTA(char * cad_datos, char * comando_pet, int tipcla)
{
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerALTA  20241021 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  char mensaje_completo[1024]; 
	int suma_long = 0;
	char long_aux[6];
	printf(">>>>>>>>>>>>>>>INICIO COMPONER MENSAJE\n");
	memset(mensaje_completo, '\0', sizeof(mensaje_completo));
	memset(long_aux, '\0', sizeof(long_aux));  
	//printf("\n>>>>>>>>>>>>>>>COMPONER MAC\n");
	strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
	suma_long = strlen(comando_pet);
	//printf(" Mensaje completo 1:%s< \n", mensaje_completo); 

	if (tipcla == 8)
	{
		strncpy(mensaje_completo + suma_long,"08W", 3); //Indicador de clave AES interna de MAC B=Externa
	}
	else if (tipcla == 3)
	{
		strncpy(mensaje_completo + suma_long,"03W", 3); //Indicador de clave AES interna de bloque de PIN
	}
	else if (tipcla == 4)
	{
		strncpy(mensaje_completo + suma_long,"04W", 3); //Indicador de clave AES interna de verificacion de PIN
	}
	else if (tipcla == 2)
	{
		strncpy(mensaje_completo + suma_long,"02W", 3); //Indicador de clave AES interna de trasnporte
	}
	else if (tipcla == 5)
	{
		strncpy(mensaje_completo + suma_long,"05W", 3); //Indicador de clave AES interna de verificacion de CVV
	}
	else if (tipcla == 6)
	{
		strncpy(mensaje_completo + suma_long,"06W", 3); //Indicador de clave AES interna de EMV
	}
	else if (tipcla == 7)
	{
		strncpy(mensaje_completo + suma_long,"07W", 3); //Indicador de clave AES interna de Datos
	}
	else if (tipcla == 27)
	{
		strncpy(mensaje_completo + suma_long,"27W", 3); //Indicador de clave de importacion par TR31
	}
	else
	{
		strncpy(mensaje_completo + suma_long,"08D", 3); //Indicador de clave DES externa de MAC
	}
	
	//printf(" Mensaje completo 2:%s< \n", mensaje_completo); 
	suma_long = suma_long + 3;
	
	strncpy(mensaje_completo + suma_long,"0128", 4); //Indicador de longitud para devolver la clave
	suma_long = suma_long + 4;
	//printf(" Mensaje completo 3:%s< \n", mensaje_completo);

	mensaje_completo[suma_long] = '\0'; 

	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';

	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerALTA  20241021 v1.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	return(0);
}

//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE PARA BAJA DE CLAVES DEL HSM                  //
//PRJ0258542 - se anade el 3º parametro para saber si la clave es TDES o AES
//*****************************************************************************************//
int fcomponerBAJA(char * cad_datos, char * comando_pet, int tipcla, char * clave_baja, long long_baj)
{

  char mensaje_completo[1024]; 
	int suma_long = 0;
	char long_aux[6];
	char separa1[1];
	char separa2[2];

	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerBAJA  20241029 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
	
	printf(">>>>>>>>>>>>>>>INICIO COMPONER 0102\n");
	memset(mensaje_completo, '\0', sizeof(mensaje_completo));
	memset(long_aux, '\0', sizeof(long_aux));  

	strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
	suma_long = strlen(comando_pet);

	strncpy(mensaje_completo + suma_long, clave_baja, long_baj);
	suma_long = suma_long + long_baj;

	mensaje_completo[suma_long] = '\0'; 

	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';

	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerBAJA  20241029 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

}

//*****************************************************************************************//
// RUTINA QUE CALCULA EL KCV DE LAS CLAVES RECIBIDAS PARA INCORPORAR AL SISTEMA            //
//PRJ0258542 
//*****************************************************************************************//
int fcomponerKCV(char * cad_datos, char * comando_pet, long long_kcv)
{
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerKCV  20241029 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  char mensaje_completo[1024]; 
	int suma_long = 0;
	char long_aux[6];
	char separa1[1];
	char separa2[2];
	
	printf(">>>>>>>>>>>>>>>INICIO COMPONER 0404\n");
	memset(mensaje_completo, '\0', sizeof(mensaje_completo));
	memset(long_aux, '\0', sizeof(long_aux));  
	
	strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
	suma_long = strlen(comando_pet);

	strncpy(mensaje_completo + suma_long, cad_datos, long_kcv);
	suma_long = suma_long + long_kcv;
	
	//printf(" Mensaje completo 1 :%s< \n", mensaje_completo); 

	mensaje_completo[suma_long] = '\0'; 

	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';

	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerKCV  20241029 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	return(0);
}

//*****************************************************************************************//
// RUTINA QUE CALCULA EL OFFSET PARTIENDO DEL BLOQUE DE PIN RECIBIDO                      //
//PRJ0258542 
//*****************************************************************************************//
int fcomponerOFF(char * cad_datos, char * comando_pet)
{
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerOFF  20241029 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

  char mensaje_completo[1024]; 
	int suma_long = 0;
	char long_aux[6];
	char separa1[1];
	char separa2[2];
	
	printf(">>>>>>>>>>>>>>>INICIO COMPONER 0618\n");
	memset(mensaje_completo, '\0', sizeof(mensaje_completo));
	memset(long_aux, '\0', sizeof(long_aux));  
	
	strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
	suma_long = strlen(comando_pet);

	strncpy(mensaje_completo + suma_long, cad_datos, strlen(cad_datos));
	suma_long = suma_long + strlen(cad_datos);
	
	//printf(" Mensaje completo 1 :%s< \n", mensaje_completo); 

	mensaje_completo[suma_long] = '\0'; 

	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';

	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerOFF  20241029 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	return(0);
}

//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DE LA CLAVE EN FORMATO TR31 PARA ENVIAR A REDSYS         //
// PRJ0258542 
//*****************************************************************************************//
int fcomponerClaveTR(char * cad_datos, char * comando_pet, int tipcla, char * clave_imp, long long_imp)
{
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerClaveTR 20241121 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
  printf("cad_datos   >%.50s<\n", cad_datos);
  printf("comando_pet >%.7s<\n", comando_pet);
  printf("tipcla      >%d<\n", tipcla);
  printf("clave_imp   >%.*s<\n", long_imp, clave_imp);
  printf("long_imp      >%d<\n", long_imp);

  char mensaje_completo[1024]; 
	int suma_long = 0;
	char long_aux[6];
	
	//printf(">>>>>>>>>>>>>>>INICIO COMPONER 0304\n");
	strncpy(mensaje_completo, comando_pet,7);
	suma_long = 7;
	strncpy(mensaje_completo + suma_long, clave_imp, long_imp);
	suma_long = suma_long + long_imp;
	strncpy(mensaje_completo + suma_long, cad_datos, strlen(cad_datos));
	suma_long = suma_long + strlen(cad_datos);
	/*Cadena que se monta segun Utimaco
		D = Indica clave AES que se va a proteger
			D0 = Se exporta una clave para datos - 15 y 16 - ZEK
			K1 = Se exporta una clave de transporte - 21 - ZMK
			P0 = Se exporta una clave de PIN - 11 y 12 - ZPK
			M6 = Se exporta una clave de MAC - 13, 14 y 22 - ZAK y ZIK
		A = Indica algoritmo AES
		E = Solamente cifrado
		00 = Version de la clave
		E = Exportabilidad
		00 = No hay bloques opcionales
	*/
	strncpy(mensaje_completo + suma_long, "D",1);
	suma_long = suma_long + 1;
	if (tipcla == 11 || tipcla == 12)
	{
		strncpy(mensaje_completo + suma_long, "P0",2);
	}
	if (tipcla == 13 || tipcla == 14 || tipcla == 22)
	{
		strncpy(mensaje_completo + suma_long, "M6",2);
	}
	if (tipcla == 15 || tipcla == 16)
	{
		strncpy(mensaje_completo + suma_long, "D0",2);
	}
	if (tipcla == 21)
	{
		strncpy(mensaje_completo + suma_long, "K1",2);
	}
	suma_long = suma_long + 2;
	strncpy(mensaje_completo + suma_long, "AE00E00",7);
	suma_long = suma_long + 7;
	mensaje_completo[suma_long] = '\0';

	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';
	
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerClaveTR 20241121 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
  printf("cad_datos   >%.*s<\n", strlen(cad_datos), cad_datos);

	return(0);
}
//PRJ0258542 - FIN

//PRJ0258542 - INI Fase 3
//*****************************************************************************************//
// RUTINA QUE HACE LA COMPOSICION DEL MENSAJE DE IMPORTAR CLAVES TR-31                     //
//     Con el parametro modo se le dice si va a ser i = importacion interna                //
//                                                  e = importacion externa                //
//PRJ0258542                                      este ultimo es para ver si vale el KCV   //
//*****************************************************************************************//
int fcomponerImpor(char * cad_datos, char * comando_pet, char * modo, int tipcla, char * clave_imp, long long_imp)
{
	fechahorasys(&tm);
	printf(" ** ** ** Entrando rutina fcomponerImpor  20241027 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);
  //printf("cad_datos   >%.128s<\n", cad_datos);
  //printf("comando_pet >%.7s<\n", comando_pet);
  //printf("tipcla      >%d<\n", tipcla);
  //printf("clave_imp   >%.150s<\n", clave_imp);
  //printf("long_imp    >%d<\n", long_imp);

  char mensaje_completo[1024]; 
	long suma_long = 0;
	char long_aux[6];
	char separa1[1];
	char separa2[2];
	
	//separa1 = '0x3B';
	//separa2 = '0x23';
	
	printf(">>>>>>>>>>>>>>>INICIO COMPONER 0206\n");
	memset(mensaje_completo, '\0', sizeof(mensaje_completo));
	memset(long_aux, '\0', sizeof(long_aux));  
	
	strncpy(mensaje_completo,comando_pet,strlen(comando_pet));
	suma_long = strlen(comando_pet);

	if (strcmp(modo, "i") == 0)
	{
		strncpy(mensaje_completo + suma_long,"1", 1); //Indicador de tipo = 1 - interna
	}
	if (strcmp(modo, "e") == 0)
	{
		strncpy(mensaje_completo + suma_long,"0", 1); //Indicador de tipo = 0 - externa
	}
	suma_long = suma_long + 1;


	//Dependiendo del tipo de clave se manda uno u otro valor
	if (tipcla == 8)
	{
		strncpy(mensaje_completo + suma_long,"08", 2); //Indicador de clave AES interna de MAC B=Externa
	}
	else if (tipcla == 3)
	{
		strncpy(mensaje_completo + suma_long,"03", 2); //Indicador de clave AES interna de bloque de PIN
	}
	else if (tipcla == 4)
	{
		strncpy(mensaje_completo + suma_long,"04", 2); //Indicador de clave AES interna de verificacion de PIN
	}
	else if (tipcla == 2)
	{
		//strncpy(mensaje_completo + suma_long,"02", 2); //Indicador de clave AES interna de transporte
		strncpy(mensaje_completo + suma_long,"27", 2); //Indicador de clave AES interna de transporte - para cumplir PCI-PIN tiene que ser LMK27 - 26/03/2025
	}
	else if (tipcla == 5)
	{
		strncpy(mensaje_completo + suma_long,"05", 2); //Indicador de clave AES interna de verificacion de CVV
	}
	else if (tipcla == 6)
	{
		strncpy(mensaje_completo + suma_long,"06", 2); //Indicador de clave AES interna de EMV
	}
	else if (tipcla == 7)
	{
		strncpy(mensaje_completo + suma_long,"07", 2); //Indicador de clave AES interna de Datos
	}
	else if (tipcla == 27)
	{
		strncpy(mensaje_completo + suma_long,"27", 2); //Indicador de clave para importacion de claves TR31
	}
	suma_long = suma_long + 2;
	
	strncpy(mensaje_completo + suma_long, "1", 1); //Indicador de contenedor, version 1
	suma_long = suma_long + 1;
	
	strncpy(mensaje_completo + suma_long, clave_imp, strlen(clave_imp));
	suma_long = suma_long + strlen(clave_imp);

	strncpy(mensaje_completo + suma_long, cad_datos, long_imp); //Clave TR31 que se va a importar
	suma_long = suma_long + long_imp;
	
	mensaje_completo[suma_long] = '\0'; 

	sprintf(long_aux, "%06d", suma_long); 
	long_aux[6] = '\0';
	strncpy(cad_datos,long_aux,6);
	strncpy(cad_datos + 6, mensaje_completo,suma_long);
	cad_datos[suma_long + 6] = '\0';
	long_imp = 0;
	long_imp = suma_long + 6;
	fechahorasys(&tm);
	printf(" ** ** ** Saliendo rutina fcomponerImpor  20241027 v0.0 %02d/%02d/%d %02d:%02d:%02d:%03d \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);

	return(0);
}

//PRJ0258542 - FIN Fase 3