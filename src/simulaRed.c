/*
 PROGRAMA:  BATERIA
 DESCRIPCION: Este programa pregunta al usuario que operación se quiere hacer, la convierte
              al formato correcto, abre un socket para mandarlo y espera la respuesta.
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>

#include "datagen.h"
#include "nptinci.h"
#include "deffich.h"
#include "ftradred.h"
#include "cforitno.h"
#include "svm.h"

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

char coderror[4];

// datos de entrada
int opcion;
char op[2];
char tipo[5];
char imp[13];
char tarj[20];
char DATPREV[36];
char digi12[3];
char fmt203[42];


formato_interno fiforitn;
mensaje_nuc  msgnuc;
NPSINCI reginci;

/******* para obtener la clave de la tabla NPTSOCK ***/
struct sockaddr_in ser_addr;

int main(int argc,char *argv[])    /***     FUNCION PRINCIPAL     ***/
{
   socklen_t Long_Cliente;
   struct sockaddr cli_addr;
   int hfd, iskopt, numop =0; 
   int enviar, fin=0; 
   char FichTraza[100];
   char otraop[3];

   signal(SIGCLD,SIG_IGN); //sinonimo de SIGCHLD en Linux
   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);
   signal(SIGALRM,SIG_IGN);


  if ((sfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
        printf("Error al crear socket \n");
        return(-1);
  }

  //rellenamos la estructura
  //mejor con htons  para que vaya protegido
  memset(&ser_addr, 0 , sizeof(ser_addr));
  ser_addr.sin_port = htons(20324);
  ser_addr.sin_family = AF_INET;
//  ser_addr.sin_family=PF_INET; 
    ser_addr.sin_addr.s_addr =INADDR_ANY;

  //hacemos el bind
   iskopt=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&retorno,sizeof(retorno)); //sets a socket option
   if(iskopt)
      fprintf(stderr,"Retorno setsockopt=%d\n",iskopt);

  if (bind (sfd,(struct sockaddr*)&ser_addr, sizeof (ser_addr)) == -1)
  {
         printf("error al hacer el bind\n");
		 printf("con error %d\n", errno);
		 close (sfd);
         return(-1);
  }
  printf("socket conectado es %d\n", sfd);

 //creamos el listen con n elementos. Elementos que pueden estar en cola a la vez.
   listen (sfd, 5);
   
//tratamiento de operaciones
   numop=0;
   while (fin==0) 
   {
    //preguntar operacion al usuario
      printf("¿Que operación quieres hacer (1200/1220/1420)?\n");
     //recogemos el dato
      scanf("%s", &tipo);
	  tipo[4]='\0';
     // sprintf(tipo,"%s","1200");
	 
      printf("Introduce los dos primero digitos del codigo de proceso\n");
      scanf("%s", &digi12);
	  digi12[3]='\0';

      printf("Introduce el importe de la operacion ej 600 = 6 euros:\n");
      scanf("%s", &imp);
      //sprintf(imp,"%s","600");


      printf("Introduce el numero de la tarjeta:\n");
      scanf("%s", &tarj);
      //sprintf(tarj,"%s","4508010004511620");
  
      if (strcmp(tipo,"1420") == 0)
      {
        printf("Introduce previa:\n");
        scanf("%s", &DATPREV);
      }
    //  retorno = cargaInicial();

     /*  ----------  traduce el mensaje de origen red  ----------  */
	 //// OJO para obtener el mensaje netplus - red simulamos la traducción netplus - red, pero no tiene que ser
	 ////     igual el mapa de bits si viene desde la red o si viene desde netplus. Cambiar el mapa de bits
	 ////     cuando se pueda probar con la red.
      usotrad = '2';
      sentido = 2;
      suba_iso = 30;
      long_iso = 0;
      error_code = 0;
      error_bit = 0;

      printf("espero cliente\n");

      Long_Cliente = sizeof (cli_addr);
      
      printf("espero cliente despues long_client \n");
		
      if ((hfd = accept (sfd, &cli_addr, &Long_Cliente)) == -1)
        {
                printf("error en el accept\n");
                printf("con error %d\n", errno);
                close(sfd);		 
                return (-1);
        }
        
      printf("despues del accept\n");
      enviar=sizeof(mensaje_nuc);
      printf("despues del accept enviar=sizeof(mensaje_nuc)\n");
	  if (numop==0)
	  {
         printf ("conectado, espero 3 seg y envio socket %i\n", hfd);
         sleep(3);
	  }
      
      printf("antes del mensaje_puc\n");
      //envio de un mensaje 1814
      mensaje_puc[0]=0;
      mensaje_puc[1]=57;
      strcpy(mensaje_puc+2,"1814"); 
      mensaje_puc[6]=128;
      mensaje_puc[7]=48;
      mensaje_puc[8]=0;
      mensaje_puc[9]=0;
      mensaje_puc[10]=2;
      mensaje_puc[11]=0;
      mensaje_puc[12]=0;
      mensaje_puc[13]=0;
      mensaje_puc[14]=0;
      mensaje_puc[15]=0;
      mensaje_puc[16]=0;
      mensaje_puc[17]=12;
      mensaje_puc[18]=0;
      mensaje_puc[19]=0;
      mensaje_puc[20]=0;
      mensaje_puc[21]=0;

      strcpy(mensaje_puc+22, "8893912006091031568000602800006020232");       

      enviar=59;
      printf("antes de EnviaSock\n");
      retorno = EnviaSock(hfd,0,mensaje_puc,enviar);
      if (retorno == -1)
      {
             printf("error al enviar datos\n");
             strcpy(reginci.innomfic,"Leer_q");
             strncpy(reginci.incoderr,itoa(retorno),8);
      }
	  else
	  {
            printf("Enviado por el socket el mensaje >%.200s<\n", mensaje_puc);
	  }
      close(hfd);
		 
	  numop ++;
	  if (numop==5)
	  {
         printf("----Se han realizado 5 operaciones. Parar todo y arrancar de nuevo----\n");
         fin = 1;	  
	  }
	  else
	  {
         printf("¿Quiere realizar otra operacion s/n?\n");
         scanf("%s", &otraop);
	     otraop[1]='\0';
	     if (otraop[0] != 's' && otraop[0] != 'S')
		   fin = 1;
	  }
  }
  close(sfd);		 
  sfd = 0;
  return(0);
} // Fin del Main

/*
FUNCION: cargaInicial , rellena el formato interno
*/
//#include "ftradred.c"
