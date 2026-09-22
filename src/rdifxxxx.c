/*** 
   PROGRAMA: RDIFxxxx.C
   DESCRIPCION: Proceso residente encargado del tratamiento de los mensajes
                diferidos que el sistema debe enviar.
                Este proceso se duerme hasta que lo despierte otro proceso
                o su despertador, al despertarse comprueba si hay diferidos
                que enviar, si es asi, y la conexion esta operativa los
                envia, en caso contrario envia un mensaje de control (TST)
                y se vuelve a dormir.  
***/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/errno.h>

#include "nptdife.h"
#include "msg_nucl.h"
#include "cforitno.h"
#include "deffich.h"

#include "nptinci.h"
#include "nptlogo.h"
#include "datagen.h"
#include "nptpara.h"
#include "svm.h"
#include "colas.h"
#include "procesos.h"

#define PID ((psvm+rtrata_svm)->str_svm.svmproc.pid)

//***************************************************************************************
// PROVISIONAL PENDIENTE DE VER EL VALOR TIEMPO_ESPERA DE SE UTILIZA EN trata_diferido.c
#define TIEMPO_ESPERA 80
//***************************************************************************************


char nomProg[50];
int cod_err;
char  coderror[4];

NPSINCI  reginci;
NPSLOGO  reglogo;
NPSDIFE  regdife;
formato_interno *pforitn;
int qdife;
SVM *psvm;

int Isubapl;
char Csubapl[3];

char comando[200];

/*** prototipos de funciones locales ***/
void sigue();
char *dtoa(double);
void activa_rdif(int sig);
int finalizar(int codigo);
int trata_tst(char * subsistema_general,char * Csubapl);
int trata_diferido();
int envia_diferido(formato_interno *pforitn);
int busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int damecola(SVM *psvm_par,char *proce,char *subsis);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
void fprintLog(char ficheroPlantilla[50], char* format, ...);
int  fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
void fechorsys(struct tm_dec *tmPtr);
int conexion_BBDD();
int desconexion_BBDD();


mensaje_nuc msg;
int t_alarma, t_max;
int long_mensaje = sizeof(msg.fiforitn); //PRJ0258452 - quitado = sizeof(msg) - sizeof(msg.tipo);
int rtrata_svm=0;
int subsis_svm=0;
char subsis[5];
char subsistema_general[3];

int nocaducada=1;
int calculo=0;

int total_res;
int qtratami;
int int_numses;
int respuesta_tst;
int cerrado=0;
NPSPARA  *reg_para;

int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
   int retorno,queda;
   int ll;

   signal(SIGCLD,SIG_IGN);
   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);
   signal(SIGALRM,SIG_IGN);
   signal(SIGUSR1,SIG_IGN);
   pforitn = &msg.fiforitn;
   strcpy(nomProg,argv[0]);
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);


#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"inicio \n");
#endif

  strncpy(subsistema_general,argv[0]+4,2);
  subsistema_general[2] = '\0';

  strncpy(Csubapl, argv[0]+6, 2);
  Csubapl[2] = '\0';
  Isubapl=atoi(Csubapl);

  strcpy(subsis, subsistema_general);
  strcpy(subsis + 2, Csubapl);

#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"subsis %s\n", subsis);
#endif

   strcpy(coderror, "   ");
   strcpy(reginci.innompro,nomProg);

   //RAQUEL modificado para arranque unificado
   retorno=conecSVM((void *)&psvm); //ORIGINAL
   if (retorno != 0)
   {
/*#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif*/
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //supervis no puede arrancar si no esta en svm
   }

   rtrata_svm=busca_proc_svm(psvm,argv[0],-1);
   if (rtrata_svm < 0 )
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR ARRANQUE NO ESTOY EN SVM\n");
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "250");
       exit(finalizar(-1));//supervis no puede arrancar si no esta en svm
   }
   /* crea cola de envios a rtratami  */
   
	fprintLog(nomProg,"Dentro de rdifxxxx, antes del bucle \n"); //PRJ0258542	 
	for (ll=0 ; ll<2 ; ll++) 
	{  
		if ((strncmp(reg_para[ll].psnument,subsistema_general,2)==0) || (strncmp(reg_para[ll].psresolu,subsistema_general,2)==0))
	  {	
	  	qtratami = damecola(psvm,reg_para[ll].pscltrat,NULL);
	    break;            		  
	  }  
	} 
	fprintLog(nomProg,"Termina bucle for de rdifxxxx \n"); //PRJ0258542	 
    
    if (qtratami == -1)
    {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"error crea cola qtrata\n");
