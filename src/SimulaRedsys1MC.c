//PRJ0247167 - Revisado ampliación P48
/********************
*******************/
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>


//tuxedo

#include "cnetplus.h"
#include "svm.h"
#include "Cola_comun.h"
#include "msgclien.h"
#include "procesos.h"

/**************************************************************/
/*********     MAIN                                      ******/
/**************************************************************/
// si el retorno es <> 0 es que ha ido mal**
int  damecola(SVM *psvm_par,char *proce,char *subsis);
int  conecSVM(void **gen_psvm);
int  desconec_svm(void *pun_svm);
int  EscribirCola_prueba(char *Nombcola, char* Msj, int LonMsj, int* idHj, int tiempo, char *emisor);
int  leerCola_prueba(char *Nombcola, char* Msj, int *LonMsj, int idHj, int tiempo, char *emisor);
int  montarrespuesta(char* MsjE,char* MsjS,char *emisor); 

SVM *psvm;
char Emisor[3];
char FicheroTraza[100];
int Posicion;
void *EnviarFichero (void *);
pthread_t idHilo;
char DatomsgIn[4096]; 
char DatomsgOut[4096]; 
char CLIENTE[10];
char SERVIDOR[10]; 
int idHj=0; 
int iTimeOut = 1000;
FILE * fcaptura = NULL;
char nomFich[1024];
char rutaFich[1024];
int segundos = 0;
void timeout_pp( );
int LongitudMensaje;
   
int main(int argc, char *argv[])
{
   int iLong=0;
   char NOMCOLA[30];
   int retorno = 0;
   char cola_cliente[5];
   char cola_servidor[8];
   

   if (argc != 2)
   {
     printf("Uso del programa : SimulaRedsys <Emisor 20 30 o 40)>\n");
     exit(0);
   } else {
     strcpy(Emisor,argv[1]);
     fprintf(stderr,"Emisor <%s>\n", Emisor);
   }

    char* HOME;
    HOME = getenv ("SF_HOME");

    sprintf(FicheroTraza,"%s%s" ,HOME, "/sisma/dat/trazas/SimulaRedsys.traza");

//   printf("trazas en /sf/sisma/dat/trazas SimulaRedsys.traza\n");
   freopen (FicheroTraza, "w", stderr);

 // if (!strncmp(Emisor, "30",2))
 // {
 // 
 //   strcpy(CLIENTE,CLIENT30); 
 //   strcpy(SERVIDOR, PRECEP30);
 // }
 // else
 // {
 //   if (!strncmp(Emisor, "40",2))
 //   {
 //     strcpy(CLIENTE, CLIENT40); 
 //     strcpy(SERVIDOR, PRECEP40);
 //   } 
 //   else
 //   {
 //     strcpy(CLIENTE, CLIENT20); 
 //     strcpy(SERVIDOR, PRECEP20);
 //   }
 // }
   strcpy(cola_cliente,CLIENT); 
   strcpy(cola_cliente+2,Emisor); 
   cola_cliente[4]='\0';
   strcpy(CLIENTE, cola_cliente); 
   
   strcpy(cola_servidor,PRECEP); 
   strcpy(cola_servidor+6,Emisor); 
   cola_servidor[8]='\0';
   strcpy(SERVIDOR, cola_servidor); 

   pthread_create (&idHilo, NULL, EnviarFichero, NULL);

/*
    if (argc != 2)
   {
      printf("Uso del programa : prueba <Emisor 30 o 40)>\n");
      exit(0);
   } else {
     strcpy(Emisor,argv[1]);
     printf("Emisor <%s>\n", Emisor);
   }
*/  
   while (1)
   {
//    fprintf(stderr,"vamos a leer cola\n");

    retorno = leerCola_prueba(CLIENTE, DatomsgIn, &iLong, getpid(), iTimeOut,Emisor); 

    if (retorno)
    {
	   fprintf(stderr,"error al escribir en la cola %d\n", retorno);  
       fprintf(stderr,"SISMA NO RESPONDE\n");	   
	   return(0);  
    }		
    else
    {
  	 if (strncmp(DatomsgIn,"1804",4) == 0)
  	 {
   	   fprintf(stderr,"Enviado esperando respuesta\n"); 
           retorno= montarrespuesta(DatomsgIn,DatomsgOut,Emisor);
           iLong = 57;
           retorno = EscribirCola_prueba(SERVIDOR, DatomsgOut, iLong, &idHj, iTimeOut,Emisor);
         }
     }
   }	
	
	return(retorno);
	
	
}//fin mainnnnn



