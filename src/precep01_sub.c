/*e*************************************************************************
   PROGRAMA:  precep01  
   EJECUTABLE: precep01
   DESCRIPCION: Proceso residente encargado del tratamiento de los mensajes
                que llegan  de la WEB.
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

#include "netwin.h" 
#include "cforitno.h"
#include "nptinci.h"
#include "nptcasa.h"
#include "traeetiq.h"
          
#include "datagen.h"
#include "deffich.h"
#include "svm.h"
#include "procesos.h"
          
#include "cformoni.h"
#include "msgclien.h"
#include "nptpara.h"

typedef struct
{
   char ptsubsis[2];
   char ptsubapl[2];
   char pttipmsj[3];
   char caractedialogo[3];
}formtPant;
formtPant datEnt;

NPSINCI reginci;
formato_interno fiforitn;

int rtrata_svm;
char  coderror[4];
int qrtratami;
/**************************************************/

SVM *psvm;

int tsubsis;
char nomProg[50];
char subsistema_general[3];
int posic_subsis, posic_subsis_dest;


/***  --------- prototipos de funciones  ----------------  ***/
void    sigue();
int     finalizar(int codigo);
int     fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
void    fprintLog(char * pf, char * msg, ...);
int     busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int     damecola(SVM *psvm_par,char *proce,char *subsis);
int     encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int fgenera_1804(formtPant * datEnt, formato_interno * pforitn);
int trata_recep1804(formtPant * datEnt, formato_interno * pforitn);
int fmonta_claves1804(formato_interno *pforitn);
int     inicfori (int tipomsg, formato_interno *pfi);
int     fnumope (char *numope, char * numcross, char Indice);
/***  ---------------   mensaje recibido   -------------   */
mensaje_lin    msglin;
int  long_leerlin = sizeof(msglin) - sizeof(msglin.tipo);

NPSPARA  *reg_para;


/***************************MAIN**************************************/
            /***     FUNCION PRINCIPAL     ***/
int main(int argc,char **argv)
{
   int trata_mensaje;
   tm_dec tm;
   char auxsub[5];
   int retorno,queda;
   int qrecep;
   int long_iso ;
   long_iso =sizeof(formato_interno);
   int ll;


   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);

   strcpy(subsistema_general,argv[0]+strlen(PRECEP));
   strcpy(nomProg,argv[0]);
   strcpy(coderror, "   ");

   strcpy(reginci.innompro,"RECEP");
   strcat(reginci.innompro,subsistema_general);

   fprintLog(nomProg, "inicio \n");

   /* --------- se ata a la memoria compartida SVM  ------------ */
    retorno=conecSVM((void *) &psvm);
    if (retorno != 0)
    {
       fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-2));
    }
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg, "conectamos \n");
#endif

    rtrata_svm=busca_proc_svm(psvm,argv[0],-1);
    if (rtrata_svm < 0 )
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR ARRANQUE NO ESTOY EN SVM %s\n", argv[0]);
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "250");
       exit(finalizar(-2));
    }
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"buscamos   \n");
#endif
    /* cola de mensajes de PRECEP  */

    qrecep = (psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
#if defined (TRAZA) && (TRAZA > 1)	
    fprintLog(nomProg,"cola por donde recibe %d\n",qrecep);
#endif
    if (qrecep == -1)
    {
       fprintLog(nomProg,"error crea cola qrecep\n");
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,reginci.innompro);
       reginci.innomfic[0]='Q';
       strcpy(coderror, "009");
       exit(finalizar(-2));
    }

    //RAQUEL AÑADIDO PARA ARRANQUE POR BBDD
   for (ll=0 ; ll<2 ; ll++) 
   { 
	  if (strncmp(subsistema_general,reg_para[ll].psmonito,2)==0)
	  {
		qrtratami = damecola(psvm,reg_para[ll].pscltrat,NULL);
		break;
	  }
   }
#if defined (TRAZA) && (TRAZA > 1)	
    fprintLog(nomProg,"cola qrtrat %d\n",qrtratami);
