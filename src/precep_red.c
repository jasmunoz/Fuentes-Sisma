//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
/***************************************************************************
   PROGRAMA:  precep_red
   EJECUTABLE: precep21 y precep19
   DESCRIPCION: Proceso residente encargado del tratamiento de los mensajes
                que llegan  de PUC. Este proceso lee de su cola de entrada
                con espera, y puede tratar : - carga de tablas dinamica
                                             - cierre del proceso
                                             - mensaje origen PUC
                El mensaje origen PUC se traduce, y se trata segun sea 
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

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>


#include "netwin.h"
#include "cforitno.h"
#include "nptinci.h"
#include "nptcasa.h"
#include "nptdhsm.h"
#include "nptiden.h"
#include "traeetiq.h"

#include "datagen.h"
#include "deffich.h"
#include "svm.h"
#include "colas.h"
#include "procesos.h"

#include "cformoni.h"
#include "ftradred.h"
#include "msgclien.h"
#include "nptpara.h"

NPSINCI reginci;
formato_interno fiforitn, *pforitn; 


typedef struct sfdssl {         /* estructura conjunta para socket y socket seguro */
 int sfd_ssl;
 SSL * ssl_ssl;
} sfdssl;

 struct sfdssl stsocketssl;
 struct sfdssl * sockssl;


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
void    sigue(); 
int     finalizar(int codigo); 
int     fcarga_tablas(char * red);
int     fmonta_claves (formato_interno *pforitn);
int     fclase_tipomsg(int tipomsg);
int     fcasar_respu(formato_interno *pforitn, mensaje_nuc *ppend);
void    monta_previa(formato_interno *pfi);
int     ftradred(char usotrad,int sentido,int suba_iso,int *long_iso, 
char    *mensaje_puc,formato_interno *mensaje_netplus,
int     *error_code,int *error_bit);
int     fsemaforo_red(int ll); 
int     fgenera_admin(); 
#ifdef LISTANEGRAONLINE
int     fgenera_petln(formato_interno *pforitn); 
#endif                 
int     fgenera_deneg(formato_interno *pforitn); 
int     fenvia_var(formato_interno *pfoin); 
int     fenvia_rtratami(formato_interno *pforitn);
char    *itoa(int numero);
int     fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
int     fnumope (char *numope, char * numcross, char Indice);
void    fprintLog(char * pf, char * msg, ...);
int     busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int     damecola(SVM *psvm_par,char *proce,char *subsis);
int     encuentra_svm(SVM *psvm_par,char *aux,int tipo);
long    milelong();
void    fver_operacion(mn_formato_interno *pmnforitn, char *operacion);
int     inicfori (int tipomsg, formato_interno *pfi);
int     fconvres(formato_interno *pfi);
int     nspaces (char *c,int n);
int     fjuliana(fecha *fechag,juliana *fechaj);
void    fechorsys(struct tm_dec *tmPtr);
int     fgregoriana(juliana * fechaj,fecha * fechag);
int     ConecSVMaux(void *gen_psvm,int shmid);
int     fver_op(FILE * fic, formato_interno * fi, char * prog);
int     conexhsm(char* codent, NPSDHSM reg_hsm);
void    desconexhsm(int sock);
int     conexion_BBDD();


/***  ---------------   mensaje recibido   -------------   */
mensaje_lin    msglin; 
int     long_leerlin = sizeof(msglin) - sizeof(msglin.tipo);

int rtrata_svm;
char p04[3];
char coderror[4];
int qenvio, qrtratami;

/***   cadena con el valor 16_44_ empaquetado   ***/
char tipo_1644[2] = {22,68};
/**************************************************/

SVM *psvm;
long tiempoL;
char   subaplicacion[3];

int cod_err;
int tsubsis;
char nomProg[50];
int   sfd_hsm;
NPSPARA  *reg_para;

int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
   int trata_mensaje,ll,clasemsg;
   tm_dec tm;
   char numoper[6]; 
   char numcross[10]; 
   char auxsub[5]; 
   int retorno,queda;
   int tipomsg; 
   int qrecep;
   mensaje_nuc ppend;

   int posic_subsis;
   NPSDHSM registro;
   int j;
   
   char cola_envio[9];

   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);

   strncpy(subsistema_general,argv[0]+strlen(PRECEP),2);
   subsistema_general[2]='\0';
   strcpy(nomProg,argv[0]);
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);

   strcpy(coderror, "   ");
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg, "** INICIO PROGRAMA precep_red.c ** \n");
#endif

   strcpy(reginci.innompro,"RECEP");
   strcat(reginci.innompro,subsistema_general);


   // --------- se ata a la memoria compartida SVM  ------------ 

	 retorno=conecSVM((void *)&psvm); //ORIGINAL

    if (retorno != 0)
    {
/*#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif*/
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg, "** Conectado al SVM \n");
#endif

    rtrata_svm=busca_proc_svm(psvm,argv[0],-1);
    if (rtrata_svm < 0 )
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR ARRANQUE NO ESTOY EN SVM %s\n", argv[0]);  
#endif
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "250");
       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }      
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"buscamos en SVM  \n");
#endif

    // cola de mensajes de PRECEP  
    qrecep = (psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
    if (qrecep == -1)
    {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error crea cola qrecep\n");  
#endif
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,reginci.innompro);
       reginci.innomfic[0]='Q';
       strcpy(coderror, "009");
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion qrecep%.2s codigo de error >%.3s<\n",subsistema_general,coderror);

       exit(finalizar(-1)); //sin svm supervis no puede arrancar
    }
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"Se asigna qrecep: %d\n", qrecep);
#endif
	fprintLog(nomProg,"Dentro de precp_red \n"); //PRJ0258542
  for (j=0 ; j<2 ; j++) 
  {
	  if (strncmp(subsistema_general,reg_para[j].psnument,2)==0)
	  {
	  	  qrtratami = damecola(psvm,reg_para[j].pscltrat,NULL); 
	  	  break;
      }
   	} 
#if defined (TRAZA) && (TRAZA > 10)	
    fprintLog(nomProg,"*** cola qrtrat %d\n", qrtratami);
#endif	
      if (qrtratami == -1)  
      {
         strcpy(reginci.innomfic,"QRTRAT00");
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"error cola %s\n",reginci.innomfic);  
         perror("error >>>");
#endif
         strcpy(coderror, "009");
        //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion QRTRAT00 codigo de error >%.3s<\n",coderror);

         fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
      }

  //RAQUEL se modifica para arranque unificado bbdd
    strcpy(cola_envio,PENVIO);
    strcat(cola_envio, subsistema_general);
    cola_envio[8] = '\0';
    qenvio = damecola(psvm, cola_envio,NULL);
  
    if (qenvio == -1)  
    {
       strcpy(reginci.innomfic,"QENVIO");
       strcat(reginci.innomfic,subsistema_general);
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error crea cola QENVIO errno >>%d<\n",errno);
#endif
       strcpy(coderror, "009");
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion QENVIO%.2s codigo de error >%.3s<\n",subsistema_general,coderror);

       fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
    }