/**************************************************************/
/*********     LANZA KILL                                 ******/
/**************************************************************/
int montarrespuesta(char * DatomsgIn,char * DatomsgOut,char *emisor)
{
//    fprintf("montar respuesta mensaje entrada DatomsgIn %s\n",DatomsgIn);
    strncpy(DatomsgOut, "1814", 4);
//mapa de bits fijo
    DatomsgOut[4] = 128;
    DatomsgOut[5] = 48;
    DatomsgOut[6] = 0;
    DatomsgOut[7] = 0;
    DatomsgOut[8] = 2;
    DatomsgOut[9] = 0;
    DatomsgOut[10] = 0;
    DatomsgOut[11] = 0;
    DatomsgOut[12] = 0;
    DatomsgOut[13] = 0;
    DatomsgOut[14] = 0;
    DatomsgOut[15] = 12;
    DatomsgOut[16] = 0;
    DatomsgOut[17] = 0;
    DatomsgOut[18] = 0;
    DatomsgOut[19] = 0;

    strncpy(DatomsgOut+20, DatomsgIn+20,18);
    strncpy(DatomsgOut+38,"800",3);
    strncpy(DatomsgOut+41, DatomsgIn+41,16);

//      strncpy(DatomsgOut+4, DatomsgIn +4,sizeof(DatomsgIn-4));
//    printf("montar respuesta mensaje salida DatomsgOut %s\n",DatomsgOut);    
//    printf("mensaje recibido==========================================================\n");
//    fhexdump(stdout, DatomsgIn, 57);
//    printf("mensaje enviado==========================================================\n");
//    fhexdump(stdout, DatomsgOut, 57);
    
    return(0);  		 
}

