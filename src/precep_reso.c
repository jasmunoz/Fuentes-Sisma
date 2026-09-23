//PRJ0247167 - Revisado ampliación P48
/***************************************************************************
   PROGRAMA:  precep_reso
   EJECUTABLE: precep03
   DESCRIPCION: Proceso residente encargado del tratamiento de los mensajes
                que llegan  de RESO. Este proceso lee de su cola de entrada
                con espera, y puede tratar : - carga de tablas dinamica
                                             - cierre del proceso
                                             - mensaje origen RESO
                El mensaje origen RESO se traduce, y se trata segun sea
                peticion o respuesta.
***************************************************************************/
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
#include <sys/stat.h>
#include <ctype.h>
#include "fclasmsg.h"
#include "cforitno.h"
#include "nptinci.h"
#include "nptcasa.h"
#include "traeetiq.h"
#include "datagen.h"
#include "deffich.h"
#include "svm.h"
#include "colas.h"
#include "procesos.h"
#include "cformoni.h"
#include "msg_nucl.h"
#include "ftradreso.h"
#include "nptpara.h"

NPSINCI  reginci;

long milelong(void);
int sentido;
int *long_iso;

#ifdef LISTANEGRAONLINE
typedef struct
{
 char mesubsis[2];
 char mesubapl[2];
 char meciadqui[13];
} clave2_tienmerc;

 clave2_tienmerc cl2_tienmerc;
#endif

/***  --------- prototipos de funciones  ----------------  ***/
void fprintLog(char ficheroPlantilla[50], char* format, ...);
void sigue();
int finalizar(int codigo);
int  fcarga_tablas(void);
int  fmonta_claves (formato_interno *pforitn);
int  fclase_tipomsg(int tipomsg);
int  fcasar_respu (formato_interno *ppforitn, formato_interno fippend);
int  fenvia_rtratami(formato_interno *pforitn);
int  fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
int  busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int  damecola(SVM *psvm_par,char *proce,char *subsis);
int  encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int  ftrad03(int sentido, int *long_iso, char *mensaje_reso,
             formato_interno *mensaje_netplus, int tipMsj);
int fverfi(formato_interno fi);
int fnumope (char *numope, char * numcross, char Indice);
int     conexion_BBDD();
int     desconexion_BBDD();


/***  ---------------   mensaje recibido   -------------   */

mensaje_comunicacion  msglin;
int long_leerlin = sizeof(msglin) - sizeof(msglin.tipo);

char *mensaje_reso;
char nomProg[50];
int  rtrata_svm;
char coderror[4];
int  qmanej,qrtratami;


SVM *psvm;

long tiempoL;

int tsubsis[ULTPUC+1];
NPSPARA  *reg_para;

int main(int argc,char *argv[])    /***     FUNCION PRINCIPAL     ***/
{
   int trata_mensaje,ll,clasemsg;
   tm_dec  *tm,vtm;
   char numoper[6];
   int retorno,queda;
   int tipomsg;
   int qrecep;
   mensaje_nuc ppend;
   formato_interno pforitn;
   int posic_subsis;
   char numcross[10]; 
   char claveB[13];
   int err;
   int j;

   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);

#ifdef MDIGITAL
#include "dignull.c"
#endif
   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);


   strcpy(subsistema_general,argv[0]+strlen(PRECEP));
   strcpy(coderror, "   ");
   strcpy(reginci.innompro,"RECEP");
   strcat(reginci.innompro,subsistema_general);  

   fprintLog(nomProg, "inicio precep_reso\n");

   /* --------- se ata a la memoria compartida SVM  ------------ */
  	fprintLog(nomProg, "-- Entrada conecSVM --\n");
	  retorno=conecSVM((void *)&psvm); //ORIGINAL
	fprintLog(nomProg, "-- Salida conecSVM --%i--\n",retorno);
 
    if (retorno != 0)
    {
/*#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif*/
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }
   fprintLog(nomProg, "-- Entrada busca_proc_svm --\n");
    rtrata_svm=busca_proc_svm(psvm,argv[0],-1);
   fprintLog(nomProg, "-- Salida busca_proc_svm --%i--\n",rtrata_svm);
    if (rtrata_svm < 0 )
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "ERROR ARRANQUE NO ESTOY EN SVM %s\n", argv[0]);
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "250");
       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }

    /* cola de mensajes de PRECEP  */

    qrecep = (psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
    if (qrecep == -1)
    {

       fprintLog(nomProg, "error crea cola qrecep\n");

       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,reginci.innompro);
       reginci.innomfic[0]='Q';
       strcpy(coderror, "009");
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creación qrecep%.2s codigo de error >%.3s<\n",subsistema_general,coderror);

       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }

		fprintLog(nomProg,"Dentro de precp_reso \n"); //PRJ0258542
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
           strcpy(reginci.innomfic,"QRTRAT00");
