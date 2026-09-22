#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#include "cnetplus.h"
#include "svm.h"
#include "colas.h"
#include "svmsock.h"
#include "deffich.h"
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
#include "procesos.h"
#include "netwin.h"
#include "nptdife.h"
#include "arranque.h"
#include "llaves1.h"


 
int todos_fuera(SVM *p2svm,int mialarm,char *para);
void sigue1();  
int espera_respuesta(SVM *p1svm,char *para);
int Cuentasub(SVM *Psvm,char *Para);
void rpad(char *bufer,char c,int lon);
int conecSVM(void **gen_psvm);
int encuentra_svm(SVM *psvm_par,char *aux,int tipo);
int parar_todo(SVM *psvm_par);
int parar_subsis(SVM *psvm_par,char *para,int pos_subsis);
int borrar_svm(int shmid);
int busca_en_svm(SVM *psvm_par,int pos,int tipo);
void salir(char *para,int tipo,int error,int shmid,SVM *psvm, char * nomsvm);

int alarma=0;
int ParadaTotal=0;
char nombre[5];
NPSIPCS  *reg_ipcs;
int carga_ipcs(char * indice);


int main(int argc,char *argv[])
{
  char       keypars[5];
  char       claveCONE[13];
  int        shmid=0;
  int        err;
  SVM        *psvm;

  struct shmid_ds buf;

  char  comando[200];
  //RAQUEL AÑADIDO PARA ARRANQUES POR TABLAS
   char indice_ipcs[2]; //siempre sera 00 para cargar toda la tabla NPTIPCS
   int i; //para recorrer la tabla IPCS y buscar
 
   strncpy(indice_ipcs, "00",2);
   indice_ipcs[2]='\0';
   i=0;
   
   if ((err = carga_ipcs(indice_ipcs)) != 0)
   {
    #if defined (TRAZA) && (TRAZA > 1)   
       fprintf(stderr,"error carga tabla NPTIPCS \n");
    #endif	   
   }
   for (i=0 ; i<50 ; i++)
   {
	  if ((strncmp(tabla_ipcs[i].ipcsnsvm,"NSVM",4)==0))
      {		
       strncpy(nombre, tabla_ipcs[i].ipcsnomb,5);  
       break;            
      }   
   }

  if (argc < 2)
    {
     strcpy(keypars,TODO); /* Para parar todos los subsistemas */
     ParadaTotal=1;
    }
  else
       if (strlen(argv[1]) == 1)
       {
        strcpy(keypars,"0");
        strcat(keypars,argv[1]); /* Para parar el subsistema especificado */
       }
       else
        strcpy(keypars,argv[1]);

   rpad(keypars,'0',4);

   if (strcmp(keypars,NUCLEO) == 0)
      {
       memset(keypars,0,sizeof(keypars));
       strcpy(keypars,TODO);
       ParadaTotal=1;
      }

    fprintf(stderr,"SISTEMA %s y Parada =%d\n",keypars,ParadaTotal);

   err=conecSVM((void *)&psvm);
   
   switch (err)
    {
     case E_NOSVM :
#if defined (TRAZA) && (TRAZA > 1)
                   fprintf(stderr,"SISTEMA NO ARRANCADO %s\n",keypars);
#endif
                   exit(15);
     case 0       :  
                   if (!ParadaTotal)
                      ParadaTotal=Cuentasub(psvm,keypars);
                   if (ParadaTotal)
                   {
/* Se asume que el supervisor esta en la posicion 0 */
                    memset(keypars,0,sizeof(keypars));
                    strcpy(keypars,TODO);
                    psvm->estado=PARADA;
                    break;
                   }
                   err=encuentra_svm(psvm,keypars,TIP_SUBSIS);
#if defined (TRAZA) && (TRAZA > 1)
                   fprintf(stderr,"encuentra %d\n",err);
#endif
                   if (err < 0)
                   {
#if defined (TRAZA) && (TRAZA > 1)
                    fprintf(stderr,"SUBSISTEMA NO ARRANCADO %s\n",keypars);
#endif
                    exit(16);
                   }
#if defined (TRAZA) && (TRAZA > 1)
                   fprintf(stderr,"encontrado %s\n",(psvm+err)->pasubsis);
#endif
                   break; 
       default    :
#if defined (TRAZA) && (TRAZA > 1)
                   fprintf(stderr,"ERROR EN LA PARADA NO SVM %d\n",err);
#endif
                   exit(11);
    }

#if defined (TRAZA) && (TRAZA > 1)
   fprintf(stderr,"Voy a parar err= %4d\n",err);
   fprintf(stderr,"Voy a parar ParadaTotal= %d\n",ParadaTotal);
#endif
   if (ParadaTotal)
      err=parar_todo(psvm);
   else
      err=parar_subsis(psvm,keypars,err);

#if defined (TRAZA) && (TRAZA > 1)
   fprintf(stderr,"Despues de parar err= %d\n",err);
#endif

   if (err)
   {
#if defined (TRAZA) && (TRAZA > 1)
    fprintf(stderr,"En parar err= %d\n",err);
#endif
    err=espera_respuesta(psvm,keypars);
      //  salir(keypars,2,0,shmid,psvm,N_SVM);
          salir(keypars,2,0,shmid,psvm,nombre);
   }

   err=espera_respuesta(psvm,keypars); 

   if (err)
      {
#if defined (TRAZA) && (TRAZA > 1)
        fprintf(stderr,"En espera dentro del if err= %d\n",err);
        /* El SIGALRM no se ha disparado por mi despertador */
        fprintf(stderr,"No es mi alarma,alguien me ha levantado antes\n");
#endif
        err=todos_fuera(psvm,0,keypars);
        if (err)
           {
#if defined (TRAZA) && (TRAZA > 1)
             fprintf(stderr,"En todos_fuera err= %d\n",err);
#endif
           //  salir(keypars,2,0,shmid,psvm,N_SVM);
		    salir(keypars,2,0,shmid,psvm,nombre);
           }
      }

   if (ParadaTotal)
    {
     err = borrar_svm(IdSvmBine);
#if defined (TRAZA) && (TRAZA > 1)
     if (err != 0)
     {
         fprintf(stderr,"ERROR(%d): al borrar SVM Bine\n",errno);
     }
#endif

     err = borrar_svm(IdSvmSock);
#if defined (TRAZA) && (TRAZA > 1)
     if (err != 0)
     {
         fprintf(stderr,"ERROR(%d): al borrar SVM Sock\n",errno);
     }
#endif
    }

//desactiva los procesos
    strncpy(claveCONE, "0001", 4);
    if (!strcmp(keypars,TODO))
       strncpy(claveCONE+4, "TODOS", 5);
    else
       strncpy(claveCONE+4, keypars, 5);
    strncpy(claveCONE+9, "   ", 3);
    claveCONE[13]='\0';
    err = tratar_fich("NPTCONE", DESACTIVAR, NULL, claveCONE);

  salir(keypars,2,0,shmid,psvm,nombre);
  exit(0);

}


