#include <stdlib.h>


char *dtoa(double valor)
{
  int numdec, sign;
 return(fcvt(valor, 0, &numdec, &sign));    
}
