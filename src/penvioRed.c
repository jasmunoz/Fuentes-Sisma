//PRJ0247167 - Revisado ampliación P48
/***************************************************************************
   PROGRAMA:  PENVIO
   DESCRIPCION: Proceso residente encargado del tratamiento de los mensajes
                que llegan del nucleo y deben enviarse a la red.Este proceso
                lee de su cola de entrada con espera, y puede tratar :
   - cierre del proceso : el proceso se descarga
   - carga de tablas de traduccion : carga en memoria de los ficheros

   - mensaje del nucleo a RED : se comprueba el estado del semaforo,
     se traduce, se envia a la cola de salida, y,
     si es una peticion, se lanza el control del timeout
****************************************************************************/

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>


#include "netwin.h"
#include "cforitno.h"
#include "nptinci.h" 
#include "nptcasa.h"
#include "datagen.h"
#include "deffich.h"
#include "svm.h"
#include "colas.h"
#include "procesos.h"
#include "ftimer.h"
#include "nptdhsm.h"
#include "nptiden.h"
          
          
#include "ftradred.h"
#include "nptpara.h"

NPSINCI  reginci;

int   tsubsis;
int   sfd_hsm;

//la conexión tiene que ser por ssl
typedef struct sfdssl {         /* estructura conjunta para socket y socket seguro */
 int sfd_ssl;
 SSL * ssl_ssl;
} sfdssl;

 struct sfdssl stsocketssl;
 struct sfdssl * sockssl;


//la conexión tiene que ser por ssl

/***  --------- prototipos de funciones  ----------------  ***/
void sigue();
int finalizar(int codigo);
int     fcarga_tablas(char * red);
int     fclase_tipomsg(int tipomsg);
int     ftradred(char usotrad,int sentido,int suba_iso,int *long_iso,
                   char *mensaje_puc,formato_interno *mensaje_netplus,
                   int *error_code,int *error_bit);
int     fsemaforo_red(int ll);
int guarda55(formato_interno * pforitn, char *comando); //PTASK000024654 - mete comando a la llamada
int     fenvia_red (char * cadenasal,int longi,int clasemsg);
int     fresponde_tratami (formato_interno *pfi);
char    *itoa(int numero);
int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
void fprintLog(char ficheroPlantilla[50], char* format, ...);
int busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int damecola(SVM *psvm_par,char *proce,char *subsis);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int ver_fi(FILE * fic,formato_interno fi);
void fechorsys(struct tm_dec *tmPtr);
int ftimer(formato_interno * pforitn, int pid_ptimer,int segundos, int tipo_tiempo);
int conexhsm(char* codent, NPSDHSM registro);
void desconexhsm(int sock);
//xavi la conexión tiene que ser por ssl
void conexhsm_C(char* codent, char *comando, struct sfdssl * sockssl, char * codResp);
//xavi la conexión tiene que ser por ssl
char comando[50]; //PTASK000024654
int crear_svmsockI(void **gen_psvm,int *shmid,int operacion);
int crear_svmsockA(void **gen_psvm,int *shmid,int operacion);
int pon_ftimer(formato_interno * pforitn, int pid_ptimer, int segundos, int tipo_tiempo);
long milelong();
int conexion_BBDD();



char PCLIE[11];
int pid_ptimer;
int posic_subsis, rtrata_svm;

/***  ---------------   mensaje a enviar   -------------   */
//PRJ0247167 - Revisado ampliación P48 - no es necesario porque ya se ha definido la longitud del mensaje
mensaje_nuc    msgnuc;
int long_msgnuc = sizeof(msgnuc.fiforitn); //PRJ0258452 - quitado = sizeof(msgnuc) - sizeof(msgnuc.tipo);

char p04[3];
char  coderror[4];
int clasemsg;
int qmanej, qrtratami;
SVM *psvm;
char nomProg[50];

NPSPARA  *reg_para;


int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
   int pid, tabla, trata_mensaje,ll;
   tm_dec  *tm,vtm;
   int retorno,queda;
   int tipomsg; 
   char w_tipomsg[4];
   char auxsub[5];
   int qenvio;
   char ch_003[3];
   NPSDHSM registro;
   char codResp[3];
   char cola_client[9];
   int j;

   
//la conexión tiene que ser por ssl   
  sockssl = &stsocketssl;

   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);

   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);

