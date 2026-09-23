/* 
  Funcion que recibe un char en Formato HHMMSS y devuelve el numero de segundos
*/
#include <strings.h>
#include <string.h>
#include <stdlib.h>

int dame_seg(char *buf)
{
char a[10];
int numero;
int segundos=0; 

strcpy(a,buf);

numero=strtol(a,(char **) 0,10); /* pasamos char a int */
segundos+=(numero/10000)*3600; /* pasamos horas a integer */ 

numero%=10000; /* Quitamos las horas */ 
segundos+=(numero/100)*60; /* pasamos los minutos a segundos */
segundos+=numero%100; /* Sumamos los segundos */ 

return(segundos);
}
