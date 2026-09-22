//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
/***************************************************************************
 PROGRAMA:  MANEJ
 DESCRIPCION: Proceso servidor del cliente tuxedo.
             - Se conecta a las colas de intercambio de mensajes con NETPLUS 
             - Se pone en disposicion de aceptar conexiones.
		 - Bucle infinto
		 - Si recibe una peticion crea un hijo para que la atienda   
		     - Proceso hijo       
			 - Recibe el mensaje 
			 - Entrega el mensaje a NETPLUS en la cola QRECEP
			 - Espera la respuesta en la cola QMANEJ
			 - Envia la respuesta al destino	
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/timeb.h>

//tuxedo
#include <atmi.h> 
//#include "atmi.h" 

#include "datagen.h"
#include "deffich.h"
#include "procesos.h"
#include "nptinci.h"
#include "svm.h"
#include "ftradreso.h"
#include "max_min.h"
#include "cnetplus.h"
//include con la ip y los puertos

#define LONMSG_IN   4096
#define LONMSG_OUT  4096
//#define SO_REUSEPORT  SO_REUSEADDR

int TBASE; //PTASK0022158 - terminal de inicio
#define MTERM 5011 //PTASK0022158 - maximo numero de terminales disponibles

//PTASK000023003 - INI
#include "nptbine.h"
char coderror[3];
typedef struct dbine tbine;
struct dbine
{
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

int fbusca_bin(tbine *pbine, char *pbinope, char *prangoope, int plongrango, char *ptboper, char *pparau1, char *pparau2,char *prespri, char *codent);
//PTASK000023003 - FIN



int flag_01; // para indicar si se trata del padre, hijo o nieto
#define PADRE 0
#define HIJO  1
#define NIETO 2
#define VACIO -1
#define MAXSOCK 100 // MARGA: pongo 40 para ir probando en pruebas de estrés

/** tabla para los sockets *****/
typedef struct
{
   int      num_ip;
   char     nompadre[11];
   int      pidpadre;
   int      pidhijo;
   int      signieto;
} tbsock;

tbsock    tabla_sock [MAXSOCK];
#define MAX_REG_TBSOCK (MAXSOCK)
#define TAM_TBSOCK       ((MAX_REG_TBSOCK+1)*sizeof(tbsock))


#define SVMhijo   (psvmsock+posic)->pidhijo
#define SVMpadre  (psvmsock+posic)->pidpadre
#define SVMnombre (psvmsock+posic)->nompadre
#define SVMip     (psvmsock+posic)->num_ip
#define SVMsig    (psvmsock+posic)->signieto

/***  --------- prototipos de funciones  ----------------  ***/
int InitBegin (const char *Msg,const char *TxName,const char *Trml,int len);
int TratamientoTuxedo(char *Msg,char *Trml,int len, char * Tx);
int LlamadaTuxedo(const char *TxName,const char *Msg,int *lenMsgSer);
int TratamientoFin(char * Msg, char * Txname, int *lenMsgSer, const char *Trml,int len);
int CommitRollback (char *Servicio);
void LiberarTux();
int AllocBuffer(const char *Msg,const char *TxName,const char *Trml,int len);
void sigue();
void cierre(int sig);
int Atiende_conexion();
int dame_posisock(tbsock *psvmsock);
void limpia_tbs(tbsock *pp);
int Pkill(int Ppid,int Psenal);
int prepara_conexion_perm();
int carga_s(tbsock *psvmsock);
void QuitarCar(char* cadena, int longi);
void    fechorsys(struct tm_dec *tmPtr);
/**--------------funciones de manejo de la SVM-----------***/
FILE * pFile;

int sfd; 
int MISOCKET;
int lenviar;
int posic;
int MIPID;
char Datomsg[max(LONMSG_IN,LONMSG_OUT)];
tbsock     *psvmsock;
int  nsfd;
int enviar=0;
struct sockaddr_in cli_addr;
int  cli_addr_len;
int tipo;
tm_dec tm;
//int pidnieto;*/
TPINIT           *tpinfo;
char             *sendbuf;        /* Buffer para enviar a tuxedo */
int lenME = 4000;
char             *recvbuf;        /* Buffer para recibir de tuxedo */
int Puerto;


int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
   int retorno; 
   int ErrConSoc=0;
   int socConectado=0;
   struct sockaddr_in cli_addr;
   int  pid;
   //tm_dec tm;
   char fechaAux[15];
   char fechaAct[20];
   char fechaAct1[20];
   char fichero[200];
   char fichaux1time[200];
   
   // incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,cierre);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);
   signal(SIGCLD,SIG_IGN);
   signal(SIGUSR1,cierre);
   signal(SIGALRM,SIG_IGN);

