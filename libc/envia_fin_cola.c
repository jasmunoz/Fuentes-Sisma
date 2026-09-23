#include <signal.h>
#include <unistd.h>
#include "svm.h"
#include "nptpara.h"
#include <string.h>
extern void sigue();
int Pkill(int Ppid,int Psenal);

int envia_fin_cola(SVM *regsvm)
{
  typedef struct mensaje
  {
   long  tipo;
   char a[10];
  } mensaje;

  mensaje    msg;
  int retorno,q_id,tamanio;

 regsvm->estado=PARADA;
 if (regsvm->tippar=='N')
  return(0);

 //RAQUEL SE UNIFICA EL IF PARA TODOS LOS CL Y PMANEJ 
   if ((strncmp(regsvm->str_svm.svmproc.proceso , "cl", 2) == 0 ) ||
        (strncmp(regsvm->str_svm.svmproc.proceso , "pmanej", 6) == 0 ))
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr,"mata %s con senial especial USR1 con pid : %d\n",
   regsvm->str_svm.svmproc.proceso, regsvm->str_svm.svmproc.pid);
#endif
  if (regsvm->str_svm.svmproc.pid > 1)
   Pkill(regsvm->str_svm.svmproc.pid,SIGUSR1);
  else
#if defined (TRAZA) && (TRAZA > 1)
   fprintf(stderr,"Error : el pid no es correcto\n");
#endif
  return(0);
 }

 if (regsvm->tippar=='S')
 {
//aquellos que tengan en arranque.h, que tipo es S, se mueren, según la senal que 
 //tengan definida, y si no lo tienen, con un SIGALRM
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr,"pongo %c en proc %s\n",
   PARADA, regsvm->str_svm.svmproc.proceso);
#endif
  if (regsvm->str_svm.svmproc.pid > 1)
  {
   if (regsvm->senal == 0)
    retorno = Pkill(regsvm->str_svm.svmproc.pid,SIGALRM);
   else
    retorno = Pkill(regsvm->str_svm.svmproc.pid,regsvm->senal);
#if defined (TRAZA) && (TRAZA > 1)
   fprintf(stderr, "retorno del kill = %d\n", retorno);
#endif
  }
  return(0);
 }
//el resto de los servicios se paran con un mensaje de PARADA 
// a traves de la cola
 msg.tipo=TIPO_PARADA;
 memset(msg.a,'0',sizeof(msg.a));


 q_id = regsvm->str_svm.svmproc.idcolaIn;

 if (strncmp(regsvm->str_svm.svmproc.proceso , "precep16" , 8) == 0)
  tamanio = 100;
 else
  tamanio = sizeof(mensaje)-sizeof(msg.tipo);
 signal(SIGALRM, sigue);
 alarm(ESSNDCOL);

 retorno = msgsnd(q_id, &msg, tamanio ,0);
/***   fin del cambio   ***/

#if defined (TRAZA) && (TRAZA > 1)
 fprintf(stderr,"retorna el mensaje %d\n",retorno);
 fprintf(stderr,"tamano %d\n",tamanio);
#endif

 alarm(0);
 if (retorno == -1)
  return(1);
 else
  return(0);
}

int parar_subsis(SVM *psvm_par,char *para,int pos_subsis)
{
  int num=MAX_REG_SVM;
  int  err=0;
  int  len=0;
  SVM  *Psvm;

 len=4;
 if(!strncmp(para+2,"00",2))
  len=2;
 Psvm=psvm_par+num;
 while ( num > pos_subsis)
 {
#if defined (TRAZA) && (TRAZA > 100)
  fprintf(stderr,"Entro en parar subsis con %d y subsis=%s\n",pos_subsis,para);
  fprintf(stderr,"tipo =%c para=%s psvmsub=%s pid=%d num=%d\n", Psvm->svmtip,
  para,Psvm->pasubsis, Psvm->str_svm.svmproc.pid, num);
#endif


  if((Psvm->svmtip == TIP_PROCES) &&
    (strncmp(Psvm->pasubsis,para,len)==0))
  {
#if defined (TRAZA) && (TRAZA > 1)
   fprintf(stderr,"Parando=%s\n", Psvm->str_svm.svmproc.proceso);
#endif
   if (Psvm->str_svm.svmproc.pid > -1)
    err=envia_fin_cola(Psvm);
   else
    Psvm->str_svm.svmproc.pid = -2; 
  }

   Psvm--;
   num--;
 }
  return(0);
}

int parar_todo(SVM *psvm_par)
{
  int err=0,posi;
  SVM * Psvm;
  int num;

/* El supervisor esta en posicion 0 */
 (psvm_par)->estado=PARADA;
 if (psvm_par->str_svm.svmproc.pid > 1)
  Pkill(psvm_par->str_svm.svmproc.pid,SIGALRM);

 Psvm = psvm_par + MAX_REG_SVM;
 while ( Psvm > psvm_par)
 {
  if (Psvm->svmtip == TIP_PROCES)
   if (Psvm->str_svm.svmproc.pid > -1)
    err=envia_fin_cola(Psvm);
   else
    Psvm->str_svm.svmproc.pid = -2; 
  Psvm--;
 }
 return(0);
}
