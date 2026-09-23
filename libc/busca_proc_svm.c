#include "svm.h"
#include <stdio.h>
#include <string.h>
/*
   Funcion busca_proc_svm:
    Devuelve: NO_EXIS_SVM=-32000 -> Si no encuentra el valor de auxpid
              int >= 0 -> La posicion donde esta el valor buscado
*/
int busca_proc_svm(SVM *psvm_par, char *proce, pid_t auxpid)
{
  int  num=0;
  SVM  *Psvm;
 
 Psvm=psvm_par; 

 while(num < MAX_REG_SVM)
 {	
  if(Psvm->svmtip == TIP_PROCES &&
    Psvm->str_svm.svmproc.pid == auxpid &&
    !strcmp(Psvm->str_svm.svmproc.proceso, proce))
   return(num);
  Psvm++;
  num++;
 }
 return(NO_EXIS_SVM);
}