//////////////////////////////// MONTA EL FICHERO DE TRAZAS ///////////////////   
   //coger fecha del sistema
   fechorsys(&tm);
//formateo fecha
   sprintf(fechaAct,"_%02d%02d%04d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);
 sprintf(fichero, "%s%s/pmanejTux.traza",HOME,DIRTRAZA);
 
  pFile = freopen (fichero, "w", stdout);

  

#if defined (TRAZA) && (TRAZA > 1) 
  fprintf(pFile, "----- ----- Inicio ----- ------\n");
  fflush(pFile);
#endif

  if (argc != 2)
  {
     fprintf(pFile, "Error en los parametros de arranque <%d>\n", argc);
     exit(1);
  }
  else
  {
     Puerto = atoi(argv[1]);
     fprintf(pFile, " ****Puerto de escucha <%d>\n", Puerto);
  }

////////////////////////////// ASIGNA EL PUNTERO A LA ESTRUCTURA DE CONEXIONES ////////////
///// sirve para tener controladas cuantas son y poder ir matándolas en el cierre
  psvmsock = (tbsock *) malloc((MAXSOCK+1) * sizeof(tbsock));
  retorno=carga_s(psvmsock);

/////////////////////////////// BUCLE PARA ESTABLECER EL SOCKET SERVIDOR /////////////////  
  while( socConectado == 0)
  {
    retorno=prepara_conexion_perm();
	if (retorno)
    {
      if (ErrConSoc == 0)//para sacar solo una traza
      {
         ErrConSoc = 1;
        //Añadido para Monitorizacion SISMA 
        fprintf(pFile,"TIPO SISTEMA – ACCESO A TUXEDO: error en la conexion del socket error %i\n", retorno);

         fprintf(pFile,"error en la conexion del socket\n");
         fflush(pFile);
      }
      sleep(3); //espera 3 segundos entre cada intento de reconexion
    }
	else
	{
	   socConectado = 1;
       fprintf(pFile,"***** Socket conectado sfd>%d<\n", sfd);
       fflush(pFile);
	}
  }
  
  MISOCKET=sfd;
  for(;;)
  {
		//PTASK0022158 - INI
		TBASE = TBASE + 1;
		if (TBASE > MTERM)
		{
			TBASE = 5000;
		}
		if (TBASE < 5000)
		{
			TBASE = 5000;
		}
		//PTASK0022158 - FIN
     /////////// REVISA SI TIENE QUE CREAR NUEVO FICHERO DE TRAZAS ////////////////////////////  
     //coger fecha del sistema
      fechorsys(&tm);
     //formateo fecha
      sprintf(fechaAct1,"_%02d%02d%04d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);
      if (strncmp(fechaAct1, fechaAct, 12)) //si las fechas son disitntas
      {
//           sprintf(fichero, "%s%s/pmanejTux_%s.traza",HOME,DIRTRTUX, fechaAct1);
           sprintf(fichero, "%s%s/pmanejTux.traza",HOME,DIRTRTUX);
   
//           fclose(stdout);
           sprintf(fichaux1time, "%s%s/pmanejTux_%s_1.traza",HOME,DIRTRAZA, fechaAct);
           rename(fichero, fichaux1time);

           pFile = freopen (fichero, "w", stdout);
	   strncpy(fechaAct,  fechaAct1, 12);
      }

	  ////////// SE PREPARA PARA RECIBIR PETICIONES: ACCEPT  ///////////////////////////
      cli_addr_len = sizeof(cli_addr);
      nsfd=accept(sfd,(struct sockaddr *)&cli_addr, (unsigned int*)&cli_addr_len);

      fechorsys(&tm);
      sprintf(fechaAux, "%.*d%.*d%.*d%.*d%.*d%.*d%",2,tm.tm_year,2,tm.tm_mon,2,tm.tm_mday,2,tm.tm_hour,2,tm.tm_min, tm.tm_sec);
      fechaAux[14]='\0';

      if (nsfd == -1)
      {
           fprintf(pFile,"%s - error llamada accept errno >%d<\n",fechaAux, errno);
           fflush(pFile);
		   sleep(5);
           continue;
      }

      flag_01 = PADRE;

      posic=dame_posisock(psvmsock);
      if (posic == -1)
      {
        fprintf(pFile,"%s - error dame_posisvmsock:%d espero y reintento\n",fechaAux, posic);
        fflush(pFile);
        usleep(10);	
		posic=dame_posisock(psvmsock);
      }
      if (posic == -1)
      {
        fprintf(pFile,"%s - segundo error dame_posisvmsock:%d - no mas intentos\n",fechaAux, posic);
        fflush(pFile);
        close(nsfd);
      }	  
      else
      {
        MIPID= getpid();
        SVMpadre=MIPID;
        strcpy(SVMnombre,argv[0]);
        if ((pid=fork())==-1)
        {
           fprintf(pFile,"%s - error fork:%d\n",fechaAux,errno);
           fflush(pFile);
           close(nsfd);
           limpia_tbs(psvmsock+posic);
         }
         else
         {
           if (pid==0) /* Hijo */
           {
             SVMhijo=getpid();
             flag_01 = HIJO;
             SVMsig=SIGUSR1;
//revisar, pq ahora se esta cerrando el socket en el hijo, y por eso se muere, pero realemten solo deberia morirse con 
//una alarma por si no le contestan en Atiende_conesion con signal(SIGALRM, cerrar) como pmanje03, en atiende hayq ue poner el 
//alarm al principio y al final
             close(sfd);
			 fprintf(pFile, "  \n");
			 fprintf(pFile, "----------------------------------------\n");
             fprintf(pFile, "-- Antes de retorno = Atiende_conexion\n");
             retorno = Atiende_conexion(nsfd);
             fprintf(pFile, "-- Despues de retorno = Atiende_conexion\n");
             shutdown(nsfd,2);
             close(nsfd);
             exit(0);
           }  /** codigo del hijo **/
           else /* Padre */
           {
		      //fprintf(pFile,"soy padre\n");
              usleep(1);
              close(nsfd);
           }  /** codigo del padre **/
         } /** fork() **/
      }  /** if posicion **/
    }  /** for infinito **/
    fprintf(pFile, "------ Salimos de main pmanejTux -----\n");
}


/***** 
FUNCION: CIERRE del proceso 
****/
void cierre(int sig)
{
  int err;
  int pp;
  err=0;
  errno=0;
fprintf(pFile, "-- Función cierre de pmanejTux --\n"); //PRJ0257005
#if defined (TRAZA) && (TRAZA > 1) 
   if (flag_01==NIETO)
       fprintf(pFile,"Soy NIETO ");
   if (flag_01==HIJO)
       fprintf(pFile,"Soy HIJO ");
   if (flag_01==PADRE)
       fprintf(pFile,"Soy PADRE ");
  fprintf(pFile,"estoy en cierre\n");
  fflush(pFile);

#endif

/*   if (flag_01==NIETO)
   {
      SVMsig=VACIO;
      SVMnieto=VACIO;
   }*/
   if (flag_01==HIJO)
   {
      signal(SIGCHLD,SIG_IGN);
   }
   if (flag_01==PADRE)
   {
      /* Hay que parar hijos y nietos limpiar SVMsock*/
      fprintf(pFile,"numero de hijos que hay que matar %i\n", MAX_REG_TBSOCK);
      fflush(pFile);

      for (pp=0;pp<MAX_REG_TBSOCK;pp++)
      {
        posic=pp;
//        Pkill(SVMnieto,SVMsig);
        Pkill(SVMhijo,SVMsig);
        limpia_tbs(psvmsock+pp);
      }
        exit(0); /* Orden de Cierrre */
   }
   exit(0);
}

int proceso_recep()
{
  int retorno;
  fprintf(pFile, "-- Función proceso_recep de pmanejTux --\n"); //PRJ0257005  
  retorno = LeerSock(nsfd, 0, Datomsg, LONMSG_IN);
  if ((retorno == -1) || (retorno == 0))
  {
    //Añadido para Monitorizacion SISMA 
   fprintf(pFile,"TIPO SISTEMA – ACCESO A TUXEDO: error al leer el socket  LeerSock retorno %i\n", retorno);
     
   fprintf(pFile,"Control 1.- H:%d-Error LeerSock >%d<\n", SVMhijo, retorno); //PRJ0247167
   fflush(pFile);
   return(-1);
  }
   enviar=retorno;
   
#if defined (TRAZA) && (TRAZA > 1)
    //fprintf(pFile,"********** Mensaje Recibido de la Linea **********\n");
   fechorsys(&tm);
   fprintf(pFile, "Control 2.- H:%d-%04d%02d%02d-%02d:%02d:%02d NUMOPE >%.6s< Recibidos del socket >%d< Bytes\n", SVMhijo, 
	        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, Datomsg +51 ,retorno); //PRJ0247167 - antes eran 66 ahora son 51
    //fprintf(pFile,"**************************************************\n");
  fflush(pFile);

#endif

   return(retorno);
}

int proceso_envio()
{
  int retorno;
  int leidos;
  int i;
  fprintf(pFile, "-- Función proceso_envio de pmanejTux --\n"); //PRJ0257005
  //validar datos
  memset(Datomsg, ' ', 4000);
/*xavi, enviamos todo el mensaje recibido de tuxedo
  if (!strncmp(recvbuf+107, "OK",2))
  {
     for(i=0; i<2000; i++)
     {
         if (recvbuf[153+i] == '<')
            break;
     }
     memcpy(Datomsg, recvbuf+153, i);
  }
*/    		
//xavi, enviamos todo el mensaje recibido de tuxedo
  fprintf(pFile, "  Mensaje a enviar a pmanej03 len <%d>\n", lenviar); //PRJ0247167
  fprintf(pFile, "  Respuesta 463 >%.2s< y 2979 >%.2s<\n", recvbuf +462, recvbuf +2978); //PRJ0247167
  memcpy(Datomsg, recvbuf, lenviar);
  retorno = EnviaSock(nsfd, 2, Datomsg, lenviar);
  if (retorno == -1)
  {
    //Añadido para Monitorizacion SISMA 
   fprintf(pFile,"TIPO SISTEMA – ACCESO A TUXEDO: error al enviar el socket EnviaSock retorno %i\n", retorno);

     fprintf(pFile, "Control 3.- H:%d-Error EnviaSock >%d<:\n", SVMhijo, retorno);
     fflush(pFile);
	 fprintf(pFile, "Respuesta 463 >%.2s< y 2979 >%.2s<\n", recvbuf +462, recvbuf +2978); //PRJ0247167
     fflush(pFile);

  }
  return(retorno);
}

//prepara el socket
int prepara_conexion_perm()
{
  int pid;
  int retorno;
  struct sockaddr_in ser_addr;

  fprintf(pFile, "-- Función prepara_conexion_perm de pmanejTux --\n"); //PRJ0257005
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(pFile,"abrir  socket\n");
  fflush(pFile);
#endif

 if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
 {
  fprintf(pFile,"**** **** error llamada socket >%d<\n",errno);
  perror("error en socket");
  //Añadido para Monitorizacion SISMA 
  fprintf(pFile,"TIPO SISTEMA – ACCESO A TUXEDO: error al abrir el socket errno %d\n", errno);

  fflush(pFile);
  return(-1);
 }

 retorno=1;
 pid=setsockopt(sfd,SOL_SOCKET,SO_REUSEPORT,&retorno,sizeof(retorno)); //SO_REUSEPORT: permite que varios procesos (retorno)

 //se unan a la misma dirección.
 if (pid != 0)
 {
  fprintf(pFile,"REUSEPORT=%d\n",pid);
  fflush(pFile);
 }


 memset(&ser_addr,'\0',sizeof(ser_addr));
 ser_addr.sin_port=htons(Puerto);
#if defined (TRAZA) && (TRAZA > 1)
 fprintf(pFile,"Escucha en el puerto >%d< MAXSOCK >%d<\n", Puerto,MAXSOCK);
 fflush(pFile);
#endif

 ser_addr.sin_family=AF_INET;
 ser_addr.sin_addr.s_addr=INADDR_ANY;

 if (bind(sfd, (struct sockaddr *)&ser_addr,sizeof(ser_addr))==-1)
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(pFile,"error en bind:%d\n",errno);
  perror("error en bind");
  fflush(pFile);
#endif
  if (errno == 125) //la dirección ya esta en uso
  {
    fprintf(pFile, "la dirección ya esta en uso\n"); 
	fflush(pFile);
    return (-2);
  }
  else
    return(-1);
 }

 //listen(sfd,256);
 listen(sfd, MAXSOCK); // pongo el mismo número de sockets simultaneos que en la estructura tbmsock

 return(0);
}