#if defined (TRAZA) && (TRAZA > 10)	
	fprintLog(nomProg,"cola qenvio %d\n", qenvio);
#endif	

	
	
   //****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE 
   //        LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR  
   //******
     
   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   // graba su pid en la SVM
 //     queda=alarm(0);
 //  fprintLog(nomProg,"RAQUEL PRECEPRED queda >%d<\n", queda);
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   (psvm+rtrata_svm)->estado=ESPERA;
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg,"se duerme\n");
#endif   
   pause();
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg,"despierta \n"); 
#endif   
   queda=alarm(0);
   if(!queda)
   {
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"ERROR ARRANQUE queda >%d<\n", queda);  
#endif
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

   //meto el subisistema a 00, pq para la funcion fcarga_tablas no se usa.
   sprintf(auxsub,"%.*s00",2,subsistema_general);

   ////////// ABRE LA CONEXION A LA BASE DE DATOS Y PERMANECE ABIERTA, SE CIERRA EN FINALIZAR
	retorno = conexion_BBDD();
	if(retorno)
	{
	   fprintLog(nomProg,"Error en el acceso a la base de datos\n");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: apertura BBDD codigo de error >%i<\n",retorno);
      
	   exit(finalizar(-2)); //supervis intenta rearrancar
	}
   

   //  ---------------  carga tabla de traduccion  ----------------- 
   if ((retorno = fcarga_tablas(auxsub)) != 0) 
   {
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"error carga tablas traduccion \n");  
#endif
      strcpy(reginci.innomfic,"TBTRAPUC");
      strcpy (coderror,"300");
      exit(finalizar(-1)); //supervis intenta rearrancar
   }      

    // ------- NOS CONECTAMOS AL HSM, DEJANDO EL SOCKET ABIERTO ------
     memset(&registro, ' ', sizeof(NPSDHSM));
     sfd_hsm=conexhsm("0001", registro);//por ahora la ponemos a pelo
     if (sfd_hsm<0)
     {
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - ACCESO HSM: conexion HSM codigo de error >%i<\n", sfd_hsm);
     }

   
   //****** BUCLE DE TRATAMIENTO DE MENSAJES ******
   trata_mensaje = 1; 
   //sleep(1); // espero un poco a que se levanten los otros servicios
   // ---------- lee con espera de la cola de mensajes --------------
   while (trata_mensaje == 1)
   {
   
#if defined (TRAZA) && (TRAZA > 5)
      fprintLog(nomProg, "////////////////lee con espera de cola %i/////////////// \n", qrecep);
#endif
      memset(msglin.mensaje_linea, '\0', long_leerlin);
      retorno = msgrcv(qrecep, &msglin, long_leerlin, 0, 0);
	  
      if(retorno== -1)
      {
#if defined (TRAZA) && (TRAZA > 1)
         perror("error >>>");
         fprintLog(nomProg,"error leer cola \n");
#endif
         desconexhsm(sfd_hsm);
         strcpy(coderror, "242");
         strcpy(reginci.innomfic,reginci.innompro);  
         reginci.innomfic[0]='Q'; 
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo lectura qrecep %i codigo de error >%.3s<\n",qrecep, coderror);
       
         exit(finalizar(-1)); //si hay un error en la cola hay que arrancar todo
      }
#if defined (TRAZA) && (TRAZA > 5)
      fprintLog(nomProg,"Recibido >%d<\n", retorno);
      //fhexdump(stderr, &msglin, retorno);
#endif
      fhexdump(stdout, &msglin, 57); 
        // -------------  orden de cierre  ------------------
      if (msglin.tipo == TIPO_PARADA)  // orden de cierre 
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"orden de cierre \n");  
#endif
        desconexhsm(sfd_hsm);  
		
        strcpy(coderror, "999");
        if ((psvm+rtrata_svm)->estado==RECARGAR) 
        {
          //Añadido para Monitorizacion SISMA 
          fprintLog(nomProg,"TIPO SISTEMA - REARRANQUE DE PROCESO: codigo de coderror >%.3s<\n",coderror);

          exit(finalizar(-2)); // Paro para rearranque
        }
        else
        {
          exit(finalizar(0)); // Orden de Cierrre 
        }
      }
	  

//  el manejador de comunicaciones debe informar de algun modo 
//del centro procesador del que llega la operacion en la variable tipo 
      printf("** ** msglin.tipo <%d>\n",msglin.tipo);
      suba_iso = msglin.tipo - (atoi(subsistema_general) * 100);
      sprintf(subaplicacion , "%02d",suba_iso);
      subaplicacion[2]='\0';
      strncpy(auxsub+2, subaplicacion, 2);
      auxsub[4]='\0';

#if defined (TRAZA) && (TRAZA > 10)
      fprintLog(nomProg, " Busco subsistema %s\n",auxsub);
#endif
      posic_subsis= encuentra_svm(psvm,auxsub,TIP_SUBSIS);
      if (posic_subsis < 0)
         tsubsis=-1;
      else
         tsubsis=posic_subsis;

#if defined (TRAZA) && (TRAZA > 10)
      fprintLog(nomProg,"obtenido %d y guardado=%d\n",posic_subsis,tsubsis);
#endif

      posic_subsis = tsubsis;

      if (msglin.mensaje_linea[0] == CAIDA_LINEA)  // orden de cierre 
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"Caida de linea %04d\n",msglin.tipo);
#endif
        if (posic_subsis > 0)
            (psvm+posic_subsis)->str_svm.svmpars.pasemsub = 0;
        continue;
      }

    //    si sabemos por comunicaciones la longitud recibida, la pasamos   
      long_iso = retorno;
      error_code = 0; 
      error_bit = 0; 

    //  ----------  prepara las variables para traducir  ----------
      usotrad = '0';
      sentido = 1;

      //  cojo el tiempo de entrada del mensaje   ***
      tiempoL = milelong();

 
