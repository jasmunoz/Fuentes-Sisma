/*******************
// Proceso para lanzar comandos sobre el HSM principal, el secundario o los dos
// Al llamarlo se le tiene que pasar uno de los tres valores indicados, 1 o 2 o 3
// Si se le pasa 1 lo hace sobre el principal
// Si se le pasa 2 lo hace sobre el secundario
// Si se le pasa 3 lo hace sobre los dos
// Arrancado lo que hace es pedir el nombre del fichero que contiene la cadena a enviar al HSM
// Deberá estar en un fichero plano en la ruta /sisma/dat
*******************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/time.h> 
#include <time.h>
#include <netdb.h>
#include <errno.h>
#include "utilidades.h" 
//#include "cnetplus.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>

long manejhsm(char * clavehsm, char* comandohsm, int tipcla, char * datoshsm, long NumByte, struct sfdssl * sockssl, char * prespuestahsm); //PRJ0258542 - Se pone el tipo de clave para saber si es AES o TDES y cambia de long a int el retorno
void manejhsm_M(char * comando, char * clave1, char * clave2, char * clave3, char * clave4, char * redes, char * offset, char * pin, char * pan, char * vid, char * prespuestahsm, char * codResp); //PRJ0378949 - Adaptacion a PCI
int lanzatest(int conex, char * entidad);
int escribetraza(char *cadena);
void desconexhsm(int sock);

int fCambiaIP();
int fConexIP(int iHSM);
char Entrada_dll[4096];
char Salida_dll[4096];
char codResp[3];
char nomFich[1024];
char rutaFich[1024];
char mensaje_hsm[4000];
char longitudComando[6];
int LongitudMensaje;
int Posicion;
FILE * fcaptura = NULL;
//fichero de trazas
FILE * pFile;

typedef struct sfdssl {         /* estructura conjunta para socket y socket seguro */
 	int sfd_ssl;                   
 	SSL * ssl_ssl;                               
} sfdssl;

struct sfdssl stsocketssl;
struct sfdssl * sockssl;
char fichero[200];
tm_dec tm;