int EscribirCola_prueba(char *Nombcola, char* Msj, int LonMsj, int* idHj, int tiempo, char *emisor)
{	
   int qenvio;
   int retorno;
   mensaje_lin    msglin;
   char mens[5];

   /* --------- se ata a la memoria compartida SVM  ------------ */
 //  if (!strncmp(Emisor, "20",2)) 
 //     retorno=conecSVMA((void *)&psvm); 
 //  else
 //     retorno=conecSVMI((void *)&psvm);
   retorno=conecSVM((void *)&psvm);
   if (retorno != 0)
   {
      fprintf(stderr,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
      return(E_NOIPC);
   }

    qenvio=damecola(psvm,Nombcola,NULL);
    if (qenvio == -1)
    {
       perror("error >>>");
       fprintf(stderr,"error crea cola QENVIO buscando %s\n", Nombcola);
       return(E_NOIPC);
    }
//    fprintf(stderr,"cola con numero <%i>\n", qenvio);
    //Monto el mensaje
    if (LonMsj > 1000)
    {
         fprintf(stderr,"Cola %s con nº:%i LonMsg <%d> MAX_BUFFER <%d>, no hay espacio\n", Nombcola, qenvio, LonMsj, MAX_BUFFER);
         return (E_LENGERR);
    }
//    msglin.tipo=getpid();
//
  //  if (!strncmp(Emisor, "30",2))
  //     msglin.tipo=3002;
  //  else
  //     if (!strncmp(Emisor, "40",2))
  //        msglin.tipo=4002;
  //  else 
  //        msglin.tipo=2002;
    strncpy(mens,Emisor,2);
    strncpy(mens+2,"02",2);
    mens[4] = '\0';	
    
    msglin.tipo=atoi(mens);

    (*idHj)=msglin.tipo;
    memcpy(msglin.mensaje_linea, Msj,  LonMsj);
//    fprintf(stderr,"por la cola %i y con el pid %i\n", qenvio, msglin.tipo);

    fprintf(stderr,"mensaje enviado==========================================================\n");
    fhexdump(stderr, &msglin, LonMsj+4);
    fprintf(stderr,"mensaje enviado==========================================================\n");
    fflush(stderr);
//Escribimos el mensaje
    retorno=msgsnd(qenvio, &msglin, LonMsj+4, 0);
//    retorno=msgsnd(qenvio, Msj, LonMsj, 0);
    if (retorno == -1)
    {
	fprintf(stderr,"Cola %s con nº:%i, cola llena", Nombcola, qenvio);
	return (E_QBUSY);
    }
    else
    {
	fprintf(stderr,"retorno de snd es %i\n", retorno);
	return(retorno);
    }

}
int leerCola_prueba(char *Nombcola, char* Msj, int *LonMsj, int idHj, int tiempo, char *emisor)
{
        int qrecep;
        int retorno;
        int flag, error;
        int LMen;
        mensaje_lin    msglin;
        LMen=sizeof(msglin) - sizeof(msglin.tipo);

   /* --------- se ata a la memoria compartida SVM  ------------ */
 //   if (!strncmp(Emisor, "20",2)) 
 //     retorno=conecSVMA((void *)&psvm); 
 //   else
 //    retorno=conecSVMI((void *)&psvm);
     retorno=conecSVM((void *)&psvm);
    if (retorno != 0)
    {
       fprintf(stderr,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
       return(E_NOIPC);
    }
    fprintf(stderr,"conectado a SVM\n");
    qrecep=damecola(psvm,Nombcola,NULL);
    fprintf(stderr,"qrecep con numero de cola <%i>\n", qrecep);
    if (qrecep == -1)
    {
       perror("error >>>");
       fprintf(stderr,"error crea cola  QRECEP  \n");
       return(E_NOIPC);
    }
     msglin.tipo = 0;
     idHj = 0;	 
	
     retorno=msgrcv(qrecep, &msglin,  LMen, idHj, 0); 
     error = errno;
     fprintf(stderr, "errno recepcion de la cola <%d>\n", error);
//     printf("Error msgrcv <%d>\n", error);	
     if (tiempo > 0) //libera la alarma
        alarm(0);

     if (retorno == -1)
     {
          if (error==ENOMSG)
                        //cola vacia, no hay datos
          {
                fprintf(stderr,"cola vacia\n");
                return (E_QVACIA); 
          }
	  else if (error==EINTR)
          {
			/* se ha cumplido el Time Out de espera en la cola */
			fprintf(stderr,"cumplico time-out de la cola de recepcion\n");
	       	return (E_TIMEOUT);
	  } else 
            {//error al leer
	          fprintf(stderr,"Cola %s con nª%i: .REcibiendo error por msgrcv\n", Nombcola, qrecep);
		  return error;
             }
     }
//     fprintf(stderr,"retorno <%d> LMen <%d> \n", retorno, LMen);
//     fprintf(stderr, "msglin <%s> \n", msglin);
//     fprintf(stderr, "msglin.mensaje_linea <%s>\n", msglin.mensaje_linea);
     fprintf(stderr, "recibidos <%d> bytes\n", retorno);
     fprintf(stderr,"- - mensaje recibido==========================================================\n");
     fhexdump(stderr, msglin.mensaje_linea, retorno);
     fprintf(stderr,"- - mensaje recibido==========================================================\n");
			
     memset(Msj, '\0', sizeof(msglin));
     memcpy(Msj, msglin.mensaje_linea, retorno);
     *LonMsj = retorno;
     retorno = desconec_svm(psvm);

     return 0;
}

void timeout_pp( )
{
   fprintf(stderr,"timeout de lectura\n");
   //mandar error.
}

void *EnviarFichero (void * arg) 
{
   char FichTraza[100];
   char otraop[3];
   int retorno=0;
   char mensaje_puc[4000]; //PRJ0247167 - Revisado ampliación P48
   
      while (1)
      {
         if (AbrirFichero() == -1)
         {
            fprintf(stderr,"Error abrir fichero\n");
//            return (-1);
            exit(0);
         }
         Posicion = 0;
         while (1)
         {
            memset(mensaje_puc, 0, 4000); //PRJ0247167 - Revisado ampliación P48 - se amplia de 2000 a 4000
            if (llenar_mensaje_con_fichero(mensaje_puc)==-1)
               break;
            
            if (strncmp(mensaje_puc, "1814",4) != 0)   
            {
//               printf("SERVIDOR <%s> mensaje_puc <%s> LongitudMensaje <%d>\n", SERVIDOR, mensaje_puc, LongitudMensaje);
               retorno = EscribirCola_prueba(SERVIDOR, mensaje_puc, LongitudMensaje, &idHj, iTimeOut,Emisor);

//            retorno = EnviaSock(hfd,0,mensaje_puc,enviar+2);
               if (retorno == -1){
                  fprintf(stderr,"error al enviar datos\n");
//               } else {
//                   fprintf(stderr,"Enviado por el socket el mensaje >%.200s<\n", mensaje_puc);
               }
            }
//            segundos = 2;
            if (segundos != 0)
            {
               fprintf(stderr,"sleep segundos : <%d>\n", segundos);
               sleep(segundos);
            }
        }
      }
	

//  if(fcaptura)
//     fclose(fcaptura);
	   
//  return(0); 
    exit(0); 
}


// funcion igual que en lanzaOpe.c
int llenar_mensaje_con_fichero(char mensaje[]){
//   int segundos=0;
   int numOpSeguidas=0;
   unsigned short longitud_archivo_short;
   int longitud_archivo;
   int i;

   memset(mensaje, 0, 4000); //PRJ0247167 - Revisado ampliación P48
//   Posicion=2;
   LongitudMensaje = 0;
   while (1)
   {

   if (fread(mensaje + LongitudMensaje, 1, 1, fcaptura) == 0)
     {
          if (LongitudMensaje != 0)
          {
             // comprobamos si la linea es un comentario, empieza en //
             if (!strncmp(mensaje, "//",2))
                if (fread(mensaje + LongitudMensaje, 1, 1, fcaptura) == 0)
                   return (0);
                else
                   return (-1);
          }
          else
     	     return (-1);
     }
//     printf("leido <%c>  <%0X> Posicion <%d>\n", mensaje[LongitudMensaje],  mensaje[LongitudMensaje], LongitudMensaje);
     LongitudMensaje++;
/*
     if (mensaje[LongitudMensaje-1] == 10)
     {
        fprintf(stderr,"final de mensaje <%d> ultimo caracter <%d>\n",i,mensaje[i]);
        longitud_archivo = Posicion;
        Posicion++;
        break;
     }
*/
     Posicion++;
   } 
   fprintf(stderr,"longitud mensaje a enviar <%d>\n", LongitudMensaje);
  
/*    
  longitud_archivo_short = (unsigned short) longitud_archivo;

  printf("longitud_archivo %d\n", longitud_archivo);
  printf("enviar %d\n", enviar);
  printf("longitud_archivo_short %u\n", longitud_archivo_short);
  memcpy(binario, (unsigned char*) &longitud_archivo_short, 2);
  
  mensaje[0]=binario[1];
  mensaje[1]=binario[0];
  
  printf(" mensaje[0] %x\n", mensaje[0]);
  printf(" mensaje[1] %x\n", mensaje[1]); 
*/  
	return(0);
}

// funcion igual que en lanzaOpe.c
int AbrirFichero()
{

    char* HOME;
    HOME = getenv ("SF_HOME");

//    printf("HOME <%s>", HOME);    
    printf("Nombre del fichero: (en /sisma/dat)\n"); 
    scanf("%s", &nomFich); 
    printf("nombre de fichero leido <%s>\n", nomFich);
//    printf("Subsistema origen 3002 o 3004\n");
//    scanf("%d", &subsisOri); 


    printf("segundos de espera entre grupos de operaciones:\n");
    scanf("%d", &segundos); 

	
    if(strlen(nomFich) >= 1024){
	   fprintf(stderr,"nombre de fichero demasiado largo\n");
	   return(-1);
    }
    if(strlen(nomFich) < 3){
	   fprintf(stderr,"nombre de fichero no valido\n");
	   return(-1);
    }
//////////////// APERTURA DEL FICHERO /////////////////	
    sprintf(rutaFich,"%s%s%s" ,HOME, DIRDATOS,nomFich);
    fprintf(stderr,"lanzaOpe del fichero >%s<\n",rutaFich);
	
    fcaptura = fopen(rutaFich, "r"); 
    if(!fcaptura){
        fprintf(stderr,"error en apertura del fichero\n");
        return(-1);
    }
    return(1);
}
