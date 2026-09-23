
/********************
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "cnetplus.h"
#include "svm.h"
#include "Cola_comun.h"
#include "msgclien.h"
#include "procesos.h"
#include "datagen.h"

/**************************************************************/
/*********     MAIN                                      ******/
/**************************************************************/
// si el retorno es <> 0 es que ha ido mal**
SVM *psvm;
char Emisor[3];

void timeout_pp( );
   
int main(int argc, char *argv[])
{
   int iLong=0;
   char NOMCOLA[30];
   int idHj=0; 
   int iTimeOut = 1000;
   int retorno = 0;
   char cola_envio[5];
   char cola_recep[9];
   
   
   int  damecola(SVM *psvm_par,char *proce,char *subsis);
   int  conecSVM(void **gen_psvm);
   int  desconec_svm(void *pun_svm);
   int  EscribirCola_prueba(char *Nombcola, char* Msj, int LonMsj, int* idHj, int tiempo, char *emisor);
   int  leerCola_prueba(char *Nombcola, char* Msj, int *LonMsj, int idHj, int tiempo, char *emisor);
   int  montarrespuesta(char* MsjE,char* MsjS,char *emisor); 

   //ESTRUCTURA DE DATOS DE ENTRADA
   typedef struct
   {
    char ptsubsis[2];
    char ptsubapl[2];
    char pttipmsj[3];
    char caractedialogo[3];
   }formtPant;
   formtPant datEnt;

   //ESTRUCTURA DE DATOS DE SALIDA
   typedef struct
    { char subsistema[2];
      char subaplicacion[2];
      char codigo[4];
      char cod039[3];
    }formtPantS;
   formtPantS datSal;

   char DatomsgIn[4096]; 
   char DatomsgOut[4096];  

   if (argc != 2)
   {
      printf("Uso del programa : prueba <Emisor 30, 40, 20, 10)>\n");
      exit(0);
   } else {
     strcpy(Emisor,argv[1]);
     printf("Emisor <%s>\n", Emisor);
   }
  
     printf("TEST A SUBSISTEMA 02 \n");
    
   while (1)
   {
    printf("vamos a leer cola\n");

       strncpy(cola_client,CLIENT,2);
	   strncpy(cola_client+2,Emisor,2);
	   cola_client[4]='\0';
       retorno = leerCola_prueba(cola_client, DatomsgIn, &iLong, getpid(), iTimeOut,Emisor); 

    if (retorno)
    {
	   printf("error al escribir en la cola %d\n", retorno);  
       printf("SISMA NO RESPONDE\n");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Lectura CLIENT%.2s codigo de error >%i<\n",Emisor,retorno);
       
	   return(0);
    }		
	else
	{
		printf("Enviado esperando respuesta\n"); 
        retorno= montarrespuesta(DatomsgIn,DatomsgOut,Emisor);
        iLong = 57;

	   strncpy(cola_recep,PRECEP,6);
	   strncpy(cola_recep+6,Emisor,2);
	   cola_recep[8]='\0';
	   retorno = EscribirCola_prueba(cola_recep, DatomsgOut, iLong, &idHj, iTimeOut,Emisor);
               
	}
     
   }	
	
	return(retorno);
	
	
}//fin mainnnnn



/**************************************************************/
/*********     LANZA KILL                                 ******/
/**************************************************************/
int montarrespuesta(char * DatomsgIn,char * DatomsgOut,char *emisor)
{
//    printf("montar respuesta mensaje entrada DatomsgIn %s\n",DatomsgIn);
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

//    strncpy(DatomsgOut+4, DatomsgIn +4,sizeof(DatomsgIn-4));
//    printf("montar respuesta mensaje salida DatomsgOut %s\n",DatomsgOut);    
    printf("mensaje recibido==========================================================\n");
    fhexdump(stdout, DatomsgIn, 57);
    printf("mensaje enviado==========================================================\n");
    fhexdump(stdout, DatomsgOut, 57);
    
    return(0);  		 
}