#endif    
    if (qrtratami == -1)
    {
       strcpy(reginci.innomfic,"QRTRAT00");
       fprintLog(nomProg,"error cola %s\n",reginci.innomfic);
       perror("error >>>");
       strcpy(coderror, "009");
       fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
    }

   /****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
           LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR
   *******/

   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   /* graba su pid en la SVM */
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   (psvm+rtrata_svm)->estado=ESPERA;
   pause();
   queda=alarm(0);
   if(!queda)
   {
       fprintLog(nomProg,"ERROR ARRANQUE queda >%d<\n", queda);
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
       exit(finalizar(-1));
   }
   (psvm+rtrata_svm)->estado=TRABAJO;
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg,"TRABAJO  \n");
#endif   

/* -- obtiene el desplazamiento del puntero para los datos del subsistema --*/
/* Carga la posicion de los distintos subsistemas en la tabla tsubsis */
      sprintf(auxsub,"%.*s01",2,subsistema_general);
#if defined (TRAZA) && (TRAZA > 10)
      fprintLog(nomProg, "Busco subsistema origen >%s<\n",auxsub);
#endif
      posic_subsis= encuentra_svm(psvm,auxsub,TIP_SUBSIS);
      if (posic_subsis < 0)
         tsubsis=-1;
      else
         tsubsis=posic_subsis;

#if defined (TRAZA) && (TRAZA > 10)
    fprintLog(nomProg, "obtenido posic_subsis >%d< y guardado tsubsis >%d<\n",posic_subsis,tsubsis);
#endif

   /****** BUCLE DE TRATAMIENTO DE MENSAJES ******/

   trata_mensaje = 1;
   /* ---------- lee con espera de la cola de mensajes --------------- */
   while (trata_mensaje == 1)
   {
#if defined (TRAZA) && (TRAZA > 5)
      fprintLog(nomProg, "////////////////lee con espera de cola/////////////// \n");
#endif

      memset(msglin.mensaje_linea, '\0', long_leerlin);
      retorno = msgrcv(qrecep, &msglin, long_leerlin, 0, 0);
      if(retorno== -1)
      {
         perror("error >>>");
         fprintLog(nomProg,"error leer cola \n");
         strcpy(coderror, "242");
         strcpy(reginci.innomfic,reginci.innompro);
         reginci.innomfic[0]='Q';
         exit(finalizar(-1));
      }
#if defined (TRAZA) && (TRAZA > 5)
      fprintLog(nomProg,"Recibido >%d<\n", retorno);
      fhexdump(stderr, &msglin, retorno);
#endif

      /*  -------------  orden de cierre  ------------------  */
      if (msglin.tipo == TIPO_PARADA)  /* orden de cierre */
      {
        fprintLog(nomProg,"orden de cierre \n");
        strcpy(coderror, "999");
        if ((psvm+rtrata_svm)->estado==RECARGAR)
          exit(finalizar(-2)); /* Paro para rearranque */
        else
          exit(finalizar(0)); /* Orden de Cierrre */
      }

/*  el manejador de comunicaciones debe informar de algun modo del centro procesador del que llega la operacion */
     posic_subsis = tsubsis;
//   LLlabelMAC((psvm+posic_subsis)->str_svm.svmpars.paetiMAC, 4, 2);

      if (msglin.mensaje_linea[0] == CAIDA_LINEA)  /* orden de cierre */
      {
        fprintLog(nomProg,"Caida de linea %05\n",msglin.tipo);
        if (posic_subsis > 0)
            (psvm+posic_subsis)->str_svm.svmpars.pasemsub = 0;
        continue;
      }

      strncpy((char*)&datEnt, msglin.mensaje_linea,retorno);
	  
/* -- obtiene el desplazamiento del puntero para los datos del subsistema destino --*/
      strncpy(auxsub,datEnt.ptsubsis,2);
      strncpy(auxsub+2, datEnt.ptsubapl,2);
	  auxsub[4]='\0';
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg, "Busco subsistema destino>%s<\n",auxsub);
#endif
      posic_subsis_dest= encuentra_svm(psvm,auxsub,TIP_SUBSIS);
      if (posic_subsis < 0)
         posic_subsis_dest=-1;
      for (ll=0 ; ll<2 ; ll++) 
      {  
         if (!strncmp(datEnt.ptsubsis,reg_para[ll].psnument,2) || 
		     !strncmp(datEnt.ptsubsis,reg_para[ll].psresolu,2))
         {
               trata_recep1804(&datEnt, &fiforitn);
			   break;	     
         }
      }
      sprintf(fiforitn.fidatpro.fipidori, "%d", msglin.tipo);