int Atiende_conexion()
{
  int tiempoIni=0;
  int pid;
  int retorno;
  size_t direc_len;
  struct sigaction act; 
  char serv_tux[9];

//PTASK000023003 - INI
	char bin [8];
	char bizum[1];
	char rango[16];
	char lrango[2];
	int longrango;
	char clave_ope[3];
	char parautor1[2];
	char parautor2[2];
	char resolutor[3];
	char entidad[4];
//PTASK000023003 - FIN


  fprintf(pFile, "-- Función Atiende_conexion de pmanejTux --\n"); //PRJ0257005
   tipo = cli_addr.sin_addr.s_addr;
   if (tipo < 1)
       tipo*=-1;
//#if defined (TRAZA) && (TRAZA > 1) 
//   fprintf(pFile,"Atiende conexion con direccion %i\n", tipo);
//   fflush(pFile);
//#endif

   SVMip=tipo;
   retorno = proceso_recep();
   QuitarCar(Datomsg, enviar); 

   fprintf(pFile, "Mensaje recibido <%s>\n", Datomsg); 
   fechorsys(&tm);
   tiempoIni = tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec;
   
// BIZUM ponemos el nombre del servicio en la variable
   strncpy(serv_tux,"GTOPESIO",8);
 
//envia al tuxedo
//   if(!TratamientoTuxedo(Datomsg,"999999",enviar,"SMPT0909"))
// BIZUM se debe revisar el mensaje para detectar mensajes bizum y controlar si se hace llamada aotro servicio tuxedo
// BIZUM revisar codope	
//PTASK000023003 - INI
		strncpy(bin,Datomsg + 152, 8);
		strncpy(bizum,Datomsg + 3581, 1);
		fprintf(pFile, "--- Este BIN %.8s es BIZUM B = Si o T = No -->%.1s<-- \n", bin, bizum);
		if (strncmp(bizum, "B", 1) == 0)
		{
			strncpy(serv_tux,"GCOPEBIZ",8);
		}
		else
		{
			strncpy(serv_tux,"GTOPESIO",8);
		}
		fprintf(pFile, " - - Servicio al que se llama >%.8s<\n", serv_tux);
//PTASK000023003 - FIN
/* //PTASK000023003
   //if ( !strncmp(Datomsg +170,"29",2) || !strncmp(Datomsg +170,"10",2) ) //BIZUM codope 2 primera posiciones
   if ( !strncmp(Datomsg +170,"29",2) || !strncmp(Datomsg +170,"10",2) )//BIZUM  nuevo servicio tuxedo
   {
      //strncpy(serv_tux,"GTTIPRUE",8);
      strncpy(serv_tux,"GCOPEBIZ",8);
      fprintf(pFile, " - - Mensaje BIZUM enviado a >%s< tuxedo CODOPE >%.2s\n", serv_tux, Datomsg +170); //BIZUM
   }
*/
   //if(!TratamientoTuxedo(Datomsg,"999999",enviar,"GTOPESIO"))
   if(!TratamientoTuxedo(Datomsg,"999999",enviar,serv_tux))
   {
        fprintf(pFile, "Control 4.- H:%d-error en el tratamiento del tuxedo\n", SVMhijo); //PRJ0247167
        fflush(pFile);
        return(-1); 
   }
   else
   {
      retorno = proceso_envio();
      fechorsys(&tm);
      fprintf(pFile, "Control 5.- H:%d-%04d%02d%02d-%02d:%02d:%02d NUMOPE >%.6s< tiempo respuesta tuxedo: %d\n", SVMhijo, 
	        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, Datomsg +39,
			((tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec)- tiempoIni)); //PRJ0247167 - antes eran 66 ahora son 39
      fflush(pFile);
      return(0);
   }
}