#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg, "***** inicio penvioRed ******\n");
#endif   

   strcpy(coderror, "   ");
   strcpy(subsistema_general,argv[0]+strlen(PENVIO));
   strcpy(reginci.innompro,"ENVIO");
   strcat(reginci.innompro,subsistema_general);
   
   /* --------- se ata a la memoria compartida SVM  ------------ */
    retorno=conecSVM((void *) &psvm); //ORIGINAL
 
   if (retorno != 0)
   {
/*#if defined (TRAZA) && (TRAZA > 10)
      // fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif	*/   
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //para sin rearranque
   }

   rtrata_svm=busca_proc_svm(psvm,argv[0],-1);
   if (rtrata_svm < 0 )
   {
#if defined (TRAZA) && (TRAZA > 10)   
       fprintLog(nomProg,"ERROR ARRANQUE NO ESTOY EN SVM %s\n", argv[0]);
#endif	   
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //para sin rearranque
   }

   /* crea cola de mensajes de PENVIO  */
   qenvio=(psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
#if defined (TRAZA) && (TRAZA > 10)   
   fprintLog(nomProg,"qenvio %d\n", qenvio);
#endif   
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
        fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Creacion QENVIO %i codigo de error >%.3s<\n",qenvio, coderror);

       exit(finalizar(-1));//para sin rearranque
   }
   
  //RAQUEL cambiado para utilizar datos de bbdd por entidad
  for (j=0 ; j<2 ; j++) 
  {
	if (strncmp(subsistema_general,reg_para[j].psnument,2)==0)
	{
		qrtratami = damecola(psvm,reg_para[j].pscltrat,NULL); 
        break;
    }
  }
#if defined (TRAZA) && (TRAZA > 10)   
   fprintLog(nomProg,"qrtratami %d\n", qrtratami);
#endif   
   if (qrtratami == -1)
   {
       perror("error >>>");
#if defined (TRAZA) && (TRAZA > 1)	   
       fprintLog(nomProg,"error crea cola  QRTRAT00\n");
#endif	   
       strcpy(reginci.innomfic,"QRTRAT00");
       strcpy(coderror, "009");
      //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Creacion QRTRAT00 codigo de error >%.3s<\n",coderror);

       fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
	   exit(finalizar(-1));//para sin rearranque
   }
   //****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
   //        LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR
   //**********

   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   //* graba su pid en la SVM 
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   (psvm+rtrata_svm)->estado=ESPERA;
   pause();
   queda=alarm(0);
   if (queda==0)
   {
#if defined (TRAZA) && (TRAZA > 1)   
       fprintLog(nomProg,"ERROR ARRANQUE queda >%d<\n", queda);
#endif	   
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
       exit(finalizar(-1));
   }
   (psvm+rtrata_svm)->estado=TRABAJO;