#if defined (TRAZA) && (TRAZA > 1)
           fprintLog(nomProg, "error cola %s\n",reginci.innomfic); 
           perror("error >>>");
#endif
           strcpy(coderror, "009");
            //Añadido para Monitorizacion SISMA 
            fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion QRTRAT00 codigo de error >%.3s<\n",coderror);
         
    	   exit(finalizar(-1)); //sin svm supervis no puede arrancar
        }


   /****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
           LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR
   *******/

   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   /* graba su pid en la SVM */
   (psvm+rtrata_svm)->estado=ESPERA;
   fprintLog(nomProg,"pid asignado %d\n", getpid()); //PRJ0247167
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "precep_reso.c - se duerme\n"); //PRJ0247167
#endif   
   pause();
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "precep_reso.c - despierta\n"); //PRJ0247167
#endif

   queda=alarm(0);
   if(!queda)
   {
       fprintLog(nomProg,"ERROR ARRANQUE queda >%d<\n", queda);
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
      //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - MUERTE DE PROCESO: codigo de error >%.3s<\n",coderror);

       exit(finalizar(-1)); //sin svm supervis no puede arrancar
   }
   (psvm+rtrata_svm)->estado=TRABAJO;


   ////////// ABRE LA CONEXION A LA BASE DE DATOS Y PERMANECE ABIERTA, SE CIERRA EN FINALIZAR
	retorno = conexion_BBDD();
	if(retorno)
	{
	   fprintLog(nomProg,"Error en el acceso a la base de datos\n");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: apertura BBDD codigo de error >%i<\n",retorno);

	   exit(finalizar(-2)); //supervis intenta rearrancar
	}

   /****** BUCLE DE TRATAMIENTO DE MENSAJES ******/
   trata_mensaje = 1;
   /* ---------- lee con espera de la cola de mensajes --------------- */
   fprintLog(nomProg, " Va a empezar a esperar en cola de espera \n"); //PRJ0247167
   while (trata_mensaje == 1)
   {
#if defined (TRAZA) && (TRAZA > 5)
    fprintLog(nomProg, "****Lee con espera de cola qrecep >%d</////////////// \n", qrecep);
#endif
      memset(msglin.long_mensaje, '\0', 4);
      memset(msglin.mensaje_linea, '\0', long_leerlin-4);
	  //PRJ0247167 - Revisado ampliación P48 - la funcion msgrcv no tiene problema con la longitud 4096 nueva del mensaje
      retorno = msgrcv(qrecep, &msglin, long_leerlin, 0, 0);
	  fprintLog(nomProg, "* Despues de msgrcv con retorno %i\n",retorno); //PRJ0247167
	  fprintLog(nomProg, "Numope >%.6s< valor de msglin.long_mensaje >%.4s< y de long_leerlin >%i< cadena >%s<\n", msglin.mensaje_linea + 22, msglin.long_mensaje, long_leerlin, msglin.mensaje_linea); //PRJ0247167
      if(retorno== -1)
      {
         perror("error >>>");
         fprintLog(nomProg,"error leer cola \n");
         strcpy(coderror, "242");
         strcpy(reginci.innomfic,reginci.innompro);
         reginci.innomfic[0]='Q';
       //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo lectura qrecep %i codigo de error >%.3s<\n",qrecep, coderror);

         exit(finalizar(-1)); //si hay un problema con la cola hay que arrancar todo
      }

      fprintLog(nomProg, "Recibido >%d<  msglin.tipo >%d<\n", retorno, msglin.tipo);
      //fhexdump(stderr, &msglin, retorno);


        /*  -------------  orden de cierre  ------------------  */
      if (msglin.tipo == TIPO_PARADA)  /* orden de cierre */
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg, "orden de cierre \n");
#endif
        strcpy(coderror, "999");
        if ((psvm+rtrata_svm)->estado==RECARGAR)
        {
          //Añadido para Monitorizacion SISMA 
          fprintLog(nomProg,"TIPO SISTEMA - REARRANQUE DE PROCESO: codigo de coderror >%.3s<\n",coderror);

          exit(finalizar(-2)); /* Paro para rearranque */
        }
        else
        {
           exit(finalizar(0)); /* Orden de Cierrre */
        }
      }
    /*  ----------  prepara las variables para traducir  ----------  */
      sentido = 1;

	  	fprintLog(nomProg,"Tipo de mensaje a validar: >%d< \n", msglin.tipo); //PTASK000023986
      clasemsg = fclase_tipomsg(msglin.tipo);
	  	fprintLog(nomProg,"Tipo de mensaje validado: >%d< \n", clasemsg); //PTASK000023986

      if (clasemsg == DESCONOCIDO)
      {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg, "mensaje no contemplado : %d\n",tipomsg);
