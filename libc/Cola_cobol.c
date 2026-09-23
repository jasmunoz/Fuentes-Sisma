#include <sys/types.h>
#include <unistd.h>  
#include <sys/msg.h> 
#include <stdio.h>
#include <errno.h> 

#include "Cola_comun.h"
#include "svm.h"
#include "msgclien.h"
#include "nptpars.h"

SVM *psvm;

void fprintLog(char ficheroPlantilla[50], char* format, ...);

//extern char nomProg[50];

/****************prototipos**********/
int damecola(SVM *psvm_par,char *proce,char *subsis);
int damecolaO(SVM *psvm_par,char *proce,char *subsis);
//int conecSVMI(void **gen_psvm);
//int conecSVMA(void **gen_psvm);
int conecSVM(void **gen_psvm);
int desconec_svm(void *pun_svm);

void timeout_lr( );
/* 
EscribirCola para programa C
parametros:char* Nombcola ---> Nombre de cola al que se envia
           char *Msj      ---> Puntero a char donde estan los datos
           int LonMsj     ---> Longitud a enviar
           int idHj       ---> id del padre al que debe retornar
		   int tiempo     ---> Tiempo de espera de envio    
Devuelve 0 si ha enviado o -1 si hay error 
*/
int EscribirCola_c(char *Nombcola, char* Msj, int LonMsj, int* idHj, int tiempo)
{	
        int qenvio;
        int retorno;
        mensaje_lin    msglin;

   /* --------- se ata a la memoria compartida SVM  ------------ */
    retorno=conecSVM ((void *)&psvm);
	
    if (retorno != 0)
    {
      // printf("ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
       return(E_NOIPC);
    }
    printf("En escribir_cola_c QENVIO Nombcola %s\n", Nombcola);
    qenvio=damecola(psvm,Nombcola,NULL);
	printf("En escribir_cola_c qenvio %i\n", qenvio);
    if (qenvio == -1)
    {
       perror("error >>>");
       printf("error crea cola QENVIO buscando %s\n", Nombcola);
       //Añadido para Monitorizacion SISMA 
       printf("TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion >%s< codigo de error>%i<\n",Nombcola, E_NOIPC);
       return(E_NOIPC);
    }

    //Monto el mensaje
    if (LonMsj > MAX_BUFFER)
    {
         printf("Cola %s con n:%i , no hay espacio", Nombcola, qenvio);
        //Añadido para Monitorizacion SISMA 
        printf("TIPO SISTEMA- ACCESO A LAS COLAS: Tipo Escritura >%s< codigo de error>%i< no hay espacio\n",Nombcola, E_LENGERR);
        return (E_LENGERR);
    }
    msglin.tipo=getpid();
    (*idHj)=msglin.tipo;
    memcpy(msglin.mensaje_linea, Msj,  LonMsj);
    printf("por la cola %i y con el pid %i\n", qenvio, msglin.tipo);

    //Escribimos el mensaje
    retorno=msgsnd(qenvio, &msglin, LonMsj+4, 0);
    if (retorno == -1)
    {
	printf("Cola %s con n:%i, cola llena", Nombcola, qenvio);
    //Añadido para Monitorizacion SISMA 
    printf("TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Escritura >%s< codigo de error>%i< cola llena\n",Nombcola, E_QBUSY);
	return (E_QBUSY);
    }
    else
    {
	printf("retorno de snd es %i\n", retorno);
	return(retorno);
    }

}
/* 
leerCola para programa C
parametros:char* Nombcola ---> Nombre de cola de la que se recibe
           char *Msj      ---> Puntero a char donde estan los datos
           int LonMsj     ---> Longitud a enviar
           int idHj       ---> id del padre al que debe retornar
		   int tiempo    ---> Tiempo de espera de envio    
Devuelve 0 si ha enviado o -1 si hay error 
*/