//  si retorna 0 es error y no se envia la respuesta
int TratamientoTuxedo(char *Msg,char *Trml,int len, char * Tx)
{
   int  LenMsg; /* Longitud a mandar al cliente */
   
   fprintf(pFile, "-- Función TratamientoTuxedo de pmanejTux --\n"); //PRJ0257005

   if (!InitBegin(Msg,Tx,Trml,len))
        return 0;
   
   // Inicializar areas y licencia Tuxedo 
   /*
   if (!AllocBuffer(Msg,IdentGlobal,Tx,Trml,len))
   {
      if(CommitRollback("ROLLBA")==0)
         LiberarTux;
      
      return 0;
   }
   */

   if (!LlamadaTuxedo(Tx,Msg,&LenMsg))
   {
	  recvbuf[161]='1'; //forzamos rollback si da error
//	  if(TratamientoFin(Msg, Tx, &LenMsg, Trml, len))
          return 0;
   }
   else
   {
     lenviar = LenMsg;
     
//	 if(TratamientoFin(Msg, Tx, &LenMsg, Trml, len))
	    return 1;
//	 else
//	    return 0;
   }
}

//si hay error devuelve 0
int TratamientoFin(char * Msg, char * Txname, int *lenMsgSer, const char *Trml,int len)
{
   char AcTx[7];
   int ErrorTux=0;
   
   fprintf(pFile, "-- Función TratamientoFin de pmanejTux --\n"); //PRJ0257005

   strncpy(AcTx, "ROLLBA", 6);
   if (memcmp(recvbuf+462, "01", 2)==0)
      strncpy(AcTx, "COMMIT", 6);
   if (memcmp(recvbuf+153+8, "2", 1)==0)
      strncpy(AcTx, "REPITE", 6);
   strncpy(AcTx, "COMMIT", 6); 
   AcTx[6]='\0';
   
   fprintf(pFile, "Valor AcTx <%s>", AcTx);
    
   //Comprobar si hay que repetir
   //en las comunicaciones hacemos commit para que se guarden y luego se vuelve a mandar la operación para que 
   //si devuelve error el rollback no borre la autorización
   if(memcmp(AcTx,"REPITE",6)==0)
   {
      fprintf(pFile, "Control 6.- H:%d-Accion=%s         >%.6s<\n", SVMhijo, AcTx, recvbuf+219); //PRJ0247167
      fflush(pFile);
   
      if(tpcommit(0)==-1)
      {
         fprintf(pFile,"Control 7.- H:%d-Error commit %i\n",SVMhijo, tperrno); //PRJ0247167
         fflush(pFile);
		 ErrorTux=tperrno;
	     strncpy(AcTx, "ROLLBA", 6);
      }
	  else
	  {	  
	     fprintf(pFile,"Control 8 .- H:%d-COMMIT y repeticion\n",SVMhijo); //PRJ0247167
         
		 if (!InitBegin(Msg,Txname,Trml,len))
         {
		    //si da error el tpbegin cuando estamos repitiendo
            fprintf(pFile, "Control 9.- H:%d-error repite Initbegin %i\n" , SVMhijo,tperrno); //PRJ0247167
            fflush(pFile);
            strncpy(AcTx, "NOTRAN", 6); //  error en tpbegin no hay que terminar la transaccion
			ErrorTux=tperrno;
	     }
	     else
	     {
	        if (!LlamadaTuxedo(Txname,Msg,&lenMsgSer))
			{  //si da error la llamada a tuxedo cuando estamos repitiendo
			   fprintf(pFile, "Control 10.- H:%d-error repite tpcall %i\n" ,SVMhijo, tperrno); //PRJ0247167
			   fflush(pFile);
	           strncpy(AcTx, "ROLLBA", 6);
			   ErrorTux=tperrno;
			}
            else
            {
               strncpy(AcTx, "ROLLBA", 6);
               if (memcmp(recvbuf+153+8, "0", 1)==0)
                 strncpy(AcTx, "COMMIT", 6);			 
            }
         }	  
      }
   }
   
   /* Realizar COMMIT/ROLLBACK*/
   if((memcmp(AcTx,"ROLLBA",6)==0) || (memcmp(AcTx,"COMMIT",6)==0))
   {
       fprintf(pFile, "Control 11.- H:%d-Accion=%s         >%.6s<\n",SVMhijo, AcTx, recvbuf+219); //PRJ0247167
       fflush(pFile);
       if(CommitRollback(AcTx)==0)
       {
         fprintf(pFile, "Control 11.- H:%d-TratamientoFin. Error CommitRollback.\n", SVMhijo); //PRJ0247167
         fflush(pFile);

         return 0;
       }
   } 
   
   if (ErrorTux)
     return 0;
   else
     return 1;
}