#endif
         strcpy(coderror, "301");
         //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%d< codigo de error >%03d<\n",tipomsg,coderror);

         fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
         continue;
      }  
	  
      /***   cojo el tiempo de entrada del mensaje   ***/
      tiempoL = milelong();
	  
     //RAQUEL cambiado para recoger los datos por bbdd y entidad
	 
  for (j=0 ; j<2 ; j++) 
  { 	
	  if ((strncmp (subsistema_general,reg_para[j].psresolu,2) == 0))
      {
		  // al traductor le paso solo los datos de formato03
		   // el tipo de mensaje y longitud se quedan fuera
           retorno = ftrad03(sentido, long_iso, (char *)msglin.mensaje_linea
		       , &ppend.fiforitn, msglin.tipo);

	     break;
      }
  }
      if(retorno)
      {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg, "error traduccion >%d<\n", retorno);
#endif
         fgraba_incidencia_cp("299", NULL, nomProg, "FTRAD03");
		 continue;
      }

/***
#if defined (TRAZA) && (TRAZA > 10)
     tm =&vtm;
     localtime1 (tm);
     fprintLog(nomProg,"tiempo despues de ftrad03: %d:%d:%d.%d \n",
             tm->tm_hour,tm->tm_min,tm->tm_sec,tm->tm_deci);
#endif
***/