#endif
        strcpy(reginci.innomfic,"qtrata");
        strcpy(coderror, "009");
        exit(finalizar(-1));//supervis no puede arrancar si no esta en svm
    }

   /* crea cola de mensajes propia  */
   qdife = (psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
   
   if (qdife == -1)
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error crea cola qdife\n");
#endif
       strcpy(reginci.innomfic,"qdife");
       strcpy(coderror, "009");
       exit(finalizar(-1)); //supervis no puede arrancar si no esta en svm
   }
   
   /* ------- obtiene el desplazamiento del puntero para
      los datos del subsistema           ----------------*/
   strncpy(subsistema_general,argv[0]+4,2);
   subsistema_general[2] = '\0';

   strncpy(Csubapl, argv[0]+6, 2);
   Csubapl[2] = '\0';
   Isubapl=atoi(Csubapl);

   strcpy(subsis, subsistema_general);
   strcpy(subsis + 2, Csubapl);

   subsis_svm = encuentra_svm(psvm,subsis,TIP_SUBSIS);
   if (subsis_svm < 0)
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR  subsistema %s no ESTA EN SVM\n", subsis);
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "002");
       exit(finalizar(-1));//supervis no puede arrancar si no esta en svm
   }

   /****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
           LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR
   *******/

#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"se duerme hasta que le despierte el arranque \n");
#endif
   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2); 
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();

   queda=0;
   if((psvm+rtrata_svm)->estado != PARADA)
   {
      (psvm+rtrata_svm)->estado=ESPERA;
       pause();
       queda=alarm(0);
   }
   signal(SIGALRM,SIG_IGN);

   if (queda==0)
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR ARRANQUE\n");
#endif
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
       exit(finalizar(-1)); //sin pid no puede arrancarlo supervis
   }

   if (signal(SIGALRM, activa_rdif) == SIG_ERR)
   {
#if defined (TRAZA) && (TRAZA > 1)
       perror("signal");   
       fprintLog(nomProg,"error al asignar signal");
#endif
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
       exit(finalizar(-1)); //supervis no puede arrancar
   }

/****** BUCLE DE TRATAMIENTO DE MENSAJES ******/
   if((psvm+rtrata_svm)->estado != PARADA)
   {
     (psvm+rtrata_svm)->estado=TRABAJO;
     (psvm+rtrata_svm)->senal=SIGALRM;
 
     //Para no perder el primer diferido al arrancar con el semaforo abierto
     (psvm+subsis_svm)->str_svm.svmpars.pasemsub = 0;
      respuesta_tst = 0;
   
      activa_rdif(0);
      while (1)
      {
#if defined (TRAZA) && (TRAZA > 1)
          fprintLog(nomProg,"bucle de espera senial o despertador\n");
#endif
          //alarm(TIMEWAIT*2); //20160609 - TIMEWAIT lo utilizamos para controlar el tiempo que tarda en arrancarse los servicios
		  //                               y lo hemos puesto alto. El rdif no puede tardar tanto en mirar si hay diferidos.
		  alarm(5);
          pause();
      }
   }
   else
   {
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"orden de cierre 1\n");
#endif
      strcpy(coderror, "999");
      exit(finalizar(0));
   }
}

/*************************************************************************
   funcion de manejo de alarma
**************************************************************************/
void activa_rdif(int sig)
{
   int  retorno;
   int fin_fich =0;
   int recuperar;
   int tipomsg;

   //desactiva la señal para que mientras se ejecuta la funcion no reciba mas señales. Al final la vuelve a poner
   signal(SIGALRM,SIG_IGN);
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"---- Estoy activa_rdif\n");
   fprintLog(nomProg,"-- Estado->%c<-\n", (psvm+rtrata_svm)->estado);  