int InitBegin (const char *Msg,const char *TxName,const char *Trml,int len)
{
	fprintf(pFile, "-- Función InitBegin de pmanejTux --\n"); //PRJ0257005

   if (tpinfo==(TPINIT *)NULL)
   {
      tpinfo=(TPINIT *)tpalloc("TPINIT",(char *)NULL,sizeof(struct tpinfo_t));
      if(tpinfo==(TPINIT *)NULL)
      {
         fprintf(pFile,"Control 12.- H:%d-Error TPINIT. tpalloc\n", SVMhijo); //PRJ0247167
         fflush(pFile);

         LiberarTux;
         return 0;
      }
   }
   
   if (!AllocBuffer(Msg,TxName,Trml,len))
   {
      fprintf(pFile,"Control 13.- H:%d-Error ALLOCKBUFFER\n", SVMhijo); //PRJ0247167
      if(CommitRollback("ROLLBA")==0)
         LiberarTux;
      
      return 0;
   }
   
/* Xavi quitamos el tpbegin, la tx la govierna tuxedo   
   if(tpbegin(4,0)==-1) //4 segundos de timeout
   {
         fprintf(pFile, "H:%d-ERROR tpbegin %i\n" ,SVMhijo, tperrno);
         fflush(pFile);
		 
         return 0;
   }
*/

#if defined (TRAZA) && (TRAZA > 1)
   fechorsys(&tm);
   fprintf(pFile, "Control 14.- H:%d-%04d%02d%02d-%02d:%02d:%02d BEGIN\n", SVMhijo, 
	        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); //PRJ0247167
   fflush(pFile);

#endif


    /* Guardar trml's y user's globales para tx */
    //GetTrmlUserGlobal(MsgRec,Trml,User);
 
   return 1;
}