/***--------   comprueba el tipo de mensaje recibido    ---------------***/
      sprintf(ppend.fiforitn.fiisocom.fiiso000, "%04d",msglin.tipo);
	  
      if ((clasemsg == RESPUESTAS_NUCLEO) ||
          (clasemsg == RESPUESTA_CIERRE))
      {
	      // pforitn contendra el msj original + respuesta de ppend

          retorno = fcasar_respu(&pforitn, ppend.fiforitn);

		  // si no consigue casar
          if(retorno)
          {
             switch(retorno)
             {
             case 1:
#if defined (TRAZA) && (TRAZA > 1)
               fprintLog(nomProg, "respuesta fuera de tiempo : %d\n",tipomsg);
#endif
               strcpy(coderror, "303");
              break;
              case 2:
#if defined (TRAZA) && (TRAZA > 1)
                 fprintLog(nomProg, "Datos de peticion no coinciden : %d\n",tipomsg);
#endif
                 strcpy(coderror, "321");
               break;
               case 3:
#if defined (TRAZA) && (TRAZA > 1)
                  fprintLog(nomProg, "MAC recibido erroneo : %d\n",tipomsg);
#endif
                  strcpy(coderror, "916"); //PTASK0022156 - No exite 282, cambiar siempre po 916 - {916,282},
               break;
               case 10:
#if defined (TRAZA) && (TRAZA > 1)
                   fprintLog(nomProg, " Error en respuesta\n");
#endif
                   strcpy(coderror, "909");
                break;
               default:
               break;
             }
			 fgraba_incidencia_cp(coderror, ppend.fiforitn.fidatpro.finumope, NULL, reginci.innomfic);
         }
		 // si si casa
         else
         {
             //cargo el tiempo de salida del resolutor   ***/
             sprintf (pforitn.fidatpro.fitisare, "%08d" , tiempoL);
			 //fprintLog(nomProg,"tiempo de salida del resolutor >%s<\n",pforitn.fidatpro.fitisare);
			 
#if defined (TRAZA) && (TRAZA > 10)
            //fprintf(stdout,"fiaut055.carcontrol \n");
            //fhexdump(stdout,pforitn.fidatiso.fiisoaut.fiaut055.carcontrol,1);
#endif
			 
			 //indica que el resolutori final ha sido este
			 strncpy(pforitn.fidatpro.firesfin,subsistema_general,2);
			 
    /*  ----------  envia la respuesta al rtratami ----------  */
             strncpy(pforitn.fidatpro.ficlatab + 8 , "50" , 2);
#if defined (TRAZA) && (TRAZA > 5)			 
			 fverfi(pforitn);
#endif			 
			 
             if ((retorno = fenvia_rtratami(&pforitn)) != 0)
             {
#if defined (TRAZA) && (TRAZA > 1)
                fprintLog(nomProg, "error al enviar al rtratami >%d<\n", retorno);
#endif
                //Añadido para Monitorizacion SISMA 
                fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Envio cola QRTRAT00 codigo de error >%.3s<\n",coderror);

                fgraba_incidencia_cp(coderror, ppend.fiforitn.fidatpro.finumope, NULL, reginci.innomfic);
             }
         }
      } //si es respuesta nucleo o respuesta cierre
	  
	  // EN INVERSIS NO VAMOS A TENER OPERACIONES QUE SE INICIEN EN EL RESOLUTOR
	  // si es peticion con origen el resolutor (ej 1304)
	  if (clasemsg == PETICION_NO_CONTROL
	   || clasemsg == PETICION_CIERRE)
	  {
#if defined (TRAZA) && (TRAZA > 1)
          fprintLog(nomProg, "Recibido mensaje de peticion no esperado\n");
#endif
         //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%04d< codigo de error 078\n",msglin.tipo);

          fgraba_incidencia_cp("078", ppend.fiforitn.fidatpro.finumope, NULL, reginci.innomfic);
	  

      }

   }  /** fin while tratamensajes  **/
} /***  fin main ***/
/***************************************************************************
   FUNCION   FCASAR_RESPU
   DESCRIPCION: Casa la respuesta con la cola QCASAR, recupera los datos de
                la peticion, le coloca los datos de respuesta, y envia al
                rtratami
***************************************************************************/
int fcasar_respu (formato_interno *ppforitn, formato_interno fippend)
{
  //PRJ0247167 - Revisado ampliación P48 - modificado el tamaño de formato_interno en los .h de su definicion
  //ppforitn --> fi de la operacion original que se recupera en nptcasa
  //fippend --> fi de la operacion pendiente de casar que responde el resolutor
  long  p_011;
  int   resp03;
  char  ch_004[4];
  NPSCASA reg_casa;
  int  retorno;
  char clave_casa[7];
  
  int fbusca_codfin(int ll, int tmsg);
  int ncampo_numerico (char *c,int n);
  int i=0;
  int tipomens;

 
#if defined (TRAZA) && (TRAZA > 10)
   fprintLog(nomProg,"=========fcasar_respu\n");
#endif 
 
   memset(&reg_casa, ' ', sizeof(NPSCASA));
   
   strncpy(clave_casa, fippend.fidatpro.finumope, 6);
   clave_casa[6] = '\0';
   
   //20160623-ademas de consultar borro para controlar que el ptimer no este haciendo lo mismo a la vez
   //retorno = tratar_fich("NPTCASA", IGUAL, &reg_casa, clave_casa);
    strncpy(reg_casa.canumope, clave_casa, 6);
    retorno = tratar_fich("NPTCASA", IGUALBORRAR, &reg_casa, clave_casa);

   if(retorno)  // no se encuentra mensaje original o da error al borrarlo pq ya lo ha borrado ptimer
   {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg, "error CASAR >%d< clave_casa>%.6s<\n", retorno, clave_casa);                    
#endif
     strcpy(reginci.innomfic, "QCASAR");
   
     return(1);
   }
#if defined (TRAZA) && (TRAZA > 10)   
   fprintLog(nomProg,"Operacion casada reg_casa-finumope >%.6s<\n", reg_casa.caforitn.fidatpro.finumope);  
   //fprintLog(nomProg,"caforitn >%.*s<\n", sizeof(formato_interno),reg_casa.caforitn);
