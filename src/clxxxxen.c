//PRJ0257005 - Revisado tokenizacion
/***************************************************************************
 PROGRAMA:  CLIENT
 DESCRIPCION: Proceso servidor de la red.
             - Se conecta a las colas de intercambio de mensajes con SISMA   
         - Abre el canal.
         - Comunica a la red la direccion del canal.
             - Se pone en disposicion de aceptar conexiones.
         - Bucle infinto
         - Si recibe una peticion crea un hijo para que la atienda  
             - Proceso hijo       
             - Espera la respuesta en la cola QMANEJ
             - Envia la respuesta al destino    
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <sys/timeb.h>
#include "netwin.h"
#include "datagen.h"
#include "deffich.h"
#include "procesos.h"
#include "nptinci.h"
#include "svm.h"
#include "colas.h"
#include "max_min.h"
#include "nptpars.h"

#define LONMSG_IN   4096
#define LONMSG_OUT  4096
#define PRUCARGA
#define USURBUFSIZE 20  //05-08-15. Alberto.

/***  --------- prototipos de funciones  ----------------  ***/
int  fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
void sigue();
void cierre();
void cierr2();
void reconecta();
void fprintLog(char ficheroPlantilla[50], char* format, ...);
int  damecola(SVM *psvm_par,char *proce,char *subsis);
int  encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int  busca_proc_svm(SVM *psvm_par,char *proce,pid_t auxpid);
int  InicializarSocket(char * clavesock);
int  modifica_estado(char* preg_sock, char * estado);
void fechorsys(struct tm_dec *tmPtr); 
char *itoa(int numero);
void conectar_socket();
int     finalizar(int codigo); 

/**--------------funciones de manejo de la SVM-----------***/
int qmanej;  
NPSINCI  reginci;
SVM    *psvm;
int posic_subsis; 
int sfd; 
int rtrata_svm=0;
int opcion;
int retorno; 
int contCX=0;

char Csubapl[3];
char nomProg[50];
char subsis[5];
char subsistema_general[3];
char MIPRECEP[30];
char coderror[4];
int  ret_modif;
int conectado=0;
int vuelveAleer=0;


typedef struct
{
   long  tipo;
   char  datos[4096];
} mensaje;
mensaje    msgnuc;
int long_msgnuc = sizeof(msgnuc) - sizeof(msgnuc.tipo);


/******* para obtener la clave de la tabla NPTSOCK ***/
char clavesock[5];
int  pid;
struct sockaddr_in cli_addr;
struct sockaddr_in ser_addr;

int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
  size_t cli_addr_len;
  int queda; 
  int tiempo, tipo;
  int enviar;
  char Datomsg[max(LONMSG_IN,LONMSG_OUT)];
  tm_dec tmPtr;
  //char ch_013[13];
  char ch_03[3]; 
  char *bufferAux;
  
  
   signal(SIGCLD,SIG_IGN); //sinonimo de SIGCHLD en Linux
   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);
   signal(SIGALRM,SIG_IGN);
 
   //asignar el nombre
   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);

   fprintLog(nomProg, "Inicio clxxxxen\n");

   strncpy(subsistema_general,argv[0]+2,2);
   strncpy(Csubapl, argv[0]+4, 2);
   subsistema_general[2] = '\0';
   Csubapl[2] = '\0';

   strcpy(subsis, subsistema_general);
   strcpy(subsis + 2, Csubapl);
   strcpy(MIPRECEP,PRECEP);
   strcat(MIPRECEP,subsistema_general);

    memset(clavesock,' ',5);
    memcpy(clavesock,subsistema_general,2);
    memcpy(clavesock +2,Csubapl,2);
    clavesock[5]='\0';
	
   sprintf(reginci.innompro,"CLIEN%s",subsistema_general);
#if defined (TRAZA) && (TRAZA > 1)    
   fprintLog(nomProg, "nombre %s\n", reginci.innompro);
