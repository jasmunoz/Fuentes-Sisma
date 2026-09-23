//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
//PRJ288711 - Tratamiento diferidos erróneos
/***
   PROGRAMA: RTRATAMI.C
***/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/errno.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>

#include "cforitno.h"
#include "nptinci.h"
#include "nptlogo.h"
#include "deffich.h"
#include "datagen.h"
#include "npttbop.h"
#include "nptpars.h"
#include "nptbine.h"
#include "npttarj.h"
#include "npttoto.h"
#include "fclasmsg.h"
#include "msg_nucl.h"
#include "nptdhsm.h"
#include "ftimer.h"
#include "ftradred.h"
#include "svm.h"
#include "colas.h"
#include "procesos.h"
#include "netwin.h"
#include "cnetplus.h"
#include "nptdife.h"
#include "npterrd.h"  //PRJ288711 - Tratamiento diferidos erróneos
#include "arranque.h"
#include "llaves1.h"
#include "nptpara.h"

mensaje_nuc Mensa;

NPSINCI reginci;
NPSLOGO reglogo;
NPSIPCS  *reg_ipcs;
NPSCONF  *reg_conf;
char nombre[5];

typedef struct sfdssl {         /* estructura conjunta para socket y socket seguro */
 int sfd_ssl;                   
 SSL * ssl_ssl;                               
} sfdssl;

 struct sfdssl stsocketssl;
 struct sfdssl * sockssl;

char coderror[4];
//punteros_autoriza paut;
//char error[3]; /**varible para incidencia**/
int   sfd_hsm;
typedef struct clave_log
{
 char fecha0[8];
 char hora00[6];
 char numope[6];
 char final[1];
} clave_log;

typedef struct clave_lgcj
{
 char ncross[10];
 char fecha0[8];
 char hora00[6];
 char final[1];
} clave_lgcj;


typedef struct dbine tbine;
struct dbine
{
 // PRJ0202598 cambio bin de 6 posiciones 8 afecta al tamaño total de la estrutura dbine
 char   t_biclave0[8];     /*  Bin                    (clave)  */
 char   t_birnginf[11];    /*  Rango inferior         (clave)  */
 char   t_birngsup[11];    /*  Rango superior                             */
 char   t_bicodent[4];     /*  Entidad                                    */
 char   t_bitboper[2];     /*  Tabla de operaciones                       */
 char   t_birespri[2];     /*  Resolutor primario                         */
 char   t_biparau1[1];     /*  Parametro de autorizacion 1                */
 char   t_biparau2[1];     /*  Parametro de autorizacion 2                */

 tbine      *bin_siguiente;
};
tbine  *pbi_binprimero;

typedef struct dtbop ttbop;
struct dtbop
{
 char     clave [18];
 char     funcion[20];
 ttbop      *siguiente;
};
ttbop *ptb_primero;

// PRJ0202598 cambio bin de 6 posiciones 8
char bin [8];
char rango[16];

int shmidtibi;
int qctm,qmoni,qoffc,qoffc17;
int qdi22,qdi19,qdi21;
int q11,q15,q16,q17,q18,q19,q21,q22;
SVM *psvm;

// PRJ0202598 cambio bin de 6 posiciones 8 afecta al tamaño total de la estructura tbbine
tbbine *psvmbine;
char nomProg[50];

void fhexdumpLog(void*, long);
int nzeroes (char *c, int n);
int ncampo_numerico (char *c,int n);
int ftratami(formato_interno * pfi);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int finalizar(int codigo);
void conexhsm_C(char* codent, char *comando, struct sfdssl * sockssl, char * codResp);
int fllama(formato_interno * pforitn, char *funciones);
int fbusca_funciones(char *pque, ttbop *ptabla, char *pobjeto);
// PRJ0202598 cambio bin de 6 posiciones 8 afecta al tamaño total de la estructura tbine 
int fbusca_bin(tbine *pbine, char *pbinope, char *prangoope, int plongrango,
                 char *ptboper, char *pparau1, char *pparau2,char *prespri, char *codent);
int fgenera_diferido_exce(formato_interno *pfi);
int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
void fprintLog(char ficheroPlantilla[50], char* format, ...);


/*****************************************************************
***  FUNCIONES DE TABLA  DE OPERACIONES
*** prototipos de funciones de autoriza validando  ***
******************************************************************/
int fcaldisp(int periodo, fecha *pfecha_operacion,
               fecha *pfechai_inicio);


/*** prototipos de funciones locales a fcaldisp  ***/
 int finicializar_periodo(int periodo, fecha *fecha_operacion,
                         fecha *pfecha_inicio);

 int fdiaria(int periodo, fecha *fecha_operacion, fecha *pfecha_inicio);
 int fsemanal(int periodo, fecha *fecha_operacion, fecha *pfecha_inicio); 
 int funoyquince(fecha *fecha_operacion, fecha *pfecha_inicio);
 int fmeses(int periodo, fecha *fecha_operacion, fecha *pfecha_inicio);
 int fdiferencia_en_dias(fecha *fecha_operacion, fecha *fecha_inicio);
 int fcompara_fechas(fecha *fecha1, fecha *fecha2);
 void amplia_q(int cola,int tamano);
 void fponemeses(int, fecha *, int);
 /******  fin funciones de fcaldisp    ******/

/***  prototipo de funcion busca previa ***/
static int funcionpr(formato_interno *pforitn);

/***  prototipo de funcion para tratar mensajes de funcion transparente sin tarjeta***/
static int funciondp(formato_interno *pforitn);

/***  prototipo de funcion cargar totales para el cierre SIO***/
static int funcionto(formato_interno *pforitn);

/***  prototipo de funcion cargar numero y fecha de sesion en fiaut28 y fiaut29***/
static int funcionss(formato_interno *pforitn);