//#if defined (TRAZA) && (TRAZA > 10)
//     fechorsys(&tm);
//     fprintLog(nomProg,"tiempo antes de ftradred: %d:%d:%d.%d \n",
//             tm.tm_hour,tm.tm_min,tm.m_sec,tm.tm_mls);
//#endif

	 
    //  ----------  traduce el mensaje de origen red  ---------- 
 
      memset(&mensaje_netplus, ' ', sizeof(formato_interno));

      retorno = ftradred(usotrad,sentido,suba_iso,&long_iso,
                              msglin.mensaje_linea,&mensaje_netplus,
                              &error_code,&error_bit);
   
      if(retorno)
      { 
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"error traduccion red1 >%d<\n", retorno);
#endif
         strcpy(coderror, "299");
         strcpy(p04,itoa(error_bit));
         lpad(p04,'0',3);
         strncpy(reginci.incoderr,p04,3);
         strncpy(reginci.incoderr+3,"E",1);
         strcpy(p04,itoa(error_code));
         lpad(p04,'0',3);
         strncpy(reginci.incoderr+4,p04,3);
         fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
         strncpy(reginci.incoderr,"        ",8);
         if (strncmp(msglin.mensaje_linea , tipo_1644 , 2) != 0)
         {   //  solo se contestan con error los distintos de 1644 
            fgenera_admin();
         }
         continue;                     
      }

 
//#if defined (TRAZA) && (TRAZA > 10)
//      fechorsys(&tm);
//      fprintLog(nomProg,"tiempo despues de ftradred: %d:%d:%d.%d \n",
//             tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mls);  
//#endif

      if(error_code)
      { 
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg,"error traduccion red : code >%d< -- bit >%d<\n",
                 error_code,error_bit);
#endif
        strcpy(coderror, "299");
        strcpy(p04,itoa(error_bit));
        lpad(p04,'0',3);
         strncpy(reginci.incoderr,p04,3);
         strncpy(reginci.incoderr+3,"E",1);
         strcpy(p04,itoa(error_code));
         lpad(p04,'0',3);
         strncpy(reginci.incoderr+4,p04,3);
         fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
         strncpy(reginci.incoderr,"        ",8);
         if (strncmp(msglin.mensaje_linea , tipo_1644 , 2) != 0)
         {   // solo se contentan con error los distintos de 1644  
            fgenera_admin();
         }
         continue;                     
      }
  // -------------- el mensaje ha sido traducido correctamente ---------

      pforitn = &mensaje_netplus;
#if defined (TRAZA) && (TRAZA > 1)
      //fver_op(stderr, pforitn, reginci.innompro);
#endif
//***--------   comprueba el tipo de mensaje recibido    ---------------
      tipomsg = atoi(pforitn->fiisocom.fiiso000);
      clasemsg = fclase_tipomsg(tipomsg);
      
      if (clasemsg == DESCONOCIDO)
      {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"mensaje no contemplado : %d\n",tipomsg);
#endif
         strcpy(coderror, "301");
         //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%d< codigo de error >%03d<\n",tipomsg,coderror);


         fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
         if (tipomsg != 1644)
         {   //***   solo se contentan con error los distintos de 1644  ***
            fgenera_admin();
         }
         continue;                     
      }

#ifdef LISTANEGRAONLINE
  //***   si llega una respuesta que implique solicitar lista negra,
  //      enviamos un 1644 al red, y seguimos el tratamiento normal  
     if ((strncmp(pforitn->fiisocom.respusal,"060",3) == 0) ||
         (strncmp(pforitn->fiisocom.respusal,"560",3) == 0) ||
         (strncmp(pforitn->fiisocom.respusal,"563",3) == 0) ||
         (strncmp(pforitn->fiisocom.respusal,"561",3) == 0) ||
         (strncmp(pforitn->fiisocom.respusal,"660",3) == 0))
     {
#if defined (TRAZA) && (TRAZA > 10)
        //fprintf(stderr,"voy a pedir lista negra por p-39 = %.*s\n", 3,
        //                pforitn->fiisocom.respusal);
#endif

         if (tipomsg != 1644)
         {   //***   solo se contestan con error los distintos de 1644   **
            fgenera_petln(pforitn); 
         }
     }
#endif      

//RAQUEL  ---Si nos envia Resdys  iso003 digito 1 y 2 = 94 estamos en lista negra y debemos devolver 1644         
//lista negra =94 lista negra  fidatiso.fiisoaut.fiaut003.dig12 
      if (strncmp(pforitn->fidatiso.fiisoaut.fiaut003.dig12,"94",2) == 0)
      { 
         //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%d< codigo proceso %.2s lista negra\n",tipomsg,pforitn->fidatiso.fiisoaut.fiaut003.dig12);
         
         if (tipomsg != 1644) 
         {   //***   solo se contentan con error los distintos de 1644  ***
            fgenera_admin();
         }
         continue;  
      }
      

     if ((clasemsg == RESPUESTAS_NUCLEO) ||
          (clasemsg == RESPUESTAS_OTROS))
     {
     		fprintLog(nomProg,"Mensaje tipo P00 >%.4s< y P39 >%.3s<\n", pforitn->fiisocom.fiiso000, pforitn->fidatiso.fiisocla.ficla039); //PRJ0258542
     		retorno = fcasar_respu(pforitn,&ppend);
     		if(retorno)
     		{
      		switch(retorno)
      		{
         		case 1:
            	fprintLog (nomProg,"respuesta fuera de tiempo : %d\n",tipomsg);
            	strcpy(coderror, "303");
            	break;
         		case 2:
	            fprintLog (nomProg,"Datos de peticion no coinciden : %d\n",tipomsg);
  	          strcpy(coderror, "321");
    	        break;
        	 	case 3:
	            fprintLog (nomProg,"MAC recibido erroneo : %d\n",tipomsg);
  	          strcpy(coderror, "916"); //PTASK0022156 - No exite 282, cambiar siempre por 916 - {916,282},
    	        break;
      	   	case 10:
            	fprintLog (nomProg," Error en cola\n");
            	strcpy(coderror, "909");
            	break;
         		case 11:
            	fprintLog(nomProg,"Codigo de Respuesta erroneo >%s<\n", pforitn->fiisocom.respusal);
            	fprintLog(nomProg,"Valor de ficlatab antes     >%.12s<\n", ppend.fiforitn.fidatpro.ficlatab); //PRJ0258542 - Fase 3
            	strcpy(coderror, "110");
            	fgenera_admin();
            	strcpy(ppend.fiforitn.fiisocom.respusal, pforitn->fiisocom.respusal);
            	strcpy(ppend.fiforitn.fidatpro.ficodfin, coderror);
            	strcpy(ppend.fiforitn.fidatpro.firesfin, "00"); 
            	strncpy(ppend.fiforitn.fidatpro.ficlatab + 6, "00" , 2);
            	strncpy(ppend.fiforitn.fidatpro.ficlatab + 8, "50" , 2);
            	fprintLog(nomProg,"Valor de ficlatab despues   >%.12s<\n", ppend.fiforitn.fidatpro.ficlatab); //PRJ0258542 - Fase 3
            	break;
         		default:
           		break;
      		}
      		fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
      		if(strncmp(coderror, "110", 3))
         		continue;
     		} // if del retorno de casar
 
       		// estamos en respuesta al nucleo: cargo el tiempo de salida del resolutor 
       		sprintf(ppend.fiforitn.fidatpro.fitisare, "%08d", tiempoL); 
    			//  ----------  envia la respuesta al rtratami ---------- 
       		if ((retorno = fenvia_rtratami(&ppend.fiforitn)) != 0)
       		{
	          fprintLog(nomProg,"error al enviar al rtratami >%d<\n", retorno);
        		//Añadido para Monitorizacion SISMA 
        		fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Envio cola QRTRAT00 codigo de error >%.3s<\n",coderror);

          	fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
       		}
					continue;
				} //PRJ0258542 - Fin del bulce para mensajes control

    //  ----------  asigna numero de operacion  ---------- 