#endif   
   
   memcpy(ppforitn, &reg_casa.caforitn, sizeof(formato_interno));
   
   //fprintLog(nomProg,"fipidori en nptcasa >%.5s<\n",ppforitn->fidatpro.fipidori);
         

   if(strncmp(ppforitn->fidatpro.firespri,subsistema_general,2))
   {
#if defined (TRAZA) && (TRAZA > 1)   
     fprintLog(nomProg, "Operacion de otro subsistema >%.2s<\n",
	    ppforitn->fidatpro.firespri);
#endif
     //Añadido para Monitorizacion SISMA 
     fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: operacion de otro subsistema %.2s \n", ppforitn->fidatpro.firespri);
	
     return(2);
   }

   if(strncmp(fippend.fiisocom.fifechor.aa,
          ppforitn->fiisocom.fifechor.aa,2)         ||
   strncmp(fippend.fiisocom.fifechor.mm,
          ppforitn->fiisocom.fifechor.mm,2)         ||
   strncmp(fippend.fiisocom.fifechor.dd,
          ppforitn->fiisocom.fifechor.dd,2)         ||
   strncmp(fippend.fiisocom.fifechor.hh,
          ppforitn->fiisocom.fifechor.hh,2)         ||
   strncmp(fippend.fiisocom.fifechor.mi,
          ppforitn->fiisocom.fifechor.mi,2)         ||
   strncmp(fippend.fiisocom.fifechor.ss,
          ppforitn->fiisocom.fifechor.ss,2))
  {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg, "------------->el mensaje no casa fecha/hora \n"); 
     fprintLog(nomProg,"ppend= %.2s%.2s%.2s%.2s%.2s%.2s\n",
                fippend.fiisocom.fifechor.aa,
                fippend.fiisocom.fifechor.mm,
                fippend.fiisocom.fifechor.dd,
                fippend.fiisocom.fifechor.hh,
                fippend.fiisocom.fifechor.mi,
                fippend.fiisocom.fifechor.ss);
     fprintLog(nomProg,"pfori= %.2s%.2s%.2s%.2s%.2s%.2s\n",
                ppforitn->fiisocom.fifechor.aa,
                ppforitn->fiisocom.fifechor.mm,
                ppforitn->fiisocom.fifechor.dd,
                ppforitn->fiisocom.fifechor.hh,
                ppforitn->fiisocom.fifechor.mi,
                ppforitn->fiisocom.fifechor.ss);
      fprintLog(nomProg,"\n");
     
