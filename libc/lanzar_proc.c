#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include "cnetplus.h"
#include "svm.h"
#include "datagen.h"

int   encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int   espera_preparado(SVM * psvm_pro);
int   haz_demonio(char * proceso, int numero, char* fechaA);
char *itoa(int valor);

int lanzar_proc(char proce[NUM_PROCES][11], SVM * psvm)  
{
  int ii=0;
  int retorno = 0;
  int nn;
  int donde = 0;
  tm_dec tm; 
  char fechaAct[20];
       
 //coger fecha del sistema
 fechorsys(&tm);
 sprintf(fechaAct,"_%02d%02d%d_%02d",tm.tm_mday,tm.tm_mon,tm.tm_year,tm.tm_hour);
 printf("fecha calculada: %s\n", fechaAct);
 nn = 1;
 
 for(ii = 0; ii < NUM_PROCES; ii++)
 {
 // if((*proce[ii]) == NULL) ---- ORIGINAL
   if((proce[ii][0]) == 0)   
   {
    break;
   }

  if(ii>0 && !strcmp(proce[ii-1], proce[ii])) //si son iguales el anterior y el actual
  {
   espera_preparado(psvm+donde); //activa una alarma si esta en ESPERA
   donde ++;
   nn++;
  }
  else
  {
   donde = encuentra_svm(psvm, proce[ii], TIP_PROCES); // busca el proceso 
   nn=1;
  }

  retorno = haz_demonio(proce[ii], nn, fechaAct); //haz un hijo para ejecutar proce[ii]
  usleep(50000);
 }
 return(retorno);
}

int haz_demonio(char * proceso, int numero, char *fechaAct) //programa ejcutado en segundo plano
{
  pid_t pid;
  mode_t oldmode;
  char comando[1024];
  char ejecutable[1024];
  int retorno;
  char fichero[300]="";
  char num[2];
  
 // incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

 pid = fork();
 switch(pid)
 {
  case 0: //hijo
  {
	// Montamos el nombre del fichero segun la fecha actual
//#if defined (TRAZA) && (TRAZA > 1)
  printf("Lanzo %s %d\n", proceso, numero);
//#endif
   if(numero > 1)
   {
     printf("lanzar_proc.c numero > 1 - creo %s%strazas/%s%i_%s_1.traza\n",HOME,DIRDATOS, proceso, numero,fechaAct); //PRJ0247167
     sprintf(comando,"%s%strazas/%s%i_1.traza",HOME,DIRDATOS, proceso,numero);
     oldmode = umask(00000);
     freopen(comando,"w",stdout);
     freopen(comando,"a+",stdout);
     sprintf(comando,"%s%strazas/%s%i_2.traza",HOME,DIRDATOS, proceso, numero);
     freopen(comando,"w",stderr);
     freopen(comando,"a+",stderr);
     umask(oldmode);
	 
	 sprintf(ejecutable,"%s%sexec/%s",HOME,HOMENET, proceso);
	 
     fprintf(stderr, "ejecutable %s%sexec/%s%i",HOME,HOMENET, proceso, numero);
   }
   else
   {
     printf("lanzar_proc.c numero else - creo %s%strazas/%s_%s_1.traza\n",HOME,DIRDATOS, proceso,fechaAct); //PRJ0247167
     sprintf(comando,"%s%strazas/%s_1.traza",HOME,DIRDATOS, proceso);
     oldmode = umask(00000);
     freopen(comando,"w",stdout);
     freopen(comando,"a+",stdout);
     sprintf(comando,"%s%strazas/%s_2.traza",HOME,DIRDATOS, proceso);
     freopen(comando,"w",stderr);
     freopen(comando,"a+",stderr);
     umask(oldmode);
     sprintf(ejecutable,"%s%sexec/%s", HOME,HOMENET, proceso);
     fprintf(stderr, "ejecutable %s%sexec/%s",HOME,HOMENET, proceso);   
   }
   printf("lanzar_proc.c antes de hacer el execlp"); //PRJ0247167
   sprintf(num, "%i", numero);
   num[1]='\n';
   retorno=execlp(ejecutable, proceso, num, (char *) 0); // ejecuta el programa (el ejecutable y proceso es lo mismo, uno con / y otro sin ello, pero puedo poner lo mismo
													// y (char*) 0 es el argumento.
													//crea los servicios!
   printf("lanzar_proc.c despues de hacer el execlp"); //PRJ0247167
   exit(0);
  }
  case -1:
   return(-1);
  default:
   return(0); 
 }
}

int alarma_01;
void marca_alarma(int sig); 
int espera_preparado(SVM * psvm_pro)
{

 alarma_01 = 0;

 signal(SIGALRM, marca_alarma); 
 alarm(5);

 while(!alarma_01)
 {
  if(psvm_pro->estado == ESPERA)
   return(alarm(0));
 }

 return(0);
}

void marca_alarma(int sig)
{
 fprintf(stderr,"signal de alarma %d\n",sig);
 alarma_01 = 1;
}
