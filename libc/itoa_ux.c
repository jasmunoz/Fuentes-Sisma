#include <stdio.h>
#include <stdlib.h>

char *itoa_ux(int valor)
{
char *a;  
 a=(char *) malloc(11); 
 sprintf(a,"%d",valor);  
 free(a);
 return a;
}
