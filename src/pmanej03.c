//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
/***************************************************************************
 PROGRAMA:  MANEJ
 DESCRIPCION:  Recibe el mensaje de la cola y crea un hijo para conectar con 
           el tuxedo y esperar su respuesta.
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/timeb.h>

#include "datagen.h"
#include "deffich.h"
#include "procesos.h"
#include "nptinci.h"
#include "svm.h"
#include "colas.h"
#include "svmsock.h"
#include "max_min.h"
#include "ftradreso.h"
//include con la ip y los puertos
// por ahora quitar #include "i_addr.h"

#define LONMSG_IN   4096
#define LONMSG_OUT  4096

#define PADRE 0
#define HIJO  1

/***  --------- prototipos de funciones  ----------------  ***/
static int inicio_pmanej_perm(char * nombre);
int ConecSVMaux(void **gen_psvm,int shmid);
int busca_proc_svm(SVM *psvm_par, char *proce, pid_t auxpid);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int damecola(SVM *psvm_par,char *proce,char *subsis);
void sigue();
int  fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
void fprintLog(char * pf, char * msg, ...);
int finalizar(int codigo);
int proceso_recep();
int dame_posisvmsock(tbsock *psvmsock); 
int prepara_socket();
void limpia_tbsock(tbsock *pp);
int Atiende_conexion();
int modifica_estado(char* preg_sock, char * estado);
char *itoa(int numero);
void cierre(int sig);
void finalH(int sig);
int Pkill(int Ppid,int Psenal); 


int flag_01;
/*** mensaje de comunicacion ****/
//PRJ0247167 modificado el tamaño de mensaje_comunicacion en los .h de su definicion
mensaje_comunicacion  msglin;
int long_leerlin = sizeof(msglin) - sizeof(msglin.tipo);

SVM    *psvm;
tbsock    *psvmsock;
char nomProg[50];
char subsis[5];
int rtrata_svm=0;
int posic_subsis; 
char coderror[4];
int qrecep,qmanej;  
char nombre_cola[9];
int posic; 

int MIPID;
NPSINCI  reginci;
int contCX=0;
char clavesock[5];
char Datomsg[max(LONMSG_IN,LONMSG_OUT)];
int enviar;

/****** sockets *****/
char portnume[5];
char num_ip[16];
unsigned short portnum=0;
struct sockaddr_in ser_addr;
int sfd; 


