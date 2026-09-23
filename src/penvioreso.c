//PRJ0247167 - Revisado ampliación P48
#include "ftimer.h"
/***************************************************************************
   PROGRAMA:  PENVIO
   DESCRIPCION: Proceso residente encargado del tratamiento de los mensajes
                que llegan  del nucleo y deben enviarse al RESOLUTOR. 
				
   - mensaje del nucleo a RESO : se comprueba el estado del semaforo, 
     se traduce, se envia a la cola de salida, y,
     si es una peticion, se lanza el control de time-out
****************************************************************************/

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
#ifdef MDIGITAL
#include <sys/uswitch.h>
int VariableEntera = 0;
#endif
#include "netwin.h"
#include "cforitno.h"
#include "nptinci.h"
#include "nptcasa.h"
#include "datagen.h"
#include "deffich.h"
#include "svm.h"
#include "colas.h"
#include "procesos.h"
#include "msg_nucl.h"

#include "cformoni.h"
#include "ftradreso.h"
#include "fclasmsg.h"

#include "npttrms.h"
#include "nptpara.h"
NPSINCI  reginci;
formato_interno fiforitn, *pforitn; 
//xavi
//char  cadenasal [1024]; 
//Raquel comprobar el tamaño de la cadena para el p48
char  cadenasal [4024];
int   longi;

int   tsubsis[ULTPUC+1];
   
/***  --------- prototipos de funciones  ----------------  ***/
void  fprintLog(char ficheroPlantilla[50], char* format, ...);
void  sigue(); 
int   finalizar(int codigo); 

int   fcarga_tablas(void);
int   fclase_tipomsg(int tipomsg);
int   fenvia_reso (char * cadenasal,int longi);
int     fresponde_tratami (formato_interno *pfi);
char    *itoa(int numero);
int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
int busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int damecola(SVM *psvm_par,char *proce,char *subsis);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
long milelong(void);
int     ftrad03(int sentido, int *long_iso, char *mensaje_reso,
                formato_interno *mensaje_netplus, int tipMsj);				
int     fsemaforo_red(int ll); 
int pon_ftimer(formato_interno * pforitn, int pid_ptimer, int segundos, int tipo_tiempo);
int conexion_BBDD();

char prog_timer[11];
char nombre_cola[11];
int pid_ptimer;
int posic_ptimer;
int posic_subsis;
char nomProg[50];  
 //char *nomProg= (char *)malloc(50); 
 int   sfd_hsm;

/* para el ftrad03 */
int sentido; 
int long_iso;

char mensaje_reso[4024]; 
//para APLSIO TIENE QUE SER 4000
int rtrata_svm;
int fverfi(formato_interno fi);

 typedef struct strMenReso
{
    char sMensaje   [4024];
} strMenReso;
strMenReso sMenAlReso;



/***  ---------------   mensaje a enviar   -------------   */
   mensaje_nuc    msgnuc; 
   int long_msgnuc = sizeof(msgnuc.fiforitn); //PRJ0258452 - quitado = sizeof(msgnuc) - sizeof(msgnuc.tipo); 
   
char p04[3];
char  coderror[4];
int clasemsg;
int qmanej,shmid; 
int qrtratami; 
SVM *psvm;
NPSPARA  *reg_para;
 
int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
   int pid, tabla, trata_mensaje,ll;
   tm_dec  *tm,vtm;
   int retorno,queda;
   int tipomsg; 
   char w_tipomsg[5]; 
   int qenvio;
   char ch_003[3];
   char auxsubsis[5];
   
   char claveB[13];
   int err;
   int j;

#ifdef MDIGITAL
#include "dignull.c"
#endif

   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);
 
   //asignar el nombre
   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);


