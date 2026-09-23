/***********************************************************************
   PROGRAMA:  PENVIO
   DESCRIPCION: Proceso residente encargado del tratamiento de los mensajes
                que llegan de la pantalla. Lee de la cola de entrada y manda 
                 a la pantalla
   - cierre del proceso : el proceso se descarga
***********************************************************************/
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
#include "nptpara.h"

typedef struct
   {  char subsistema[2]; 
      char subaplicacion[2];
      char codigo[4];
      char cod039[3];
   }formtPantS;
formtPantS datoSal;

typedef struct mensaje_comunicacion
{
      long tipo;
      char mensaje_linea[4096];
} mensaje_comunicacion;

NPSINCI  reginci;

int   longi;
char subsistema_general[3];


/***  --------- prototipos de funciones  ----------------  ***/

void sigue();
int finalizar(int codigo);
int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro,
                         char *nomfic);
void fprintLog(char ficheroPlantilla[50], char* format, ...);
int busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int damecola(SVM *psvm_par,char *proce,char *subsis);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int ftraduce_accionpanta(formato_interno *pforitn);
int rtrata_svm;

/***  ---------------   mensaje a enviar   -------------   */

mensaje_nuc msgnuc;
int long_msgnuc = sizeof(msgnuc.fiforitn); //PRJ0258452 - quitado = sizeof(msgnuc) - sizeof(msgnuc.tipo);
mensaje_comunicacion  msglin;
int  long_msglin = sizeof(msglin);

char  coderror[4];
int qmanej;
SVM *psvm;
char nomProg[50];
NPSPARA  *reg_para;

                 /***     FUNCION PRINCIPAL     ***/
int main(int argc,char **argv)
{
   int trata_mensaje;
   int retorno,queda;
   int qenvio;

   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);


   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);

   fprintLog(nomProg, "inicio penvio01\n");

   strcpy(coderror, "   ");
   strcpy(subsistema_general,argv[0]+strlen(PENVIO));
   strcpy(reginci.innompro,"ENVIO");
   strcat(reginci.innompro,subsistema_general);
   strcat(reginci.innompro,"0");

   /* --------- se ata a la memoria compartida SVM  ------------ */

  //RAQUEL SE DEVUELVE A SU ESTADO ORIGINAL PARA UNIFICAR DESDE BBDD 
	 retorno=conecSVM((void *)&psvm); //ORIGINAL
    if (retorno != 0)
    {
      // fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
    }

    rtrata_svm=busca_proc_svm(psvm,argv[0],-1);
    if (rtrata_svm < 0 )
    {
       fprintLog(nomProg,"ERROR ARRANQUE NO ESTOY EN SVM %s\n", argv[0]);
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "250");
       exit(finalizar(-1)); //sin svm supervis no puede arrancarlo 
    }

    /* crea cola de mensajes de PENVIO  */
    qenvio=(psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
    if (qenvio == -1)
    {
       fprintLog(nomProg,"error crea cola qenvio\n");
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"QENVIO");
       strcat(reginci.innomfic,subsistema_general);
       strcpy(coderror, "009");
       exit(finalizar(-1)); //sin svm supervis no puede arrancarlo
    }

   /*  se asocia la cola de mensajes del CLIEN  */

    qmanej=(psvm+rtrata_svm)->str_svm.svmproc.idcolaOut;
    if (qmanej == -1)
    {
       fprintLog(nomProg,"error crea cola qmanej>%d< errno >>%d<\n",qmanej, errno);
       strcpy(reginci.innomfic,"QMANEJ");
       strcat(reginci.innomfic,subsistema_general);
       strcpy(coderror, "009");
       fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
    }

