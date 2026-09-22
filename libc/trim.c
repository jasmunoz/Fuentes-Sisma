#include <strings.h>
#include <string.h>

/* Elimina todos los caracters c de la derecha de la cadena bufer   */
void rtrim(char *bufer,char c)
{
 int tam=0; 
 
 tam=strlen(bufer);

 while ((bufer[tam]==c) || (bufer[tam]=='\0') && (tam > 0)) tam--; 
 bufer[tam+1]='\0';
 return;
}

/* Elimina todos los caracters c de la izquierda de la cadena bufer   */

void ltrim(char *bufer,char c)
{
 int tam=0;

 while ((bufer[tam]==c) && (bufer[tam]!='\0')) tam++; 
 strcpy(bufer,&bufer[tam]);
 return;
}