#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"********* inicio penvioreso *********\n");
#endif

   strcpy(coderror, "   ");
   strcpy(subsistema_general,argv[0]+strlen(PENVIO));
   strcpy(reginci.innompro,"ENVIO");
   strcat(reginci.innompro,subsistema_general);

   /* --------- se ata a la memoria compartida SVM  ------------ */

	 retorno=conecSVM((void *)&psvm); //ORIGINAL
	 
    if (retorno != 0)
    {
/*#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif*/
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
    }

    rtrata_svm=busca_proc_svm(psvm,argv[0],-1);

    if (rtrata_svm < 0 )
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR ARRANQUE NO ESTOY EN SVM %s\n", argv[0]);  
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
    }      
	
    /* crea cola de mensajes de PENVIO  */
    qenvio=(psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
	
    if (qenvio == -1)  
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error crea cola qenvio\n");  
#endif
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"QENVIO");
       strcat(reginci.innomfic,subsistema_general);
       strcpy(coderror, "009");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Crear QENVIO%s codigo de error >%.3s<\n",subsistema_general, coderror);

       exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
    }
	
   /*  se asocia la cola de mensajes del NUCLEO  */

    //RAQUEL CAMBIADO COMPROBAR SI FUNCIONA CON EL DISPLAY
  for (j=0 ; j<2 ; j++) 
  {
	if (strncmp(subsistema_general,reg_para[j].psresolu,2)==0) 
    {
	  qrtratami = damecola(psvm,reg_para[j].pscltrat,NULL); 
	  break;
	} 

  }
     if (qrtratami == -1)  
     {
#if defined (TRAZA) && (TRAZA > 1)
           perror("error >>>");
           fprintLog(nomProg,"error crea cola  QRTRAT\n");
#endif
           strcpy(reginci.innomfic,"QRTRAT00");
           strcpy(coderror, "009");
          //Añadido para Monitorizacion SISMA 
           fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Crear QRTRAT codigo de error >%.3s<\n", coderror);
       
           exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
      }
	
   /*  se asocia la cola de mensajes del MANEJ  */
    sprintf(nombre_cola,"%s%s",PMANEJ,subsistema_general);
 
    qmanej = damecola(psvm, nombre_cola,NULL);

    if (qmanej == -1)  
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error crea cola %s errno >%d<\n",
                  nombre_cola, errno);
#endif
       strcpy(reginci.innomfic,"QMANEJ");
       strcat(reginci.innomfic,subsistema_general);
       strcpy(coderror, "009");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Crear QMANEJ%s% codigo de error >%.3s<\n",subsistema_general, coderror);

       exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
    }
   
   /****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE 
           LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR  
   *******/   
     
   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   
  // queda=alarm(0);
  // fprintLog(nomProg,"RAQUEL PRECEPRESO queda >%d<\n", queda);
   
   fprintLog(nomProg,"pid %d\n", getpid());
   
   // graba su pid en la SVM 
   (psvm+rtrata_svm)->estado=ESPERA;
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg, "se duerme\n");
#endif   
   pause();
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg, "despierta\n");
#endif   
   queda=alarm(0);
   if (queda==0)
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR ARRANQUE queda >%d<\n", queda);
#endif
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
      //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – MUERTE DE PROCESO:  %s queda >%d< codigo de error >%.3s<\n",nomProg,queda, coderror);

       exit(finalizar(-1));
   } 
   
   (psvm+rtrata_svm)->estado=TRABAJO;
   //-------- obtiene el pid del proceso de control del time-out ----
   sprintf(prog_timer,"ptimer%s",subsistema_general);
   posic_ptimer = encuentra_svm(psvm,prog_timer,TIP_PROCES);         
   if (posic_ptimer < 0)
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR %s no esta en svm \n",prog_timer);  
#endif
       strcpy(reginci.innomfic,"PTIMER");
       strcat(reginci.innomfic,subsistema_general);
       strcpy(coderror, "011");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – MUERTE DE PROCESO:  %s prog_timer >%s< codigo de error >%.3s< el proceso no esta en SVM\n",nomProg,prog_timer, coderror);

       fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
   }  
   
   // -- obtiene el desplazamiento del puntero para los datos del subsistema --
   // Carga la posicion de los distintos subsistemas en la tabla tsubsis 
   sprintf(auxsubsis, "%.2s%.2s", subsistema_general, "01");
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg, "subsistema resolutor >%s<\n", auxsubsis );
#endif
   
   posic_subsis= encuentra_svm(psvm,auxsubsis,TIP_SUBSIS);
      
   if (posic_subsis < 0)
   {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg, "Error - No encuentra subsistema en svm\n");