int EscribirCola_prueba(char *Nombcola, char* Msj, int LonMsj, int* idHj, int tiempo, char *emisor)
{	
        int qenvio;
        int retorno;
        mensaje_lin    msglin;
		char mens[5];

   /* --------- se ata a la memoria compartida SVM  ------------ */
    retorno=conecSVM((void *)&psvm);
    if (retorno != 0)
    {
       printf("ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
       return(E_NOIPC);
    }

    qenvio=damecola(psvm,Nombcola,NULL);
    if (qenvio == -1)
    {
       perror("error >>>");
       printf("error crea cola QENVIO buscando %s\n", Nombcola);
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Creacion QENVIO %s codigo de error >%i<\n",Nombcola, qenvio);

       return(E_NOIPC);
    }
    printf("cola con numero <%i>\n", qenvio);
    //Monto el mensaje
    if (LonMsj > MAX_BUFFER)
    {
         printf("Cola %s con nº:%i , no hay espacio", Nombcola, qenvio);
         return (E_LENGERR);
    }

    strncpy(mens,Emisor,2);
    strncpy(mens+2,"02",2);
    mens[4] = '\0';	
    
    msglin.tipo=atoi(mens);
    (*idHj)=msglin.tipo;
    memcpy(msglin.mensaje_linea, Msj,  LonMsj);
    printf("por la cola %i y con el pid %i\n", qenvio, msglin.tipo);

    fhexdump(stdout, &msglin, 57);
    //Escribimos el mensaje
    retorno=msgsnd(qenvio, &msglin, LonMsj+4, 0);

    if (retorno == -1)
    {
	   printf("Cola %s con nº:%i, cola llena", Nombcola, qenvio);
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo envio QENVIO %s codigo de error >%i< cola llena\n",Nombcola, qenvio);

	   return (E_QBUSY);
    }
    else
    {
	   printf("retorno de snd es %i\n", retorno);
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

    retorno=conecSVM((void *)&psvm);
    if (retorno != 0)
    {
       printf("ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
       return(E_NOIPC);
    }
    printf("conectado a SVM\n");
    qrecep=damecola(psvm,Nombcola,NULL);
    printf("qrecep con numero de cola <%i>\n", qrecep);
    if (qrecep == -1)
    {
       perror("error >>>");
       printf("error crea cola  QRECEP  \n");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo creacion QRECEP codigo de error >%i< \n",qrecep);

       return(E_NOIPC);
    }

    //Escribimos el mensaje
//     msglin.tipo = idHj;
     msglin.tipo = 0;
     idHj = 0;	 
	 //printf("LMen = %d\n", LMen);
	
     retorno=msgrcv(qrecep, &msglin,  LMen, idHj, 0); 
     error = errno;
     printf("retorno <%d>\n", retorno);
	
     if (tiempo > 0) //libera la alarma
	alarm(0);

     if (retorno == -1)
     {
          if (error==ENOMSG)
                        //cola vacia, no hay datos
          {
             //Añadido para Monitorizacion SISMA 
             fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Lectura qrecep %s %d msgrcv codigo de error >%i< cola vacia\n",Nombcola,qrecep, retorno);

             printf("cola vacia\n");
             return (E_QVACIA); 
          }
	  else if (error==EINTR)
          {
			/* se ha cumplido el Time Out de espera en la cola */
	       	return (E_TIMEOUT);
	  } else 
            {//error al leer
             //Añadido para Monitorizacion SISMA 
             fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Lectura qrecep %s %d msgrcv codigo de error >%i< cola vacia\n",Nombcola,qrecep, retorno);

	          printf("Cola %s con nª%i: .REcibiendo error por msgrcv\n", Nombcola, qrecep);
		  return error;
             }
     }
			
     memset(Msj, '\0', sizeof(msglin));
     memcpy(Msj, msglin.mensaje_linea, retorno);
     *LonMsj = retorno;
     /*desconecto de la svm*/
     retorno = desconec_svm(psvm);

     return 0;
}

void timeout_pp( )
{
   printf("timeout de lectura\n");
   //mandar error.
}
   //
