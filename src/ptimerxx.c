/***************************************************************************
   PROGRAMA:  PTIMERXX
   DESCRIPCION: Proceso residente encargado del control de time-outs de los
                mensajes que se envian a los resolutores 

                Este proceso esta en pause hasta que los despierta otro
                proceso o salta su alarma.
                Cuando se despierta compara la hora de expiracion de los
                mensajes de la tabla de timer con la hora actual, si ha
                expirado su tiempo y no ha sido respondido, modifica la
                clave del mensaje y lo entrega al RTRATAMI, en caso
                contrario selecciona el menor de ellos y espera.
***************************************************************************/
/***************************************************************************
 Modifica los campos de la clave de tabla de operaciones de la siguiente
manera:
  Si al arrancar encuentra operaciones en vuelo
   -- "40" en posicion 6

  Si salta el timeout
   -- "41" en posicion 8
  Si es por respuesta no confirmada:
    -- "07" en posicion 6
  Si es una peticion
    -- subsistema_general en posicion 4
  El escribir la posicion 6 implica que se va a generar una anulacion.
   Si el valor es "07" se totaliza en caso contrario no.
  Se diferencia entre operaciones en vuelo al arrancar y timeout normal
  para poder tener otra tabla de operaciones y evitar respaldos tardios.
***************************************************************************/
 
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

#include "cforitno.h"
#include "nptinci.h"
#include "datagen.h"
#include "deffich.h"
#include "svm.h"
#include "colas.h"
#include "procesos.h"
#include "nptcasa.h"
#include "ftimer.h"
#include "msg_nucl.h"
#include "nptpara.h"

NPSINCI  reginci;
formato_interno fiforitn, *pforitn;
mensaje_nuc msg;
int long_msg = sizeof(msg.fiforitn); //PRJ0258452 - quitado = sizeof(msg) - sizeof(msg.tipo);

/***  --------- prototipos de funciones  ----------------  ***/
void sigalar_manejador(int);
void sigue();
int finalizar(int codigo);

int fenvia_rtratami (formato_interno *pforitn);
int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
int busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int limpia_fichero_casar();
int damecola(SVM *psvm_par,char *proce,char *subsis);
void fprintLog(char ficheroPlantilla[50], char* format, ...);
int rtrata_svm;
char nomProg[50];
char  coderror[4];
int qrtratami,shmid;
SVM *psvm;
int pid_limpia;
char cadena[200];
//int retorno;
int t_alarma, t_menor;
int t_max = 2; // cada t_max segundos revisa op no contestadas si no le llega se𠪍
time_t t_exp=0, t_actual;
char subsistema_general[3];
SVMtimer *PUN_SVMtimer;
SVMtimer *PUN_SVM_i_timer;
int finalizar(int codigo);
int Nsub;

NPSPARA  *reg_para;

int main(int argc,char *argv[])    /***     FUNCION PRINCIPAL     ***/
{
   int retorno,queda;

   char clave_pr[2];
   int resultado=0;
   int ll;
   
   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);

   // signal(par1, par2) --> funcion para capturar una se𠪍
   // par1 --> identificador de la se𠪠(SIGALARM es la se𠪠de terminacion de un temporizador)
   // par2 --> orden o funcion cuando se activa la se𠪠(SIG_IGN es ignorar)
   signal(SIGHUP,SIG_IGN); 
   signal(SIGINT,SIG_IGN); // interrupcion con ctrl + c
   signal(SIGQUIT,SIG_IGN); 
   signal(SIGTERM,SIG_IGN);
   signal(SIGCHLD,SIG_IGN);