#if defined (TRAZA) && (TRAZA > 10)	
	 fprintLog(nomProg, "---- Antes de fnumope - precep_red.c\n");
#endif
     // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a la longitud total de finumPAN 
     if ((retorno = fnumope(numoper,numcross,pforitn->fidatiso.fiisoaut.finumPAN.datos[15])) != 0)
     { 
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"error al asignar numero de operacion \n");
#endif
         strcpy(coderror,"264");
         strcpy(p04,itoa(retorno));
         lpad(p04,'0',3);
         strncpy(reginci.incoderr,p04,3);
         fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
         strncpy(reginci.incoderr,"        ",8);
         strcpy(pforitn->fiisocom.respusal,"909");
         if (tipomsg != 1644)
         { //***   solo se contentan con error los distintos de 1644  
            fgenera_deneg(pforitn);
         }
            continue;                     
     }
     else
     {
	     fprintLog(nomProg, "*** NUMOPE asignado *** >%.6s< precep_red.c *** *** \n", numoper);
         strncpy(pforitn->fidatpro.finumope,numoper,6);
         pforitn->fidatpro.finumope[6] = '\0';
     }
	 
	 
    //*  ----------  acaba de configurar el formato interno  ---------- 
   //****    datos procesales   **
   fechorsys(&tm);

     sprintf(pforitn->fidatpro.fifecha0, "%.*d%.*d%.*d%.*d%.*d%",2,tm.tm_year,2,tm.tm_mon,2,tm.tm_mday,2,tm.tm_hour,2,tm.tm_min);

     sprintf(pforitn->fidatpro.fihora00, "%.*d%.*d%.*d%.*d",2,tm.tm_hour,2,tm.tm_min,2,tm.tm_sec,2,tm.tm_mls);
 

     strncpy(pforitn->fidatpro.fiorigen, subsistema_general,2); 
     strncpy(pforitn->fidatpro.fioriini, subsistema_general,2); 
     strcpy(pforitn->fidatpro.firespri, "  "); 
     strcpy(pforitn->fidatpro.firesfin, "  "); 
     strcpy(pforitn->fidatpro.ficodfin, "   "); 

     strncpy(pforitn->fidatpro.fisubori , subaplicacion,2);

     // estamos en peticiones: cargo el tiempo en tienre para tenerlo en algun sitio mientras viaja la operacion. 
     // Luego se machaca con el tiempo de entrada al resolutor de verdad.
     sprintf(ppend.fiforitn.fidatpro.fitisare, "%08d", tiempoL); 
 
  //***    configurar tabla de operaciones    **
     if ((retorno = fmonta_claves(pforitn)) != 0)
     {
#if defined (TRAZA) && (TRAZA > 1)
        fprintf(stdout,"error montar claves : mensaje no contemplado\n");
#endif
        strcpy(coderror, "301");
        //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%04d< codigo de error >%.3s<\n",tipomsg,coderror);
        
        fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
        strcpy(pforitn->fiisocom.respusal,"909");
        if (tipomsg != 1644)
        { //***   solo se contentan con error los distintos de 1644   **
            fgenera_deneg(pforitn);
        }
            continue;                     
     }
#if defined (TRAZA) && (TRAZA > 1)
  //fver_op(stderr, pforitn, reginci.innompro);
#endif
#if defined (TRAZA) && (TRAZA > 10)
     fprintLog(nomProg,"**** Mandamos al rtratami \n");
#endif	 

    //*  ----------  envia la operacion al rtratami ----------  
      if ((retorno = fenvia_rtratami(pforitn)) != 0)
      { 
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"error al enviar al rtratami\n");
#endif
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Envio cola QRTRAT00 codigo de error >%.3s<\n",coderror);

         fgraba_incidencia_cp(coderror, NULL, NULL, NULL);
           strcpy(pforitn->fiisocom.respusal,"909");
         if (tipomsg != 1644)
         {   //***   solo se contentan con error los distintos de 1644   **
            fgenera_deneg(pforitn);
         }
      }
	  
   }  //** fin while tratamensajes  * ------------------------------- 
   
   //DESCONECTAMOS EL HSM
   desconexhsm(sfd_hsm);  
   
} //***  fin main **