#endif     
     return(2);
  }
  

   if(ncampo_numerico(fippend.fiisocom.respusal,2)==0)
   {
#if defined (TRAZA) && (TRAZA > 1)   
          fprintLog(nomProg, "Codigo de respuesta no esperado resp03 >%.3s<\n", fippend.fiisocom.respusal);
#endif		  
          strcpy(ppforitn->fidatpro.ficodfin , "190");   //PRJ0258542 - Se cambia 261 por 190
          strcpy(reginci.innomfic, "RESP03");
         return(10);  
   }

   resp03 = atoi(fippend.fiisocom.respusal);
   tipomens = atoi(fippend.fiisocom.fiiso000);
		//PRJ0331925 - Cambio literal
   	fprintLog(nomProg, "Valor respusal de resp03  >%d<\n", resp03);
   	fprintLog(nomProg, "Valor fiiso00 de tipomens >%d<\n", tipomens); 

   strncpy(ppforitn->fiisocom.respusal , fippend.fiisocom.respusal, sizeof(ppforitn->fiisocom.respusal)); 
  
   switch(resp03) 
   {
    case 0:
		case 1:
    case 300:
    case 400:
    case 500:
    case 600:
    case 800:
    case 900:
    case 99:
      strcpy(ppforitn->fidatpro.ficodfin , "000");
    break;
    default:
      /***   funcion que convierte el codigo RESO en codfin   ***/
      retorno = fbusca_codfin(resp03, tipomens);

      if (retorno < 0)
      {
      /***   codigo de respuesta no tratado   ***/
#if defined (TRAZA) && (TRAZA > 1)	  
          fprintLog(nomProg, "Codigo de respuesta no tratado resp03 >%03d<\n", resp03);
#endif		  
          strcpy(ppforitn->fidatpro.ficodfin , "190"); //PRJ0258542 - Se cambia 261 por 190
      }
      else
      {
          sprintf(ch_004, "%03d", retorno);
          strcpy(ppforitn->fidatpro.ficodfin , ch_004);
       }
    break;
    }
	
	//// copia los datos que aporta la respuesta fippend
  sprintf(ppforitn->fidatiso.fiisoaut.fiaut038,"%.6s", fippend.fidatiso.fiisoaut.fiaut038);
  sprintf(ppforitn->fidatpro.ficomisi,"%.8s", fippend.fidatpro.ficomisi);
	strncpy(ppforitn->fidatpro.fitipcom,fippend.fidatpro.fitipcom,1);
	sprintf(ppforitn->fidatpro.firesprs,"%.3s", fippend.fidatpro.firesprs);
	
	//si es una retirada de efectivo puede haber comision adicional
	if(strncmp(ppforitn->fidatiso.fiisoaut.fiaut003.dig12, "01",2)==0)
	{
		printf("------- Indica que tiene que poner comision --reso---- \n"); //PRJ0387060 INI
		if (strncmp(fippend.fidatpro.ficomadi, "     ",5) == 0)
		{
			strncpy(fippend.fidatpro.ficomadi, "00000",5);
		}
		if (strncmp(fippend.fidatpro.fitipcom, "  ",2) == 0)
		{
			strncpy(fippend.fidatpro.fitipcom, "00",2);
		}
		if (strncmp(fippend.fidatpro.ficomisi, "        ",8) == 0)
		{
			strncpy(fippend.fidatpro.ficomisi, "00000000",8);
		}
	  strncpy(ppforitn->fidatpro.ficomisi,fippend.fidatpro.ficomisi,8);
		//PRJ0387060 FIN

	  strncpy(ppforitn->fidatpro.ficomadi,fippend.fidatpro.ficomadi,5);
    strncpy(ppforitn->fidatpro.fitipcom +1,fippend.fidatpro.fitipcom +1,1);		  
	}
	else //PRJ0387060
	{
		printf("------- Indica que no tiene que poner comision --reso---- \n");
	  strncpy(ppforitn->fidatpro.ficomadi,"     ",5);
		strncpy(ppforitn->fidatpro.ficomisi,"        ",8);
		strncpy(ppforitn->fidatpro.fitipcom,"  ",2);	
	} //PRJ0387060
	printf("------- Campos comisiones --reso-- fitipcom %.2s ficomisi %.8s ficomadi %.5s \n",ppforitn->fidatpro.fitipcom,ppforitn->fidatpro.ficomisi,ppforitn->fidatpro.ficomadi); //PRJ0387060
    // transferencias 
     if ((strncmp(ppforitn->fiisocom.fiiso000, "12", 2) ==0) &&
		strncmp(ppforitn->fidatiso.fiisoaut.fiaut003.dig12, "40",2)==0)
	{
       //bit 103
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut103.datos,
		      fippend.fidatiso.fiisoaut.fiaut103.datos,28);
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut103.longitud,
		      fippend.fidatiso.fiisoaut.fiaut103.longitud,2);
       //bit 102
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut102.datos,
		      fippend.fidatiso.fiisoaut.fiaut102.datos,28);
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut102.longitud,
		      fippend.fidatiso.fiisoaut.fiaut102.longitud,2);            
       //bit 48.61
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.comun01.fiiso4861,
		      fippend.fidatiso.fiisoaut.fiaut048.comun01.fiiso4861,10); 
       //bit 18
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut018,
		      fippend.fidatiso.fiisoaut.fiaut018,4);
       //bit 26
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut026,
		      fippend.fidatiso.fiisoaut.fiaut026,4);
    }
    
	// si es una petición no de movimientos y retornan los saldos
	if((strncmp(ppforitn->fiisocom.fiiso000, "11", 2) ==0 ||
	    strncmp(ppforitn->fiisocom.fiiso000, "12", 2) ==0) &&
		(strncmp(ppforitn->fidatiso.fiisoaut.fiaut003.dig12, "30",2)==0) ||
        (strncmp(fippend.fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas, "1",1)==0))
	{
       

       strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas,
		      fippend.fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas,1);
          
      for (i = 0; i<4; i++)
	  {
          strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat[i],
		           fippend.fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat[i],24);
 	  }						   
	}
	   
	// si es una consulta de movimientos y ha ido bien
	if(strncmp(ppforitn->fidatiso.fiisoaut.fiaut003.dig12, "31", 2) ==0 && 
	   strncmp(ppforitn->fidatpro.ficodfin, "000", 2) ==0) 
    {
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.fecha,
		      fippend.fidatiso.fiisoaut.fiaut048.movim.sub04806.fecha,6);
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.signo,
		      fippend.fidatiso.fiisoaut.fiaut048.movim.sub04806.signo,1);
       strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.importe,
		      fippend.fidatiso.fiisoaut.fiaut048.movim.sub04806.importe,12);
        strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.moneda,
		      fippend.fidatiso.fiisoaut.fiaut048.movim.sub04806.moneda,3);
         strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.num_movi,
		      fippend.fidatiso.fiisoaut.fiaut048.movim.sub04806.num_movi,1);
           
      for (i = 0; i<5; i++)
	  {
          strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.movimi[i],
		           fippend.fidatiso.fiisoaut.fiaut048.movim.sub04806.movimi[i],37);
	  }		  
    }	
	
    // si es una petición de autorización de compra que admite importe parcial
    // y la respuesta dice que ha habido autorizacion parcial
    // y nos informan de dicho importe parcial
	
	if (strncmp(ppforitn->fiisocom.fiiso000, "11", 2) ==0
    &&  strncmp(ppforitn->fidatiso.fiisoaut.fiaut003.dig12, "00", 2) ==0)
	{
        //fprintLog(nomProg,"ImpParcial %.12s\n", fippend.fidatiso.fiisoaut.fiaut048.comun01.importeParcial);	
        if(strncmp(ppforitn->fidatiso.fiisoaut.fiaut048.comun01.flagImpParcial, "1", 1) == 0
        && strncmp(fippend.fidatiso.fiisoaut.fiaut048.comun01.importeParcial, "000000000000", 12) != 0
		&& strncmp(fippend.fidatiso.fiisoaut.fiaut048.comun01.importeParcial, "            ", 12) != 0)
		{
	        strncpy(ppforitn->fidatiso.fiisoaut.fiaut048.comun01.importeParcial,
		              fippend.fidatiso.fiisoaut.fiaut048.comun01.importeParcial,12);		
        }					  
	}			   
	
	
