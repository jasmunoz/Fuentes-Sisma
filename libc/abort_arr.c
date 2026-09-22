#include <stdlib.h>
#include <signal.h>
#include "svm.h"
#include "colas.h"
#include "svmbines.h"
#include <errno.h> 

/*   El parametro tipo es el que nos indica lo que tenemos que eliminar del
     sistema:
        0 => Solo la SVM.
        1 => La SVM y las colas. 
        2 => Mata los procesos,la SVM y las colas.
*/
int   limpia_qsvm(SVM *psvm,char *para);
int   borrarSVM(char *nomsvm,char *pars);
int   busca_en_svm(SVM *psvm_par,int pos,int tipo);
int   no_exis_qsvm(SVM *psvm_par,int posi);
int Pkill(int Ppid,int Psenal);
int borrar_svm(int shmid);

//segun la entrada mato SVM, colas, proceso...
void salir(char *para,int tipo,int error,int shmid,SVM *psvm,char * nombresvm)
{
int err;

   switch (tipo)
     {
       case 2:
              /*
                Mato a los procesos que no se han descolgado
              */
       case 1:
                    err=limpia_qsvm(psvm,para);  //limpio el proceso y colas si no las usa nadie mas
#if defined (TRAZA) && (TRAZA > 1)
fprintf(stderr, "vuelvo de limpia_qsvm %d \n", err);
#endif
       case 0:
              if ((strcmp(para,NUCLEO) == 0) || (strcmp(para,TODO) == 0))
                   {
#if defined (TRAZA) && (TRAZA > 1)
fprintf(stderr, "voy a borrarSVM shmid <%d>\n",shmid);
#endif
                    err=borrarSVM(nombresvm,para);
        
#if defined (TRAZA) && (TRAZA > 1)
fprintf(stderr, "vuelvo de borrar_svm %d \n", err);
#endif
                   }
               exit(error);

     }
#if defined (TRAZA) && (TRAZA > 1)
fprintf(stderr, "vuelvo de salir %d \n", err);
#endif
    return;
}
//borrar svm
int borrar_svm(int shmid)
{
  int error;
  struct shmid_ds buf;

       if ((error = shmctl(shmid,IPC_STAT,&buf)) == -1)
         {
/*#if defined (TRAZA) && (TRAZA > 1)
          fprintf(stderr,"ERROR(%d): Imposible obtener status de la S.V.M.\n",errno);
#endif */
          printf("error al obtener el status\n");
          return (E_NOSVM);
         }

       if ((error = shmctl(shmid,IPC_RMID,&buf)) == -1) //destruir SVM
         {
/*#if defined (TRAZA) && (TRAZA > 1)
          fprintf(stderr,"ERROR(%d): Imposible eliminar la S.V.M. del sistema\n",errno);
#endif*/
          printf("error al eliminar\n");
          return (E_NOSVM);
         }

  return (0);
}
//borrar cola
void borra_cola(int q_id)
{
  int    err;
  struct msqid_ds buf_id;

  if (q_id < 0)
      return;

#if defined (TRAZA) && (TRAZA > 100)
  fprintf(stderr,"Voy a borrar la cola(%d)\n",q_id);
#endif
  if ((err = msgctl(q_id,IPC_RMID,&buf_id)) == -1)
    {
#if defined (TRAZA) && (TRAZA > 100)
    fprintf(stderr,"ERROR(%d): Imposible borrar la cola(%d) \n",errno,q_id);
#endif
    return;
   }

}

