#include "svm.h"
/*
   Funcion busca_en_svm:
    Busca una entrada que sea del tipo "tipo" en la variable SVM a partir de la posicion
    indicada por "pos" hacia atras
    Devuelve: NO_EXIS_SVM=-32000 -> Si no encuentra el valor 
              int >= 0 -> La posicion donde esta el valor buscado
*/
int busca_en_svm(SVM *psvm_par,int pos,int tipo)
{
 int  num=pos;
 SVM  *Psvm;

 Psvm=psvm_par+pos;
 while (num >= 0)
       {
        if (Psvm->svmtip == tipo) 
                return(num);  
        Psvm--;
        num--;
       }
 return (NO_EXIS_SVM);
}