int main(int argc,char **argv)    /***     FUNCION PRINCIPAL     ***/
{
   int  pid;
   int  nsfd;
   struct sockaddr_in cli_addr;
   socklen_t Long_Cliente;
   int retorno; 
   char ch_numope[7];

   strncpy(nomProg, argv[0], strlen(argv[0]));
   if (atoi(argv[1]) > 1)
      strncpy(nomProg + strlen(argv[0]), argv[1], 1);

   signal(SIGHUP,SIG_IGN);
   signal(SIGINT,SIG_IGN);
   signal(SIGQUIT,SIG_IGN);
   signal(SIGTERM,SIG_IGN);

#if defined (TRAZA) && (TRAZA > 1) 
   fprintLog(nomProg, "inicio \n");
#endif

  retorno = inicio_pmanej_perm(argv[0]);
  if(retorno)
  {
#if defined (TRAZA) && (TRAZA > 1) 
      fprintLog(nomProg, "PROCESO FINALIZADO(%d)\n",retorno);
#endif
      fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic);
      finalizar(retorno);
      exit(retorno);
  }
 
  while (1)
  {
     //recibe el mensaje de la cola
     retorno = proceso_recep();
#if defined (TRAZA) && (TRAZA > 5) 	 
	 fprintLog(nomProg,"Numope >%.6s< recibimos mensaje de penvio >%s<\n", msglin.mensaje_linea + 22, msglin.mensaje_linea);  //PRJ0247167 - antes ponia 58 ahora 22
#endif	 
     if (retorno == -1) //error al leer de la cola
          exit(finalizar(-1));
     memcpy(ch_numope,msglin.mensaje_linea + 58, 6);
	 ch_numope[6]='\0';
	 
     flag_01 = PADRE;

     posic=dame_posisvmsock(psvmsock); //retorna la primera posicion vacia del socket
     if (posic == -1)
     {
#if defined (TRAZA) && (TRAZA > 1) 
        fprintLog(nomProg,"error dame_posisvmsock espero y reintento\n");
#endif	 
		usleep(100); //la primera vez espero un poco y lo reintento
		posic=dame_posisvmsock(psvmsock); //retorna la primera posicion vacia del socket
     }
     if (posic == -1)
     {      	  
#if defined (TRAZA) && (TRAZA > 1) 
        fprintLog(nomProg,"segundo error dame_posisvmsock\n");
		fgraba_incidencia_cp("014", ch_numope, "pmanej03", "svmsock");
#endif
     }
     else
     {
        SVMpadre=MIPID;
        strcpy(SVMnombre,argv[0]);
        if ((pid=fork())==-1) // error al crear el hijo
        {
#if defined (TRAZA) && (TRAZA > 1) 		
             fprintLog(nomProg,"error fork:%d\n",errno);
#endif			 
             strcpy(reginci.innomfic,"FORK");
             fgraba_incidencia_cp("909", NULL, NULL, reginci.innomfic);
             close(sfd);
             limpia_tbsock(psvmsock+posic);
        }
        else
        {
          if (pid==0) /* Hijo */
          {
             SVMhijo=getpid();
             SVMsig=SIGUSR1;
             flag_01 = HIJO;
             signal (SIGALRM, finalH);

             prepara_socket();
             retorno = Atiende_conexion();
             
             if (retorno<0)
             {
                 limpia_tbsock(psvmsock+posic);
             }
             shutdown(sfd,2);
             close(sfd);
             exit(0);
          }  /** codigo del hijo **/
          else /* Padre */
          {
		     fprintLog(nomProg,"soy padre\n");
			 usleep(1);
          }  /** codigo del padre **/
        } /** fork() **/
     }  /** if posic **/
  }  /** while infinito */
}

/***** 
FUNCION: CIERRE DEL HIJO
****/
void finalH(int sig)
{
#if defined (TRAZA) && (TRAZA > 10) 
   fprintLog(nomProg,"hijo>%d< finalizo hijo\n", SVMhijo);
#endif
   SVMsig=VACIO;
   limpia_tbsock(psvmsock+posic);
   shutdown(sfd,2);
   close(sfd);
   exit(0);
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

#if defined (TRAZA) && (TRAZA > 10) 
   if (flag_01==HIJO)
       fprintLog(nomProg,"Cierre del HIJO %d posic %d\n", SVMhijo, posic );
   if (flag_01==PADRE)
       fprintLog(nomProg,"Cierre del PADRE\n");
#endif

   if (flag_01==HIJO)
   {
      SVMsig=VACIO;
      limpia_tbsock(psvmsock+posic);
   }
   if (flag_01==PADRE)
   {
      for (pp=0;pp<MAX_REG_TBSOCK;pp++)
      {
        posic=pp;
#if defined (TRAZA) && (TRAZA > 10) 		
        fprintLog(nomProg,"mato hijo %d\n", SVMhijo );
#endif		
        Pkill(SVMhijo ,SVMsig);
        limpia_tbsock(psvmsock+pp);
      }
      if ((psvm+rtrata_svm)->estado==RECARGAR)
        exit(finalizar(-2)); /* Paro para rearranque */
      else
        exit(finalizar(0)); /* Orden de Cierrre */
   }
   exit(0);
        
		
}