#endif
     strcpy(coderror, "502");
     exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – MUERTE DE PROCESO:  %s  codigo de error >%.3s< el subsistema %s no esta en SVM\n",nomProg,auxsubsis, coderror);
  
   }
   
   ////////// ABRE LA CONEXION A LA BASE DE DATOS Y PERMANECE ABIERTA, SE CIERRA EN FINALIZAR
	retorno = conexion_BBDD();
	if(retorno)
	{
	   fprintLog(nomProg,"Error en el acceso a la base de datos\n");
      //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Conexion codigo de error >%i< \n",retorno);

	   exit(finalizar(-2)); //supervis intenta rearrancar
	}	

   //****** BUCLE DE TRATAMIENTO DE MENSAJES ******
   trata_mensaje = 1; 
   
   // ---------- lee con espera de la cola de mensajes ---------------
   while (trata_mensaje == 1)
   {
#if defined (TRAZA) && (TRAZA > 5)
    fprintLog(nomProg,"///lee con espera de cola///>%d<///>%d< Bytes///\n", qenvio, long_msgnuc);
#endif	
    if(msgrcv(qenvio, &msgnuc, long_msgnuc, 0, 0) == -1)
    {
#if defined (TRAZA) && (TRAZA > 1)
     perror("error >>>");  
     fprintLog(nomProg,"error leer cola \n");  
#endif
     strcpy(coderror, "242");
     strcpy(reginci.innomfic,"qenvio");  
     strcat(reginci.innomfic,subsistema_general);
     //Añadido para Monitorizacion SISMA 
     fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Lectura qenvio%s codigo de error >%.3s< \n",subsistema_general,coderror);

     exit(finalizar(-1));
    } 
 #if defined (TRAZA) && (TRAZA > 5)
    fprintLog(nomProg,"/////leido tipo//>%d<///\n", msgnuc.tipo);
    //fhexdump(stderr, &msgnuc, long_msgnuc);

#endif

        //  -------------  orden de cierre  ------------------  
    if (msgnuc.tipo == TIPO_PARADA)  // orden de cierre 
    {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"orden de cierre\n");
#endif
     strcpy(coderror, "999");
     if ((psvm+rtrata_svm)->estado==RECARGAR)
       exit(finalizar(-2)); // Paro para rearranque
     else
       exit(finalizar(0)); // Orden de Cierrre 
    }
#if defined (TRAZA) && (TRAZA > 5)
	fverfi(msgnuc.fiforitn);
#endif	
    //  ----------  prepara las variables para traducir  ----------  
			 
	tipomsg = atoi(msgnuc.fiforitn.fiisocom.fiiso000); 
	
	//RAQUEL SE CAMBIA EL IF A PELO POR EL CAMPO DE LA TABLA NTPPARA 
  //  if ((strncmp(subsistema_general, "03", 2) == 0) || (strncmp(subsistema_general, "05", 2) == 0))
	if ((strncmp(subsistema_general, reg_para[j].psresolu, 2) == 0))
    {
      sentido = 2;

      retorno = ftrad03(sentido, &long_iso, mensaje_reso
	      , &msgnuc.fiforitn, tipomsg);

    }
    if(retorno)
    {
#if defined (TRAZA) && (TRAZA > 1)	
       fprintLog (nomProg,"error traduccion reso \n");
#endif	   
       strcpy(msgnuc.fiforitn.fidatpro.ficodfin,"280");
       strcpy(coderror, "299");
       fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
       strncpy(reginci.incoderr,"        ",8);
       continue;
     }

     
//--------   comprueba el tipo de mensaje a enviar    ---------------
      clasemsg = fclase_tipomsg(tipomsg);
      
      if (clasemsg == DESCONOCIDO)
      {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"tipo mensaje desconocido %d\n",tipomsg);
#endif
         strcpy(msgnuc.fiforitn.fidatpro.ficodfin,"280");
         strcpy(coderror, "301");
          //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL – TIPO DE MENSAJE NO CONTEMPLADO: mensaje >%d< codigo de error >%.3s<\n",tipomsg, coderror);
 
         //memcpy(reginci.inmensaj, &msgnuc.fiforitn, sizeof(reginci.inmensaj));
         fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
         retorno = fresponde_tratami(&msgnuc.fiforitn);
         continue;                     
      }
 
      // ----------  envia la operacion a reso --------
         printf("nombreficher %s\n", nomProg );
      if ((retorno = fenvia_reso(mensaje_reso, long_iso)) != 0)
      { 
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"error al enviar a reso\n");
#endif
         fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
         retorno = fresponde_tratami(&msgnuc.fiforitn);
      }
	  
   }  // fin while tratamensajes   
   
} //  fin main


