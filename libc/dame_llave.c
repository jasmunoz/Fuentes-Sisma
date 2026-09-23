#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cnetplus.h" 
#include <sys/timeb.h>

#define FICHERO_LLAVE   "/sf/sisma/dat/ficareas.lla"

key_t dame_llave(char clave)
{
  key_t llave;
  
 /* Me devuelve una llave */ 

  llave=ftok(FICHERO_LLAVE,clave); 
 
  return (llave);
}