//recibe mensaje de la cola
int proceso_recep()
{
  int retorno;

	fprintLog(nomProg," \n"); //PRJ0257005
	fprintLog(nomProg,"------------------- \n"); //PRJ0257005
#if defined (TRAZA) && (TRAZA > 5) 
   fprintLog(nomProg,"proceso_recep. Leo cola qmanej = %d \n", qmanej);
#endif
   memset(msglin.long_mensaje, '\0', 4);
   memset(msglin.mensaje_linea, '\0', long_leerlin -4);
     
   //PRJ0247167 - Revisado ampliación P48 - msgrcv no tiene problema con el tamaño del mesaje nuevo
   retorno = msgrcv(qmanej, &msglin, long_leerlin, 0, 0);
    
   if (retorno == -1)
   {
      strcpy(reginci.innomfic,"Leer_q");
      fgraba_incidencia_cp("242", NULL, NULL, reginci.innomfic);
      //Añadido para Monitorizacion SISMA 
      fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Lectura qmanej %d codigo de error 242 retorno %d<\n",qmanej,retorno);

#if defined (TRAZA) && (TRAZA > 1)
      fprintLog(nomProg, "Error LeerCola >%d<:\n", retorno);

#endif
      return(retorno);
   }
#if defined (TRAZA) && (TRAZA > 10)    
   //fprintLog(nomProg,"hemos leido de la cola qmanej retorno>%d<\n", retorno);
   //fprintLog(nomProg,"msglin.tipo>%d< msglin.long_mensaje>%.4s<\n", msglin.tipo, msglin.long_mensaje);
   
   //fhexdump(stderr, &msglin, retorno);
#endif
   return(retorno);
}

//envia mensaje por la cola
int proceso_envio()
{
  int retorno;
  int leidos; 
  char tipoM[5];
  int CodError;

#if defined (TRAZA) && (TRAZA > 10) 
   fprintLog(nomProg,"hijo>%d< proceso_envio. Envio por cola de lectura de precep_reso\n", SVMhijo);
#endif
    leidos=retorno;
#if defined (TRAZA) && (TRAZA > 10) 
    //fprintLog(nomProg, "Recibidos >%d< Bytes\n", enviar);
    //fhexdump(stderr, Datomsg ,enviar); 
#endif

   memcpy(tipoM, Datomsg+99, 4);
   tipoM[4]='\0';
   fprintLog(nomProg,"tipoM %s \n",tipoM);
   msglin.tipo = atoi(tipoM);

//DMND0036176 se añaden los mensajes de preautorizaciones
//PTASK000024640 - se une el tipo de mensaje 221 debajo del 220 y encima del 1220
   fprintLog(nomProg,"1.- msglin.tipo %i \n",msglin.tipo);
   switch(msglin.tipo)
   {
     case 200:
     case 1200:
       msglin.tipo=1210;
       break;
     case  220:
     case  221:
     case  1220:
       msglin.tipo=1230;
       break;
     case  420:
     case  1420:
       msglin.tipo=1430;
       break;
     case  510:
     case  500:
       msglin.tipo=1534;
       break;
     case  700:
     case 1100:
     case 5000:
     case 5200:
     case 5300:
     case 5400:
     case 5500:
     case 5600:
       msglin.tipo=1110;
       break;
     case 1804:
       msglin.tipo=1814;
       break;
     case 620:
     case 1120:
       msglin.tipo=1130;
       break;
     case 260:
       msglin.tipo=1230;
       break;
   }
   fprintLog(nomProg,"2.- msglin.tipo %i \n",msglin.tipo);
   memcpy (msglin.long_mensaje, Datomsg +12, 4);
   memcpy (msglin.mensaje_linea, Datomsg +17 , enviar-17);
#if defined (TRAZA) && (TRAZA > 10)    
   //fprintLog(nomProg, "hijo>%d< Mandamos por qrecep el msglin.tipo >%d<\n", SVMhijo, qrecep,msglin.tipo);
#endif
    fprintLog(nomProg,"Long de enviar >%i< msglin.long_mensaje >%d< , msglin.tipo >%d< , qrecep >%d< \n",enviar,msglin.long_mensaje,msglin.tipo,qrecep); //PRJ0247167
	fprintLog(nomProg,"Numope >%.6s< mensaje que envia >%s< \n",msglin.mensaje_linea + 22, msglin.mensaje_linea); //PRJ0247167
    //PRJ0247167 - Revisado ampliación P48 - msgsnd no tiene problema con el tamaño del mensaje nuevo
	retorno = msgsnd(qrecep, &msglin, enviar+4, IPC_NOWAIT);  
    CodError = errno;
    printf("Retorno funcion msgsnd retorno >%i< y errno >%d<\n", retorno, CodError); //PRJ0247167
    if (retorno == -1)
    {
#if defined (TRAZA) && (TRAZA > 1) 	 	
       fprintLog(nomProg, "Error al mandar mensaje por cola %i\n", qrecep);
#endif	   
       strcpy(reginci.innomfic,"Envia_q");
      //Añadido para Monitorizacion SISMA 
      fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio qrecep %d codigo de error 249 retorno %d<\n",qrecep,retorno);

       fgraba_incidencia_cp("249", NULL, NULL, reginci.innomfic);
    }
    return(retorno);
}