int AllocBuffer(const char *Msg, const char *TxName,const char *Trml,int len)
{
   long CodTx;
   long sendlen =4024 ;//tamaño mensaje SISMA
   char CabeceraInput[300];
   char DatosInput[sendlen+1];
   
   fprintf(pFile, "-- Función AllocBuffer de pmanejTux --\n"); //PRJ0257005
   
   if (sendbuf==(char *)NULL)
   {
      if((sendbuf=(char *)tpalloc("CARRAY",(char *)NULL,sendlen))==(char *)NULL)
      {
         fprintf(pFile,  "Control 15.- H:%d-ERROR AllocBuffer. tpalloc sendbuf\n", SVMhijo); //PRJ0247167
         fflush(pFile);

         LiberarTux;
         return 0;
      }
   }

//xavi
//xavi modifica la cabecera del mensaje a tuxedo
//  memset(CabeceraInput,0x20,100); //Cabecera inpoout
//  sprintf(CabeceraInput,"%.6sX+000003988<?xml version=\"1.0\" encoding=\"UTF-8\"?><TRANSACTION idCopy=\"%.8s\">",
//          Trml,TxName);
  //TAMAÑO ES 264 + MENSAJE
    
  //montar mensaje
//   sprintf(sendbuf,"%.*s<IN><CABMEN entity=\"0001\" terminal=\"%.6s\" oficina=\"0001\" location=\"ES\" nomTx=\"SMPMES02\" language=\"01\" codOper="" user=\"009001\" /><DATOS><TTSISMA>%.*s</TTSISMA></DATOS><CONTROL /></IN></TRANSACTION>",
//   strlen(CabeceraInput),CabeceraInput,Trml, len, Msg);

//LA CABECERA SE MONTA EN EL FTRAD03 Y SE PASA YA EL MSG CON CABECERA MONTADA

   sendlen=6+1+10+6+6+len;
   //sprintf(CabeceraInput,"%0*dA+00000%04d%.*s%.*s",6,5000,len+7,6,"593790",6,"8A9  0"); //PTASK0022158 - comentado
   //PTASK0022158 - INI
   fprintf(pFile, "-- Valor puesto TBASE >%d< -- \n", TBASE);
   sprintf(CabeceraInput,"%0*dA+00000%04d%.*s%.*s",6,TBASE,len+7,6,"593790",6,"8A9  0");
   fprintf(pFile, "-- Valor CabeceraInput >%.*s< -- \n", strlen(CabeceraInput), CabeceraInput);
   //PTASK0022158 - FIN

   sprintf(sendbuf,"%s%s", CabeceraInput, Msg);
//    sprintf(sendbuf,"%s", Msg);

 
   if (recvbuf ==(char *)NULL)
   {
      if((recvbuf=(char *)tpalloc("CARRAY",(char *)NULL,sendlen))==(char *)NULL)
      {
         fprintf(pFile,"Control 16.- H:%d-ERROR AllocBuffer. tpalloc recvbuf\n", SVMhijo); //PRJ0247167
         fflush(pFile);

         LiberarTux;
         return 0;
      }
   }
  fprintf(pFile,"Control 17.- H:%d-tpalloc de los buffer de entrada y salida OK\n", SVMhijo); //PRJ0247167
  fflush(pFile);
   
   return 1;
}