//* al subsistema le ponemos 00 a pelo, porque tal ya que en carga_tablas no es necesario 
    sprintf(auxsub,"%.*s00",2,subsistema_general);

   ////////// ABRE LA CONEXION A LA BASE DE DATOS Y PERMANECE ABIERTA, SE CIERRA EN FINALIZAR
	retorno = conexion_BBDD();
	if(retorno)
	{
	   fprintLog(nomProg,"Error en el acceso a la base de datos\n");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: apertura BBDD codigo de error >%i<\n",retorno);

	   exit(finalizar(-2)); //supervis intenta rearrancar
	}	
	
   //*  ---------------  carga tabla de traduccion  -----------------
   if ((retorno = fcarga_tablas(auxsub)) != 0)
   {
#if defined (TRAZA) && (TRAZA > 1)   
       fprintLog(nomProg,"error carga tablas traduccion \n");
#endif	   
       strcpy(reginci.innomfic,"TBTRAPRI");
       strcpy(coderror, "305");
       exit(finalizar(-2)); //supervis intenta rearrancar
   }
   memset(&registro, ' ', sizeof(NPSDHSM));

   //***** NOS CONECTAMOS AL HSM, DEJANDO EL SOCKET ABIERTO *****
		strcpy(comando, reg_para[j].psclvhsm); //PTASK000024654
		conexhsm_C("0001",reg_para[j].psclvhsm, sockssl, codResp);
   
     sfd_hsm=sockssl->sfd_ssl;
   
         
   //****** BUCLE DE TRATAMIENTO DE MENSAJES *****
   //sleep(1);
   trata_mensaje = 1;
   // ---------- lee con espera de la cola de mensajes ---------------
   while (trata_mensaje == 1)
   {
#if defined (TRAZA) && (TRAZA > 5)
     
     fprintLog(nomProg,"///lee con espera de cola///>%d<///>%d< Bytes///\n",
                                 qenvio, long_msgnuc);
#endif
      
     if(msgrcv(qenvio, &msgnuc, long_msgnuc, 0, 0) == -1)
     {
       perror("error >>>");
#if defined (TRAZA) && (TRAZA > 1)	   
       fprintLog(nomProg,"error leer cola \n");
#endif	   
       desconexhsm(sfd_hsm); //cuando finalizamos desconecta hsm
       strcpy(coderror, "242");
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Lectura QENVIO msgrcv codigo de error >%.3s<\n",coderror);

       strcpy(reginci.innomfic,"qenvio");
       strcat(reginci.innomfic,subsistema_general);
       exit(finalizar(-1));
     }
	 
#if defined (TRAZA) && (TRAZA > 5)
     fprintLog(nomProg,"///recibido el tipo//>%d<///\n", msgnuc.tipo);
#endif
     
        //  -------------  orden de cierre  ------------------ 
     if (msgnuc.tipo == TIPO_PARADA)  // orden de cierre 
     {
#if defined (TRAZA) && (TRAZA > 1)	 
       fprintLog(nomProg,"orden de cierre\n");
#endif	   
       desconexhsm(sfd_hsm); //cuando finalizamos desconecta hsm
       strcpy(coderror, "999");
       if ((psvm+rtrata_svm)->estado==RECARGAR)
       {
          //Añadido para Monitorizacion SISMA 
          fprintLog(nomProg,"TIPO SISTEMA – REARRANQUE DE PROCESO: codigo de coderror >%.3s<\n",coderror);           
          exit(finalizar(-2)); // Paro para rearranque
       }
       else
       {
          exit(finalizar(0)); // Orden de Cierrre
       }
     }

	 
#if defined (TRAZA) && (TRAZA > 10)
     fprintLog(nomProg,"recibidos >%d<\n", retorno);
     //fhexdump(stderr, &msgnuc, long_msgnuc);
	 fprintLog(nomProg,"mensaje es %.6s\n", msgnuc.fiforitn.fidatpro.finumope);
#endif

	 msgnuc.fiforitn.fiisocom.fiiso000[4]='\0';
     tipomsg = atoi(msgnuc.fiforitn.fiisocom.fiiso000);
     clasemsg = fclase_tipomsg(tipomsg);

     if (clasemsg == DESCONOCIDO)
     {
   //si el mensaje es desconocido, no lo traducimos y grabamos incidencia
   //no mandamos al tratami porque el mensaje no es l󧩣o.
#if defined (TRAZA) && (TRAZA > 1)   
         fprintLog(nomProg,"tipo mensaje no contemplado %d\n",tipomsg);
#endif		 
         strcpy(msgnuc.fiforitn.fidatpro.ficodfin,"280");
         strcpy(coderror, "301");
          //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL – TIPO DE MENSAJE NO CONTEMPLADO: operacion no contemplada tipomsg %d codigo de error >%03d< \n", tipomsg,coderror);

         fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
         continue;
     }

     if((clasemsg == RESPUESTAS_NUCLEO) ||
        (clasemsg == RESPUESTA_CIERRE) ||
         (clasemsg == RESPUESTAS_OTROS))
        strncpy(auxsub + 2,msgnuc.fiforitn.fidatpro.fisubori, 2);
     else
        strncpy(auxsub + 2, msgnuc.fiforitn.fidatpro.fisubres, 2);
     auxsub[4]='\0';
     suba_iso = atoi(auxsub) - (atoi(subsistema_general)*100);
 //xavi
     if (suba_iso < 0)
        suba_iso = 2;
//xavi
#if defined (TRAZA) && (TRAZA > 10)
     fprintLog(nomProg," Busco subsistema %s\n",auxsub);
#endif

      posic_subsis= encuentra_svm(psvm,auxsub,TIP_SUBSIS);
      if (posic_subsis < 0)
         tsubsis=-1;
      else
         tsubsis=posic_subsis;

#if defined (TRAZA) && (TRAZA > 10)
       fprintLog(nomProg,"suba_iso >%d<\n", suba_iso);
#endif
//***---solo deberiamos comprobar semaforo para la peticion no control--
     if (clasemsg == PETICION_NO_CONTROL)
     {
    //*  ----------  comprueba si el sistema esta abierto  ---------
         posic_subsis = tsubsis;

         if(posic_subsis < 0 ||
                 (retorno = fsemaforo_red(posic_subsis)) != 1)
         {
#if defined (TRAZA) && (TRAZA > 1)
            fprintLog(nomProg,"semaforo red cerrado o erroneo >%d<\n", retorno);
#endif
            strcpy(msgnuc.fiforitn.fidatpro.ficodfin,"241");
            //respondo al tratami y no traduzco porque no es necesario.
            retorno = fresponde_tratami(&msgnuc.fiforitn);
            continue;
         }
     }

   strcpy(cola_client,CLIENT);
   strcat(cola_client, subsistema_general);
   cola_client[8] = '\0';
   sprintf(PCLIE,"%s",cola_client); //MODIFICADO PARA 3 ENTIDADES
   sprintf(PCLIE+4, "%.2den", suba_iso); //MODIFICADO PARA 3 ENTIDADES
 
  
#if defined (TRAZA) && (TRAZA > 10)
     fprintLog(nomProg,"hay que mandar por la cola %s\n", PCLIE);
#endif
     qmanej = damecola(psvm,PCLIE,NULL);
#if defined (TRAZA) && (TRAZA > 10)	 
     fprintLog(nomProg,"qmanej vale %i\n", qmanej);
#endif	 
     if (qmanej == -1)
     {
#if defined (TRAZA) && (TRAZA > 1)	 
       fprintLog(nomProg,"error crea cola %s errno >>%d<\n",
                  PCLIE, errno);
#endif				  
       strcpy(reginci.innomfic,"QMANEJ");
       strcat(reginci.innomfic,subsistema_general);
       strcpy(coderror, "009");
       //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio QMANEJ %.2s  codigo de error >%.3s<\n",subsistema_general, coderror);

       fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
     }

    //*  ----------  prepara las variables para traducir  ---------- 
   usotrad = '0';
   sentido = 2;

   long_iso = 0;
   error_code = 0;
   error_bit = 0;

    posic_subsis = tsubsis;

// para tarjetas EMV
	//PRJ0258542 - no se tiene que hacer para mensajes 18xx de cambio de claves
	if ((tipomsg < 1800) || (tipomsg > 1899))
	{
		if ((tipomsg < 1524) || (tipomsg > 1534)) //PTASK0023272 - Se amplia al mensaje de totales
		{
	    fprintLog(nomProg,"p22(6) >%.1s< fiaut055.longitud >%.3s<\n", msgnuc.fiforitn.fidatiso.fiisoaut.fiaut022+6, msgnuc.fiforitn.fidatiso.fiisoaut.fiaut055.longitud); //PTASK0023272
	    if((   !strncmp(msgnuc.fiforitn.fidatiso.fiisoaut.fiaut022+6,"5",1) 
			|| !strncmp(msgnuc.fiforitn.fidatiso.fiisoaut.fiaut022+6,"M",1) 
		   )&&
	       (atoi(msgnuc.fiforitn.fidatiso.fiisoaut.fiaut055.longitud) != 0))
	    {
	        guarda55(&msgnuc.fiforitn, comando); //PTASK000024654 - mete comando a la llamada
	    }
	    else
	    {
	       strncpy(msgnuc.fiforitn.fidatiso.fiisoaut.fiaut055.longitud,"000",3);
	       memset(msgnuc.fiforitn.fidatiso.fiisoaut.fiaut055.datos.max055,'\0',
	       sizeof(msgnuc.fiforitn.fidatiso.fiisoaut.fiaut055.datos));
	 		}
	 	} //PTASK0023272 - Se amplia al mensaje de totales - FIN
 	} //PRJ0258542 - no se tiene que hacer para mensajes 18xx de cambio de claves - FIN

      error_code = 0;
    //  ----------  traduce el mensaje  a destino red  ---------- 
    	fprintLog (nomProg,"Va a entrar en ftradred\n"); //PRJ0258452
    	retorno = fcarga_tablas(auxsub); //PRJ0258542 - Fase 3
      retorno = ftradred(usotrad,sentido,suba_iso,&long_iso,mensaje_puc,&msgnuc.fiforitn,&error_code,&error_bit);
      if(retorno)
      {
#if defined (TRAZA) && (TRAZA > 1)	  
         fprintLog (nomProg,"error traduccion red \n");
#endif		 
         strcpy(msgnuc.fiforitn.fidatpro.ficodfin,"280");
         strcpy(coderror, "299");
         strcpy(p04,itoa(error_bit));
         lpad(p04,'0',3);
         strncpy(reginci.incoderr,p04,3);
         strncpy(reginci.incoderr+3,"E",1);
         strcpy(p04,itoa(error_code));
         lpad(p04,'0',3);
         strncpy(reginci.incoderr+4,p04,3);
         fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
         strncpy(reginci.incoderr,"        ",8);
         continue;
      }

#if defined (TRAZA) && (TRAZA > 50)
     //fprintf(stderr,"mensaje_red a enviar -----------------------\n");
     //fhexdump(stderr,mensaje_puc,250);
     //fprintf(stderr,"\nFin mensaje_red a enviar -----------------------\n");
#endif

#if defined (TRAZA) && (TRAZA > 10)
     //tm =&vtm;
     //fechorsys (tm);
     //fprintf(stdout,"tiempo despues de ftradred: %d:%d:%d.%d \n",
     //        tm->tm_hour,tm->tm_min,tm->tm_sec,tm->tm_mls); 
#endif

      if (error_code != 0)
      {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error traduccion red : code >%d< -- bit >%d<\n",
                 error_code,error_bit);
#endif
       strcpy(msgnuc.fiforitn.fidatpro.ficodfin,"280");
       strcpy(coderror, "299");
       strcpy(p04,itoa(error_bit));
       lpad(p04,'0',3);
       strncpy(reginci.incoderr,p04,3);
       strncpy(reginci.incoderr+3,"E",1);
       strcpy(p04,itoa(error_code));
       lpad(p04,'0',3);
       strncpy(reginci.incoderr+4,p04,3);
       fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
       strncpy(reginci.incoderr,"        ",8);
       continue;
      }

    //***---si llega hasta aqui es que el mensaje ha sido traducido correctamente ---
    //*  ----------  envia la operacion a red ---------- 
    fprintLog(nomProg,"Va a entrar en fenvia_red NUMOPE >%.6s<\n",msgnuc.fiforitn.fidatpro.finumope); //PRJ0258542 - Fase 3 - Cambio 11052026
      if ((retorno = fenvia_red(mensaje_puc,long_iso,clasemsg)) != 0)
      {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"error al enviar a red\n");