//***************************************************************************
//   FUNCION  FGENERA_ADMIN  //PRJ0258542 - Fase 3 - Cambio 11052026 - se tocan varias cosas
//   DESCRIPCION: Responde a red un 1644 si se encuentra un error de formato
//**************************************************************************
int fgenera_admin( )
{
 formato_interno foin;
 char claveP[5];
 char auxYear[5];
 int retorno, pos_sub;
 
 fprintLog(nomProg,"*** Va a generar administrativa \n"); //PRJ0258542 - Fase 3 - Cambio 11052026

  retorno = inicfori(1644,&foin);
  strcpy(foin.fiisocom.fiiso000 , "1644");

  //strcpy(foin.fiisocom.fiidtran , "999999"); //PRJ0258542 - Fase 3 - Cambio 11052026

  tm_dec tm;

  fechorsys(&tm);
  strcpy(auxYear,itoa(tm.tm_year)); 
  strncpy(foin.fiisocom.fifechor.aa, auxYear +2, 2); 
  lpad(foin.fiisocom.fifechor.aa, '0', 2);
  strcpy(foin.fiisocom.fifechor.mm, itoa(tm.tm_mon)); 
  lpad(foin.fiisocom.fifechor.mm, '0', 2);
  strcpy(foin.fiisocom.fifechor.dd, itoa(tm.tm_mday)); 
  lpad(foin.fiisocom.fifechor.dd, '0', 2);
  strcpy(foin.fiisocom.fifechor.hh, itoa(tm.tm_hour)); 
  lpad(foin.fiisocom.fifechor.hh, '0', 2);
  strcpy(foin.fiisocom.fifechor.mi, itoa(tm.tm_min));
  lpad(foin.fiisocom.fifechor.mi, '0', 2);
  strcpy(foin.fiisocom.fifechor.ss, itoa(tm.tm_sec)); 
  lpad(foin.fiisocom.fifechor.ss, '0', 2);

	strcpy(foin.fiisocom.fiidtran, foin.fiisocom.fifechor.dd); //PRJ0258542 - Fase 3 - Cambio 11052026
	strcpy(foin.fiisocom.fiidtran+2, foin.fiisocom.fifechor.hh); //PRJ0258542 - Fase 3 - Cambio 11052026
	strcpy(foin.fiisocom.fiidtran+4, foin.fiisocom.fifechor.mi); //PRJ0258542 - Fase 3 - Cambio 11052026

  strcpy(foin.fiisocom.fiiso024 , "689");
  strcpy(foin.fiisocom.respusal , "687");

//strcpy(foin.fidatiso.fiisoadm.fiadm072.longitud , "100");
//strncpy(foin.fidatiso.fiisoadm.fiadm072.datos , msglin.mensaje_linea , 100);

  strcpy(foin.fidatiso.fiisoadm.fiadm072.longitud , "103");
  strncpy(foin.fidatiso.fiisoadm.fiadm072.datos , "999", 3);
  memcpy(foin.fidatiso.fiisoadm.fiadm072.datos+3 , msglin.mensaje_linea , 100);

#if defined (TRAZA) && (TRAZA > 10)
   fprintLog(nomProg,"fiadm072 -----------------------\n");
   fhexdump(stdout,foin.fidatiso.fiisoadm.fiadm072.datos,200);
   fprintLog(nomProg,"fiadm072 -----------------------\n");
#endif


#if 0
  //*****************************************************************************
  //**  Modificado para que lea entidad origen y resolutor primario de parsub  ** 
  //***************************************************************************** 
   strncpy(claveP, subsistema_general, 2);
   sprintf(claveP+2,"%.2d" , suba_iso);
   claveP[4]='\0';
   cod_err = tratar_fich("NPTPARS",IGUAL,&reg_pars, claveP);
   strncpy(foin.fidatiso.fiisoadm.fiadm093.longitud,reg_pars.pacidesti,2);
   foin.fidatiso.fiisoadm.fiadm093.longitud[2] = '\0';
   strncpy(foin.fidatiso.fiisoadm.fiadm093.datos ,reg_pars.pacidesti + 2,11);
   foin.fidatiso.fiisoadm.fiadm093.datos[11] = '\0';

   strncpy(foin.fiisocom.fi032094.longitud,reg_pars.paciprese,2);
   foin.fiisocom.fi032094.longitud[2] = '\0';
   strncpy(foin.fiisocom.fi032094.datos ,reg_pars.paciprese + 2,11);
   foin.fiisocom.fi032094.datos[11] = '\0';
#else // de if 0 
   memset(foin.fidatiso.fiisoadm.fiadm093.longitud,' ', 2);
   memset(foin.fidatiso.fiisoadm.fiadm093.datos   ,' ', 11);
   memset(foin.fiisocom.fi032094.longitud,' ', 2);
   memset(foin.fiisocom.fi032094.datos   ,' ', 11);
   pos_sub = tsubsis;
   if (pos_sub > 0)
    {
     strncpy(foin.fidatiso.fiisoadm.fiadm093.longitud,
         (psvm+pos_sub)->str_svm.svmpars.pacidest,2);
     strncpy(foin.fidatiso.fiisoadm.fiadm093.datos,
         (psvm+pos_sub)->str_svm.svmpars.pacidest + 2,11);
     strncpy(foin.fiisocom.fi032094.longitud,
         (psvm+pos_sub)->str_svm.svmpars.paciadqui,2);
     strncpy(foin.fiisocom.fi032094.datos,
         (psvm+pos_sub)->str_svm.svmpars.paciadqui + 2,11);
    }
   foin.fidatiso.fiisoadm.fiadm093.longitud[2] = '\0';
   foin.fidatiso.fiisoadm.fiadm093.datos[11] = '\0';
   foin.fiisocom.fi032094.longitud[2] = '\0';
   foin.fiisocom.fi032094.datos[11] = '\0';
#endif //* de if 0 

  retorno = fenvia_var(&foin);
  if (retorno != 0)
  {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"error al enviar a red un 1644 de error\n");
#endif
  }

return (0);
}


//***************************************************************************
//   FUNCION   FGENERA_DENEG
//   DESCRIPCION: Deniega a red un mensaje por distintas causas
//***************************************************************************
int fgenera_deneg(formato_interno *pfi )
{
int retorno;

  retorno = fconvres(pfi);
  retorno = fenvia_var(pfi);
  if (retorno != 0)
  {
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"error al enviar a red una denegacion por motivos varios\n");
#endif
  }

  return (0);
}

#ifdef LISTANEGRAONLINE
//***************************************************************************
//   FUNCION   FGENERA_PETLN
//   DESCRIPCION: Genera una solicitud de actualizacion de lista negra
//***************************************************************************
int fgenera_petln(formato_interno * pforitn )
{
#include "npttime.h"
formato_interno foin;
tm_dec  tm;
int retorno;
char numoper[6]; 
NPSTIME regtime;
char claveP[5];
char auxYear[5];


retorno = inicfori(1644,&foin);
strcpy(foin.fiisocom.fiiso000 , "1644");
//********************************************************

fechorsys (&tm);

strcpy(auxYear,itoa(tm.tm_year));
strncpy(foin.fiisocom.fifechor.aa, auxYear +2, 2);
lpad(foin.fiisocom.fifechor.aa, '0', 2);
strcpy(foin.fiisocom.fifechor.mm, itoa(tm.tm_mon));
lpad(foin.fiisocom.fifechor.mm, '0', 2);
strcpy(foin.fiisocom.fifechor.dd, itoa(tm.tm_mday));
lpad(foin.fiisocom.fifechor.dd, '0', 2);
strcpy(foin.fiisocom.fifechor.hh, itoa(tm.tm_hour));
lpad(foin.fiisocom.fifechor.hh, '0', 2);
strcpy(foin.fiisocom.fifechor.mi, itoa(tm.tm_min));
lpad(foin.fiisocom.fifechor.mi, '0', 2);
strcpy(foin.fiisocom.fifechor.ss, itoa(tm.tm_sec));
lpad(foin.fiisocom.fifechor.ss, '0', 2);

                                                
//********************************************************
//  aqui se podria dar numero de operacion  
strcpy(foin.fiisocom.fiidtran , "999999");
retorno = 0;
if (( retorno = fnumope(numoper)) == 0)
{
	strncpy(foin.fiisocom.fiidtran, numoper,6);    
	foin.fiisocom.fiidtran[6] = '\0';           
}

strcpy(foin.fiisocom.fiiso024 , "691");
strcpy(foin.fiisocom.respusal , "600");
strcpy(foin.fidatiso.fiisoadm.fiadm072.longitud , "010");
strncpy(foin.fidatiso.fiisoadm.fiadm072.datos     , "001" , 3 );


//strncpy(foin.fidatiso.fiisoadm.fiadm072.datos + 3 , regtime.meversln , 6 );
strncpy(foin.fidatiso.fiisoadm.fiadm072.datos + 3 , "000000" , 6 );
foin.fidatiso.fiisoadm.fiadm072.datos [9] = '0';

#if defined (TRAZA) && (TRAZA > 10)
 //fprintf(stderr,"version lista negra  = %.*s\n", 6, "000000");
 //fprintf(stderr,"tama𭟭aximo lista negra  = %.*s\n", 6, "010000");
#endif


strcpy(foin.fidatiso.fiisoadm.fiadm093.longitud , "06");

strncpy(reg_pars.pasubsis, subsistema_general,2);
sprintf(reg_pars.pasubapl, "%.2d" , suba_iso);
   strncpy(claveP, reg_pars.pasubsis, 2);
   strncpy(claveP+2, reg_pars.pasubapl, 2);
    claveP[4]='\0';
 
cod_err = tratar_fich("NPTPARS",IGUAL,&reg_pars, claveP);
strncpy(foin.fidatiso.fiisoadm.fiadm093.longitud,reg_pars.pacidesti,2);
foin.fidatiso.fiisoadm.fiadm093.longitud[2] = '\0';
strncpy(foin.fidatiso.fiisoadm.fiadm093.datos ,reg_pars.pacidesti + 2,11);
foin.fidatiso.fiisoadm.fiadm093.datos[11] = '\0';

strncpy(foin.fiisocom.fi032094.longitud, pforitn->fiisocom.fi032094.longitud,2);
foin.fiisocom.fi032094.longitud[2] = '\0';
strncpy(foin.fiisocom.fi032094.datos, pforitn->fiisocom.fi032094.datos,11);
foin.fiisocom.fi032094.datos[11] = '\0';

//********************************************************************
retorno = fenvia_var(&foin);
if (retorno != 0)
{
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"error al enviar a red una pet. de actualizacion de LN\n");
#endif
}

