#include <stdio.h>
#include <strings.h>
#include <string.h>
char *itoa(int valor)
{
  static char a[15];   

 memset(a,'\0',15);
 sprintf(a,"%d",valor);  
 return a;
}
