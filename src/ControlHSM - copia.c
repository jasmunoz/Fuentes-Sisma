/*******************
// Proceso para comprobar la conexión con el HSM principal, conecta y lanza test, si es ok, termina = 0
// si no es ok, conecta y lanza test al HSM secundario, si es OK, termina =0
// si no consigue conectar con el secundario o no permite cambiarlos termina 2 (para avisar error)
// si conecta con el secundario y los cambia termina 1 (para avisar revisión)
*******************/
//****************coger la fecha de sistema si lo quieren datar por dia y fecha si no solo la fecha sin hora
//****************en todo caso segun email de Juanmi mar 08/10/2019 14:06 ellos se encargan del borrado y no será necesario que sea por hora, se hara diario 
//****************se lanzara cada 10 minutos aunque pregunta si es parametrizable
//****************se incluira la hora de conexion y la IP conectada
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
//int conecunhsm(char* codent, NPSDHSM reg_hsm, int hsm);

int lanzatest(int conex, char * entidad);
int escribetraza(char *cadena);
void desconexhsm(int sock);
int fCambiaIP();
int fConexIP(int iHSM);
char Entrada_dll[4096];
char Salida_dll[4096];
char codResp[3];
//fichero de trazas
FILE * pFile;

typedef struct sfdssl {         /* estructura conjunta para socket y socket seguro */
 	int sfd_ssl;                   
 	SSL * ssl_ssl;                               
} sfdssl;

struct sfdssl stsocketssl;
struct sfdssl * sockssl;


int main(int argc, char *argv[])
{

  int retorno = 0;
  int socket_con = 0;
  int estado_sal = 0;
  NPSDHSM registro;
  char clave1[151];
  char clave2[151];
  char cadena[7];
  int numHSM = 0;
  int nConectado = 0;
  char fichero[200];
  char printable[4000];
  char ip_conex1[15];
  char ip_conex2[15];
	char tiempo[4];
	char puerto[4];
	char cabecera[4];
	NPSDHSM reg_hsm;

  // Montamos el nombre del fichero
  char* HOME;
	HOME = getenv ("SF_HOME");

	tm_dec tm;
	fechorsys(&tm);
  sprintf(fichero, "%s%s/ControlHSM_%d%02d%02d_%02d.traza", HOME, DIRTRAZA,tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour);
  //freopen(fichero, "w", stdout); //w = modo de escritura, uno nuevo cada vez
  freopen(fichero, "a", stdout); //a = modo de escritura y lectura, abre el creado o crea un nuevo

	fprintf(stdout," \n");
	escribetraza("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
	escribetraza("-------------------");
	escribetraza("Arrancar ControlHSM");
  struct hostent *p = gethostbyname("localhost");
   
	memset(registro.dhcodent, ' ', sizeof(NPSDHSM)); 

	escribetraza("Obtencion datos HSMs");
	
	retorno = tratar_fich("NPTDHSM", IGUAL, &reg_hsm, "0001");
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
		escribetraza(printable);
		strncpy(printable, " ", 30);
	}
	
	strncpy(cadena, reg_hsm.dhcodent, 4);
  strncpy(ip_conex1, reg_hsm.dhnumip, 15);
  strncpy(ip_conex2, reg_hsm.dhnumip2, 15);
	strncpy(tiempo, reg_hsm.dhtiempo, 4);
	strncpy(puerto, reg_hsm.dhpuerto, 4);
	strncpy(cabecera, reg_hsm.dhcabece, 4);

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
		retorno = lanzatest(socket_con, reg_hsm.dhcodent);
		if (retorno == 0)
		{
			strncpy(cadena + 4, "101", 3);
			cadena[7] = '\0';
		}
		else
		{
			strncpy(cadena + 4, "100", 3);
			cadena[7] = '\0';
			estado_sal = 1;
		}
	}
	retorno = tratar_fich("NPTDHSM", ESCRIBIR, cadena, "0001");
	escribetraza("-- Salida tratar_fich --");
	
	escribetraza("-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-");
	
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
		retorno = lanzatest(socket_con, reg_hsm.dhcodent);
		if (retorno == 0)
		{
			strncpy(cadena + 4, "201", 3);
			cadena[7] = '\0';
		}
		else
		{
			strncpy(cadena + 4, "200", 3);
			cadena[7] = '\0';
			estado_sal = 1;
		}
	}
	retorno = tratar_fich("NPTDHSM", ESCRIBIR, cadena, "0001");
	escribetraza("-- Salida tratar_fich --");

	escribetraza("Salida ControlHSM - fin de programa");
	escribetraza("-----------------------------------");
	return (estado_sal);
}//fin main

int lanzatest(int conex, char * entidad)
{
	int salida = 0;
	char clave[151];
	char Entrada_dll[4096];
	char Salida_dll[4096];
	long longE = 0;
	long retorno = 6;
	int socket_on = 0;
	
	escribetraza("Entrada a lanzatest ");
	sockssl = &stsocketssl;
	socket_on = conex;
	//sockssl->sfd_ssl = socket_on;
	//sprintf(sockssl->sfd_ssl, "%08d", socket_on);
	strncpy(clave,entidad,4);
	strncpy(Entrada_dll," ",1);
	memset(Salida_dll,' ', sizeof(Salida_dll));
	longE = strlen(Entrada_dll);
	//escribetraza("***** Va a conexhsm_1");
	//conexhsm_1(entidad,clave, sockssl, codResp, 1);
	printf("Valor sockssl->sfd_ssl >%i<\n",sockssl->sfd_ssl);
	//escribetraza("***** Vuelve de conexhsm_1");
	//if (!strcmp(codResp, "01")) 
	//{
		retorno = manejhsm(clave, "01", 0, Entrada_dll, longE, sockssl, Salida_dll); 
	//}

	if (retorno == 6)
	{
		escribetraza("Test lanzado correcto");
		retorno = 0;
	}
	else
	{
		escribetraza("Test lanzado con error");
		retorno = -1;
	}
	
	escribetraza("Salida de lanzatest");
	return(retorno);
} //fin lanzatest

int escribetraza(char * cadena)
{
	tm_dec tm;
	fechorsys(&tm);
	fprintf(stdout,"%02d/%02d/%d %02d:%02d:%02d:%03d --> %.*s \n",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls, strlen(cadena), cadena);
} //fin escribetraza