return (0);
}

#endif                 

//***************************************************************************
//   FUNCION   FENVIA_VAR
//   DESCRIPCION: Configura y envia un mensaje en formato PRICE
//**************************************************************************
int fenvia_var(formato_interno *pfoin)
{
   int retorno;
   mensaje_nuc mensa1;

#if defined (TRAZA) && (TRAZA > 10)
 fprintLog(nomProg,"-------voy a enviar al PENVIO-----\n");
#endif
 int aux_longmensa = sizeof(mensa1.fiforitn); //PRJ0258452 - quitado = sizeof(mensa1)-sizeof(mensa1.tipo);

 if (atoi(pfoin->fidatpro.fipidori) !=0)
      mensa1.tipo = atoi(pfoin->fidatpro.fipidori);
 else
      mensa1.tipo=1111;

 memcpy(&mensa1.fiforitn,pfoin,sizeof(mensa1.fiforitn));

#if defined (TRAZA) && (TRAZA > 10)
 fprintLog(nomProg,"Envio al manejador con tipo=%i\n",qenvio);
#endif
 signal(SIGALRM,sigue);
 alarm(ESSNDCOL);
 if ((msgsnd(qenvio, &mensa1, aux_longmensa, 0))== -1)
 {
   alarm(0);
#if defined (TRAZA) && (TRAZA > 1)
    perror("mensaje>>>>>");
#endif
       //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Envio PENVIO %i codigo de error >%.3s<\n",qenvio, coderror);

   return(-1);
}
 alarm(0);
 return (0);

}  //*  fin de fenvia_var  


//***************************************************************************
//   FUNCION   FCASAR_RESPU
//   DESCRIPCION: Casa la respuesta con la cola QCASAR, recupera los datos de
//                la peticion, le coloca los datos de respuesta, y envia al 
//                rtratami
//**************************************************************************
int fcasar_respu (formato_interno *pforitn,mensaje_nuc *ppend)

{ 
  int   p_039;
  char  ch_004[4];
  NPSCASA reg_casa;
  char claveC[7];
  
  petiquetas Tetique;

  int fbusca_codfin(int ll);

  mensaje_nuc msgpend; 
  int long_msgpend = sizeof(msgpend.fiforitn); //PRJ0258452 - quitado = sizeof(msgpend) - sizeof(msgpend.tipo); 
  int retorno;


#if defined (TRAZA) && (TRAZA > 10)
  fprintLog(nomProg,"=========fcasar_respu\n");
#endif

#if defined (TRAZA) && (TRAZA > 10)
  fprintLog(nomProg,"iso011:::  >%.6s<\n", pforitn->fiisocom.fiidtran);
  fprintLog(nomProg,"fcasar:::::: long_msgpend  >%d<\n",long_msgpend);
#endif
   memset(&reg_casa, ' ', sizeof(NPSCASA));

 strncpy(reg_casa.canumope, pforitn->fiisocom.fiidtran, 6);
 strncpy(claveC, pforitn->fiisocom.fiidtran, 6);
 claveC[7]='\0';

 retorno = tratar_fich("NPTCASA", IGUAL, &reg_casa, claveC);
 if(retorno)  // el mensaje esta sin responder 
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg, "error CASAR >%d<\n", retorno);
#endif
  strcpy(reginci.innomfic, "QCASAR");
  strcpy(coderror, "009");

  //Añadido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: Tipo Leer NPTCASA codigo de error >%03d< \n",coderror);
  

  return(1);
 }

 //////////////////////// ppend   es la operaci򬟥ncontrada en NPTCASA //////////////////////
 //////////////////////// pforitn es la operaci򬟲ecibida y traducida  //////////////////////
 memcpy(&ppend->fiforitn, &reg_casa.caforitn, sizeof(formato_interno));
 ppend->tipo = atoi(ppend->fiforitn.fidatpro.fipidori);

 if(strncmp(ppend->fiforitn.fidatpro.firespri,subsistema_general,2))
 {
     fprintLog(nomProg, "Operacion de otro subsitema >%.2s<\n",
           ppend->fiforitn.fidatpro.firespri);
          //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL - TIPO DE MENSAJE NO CONTEMPLADO: operacion de otro subsistema %.2s \n", ppend->fiforitn.fidatpro.firespri);

     return(2);
 }

  if(strncmp(ppend->fiforitn.fiisocom.fifechor.aa,
          pforitn->fiisocom.fifechor.aa,2)         ||
   strncmp(ppend->fiforitn.fiisocom.fifechor.mm,
          pforitn->fiisocom.fifechor.mm,2)         ||
   strncmp(ppend->fiforitn.fiisocom.fifechor.dd,
          pforitn->fiisocom.fifechor.dd,2)         ||
   strncmp(ppend->fiforitn.fiisocom.fifechor.hh,
          pforitn->fiisocom.fifechor.hh,2)         ||
   strncmp(ppend->fiforitn.fiisocom.fifechor.mi,
          pforitn->fiisocom.fifechor.mi,2)         ||
   strncmp(ppend->fiforitn.fiisocom.fifechor.ss,
          pforitn->fiisocom.fifechor.ss,2))
{
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"------------->el mensaje no casa fecha/hora \n");
  fprintLog(nomProg,"ppend= %.2s%.2s%.2s%.2s%.2s%.2s\n",
                ppend->fiforitn.fiisocom.fifechor.aa,
                ppend->fiforitn.fiisocom.fifechor.mm,
                ppend->fiforitn.fiisocom.fifechor.dd,
                ppend->fiforitn.fiisocom.fifechor.hh,
                ppend->fiforitn.fiisocom.fifechor.mi,
                ppend->fiforitn.fiisocom.fifechor.ss);
  fprintLog(nomProg,"pfori= %.2s%.2s%.2s%.2s%.2s%.2s\n",
                pforitn->fiisocom.fifechor.aa,
                pforitn->fiisocom.fifechor.mm,
                pforitn->fiisocom.fifechor.dd,
                pforitn->fiisocom.fifechor.hh,
                pforitn->fiisocom.fifechor.mi,
                pforitn->fiisocom.fifechor.ss);
  fprintLog(nomProg,"\n");

