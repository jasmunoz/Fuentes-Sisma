#include <time.h>
#include <sys/timeb.h>
long centlong()
{
  struct timeb ttp; 
  long centesimas;  

 ftime(&ttp); 
 /*
   reducimos el numero de segundos que devuelve ftime a un numero
   de 6 cifras para poder pasarlo a centesimas y que quepa en un long
 */
 centesimas=((ttp.time-(1000000*(ttp.time/1000000)))*100)+(ttp.millitm/10);
 return(centesimas);
}