#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg, "inicio timer\n");
#endif
   pid_limpia = 0;
   strcpy(subsistema_general, argv[0] + strlen(PTIMER));
   strcpy(coderror, "   ");
   sprintf(reginci.innompro, "TIMER%.2s", subsistema_general);

   Nsub=atoi(subsistema_general);

   /* --------- se ata a la memoria compartida SVM  ------------ */
	 retorno=conecSVM((void *)&psvm); //ORIGINAL

    if (retorno != 0)
    {
/*#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif*/
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //supervis no puede arrancar sin svm
    }
    rtrata_svm=busca_proc_svm(psvm, argv[0], -1);
    if (rtrata_svm < 0 )
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "ERROR ARRANQUE NO ESTOY EN SVM\n");
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "250");
       exit(finalizar(-1)); //supervis no puede arrancar sin svm
    }

   /*  se asocia la cola de mensajes del NUCLEO  */
   // qrtratami = damecola(psvm,RTRATAMI,NULL);//ORIGINAL
  //RAQUEL se cambia para recoger los datos por bbdd y entidad
	 
  for (ll=0 ; ll<2 ; ll++) 
  {  
    if ((strncmp(reg_para[ll].psnument,subsistema_general,2)==0) ||
	    (strncmp(reg_para[ll].psresolu,subsistema_general,2)==0) ||
		(strncmp(reg_para[ll].psmonito,subsistema_general,2)==0)) 
    {	
        qrtratami = damecola(psvm,reg_para[ll].pscltrat,NULL);		
        break;            
    }  
  } 

	if (qrtratami == -1)
	{
#if defined (TRAZA) && (TRAZA > 1)
			fprintLog(nomProg, "error crea cola  QRTRAT00\n");
#endif
			strcpy(reginci.innomfic,"QRTRAT00");
			strcpy(coderror, "009");
			//Añadido para Monitorizacion SISMA 
			fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion QTRTRAT00 codigo de error >%.3s<\n",coderror);

			exit(finalizar(-1)); //supervis no puede arrancar sin svm
	}
 
   /****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
           LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR
   *******/

   // TIMEWAIT es 5 segundos
   //si termina el temporizador, ejecuta sigue(), pero sigue no hace nada.
   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   /*   graba su pid en la SVM  */
  // queda=alarm(0);
  // fprintLog(nomProg,"RAQUEL PTIMER queda >%d<\n", queda);
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   (psvm+rtrata_svm)->estado=ESPERA;
   
   // queda suspendido hasta que recibe una se𠪠del proceso de arranque o pasen los 10seg
   pause();
   // cuando se despierta se reinicia el temporizador con alarm(0)
   // si se habia consumido todo el tiempo anterior (!queda) damos error de arranque.
   queda=alarm(0);
   if(!queda)
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "ERROR ARRANQUE\n");
#endif
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - MUERTE DE PROCESO: codigo de error >%.3s<\n",coderror);

       exit(finalizar(-1)); //supervis no puede arrancar sin svm
   }
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg, "ARRANQUE pid >%d<\n",
                 (psvm+rtrata_svm)->str_svm.svmproc.pid);
#endif

  //Tiempo maximo para el temporizador para revisar operaciones no contestadas
   t_alarma = t_max;
   (psvm+rtrata_svm)->estado=TRABAJO;
  
  //Se define lo que queremos que ocurra cuando llega la se𠪠SIGALARM
  //si no es posible, el sistema devuelve SIG_ERR
  if (signal(SIGALRM, sigalar_manejador) == SIG_ERR)
  {
#if defined (TRAZA) && (TRAZA > 1)
     perror("signal");
#endif
     (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
     exit(finalizar(-1)); //supervis no puede arrancar
  }
  
  (psvm+rtrata_svm)->senal=SIGALRM;
  
  //RAQUEL el timer es algo que no podemos cambiar hay que añadir manualmente en 
  //el svm.h y aqui los timers nuevos si hay nuevas entidades  
   switch (Nsub)
   {
     case 03:
       PUN_SVMtimer= &(psvm->tb_timer03); 
       break;
     case 05:
       PUN_SVMtimer= &(psvm->tb_timer05);
	   break;
	 case 07:
       PUN_SVMtimer= &(psvm->tb_timer07);
       break;
     case 20:
       PUN_SVMtimer= &(psvm->tb_timer20);
       break;
     case 30:
       PUN_SVMtimer= &(psvm->tb_timer30);
       break;
     case 40:
       PUN_SVMtimer= &(psvm->tb_timer40);
	   break;
	 case 10:
       PUN_SVMtimer= &(psvm->tb_timer10);
       break;
  
     default:

#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg, "ERROR SUBSISTEMA >%s<\n", subsistema_general);
      exit(finalizar(-1)); //supervis no puede arrancar
#endif
   } //del switch //RAQUEL ahora el else
  
     ////////// ABRE LA CONEXION A LA BASE DE DATOS Y PERMANECE ABIERTA, SE CIERRA EN FINALIZAR
	retorno = conexion_BBDD();
	if(retorno)
	{
	   fprintLog(nomProg,"Error en el acceso a la base de datos\n");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: apertura BBDD codigo de error >%i<\n",retorno);
 
	   exit(finalizar(-2)); //supervis intenta rearrancar
	}

  // se llama por primera vez a sigalar_manejador
  // luego se ejecutara cada vez que salte la se𠪠del temporizador
  // o cuando le llegue una se𠪠desde el penvio con ftimer.
  sigalar_manejador(0);
  while (1)
  {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"se duerme t_alarma >%d<\n", t_alarma);
#endif
     alarm(t_alarma);
     pause();

   /*  -------------  orden de cierre  ------------------  */
      if ((psvm+rtrata_svm)->estado==PARADA) /* orden de cierre */ 
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"orden de cierre \n");
#endif
        strcpy(coderror, "999");
        if ((psvm+rtrata_svm)->estado==RECARGAR)
        {
          //Añadido para Monitorizacion SISMA 
          fprintLog(nomProg,"TIPO SISTEMA - REARRANQUE DE PROCESO: codigo de coderror >%i<\n",coderror);
 
          exit(finalizar(-2)); /* Paro para rearranque */
        }
        else
        {
          exit(finalizar(0)); /* Orden de Cierrre */
        }
      }
  }
}