int leerCola_c(char *Nombcola, char* Msj, int *LonMsj, int idHj, int tiempo)
{
        int qrecep;
        int retorno;
        int flag, error;
        int LMen;
        mensaje_lin    msglin;
        LMen=sizeof(msglin) - sizeof(msglin.tipo);
		printf("En leer_cola_c >%s<\n", Nombcola); //PRJ0258542

   /* --------- se ata a la memoria compartida SVM  ------------ */
    retorno=conecSVM((void *)&psvm);
	
    if (retorno != 0)
    {
       printf("ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
       return(E_NOIPC);
    }
    qrecep=damecolaO(psvm,Nombcola,NULL);
	printf("En leer_cola_c qrecep %i\n", qrecep);
    if (qrecep == -1)
    {
       perror("error >>>");
       printf("error crea cola  QRECEP  \n");
      //Añadido para Monitorizacion SISMA 
      printf("TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion >%s< codigo de error>%i<\n",Nombcola, E_NOIPC);
       return(E_NOIPC);
    }
	//printf("cola de lectura: %d\n", qrecep);

    //tiempo
    signal(SIGALRM, timeout_lr);
	
	//printf("tiempo espera %d\n", tiempo);
    if (tiempo == -1)
       flag = 0;   //se bloquea la cola de lectura infinitamente
    else 
       if (tiempo == 0)
          flag = IPC_NOWAIT;  //no espera por mensaje 
       else
       {
            flag = 0;   //se bloquea por un tiempo lemitado
            alarm(tiempo);  //pasado el tiempo, se activa la se𠪠SIGALRM
       }
    //Escribimos el mensaje
     msglin.tipo = idHj;
	 
	 //printf("LMen = %d\n", LMen);
	
     printf("en leercolaC LMen >%d< msglin.mensaje_linea >%s< msglin.tipo >%d< qrecep >%d< \n", LMen, msglin.mensaje_linea, msglin.tipo, qrecep);
     retorno=msgrcv(qrecep, &msglin,  LMen, idHj, flag);
     error = errno;
	
     if (tiempo > 0) //libera la alarma
	alarm(0);

     if (retorno == -1)
     {
          if (error==ENOMSG)
                        //cola vacia, no hay datos
          {
                printf("cola vacia\n");
                return (E_QVACIA); 
          }
	  else if (error==EINTR)
          {
			/* se ha cumplido el Time Out de espera en la cola */
	       	return (E_TIMEOUT);
	  } else 
            {//error al leer
	          printf("Cola %s con n%i: .REcibiendo error por msgrcv\n", Nombcola, qrecep);
              //Añadido para Monitorizacion SISMA 
             printf("TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Lectura >%s< codigo de error>%i<\n",Nombcola, error);
		  
          return error;
             }
     }
			
     memset(Msj, '\0', sizeof(msglin));
     memcpy(Msj, msglin.mensaje_linea, retorno); 
     *LonMsj = retorno;
	  printf("al salir de leercolaC Msj >%s< msglin.mensaje_linea >%s<  LonMsj >%i< \n",Msj, msglin.mensaje_linea,LonMsj);
     /*desconecto de la svm*/
     retorno = desconec_svm(psvm);

     return 0;
}
void timeout_lr( )
{
   printf("timeout de lectura\n");
   //mandar error.
}

/* 
leer_cola_cob para programa COBOL
parametros:char* cob_nombcola --> Nombre de cola de la que se recibe
           char* cob_tiempo   --> tiempo de espera
           char* cob_Msj      --> Puntero a char donde estan los datos
           char* cob_lonj     --> Longitud a enviar
Devuelve 0 si ha enviado o -1 si hay error 
*/
int leer_cola_cob(char *cob_nombcola,
                  char *cob_tiempo,
		  char *cob_Msj,
                  char *cob_lonj)
{
	int retorno, Derr;
	int tiempo;
	int Clon = 0;
		
	tiempo = atoi(cob_tiempo);
	
	retorno = leerCola_c(cob_nombcola, cob_Msj, &Clon, 0, tiempo);

        /* Me desconecto de la SVM */
        Derr = desconec_svm(psvm);
	
	sprintf(cob_lonj,"%05d",Clon);
	
	return(retorno);
}

/* 
leer_cola_cob_id para programa cobol
parametros:char* cob_nombcola --> Nombre de cola de la que se recibe
           char* cob_idhij    --> mensaje que debe leer
           char* cob_tiempo   --> tiempo de espera
           char* cob_Msj      --> Puntero a char donde estan los datos
           char* cob_lonj     --> Longitud a enviar
Devuelve 0 si ha enviado o -1 si hay error 
*/
int leer_cola_cob_id(char *cob_nombcola, char *cob_idhij, char *cob_tiempo, char *cob_Msj, char *cob_lonj)
{
	int retorno, tiempo, Derr;
	int idHij;
	int Clon = 0;
	
	tiempo = atoi(cob_tiempo);
	idHij = atoi(cob_idhij);
	printf("EN LEER_COLA_COB_ID >%s<\n",cob_nombcola);
	retorno = leerCola_c(cob_nombcola, cob_Msj, &Clon, idHij, tiempo);

        /* Me desconecto de la SVM */
        Derr = desconec_svm(psvm);
	
	sprintf(cob_lonj,"%05d",Clon);
	
	return(retorno);	
}

/* 
escribir_cola_cob_id para programa COBOL
parametros:char* cob_nombcola --> Nombre de cola de la que se recibe
           char* cob_idhij    --> mensaje que debe leer
           char* cob_tiempo   --> tiempo de espera
           char* cob_Msj      --> Puntero a char donde estan los datos
           char* cob_lonj     --> Longitud a enviar
Devuelve 0 si ha enviado o -1 si hay error 
*/
int escribir_cola_id(char *cob_nombcola,
                     char *cob_idhij,
	             char *cob_tiempo,
		     char *cob_Msj,
		     char *cob_lonj)
{
	int retorno, Derr;
	int Long;
	int idHj=0;
        printf("entramos en C\n");

	if ((Long=atoi(cob_lonj)) > MAX_BUFFER ) {
		return(E_LENGERR);
	}
        cob_Msj[Long]='\0';
	retorno = EscribirCola_c(cob_nombcola, cob_Msj, Long, &idHj, atoi(cob_tiempo));
        /* Me desconecto de la SVM */
        Derr = desconec_svm(psvm);

        sprintf(cob_idhij, "%05d", idHj);

	return(retorno);
}