void LiberarTux()
{
   char comando[30];
   
   fprintf(pFile, "-- Función LiberaTux de pmanejTux --\n"); //PRJ0257005
   
   /* liberar licencia de tuxedo */
   fprintf(pFile, "Control 18.- H:%d-LiberarTux\n", SVMhijo); //PRJ0247167
   fflush(pFile);
   
   tpterm(); //instruccion tuxedo
   
   // tpterm libera el cliente y ya no es capaz de recuperarse, por lo que tiramos el pmanejTux para que se rearranque.
   sprintf(comando,"kill -9 %d", SVMpadre);
   fprintf(pFile, "Control 19.- H:%d-comando >%s<\n", SVMhijo, comando); //PRJ0247167
   fflush(pFile); 		 
   system(comando);
   fprintf(pFile, "Esto no debe salir pq ya esta muerto\n");
   fflush(pFile);   
}


int CommitRollback (char *Servicio)
{
   fprintf(pFile, "-- Función CommitRollback de pmanejTux --\n"); //PRJ0257005
   /* Hacemos commit */
   if(memcmp(Servicio,"COMMIT",6)==0)
   {
//      if(tpcommit(0)==-1)
//      {
//         fprintf(pFile,"H:%d-SrvIbmSun. CommitRollback. tpcommit. tperrno %i\n", SVMhijo, tperrno);
//         fflush(pFile);

//         return 0;
//      }
#if defined (TRAZA) && (TRAZA > 1)
     fechorsys(&tm);
     fprintf(pFile, "Control 20.- H:%d-%04d%02d%02d-%02d:%02d:%02d COMMIT\n", SVMhijo, 
	        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); //PRJ0247167
     fflush(pFile);

#endif	    
   }
   else 
   {
        fprintf(pFile, "No se hace tpabort\n");
//      if(tpabort(0)==-1)
//      {
//         fprintf(pFile,"H:%d-SrvIbmSun. CommitRollback. tpabort. tperrno %i\n", SVMhijo, tperrno);
//         fflush(pFile);
//
//         return 0;
//      }
#if defined (TRAZA) && (TRAZA > 1)
     fechorsys(&tm);
     fprintf(pFile, "Control 21.- H:%d-%04d%02d%02d-%02d:%02d:%02d ROLLBACK\n", SVMhijo, 
	        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); //PRJ0247167
     fflush(pFile);
#endif	  
   }
   return 1;
}

