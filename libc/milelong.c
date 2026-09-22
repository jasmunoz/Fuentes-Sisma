#include <sys/time.h>
#include <time.h>
long milelong()
{
 /*
   el resultado tiene como máximo 24 horas * 3600000 = 86400000  (8 cifras, cabe en un dato tipo long)
   hay que incluir time.h en el programa llamante.
   la funcion gettimeofday devuelve los microsegundos del momento
   la funcion localtime devuelve la fecha y la hora minuto segundo
   De la fecha que devuelve al año hay que sumarle 1900 y al mes 1. 
 */

   struct timeval tim;
   double t1_usec;
   int iMiliSec;
   long mltotal;


   gettimeofday(&tim, NULL); 
   t1_usec=tim.tv_usec;
   iMiliSec = t1_usec/1000; 


   time_t tim_t=time(NULL);
   struct tm *now=localtime(&tim_t);

   mltotal = (now->tm_hour * 3600000) + (now->tm_min * 60000)
         + (now->tm_sec * 1000) + iMiliSec; 

   return(mltotal);
}