#endif
 
  //RAQUEL DEVOLVEMOS EL ORIGINAL PORQUE YA BUSCA COMO SI HUBIERA SOLO UNA ENTIDAD
    retorno=conecSVM((void *) &psvm); // ORIGINAL
 

   if (retorno != 0)
   {
#if defined (TRAZA) && (TRAZA > 1) 
    fprintLog(nomProg,"error al conseguir svm\n");  
#endif
    strcpy(reginci.innomfic,"CON_SVM");
    strcpy(coderror, "011");
    fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
    exit(-1);
   }

   rtrata_svm=busca_proc_svm(psvm,argv[0], -1);
   if (rtrata_svm < 0)
   {
#if defined (TRAZA) && (TRAZA > 1) 
    fprintLog(nomProg,"eror al buscar proceso en svm \n");  
#endif
    strcpy(reginci.innomfic,"BUSCA_P");
    strcpy(coderror, "011");
    fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
    exit(-1);
   }

   posic_subsis=encuentra_svm(psvm,subsis,TIP_SUBSIS);
   if (posic_subsis < 0)
   {
#if defined (TRAZA) && (TRAZA > 1) 
    fprintLog(nomProg,"error al buscar posicion en svm \n");  
#endif
    strcpy(reginci.innomfic,"ENC_SVM");
    strcpy(coderror, "011");
    fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
    exit(-1);
   }
   tiempo =(psvm + posic_subsis)->str_svm.svmpars.patrepet; //tiempo de respuesta a peticiones

   qmanej=(psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
   if (qmanej == -1)
   {
#if defined (TRAZA) && (TRAZA > 1) 
    fprintLog(nomProg,"error crea cola QMANEJ%s\n",subsistema_general);  
#endif
    sprintf(reginci.innomfic,"QMANEJ%s",subsistema_general);
    strcpy(coderror, "009");
    
    //Añadido para Monitorizacion SISMA 
    fprintLog(nomProg,"TIPO SISTEMA - ACCESO A LAS COLAS: Tipo Creacion QMANEJ%s codigo de error >%.3s<\n",subsistema_general, coderror);

    fgraba_incidencia_cp(coderror, NULL,  NULL, reginci.innomfic);
    exit(-1);
   }
#if defined (TRAZA) && (TRAZA > 1) 
    fprintLog(nomProg,"SVM TODO BIEN\n");  
#endif
   

   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   /* graba su pid en la SVM */   
  //    queda=alarm(0);
  // fprintLog(nomProg,"RAQUEL CLEN queda >%d<\n", queda);
   (psvm+rtrata_svm)->str_svm.svmproc.pid=getpid();
   (psvm+rtrata_svm)->estado=ESPERA;
   pause();
   queda=alarm(0);
   if (queda==0)
   {
    (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
#if defined (TRAZA) && (TRAZA > 1) 
    fprintLog(nomProg,"No me han despertado seqal\n");
#endif
    strcpy(reginci.innomfic,"      ");
    strcpy(reginci.innomfic,"ALARM");
    strcpy(coderror, "251");
    //Añadido para Monitorizacion SISMA 
    fprintLog(nomProg,"TIPO SISTEMA - MUERTE PROCESO: codigo de error >%.3s<\n",coderror);

    fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
    exit(-1);
   } 
   (psvm+rtrata_svm)->estado=TRABAJO;
   (psvm+rtrata_svm)->senal=SIGUSR1;

   //   este proceso se cierra con esta segnal especial  
   signal(SIGUSR1,cierre);

   //   este proceso se para desde el clxxxxre con esta segnal especial  
   //signal(SIGUSR2,cierr2);
   
   //  en vez de pararse por la se�al del clre se reconecta. Asi no es necesario supervis
   signal(SIGUSR2, reconecta);
   

   ///////////////////////////////conectamos el socket /////////////////////////////////
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"conectar socket\n");
#endif
   conectar_socket();

  //inicialmente el socket estar� cerrado, por ahora asteriscado.
  //EL CLRE LO DEJA CERRADO EN INICIO PARA QUE LO ABRA EL PRIMER TEST OK
  //ret_modif = modifica_estado(clavesock, "02");
  
  //////////////////////////////////////////////////////////////////////////////
  ///////////////////////// bucle para leer de la cola /////////////////////////
  for(;;)
  {
    vuelveAleer=0;
    fprintLog(nomProg,"esperando que alguien mande datos por la cola %i\n", qmanej);

   //ponemos el tiempo a cero
   retorno = LeerCola(qmanej,0,Datomsg,LONMSG_OUT,0);
   
   if(vuelveAleer)
   {
	   fprintLog(nomProg, "vuelveAleer\n");
	   continue;
   }
   else
   {
      fprintLog(nomProg,"retorno leer cola >%i<\n", retorno);
      if (retorno == -1)
      {
        strcpy(reginci.innomfic,"Leer_q");
        strncpy(reginci.incoderr,itoa(retorno),8);
        break;
      } 
   }

   enviar=retorno;

   fechorsys(&tmPtr);
   //sprintf(ch_013, "%.2d%.2d%.2d%.2d%.2d%.2d",
   //         tmPtr.tm_year, tmPtr.tm_mon, tmPtr.tm_mday, tmPtr.tm_hour, tmPtr.tm_min, tmPtr.tm_sec);


   /* Es necesario pasar la len(enviar) a binario 2 bytes, y estos dos bytes */	
   /* se introducen al inicio del mensaje(Datomsg).                          */	   	
   bufferAux=(char *)malloc(LONMSG_OUT);
   memcpy(bufferAux,Datomsg,enviar);
   getLenBin(ch_03,enviar); //Se obtiene la len del mensaje en binario.
   memcpy(Datomsg,ch_03,2);
   memcpy(Datomsg+2,bufferAux,enviar);
   enviar+=2; //Sumamos los dos bytes a la len total del msg.
   free(bufferAux);
  
   
      
#if defined (TRAZA) && (TRAZA > 1) 
   fprintLog(nomProg, "A enviar >%d< bytes\n", enviar);
   fprintf(stderr, "%.4d%.2d%.2d %.2d:%.2d:%.2d\n", 
         tmPtr.tm_year, tmPtr.tm_mon, tmPtr.tm_mday, tmPtr.tm_hour, tmPtr.tm_min, tmPtr.tm_sec);
   fhexdump(stderr, Datomsg, enviar); 
   fflush(stderr);
#endif

   if(vuelveAleer)
   {
	   fprintLog(nomProg, "vuelveAleer\n");
	   continue;
   }
   else
   {

     fprintLog(nomProg,"antes Sleep tipo mensaje <%.4s>\n", Datomsg+2);
//     if (strncmp(Datomsg+2,"1430",4) == 0 || strncmp(Datomsg+2,"1210",4) == 0)
//     {
//        sleep(30);
//     }
     fprintLog(nomProg,"despues Sleep tipo mensaje <%.4s>\n", Datomsg+2);

     retorno = EnviaSock(sfd,2,Datomsg, enviar);
     fprintLog(nomProg,"tras enviasock retorno %i  sfd %i\n", retorno, sfd);
     if (retorno == -1)
     {
        strcpy(reginci.innomfic,"Leer_q");
        strncpy(reginci.incoderr,itoa(retorno),8);
        break;
      }
    }
  } //fin for
}

