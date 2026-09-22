/**************************************************************************
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
#include "colas.h"
#include "procesos.h"
//#include "ftradreso.h"
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
struct sfdssl * sockssl; //PRJ0258542
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
int fmonta_claves1804(formtPant * datEnt,formato_interno *pforitn); //PRJ0258542 - se nade 1 parametro
int     inicfori (int tipomsg, formato_interno *pfi);
int     fnumope (char *numope, char * numcross, char Indice);
/***  ---------------   mensaje recibido   -------------   */
mensaje_lin msglin;
//mensaje_comunicacion  msglin;
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
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);

   strcpy(coderror, "   ");

   strcpy(reginci.innompro,"RECEP");
   strcat(reginci.innompro,subsistema_general);

   fprintLog(nomProg, "inicio precep01\n");

   /* --------- se ata a la memoria compartida SVM  ------------ */
   fprintLog(nomProg, "-- Entrada conecSVM --\n");
	 retorno=conecSVM((void *)&psvm); //ORIGINAL
   fprintLog(nomProg, "-- Salida conecSVM --%i--\n",retorno);
  

    if (retorno != 0)
    {
     //  fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
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
       fprintLog(nomProg,"ERROR ARRANQUE NO ESTOY EN SVM %s\n", argv[0]);
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "250");
       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }
    /* cola de mensajes de PRECEP  */

    qrecep = (psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
    fprintLog(nomProg,"-- Cola por donde va a recibir %d\n",qrecep); 
    if (qrecep == -1)
    {
       fprintLog(nomProg,"error crea cola qrecep\n");
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
	   strcpy(reginci.innomfic,"QRECEP00");
       strcpy(coderror, "009");
       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }

    //RAQUEL modificado para recoger los datos por bbdd y entidad
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
	   fprintLog(nomProg,"error crea cola qrtratami\n");
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
	   strcpy(reginci.innomfic,"QRTRAT00");
       strcpy(coderror, "009");
	   perror("error >>>");
       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }

   /****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
           LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR
   *******/

   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   /* graba su pid en la SVM */
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   (psvm+rtrata_svm)->estado=ESPERA;
   fprintLog(nomProg,"pid asignado %d\n", getpid()); //PRJ0247167
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "precep01.c - se duerme\n"); //PRJ0247167
#endif   
   pause();
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "precep01.c - despierta\n"); //PRJ0247167
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
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg,"TRABAJO  \n");
#endif   

/* -- obtiene el desplazamiento del puntero para los datos del subsistema --*/
/* Carga la posicion de los distintos subsistemas en la tabla tsubsis */
      sprintf(auxsub,"%.2s01",subsistema_general);
	  auxsub[4]='\0';
#if defined (TRAZA) && (TRAZA > 10)
      fprintLog(nomProg, "Busco subsistema origen >%s<\n",auxsub);
