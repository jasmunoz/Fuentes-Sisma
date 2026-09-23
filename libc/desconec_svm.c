#include <stdio.h>
#include "svm.h"
int desconec_svm(void *pun_svm)
{
  int retorno=0; 

 retorno = shmdt((char *) pun_svm);  

  return(retorno);
}