#endif
  return(2);
 }

 retorno = 0;
 retorno = tratar_fich("NPTCASA", BORRAR, NULL, claveC);

 if(retorno)
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stdout,"error BORRAR CASAR >%d<\n", retorno);
#endif
  strcpy(reginci.innomfic, "QCASAR");
  strcpy(coderror, "009");


  //Añadido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: Tipo Borrar NPTCASA codigo de error >%03d< \n",coderror);
 
  return(1);
 }


  ////////// informar los datos que aporta la respuesta ///////////////
  
  strncpy(ppend->fiforitn.fiisocom.fiiso000 , pforitn->fiisocom.fiiso000,4);
  ppend->fiforitn.fiisocom.fiiso000[4]='\0';

  strncpy(cadena_verif , pforitn->fiisocom.fiiso000, 4);
  cadena_verif[4]='\0';

  strncpy(ppend->fiforitn.fiisocom.respusal , pforitn->fiisocom.respusal,3);
  ppend->fiforitn.fiisocom.respusal[3]='\0';
  
  p_039 = atoi(ppend->fiforitn.fiisocom.respusal);



 switch(p_039)
 {
  case 0:
  case 60:
  case 300:
  case 400:
  case 500:
  case 560:
  case 600:
  case 660:
  case 800:
  case 900:
  case 901:
   strcpy(ppend->fiforitn.fidatpro.ficodfin , "000");
   break;
  default:
//***   funcion que convierte el codigo red en codfin   **
   retorno = fbusca_codfin(p_039);
   if (retorno < 0)
   {
      //***   codigo de respuesta no tratado   **
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"Codigo de respuesta no tratado p_039 >%03d<\n", p_039);
#endif
//*** Cuando el primer digito del P39 es cero la respuesta es autorizada **
    if(p_039 < 100)
     strcpy(ppend->fiforitn.fidatpro.ficodfin , "000");
    else
     strcpy(ppend->fiforitn.fidatpro.ficodfin , "190"); //PRJ0258542 - Se cambia 261 por 190
   }
   else
   {
    sprintf(ch_004, "%d", retorno);
    lpad(ch_004 , '0' , 3);
    strcpy(ppend->fiforitn.fidatpro.ficodfin , ch_004);
   }
   break;
 }