int LlamadaTuxedo(const char *TxName,const char *Msg,int *lenMsgSer)
{
   //si da error retorna 0, si va bien retorna 1
   
   long recvlen,sendlen;
   long CallFlags=0;
   int  Longitud;
   int  retorno=0;
   char comando[20];
	fprintf(pFile, "-- Función LlamadaTuxedo de pmanejTux --\n"); //PRJ0257005
   sendlen=strlen(sendbuf);
//xavi 
		fechorsys(&tm); //PRJ0247167
		fprintf(pFile, "Fecha y hora de envio - %04d%02d%02d-%02d:%02d:%02d\n", tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec); //PRJ0247167
		fflush(pFile); //PRJ0247167

   fprintf(pFile, " - - Mensaje enviado a tuxedo numope >%.6s< mensaje >%.4s< sendlen <%d> sendbuf <%s>\n", sendbuf +51, sendbuf +111, sendlen, sendbuf); //PRJ0247167
   if((tpcall((char *)TxName,sendbuf,sendlen,&recvbuf,&recvlen,CallFlags))==-1)
   {
      fprintf(pFile, " LlamadaTuxedo. tpcall. ERROR TUXEDO %s, errno(%i)\n",tpstrerror(tperrno),tperrno);
      //Añadido para Monitorizacion SISMA 
      fprintf(pFile,"TIPO SISTEMA – ACCESO A TUXEDO: error llamada a tuxedo tpcall tperrno %i\n", tperrno);
     
      fprintf(pFile, "1 Fecha y hora de recepcion - H:%d-LlamadaTuxedo. tpcall. ERROR TUXEDO %i\n" ,SVMhijo, tperrno); //PRJ0247167
      fflush(pFile);
 
      //LiberarTux; --------- 20160525 -- NO LIBERAR TUX PQ CREEMOS QUE LIBERA TODOS LOS HILOS
	  //--------------------------------- Y YA NO ES CAPAZ DE CONECTARSE
		  
	  retorno=0;
   }
   else
   {
      *lenMsgSer= recvlen;
	  retorno=1;
   }
   if(recvlen > 0)
   {
//#if defined (TRAZA) && (TRAZA > 1)
     fechorsys(&tm);
     fprintf(pFile, "2 Fecha y hora de recepcion - H:%d-%04d%02d%02d-%02d:%02d:%02d recvlen <%d> recvbuf >%s<\n", SVMhijo, 
	        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, recvlen, recvbuf); //PRJ0247167
	 fflush(pFile);
//#endif	  
   }
   return(retorno);
}

void limpia_tbs(tbsock *pp)
{
   memset(pp->nompadre , ' ' , sizeof(pp->nompadre));
   pp->num_ip=VACIO;
   pp->pidpadre=VACIO;
   pp->pidhijo=VACIO;
   pp->signieto=VACIO;
}

int dame_posisock(tbsock *psvmsock)
{
  int posic;

   for (posic=0 ; posic<MAX_REG_TBSOCK ; posic++)
   {
      if (SVMip==VACIO) return(posic);
   }
   return(-1);
}
int carga_s(tbsock *psvmsock)
{
  int ll,salir=0;

   for (ll=0 ; ll<MAX_REG_TBSOCK ; ll++)
   {
      limpia_tbs(psvmsock+ll);
   }
   return(salir);
}

void QuitarCar(char* cadena, int longi)
{
   int i=0;
   for(i=0; i<longi; i++)
   {
      if (  cadena[i] == '\0'
         || cadena[i] < 41
         || cadena[i] == 44
         )
         cadena[i] = ' ';
   }
}