/* borra las colas si no las usa nadie mas, e inicializa procesos y subsistemas*/
int  limpia_qsvm(SVM *psvm,char *para)
{
 int err_pro=0,posi_pro=0;
 int err_sub=0,posi_sub=0;
 int err=0;
 int err_proc=0;
 int len=0;

  len=4;
  if (!strncmp(para+2,"00",2))
    len=2;

  posi_sub=MAX_REG_SVM;

/* si es parada total, no se para por subsistemas */
  if ((strncmp(para,NUCLEO,len)==0) || (strncmp(para,TODO,len)==0))
  {
/* Busca el ultimo proceso en la cola */
   err_proc=busca_en_svm(psvm,MAX_REG_SVM,TIP_PROCES);
   while (err_proc >=0) //desde el encontrado recorre todos y si son proceso, los mata
   {
    if ((psvm+err_proc)->svmtip == TIP_PROCES)
    {
     if((psvm+err_proc)->str_svm.svmproc.pid >= 0)
     {
      Pkill((psvm+err_proc)->str_svm.svmproc.pid,SIGKILL); //mata el proceso con la señal SIGKILL
      (psvm+err_proc)->str_svm.svmproc.pid = -1; //lo marca como que noe sta arrancado
     }
     if((psvm+err_proc)->str_svm.svmproc.idcolaIn != -1) // Si hay cola, la borra
     {
      borra_cola((psvm+err_proc)->str_svm.svmproc.idcolaIn);
     }
     if((psvm+err_proc)->str_svm.svmproc.idcolaOut != -1) // Si hay cola de salida, la borra
     {
      borra_cola((psvm+err_proc)->str_svm.svmproc.idcolaOut);
     }

     memset((psvm+err_proc), '\0', sizeof(SVM)); //inicializa
     (psvm+err_proc)->svmtip=NO_TIPO;
    }
    err_proc--;
   }
   return(0);
  }
//si no es parada total, paramos por subsistema
  while ((err_sub=busca_en_svm(psvm,posi_sub,TIP_SUBSIS)) >=0) //buscamos el último subsistema
    {
     posi_pro=MAX_REG_SVM;
     while ((err_pro=busca_en_svm(psvm,posi_pro,TIP_PROCES)) >0) //buscamos el proceso
       {
#if defined (TRAZA) && (TRAZA > 100)
         fprintf(stderr,"voy a Tratar subs(%s) para(%s) dif(%d) \n",
                        (psvm+err_pro)->pasubsis,
                        para,
                        strcmp(para,(psvm+err_pro)->pasubsis));
#endif
        if (strncmp(para,(psvm+err_pro)->pasubsis,len) ==0) //comprobamos si es el subsistema que pararemos
          {
#if defined (TRAZA) && (TRAZA > 100)
           fprintf(stderr,"voy a borrar subs  %s\n", (psvm+err_pro)->pasubsis);
#endif
           if ((psvm+err_pro)->str_svm.svmproc.pid < 0) //ya esta matado el proceso
             {
              posi_pro=err_pro;
              if ((err=no_exis_qsvm(psvm,posi_pro))==0)  //miro si algún proceso usa la cola, para borrarla o no
                    borra_cola((psvm+posi_pro)->str_svm.svmproc.idcolaIn); //borro la cola
              memset((psvm+posi_pro), '\0', sizeof(SVM)); //inicializo
              (psvm+posi_pro)->svmtip=NO_TIPO;
              posi_pro=err_pro-1;
             }
           else //hay que matar el proceso
             {
              fprintf(stderr,"debo matar %s %d\n",
                 (psvm+err_pro)->str_svm.svmproc.proceso,
                 (psvm+err_pro)->str_svm.svmproc.pid);
#if defined (TRAZA) && (TRAZA > 1)
#endif
              errno = 0;
              Pkill((psvm+err_pro)->str_svm.svmproc.pid,SIGKILL); //mato el proceso
              (psvm+err_pro)->str_svm.svmproc.pid = -1;
              posi_pro=err_pro;
             }
          }
        else //seguimos leyendo
          posi_pro=err_pro-1;
       }
     if (strncmp(para,(psvm+err_sub)->pasubsis,len) ==0) //inicializamos el subsistema que estamso parando
        {
          memset((psvm+err_sub), '\0', sizeof(SVM)); 
          (psvm+err_sub)->svmtip=NO_TIPO;
        }
     posi_sub=err_sub-1;
/*
     fprintf(stderr,"Fin del while posi_sub=%d  err_sub=%d\n", posi_sub,err_sub);
*/
    }
/*
fprintf(stderr,"fuera del while posi_pro %d\n", posi_pro);
*/
return(0); 
}
/*
*/
int no_exis_qsvm(SVM *psvm_par,int posi)
{
 int  num=0;
 SVM  *Psvm;

 Psvm=psvm_par;
 while (num <= MAX_REG_SVM)
       {
#define IDCOLAIN  str_svm.svmproc.idcolaIn
        if ((Psvm->IDCOLAIN==(psvm_par+posi)->IDCOLAIN) && (num != posi))
                return(num);
        Psvm++;
        num++;
       }
 return (0);

}