/***  prototipo de funcion cambio de pin ***/
static int funcioncp(formato_interno *pforitn);

/***  prototipo de funcion validar el cajero ***/
static int funcioncj(formato_interno *pforitn);

/***  prototipo de funcion envia diferido a resolutor ***/
static int funcionrd(formato_interno *pforitn);

/***  prototipo de funcion trata mensajes varios origen puc ***/
static int funcionms(formato_interno *pforitn);

/***  prototipo de funcion control HOST   ***/
static int funcionch(formato_interno *pforitn);

/*** prototipo de funcion de control HSM  ***/
int funcionvh(formato_interno *pforitn);

/***  prototipo de funcion tratamiento cierre enviado a la red ***/
static int funcionac(formato_interno *pforitn);

/***  prototipo de funciones de forma correctora ***/
int funcionco(formato_interno *pforitn);

/**tratamiento de mensajes de cierre para el host**/
int funcionth(formato_interno *pforitn);

/**tratamiento de mensajes de notificacion, solo graba en el log**/
int funcionlg(formato_interno *pforitn);


static int funciondi(formato_interno *pforitn);

//int fgraba_diferidos(formato_interno *pforitn);
int fgenera_correctora(formato_interno *pforitn);
int forma_correctora(formato_interno *pforitn);
int funcioneo(formato_interno *pforitn);
int funcioner(formato_interno *pforitn);
int funcionrp(formato_interno *pforitn);
int funcioned(formato_interno *pforitn);
int fgradife(formato_interno *pforitn);
int funciongd(formato_interno *pforitn, char* opcion); //PRJ288711 - Tratamiento diferidos erróneos
//int fgradifecj(formato_interno * pforitn); // graba diferidos de mensajes incidencias cajeros

long milelong();
long centlong();
int fnumope(char *numope, char * numcross, char numtabla);

int festado_conexion(int  opcion,char * canal,char * estado, char opcenvoff);
 
int     busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int     damecola(SVM *psvm_par,char *proce,char *subsis);
int     ConecSVMaux(void *gen_psvm,int shmid);
/*** fin prototipos funciones   ***/

#include "tbtratam.h"

unsigned int long_foritn = sizeof(formato_interno);
unsigned int long_msgnuc = sizeof(mensaje_nuc);

//  prototipos de funciones utilizadas
void    sigue();
void fgenera_rechazo(formato_interno *pforitn); 
int campo_numerico (char * campo);

// PRJ0202598 cambio bin de 6 posiciones 8 modificado en fcargabine.c y en carga_bines.c
int fcarga_bines();
int fcarga_tbop();
int rtrata_svm;

int fverfi(formato_interno fi);
int conexhsm(char* codent, NPSDHSM reg_hsm);
void desconexhsm(int sock);

int conexion_BBDD();


long tisare, tiempoL;
int num_origen;

#ifdef TIEMPOS 
FILE * pf;
#endif
#define INTENTOS 10

//RAQUEL añadimos el registro de NPTPARA externo para usar en tratami
NPSPARA  *reg_para;

char ficlatab[12]; //PRJ0258542 - Fase 3 - Cambio 11052026

int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
  int qrtratami;
  int retorno,queda;
  mensaje_nuc  Mensa;
  int long_mensaje = sizeof(Mensa.fiforitn); //PRJ0258452 - quitado = sizeof(Mensa) - sizeof(Mensa.tipo);
  NPSDHSM registro;
  char codResp[3];
  sockssl = &stsocketssl;
  int ll; 
  char origen_mensaje[3];
  char clavecone[4];
  
  fflush(stdout);
  // incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

#if defined (TRAZA) && (TRAZA > 1)   
  fprintf(stdout, "***** ****** Inicio rtratami ****** ******\n");
  fflush(stdout);
#endif  

//asignar el nombre

   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);


   signal(SIGCLD,SIG_IGN); //cuando recibe la senal SIGCLD hace la accion SIG_IGN