//////////////////////////////////	
    strncpy(ppforitn->fiprivado.impopfi    , fippend.fiprivado.impopfi, 12);
    strncpy(ppforitn->fiprivado.numcuotas    , fippend.fiprivado.numcuotas, 3);
    strncpy(ppforitn->fiprivado.impcuota     , fippend.fiprivado.impcuota, 12);
    strncpy(ppforitn->fiprivado.fecprmvto    , fippend.fiprivado.fecprmvto, 8);
    strncpy(ppforitn->fiprivado.porintrs     , fippend.fiprivado.porintrs, 7);
    strncpy(ppforitn->fiprivado.portae       , fippend.fiprivado.portae, 7);
    strncpy(ppforitn->fiprivado.impcomestd   , fippend.fiprivado.impcomestd, 12);
    strncpy(ppforitn->fiprivado.porintrsmor  , fippend.fiprivado.porintrsmor, 7);
    strncpy(ppforitn->fiprivado.portaemor    , fippend.fiprivado.portaemor, 7);
    strncpy(ppforitn->fiprivado.impcommor    , fippend.fiprivado.impcommor, 12);
    strncpy(ppforitn->fiprivado.impseguro    , fippend.fiprivado.impseguro, 12);
    strncpy(ppforitn->fiprivado.imptotopfi   , fippend.fiprivado.imptotopfi, 12);
    strncpy(ppforitn->fiprivado.indcmpragrts , fippend.fiprivado.indcmpragrts, 1);
    strncpy(ppforitn->fiprivado.nomtitu      , fippend.fiprivado.nomtitu, 30);
	
    return(0);
}

#include "fenvia_rtratami.c" 
 
/***************************************************************************
   FUNCION   FBUSCA_CODFIN
   DESCRIPCION: Traduce el Cod respuesta del RESOLUTOR al codfin
***************************************************************************/
int fbusca_codfin (int resp03, int tipomens) 
{
#include "tbResp03fin.h"
  int ll;
  
  fprintLog(nomProg, " - Entra en fbusca_codfin con resp03 %i y tipomens %i \n",resp03,tipomens);

 for (ll=0 ;datos_resp03fin[ll].tab_resp03 > 0 ; ll++ )
 {

  if((datos_resp03fin[ll].tab_tipo == tipomens)&&(datos_resp03fin[ll].tab_resp03 == resp03))
  {
   return(datos_resp03fin[ll].tab_fin); 
  }
 }

 return(-1);

}

#include "ftrad03.c"
#include "fclasmsg.c"
