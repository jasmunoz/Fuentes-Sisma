#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include "cforitno.h"  
#include "svm.h"
#include "colas.h"
#include "svmsock.h"
#include "deffich.h"
#include "nptpara.h"
#include "nptpars.h"
#include "nptbine.h"
#include "datagen.h"
#include "arranque.h"
#include "llaves1.h"


void tracea_proces(SVM *psvm, int posi, int nproc);
void tracea_subsis(SVM *psvm,int posi);
int  encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int  desconec_svm(void *pun_svm);
int espera_resp(SVM *psvm,char *proce);
void sigue_sup(int sig);
void sigue(int sig);
int ya_bien(SVM *,char *proce,int mialarm);
void carga_subs(SVM *Psvm,NPSPARS *reg);
int Pkill(int Ppid,int Psenal);
int ConecSVMaux(void **gen_psvm,int shmid);
void fechorsys(struct tm_dec *tmPtr);
void recorre_svm(SVM *p1svm);
int nspaces (char *c,int n);
int haz_demonio(char * proceso, int numero, char* fechaA);
void desconectaTodo();

#ifndef SIGNULL
#define SIGNULL 0
#endif
#define NUMMSG  10

char nomProg[50];
int alarma=0;
tm_dec tm;
char  Msubs[20];
char  Mbine[20];
char claveCONE[13];

typedef struct {
   char fecha[14];
   char resto [6];
}Testructura;
Testructura estructura;