//Funcion para conectar el socket
void conectar_socket()
{
   //libera la conexion si hubiera
   if(sfd != -1)
   {
      close(sfd);
      fprintLog(nomProg, "shutdown liberando >%d<\n", sfd);
   }
   sfd = 0;

#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"abrir socket\n");
#endif
   while (conectado == 0)
   {

   /* antes del socket llamamos a la nueva funcion InicializarSocket */
     retorno = InicializarSocket(clavesock);
     if (retorno != 0)
     {
        fprintLog(nomProg,"error inicializar socket %d\n");
        strcpy(reginci.innomfic,"SOCKET");
        strcpy(coderror, "909");
        fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
        exit(-1);
     }

  /******* este de conectar ****/
  }//while
  //return(0);

}

/***** 
FUNCION: Inicializar Socket
****/
int InicializarSocket(char * clavesock)
{
  char portnume[6];
  char num_ip[16];
  unsigned short portnum=0;
  unsigned long addr;
  struct hostent * php;
  int cab=0;
  
  
  char send_buffer[USURBUFSIZE]; 
  int return_write =0; 


  if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg,"error llamada socket %d\n",errno);
      perror("error en socket");
#endif
      return(-1);
  }

   /* recupera la ip y el puerto a variables internas */
   strncpy(num_ip, (psvm + posic_subsis)->str_svm.svmpars.panip, 15);
   num_ip[15]='\0';
   strncpy(portnume, (psvm + posic_subsis)->str_svm.svmpars.paportnume, 5);
   portnume[5]='\0';

   /* formateamos el puerto */
   portnum = (unsigned short) atoi(portnume);
   ser_addr.sin_port=htons(portnum);

   /* formateamos la ip */
   //php = gethostbyname(num_ip);

   /* inicializamos los datos del socket */
   ser_addr.sin_family=AF_INET;


   ser_addr.sin_addr.s_addr= inet_addr(num_ip);