/**** inicializacion de las colas, svm y datos necesarios ***/
static int inicio_pmanej_perm(char * nombre)
{
  int queda;
  int retorno;

   signal(SIGCLD,SIG_IGN);
   signal(SIGALRM,SIG_IGN);
   signal(SIGUSR1,SIG_IGN);

   strncpy(subsis,nomProg+strlen(PMANEJ),2);
   strncpy(subsis+2, "01", 3);
   subsis[4]='\0';

   strcpy(coderror, "   ");
   strcpy(reginci.innompro,"MANEJ");
   strcat(reginci.innompro,subsis);

/* --------- se ata a la memoria compartida SVM  ------------ */
  //  int shmid;  
    
 //   retorno = crearSVMI((void *)&psvm,&shmid,1,N_SVM);
   // printf(" RAQUEL shmid linea 357 pmanej3 CrearSVM %i\n", shmid);
   //RAQUEL SE DEVUELVE A SU ESTADO ORIGINAL PARA UNIFICAR DESDE BBDD
	  retorno=conecSVM((void *)&psvm); //ORIGINAL
   if(retorno)
   {
#if defined (TRAZA) && (TRAZA > 1) 
     fprintLog(nomProg, "Error al conseguir svm \n");  
#endif
      strcpy(reginci.innomfic,"CON_SVM");
      strcpy(coderror, "011");
      return   (-1); //supervis no arranca sin svm
   }

//svm para los svmsock de hijo/padre
   retorno=ConecSVMaux((void *)&psvmsock,IdSvmSock);
   if(retorno)
   {
#if defined (TRAZA) && (TRAZA > 1) 
       fprintLog(nomProg, "Error al conseguir svmsock \n");  
#endif
       strcpy(reginci.innomfic,"SVM_SOC");
       strcpy(coderror, "011");
       return   (-1); //supervis no arranca sin svm
   }

   rtrata_svm=busca_proc_svm(psvm, nombre, -1);
   if(rtrata_svm < 0)
   {
#if defined (TRAZA) && (TRAZA > 1) 
      fprintLog(nomProg, "ERROR no estoy en svm \n");  
#endif
      strcpy(reginci.innomfic,"BUSCA_P");
      strcpy(coderror, "011");
      return   (-1); //supervis no arranca sin svm
   }

//buscar subsistema
   posic_subsis=encuentra_svm(psvm,subsis,TIP_SUBSIS);
   if (posic_subsis < 0)
   {
#if defined (TRAZA) && (TRAZA > 1) 
      fprintLog(nomProg, "ERROR no encuentra svm \n");  
#endif
      strcpy(reginci.innomfic,"ENC_SVM");
      strcpy(coderror,"011");
      return   (-1); //supervis no arranca sin svm
   }

//cola de mensajes de MANEJ
   qmanej=(psvm+rtrata_svm)->str_svm.svmproc.idcolaIn;
   if(qmanej == -1)
   {
#if defined (TRAZA) && (TRAZA > 1) 
      fprintLog(nomProg, "error crea cola  QMANEJ\n");  
#endif
      strcpy(reginci.innomfic,reginci.innompro);
      reginci.innomfic[0]='Q';
      strcpy(coderror,"009");
      //Añadido para Monitorizacion SISMA 
      fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Crear QMANEJ codigo de error %d<\n",coderror);

      return   (-1); //supervis no arranca sin svm
   }

   strcpy(nombre_cola,PRECEP);
   strcat(nombre_cola, subsis);
   nombre_cola[8] = '\0';
   qrecep = damecola(psvm, nombre_cola,NULL);
   if(qrecep == -1)  
   {
#if defined (TRAZA) && (TRAZA > 1) 
      fprintLog(nomProg, "error crea cola  QRRECP\n");  
#endif
      strcpy(coderror, "009");
      //Añadido para Monitorizacion SISMA 
      fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Crear QRRECP codigo de error %d<\n",coderror);

      return(-1); //supervis no arranca sin svm
   }
#if defined (TRAZA) && (TRAZA > 10) 	    
   fprintLog(nomProg, "todas colas encontradas\n");
#endif   

   signal(SIGALRM,sigue);
   alarm(TIMEWAIT*2);
   /* graba su pid en la SVM */  
   //queda=alarm(0);
   //fprintLog(nomProg,"RAQUEL PMANEJ03 queda >%d<\n", queda);   
   MIPID= getpid();
   (psvm+rtrata_svm)->str_svm.svmproc.pid=MIPID;
   (psvm+rtrata_svm)->estado=ESPERA;

#if defined (TRAZA) && (TRAZA > 1)
   fprintLog(nomProg,"Espero seqal\n");
#endif
   pause();
   queda=alarm(0);
   
   if(!queda)
   {
      (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
#if defined (TRAZA) && (TRAZA > 1) 
       fprintLog(nomProg, "No me han despertado seqal\n");
#endif
       strcpy(reginci.innomfic,"      ");
       strcpy(reginci.innomfic,"ALARM");
       strcpy(coderror, "251");
       //Añadido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA – MUERTE DE PROCESO: codigo de error >%.3s<\n",coderror);

       return(-1); // supervis no arranca sin svm
   } 
   (psvm+rtrata_svm)->estado=TRABAJO;
   (psvm+rtrata_svm)->senal=SIGUSR1;
#if defined (TRAZA) && (TRAZA > 1)   
    fprintLog(nomProg, "despierta\n");
#endif	

   /***   este proceso se cierra con esta segnal especial   ***/
   signal(SIGUSR1,cierre);
   return(0);
}

//////////////////////////////////////////////////////////////////////////
/** inicializ ala comunicacion con el socket ***/
int prepara_socket()
{
   if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      fprintLog(nomProg,"hijo>%d< error al establecer el socket %d\n",SVMhijo,errno);
      perror("error en socket");
      strcpy(reginci.innomfic,"SOCKET");
      fgraba_incidencia_cp("909", NULL, NULL, reginci.innomfic);
      return(-1);
   }

   /* recupera la ip y el puerto a variables internas */
   strncpy(num_ip, (psvm+posic_subsis)->str_svm.svmpars.panip, 15);
   num_ip[15]='\0';

   /* formateamos el puerto */
   strncpy(portnume, (psvm+posic_subsis)->str_svm.svmpars.paportnume, 4);
   portnume[4]='\0';
   portnum = (unsigned short) atoi(portnume);
   memset(&ser_addr, 0, sizeof(ser_addr));
   ser_addr.sin_port=htons(portnum);

   /* inicializamos los datos del socket */
   ser_addr.sin_family=AF_INET;
   ser_addr.sin_addr.s_addr=inet_addr(num_ip);

}//fin socket
 