/****************************************************************************
   FUNCION   FENVIA_RESO
   DESCRIPCION: Envia un mensaje al manejador de salida hacia reso
****************************************************************************/
int fenvia_reso (char * cadenasal,int longi)
{
   int tipomsg,tiempo_espera,retorno;
   int errmsg;
   char longichar[4]; 
   long tiempoML;
   int tiempo2;
   char * prueba;

   mensaje_comunicacion    msglin; 
   int  long_msglin = sizeof(msglin); 
   
 
  //  SI EL RESOLUTOR DE LA OPERACION ES ESTE SUBSISTEMA GUARDO LOS TIEMPOS	
  //  (SI ES UNA RESPUESTA HACIA LA ENTIDAD NO)

   if(strncmp(msgnuc.fiforitn.fidatpro.firespri,subsistema_general,2)==0)
   {
     //  tiempo de entrada al resolutor en milisegundos desde las 0 horas de ese dia
     sprintf (msgnuc.fiforitn.fidatpro.fitienre, "%08d" , milelong());
#if defined (TRAZA) && (TRAZA > 10)	 
     printf("nombreficher %s\n", nomProg );
     fprintf(stdout, "fitienre>%s<\n", msgnuc.fiforitn.fidatpro.fitienre);
#endif	 
   }

//////////////////////////////
/***elige el time-out para esta operacion***/ 

    //msgnuc.tipo = atof(msgnuc.fiforitn.fidatpro.finumope);
/***--------   comprueba el tipo de mensaje a enviar    ---------------***/
   tipomsg = atoi(msgnuc.fiforitn.fiisocom.fiiso000);
	fprintLog(nomProg,"despues del atoi tipomsg %i\n",tipomsg); 
//DMND0036176 se añaden respuestas a preautorizaciones
    switch(tipomsg)
    {
    case 1121:
    case 1221:
    case 1421:
    case 1521:
       tiempo_espera = (psvm + posic_subsis)->str_svm.svmpars.patrerep;
       break;
    case 1804:
       if (strcmp(msgnuc.fiforitn.fiisocom.fiiso024,"802") == 0)
          tiempo_espera = (psvm + posic_subsis)->str_svm.svmpars.patresnf;
       else
          tiempo_espera = (psvm + posic_subsis)->str_svm.svmpars.patrepet;
       break;
    default:
       tiempo_espera = (psvm + posic_subsis)->str_svm.svmpars.patrepet;
       break;
    }
/***   lanza el timer   ***/
    pid_ptimer=(psvm+posic_ptimer)->str_svm.svmproc.pid;
    //utilizamos milisegundos.
// #if defined (TRAZA) && (TRAZA > 10)
    fprintf(stdout, "Llama a pon_ftimer con >%d< segundos\n", tiempo_espera);
 //#endif

     retorno=pon_ftimer(&msgnuc.fiforitn, pid_ptimer,
                        tiempo_espera, TMP_REL);
  
     if (retorno)
     {
       strcpy(coderror, "045");
     }
     else
	 {

        /***  ---------------   mensaje a enviar   -------------   */
       msglin.tipo = tipomsg;
       
        //longitud del formato resolutor + 4 de su longitud + 4 del tipo
       sprintf(longichar,"%04d",longi+4);

       strncpy(msglin.long_mensaje,longichar,4);
 
       memcpy (msglin.mensaje_linea , cadenasal, longi);  

       //fprintLog(nomProg, "cadenasal >%s<\n");
   
       signal(SIGALRM,sigue); 
       alarm(ESSNDCOL);
       
       //longi +4 (formato03 + 4 de longitud)
       errmsg = msgsnd(qmanej, &msglin, longi+4 ,IPC_NOWAIT);
#if defined (TRAZA) && (TRAZA > 10)  
       fprintf(stdout, "Envia por qmanej>%d<\n", qmanej );
       
#endif   
       if(errmsg)
       {
         alarm(0);
         perror("msgsnd >>>>>>");
         strcpy(coderror, "244");
#if defined (TRAZA) && (TRAZA > 1)	  
	     fprintLog(nomProg, "Error al enviar por la cola\n"); 
#endif	 
        //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio cola qmanej >%i codigo de error >%.3s<\n",qmanej,coderror); 
  
         retorno = 1;
       }
       alarm(0); 
	 }   
     return(retorno);
}

#include "frespTra.c"
#include "ftrad03.c"
#include "fsemaforo_red.c"
#include "fclasmsg.c"
#include "ftimer.c"
#include "pon_ftimer.c"