#if defined (TRAZA) && (TRAZA > 1)   
   fprintLog(nomProg,"socket addr>%s< y puerto>%s<\n", num_ip , portnume);
#endif   

   opcion=1;

   retorno = setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&opcion, sizeof(opcion));

   if (retorno)
   {
       fprintLog(nomProg,"retorno de setsockopt es %d ", retorno);
       fprintLog(nomProg,"con error %d\n", errno);
       return(-1);
   }

   retorno = connect(sfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
   if (retorno == -1)
   {
       if (contCX == 0)
       {
         fprintLog(nomProg, "Error de connect PRINCIPAL retorno >%d<\n",retorno);
         fprintLog(nomProg,"error llamada socket %d\n",errno);
         //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO SISTEMA - CONEXION TCP-IP CON REDSYS: Tipo Conexion IP >%s< PUERTO >%s<codigo de error>%d<\n",num_ip ,portnume, errno);

       }
       contCX++;
       if ( contCX == 11)
          contCX=0;
       sleep(1);
   }
   else
   {
      fprintLog(nomProg,"CONECTADO PRINCIPAL sfd %i\n", sfd);
	  conectado=1; 
	  
    //  return_write = sendUserData(sfd,send_buffer,Csubapl, "01", "1"); //ORIGINAL
        return_write = sendUserData(sfd,send_buffer,subsistema_general,Csubapl, "01", "1"); //SUBSISTEMA 

      if (return_write<=0)
      {
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA - CONEXION TCP-IP CON REDSYS: Tipo Envio sfd >%i< codigo de error>%d<\n",sfd, return_write);

        fprintLog(nomProg,"ERROR NO SE HA ENVIADO LA TRAMA DE ID DEL SOCKET\n",return_write);
      }  
      else{
        fprintLog(nomProg,"OK SE HA ENVIADO LA TRAMA DE ID DEL SOCKET\n",return_write);    
        fhexdump(stderr, send_buffer, USURBUFSIZE); //si va bien pintamos el msg  
		fflush(stderr);
      } 
   }

   return(0);

}  // fin inicializar socket


/***** 
FUNCION: CIERRE del proceso 
****/
void cierre()
{
  int err;
  err=0;
  errno=0;

#if defined (TRAZA) && (TRAZA > 1) 
   fprintLog(nomProg,"estoy en cierre por parada, y voy a cerrar el socket\n");
#endif
   err= close(sfd);
#if defined (TRAZA) && (TRAZA > 1) 
   perror("He cerrado el socket");
   fprintLog(nomProg,"He cerrado el socket. err=%d , errno = %d\n",err,errno);
#endif

   (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
   (psvm+rtrata_svm)->estado=NOUSO;
   exit(0);

} // fin funcion cierre

void cierr2()
{
  int err;
  err=0;
  errno=0;

#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"estoy en cierre2 por signal desde hermano, y voy a cerrar el socket\n");
#endif
   err= close(sfd);
#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"Cerrado el socket err=%d errno = %d\n",err,errno);
#endif

exit(finalizar(-2)); // Paro para rearranque

} // fin funcion cierre

void reconecta()
{
   fprintLog(nomProg,"Reconecta por sign del clxxxxre.\n");
   conectado = 0;
   conectar_socket();
   fprintLog(nomProg,"Reconectado\n"); 
   vuelveAleer=1;
} 


#include "fEstadoSockt.c"
