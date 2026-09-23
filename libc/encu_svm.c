/*
   Funcion encuentra_svm:
    Devuelve: NO_EXIS_SVM=-32000 -> Si no encuentra el valor ni sitio en SVM
              int >= 0 -> La posicion donde esta el valor buscado
              int <= 0 -> La posicion siguiente donde insertar en negativo
*/
#include "svm.h"
#include <stdlib.h>
int encuentra_svm(SVM *psvm_par,char *aux,int tipo)
{
  int  num=0;
  SVM  *Psvm;
 
 Psvm=psvm_par;

 while (num < MAX_REG_SVM+1)
 {  
  if(tipo == Psvm->svmtip)
  {	 
   switch(tipo)
   {
    case TIP_SUBSIS:
     if(!strncmp(Psvm->pasubsis, aux, 4))
      return (num);
     break;
    case TIP_PROCES:
     if(!strncmp(Psvm->str_svm.svmproc.proceso, aux, 10))
	 {
      return (num);
	 }
     break;
    case TIP_SUPER:
     return(num);
    case NO_TIPO:
     return(num);
    default:
     break;
   }
  }
  Psvm++; 
  num++;
 }
 num=0;
 Psvm=psvm_par;
 while(Psvm->svmtip!=NO_TIPO)
 {
  if(num==MAX_REG_SVM)
   { 
   return(NO_EXIS_SVM);
   }                                            //@gus
  Psvm++;
  num++;
 }
 return(-1*num);
}

int Dame_info_svm(SVM *psvm_par,char *aux,int tipo_B,int quedan)
{
  int  num=0;
  int  Iaux=0;
  SVM  *Psvm;
 
 switch(tipo_B)
 {
  case B_NOMBRE:
    break;
  case B_COLA:
  case B_PID:
    Iaux=atoi(aux);
    break;
  default:
    return (-1);
 }
 Psvm=psvm_par;
 while (num < quedan)
 {
  if(TIP_PROCES == Psvm->svmtip)
  {
   switch(tipo_B)
   {
    case B_NOMBRE:
      if(!strncmp(Psvm->str_svm.svmproc.proceso, aux, 10))
       return (num);
    case B_COLA:
      if (Psvm->str_svm.svmproc.idcolaIn == Iaux)
       return (num);
    case B_PID:
      if (Psvm->str_svm.svmproc.pid == Iaux)
       return(num);
    default:
      break;
   }
  }
  Psvm++;
  num++;
 }
 return(-1);
}