#endif
      posic_subsis= encuentra_svm(psvm,auxsub,TIP_SUBSIS);
      if (posic_subsis < 0)
	  {
         tsubsis=-1;
         fprintLog(nomProg,"no encuentra svm del subsistema >%s<\n", auxsub);
         (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
         strcpy(reginci.innomfic,"      ");
         strcpy(coderror, "251");
         exit(finalizar(-1)); //sin svm supervis no puede arrancar
	  }
      else
         tsubsis=posic_subsis;

#if defined (TRAZA) && (TRAZA > 10)
    fprintLog(nomProg, "obtenido posic_subsis >%d< y guardado tsubsis >%d<\n",posic_subsis,tsubsis);
#endif


   /****** BUCLE DE TRATAMIENTO DE MENSAJES ******/

   trata_mensaje = 1;
   /* ---------- lee con espera de la cola de mensajes --------------- */
   fprintLog(nomProg, " Va a empezar a esperar en cola de espera \n"); //PRJ0247167
   while (trata_mensaje == 1)
   {
#if defined (TRAZA) && (TRAZA > 5)
    fprintLog(nomProg, "****Lee con espera de cola qrecep >%d</////////////// \n", qrecep);
#endif
	  //PRJ0247167 - Revisado ampliación P48 - la funcion msgrcv no tiene problema con la longitud 4096 nueva del mensaje
      retorno = msgrcv(qrecep, &msglin, long_leerlin, 0, 0);
	  fprintLog(nomProg, "* Despues de msgrcv con retorno %i\n",retorno); //PRJ0247167
	  fprintLog(nomProg, "Mensaje completo >%d< >%.61s< y de long_leerlin >%i<\n", msglin.tipo, msglin.mensaje_linea, long_leerlin);
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
        fprintLog(nomProg,"orden de cierre \n");
        strcpy(coderror, "999");
        if ((psvm+rtrata_svm)->estado==RECARGAR)
          exit(finalizar(-2)); /* Paro para rearranque */
        else
          exit(finalizar(0)); /* Orden de Cierrre */
      }

     posic_subsis = tsubsis;

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

      fprintLog(nomProg, "Busco subsistema destino>%s<\n",auxsub);

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

      fprintLog(nomProg,"tipo %i \n", msglin.tipo);

      msglin.tipo = 1111;
      
      memcpy(&msglin.mensaje_linea, &fiforitn, long_iso);
#if defined (TRAZA) && (TRAZA > 1)	  
      fprintLog(nomProg,"enviar por la cola %i el tamaño %i\n", qrtratami, long_iso);
#endif	  
      
      
      //fhexdump(stderr, &msglin, long_iso+4);
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
   DESCRIPCION: Genera un mensaje de control traduciendo lo que le llega de 
   la lanzadora correspondiente
/***************************************************************************/
int fgenera_1804(formtPant * datEnt, formato_interno * pforitn)
{
	char numoper[6];
  char numcross[10];
  char tipclac[2]; //PRJ0258542
  int tipclai; //PRJ0258542
  char cadenaSal[151]; //PRJ0258542
  char identi[50]; //PRJ0258542

  int retorno=0;
	int ll;
  tm_dec tm;
  fechorsys(&tm);
	
  retorno = inicfori(1804, pforitn);
  if (strncmp(datEnt->pttipmsj,"V",1)==0 || strncmp(datEnt->pttipmsj,"G",1)==0) //PRJ0258542 - Se mete nuevo control para repeticiones
  {
  	strcpy(pforitn->fiisocom.fiiso000 , "1805");
  }
  else
  {
  	strcpy(pforitn->fiisocom.fiiso000 , "1804");
  }

  strncpy(pforitn->fidatpro.fifecha0,"20",2); //PRJ0258542
  //strncpy(pforitn->fidatpro.fifecha0,"%02d", tm.tm_year); //PRJ0258542
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
  for (ll=0 ; ll<2 ; ll++) //inicio for 1
	{
		if (!strncmp(datEnt->ptsubsis,reg_para[ll].psresolu,2))
    {
#if defined (TRAZA) && (TRAZA > 1)	
			fprintLog(nomProg,"tst para el resolutor de medios de pago\n");
#endif		
			strncpy(pforitn->fidatiso.fiisoctr.fictr028.aa,(psvm+posic_subsis_dest)->str_svm.svmpars.pafecses    , 2);
			strncpy(pforitn->fidatiso.fiisoctr.fictr028.mm,(psvm+posic_subsis_dest)->str_svm.svmpars.pafecses + 2, 2);
			strncpy(pforitn->fidatiso.fiisoctr.fictr028.dd,(psvm+posic_subsis_dest)->str_svm.svmpars.pafecses + 4, 2);
			//strncpy(pforitn->fidatiso.fiisoctr.fictr029,(psvm+posic_subsis_dest)->str_svm.svmpars.panumses,3);

      strncpy(pforitn->fidatpro.fiorigen,"01",2);
			strncpy(pforitn->fidatpro.fisubori,"01",2);
	
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
      memset(pforitn->fidatiso.fiisoctr.fictr094.longitud,'0',sizeof(pforitn->fidatiso.fiisoctr.fictr094.longitud)); //PRJ0258542
      memset(pforitn->fidatiso.fiisoctr.fictr094.datos,' ',sizeof(pforitn->fidatiso.fiisoctr.fictr094.datos)); //PRJ0258542

      memset(pforitn->fidatiso.fiisoctr.fictr093.longitud,'0',sizeof(pforitn->fidatiso.fiisoctr.fictr093.longitud));
      memset(pforitn->fidatiso.fiisoctr.fictr093.datos,' ',sizeof(pforitn->fidatiso.fiisoctr.fictr093.datos));
			break;		
		}
	} //final for 1
  ///****************TST PRICE
	//RAQUEL modificado para utilizar los datos por bbdd y entidad
  for (ll=0 ; ll<2 ; ll++) 
  {  //inicio for 2
 		if (!strncmp(datEnt->ptsubsis,reg_para[ll].psnument,2))
    {
#if defined (TRAZA) && (TRAZA > 1)	
    	fprintLog(nomProg,"Mensaje para Redsys\n");
#endif		
      
			strncpy(pforitn->fidatpro.fiorigen,reg_para[ll].psmonito,2);
			strncpy(pforitn->fidatpro.fisubori,"01",2);
			strncpy(pforitn->fidatpro.firespri,datEnt->ptsubsis,2);
			strncpy(pforitn->fidatpro.fisubres, datEnt->ptsubapl,2);
			strncpy(pforitn->fiisocom.fiidtran ,pforitn->fidatpro.finumope,6);
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
      	strncpy(pforitn->fiisocom.fiiso024 , "831",3); //test
				//strncpy(pforitn->fiisocom.fiiso024,"801",3); // sign on para poder probarlo ahora
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
        	{
          	strncpy(pforitn->fiisocom.fiiso024,"801",3);
          }
          else // CAMBIO DE CARACTERISTICAS DE DIALOGO
          {
          	if (strncmp(datEnt->pttipmsj,"P",1)==0 || strncmp(datEnt->pttipmsj,"G",1)==0) //PRJ0258542 - Se mete nuevo control P para normal, G para repeticion
          	{
          		strncpy(pforitn->fiisocom.fiiso024,"811",3); //Peticion generacion nuevas claves
          	}
          	else if (strncmp(datEnt->pttipmsj,"R",1)==0 || strncmp(datEnt->pttipmsj,"V",1)==0) //PRJ0258542 - Se mete nuevo control R para normal, V para repeticion
          	{
          		strncpy(pforitn->fiisocom.fiiso024,"816",3); //Peticion verificacion claves
          	}
          	else
          	{
          		strncpy(pforitn->fiisocom.fiiso024,"890",3);
          	} //PRJ0258542 - Fin igual P o G
          }
        	if (strncmp(datEnt->pttipmsj,"P",1)!=0 && strncmp(datEnt->pttipmsj,"R",1)!=0 && strncmp(datEnt->pttipmsj,"V",1)!=0 && strncmp(datEnt->pttipmsj,"G",1)!=0) //PRJ0258542 - Se mete nuevo control
        	{
          	if (strncmp(datEnt->caractedialogo,"TOT",3)==0)
          	{
            	strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507000",8);
            }
						else
						{
            	if (strncmp(datEnt->caractedialogo,"DEB",3)==0)
            	{
              	strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507001",8);
              }
							else
							{
            		if (strncmp(datEnt->caractedialogo,"CRE",3)==0)
            		{
                	strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507002",8);
                }
								else
								{
		       				strncpy(pforitn->fidatiso.fiisoctr.fictr048.contr.cardia,"00507000",8);
		       			}
		       		}
			       	
#if defined (TRAZA) && (TRAZA > 1)			   
			    	fprintLog(nomProg,"fi48 >%.*s<\n",8,pforitn->fidatiso.fiisoctr.fictr048.contr.cardia);
#endif
						}
					} //PRJ0258542 - Fin distinto P, R, V y G
        }
    	}
    	if (strncmp(datEnt->pttipmsj,"P",1)==0 || strncmp(datEnt->pttipmsj,"R",1)==0 || strncmp(datEnt->pttipmsj,"V",1)==0 || strncmp(datEnt->pttipmsj,"G",1)==0) //PRJ0258542 - Se mete nuevo control
    	{
    		fprintLog(nomProg, "Mensaje de control de claves, entrada >%.3s<\n",datEnt->pttipmsj);
    		
				strncpy(pforitn->fidatiso.fiisocla.ficla011, pforitn->fidatpro.finumope,6);
				pforitn->fidatiso.fiisocla.ficla011[6] = '\0';
				strncpy(pforitn->fidatiso.fiisocla.ficla012, pforitn->fidatpro.fifecha0 + 2,6);
				strncpy(pforitn->fidatiso.fiisocla.ficla012 + 6, pforitn->fidatpro.fihora00,6);
				pforitn->fidatiso.fiisocla.ficla012[12] = '\0';
      	if (strncmp(datEnt->pttipmsj,"P",1)==0 || strncmp(datEnt->pttipmsj,"G",1)==0)
      	{
      		strncpy(pforitn->fiisocom.fiiso024,"811",3); //Peticion generacion nuevas claves
      		strncpy(pforitn->fidatiso.fiisocla.ficla024,"811",3); //Peticion generacion nuevas claves
      	}
      	if (strncmp(datEnt->pttipmsj,"R",1)==0 || strncmp(datEnt->pttipmsj,"V",1)==0)
      	{
      		strncpy(pforitn->fiisocom.fiiso024,"816",3); //Peticion verificacion claves
      		strncpy(pforitn->fidatiso.fiisocla.ficla024,"816",3); //Peticion generacion nuevas claves
      	}
     		pforitn->fidatiso.fiisocla.ficla024[3] = '\0';
     		strncpy(pforitn->fidatiso.fiisocla.ficla025,"8100",4);
     		pforitn->fidatiso.fiisocla.ficla025[4] = '\0';
	      strncpy(pforitn->fidatiso.fiisocla.ficla033.longitud,(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui,2);
	      pforitn->fidatiso.fiisocla.ficla033.longitud[2] = '\0';
	      strncpy(pforitn->fidatiso.fiisocla.ficla033.datos ,(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui + 2,11);
	      pforitn->fidatiso.fiisocla.ficla033.datos[11] = '\0';

				strncpy(pforitn->fidatiso.fiisocla.ficla053.control,"02",2);
				pforitn->fidatiso.fiisocla.ficla053.control[2] = '\0';
				//strncpy(pforitn->fidatiso.fiisocla.ficla053.identificador,"000000",6); //PRJ0258542 - Quitado fase 3
				strncpy(pforitn->fidatiso.fiisocla.ficla053.identificador,pforitn->fidatiso.fiisocla.ficla033.datos,6); //PRJ0258542 - Sustituido fase 3
				strncpy(tipclac,datEnt->pttipmsj+1,2);
				tipclac[2] = '\0';
				tipclai = atoi(tipclac);
				strncpy(pforitn->fidatiso.fiisocla.ficla053.identificador+6,tipclac,2);
				pforitn->fidatiso.fiisocla.ficla053.identificador[8] = '\0';
				strncpy(pforitn->fidatiso.fiisocla.ficla053.algoritmo,"05",2);
				pforitn->fidatiso.fiisocla.ficla053.algoritmo[2] = '\0';
				strncpy(pforitn->fidatiso.fiisocla.ficla053.longitud,"0000",4);
				pforitn->fidatiso.fiisocla.ficla053.longitud[4] = '\0';
				strncpy(pforitn->fidatiso.fiisocla.ficla053.proteccion,"04",2);
				pforitn->fidatiso.fiisocla.ficla053.proteccion[2] = '\0';
				strncpy(pforitn->fidatiso.fiisocla.ficla053.resto,"000000",6);
				pforitn->fidatiso.fiisocla.ficla053.resto[6] = '\0';

	      strncpy(pforitn->fidatiso.fiisocla.ficla093.longitud,(psvm+posic_subsis_dest)->str_svm.svmpars.pacidest,2);
	      pforitn->fidatiso.fiisocla.ficla093.longitud[2] = '\0';
	      strncpy(pforitn->fidatiso.fiisocla.ficla093.datos ,(psvm+posic_subsis_dest)->str_svm.svmpars.pacidest + 2,11);
	      pforitn->fidatiso.fiisocla.ficla093.datos[11] = '\0';
	      strncpy(pforitn->fidatiso.fiisocla.ficla094.longitud,(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui,2);
	      pforitn->fidatiso.fiisocla.ficla094.longitud[2] = '\0';
	      strncpy(pforitn->fidatiso.fiisocla.ficla094.datos ,(psvm+posic_subsis_dest)->str_svm.svmpars.paciadqui + 2,11);
	      pforitn->fidatiso.fiisocla.ficla094.datos[11] = '\0';
				strncpy(pforitn->fidatiso.fiisocla.ficla096.longitud, "000",3);
				pforitn->fidatiso.fiisocla.ficla096.longitud[3] = '\0';
				strncpy(pforitn->fidatiso.fiisocla.ficla096.datos," ",360);
				pforitn->fidatiso.fiisocla.ficla096.datos[360] = '\0';
	      strncpy(pforitn->fidatiso.fiisocla.ficlamac, "AAAAAAAA",8);
	      pforitn->fidatiso.fiisocla.ficlamac[8] = '\0';
	      

				fprintLog(nomProg,"RESTO CAMPOS DEL MENSAJE A ENVIAR >%.4s< \n", pforitn->fiisocom.fiiso000); //Llega en todos
				fprintLog(nomProg,"Subsistema y subaplicaci >%.2s< >%.2s<\n", pforitn->fidatpro.firespri, pforitn->fidatpro.fisubres); //Llega en todos
				fprintLog(nomProg,"  Valor del P11          >%.6s< \n", pforitn->fidatiso.fiisocla.ficla011); //MAC - Llega en todos
				fprintLog(nomProg,"  Valor del P12          >%.12s< \n", pforitn->fidatiso.fiisocla.ficla012); //MAC - Llega en todos
				fprintLog(nomProg,"  Valor del P24          >%.3s< \n", pforitn->fidatiso.fiisocla.ficla024); //MAC - solo 1804, 1805, 1824, 1825
				fprintLog(nomProg,"  Valor del P25          >%.4s< \n", pforitn->fidatiso.fiisocla.ficla025); //MAC - solo 1804, 1805, 1824, 1825
				fprintLog(nomProg,"  Valor del P33          >%.2s< >%.11s< \n", pforitn->fidatiso.fiisocla.ficla033.longitud, pforitn->fidatiso.fiisocla.ficla033.datos); //MAC - solo 1804, 1805, 1824, 1825
				fprintLog(nomProg,"  Valor del P53          >%.2s%.8s%.2s%.4s%.2s%.6s< \n", pforitn->fidatiso.fiisocla.ficla053.control, pforitn->fidatiso.fiisocla.ficla053.identificador, pforitn->fidatiso.fiisocla.ficla053.algoritmo, 
																																										pforitn->fidatiso.fiisocla.ficla053.longitud, pforitn->fidatiso.fiisocla.ficla053.proteccion, pforitn->fidatiso.fiisocla.ficla053.resto); //MAC - 1804, 1805, 1824, 1825, ¿1814
				fprintLog(nomProg,"  Valor del S93          >%.2s< >%.11s< \n", pforitn->fidatiso.fiisocla.ficla093.longitud, pforitn->fidatiso.fiisocla.ficla093.datos); // en todos
				fprintLog(nomProg,"  Valor del S94          >%.2s< >%.11s< \n", pforitn->fidatiso.fiisocla.ficla094.longitud, pforitn->fidatiso.fiisocla.ficla094.datos); // en todos
				fprintLog(nomProg,"  Valor del S96          >%.3s< >%.360s< \n", pforitn->fidatiso.fiisocla.ficla096.longitud, pforitn->fidatiso.fiisocla.ficla096.datos); // en todos
    	}
    	else //PRJ0258542 - el resto sigue igual
    	{
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
			}
     	break;
    }//fin del if subsis 30 PRICE

	} //final for 2
  return(retorno);
}

/***************************************************************************************/
/***************************************************************************************/
/***************************************************************************************/

int trata_recep1804(formtPant * datEnt,formato_interno * pforitn)
{
	int retorno;
	fprintLog(nomProg,"Entrada a trata_recep1804 20241108\n"); //PRJ0258542
  if (retorno = fgenera_1804(datEnt,pforitn)!= 0)
  {
  	return(1);        // error al generar 1804 (ya se ha grabado la incidencia)
  }


  /***    configurar tabla de operaciones    ***/
#if defined (TRAZA) && (TRAZA > 1)  
   fprintLog(nomProg,"antes de monta_claves1804 con numop %.6s\n", pforitn->fidatpro.finumope);
#endif  

   if ((retorno = fmonta_claves1804(datEnt,pforitn)) != 0) //PRJ0258542 - se anade 1 parametro
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

/************************************************************************************
   FUNCION   FMONTA_CLAVES1804
   DESCRIPCION: Pone las claves de operacion para los mensajes de control de pantalla
*************************************************************************************/
int fmonta_claves1804 (formtPant * datEnt, formato_interno *pforitn)
{
	if (strncmp(datEnt->pttipmsj,"P",1) ==0 || strncmp(datEnt->pttipmsj,"R",1) ==0) //PRJ0258542 - Se mete nuevo control
	{
  	strncpy(pforitn->fidatpro.ficlatab     , "18",2);
    strncpy(pforitn->fidatpro.ficlatab + 2 , "00",2);
#if defined (TRAZA) && (TRAZA > 1)	
    fprintLog(nomProg,"monta respri para claves %s\n", pforitn->fidatpro.firespri);
#endif	
    strncpy(pforitn->fidatpro.ficlatab + 4 , pforitn->fidatpro.firespri,2);
    strncpy(pforitn->fidatpro.ficlatab + 6 , "00",2);
    strncpy(pforitn->fidatpro.ficlatab + 8 , "00",2);
    strncpy(pforitn->fidatpro.ficlatab +10 , pforitn->fidatpro.firespri,2);
    pforitn->fidatpro.ficlatab[12] = '\0';
	}
	else
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
	}
	fprintLog(nomProg,"  Valor de ficlatab      >%.12s<\n", pforitn->fidatpro.ficlatab);
  return (0);
}