int espera_respuesta(SVM *p1svm,char *para)
{
  int queda,yaesta=0;

 signal(SIGALRM,sigue1);
 /*
 alarm(TIMEWAIT);
 */
 alarm(5);
 alarma=0;
 while ((yaesta=todos_fuera(p1svm,alarma,para)) !=0) {}
 queda=alarm(0);
 if(yaesta)
  return (queda);
 return (0);
}

void sigue1() {
   alarma=1;
}

int todos_fuera(SVM *p2svm,int mialarm,char *para)
{
 int donde,ii,tipo;
 static char proceso[9] = "";

  ii=MAX_REG_SVM;

/* Esto es lo nuevo */
  tipo=TIP_PROCES;
/*
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr, "dentro de todos_fuera con mialarm = %d\n", mialarm);
#endif
*/
  while (mialarm == 0)
   {
/*
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr, "bucle exterior mialarm %d\n", mialarm);
#endif
*/
    while (ii>=0)
     {
      donde = busca_en_svm(p2svm,ii,tipo);
      if (donde < 0) 
        ii--;
      else
      {
       if((strcmp(para,TODO)==0) || 
          (strcmp(para,(p2svm+donde)->pasubsis)==0))
       {
        if ((p2svm+donde)->str_svm.svmproc.pid < 0)
        {
         (p2svm+donde)->str_svm.svmproc.pid = -2;
         ii--;
        }
        else
        {
#if defined (TRAZA) && (TRAZA > 1)
         if(strcmp((p2svm+donde)->str_svm.svmproc.proceso, proceso))
         {
          fprintf(stderr, "encontrado %s pid: %d en puesto %d\n",
          (p2svm+donde)->str_svm.svmproc.proceso,
          (p2svm+donde)->str_svm.svmproc.pid, ii);
          strcpy(proceso, (p2svm+donde)->str_svm.svmproc.proceso);
         }
#endif
/*
#if defined (TRAZA) && (TRAZA > 1)
         fprintf(stderr, "devuelve -1 todos_fuera con mialarm = %d\n", mialarm);
#endif
*/
         return(-1);
        }
       }
       else
        ii--;
      }
     } /* while de ii */

    if (tipo==TIP_SUPER)
        mialarm=1;

    tipo=TIP_SUPER; 
    ii=MAX_REG_SVM;
   } /* while de alarm */
/* Fin nuevo */
/*
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr, "devuelve 0 todos_fuera con mialarm = %d\n", mialarm);
#endif
*/
 return(0); 
}
int Cuentasub(SVM *Psvm,char *Para)
{
 int ii;
 int len;
 int cuantos;

  len=4;
  if (!strncmp(Para+2,NUCLEO,2))
     len=2; 

  ii=0;
  cuantos=0;  
  while ((ii<=MAX_REG_SVM) && (!cuantos))
   {
     if (((Psvm+ii)->svmtip == TIP_SUBSIS)       &&
         (strncmp(Para,(Psvm+ii)->pasubsis,len)) &&
         (strncmp(NUCLEO,(Psvm+ii)->pasubsis,len)))
        cuantos++; 
     ii++;
   }
  return(!cuantos);
}