int main(int argc, char **argv)
{
  int        err,posi_sup;
  SVM        *psvm;
  tbsock     *Psvmsock;
  tbbine     *Psvmbine;
  char        fechaAct[20];
  char        fechaAct1[20];
  char        fichero[200];
 

// incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");
  
//ficheros de trazas
   fechorsys(&tm);
   sprintf(fechaAct,"_%02d%02d%04d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);
   sprintf(fichero, "%s%s/superviW_1.traza",HOME,DIRTRAZA);
   freopen(fichero, "w", stdout);
   sprintf(fichero, "%s%s/superviW_2.traza",HOME,DIRTRAZA);
   freopen (fichero, "w", stderr); 
   strncpy(nomProg, argv[0], strlen(argv[0]));
 
   fprintf(stdout,"stdout empieza supervis \n");
   fflush(stdout);

   signal(SIGCLD,SIG_IGN);  //TERMINACION DEL PROCESO HIJO, EN LINUX
   
   //inicializo Msubs y Mbine
   strncpy(Msubs,"                    ",20);
   strncpy(Mbine,"                    ",20);

    //* --------- se ata a la memoria compartida SVM  -------------

   err=conecSVM((void *)&psvm);//ORIGINAL
  
   switch (err)
   {
     case E_NOSVM :
          fprintf(stdout,"E_NOSVM\n");
		  fflush(stdout);
          desconectaTodo();		  
          exit(15);
     case 0       :  
          break;
     default    : 
          fprintf(stdout,"conecSVM >%d<\n",err);
		  fflush(stdout);
		  desconectaTodo();
          exit(15);
   }
   fprintf(stdout,"conectado al psvm\n");
   fflush(stdout);

   posi_sup=encuentra_svm(psvm,"",TIP_SUPER);
   if (posi_sup <0)
   {
       fprintf(stdout,"ERROR No hay supervisor en SVM \n",err);
	   fflush(stdout);
	   desconectaTodo();
       exit(0);
   }

   //rellenamos datos del SVN del superusuario
   (psvm+posi_sup)->str_svm.svmsupe.farran.dd=tm.tm_mday;
   (psvm+posi_sup)->str_svm.svmsupe.farran.mm=tm.tm_mon;
   (psvm+posi_sup)->str_svm.svmsupe.farran.ss=tm.tm_year/100;
   (psvm+posi_sup)->str_svm.svmsupe.farran.aa=tm.tm_year%100;
   (psvm+posi_sup)->str_svm.svmsupe.harran.hh=tm.tm_hour;
   (psvm+posi_sup)->str_svm.svmsupe.harran.mm=tm.tm_min;
   (psvm+posi_sup)->str_svm.svmsupe.harran.ss=tm.tm_sec;
   (psvm+posi_sup)->str_svm.svmsupe.harran.dd=tm.tm_mls;
    
	//sprintf(fechaAct,"_%02d%02d%d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);

   (psvm+posi_sup)->str_svm.svmsupe.timesp=10; 
   (psvm+posi_sup)->str_svm.svmsupe.pid = getpid();

   if((psvm+posi_sup)->estado!=PARADA)
      (psvm+posi_sup)->estado=TRABAJO;
   else
   {
      (psvm+posi_sup)->str_svm.svmsupe.pid = -1;
      (psvm+posi_sup)->estado=NOUSO;
      (psvm+posi_sup)->svmtip=NO_TIPO;
      err=desconec_svm(psvm);
	  desconectaTodo();
      exit(0);
   }
   err=ConecSVMaux((void *) &Psvmsock,IdSvmSock);
   err=ConecSVMaux((void *) &Psvmbine,IdSvmBine);

   while (1)
   {
   
     /////////// REVISA SI TIENE QUE CREAR NUEVO FICHERO DE TRAZAS ////////////////////////////  
     //coger fecha del sistema
      fechorsys(&tm);
     //formateo fecha
      sprintf(fechaAct1,"_%02d%02d%04d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);
      if (strncmp(fechaAct1, fechaAct, 12)) //si las fechas son distintas
      {		 
		 strncpy(fechaAct,  fechaAct1, 12);		 
         fclose(stdout);
		 fclose(stderr);

		 sprintf(fichero, "%s%s/superviW_1.traza",HOME,DIRTRAZA);
		 freopen (fichero, "w", stdout);
		 sprintf(fichero, "%s%s/superviW_2.traza",HOME,DIRTRAZA);
		 freopen (fichero, "w", stderr);  
        
      }
#if defined (TRAZA) && (TRAZA > 5)
      fprintf(stdout,"------------------------%04d-%02d-%02d %02d:%02d:%02d-----------------", 
	  tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	  fflush(stdout);
#endif	     
       signal(SIGALRM,sigue_sup);
#if defined (TRAZA) && (TRAZA > 5)
       fprintf(stdout, "pongo alarma %d segundos\n",(psvm+posi_sup)->str_svm.svmsupe.timesp);
	   fflush(stdout);
#endif
       alarm((psvm+posi_sup)->str_svm.svmsupe.timesp); 
       pause();
#if defined (TRAZA) && (TRAZA > 5)	   
       fprintf(stdout, "continuo tras la pausa\n"); 
	   fflush(stdout);
#endif	   
       signal(SIGALRM,SIG_IGN);
       fechorsys(&tm);
       if ((psvm+posi_sup)->estado == PARADA)
       {
	       fprintf(stdout, "estado == PARADA\n");
		   fflush(stdout);
           (psvm+posi_sup)->str_svm.svmsupe.pid = -1;
           (psvm+posi_sup)->estado=NOUSO;
           (psvm+posi_sup)->svmtip=NO_TIPO;
           err=desconec_svm(psvm);
           exit(0);
       }

	   ///// PROCESO QUE SUPERVISA LOS SERVICIOS
       recorre_svm(psvm);
	   //fprintf(stdout, "retorna de recorre_svm(psvm)\n");

     }
}

/* recorre el svm, y i encuentra:
	subsistema:si ha habido cambio en nptpars, comprueba si le afecta
                    diferido
 	proceso:
*/
void recorre_svm(SVM *p1svm)
{
  int num=0;
  int cambio=0;
  int err;
  int nproc;
  char proc_ant[11];
  
  ///////////////// REVISAR SI HAY CAMBIOS EN PARS ////////////////////////

  //recuperamos la ultima fecha de modificacion de NPTPARS para ver si se ha modificado el NPTPARS

  err=tratar_fich("NPTPARS",FECMODIF,&estructura,NULL);
  
#if defined (TRAZA) && (TRAZA > 1)
  if(err)
  {
     fprintf(stdout,"Error(%d) al conseguir status de subs\n",err);
	 fflush(stdout);
  }
#endif

  if(strncmp(Msubs,estructura.fecha, 14) != 0) //comprueba que Msubs no sea igual a la última fecha de moficiación
  {
      strncpy(Msubs,estructura.fecha,14);
      Msubs[14]='\0'; 
      cambio=1;
  }
  else
  {
      cambio=0;
  }

  //////////////////////  RECORRER SVM PARA PROCESOS Y PARS /////////////////////////
  
  memset(proc_ant, ' ', 11);
  nproc = 1;
  
  while(num < MAX_REG_SVM)
  {    
    switch((p1svm+num)->svmtip)
    {
       case TIP_SUBSIS:
         if(cambio)
            tracea_subsis(p1svm,num); //actualizamos el subsistema
             break; 
		 
       case TIP_PROCES:
#if defined (TRAZA) && (TRAZA > 99)
         //fprintf(stdout, "num=%2d  PROCESO=%.11s \n",num, (p1svm+num)->str_svm.svmproc.proceso);
#endif

         if(!strncmp(proc_ant, (p1svm+num)->str_svm.svmproc.proceso, 11)) //si es el mismo proceso aumento el numero
           nproc++;
         else
         {
             nproc = 1;
             strncpy(proc_ant, (p1svm+num)->str_svm.svmproc.proceso, 11);
         }
         tracea_proces(p1svm,num, nproc); //arranca el proceso
         break; 
       case NO_TIPO   :
         break; 
    }
    num++;
  }
  
}

void tracea_subsis(SVM *pun_svm,int posi)
{
  char  comando[250];
  NPSPARS reg_parsub;
  int err;
  SVM *Psvm;

  Psvm=pun_svm+posi;

#if defined (TRAZA) && (TRAZA > 99)
   fprintf(stdout, "tracea subsis\n");
   fflush(stdout);
#endif

/* copia pasubsis+pasubapl, el campo de svn comprende los dos  */
  strncpy(reg_parsub.pasubsis,Psvm->pasubsis,sizeof(Psvm->pasubsis));
  err = tratar_fich("NPTPARS",IGUAL,&reg_parsub, &reg_parsub); 
  if (err)
  {
       sprintf(comando,"parada %s",Psvm->pasubsis);
#if defined (TRAZA) && (TRAZA > 1)
       fprintf(stdout,"ERROR(%d): ", err);
       fprintf(stdout,"Imposible leer el fichero de subsistema\n");
	   fflush(stdout);
#endif
      // paramos el subsistema porque no existe en la tabla
       system(comando);
       return;
  }

/* cargamos el subsistema porque ha cambiado */
   carga_subs(Psvm,&reg_parsub);
}

void tracea_proces(SVM *pun_svm, int posi, int nproc)
{
 pid_t oldpid;
 char oldestado;
 int err,err1;
 char proce[NUM_PROCES][11];
 SVM *Psvm;
 char fechanum[50];
 char comando[250];
 int ppadre;
 char fechaAct[20];

  Psvm=pun_svm+posi;
  
  // incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

#if defined (TRAZA) && (TRAZA > 10) 
  fprintf(stdout, "\ntraceo proces %s pid %d\n",Psvm->str_svm.svmproc.proceso, Psvm->str_svm.svmproc.pid);
  fflush(stdout);
#endif

  if (Psvm->str_svm.svmproc.pid > 0)
  {
     err=Pkill(Psvm->str_svm.svmproc.pid,SIGNULL); //mandamos esta señal para acceder al PID
#if defined (TRAZA) && (TRAZA > 1) 	 
     if (err)
	 {
       fprintf(stdout, "%s despues del kill err=%d\n", Psvm->str_svm.svmproc.proceso, err);
	   fflush(stdout);
	 }
#endif	 
  }
  else
    if (Psvm->str_svm.svmproc.pid == PID_PDTE)
        err=-1;
     else
        err=0;

  oldestado=Psvm->estado;
#if defined (TRAZA) && (TRAZA > 10)  
  fprintf(stdout, "err=%d  errno=%d oldestado=%c\n", err, errno, oldestado);
  fflush(stdout);
#endif  

/* 
   Devuelve err=-1 si ha habido error y:
       errno=ESRCH si el proceso no existe 
       errno=EPERM si el proceso es de otro usuario y no tenemos permisos
*/

  if ((err==-1) && (errno==ESRCH))
  {
     strcpy(proce[0],Psvm->str_svm.svmproc.proceso);
       *proce[1] = '\0';


        //DESACTIVARLO EN NPTCONE PARA QUE SALGA EL AVISO POR PANTALLA
        strncpy(claveCONE, "0001", 4);
        strncpy(claveCONE+4, proce[0], 8);
        claveCONE[13]='\0';
        err = tratar_fich("NPTCONE", DESACTIVAR, NULL, claveCONE); 

		
		if (oldestado != PARADA) //SOLO INTENTO ARRANCAR SI NO SE ESTA INDICANDO QUE EL SERVICIO ESTA PARADO CON FINALIZAR(-1)
		{
        /***   para guardar las trazas   ***/
        /***   coger fecha del sistema   ***/
           fechorsys(&tm);
		   
           sprintf(fechanum , "%04d%02d%02d%02d%02d%02d",
                            tm.tm_year,tm.tm_mon,tm.tm_mday,
                            tm.tm_hour,tm.tm_min,tm.tm_sec);

           sprintf(comando,"mkdir %s%sdat/trazas/sp%s",HOME,HOMENET,fechanum);  
           system(comando);

           sprintf(comando, "mv %s%s/dat/trazas/%s*.traza  %s%sdat/trazas/sp%s",
               HOME,HOMENET,proce[0],HOME,HOMENET,fechanum);
           system(comando);

          oldpid=Psvm->str_svm.svmproc.pid;
 

#if defined (TRAZA) && (TRAZA > 1)
          fprintf(stdout, "comando >%s<\n", comando);
          fprintf(stdout, "\na lanzar proces %s pid %d\n",
             Psvm->str_svm.svmproc.proceso, Psvm->str_svm.svmproc.pid);
          fprintf(stdout, "nproc >%d<\n", nproc);
		  fflush(stdout);
#endif
		
        /***   lanza el proceso caido   ***/
        /***                            ***/		
           Psvm->str_svm.svmproc.pid = -1;
           fechorsys(&tm);
           sprintf(fechaAct,"_%02d%02d%d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);
        		
           err=haz_demonio(proce[0], nproc, fechaAct); //ejecuto el proceso
#if defined (TRAZA) && (TRAZA > 1)
           fprintf(stdout, "Vuelve de haz_demonio %s\n", proce[0]);
		   fflush(stdout);
#endif
           err=espera_resp(Psvm,proce[0]);//comprueba si se esta OK

#if defined (TRAZA) && (TRAZA > 1)
           fprintf(stdout, "despues de espera_resp err=%d\n", err);
		   fflush(stdout);
#endif
           if (err)
           {// alguien me ha despertado antes de tiempo, es decir, err <> 0
              signal(SIGALRM,sigue_sup);
              alarm(5+err);
              pause();
              Psvm->estado=oldestado;
              Psvm->str_svm.svmproc.pid = oldpid;
              alarm(0);
           }
           else
           {//se ha despertado, 
              if ((Psvm)->str_svm.svmproc.pid > 0)
              {
                 // hay que activar el módulo
                 err = tratar_fich("NPTCONE", ACTIVAR, NULL, claveCONE);
                 err1=Pkill((Psvm)->str_svm.svmproc.pid ,SIGNULL); //comprobar el estado del pid
                 if (err1 != -1)
                   err1=Pkill((Psvm)->str_svm.svmproc.pid ,SIGALRM); //le manda la señal SIGALRM para despertarlo
              }
           }
        }
	}
}

int espera_resp(SVM *p2svm,char *proce)
{
 int queda,yaesta;
   printf("entrada espera_resp\n");
   signal(SIGALRM,sigue);
   alarm(TIMEWAIT);
   alarma=0;
   alarma=1; // Cambio prueba Trazas REVISAR *********************************************
   printf("entrada bucle ya_bien\n");
   while ((yaesta=ya_bien(p2svm,proce,alarma))<0) {} 
   queda=alarm(0);
   return(yaesta);
} 

void sigue_sup(int sig)
{
#if defined (TRAZA) && (TRAZA > 5)
 fprintf(stdout,"signal de super %d\n",sig);
 fflush(stdout);
#endif
}

void sigue(int sig)
{
   alarma=1;
#if defined (TRAZA) && (TRAZA > 5)
 fprintf(stdout,"signal en sigue %d\n",sig);
 fflush(stdout);
#endif
}

int ya_bien(SVM *p3svm, char *proce, int mialarm)
{
  int donde, ii;
  
 ii=0; 
 printf("proce <%s> p3svm->str_svm.svmproc.pid <%d> proceso <%s>\n", proce, p3svm->str_svm.svmproc.pid,  p3svm->str_svm.svmproc.proceso);
 printf("Proceso que levanta: >%s<\n", proce);
 if(!mialarm) //mialarm=0
 {
    if(p3svm->str_svm.svmproc.pid != -1) 
    {
      if(p3svm->estado==ESPERA) 
        return(0);
      else
        return(-1);  
    }
    else
      return(-1);
 }
 return(1);
}

//
void desconectaTodo()
{
    char claveCONE[13];
  
//desactiva todos los procesos
    strncpy(claveCONE, "0001", 4); 
    strncpy(claveCONE+4, "TODOS", 5); 
    strncpy(claveCONE+9, "   ", 3);
    claveCONE[13]='\0';
    tratar_fich("NPTCONE", DESACTIVAR, NULL, claveCONE);
	fprintf(stdout,"Desconectado todo\n");
	fflush(stdout);
}





