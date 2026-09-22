/**********************************************/
#include "svm.h"
#include "deffich.h"
#include "datagen.h"

int cuenta_reg(char *fich)
{
  char regreso[2048]; 
  int cuantos;
  int retorno;

retorno=tratar_fich(fich, PRIMERO, &regreso, NULL);
if(retorno)
{
 if(retorno != 101)
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr, "Error %d en PRIMERO Fichero fich\n",retorno);   
#endif
  return(-2);
 }
}

cuantos=0;
while(!retorno)
{
  cuantos++;
  retorno=tratar_fich(fich, SIGUIENTE, &regreso, NULL);
 }

return(cuantos);
}
