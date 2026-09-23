/*
 PROGRAMA:  BATERIA
 DESCRIPCION: Este programa pregunta al usuario que operaci�n se quiere hacer, la convierte
              al formato correcto, abre un socket para mandarlo y espera la respuesta.
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>

#include "datagen.h"
#include "nptinci.h"
#include "deffich.h"
#include "ftradred.h"
#include "cforitno.h"
#include "svm.h"
#include "cnetplus.h"
#include "procesos.h"

#define LONMSG_IN   4096
#define LONMSG_OUT  4096
#define PRUCARGA

#define PORTNUM atoi((psvm+posic_subsis)->str_svm.svmpars.papuerto)

/***  --------- prototipos de funciones  ----------------  ***/
void fechorsys(struct tm_dec *tmPtr);
char *itoa(int numero);
//int     fcarga_tablas(char * red);

int ftradred(char usotrad,int sentido,int suba_iso,int *long_iso,
             char *mensaje_puc,formato_interno *mensaje_netplus,
             int *error_code,int *error_bit);
int sfd;
int retorno;
SVM *psvm;

int llenar_mensaje_con_fichero(char mensaje[]);

char coderror[4];

// datos de entrada
int opcion;
char op[2];
char tipo[5];
char imp[13]; 
// PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de tarj
char tarj[20];
char DATPREV[36];
char digi12[3];
char fmt203[42];

int enviar = 59; //59 es el tamano por defecto

char binario[2];

formato_interno fiforitn;
mensaje_nuc  msgnuc;
NPSINCI reginci;

/******* para obtener la clave de la tabla NPTSOCK ***/
struct sockaddr_in ser_addr;

// funcion igual que en simulaRed.c
int main(int argc,char *argv[]) {
   socklen_t Long_Cliente;
   struct sockaddr cli_addr;
   int hfd, iskopt;  
   char FichTraza[100];
   char otraop[3];
   int retorno=0;

   signal(SIGCLD,SIG_IGN); //sinonimo de SIGCHLD en Linux
   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);
   signal(SIGALRM,SIG_IGN);

  //rellenamos la estructura
  //mejor con htons  para que vaya protegido
  memset(&ser_addr, 0 , sizeof(ser_addr));
  printf("argc <%d>\n",argc);
  if (argc < 2)
  {
     printf("Uso : simulaMensaje <Emisor> (I Inversis, S Sefide, A AndBank\n");
     exit(0);
  }

  if (strncmp(argv[1], "I",1) == 0)
     ser_addr.sin_port = htons(20324);// inversis
  else
     if (strncmp(argv[1], "S",1) == 0)
       ser_addr.sin_port = htons(27054); // sefide
     else
       if (strncmp(argv[1], "A",1) == 0)
         ser_addr.sin_port = htons(25444); // anbank
       else
         exit(1);


  // ser_addr.sin_port = htons(20324);// inversis
  //ser_addr.sin_port = htons(27054); // sefide
   // ser_addr.sin_port = htons(25444); // anbank
    ser_addr.sin_family = AF_INET;
    //  ser_addr.sin_family=PF_INET; 
    ser_addr.sin_addr.s_addr =INADDR_ANY;

 
  if ((sfd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
        printf("Error al crear socket \n");
        return(-1);
  }
  //hacemos el bind
 iskopt=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&retorno,sizeof(retorno)); //sets a socket option
   if(iskopt)
      fprintf(stderr,"Retorno setsockopt=%d\n",iskopt);

  if (bind (sfd,(struct sockaddr*)&ser_addr, sizeof (ser_addr)) == -1){
        printf("error al hacer el bind\n");
		printf("con error %d\n", errno);
		close (sfd);
        return(-1);
  }
  printf("socket conectado es %d\n", sfd);

    //creamos el listen con n elementos. Elementos que pueden estar en cola a la vez.
   listen (sfd, 5);
   
    //tratamiento de operaciones
      printf("espero cliente\n");

      Long_Cliente = sizeof (cli_addr);
      
      printf("espero cliente despues long_client \n");
		
      if ((hfd = accept (sfd, &cli_addr, &Long_Cliente)) == -1){
            printf("error en el accept\n");
            printf("con error %d\n", errno);
            close(sfd);		 
            return (-1);
      }
        
      printf("despues del accept\n");
//      while (!retorno)
      while (1)
      {
//         retorno=llenar_mensaje_con_fichero(mensaje_puc);
         printf("Antes de la llamada a llenar_mensaje_con_fichero\n");
         if (llenar_mensaje_con_fichero(mensaje_puc)==-1)
            break;
         
         printf("antes de EnviaSock\n");
         retorno = EnviaSock(hfd,0,mensaje_puc,enviar+2);
         if (retorno == -1){
            printf("error al enviar datos\n");
            strcpy(reginci.innomfic,"Leer_q");
            strncpy(reginci.incoderr,itoa(retorno),8); 
         } else {
             printf("Enviado por el socket el mensaje >%.200s<\n", mensaje_puc);
	     }
      }
	
  close(sfd);		 
  sfd = 0;
  return(0);  
}


// funcion igual que en lanzaOpe.c
int llenar_mensaje_con_fichero(char mensaje[]){
    FILE * fcaptura = NULL;
    int segundos=0;
    int numOpSeguidas=0;
    int subsisOri=0;
    char nomFich[1024];
    char rutaFich[1024];
     
    printf("Nombre del fichero: (en /sisma/dat)\n"); 
    scanf("%s", &nomFich); 
    printf("nombre de fichero leido <%s>\n", nomFich);
    printf("Subsistema origen 3002 o 3004\n");
    scanf("%d", &subsisOri); 

	
    //    printf("numero de operaciones seguidas antes de cada espera:\n");
    //    scanf("%d", &numOpSeguidas); 
    numOpSeguidas=1;

    //    printf("segundos de espera entre grupos de operaciones:\n");
    //    scanf("%d", &segundos); 
    segundos = 0;

	
    if(strlen(nomFich) >= 1024){
	   printf("nombre de fichero demasiado largo\n");
	   return(-1);
	}
    if(strlen(nomFich) < 3){
	   printf("nombre de fichero no valido\n");
	   return(-1);
	}

// incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

    //////////////// APERTURA DEL FICHERO /////////////////	
    sprintf(rutaFich,"%s%s/%s" ,HOME,DIRDATOS,nomFich);
	printf("lanzaOpe del fichero >%s<\n",rutaFich);
	
    fcaptura = fopen(rutaFich, "r"); 
    if(!fcaptura){
        printf("error en apertura del fichero\n");
        return(-1);
    }

    fseek(fcaptura, 0, SEEK_END);
    int longitud_archivo = ftell(fcaptura);
    fseek(fcaptura, 0, SEEK_SET);
    //longitud_archivo no deberia ser mayor de 2000
    fread(mensaje + 2, 1, longitud_archivo, fcaptura);

    //los 2 primeros bytes indican la longitud del mensaje
    enviar = longitud_archivo;
    unsigned short longitud_archivo_short = (unsigned short) longitud_archivo;

    printf("longitud_archivo %d\n", longitud_archivo);
    printf("enviar %d\n", enviar);
    printf("longitud_archivo_short %u\n", longitud_archivo_short);
    memcpy(binario, (unsigned char*) &longitud_archivo_short, 2);
    
    mensaje[0]=binario[1];
    mensaje[1]=binario[0];
    
    printf(" mensaje[0] %x\n", mensaje[0]);
    printf(" mensaje[1] %x\n", mensaje[1]); 

    if(fcaptura)
        fclose(fcaptura);
	   
	return(0);
}