//conecta el socket, 
int Atiende_conexion(int tiempo)
{
  int conectado;
  int retorno;
  int tipo;
  char tama[5];
  int queda;
  struct sigaction act;

//esta señal es para indicar que el hijo ha acabado, pero este no tiene hijo

  signal (SIGALRM, finalH);
  

  //activamos la alarma, para que si en 3seg no se ha contestado se muera el proceso.
  alarm(3);
  tipo=1;
  retorno = setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&tipo, sizeof(tipo));
  
  if (retorno)
  {
#if defined (TRAZA) && (TRAZA > 1) 	   
        fprintLog(nomProg,"hijo>%d< retorno de setsockopt es >%d< errno >%d< ", SVMhijo,retorno, errno);
#endif        
        strcpy(reginci.innomfic,"setsockopt");
        fgraba_incidencia_cp("909", NULL, NULL, reginci.innomfic);
        return(-1);
  }

  memset(clavesock,' ',5);
  memcpy(clavesock,subsis,4);
  clavesock[5]='\0';
  conectado=0;
  while (conectado == 0)
  {
    retorno = connect(sfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
		
    if (retorno == -1)
    {
      if (contCX == 0)
      {
	     fprintLog(nomProg,"hijo>%d<  retorno de connect>%d< errno >%d<\n", SVMhijo, retorno, errno);
		 fprintLog(nomProg,"hijo>%d< Datos conexion>%s:%s<\n", SVMhijo,num_ip, portnume);
         fgraba_incidencia_cp("700", NULL, NULL, "hijoconn");
      }
      contCX++;
      if ( contCX == 61) //para que grabe solo una incidencia al minuto
          contCX=0;
      sleep(1);
    }
    else
    {
     // socket conectado.
       retorno= modifica_estado(clavesock, "01");
       conectado=1;
    }
  }

  tipo = ser_addr.sin_addr.s_addr;
  if (tipo < 1)
       tipo*=-1;
	   

  SVMip=tipo;

//enviar mensaje por el socket
  strncpy(tama, msglin.long_mensaje, 4);
  tama[4]='\0';
  conectado=atoi(tama);
  memcpy(Datomsg, msglin.mensaje_linea, atoi(tama));

 //xavi
  printf("Numope >%.6s< datos a enviar a pmanejTux <%s>\n", Datomsg + 22, Datomsg);
// 
  retorno = EnviaSock(sfd,2,Datomsg,conectado);

  if (retorno == -1)
  {
      strcpy(reginci.innomfic,"Leer_q");
      strncpy(reginci.incoderr,itoa(retorno),6); 
      return(-1);
  }

//leemos lo que nos mande el socket
  retorno = LeerSock(sfd,0,Datomsg,LONMSG_IN);
  enviar=retorno;
#if defined (TRAZA) && (TRAZA > 5) 	   
  //fprintLog(nomProg,"hijo>%d< leemos del socket num bits >%i<\n", SVMhijo, retorno);
  //fprintLog(nomProg,"NUMOPE >%.6s< hijo>%d< leemos respuesta del socket retorno>%d<\n", Datomsg +39, SVMhijo, retorno); //PRJ0247167 - antes ponia 66 y ahora 39
#endif  
  printf("Datos recibidos de pmanejTux: retorno >%d< Numope >%.6s< Datomsg >%s<\n", retorno, Datomsg +39, Datomsg); //PRJ0247167
  printf("Respuesta 463 >%.2s< y 2979 >%.2s<\n", Datomsg +462, Datomsg +2978); //PRJ0247167

  if (retorno == -1)
  {
        strcpy(reginci.innomfic,"LeerSoc");
        strncpy(reginci.incoderr,itoa(retorno),6); 
        retorno=modifica_estado(clavesock, "02");
        return(-2);
  }
  if (retorno == 0) //socket se ha cerrado
  {
        strcpy(reginci.innomfic,"LeerSocCerrado");
        strncpy(reginci.incoderr,itoa(retorno),6);
        retorno=modifica_estado(clavesock, "02");
        return(-2);
  }
 
    //envia por la cola
  retorno=proceso_envio();
  if (retorno == -1)
  {
        strcpy(reginci.innomfic,"Enviar cola");
        strncpy(reginci.incoderr,itoa(retorno),6);
        return(-3);
  }
  alarm(0);
 
  //hay que meter un cierre para liberar el hijo
  cierre(0);
  return(0);
}


#include "fEstadoSockt.c"