//*** fin para los hijos que crean las funciones del tratami 
   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);

    //* --------- se ata a la memoria compartida SVM  -------------
		  
    retorno=conecSVM((void *)&psvm);//ORIGINAL
	
    if (retorno != 0)
    {
/*#if defined (TRAZA) && (TRAZA > 1)	
       fprintLog(nomProg,"ERROR(%d): Imposible conectar a la S.V.M.\n",errno);
#endif	*/   
       strcpy(reginci.innomfic,"SVM");
       strcpy(coderror, "011");
       exit(finalizar(-1)); //sin svm supervis no puede levantar
    }

   rtrata_svm = busca_proc_svm(psvm,argv[0],-1);

   if (rtrata_svm < 0 )
   {
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg, "error busca_proc_svm\n");
#endif
        strcpy(reginci.innomfic,"SVM");
        strcpy(coderror, "250");
        exit(finalizar(-1)); //sin svm supervis no puede levantar
   }

 
    //* crea cola de mensajes de RTRATAMI 
   qrtratami = (psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
   fprintLog(nomProg, "Creo cola de tratami  qrtratami %d \n",qrtratami);

   if (qrtratami == -1)
   {
      (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg, "error crear cola qrtratami\n");
#endif
      strcpy(reginci.innomfic,"QTRATA");
      strcpy(coderror, "009");
      //Añadido para Monitorizacion SISMA 
      fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion QTRATA codigo de error >%.3s<\n", coderror);

      exit(finalizar(-1));  //sin svm supervis no puede levantar
    }

   if (IdSvmBine > -1)
      retorno=ConecSVMaux(&psvmbine,IdSvmBine);

   if (retorno != 0)
   {
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
#if defined (TRAZA) && (TRAZA > 1)
       fprintLog(nomProg, "error conseguir svmbine \n");
#endif
        strcpy(reginci.innomfic,"SVMbine");
        strcpy(coderror, "011");
        exit(finalizar(-1)); //sin svm supervis no puede levantar
   }

  //****** SE DUERME EN ESPERA DE QUE EL PROCESO DE ARRANQUE
  //****** LE MANDE UNA SENIAL O LE SALTE EL DESPERTADOR

   signal(SIGALRM,sigue); //cuando expire el tiempo hace la funci򬟳igue.
   alarm(TIMEWAIT*2);
  //* graba su pid en la SVM 

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
#if defined (TRAZA) && (TRAZA > 1)
    fprintLog(nomProg, "queda de alarm %d \n",queda);  
#endif
    if (queda==0)
    {
#if defined (TRAZA) && (TRAZA > 1)	
       fprintLog(nomProg,"ERROR ARRANQUE queda >%d<\n", queda);
#endif	   
       (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
       strcpy(reginci.innomfic,"      ");
       strcpy(coderror, "251");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - MUERTE DE PROCESO: codigo de error >%.3s<\n",coderror);

       exit(finalizar(-1)); //sin svm supervis no puede levantar
    }

    (psvm+rtrata_svm)->estado=TRABAJO;
	
	////////// ABRE LA CONEXION A LA BASE DE DATOS Y PERMANECE ABIERTA, SE CIERRA EN FINALIZAR
	retorno = conexion_BBDD();
	if(retorno)
	{
	   fprintLog(nomProg,"Error en el acceso a la base de datos\n");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: apertura BBDD codigo de error >%i<\n",retorno);

	   exit(finalizar(-2));
	}
	
   //*  carga tabla de operaciones 
   if ((retorno = fcarga_tbop()) != 0)
   {
#if defined (TRAZA) && (TRAZA > 1)   
      fprintLog(nomProg,"error carga tabla op \n");
#endif	  
      exit(finalizar(-2));
   }
  // PRJ0202598 cambio bin de 6 posiciones 8 modificada carga de bines en fcargabine.c y en carga_bines.c
  //* carga tabla de bines    
   if ((retorno = fcarga_bines()) != 0)
   {
#if defined (TRAZA) && (TRAZA > 1)   
      fprintLog(nomProg,"error carga tbla bines \n");
#endif	  
      exit(finalizar(-2));
   }

#ifdef TIEMPOS
 pf = fopen(HOMEDIRTRAZA"tiemposr.dat", "a");
 if(!pf)
 {
  fprintLog(nomProg,"error al abrir %s%stiemposr.dat\n", HOME,DIREXECS);
  pf = fichsal;
 }
#endif

   //***** NOS CONECTAMOS AL HSM, DEJANDO EL SOCKET ABIERTO ****
     memset(&registro, ' ', sizeof(NPSDHSM));

   //RAQUEL modificado para utilizar los datos de bbdd por entidad
   //  conexhsm_C("0001","KMCA.HANDLE.00000014                              " , sockssl, codResp);

     conexhsm_C("0001",reg_para[0].psclvhsm, sockssl, codResp);
     if (!strcmp(codResp, "01"))
     {
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - ACCESO HSM: conexion HSM codigo de error >%s<\n", codResp);
	     //Bizum - mando mensaje test para tener activo el HSM
	     
	     fprintLog(nomProg,"Antes de realizar la llamada a HSM para Test \n");
	     conexhsm_C("0001", "01", sockssl, codResp);
	     fprintLog(nomProg,"Despues de realizar la llamada a HSM para Test, retorno >%s<\n", codResp);
	     
	     //Bizum - mando mensaje test para tener activo el HSM - FIN

     }    

     sfd_hsm=sockssl->sfd_ssl;
     
   //****** BUCLE DE TRATAMIENTO DE MENSAJES ****
   while ( 1 )
   {
   //* lee con espera de la cola de mensajes 
   #if defined (TRAZA) && (TRAZA > 5)
     fprintLog(nomProg, "////////////////lee con espera de cola=%i///////////////long=%i \n", qrtratami,long_mensaje);
   #endif
 
     retorno = msgrcv(qrtratami, &Mensa, long_mensaje,0,0);
		 fprintLog(nomProg,"---- -- -- -- -- -- -- -- -- -- ------------------------------------------------------------------------\n");
     fprintLog(nomProg,"--- --- errno lectura de cola >%d< cola qrtratami >%d< retorno >%i<\n", errno,qrtratami,retorno); 
	   fprintLog(nomProg,"-- ** -- Mensa -->%s< \n", Mensa.fiforitn.fidatpro.fiorigen);
	   fprintLog(nomProg,"Longitud recibida long_mensaje >%d< msngnuc >%.6s< cola de recepcion >%i<\n", long_mensaje, (char*) &Mensa.fiforitn, qrtratami); //PRJ0258542 - Fase 3 - Cambio 11052026
     if(retorno == -1)
     {
   #if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"Error en lectura de cola: >%d<\n", retorno);
        perror("msgrcv:");
   #endif
        desconexhsm(sfd_hsm); //cada vez que salimos desconectamos el HSM
        strcpy(coderror, "242");
        strcpy(reginci.innomfic,"QTRATA");
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion QTRATA codigo de error >%.3s<\n",coderror);

        exit(finalizar(-1)); //error en cola svm, supervis no puede arrancarlo. Hay que parar y arrancar todos.
     }
    fprintLog(nomProg,"--- Recibido mensaje a tratar --- %d de longitud y de tipo %d\n", retorno, Mensa.tipo);  
	 
	 //fhexdump(stderr, &Mensa , retorno);
	 
     if (Mensa.tipo == TIPO_PARADA)  //orden de cierre 
     {
   #if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg, "orden de cierre \n");
   #endif
        desconexhsm(sfd_hsm); //cada vez que salimos desconectamos el HSM
        strcpy(coderror, "999");
        if ((psvm+rtrata_svm)->estado==RECARGAR)
        {
          //Añadido para Monitorizacion SISMA 
          fprintLog(nomProg,"TIPO SISTEMA - REARRANQUE DE PROCESO: codigo de coderror >%i<\n",coderror);

           exit(finalizar(-2)); // Paro para rearranque 
        }
        else
        {
           exit(finalizar(0)); // Orden de Cierrre 
        }
     }
	 
	 	//BIMSF-4588 - Problemas colas de mensajes - BIMSF-4997 - Anado del 01 al 05 porque se cierra con los mensajes de test
		if (strncmp(Mensa.fiforitn.fidatpro.fiorigen, "10", 2) == 0 || strncmp(Mensa.fiforitn.fidatpro.fiorigen, "20", 2) == 0 
				|| strncmp(Mensa.fiforitn.fidatpro.fiorigen, "30", 2) == 0 || strncmp(Mensa.fiforitn.fidatpro.fiorigen, "40", 2) == 0
				|| strncmp(Mensa.fiforitn.fidatpro.fiorigen, "50", 2) == 0 || strncmp(Mensa.fiforitn.fidatpro.fiorigen, "01", 2) == 0
				|| strncmp(Mensa.fiforitn.fidatpro.fiorigen, "02", 2) == 0 || strncmp(Mensa.fiforitn.fidatpro.fiorigen, "03", 2) == 0
				|| strncmp(Mensa.fiforitn.fidatpro.fiorigen, "04", 2) == 0 || strncmp(Mensa.fiforitn.fidatpro.fiorigen, "05", 2) == 0)
		{
    	fprintLog(nomProg,"Origen mensaje >%.2s< (20260218)\n", Mensa.fiforitn.fidatpro.fiorigen);		
		}
		else
		{
			fprintLog(nomProg, "Mensaje vacio, no trae contenido, cierra. Tipo origen %.2s \n", Mensa.fiforitn.fidatpro.fiorigen);
			exit(finalizar(0));
		}
		//BIMSF-4588 - Problemas colas de mensajes
     
   #ifdef TIEMPOS
	 tiempoL = milelong();
     fprintLog(nomProg,"milelong ->%d<-\n", tiempoL);
   
     if(!strncmp(Mensa.fiforitn.fiisocom.fiiso000, "1100", 4) ||
        !strncmp(Mensa.fiforitn.fiisocom.fiiso000, "0100", 4))
     {
           Mensa.fiforitn.fidatpro.fitienre[8]='\0';
           fprintf(pf, "Entra en tratami %05lu\n", tiempoL - atol(Mensa.fiforitn.fidatpro.fitienre));
           fflush(pf);
     }
   #endif


     	fprintLog(nomProg,
      	"numope>%.*s< %.*s:%.*s dig12>%.*s-%.*s< codfin>%.*s< ori>%.*s< respri>%.*s< resfin>%.*s<\n",
             6,Mensa.fiforitn.fidatpro.finumope,
             2,Mensa.fiforitn.fiisocom.fifechor.hh,
             2,Mensa.fiforitn.fiisocom.fifechor.mi,
             2,Mensa.fiforitn.fidatiso.fiisoaut.fiaut003.dig12,
             4,Mensa.fiforitn.fiisocom.fiiso000,
             3,Mensa.fiforitn.fidatpro.ficodfin,
             2,Mensa.fiforitn.fidatpro.fiorigen,
             2,Mensa.fiforitn.fidatpro.firespri,
             2,Mensa.fiforitn.fidatpro.firesfin);

     fprintLog(nomProg,"Antes de llamar a ftratami\n"); //PRJ0258542
     if (strncmp(Mensa.fiforitn.fiisocom.fiiso000, "1804", 4) == 0 && strncmp(Mensa.fiforitn.fiisocom.fi032094.datos, "1805", 4) == 0 && (strncmp(Mensa.fiforitn.fiisocom.fiiso024, "811", 3) == 0 || strncmp(Mensa.fiforitn.fiisocom.fiiso024, "816", 3) == 0)) //PRJ0258542 - Fase 3 - Cambio 11052026
     {
     	strncpy(Mensa.fiforitn.fiisocom.fiiso000, "1805", 4);
     	strncpy(Mensa.fiforitn.fiisocom.fi032094.datos, "    ", 4);
			strncpy(ficlatab, Mensa.fiforitn.fidatpro.ficlatab, 12); //PRJ0258542 - Fase 3 - Cambio 11052026
			strncpy(Mensa.fiforitn.fidatpro.ficlatab + 8, "00", 2); //PRJ0258542 - Fase 3 - Cambio 11052026
			strncpy(Mensa.fiforitn.fidatpro.ficlatab + 10, Mensa.fiforitn.fidatpro.ficlatab + 10, 2); //PRJ0258542 - Fase 3 - Cambio 11052026
			fprintLog(nomProg,"Convierte a 1805 y resto clave fin>%.12s<\n", Mensa.fiforitn.fidatpro.ficlatab); //PRJ0258542 - Fase 3 - Cambio 11052026
     }
     retorno=ftratami(&Mensa.fiforitn);
     fprintLog(nomProg,"Despues de llamar a ftratami\n"); //PRJ0258542

   }    
}  //  fin del main



///////////////////////////////////////////////////////////////////////////////
//  FUNCTION: ftratami
//////////////////////////////////////////////////////////////////////////////
int ftratami(formato_interno * pfi)
{

  int retorno;
  int tabla;
  char w_tipomsg[5];

  char   clave_ope[3];
  char   parautor1[2];
  char   parautor2[2];
  char   resolutor[3];
  char   entidad[4];
  char   clave[19];
  char   funciones[20];
  int    longrango;

	fprintLog(nomProg,"Entra en la funcion ftratami\n"); //PRJ0258542 
	fprintLog(nomProg,"Mensaje de entrada %.4s\n",pfi->fiisocom.fiiso000); //PRJ0258542
 	strncpy(w_tipomsg,pfi->fiisocom.fiiso000,4);
 	if (strncmp(w_tipomsg, "    ", 4) == 0)
 	{
 		fprintLog(nomProg,"Mensaje de entrada vacio, devolver error\n");
 		return (-1);
 	}
 strcpy(coderror, "   ");
 w_tipomsg[4] = '\0';
 clave_ope[2] = '\0';
 parautor1[1] = '\0';
 parautor2[1] = '\0';
 resolutor[2] = '\0';
 //fprintLog(nomProg," Punto de control 1\n"); //PRJ0258542
 //// PRJ0202598 cambio bin de 6 posiciones 8 
 memset(bin, ' ', sizeof(bin));
 memset(rango, ' ', sizeof(rango));
 
	fprintLog(nomProg," Inicio valor pfi->fidatpro.ficlatab >%.12s<\n", pfi->fidatpro.ficlatab); //PRJ0258542 - Fase 3 - Cambio 11052026
  // OPERACIONES SIN TARJETA: ADMINISTRATIVAS, DE CONCILIACION Y CONTROL DE DIALOGO
  if (!strncmp(w_tipomsg, CONCILIA,4)  ||  //1524
       !strncmp(w_tipomsg, REPE_CONC,4) || //1525	   
       !strncmp(w_tipomsg, RESP_CONC,4) || //1534
       !strncmp(w_tipomsg, CONC_OFF,4)  || //1544
       !strncmp(w_tipomsg, ADMINISTR,4) || //1644
	   	 !strncmp(w_tipomsg, "1824",4) || !strncmp(w_tipomsg, "1825",4) || !strncmp(w_tipomsg, "1805",4) || //PRJ0258542 - se añade 1825 y 1805
	     !strncmp(w_tipomsg, "1834",4) || //1834 
       !strncmp(w_tipomsg, PETI_DIAL,4) || //1804 
       !strncmp(w_tipomsg, RESP_DIAL,4) )  //1814
  {
  	//fprintLog(nomProg," Punto de control 3\n"); //PRJ0258542

    //inicializamos bin a ceros con memset
    // PRJ0202598 cambio bin de 6 posiciones 8
    memset(bin, '0', sizeof(bin));
    memset(rango, '0', sizeof(rango));
    strncpy(pfi->fidatpro.finetpri, "0000",4);  //entidad resolutora primaria
#if defined (TRAZA) && (TRAZA > 10)	  
    fprintLog(nomProg,"bin a ceros     \n");
#endif	  
	  
	 	// el resolutor vendra indicado en la clave 4 de la tabla de operaciones
    if(nspaces(pfi->fidatpro.fisubres, 2))
    	strcpy(pfi->fidatpro.fisubres, "01");
    
    strncpy(pfi->fidatpro.firespri, pfi->fidatpro.ficlatab + 4 , 2 );
    pfi->fidatpro.firespri[2] = '\0';
	 	strncpy(pfi->fidatpro.fitipcla, "88" , 2 ); // operaciones sin tarjeta
    pfi->fidatpro.fitipcla[2] = '\0';

#if defined (TRAZA) && (TRAZA > 10)	 
    fprintLog(nomProg,"operacion sin tarjeta---resolutor : %.2s ---- origen : %.2s\n", pfi->fidatpro.firespri, pfi->fidatpro.fiorigen);	  
#endif	 
	}
  else
  {
  	//fprintLog(nomProg," Punto de control 4\n"); //PRJ0258542
    // OPERACIONES PARA ABONO A COMERCIOS 1124,1125,1134,1224,1225,1234,1424,1425,1434
	 	// ADQUIRENTE TERMINALES EN CAPTURA
    // (TARJETAS AJENAS EN TERMINALES PROPIOS QUE CUELGAN DE SERVIRED (TERMINALES EN CAPTURA)
    if (!strncmp(w_tipomsg,COMU_AUTO_TCAP,4) || //1124
          !strncmp(w_tipomsg, REPE_COMU_AUTO_TCAP,4) || //1125
          !strncmp(w_tipomsg, RESP_COMU_AUTO_TCAP,4) || //1134
          !strncmp(w_tipomsg, COMU_CONT_TCAP,4) || //1224
          !strncmp(w_tipomsg, REPE_COMU_CONT_TCAP,4) || //1225
          !strncmp(w_tipomsg, RESP_COMU_CONT_TCAP,4) || //1234
          !strncmp(w_tipomsg, ANUL_TCAP,4) || //1424
          !strncmp(w_tipomsg, REPE_ANUL_TCAP,4) || //1425		  
          !strncmp(w_tipomsg, RESP_ANUL_TCAP,4))   //1434 
		{		  
    	strncpy(pfi->fidatpro.finetpri, "0000",4);
#if defined (TRAZA) && (TRAZA > 10)		  
      fprintLog(nomProg,"Entidad adquirente 0000\n");
#endif		  
 
      // el resolutor vendra indicado en la clave 4 de la tabla de operaciones
      if(nspaces(pfi->fidatpro.fisubres, 2))
      	strcpy(pfi->fidatpro.fisubres, "01");
      
      strncpy(pfi->fidatpro.firespri, pfi->fidatpro.ficlatab + 4 , 2 );
      pfi->fidatpro.firespri[2] = '\0';
      strncpy(pfi->fidatpro.fitipcla, "22" , 2 ); // clave de operaciones de terminales en captura	 
      pfi->fidatpro.fitipcla[2] = '\0';
		}
	  else
	  { // RECARGAS TELEFONICAS - MENSAJE A LA OPERADORA VIA SERMEPA (se tratan todas como ajenas)
	  	if(!strncmp(pfi->fidatpro.fiorigen,"07",2) && // origen cajero
            !nspaces(pfi->fidatiso.fiisoaut.fiaut048.comun01.telefo, 9) && // informado el n� de tel襯no
             strncmp(pfi->fidatpro.ficlatab+8, "00" , 2)) // es peticion, no respuesta ni timeout
		 	{
#if defined (TRAZA) && (TRAZA > 10)		 
		  	fprintLog(nomProg,"Peticion de RECARGA TFN\n");
#endif			
		    //el resolutor sera red adquirente 
				//RAQUEL modificado para usar dato correspondiente de bbdd y entidad
				strcpy(pfi->fidatpro.firespri, reg_para[0].psnument);
				pfi->fidatpro.firespri[2] = '\0';
				strcpy(pfi->fidatpro.fisubres, "02");
				pfi->fidatpro.fisubres[2] = '\0';
				//el tipo de clave de operaciones tjt ajenas
				strncpy(pfi->fidatpro.fitipcla,"44",2);
				pfi->fidatpro.fitipcla[2] = '\0';
				//la entidad 0000
        strncpy(pfi->fidatpro.finetpri,"0000",4);
				pfi->fidatpro.finetpri[4] = '\0';
		 	} 
      // OPERACIONES CON TARJETA (PROPIAS Y AJENAS)
      else
      {
      	//establecemos el valor bin con los 6 primeros d즩tos del pan
        // PRJ0202598 cambio bin de 6 posiciones 8 
        strncpy(bin, pfi->fidatiso.fiisoaut.finumPAN.datos, 8);
        //el resto del pan es el rango
        longrango = atoi(pfi->fidatiso.fiisoaut.finumPAN.longitud) - 8;
        strncpy(rango, pfi->fidatiso.fiisoaut.finumPAN.datos + 8, longrango);
      
        /***
        en tarjetas propias el resolutor es el que indica la tabla de bines, excepto en
        las operaciones sin tarjeta, que vendra indicado SIEMPRE
        en la clave 4 de la tabla de operaciones
		  	en tarjetas ajenas el resolutor es la red
        ***/
        // buscamos resolutor en la tabla de bines si esta informado el bin
        // y Si no es respuesta                        
#if defined (TRAZA) && (TRAZA > 10)		 
        fprintLog(nomProg,"respri >%.2s<\n",pfi->fidatpro.firespri);
#endif		 

        if (!strncmp(pfi->fidatpro.firespri,"  ",2) &&
           strncmp(pfi->fidatpro.ficlatab+8, "40" , 2) &&
           strncmp(pfi->fidatpro.ficlatab+8, "41" , 2) &&
           strncmp(pfi->fidatpro.ficlatab+8, "42" , 2) &&
           strncmp(pfi->fidatpro.ficlatab+8, "50" , 2) )
        {
        	retorno = fbusca_bin(pbi_binprimero, bin, rango, longrango,
          	clave_ope, parautor1, parautor2, resolutor, entidad);

          if(retorno)
          {
		      	/////TARJETAS AJENAS
            //los bines que no aparecen en la tabla, son tarjetas ajenas que vienen de Cajero
            // y hay que enviar a la Red
			  		// salvo que lleguen por error desde redsys, en ese caso se deniegan
			  		if(strncmp(pfi->fidatpro.fiorigen,"07",2)!=0) /// si no viene de cajeros
			  		{
            	strcpy(pfi->fidatpro.ficodfin, "076"); /* tarjeta ajena*/ 
			  		}
			  		else
			  		{
            	//resolutor es la red 
							//RAQUEL modificado para recoger dato bbdd y entidad
              strcpy(pfi->fidatpro.firespri, reg_para[0].psnument);
              pfi->fidatpro.firespri[2] = '\0';			
              strcpy(pfi->fidatpro.fisubres, "01");	
			  		}
            //ponemos la entidad a cero, ya que no la tenemos
            strncpy(pfi->fidatpro.finetpri,"0000",4);
		      	strncpy(pfi->fidatpro.fitipcla, "44" , 2 ); // operaciones de tjt ajena
		      	pfi->fidatpro.fitipcla[2]='\0';
					}
		   		else
		   		{
          	//////TARJETAS PROPIAS
	        	//con el bin obtenemos la entidad y el resolutor
#if defined (TRAZA) && (TRAZA > 10)			
           	fprintLog(nomProg,"entidad >%.4s< resolutor >%.2s< \n", entidad, resolutor); 
#endif			        
            strncpy(pfi->fidatpro.finetpri, entidad,4);
            strcpy(pfi->fidatpro.firespri, resolutor);
            pfi->fidatpro.firespri[2] = '\0';
            //La subaplicaci򬟤el resolutor ¿Deber쟠ir en tabla de bines? 
            strcpy(pfi->fidatpro.fisubres, "01");
		    		// y el tipo de clave de operaciones
		    		strncpy(pfi->fidatpro.fitipcla, clave_ope,2);
		    		pfi->fidatpro.fitipcla[2]='\0';
		   		} 
     		}
	  	}
  	}  
 	}
  ///////// la clave de operaciones se busca con la ENTIDAD(4) + TIPO CLAVE(2) + FICLATAB(12)
  strncpy(clave,pfi->fidatpro.finetpri,4);
  strncpy(clave+4, pfi->fidatpro.fitipcla,2);
	fprintLog(nomProg," Clave asignada >%.6s<\n", clave); //PRJ0258542
	fprintLog(nomProg," resto clave ini>%.12s<\n", pfi->fidatpro.ficlatab); //PRJ0258542 - Fase 3 - Cambio 11052026
/* BUCLE DE LLAMADA A LAS FUNCIONES DE LA TABLA DE OPERACIONES */
 	tabla = 1;
 	while (tabla)
 	{
	
    strncpy(clave+6, pfi->fidatpro.ficlatab, sizeof(pfi->fidatpro.ficlatab));
		clave[18]='\0';
	
    fprintLog(nomProg,"Tabla numero %d, clave >%s< ptb_primero >%s< \n", tabla, clave, ptb_primero); //BIMSF-3597
    if ((retorno = fbusca_funciones(clave, ptb_primero, funciones)) != 0)
    {
#if defined (TRAZA) && (TRAZA > 1)	
      //fprintLog(nomProg,"Tabla de operaciones no encontrada ->%d<- clave ->%s<- \n", retorno, clave); //PRJ0257005
	  	fprintLog(nomProg,"Tabla numero %d no encontrada >%s< >%s< >%s< \n", tabla, clave, ptb_primero, funciones);
#endif	  
      fgraba_incidencia_cp("243", pfi->fidatpro.finumope, NULL, NULL);
      // respondemos denegando la operacion
      strcpy(pfi->fidatpro.ficodfin, "190"); /* operacion no encontrada en tablas de operaciones*/ //PRJ0258542 - Se cambia 261 por 190
      strcpy(pfi->fidatpro.firesfin, "00");
      if (strncmp(pfi->fidatpro.ficlatab,"55",2) == 0)
      {
         retorno = funcioned(pfi);   /* ENVIA AL ORIGEN  */
      }
      else
      {
         retorno = funcioneo(pfi);   /* ENVIA AL ORIGEN  */
      }
      return (9);
     }
			fprintLog(nomProg," Resto clave antes>%.12s<\n", pfi->fidatpro.ficlatab); //PRJ0258542 - Fase 3 - Cambio 11052026
			if (strncmp(pfi->fiisocom.fiiso000, "1805", 4) == 0 && strncmp(pfi->fiisocom.fiiso024, "816", 3) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
			{
				strncpy(pfi->fidatpro.ficlatab, ficlatab, 12); //PRJ0258542 - Fase 3 - Cambio 11052026
			}
			fprintLog(nomProg," Resto clave despues>%.12s<\n", pfi->fidatpro.ficlatab); //PRJ0258542 - Fase 3 - Cambio 11052026

     /* llama a las funciones para tratar el mensaje  */
     retorno = fllama(pfi, funciones);

     switch (retorno)
     {
     case 1: /*** vuelve a entrar en tabla  ***/
#if defined (TRAZA) && (TRAZA > 10)	 
       fprintLog(nomProg,"Entra otra vez en tabla\n");
#endif	   
     break;
     case 2:  /*** error en los tratamientos, fin proceso **/
#if defined (TRAZA) && (TRAZA > 1)	 
       fprintLog(nomProg,"fin por error \n");
#endif	   
        desconexhsm(sfd_hsm); //cada vez que salimos desconectamos el HSM 
        exit(finalizar(-2)); 
      default: /*** fin  tratamiento del mensaje ***/
        tabla = 0;
     }

 }  /** fin while tabla llama funciones **/

	fprintLog(nomProg,"Sale de la funcion ftratami\n"); //PRJ0258542	
 	return(0);
}    //  fin de la funcion ftratami


/**********************************************/
/***   funcion: busca bin                   ***/
/**********************************************/
int fbusca_bin(tbine *pbine, char *pbinope, char *prangoope, int plongrango,
                 char *ptboper, char *pparau1, char *pparau2,char *prespri, 
				 char *codent)

{
  // PRJ0202598 cambio bin de 6 posiciones 8 cambia el tamaño de tbine y auxbin  y a MAXBINE cambiado en cnetplus.c
  char auxbin[MAXBINE+1];
  char auxrng[17];
  tbine *pp;
  int numaster; /* numero de asteriscos */

 pp = pbine;

 strncpy(auxbin , pbinope , MAXBINE);
 auxbin[MAXBINE] = '\0';

 strncpy(auxrng, prangoope, plongrango);
 auxrng[plongrango] ='\0';

#if defined (TRAZA) && (TRAZA > 10)
 fprintLog(nomProg,"CLAVE bin = %.*s  MAXBINE=%d\n", MAXBINE, pbinope, MAXBINE);
#endif 

 while (pp != NULL)
 {
   // PRJ0202598 cambio bin de 6 posiciones 8 cambia el tamaño de t_biclave0
      if (!strncmp(auxbin, pp->t_biclave0, MAXBINE)
      && strncmp(auxrng, pp->t_birnginf, plongrango) > 0
      && strncmp(auxrng, pp->t_birngsup, plongrango) < 0)
      {
#if defined (TRAZA) && (TRAZA > 10)	  
         fprintLog(nomProg, "bin encontrado\n"); 
#endif		 
         strncpy (ptboper, pp->t_bitboper,sizeof(pp->t_bitboper));
         strncpy (prespri, pp->t_birespri,sizeof(pp->t_birespri));
         strncpy (codent,  pp->t_bicodent,sizeof(pp->t_bicodent));
         strncpy (pparau1, pp->t_biparau1,sizeof(pp->t_biparau1)); //PTASK000023003
         return (0);
      }
    //}
    pp = pp->bin_siguiente;
 }
#if defined (TRAZA) && (TRAZA > 1)
 fprintLog(nomProg,"Tarjeta ajena\n");
#endif 
 return (1);

}

/**********************************************/
/***   funcion: busca funciones             ***/
/**********************************************/
int fbusca_funciones(char *pque, ttbop *ptabla, char *pobjeto)
{
  ttbop *p;

  p = ptabla;
#if defined (TRAZA) && (TRAZA > 10)  
  fprintLog(nomProg,"CLAVE TBOP = %s \n", pque);
#endif  
  while (p != NULL)
  {
    if (!strncmp(pque, p->clave,18))
    {
       strncpy (pobjeto, p->funcion, sizeof(p->funcion));
       return (0);
     }
     else
       p = p->siguiente;
  }
  return(1);
}

/**********************************************/
/***  llama a las funciones                 ***/
/**********************************************/
int fllama(formato_interno * pforitn, char *funciones)
{
  int k, i, retorno, j = 0;
  int encontrada;
  char f[3];

#if defined (TRAZA) && (TRAZA > 5)  
 fprintLog(nomProg,"funciones = >%.*s<\n", sizeof(ptb_primero->funcion), funciones);
#endif 

 	while (funciones[j])      /* bucle hasta fin del array
                              de una dimension funciones */
	{
   	for (i = 0; i < 2; i++)  /* va tomando de dos en dos los elementos del
                              array funciones  */
  	{
   		f[i] = toupper(funciones[j]);
   		j++;
  	}                         /* fin del for */

  	f[i]='\0';

#if defined (TRAZA) && (TRAZA > 5)
  	fprintLog(nomProg,"Llama desde rtratami ->%s<---- \n", f);
#endif  
		//PRJ288711 - INI
		//fprintLog(nomProg,"Recepcion de operacion con iso_00 %.4s y con respusal: %.*s\n",pforitn->fiisocom.fiiso000,3,pforitn->fiisocom.respusal); 
		if (!strcmp(f,"ED"))
		{
			fprintLog(nomProg,"Recepcion de diferido con respusal: %.*s\n",3,pforitn->fiisocom.respusal); 
			if (strstr(pforitn->fiisocom.respusal,"01") == NULL)
			{
				fprintLog(nomProg,"Diferido distinto de 01, graba diferido erroneo\n"); 
				retorno = funciongd(pforitn, "E"); // Al ser un diferido y devolver error, actualiza en NPTERRD 
			}
			else
			{
				fprintLog(nomProg,"Diferido contestado OK, no lo graba como erroneo\n"); 
				retorno = funciongd(pforitn, "B"); // Al ser un diferido y devolver error, borra de NPTERRD
			}
		}
		//PRJ288711 - FIN

  	if (!strcmp(f,"TB"))  /** vuelve a entrar en tabla ***/
   		return(1);

  	if (!strcmp(f,"**")) /*** fin de los tratamientos  **/
   		return(0);
                              /** busca en la tabla tbtratam el
                                  puntero de la funcion que debe llamar  */

  	encontrada = 0;
  	k = 0;
  	while (strcmp(tratamientos[k].simbolo,"xx"))
  	{
  		//    fprintLog("llama dentro while ->%s< tratamientos[k] >%s<---- k >%s<\n", f,tratamientos[k],k);

   		if (!strcmp(f,tratamientos[k].simbolo))
   		{
    		encontrada = 1;
    		retorno = tratamientos[k].pfunc(pforitn); /* llamada a la funcion */
    		fprintLog(nomProg,"Salida de la funcion >%s< con retorno >%i<\n", f, retorno);
				switch (retorno)
    		{
     			case 11:  /* la funcion termino OK, llamar a la siguiente*/
      			break;
     			case 97:  /* la funcion termino OK, NO responder. Esperamos repeticiones */
      			return(retorno);
     			case 98:  /* la funcion termino OK, responder */
        		if (strncmp(pforitn->fidatpro.ficlatab,"55",2) == 0)
        		{
            	retorno = funcioned(pforitn);   /* ENVIA AL ORIGEN  */
        		}
        		else
        		{
            	retorno = funcioneo(pforitn);
        		}
      			return(retorno);
     			case 99:  /* hay que entrar otra vez en tabla operaciones */
      			return(1);
      			break;
     			default: /* la funcion termino con error, fin proceso */
      			return(2);
      			break;
    		}
    		break;
   		}
   		else
   		{
    		k++;
   		} /** fin else **/
  	} /** fin while tratamientos **/
#if defined (TRAZA) && (TRAZA > 5)
  	fprintLog(nomProg,"Fin bucle rtratami \n");
#endif  
  	if(!encontrada)
  	{
#if defined (TRAZA) && (TRAZA > 1)  
    	fprintLog(nomProg,"No encontrada la funcion >%s<\n", f);
#endif	
   		fgraba_incidencia_cp("909", pforitn->fidatpro.finumope, NULL, NULL);
    	// respondemos denegando la operacion
    	strcpy(pforitn->fidatpro.ficodfin, "909"); /* error del sistema*/
    	strcpy(pforitn->fidatpro.firesfin, "00");
    	if (strncmp(pforitn->fidatpro.ficlatab,"55",2) == 0)
    	{
     		retorno = funcioned(pforitn);   /* ENVIA AL ORIGEN  */
    	}
    	else
    	{
      	retorno = funcioneo(pforitn);   /* ENVIA AL ORIGEN  */
    	}
    	break;
  	}
 	} /*** fin de while funciones **/
 	return(0);
} /** fin funcion ***/

/***
    Funciones generales
**/
#include "ftrataer.c"
#include "ftrataeo.c"
#include "ftrataed.c"
#include "fcartbop.c"
#include "ftrataco.c"
#include "ftrataac.c"
#include "ftratams.c"
#include "ftratapr.c"
#include "ftratarp.c"
#include "ftratard.c"
#include "ftratato.c"

/** atraves del subsistema 0301 **/
#include "ftratass.c"
#include "ftratadi.c"
#include "ftratadp.c"
#include "ftratach.c"
#include "ftratath.c"
#include "ftratavh.c"
#include "ftratalg.c"
#include "fgrablog.c"
#include "fgradife.c"
#include "ftratade.c" //PRJ288711 - Tratamiento diferidos erróneos
#include "fclasmsg.c"
#include "fsemaf00.c"
#include "fcarbine.c"
#include "fsemaforo_red.c"
#include "fcifraPan.c"