#endif
   /*  -------------  orden de cierre  ------------------  */
   if ((psvm+rtrata_svm)->estado==PARADA) /* orden de cierre */
   {
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"orden de cierre en bucle tratamiento\n");
#endif
      strcpy(coderror, "999");
      if ((psvm+rtrata_svm)->estado==RECARGAR)
        exit(finalizar(-2)); /* Paro para rearranque */
      else
        exit(finalizar(0)); /* Orden de Cierrre */
   }
   if ((psvm+rtrata_svm)->estado!=TRABAJO)
      return;

	
	  
	////////// ABRE LA CONEXION A LA BASE DE DATOS Y PERMANECE ABIERTA, SE CIERRA EN FINALIZAR
	retorno = conexion_BBDD();
	if(retorno)
	{
	   fprintLog(nomProg,"Error en el acceso a la base de datos\n");
	   exit(finalizar(-2));
	}  

	fprintLog(nomProg,"- - - - - - - - - - - - - - - - - - - - - - - -\n");
  
	while (fin_fich !=1)
	{
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"* * * * Bucle de trata diferido semaforo %i * * * *\n", (psvm+subsis_svm)->str_svm.svmpars.pasemsub);
#endif
	 
      // semaforo no abierto
     while(!respuesta_tst ||
        ((psvm+subsis_svm)->str_svm.svmpars.pasemsub) != 1)
     {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"-No cierre- estado ->%c<-\n", (psvm+rtrata_svm)->estado);
#endif
        if ((psvm+rtrata_svm)->estado==PARADA) // orden de cierre
        {
           break;
        }
 		fprintLog(nomProg,"Lanza test con subsistema_general >%.2s< y Csubapl >%.2s< \n", subsistema_general,Csubapl);
		retorno = trata_tst(subsistema_general,Csubapl);
 		fprintLog(nomProg,"Lanza test con subsistema_general >%.2s<, Csubapl >%.2s< y salida >%d< \n", subsistema_general,Csubapl,retorno); //PTASK0022158
    if(retorno == 0) // No hay respuesta //PTASK0022158 - antes if(retorno)
		{
			fprintLog(nomProg,"--- Retorno TEST = 0 = No hay respuesta\n");
      respuesta_tst = 0;
		}
        else
		{
			fprintLog(nomProg,"--- Retorno TEST = 1 = Si hay respuesta\n");
			respuesta_tst = 1;
		}
     }//fin while respuesta_tst
	 
	 	 
     if ((psvm+rtrata_svm)->estado==PARADA) // orden de cierre
     {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"orden de cierre 3\n");
#endif	     
        break;
    }
	 
	fprintLog(nomProg,"*** Va a trata_diferido ***\n");

    retorno = trata_diferido();
    if(retorno==1) // no hay mas registros por ahora
    {
		fprintLog(nomProg,"*** No mas registros ***\n");
        fin_fich =1; 
        break;
    }
    if(retorno==2) //cierro el semaforo para que hasta que no se conteste un test no lo intente de nuevo
    {
		fprintLog(nomProg,"*** Cierre de semaforo ***\n");
        fin_fich =1;
       //20160704 - cierra el semaforo
        (psvm+subsis_svm)->str_svm.svmpars.pasemsub = 0;
        respuesta_tst = 0;
        break;
    }
	 
   }//fin while de tratar diferido
   desconexion_BBDD(); //desconecto la bbdd al finalizar el servicio

   /***** Comprobamos las operaciones a recuperar de Descuadres ****/
   fprintLog(nomProg,"No hay diferidos\n");

   /* ------  resetea el manejador de alarma  --------- */
   //vuelve a asignar la funcion a la seÃ±al 
   if (signal(SIGALRM, activa_rdif) == SIG_ERR)
   {
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"Error signal \n");
      perror("signal");
#endif
      strcpy(coderror, "999");
      exit(finalizar(-2)); // supervis intenta arrancar
   }

   if ((psvm+rtrata_svm)->estado==PARADA) /* orden de cierre */
   {
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"orden de cierre 4 \n");
#endif
      strcpy(coderror, "999");
      if ((psvm+rtrata_svm)->estado==RECARGAR)
        exit(finalizar(-2)); /* Paro para rearranque */
      else
        exit(finalizar(0)); /* Orden de Cierrre */ 
   }
}

#include "trata_diferido.c"
#include "envia_diferido.c"
#include "tstautom.c"
