#include "svm.h"
/*
   Funcion damecola:
    Recibe: psvm_par-> Puntero a SVM
            proce -> puntero al nombre del proceso 
            subsis -> subsistema
    Devuelve: -1 -> Si no encuentra el valor 
              int >= 0 -> El identificador de la cola pedida
*/
#include <string.h>

int damecola(SVM *psvm_par,char *proce,char *subsis)
{
 int  num=0;
 SVM  *Psvm;
 Psvm=psvm_par;
 
 printf("en damecola proce %s \n", proce);
 
 while (num < MAX_REG_SVM)
  {
   if ((Psvm->svmtip == TIP_PROCES) && 
       (strncmp(Psvm->str_svm.svmproc.proceso,proce,strlen(proce))==0))
    {
	   printf("en damecola subsis %s \n", subsis);
	   printf("en damecola Psvm->str_svm.svmproc.idcolaIn %d \n", Psvm->str_svm.svmproc.idcolaIn);
       if (subsis == NULL)
            return(Psvm->str_svm.svmproc.idcolaIn);
       else
         if (strcmp(Psvm->pasubsis,subsis)==0)
            return(Psvm->str_svm.svmproc.idcolaIn);
    }

   Psvm++;
   num++;
  }
 return (-1);
 
}

int damecolaO(SVM *psvm_par,char *proce,char *subsis)
{
 int  num=0;
 SVM  *Psvm;
 Psvm=psvm_par;
 
 while (num < MAX_REG_SVM)
  {
   if ((Psvm->svmtip == TIP_PROCES) && 
       (strncmp(Psvm->str_svm.svmproc.proceso,proce,strlen(proce))==0))
    {
       printf("Entra por if principal damecola0 \n");
       if (subsis == NULL)
       {
            printf("retorna cola <%d>\n", Psvm->str_svm.svmproc.idcolaOut);
            return(Psvm->str_svm.svmproc.idcolaOut);
       } else {
          printf("Entra por else cola <%d>\n", Psvm->str_svm.svmproc.idcolaOut);
         if (strcmp(Psvm->pasubsis,subsis)==0)
            return(Psvm->str_svm.svmproc.idcolaOut);
       }
    }

   Psvm++; 
   num++;
  }
 return (-1);
 
}