/********** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
           LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR     **********/

   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);

   /* graba su pid en la SVM */
  // queda=alarm(0);
  // fprintLog(nomProg,"RAQUEL PENVIO01 queda >%d<\n", queda);
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   (psvm+rtrata_svm)->estado=ESPERA;
   pause();
   queda=alarm(0);
   if (queda==0)
   {
       fprintLog(nomProg,"ERROR ARRANQUE queda >%d<\n", queda);
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
       exit(finalizar(-1));
   }
   (psvm+rtrata_svm)->estado=TRABAJO;

   /****** BUCLE DE TRATAMIENTO DE MENSAJES ******/

   trata_mensaje = 1;

   /* ---------- lee con espera de la cola de mensajes --------------- */

   while (trata_mensaje == 1)
   {
#if defined (TRAZA) && (TRAZA > 5)
      fprintLog(nomProg, "///lee con espera de cola///>%d<///>%d< Bytes///\n",
                                 qenvio, long_msgnuc);
#endif
      if(msgrcv(qenvio, &msgnuc, long_msgnuc, 0, 0) == -1)
      {
        perror("error >>>");
        fprintLog(nomProg,"error leer cola \n");
        strcpy(coderror, "242");
        strcpy(reginci.innomfic,"qenvio");
        strcat(reginci.innomfic,subsistema_general);
        exit(finalizar(-1));
      }

#if defined (TRAZA) && (TRAZA > 5)
      fprintLog(nomProg, "///recibido el tipo//>%d<///\n", msgnuc.tipo);
#endif

        /*  -------------  orden de cierre  ------------------  */

      if (msgnuc.tipo == TIPO_PARADA)  /* orden de cierre */
      {
         fprintLog(nomProg, "orden de cierre\n");
         strcpy(coderror, "999");
         if ((psvm+rtrata_svm)->estado==RECARGAR)
           exit(finalizar(-2)); /* Paro para rearranque */
         else
           exit(finalizar(0)); /* Orden de Cierrre */
      }

#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg, "recibidos >%d<\n", long_msgnuc);
      //fhexdump(stderr, &msgnuc, long_msgnuc);
#endif

      retorno = ftraduce_accionpanta(&msgnuc.fiforitn);
      //PRJ0258542 - Fase 3 - Meto el if, el else hace lo de siempre
			if (strncmp(msgnuc.fiforitn.fiisocom.fiiso024, "811", 3) == 0 || strncmp(msgnuc.fiforitn.fiisocom.fiiso024, "815", 3) == 0 || strncmp(msgnuc.fiforitn.fiisocom.fiiso024, "816", 3) == 0)
			{
				fprintLog(nomProg, "Valor ficodfin para mensaje de claves >%.3s<\n", msgnuc.fiforitn.fidatpro.ficodfin);
				strncpy(datoSal.cod039,msgnuc.fiforitn.fidatpro.ficodfin,3);
			}
			else
			{
      	strncpy(datoSal.cod039,msgnuc.fiforitn.fiisocom.respusal,3);
      }
      strncpy(datoSal.codigo,msgnuc.fiforitn.fiisocom.fiiso000,4);
      strncpy(datoSal.subsistema,msgnuc.fiforitn.fidatpro.firespri,2);
      strncpy(datoSal.subaplicacion, msgnuc.fiforitn.fidatpro.fisubres,2);
      fprintLog(nomProg, "Respuesta 039 >>>%.*s<<<\n",sizeof(datoSal.cod039),datoSal.cod039);

      msglin.tipo = atoi(msgnuc.fiforitn.fidatpro.fipidori);
      longi= sizeof(formtPantS);
      memcpy(msglin.mensaje_linea, (char *)&datoSal, sizeof(formtPantS));
      signal(SIGALRM,sigue);
      alarm(ESSNDCOL);
#if defined (TRAZA) && (TRAZA > 1)	  
      fprintLog(nomProg, "mando por la cola %i el tamao %i con el mensaje %s\n", qmanej, longi, msglin.mensaje_linea);
#endif	  
      if ((msgsnd(qmanej, &msglin, longi,0)) == -1)
      {
         alarm(0);
         perror("msgsnd >>>>>>");
         strcpy(coderror, "266");
         fgraba_incidencia_cp(coderror, NULL, reginci.innompro, reginci.innomfic);
      }
      alarm(0);

   }  /** fin while tratamensajes  **/

} /***  fin main ***/


/*********************************************************************************/
/***   traduce la respuesta de ficodfin a respusal para enviar a la pantalla   ***/
/*********************************************************************************/

int ftraduce_accionpanta(formato_interno *pforitn)
{
#include "tbfin039.h"
  int ll;
  int tiponum;
  int codfin;
  char p_039[4];
  int j;

  
 codfin = atoi(pforitn->fidatpro.ficodfin);
 tiponum = atoi(pforitn->fiisocom.fiiso000);
 
 fprintLog(nomProg, "numope >%.6s< tiponum >%d< codfin >%d<\n",pforitn->fidatpro.finumope, tiponum, codfin);
 
//RAQUEL se cambia para recoger los datos por bbdd por entidad

  for (j=0 ; j<2 ; j++) 
  {

	if(strncmp(pforitn->fidatpro.firespri,reg_para[j].psnument,2)==0)
  {
      //PRJ0258542 - Fase 3 - Meto el if, el else hace lo de siempre
			if (strncmp(msgnuc.fiforitn.fiisocom.fiiso024, "811", 3) == 0 || strncmp(msgnuc.fiforitn.fiisocom.fiiso024, "815", 3) == 0 || strncmp(msgnuc.fiforitn.fiisocom.fiiso024, "816", 3) == 0)
			{
				fprintLog(nomProg, "Valor respusal para mensaje de claves >%.3s<\n", pforitn->fiisocom.respusal);
			}
			else //PRJ0258542 - Fase 3
			{
	      if(codfin==0)
	      {
			  //DMND0036176 se añaden las respuestas a preautorizaciones
	       switch(tiponum)
	       {
	        case 1110:
	         strcpy(pforitn->fiisocom.respusal,"000");
	         break;
	        case 1130:
	         strcpy(pforitn->fiisocom.respusal,"900");
	         break;
	        case 1430:
	         strcpy(pforitn->fiisocom.respusal,"400");
	         break;
	        case 1314:
	         strcpy(pforitn->fiisocom.respusal,"300");
	         break;
	        case 1814:
	        case 1834:
	         strcpy(pforitn->fiisocom.respusal,"800");
	         break;
	        default:
	         return(1);
	       }// Fin case
	      } // Fin del if ficodfin 
	      else
	      {
	         /***     falta rellenar la tabla tbfin039 completamente     ***/
	        strcpy(pforitn->fiisocom.respusal, pforitn->fidatpro.ficodfin ); 
	        for (ll=0 ; ; ll++ )
	        {
	          if((datos_fin039[ll].tab_fin == codfin) ||
	             (datos_fin039[ll].tab_fin == 0))
	          {
	            sprintf(p_039,"%03d", datos_fin039[ll].tab_039);
	            strcpy(pforitn->fiisocom.respusal , p_039);
	            break;
	          }
	        }//fin for
	       }//fin else
	    }//fin nuevo else //PRJ0258542 - Fase 3
      break;
    }//fin distinto del 30
 }
 return(0);
}