#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"tipo %i \n", msglin.tipo);
#endif	  
      msglin.tipo = 1111;
      memcpy(&msglin.mensaje_linea, &fiforitn, long_iso);
#if defined (TRAZA) && (TRAZA > 1)	  
      fprintLog(nomProg,"enviar por la cola %i el tamaño %i\n", qrtratami, long_iso);
#endif	  

      fhexdump(stderr, &msglin, long_iso+4);
      if ((msgsnd(qrtratami, &msglin, long_iso ,0))== -1)
      {
        alarm(0);
        #if defined (TRAZA) && (TRAZA > 1)
            perror("mensaje>>>>>");
        #endif
        return(-1);
      }

   }  /** fin while tratamensajes  **/
} /***  fin main ***/

/***************************************************************************
   FUNCION   FGENERA_1804
   DESCRIPCION: Genera un mensaje de control traduciendo lo que le llega de la pantalla
/***************************************************************************/
int fgenera_1804(formtPant * datEnt,formato_interno * pforitn)
{
    char numoper[6];
    char numcross[10];
    int retorno=0;
    tm_dec tm;
    fechorsys(&tm);
    int ll;
	
    retorno = inicfori(1804, pforitn);
    strcpy(pforitn->fiisocom.fiiso000 , "1804");

    strncpy(pforitn->fidatpro.fifecha0,"20",2);
    sprintf(pforitn->fidatpro.fifecha0+2,"%02d", tm.tm_year - 2000);
    sprintf(pforitn->fidatpro.fifecha0+4,"%02d", tm.tm_mon);
    sprintf(pforitn->fidatpro.fifecha0+6,"%02d", tm.tm_mday);
    sprintf(pforitn->fidatpro.fihora00,"%02d", tm.tm_hour);
    sprintf(pforitn->fidatpro.fihora00+2,"%02d", tm.tm_min);
    sprintf(pforitn->fidatpro.fihora00+4,"%02d", tm.tm_sec);
	
    if ((retorno = fnumope(numoper, numcross, '0')) != 0)
    {
        fprintLog(nomProg,"error al asignar numero de operacion \n");
        fgraba_incidencia_cp("264", NULL,NULL,NULL);

        strcpy(pforitn->fiisocom.respusal,"909");
        return(retorno);
    }
    else
    {
        strncpy(pforitn->fidatpro.finumope,numoper,6);
        pforitn->fidatpro.finumope[6] = '\0';

    }
    
	/////////// TEST AL RESOLUTOR DE MEDIOS DE PAGO
	//RAQUEL se cambia para recoger los datos por bbdd y entidad
   for (ll=0 ; ll<2 ; ll++) 
   {  
      // if (!strncmp(datEnt->ptsubsis,"03",2))
      if (!strncmp(datEnt->ptsubsis,reg_para[ll].psresolu,2))
      {
#if defined (TRAZA) && (TRAZA > 1)	
        fprintLog(nomProg,"tst para el resolutor de medios de pago\n");
#endif		

        //strncpy(pforitn->fidatiso.fiisoctr.fictr028.aa,(psvm+posic_subsis_dest)->str_svm.svmpars.pafecses    , 2); //PRJ0258542
        //strncpy(pforitn->fidatiso.fiisoctr.fictr028.mm,(psvm+posic_subsis_dest)->str_svm.svmpars.pafecses + 2, 2); //PRJ0258542
        //strncpy(pforitn->fidatiso.fiisoctr.fictr028.dd,(psvm+posic_subsis_dest)->str_svm.svmpars.pafecses + 4, 2); //PRJ0258542
				//strncpy(pforitn->fidatiso.fiisoctr.fictr029,(psvm+posic_subsis_dest)->str_svm.svmpars.panumses,3); //PRJ0258542
		
        strncpy(pforitn->fidatpro.fiorigen,"01",2);
        strncpy(pforitn->fidatpro.firespri,datEnt->ptsubsis,2);
        strncpy(pforitn->fidatpro.fisubres,datEnt->ptsubapl,2);
        strncpy(pforitn->fiisocom.fiidtran ,pforitn->fidatpro.finumope, 6);
        strncpy(pforitn->fiisocom.fifechor.aa, pforitn->fidatpro.fifecha0+2, 2);
        strncpy(pforitn->fiisocom.fifechor.mm, pforitn->fidatpro.fifecha0+4, 2);// tt.wMonth);
        strncpy(pforitn->fiisocom.fifechor.dd, pforitn->fidatpro.fifecha0+6, 2);// tt.wDay);
        strncpy(pforitn->fiisocom.fifechor.hh, pforitn->fidatpro.fihora00, 2);//tt.wHour);
        strncpy(pforitn->fiisocom.fifechor.mi, pforitn->fidatpro.fihora00+2, 2);//tt.wMinute);
        strncpy(pforitn->fiisocom.fifechor.ss, pforitn->fidatpro.fihora00+4, 2);//tt.wSecond);

        strncpy(pforitn->fiisocom.fiiso024,"831",3);

        memset(pforitn->fiisocom.fi032094.longitud,'0',sizeof(pforitn->fiisocom.fi032094.longitud));
        memset(pforitn->fiisocom.fi032094.datos,' ',sizeof(pforitn->fiisocom.fi032094.datos));

        memset(pforitn->fidatiso.fiisoctr.fictr093.longitud,'0',sizeof(pforitn->fidatiso.fiisoctr.fictr093.longitud));
        memset(pforitn->fidatiso.fiisoctr.fictr093.datos,' ',sizeof(pforitn->fidatiso.fiisoctr.fictr093.datos));
		break;
    }

   }
    ///****************TST PRICE
	//RAQUEL se cambia para recoger los datos por bbdd y entidad

   // if (!strncmp(datEnt->ptsubsis,"30",2))
   for (ll=0 ; ll<2 ; ll++) 
   {  
    if (!strncmp(datEnt->ptsubsis,reg_para[ll].psnument,2)) 
    {
#if defined (TRAZA) && (TRAZA > 1)	
        fprintLog(nomProg,"tst para la red price\n");
#endif		

        strncpy(pforitn->fidatpro.fiorigen,"01",2);
        //strncpy(pforitn->fidatpro.firespri,"30",2);
		strncpy(pforitn->fidatpro.firespri,reg_para[ll].psnument,2);
        strcpy(pforitn->fidatpro.fisubres, datEnt->ptsubapl);
        strcpy(pforitn->fiisocom.fiidtran ,pforitn->fidatpro.finumope);
        strncpy(pforitn->fiisocom.fifechor.aa, pforitn->fidatpro.fifecha0+2,2);// Año);
        strncpy(pforitn->fiisocom.fifechor.mm, pforitn->fidatpro.fifecha0+4,2);// tt.wMonth);
        strncpy(pforitn->fiisocom.fifechor.dd, pforitn->fidatpro.fifecha0+6,2);// tt.wDay);
        strncpy(pforitn->fiisocom.fifechor.hh, pforitn->fidatpro.fihora00,2);//tt.wHour);
        strncpy(pforitn->fiisocom.fifechor.mi, pforitn->fidatpro.fihora00+2,2);//tt.wMinute);
        strncpy(pforitn->fiisocom.fifechor.ss, pforitn->fidatpro.fihora00+4,2);//tt.wSecond);

#if defined (TRAZA) && (TRAZA > 1)	
        fprintLog(nomProg,"datEnt->pttipmsj %.3s\n", datEnt->pttipmsj);
#endif		
        if (strncmp(datEnt->pttipmsj,"TST",3)==0)
        {
            strncpy(pforitn->fiisocom.fiiso024 , "831",3);
        }
        else 
        {
         if(strncmp(datEnt->pttipmsj,"SNF",3)==0)
         {
            strncpy(pforitn->fiisocom.fiiso024,"802",3);
         }
         else
         {
            if(strncmp(datEnt->pttipmsj,"SNN",3)==0)
                strncpy(pforitn->fiisocom.fiiso024,"801",3);
            else // CAMBIO DE CARACTERISTICAS DE DIALOGO
                strncpy(pforitn->fiisocom.fiiso024,"890",3);

           // el iso 24 se rellenara dependiendo de lo que nos ha llegado. Por ahora siempre dialogo TOTAL
            if (strncmp(datEnt->caractedialogo,"TOT",3)==0)
                strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507000",8);
			else
            if (strncmp(datEnt->caractedialogo,"DEB",3)==0)
                strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507001",8);
			else
            if (strncmp(datEnt->caractedialogo,"CRE",3)==0)
                strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507002",8);
			else
		       strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507000",8);
#if defined (TRAZA) && (TRAZA > 1)			   
		    fprintLog(nomProg,"fi48 >%.*s<\n",8,pforitn->fidatiso.fiisoctr.fictr048.contr.cardia);
#endif		   
         }
        }
		fprintLog(nomProg, "(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui %.13s\n",
                		(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui);
		
        strncpy(pforitn->fiisocom.fi032094.longitud,(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui,2);
        pforitn->fiisocom.fi032094.longitud[2] = '\0';
        strncpy(pforitn->fiisocom.fi032094.datos ,(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui + 2,11);
        pforitn->fiisocom.fi032094.datos[11] = '\0';

		fprintLog(nomProg, "(psvm+posic_subsis_dest)->str_svm.svmpars.pacidesti %.13s\n",
                		(psvm+posic_subsis_dest)->str_svm.svmpars.pacidest);
		
        strncpy(pforitn->fidatiso.fiisoctr.fictr093.longitud,(psvm+posic_subsis_dest)->str_svm.svmpars.pacidest,2);
        pforitn->fidatiso.fiisoctr.fictr093.longitud[2] = '\0';
        strncpy(pforitn->fidatiso.fiisoctr.fictr093.datos ,(psvm+posic_subsis_dest)->str_svm.svmpars.pacidest + 2,11);
        pforitn->fidatiso.fiisoctr.fictr093.datos[11] = '\0';
       break;
    }//fin del if subsis 30 PRICE
   
  }
    return(retorno);
}

/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/

int trata_recep1804(formtPant * datEnt,formato_interno * pforitn)
{
   int retorno;
   
   if (retorno = fgenera_1804(datEnt,pforitn)!= 0)
   {
        return(1);        // error al generar 1804 (ya se ha grabado la incidencia)
   }


  /***    configurar tabla de operaciones    ***/
#if defined (TRAZA) && (TRAZA > 1)  
   fprintLog(nomProg,"antes de monta_claves1804 con numop %.6s\n", pforitn->fidatpro.finumope);
#endif  

   if ((retorno = fmonta_claves1804(pforitn)) != 0)
   {
       fprintLog(nomProg,"error montar claves : mensaje no contemplado\n");
       strcpy(pforitn->fiisocom.respusal,"909");
       return(1); // error al montar claves
   }
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg,"claves son %s\n", pforitn->fidatpro.ficlatab);
#endif

    return(0);
}

/***************************************************************************
   FUNCION   FMONTA_CLAVES1804
   DESCRIPCION: Pone las claves de operacion para los mendajes de control de pantalla
*************************************************************************************/
int fmonta_claves1804 (formato_interno *pforitn)
{
    strncpy(pforitn->fidatpro.ficlatab     , "18",2);
    strncpy(pforitn->fidatpro.ficlatab + 2 , "00",2);
#if defined (TRAZA) && (TRAZA > 1)	
    fprintLog(nomProg,"monta respri %s\n", pforitn->fidatpro.firespri);
#endif	
    strncpy(pforitn->fidatpro.ficlatab + 4 , pforitn->fidatpro.firespri,2);
    strncpy(pforitn->fidatpro.ficlatab + 6 , "00",2);
    strncpy(pforitn->fidatpro.ficlatab + 8 , "00",2);
    strncpy(pforitn->fidatpro.ficlatab +10 , "01",2);
    pforitn->fidatpro.ficlatab[12] = '\0';

    return (0);
}