#endif
         strcpy(msgnuc.fiforitn.fidatpro.ficodfin,"266");
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio NUMOPE >%.6s< a la red  codigo de error 266\n",msgnuc.fiforitn.fidatpro.finumope);

         fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
      }
	  else
	  {
	  
#if defined (TRAZA) && (TRAZA > 10)
       fprintLog(nomProg,"NUMOPE >%.6s< enviado a la red\n",msgnuc.fiforitn.fidatpro.finumope);
#endif
      }
   }  //** fin while tratamensajes 
   
   //DESCONECTAMOS EL HSM
   desconexhsm(sfd_hsm);

} //***  fin main *


/****************************************************************************
   FUNCION   fenvia_red
   DESCRIPCION: Envia un mensaje al manejador de salida hacia red
****************************************************************************/
int fenvia_red (char * cadenasal,int longi,int clasemsg)
{
   int tipomsg,tiempo_espera,retorno,  t_absoluto=0;
   NPSCASA reg_casa;
   int errmsg;
   char longichar[5];
   char auxChar[11];
   int pid_timer, posic_ptimer;


   
   mensaje_comunicacion  msglin;
   int  long_msglin = sizeof(msglin);

#if defined (TRAZA) && (TRAZA > 10)
   fprintLog(nomProg,"en envia-red------->>>\n");
#endif


  //--------   comprueba el tipo de mensaje a casar    -------------
  //           si ya son respuestas, no debemos casar posteriormente   
  //           por lo tanto no graban en nptcasa
  //           se graban en CASA los mensajes hacia la red, no las respuestas a la red  
   if ((clasemsg != RESPUESTAS_NUCLEO) &&
       (clasemsg != RESPUESTA_CIERRE) &&
       (clasemsg != RESPUESTAS_OTROS))
   {	   
      msgnuc.tipo = atof(msgnuc.fiforitn.fiisocom.fiidtran);   
#if defined (TRAZA) && (TRAZA > 10)
      //fprintLog(nomProg, "finumope   %s \n ", msgnuc.fiforitn.fidatpro.finumope);
      //fprintLog(nomProg, "fiiso011   %s \n ", msgnuc.fiforitn.fiisocom.fiidtran);
      //fprintLog(nomProg, "msgnuc.tipo %d \n ", msgnuc.tipo);
#endif

      tipomsg = atoi(msgnuc.fiforitn.fiisocom.fiiso000);
      posic_subsis = tsubsis;

//DMND0036176 añadidas preautorizaciones
     if(posic_subsis>0)
     {
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
        case 1644: //PRJ0258542 - Fase 3 - Cambio 11052026
        	tiempo_espera = 0; //No tiene que esperar nada
        default:
          tiempo_espera = (psvm + posic_subsis)->str_svm.svmpars.patrepet;
          break;
        }
     }
     else
     {		 
         tiempo_espera = 5; //PRJ0258542 - Tiempo de espera bajado de 20 a 5 para todas igual que nptpars
	 }		 

     ///--------------   lanza el timer -----------------------------
     sprintf(auxChar,"ptimer%s",subsistema_general);
     posic_ptimer = encuentra_svm(psvm,auxChar,TIP_PROCES);
     if (posic_ptimer < 0)
     {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR %s no esta en svm \n",auxChar);
#endif
       strcpy(reginci.innomfic,"PTIMER");
       strcat(reginci.innomfic,subsistema_general);
       strcpy(coderror, "011");
       fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
     }

     pid_ptimer=(psvm+posic_ptimer)->str_svm.svmproc.pid;
   
#if defined (TRAZA) && (TRAZA > 10)   
     fprintLog(nomProg,"Llama a pon_ftimer con >%d< segundos\n", tiempo_espera);
#endif

     retorno=pon_ftimer(&msgnuc.fiforitn, pid_ptimer, tiempo_espera, TMP_REL);
     if (retorno)
     {	   
       strcpy(coderror, "045");
       return(1);
     }
   }

 
  // ***  ---------------   mensaje a enviar   ----------------------------------- 