int main(int argc, char *argv[])
{

  int retorno = 0;
  long sale = 0;
  int socket_con = 0;
  int estado_sal = 0;
  NPSDHSM registro;
  char clave[150];
  char clave1[151];
  char clave2[151];
  char cadena[7];
  int numHSM = 0;
  int nConectado = 0;
  char printable[4000];
  char ip_conex1[15];
  char ip_conex2[15];
	char tiempo[4];
	char puerto[4];
	char cabecera[4];
	char lanzar[1];
	int segundos, minutos;
	NPSDHSM reg_hsm;

  // Montamos el nombre del fichero
  char* HOME;
	HOME = getenv ("SF_HOME");

	if (argc != 2)
	{
		printf("EnviaComandosHSM, introducir 1 para HSM1, 2 para HSM2 o 3 para los dos HSM. 4 simular llamada \n");
		exit(-1);
	} 
	else 
	{
		strcpy(lanzar,argv[1]);
		lanzar[1] = '\0';
	}
	
	printf("Introduzca el nombre de fichero: ");
	scanf("%s", &nomFich);


	fechorsys(&tm);
  sprintf(fichero, "%s%s/EnviaComandosHSM_%d%02d%02d_%02d.traza", HOME, DIRTRAZA,tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour);
  //freopen(fichero, "w", stdout); //w = modo de escritura, uno nuevo cada vez
  freopen(fichero, "a", stdout); //a = modo de escritura y lectura, abre el creado o crea un nuevo
  
	fprintf(stdout," \n");
	fprintf(stdout,"* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");

	memset(registro.dhcodent, ' ', sizeof(NPSDHSM)); 
	strncpy(printable,"Valor lanzar >", 14);
	strncpy(printable + 14,lanzar,1);
	strncpy(printable + 15, "<", 1);
	escribetraza(printable);
	if (strcmp(lanzar,"4") == 0)
	{
		char prespuestahsm[1500];
		prespuestahsm[1500]='\0';
		char codResp[3];
		codResp[2]='\0';
		// Llamada a comando 0619 con el 39 funciona - 12/03/2026
		if (strcmp(lanzar,"4") == 0)
		{
			strncpy(printable,"Entrada al modulo manejhsm_M",29);
			escribetraza(printable);
			
			freopen(fichero, "a", stdout);
			//Funciona, llamada 36 18/03/2026
			//manejhsm_M("36", "4C58CB006AEC1480", "BPIN_TRANS_INTERMIG_SFOWN", "BPIN_TRANS_MIGRA_SFOWN", " ", "0000", "0000", "0000", "5299330016785008", " ", prespuestahsm, codResp);
			//Funciona, llamada 37 18/03/2026
			//manejhsm_M("37", "PIN_VERIF_INTER_SFOWN", "BPIN_TRANS_IDEMIA_SFOWN", " ", " ", "0000", "0009", "0000", "5299330016785008", " ", prespuestahsm, codResp);
			//Funciona, llamada 38 18/03/2026
			//manejhsm_M("38", "BPIN_TRANS_MGNOPCI_SFOWN", " ", " ", " ", "0000", "0000", "5231", "5299330016785008", " ", prespuestahsm, codResp);
			//Funciona, llamada 39 12/03/2026
			//manejhsm_M("39", "PIN_VERIF_INTER_SFOWN", "BPIN_TRANS_INTERMIG_SFOWN", "BPIN_TRANS_IDEMIA_SFOWN", " ", "0000", "0437", "0000", "5299330020746012", " ", prespuestahsm, codResp);
			//manejhsm_M("45", "PIN_VERIF_INTER_SFOWN", "BPIN_TRANS_INTERMIG_SFOWN", " ", " ", "0000", "0437", "0000", "5299330020746012", " ", prespuestahsm, codResp);
			//Funciona, llamada 40 20/03/2026
			//manejhsm_M("40", "PIN_VERIF_INTER_SFOWN", "BPIN_TRANS_MGNOPCI_SFOWN", "BPIN_TRANS_INTERMIG_SFOWN", "BPIN_TRANS_MIGRA_SFOWN", "0000", "0000", "2438", "5299330020746012", " ", prespuestahsm, codResp);
			//Funciona, llamada 40 20/03/2026
			//manejhsm_M("41", "PIN_VERIF_INTER_SFOWN", "BPIN_TRANS_MIGRA_SFOWN", " ", " ", "0000", "0000", "0000", "5299330016785008", " ", prespuestahsm, codResp);
			//Funciona, llamada 42 13/04/2026 //Calculo con MAC
			//manejhsm_M("42", "PIN_VERIF_INTER_SFOWN", "LMK4_REDSYS_SFOWN", "LMK7_REDSYS_SFOWN", "LMK8_REDSYS_SFOWN", "0000", "0000", "0000", "000016966", " ", prespuestahsm, codResp);
			//Funciona, llamada 43 13/04/2026 //Calculo con CMAC
			//manejhsm_M("43", "PIN_VERIF_INTER_SFOWN", "LMK4_REDSYS_SFOWN", "LMK7_REDSYS_SFOWN", "LMKXX_REDSYS_SFOWN", "0000", "0000", "0000", "000016980", " ", prespuestahsm, codResp);
			manejhsm_M("43", "PIN_VERIF_INTER_SFOWN", "LMK4_REDSYS_SFOWN", "LMK7_REDSYS_SFOWN", "LMK42_REDSYS_SFOWN", "0000", "0000", "0000", "000020584", " ", prespuestahsm, codResp);
			//manejhsm_M("43", "PIN_VERIF_INTER_SFAND", "LMK4_REDSYS_SFAND", "LMK7_REDSYS_SFAND", "LMK42_REDSYS_SFAND", "0000", "0000", "0000", "000020622", " ", prespuestahsm, codResp);
			//Funciona, llamada 44 xx/04/2026 //Calculo de un nuevo offset para obtener el mismo PIN
			//manejhsm_M("44", "PIN_VERIF_INTER_SFOWN", " ", " ", " ", "0000", "0125", "0000", "5299330018484006", "5299330017079005", prespuestahsm, codResp);
			fclose(stdout);
			
			strncpy(printable,"Salida del modulo manejhsm_M -",30);
			strncpy(printable + 30, codResp, 2);
			strncpy(printable + 32, "-", 1);
			strncpy(printable + 33, prespuestahsm, 32);
			strncpy(printable + 65, "-", 1);
			escribetraza(printable);
			return(0);
		}
	}
	
	escribetraza("Obtencion datos HSMs");
	freopen(fichero, "a", stdout);
	retorno = tratar_fich("NPTDHSM", IGUAL, &reg_hsm, "0001");
	fclose(stdout);
	if (retorno == 0)
	{
		escribetraza("** Datos de configuracion HSMs **");
		strncpy(printable,"*  Entidad    >", 15);
		strncpy(printable+15,reg_hsm.dhcodent,4);
		strncpy(printable+19,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  Tiempo     >", 15);
		strncpy(printable+15,reg_hsm.dhtiempo,4);
		strncpy(printable+19,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  Cabecera   >", 15);
		strncpy(printable+15,reg_hsm.dhcabece,4);
		strncpy(printable+19,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  IP HSM1    >", 15);
		strncpy(printable+15,reg_hsm.dhnumip,15);
		strncpy(printable+30,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  Puerto     >", 15);
		strncpy(printable+15,reg_hsm.dhpuerto,4);
		strncpy(printable+19,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  Produccion >", 15);
		strncpy(printable+15,reg_hsm.dhprodt,1);
		strncpy(printable+16,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  IP HSM2    >", 15);
		strncpy(printable+15,reg_hsm.dhnumip2,15);
		strncpy(printable+30,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  Semaf HSM1 >", 15);
		strncpy(printable+15,reg_hsm.dhsemafhsm1,1);
		strncpy(printable+16,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  Semaf HSM2 >", 15);
		strncpy(printable+15,reg_hsm.dhsemafhsm2,1);
		strncpy(printable+16,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 30);
		strncpy(printable,"*  Renovacion >", 15);
		strncpy(printable+15,reg_hsm.dhparenov,2);
		strncpy(printable+17,"<",1);
		strncpy(printable+18," Conectar con HSM >",19);
		strncpy(printable+37,lanzar,1);
		strncpy(printable+38,"<",1);
		escribetraza(printable);
		strncpy(printable, " ", 40);
	}
	else
	{
		escribetraza("Error al obtener los datos de los HSM");
		exit(-1);
	}
	
	strncpy(printable, "Nombre de fichero leido >",25);
  strncpy(printable+25,nomFich,strlen(nomFich));
	strncpy(printable+25+strlen(nomFich), "<",1);
  escribetraza(printable);
	if(strlen(nomFich) >= 1024)
	{
  	escribetraza("ERROR, nombre de fichero demasiado largo");
   	return(-1);
  }
  if(strlen(nomFich) < 3)
  {
   	escribetraza("ERROR, nombre de fichero no valido");
   	return(-1);
  }
  sprintf(rutaFich,"%s%s%s" ,HOME, DIRDATOS,nomFich);
	freopen(fichero, "a", stdout);
  fprintf(stdout,"Ruta del fichero: >%s<\n", rutaFich);
	fclose(stdout);
  fcaptura = fopen(rutaFich, "r"); 
  if(!fcaptura)
  {
  	escribetraza("ERROR en apertura del fichero");
    return(-1);
  }
  escribetraza("----- Empieza a leer fichero -----");
  memset(mensaje_hsm, 0, 4000);
 	LongitudMensaje = 0;
 	strncpy(longitudComando,"000000",6);
 	Posicion = 0;
	freopen(fichero, "a", stdout);
 	while (Posicion < 1)
 	{
		if (fread(mensaje_hsm + LongitudMensaje, 1, 1, fcaptura) == 0)
   	{
   		Posicion++;
   	}
   	if (strcmp(mensaje_hsm + LongitudMensaje, " ") == 0)
   	{
   		Posicion++;
   	}
   	else
   	{
   		LongitudMensaje++;
   	}
	} 
  fprintf(stdout,"Linea leida >%.*s< de longitud %d\n",LongitudMensaje,mensaje_hsm,LongitudMensaje);
	strncpy(longitudComando,mensaje_hsm,6);
	LongitudMensaje = atoi(longitudComando) + 6;
	fprintf(stdout,"Linea leida >%.*s< de longitud %d\n",LongitudMensaje,mensaje_hsm,LongitudMensaje);
	fclose(stdout);
	
  struct hostent *p = gethostbyname("localhost");
   
	strncpy(cadena, reg_hsm.dhcodent, 4);
  strncpy(ip_conex1, reg_hsm.dhnumip, 15);
  strncpy(ip_conex2, reg_hsm.dhnumip2, 15);
	strncpy(tiempo, reg_hsm.dhtiempo, 4);
	strncpy(puerto, reg_hsm.dhpuerto, 4);
	strncpy(cabecera, reg_hsm.dhcabece, 4);
	strncpy(printable, "                                                            ",60);
	escribetraza("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-");
	if (strcmp(lanzar,"1") == 0)
	{
		strncpy(printable,"Valor lanza1 >", 14);
		strncpy(printable + 14,lanzar,1);
		strncpy(printable + 15, "<", 1);
		escribetraza(printable);
	}
	if (strcmp(lanzar,"2") == 0)
	{
		strncpy(printable,"Valor lanza2 >", 14);
		strncpy(printable + 14,lanzar,1);
		strncpy(printable + 15, "<", 1);
		escribetraza(printable);
	}
	if (strcmp(lanzar,"3") == 0)
	{
		strncpy(printable,"Valor lanza3 >", 14);
		strncpy(printable + 14,lanzar,1);
		strncpy(printable + 15, "<", 1);
		escribetraza(printable);
	}
	
	if (strcmp(lanzar,"1") == 0 || strcmp(lanzar,"3") == 0)
	{
		escribetraza("* * * Realiza conexion con el HSM 1 * * *");
		sockssl = &stsocketssl;
		conexhsm_1(ip_conex1, puerto, tiempo, cabecera, sockssl, codResp, 1);
		if (strcmp(codResp, "01") != 0) 
		{
			escribetraza("FALLO CONEXION - Salida fConexHSM HSM 1, valor <0");
			strncpy(cadena + 4, "100", 3);
			cadena[7] = '\0';
			estado_sal = 1;
		}
		else
		{
			escribetraza("OK CONEXION - Salida fConexHSM HSM 1, valor >0");
			socket_con = retorno;
			retorno = 0;
			if (retorno == 0)
			{
				escribetraza("Va lanzar mensaje contra HSM 1");
				freopen(fichero, "a", stdout);
				//printf("*** manejhsm 2 *** *%s*%s*%i*%s*\n", clave, mensaje_hsm, LongitudMensaje, Salida_dll);
				//fflush(stdout);
				sale = manejhsm(clave, "50", 0, mensaje_hsm, LongitudMensaje, sockssl, Salida_dll);
				fprintf(stdout,"Retorno >%d< cadena salida >%s<\n",sale,Salida_dll);
				fclose(stdout);
			}
			else
			{
				escribetraza("* * * Error al lanzar el test * * *");
			}
		}
	}
	
	escribetraza("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-");
	strncpy(printable,"Valor lanzar >", 14);
	strncpy(printable + 14,lanzar,1);
	strncpy(printable + 15, "<", 1);
	escribetraza(printable);
	
	if (strcmp(lanzar,"2") == 0 || strcmp(lanzar,"3") == 0)
	{
		escribetraza("* * * Realiza conexion con el HSM 2 * * *");
		sockssl = &stsocketssl;
		conexhsm_1(ip_conex2, puerto, tiempo, cabecera, sockssl, codResp, 2);
		if (strcmp(codResp, "01") != 0) 
		{
			escribetraza("FALLO CONEXION - Salida fConexIp HSM 2, valor <0");
			strncpy(cadena + 4, "200", 3);
			cadena[7] = '\0';
			estado_sal = 1;
		}
		else
		{
			escribetraza("OK CONEXION - Salida fConexIp HSM 2, valor >0");
			socket_con = retorno;
			retorno = 0;
			if (retorno == 0)
			{
				escribetraza("Va lanzar mensaje contra HSM 2");
				freopen(fichero, "a", stdout);
				sale = manejhsm(clave, "50", 0, mensaje_hsm, LongitudMensaje, sockssl, Salida_dll);
				fprintf(stdout,"Retorno >%d< cadena salida >%s<\n",sale,Salida_dll);
				fclose(stdout);
			}
			else
			{
				escribetraza("* * * Error al lanzar el test * * *");
			}
		}
	}	
	escribetraza("Salida ControlHSM - fin de programa");
	escribetraza("-----------------------------------");
	return (estado_sal);
}//fin main

int escribetraza(char * cadena)
{
	freopen(fichero, "a", stdout);
	fechorsys(&tm);
	fprintf(stdout,"%02d/%02d/%d %02d:%02d:%02d:%03d --> %.*s \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls, strlen(cadena), cadena);
	fclose(stdout);
} //fin escribetraza