//DMND0036176 añadidas respuestas preautorizaciones
 switch(atoi(cadena_verif))
 {
  case 1110:
  case 1210:
  case 1230:
    if(pforitn->fiisocom.respusal[0] != '0' &&
       pforitn->fiisocom.respusal[0] != '1' &&
       pforitn->fiisocom.respusal[0] != '2' &&
       pforitn->fiisocom.respusal[0] != '9')
          return(11);

     if(!strcmp(cadena_verif, "1230"))
        break;

    strncpy(ppend->fiforitn.fidatiso.fiisoaut.fiaut038,
       pforitn->fidatiso.fiisoaut.fiaut038, 6);
    strncpy(ppend->fiforitn.fidatiso.fiisoaut.fiaut058.longitud,
       pforitn->fidatiso.fiisoaut.fiaut058.longitud, 2);
    strncpy(ppend->fiforitn.fidatiso.fiisoaut.fiaut058.datos,
       pforitn->fidatiso.fiisoaut.fiaut058.datos, 11);

   retorno = strcmp(ppend->fiforitn.fidatiso.fiisoaut.fiaut003.dig12,"31");
   if(!retorno)
   {
    strncpy (ppend->fiforitn.fidatiso.fiisoaut.fiaut048.movim.sub04806.movimi[1],
         pforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.movimi[1],138);
   }
   else
   {
    if(ppend->fiforitn.fidatiso.fiisoaut.fiaut003.dig12[0] == '9')
    {
     strncpy((char *)&ppend->fiforitn.fidatiso.fiisoaut.fiaut048.augen.saldos,
            (char *)&pforitn->fidatiso.fiisoaut.fiaut048.augen.saldos,
            sizeof(iso48saldos));
    }
    else
    {
///*** 
//eliminados un parrafo comentado
    }
   }
   break;
  case 1314:
   if(pforitn->fiisocom.respusal[0] != '3' &&
      pforitn->fiisocom.respusal[0] != '9')
    return(11);
   strncpy(ppend->fiforitn.fidatiso.fiisofic.fific072.longitud,
       pforitn->fidatiso.fiisofic.fific072.longitud, 3);
   strncpy(ppend->fiforitn.fidatiso.fiisofic.fific072.datos,
       pforitn->fidatiso.fiisofic.fific072.datos, 256);
   break;
  case 1430:
   if(pforitn->fiisocom.respusal[0] != '4' &&
      pforitn->fiisocom.respusal[0] != '9')
    return(11);
   break;
  case 1814:
   
   if(pforitn->fiisocom.respusal[0] != '8')
    return(11);

   strncpy(ppend->fiforitn.fidatiso.fiisoctr.fictr028.aa,pforitn->fidatiso.fiisoctr.fictr028.aa, 2);
   strncpy(ppend->fiforitn.fidatiso.fiisoctr.fictr028.mm,pforitn->fidatiso.fiisoctr.fictr028.mm, 2);
   strncpy(ppend->fiforitn.fidatiso.fiisoctr.fictr028.dd,pforitn->fidatiso.fiisoctr.fictr028.dd, 2);
   strncpy(ppend->fiforitn.fidatiso.fiisoctr.fictr029,pforitn->fidatiso.fiisoctr.fictr029, 2);
  	//PRJ0258542 - INI
  	if (atoi(pforitn->fidatiso.fiisocla.ficla033.longitud) > 0)
  	{
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla011,pforitn->fidatiso.fiisocla.ficla011, 6);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla012,pforitn->fidatiso.fiisocla.ficla012, 12);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla024,pforitn->fidatiso.fiisocla.ficla024, 3);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla025,pforitn->fidatiso.fiisocla.ficla025, 4);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla033.longitud,pforitn->fidatiso.fiisocla.ficla033.longitud, 3);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla033.datos,pforitn->fidatiso.fiisocla.ficla033.datos, 11);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla039,pforitn->fidatiso.fiisocla.ficla039, 3);
	  	if (strncmp(pforitn->fidatiso.fiisocla.ficla053.control, "  ", 2) != 0)
	  	{
		  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla053.control,pforitn->fidatiso.fiisocla.ficla053.control, 2);
		  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla053.identificador,pforitn->fidatiso.fiisocla.ficla053.identificador, 8);
		  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla053.algoritmo,pforitn->fidatiso.fiisocla.ficla053.algoritmo, 2);
		  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla053.longitud,pforitn->fidatiso.fiisocla.ficla053.longitud, 4);
		  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla053.proteccion,pforitn->fidatiso.fiisocla.ficla053.proteccion, 2);
		  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla053.resto,pforitn->fidatiso.fiisocla.ficla053.resto, 6);
		  }
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla093.longitud,pforitn->fidatiso.fiisocla.ficla093.longitud, 3);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla093.datos,pforitn->fidatiso.fiisocla.ficla093.datos, 11);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla094.longitud,pforitn->fidatiso.fiisocla.ficla094.longitud, 3);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla094.datos,pforitn->fidatiso.fiisocla.ficla094.datos, 11);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla096.longitud,pforitn->fidatiso.fiisocla.ficla096.longitud, 3);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficla096.datos,pforitn->fidatiso.fiisocla.ficla096.datos, 360);
	  	strncpy(ppend->fiforitn.fidatiso.fiisocla.ficlamac,pforitn->fidatiso.fiisocla.ficlamac, 8);
	  	fprintLog(nomProg,"RESTO CAMPOS DEL MENSAJE >%.4s< \n", ppend->fiforitn.fiisocom.fiiso000);
	  	fprintLog(nomProg,"  Valor del P11          >%.6s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla011);
	  	fprintLog(nomProg,"  Valor del P12          >%.12s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla012);
	  	fprintLog(nomProg,"  Valor del P24          >%.3s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla024);
	  	fprintLog(nomProg,"  Valor del P25          >%.4s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla025);
	  	fprintLog(nomProg,"  Valor del P33          >%.2s< >%.11s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla033.longitud, ppend->fiforitn.fidatiso.fiisocla.ficla033.datos);
	  	fprintLog(nomProg,"  Valor del P39          >%.3s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla039);
	  	fprintLog(nomProg,"  Valor del P53          >%.2s%.8s%.2s%.4s%.2s%.6s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla053.control,ppend->fiforitn.fidatiso.fiisocla.ficla053.identificador,ppend->fiforitn.fidatiso.fiisocla.ficla053.algoritmo,
	  																																							ppend->fiforitn.fidatiso.fiisocla.ficla053.longitud,ppend->fiforitn.fidatiso.fiisocla.ficla053.proteccion,ppend->fiforitn.fidatiso.fiisocla.ficla053.resto);
	  	fprintLog(nomProg,"  Valor del S93          >%.2s< >%.11s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla093.longitud, ppend->fiforitn.fidatiso.fiisocla.ficla093.datos);
	  	fprintLog(nomProg,"  Valor del S94          >%.2s< >%.11s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla094.longitud, ppend->fiforitn.fidatiso.fiisocla.ficla094.datos);
	  	fprintLog(nomProg,"  Valor del S96          >%.3s< >%.360s< \n", ppend->fiforitn.fidatiso.fiisocla.ficla096.longitud, ppend->fiforitn.fidatiso.fiisocla.ficla096.datos);
	  	fprintLog(nomProg,"  Valor del S128         >%.8s< \n", ppend->fiforitn.fidatiso.fiisocla.ficlamac);
	  }
		//PRJ0258542 - FIN
   break;
  case 1530:
   if(pforitn->fiisocom.respusal[0] != '5' &&
     pforitn->fiisocom.respusal[0] != '9')
    return(11);
  case 1614:
   if(pforitn->fiisocom.respusal[0] != '6' &&
     !strcmp(cadena_verif, "1614"))
    return(11);

   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot074,
       pforitn->fidatiso.fiisotot.fitot074, 10);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot075,
       pforitn->fidatiso.fiisotot.fitot075, 10);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot076,
       pforitn->fidatiso.fiisotot.fitot076, 10);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot077,
       pforitn->fidatiso.fiisotot.fitot077, 10);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot086,
       pforitn->fidatiso.fiisotot.fitot086, 16);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot087,
       pforitn->fidatiso.fiisotot.fitot087, 16);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot088,
       pforitn->fidatiso.fiisotot.fitot088, 16);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot089,
       pforitn->fidatiso.fiisotot.fitot089, 16);
   strncpy(ppend->fiforitn.fidatiso.fiisotot.fitot097,
       pforitn->fidatiso.fiisotot.fitot097, 17);
   break;
 }  

//** Datos respuestA AL CHIP   **
 if(ppend->fiforitn.fidatiso.fiisoaut.fiaut022[6] == '5' ||
    ppend->fiforitn.fidatiso.fiisoaut.fiaut022[6] == 'M')
 {
  memcpy((char*)&ppend->fiforitn.fidatiso.fiisoaut.fiaut055,
          (char*)&pforitn->fidatiso.fiisoaut.fiaut055,
          sizeof(ppend->fiforitn.fidatiso.fiisoaut.fiaut055));
 }

 strncpy(ppend->fiforitn.fidatpro.ficlatab + 6 , "00" , 2);
 strncpy(ppend->fiforitn.fidatpro.ficlatab + 8 , "50" , 2);
 strncpy(ppend->fiforitn.fidatpro.firesfin , subsistema_general, 2);
 sprintf(ppend->fiforitn.fidatpro.fisubres, "%02d", suba_iso);
 
 return(0);
}//////////////////////// TERMINA FCASAR_RESPU



#include "fenvia_rtratami.c"

//***************************************************************************
//   FUNCION   FBUSCA_CODFIN
//   DESCRIPCION: Traduce el p-39 RED al codfin
//**************************************************************************
int fbusca_codfin (int p_039)
{
#include "tb039fin.h"
  int ll;

 for (ll=0 ;datos_039fin[ll].tab_039 > 0 ; ll++ )
 {
  if(datos_039fin[ll].tab_039 == p_039)
   return(datos_039fin[ll].tab_fin);
 }

 return(-1);

}


#include "ftradred.c"
#include "fclasmsg.c"
//#include "fcifraPan.c"