#if defined (TRAZA) && (TRAZA > 10)
    fprintLog(nomProg, "A enviar longi: %d ,  longi+4: %d\n",longi,longi+4);
#endif
    msglin.tipo = 1111;
    sprintf(longichar,"%04d",longi);
    memcpy (msglin.mensaje_linea, cadenasal, longi);

#if defined (TRAZA) && (TRAZA > 10)
    //fprintLog(nomProg,"mensaje_red a enviar con longi--------------\n");
    //fhexdump(stderr, msglin.mensaje_linea, longi);
    fhexdump(stdout, msglin.mensaje_linea, longi); 
    fprintLog(nomProg, "tipo: >%d<\n", msglin.tipo);
    fprintLog(nomProg, "longi: %d ,  longi+4: %d\n",longi,longi+4);
    fprintLog(nomProg,"por la cola %i mando el mensaje de longitud %i\n", qmanej, longi);   
#endif
   
    signal(SIGALRM,sigue);
    alarm(ESSNDCOL);

    if ((msgsnd(qmanej, &msglin, longi,0)) == -1)
    {
       alarm(0);
       perror("msgsnd >>>>>>");
       strcpy(coderror, "266");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio qmanej %i  msgsnd codigo de error >%.3s<\n",qmanej, coderror);

       return(1);
    }
   
#if defined (TRAZA) && (TRAZA > 10)   
    fprintLog(nomProg,"he mandado el mensaje\n");
#endif	
    alarm(0);
   	   
   //  cojo tiempo de entrada al resolutor si el resolutor es la red
   if(strncmp(msgnuc.fiforitn.fidatpro.firespri,subsistema_general,2)==0)
      sprintf(msgnuc.fiforitn.fidatpro.fitienre,"%08d",milelong());

   msgnuc.fiforitn.fidatpro.fitienre[8]='\0';
   msgnuc.fiforitn.fidatpro.fitisare[8]='\0';	  
	  
#if defined (TRAZA) && (TRAZA > 10)
   fprintLog(nomProg, "tiempo entrada al resolutor >%.8s<\n",
                       msgnuc.fiforitn.fidatpro.fitienre);
  
#endif


   return(0);
}

                 
#include "frespTra.c"
#include "ftradred.c"
#include "fclasmsg.c"
#include "guarda55.c"
#include "ftimer.c"
#include "pon_ftimer.c"

//#include "fcifraPan.c"