/*************************************************************************
   funcion de manejo de alarma
**************************************************************************/
void sigalar_manejador(int sig)
{
  int  retorno;
  NPSCASA reg_casa;
  char clave_casa[7];
  int ii = 0;

 // una vez que entra en la funcion ignora el resto de se𠫥s
 // hasta que termina que vuelve a restaurarla
 if (signal(SIGALRM,SIG_IGN) == SIG_ERR)
 {
    fprintLog(nomProg, "error al ignorar SIGALRM\n");
    perror("signal");
 }
 ii=alarm(0);
 retorno=0;
#if defined (TRAZA) && (TRAZA > 1)
 fprintLog(nomProg, "Despierta quedando >%d< segundos\n", ii);
#endif
 ii=0;
 pforitn=&msg.fiforitn;

#if defined (TRAZA) && (TRAZA > 1)
 if ((psvm+rtrata_svm)->estado==PARADA) /* orden de cierre */
     fprintLog(nomProg,"Es la parada y estoy en el manejador\n");
#endif
 time(&t_actual);
 if (PUN_SVMtimer->t_exp == 0)
 {
     t_alarma=t_max;
 }
 else  /* trata tabla de msg pendientes */
 {
     ii=1;
     //tiempo mas proximo en que debe saltar la alarma
     t_menor = t_actual + t_max;
	 
  //RAQUEL el timer es algo que no podemos cambiar hay que añadir manualmente en 
  //el svm.h y aqui los timers nuevos si hay nuevas entidades  
    while(ii<MAX_REG_SVM) /** tratamiento de la tabla de msg pendientes **/
    {
      switch (Nsub) 
      {
        case 3:
           PUN_SVM_i_timer= &((psvm+ii)->tb_timer03);
           break;
        case 5:
           PUN_SVM_i_timer= &((psvm+ii)->tb_timer05);
           break;
        case 7:
           PUN_SVM_i_timer= &((psvm+ii)->tb_timer07);
           break;
        case 20:
           PUN_SVM_i_timer= &((psvm+ii)->tb_timer20);
           break;
        case 30:
           PUN_SVM_i_timer= &((psvm+ii)->tb_timer30);
           break;	
        case 40:
           PUN_SVM_i_timer= &((psvm+ii)->tb_timer40);
           break; 
        case 10:
           PUN_SVM_i_timer= &((psvm+ii)->tb_timer10);
           break;           
      }
	 
       if(PUN_SVM_i_timer->numope) /* no ha sido respondido */
       {
#if defined (TRAZA) //&& (TRAZA > 100)
          fprintLog(nomProg,"ii >%d< de MAX_REG_SVM >%d<\n", ii, MAX_REG_SVM);
          fprintLog(nomProg,"PUN_SVM_i_timer->numope >%d<\n", PUN_SVM_i_timer->numope);
          fprintLog(nomProg,"t_actual = %d\n",t_actual);
	  fprintLog(nomProg,"t_expira = %d\n",PUN_SVM_i_timer->t_exp);
#endif

          if(PUN_SVM_i_timer->t_exp <= t_actual)
          {

             fprintLog(nomProg,"** operacion >%d< expirada **\n", PUN_SVM_i_timer->numope); //PRJ0258542 - Fase 3 - Cambio 11052026
             sprintf(clave_casa, "%06d", PUN_SVM_i_timer->numope);
             strncpy(reg_casa.canumope, clave_casa, 6);
             retorno = tratar_fich("NPTCASA", IGUALBORRAR, &reg_casa, clave_casa);

             if(!retorno)  /* el mensaje esta sin responder */
             {
                memcpy(pforitn, &reg_casa.caforitn, sizeof(formato_interno));
                fprintLog(nomProg,"** operacion >%d< no contestada **\n", PUN_SVM_i_timer->numope); //PRJ0258542 - Fase 3 - Cambio 11052026

          			/* en la posicion de clave-5 pone 41(time_out) */
                strncpy(pforitn->fidatpro.ficlatab+8,"41",2);

              	// ponemos codigo de finalizacion 297 (timeout)
	      				// y que la operacion fue resuelta por sisma
	        			strncpy(pforitn->fidatpro.ficodfin,"297",3);
	        			strncpy(pforitn->fidatpro.firesfin,"00",2);
								printf("** mensaje a enviar %s **\n", pforitn->fiisocom.fiiso000); //PRJ0258542 - Fase 3 - Cambio 11052026
								if (strncmp(pforitn->fiisocom.fiiso000, "1804", 4) == 0 && (strncmp(pforitn->fiisocom.fiiso024, "811", 3) == 0 || strncmp(pforitn->fiisocom.fiiso024, "816", 3) == 0)) //PRJ0258542 - Fase 3 - Cambio 11052026
	  						{
	  							strncpy(pforitn->fiisocom.fi032094.datos, "1805", 4);
	  							strncpy(pforitn->fidatpro.ficodfin, "   ", 3);
	  							strncpy(pforitn->fidatpro.firesfin, "  ", 2);
	  						}
	  						retorno = fenvia_rtratami(pforitn);
              	sleep(3);
             }
	     else
	     {
       			fprintLog(nomProg,"** operacion >%d< si contestada **\n", PUN_SVM_i_timer->numope); //PRJ0258542 - Fase 3 - Cambio 11052026
	     }
	   // mensaje respondido o tratado, libera la entrada
             PUN_SVM_i_timer->numope = 0; /* libera la entrada */
             PUN_SVM_i_timer->t_exp = 0; /* libera la entrada */
	  } 
          else //tiempo no expirado
          {
	        fprintLog(nomProg,"tiempo no expirado, asigna menor tiempo\n");
	        fprintLog(nomProg,"PUN_SVM_i_timer->t_exp >%d<\n", PUN_SVM_i_timer->t_exp);
	        fprintLog(nomProg,"t_menor                >%d<\n", t_menor);
                if(PUN_SVM_i_timer->t_exp < t_menor)
                {
                  t_menor=PUN_SVM_i_timer->t_exp;
                  PUN_SVMtimer->t_exp=PUN_SVM_i_timer->t_exp;
                }
          }
       }
       ii++;
    }/*** fin while ***/  
    t_alarma=t_menor-t_actual;
    if ((t_alarma >= t_max) || (t_alarma <= 0))
    {
       t_alarma=t_max;
    }
 }
 if (signal(SIGALRM, sigalar_manejador) == SIG_ERR)
 {
#if defined (TRAZA) && (TRAZA > 1)
    fprintLog(nomProg,"error al activar manejador de alarma \n");
    perror("signal");
#endif
    exit(-1);
 }
}

#include "ftimer.c"
#include "fenvia_rtratami.c"
#include "pon_ftimer.c"
