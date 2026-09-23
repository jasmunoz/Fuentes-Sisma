#include "cnetplus.h"
/*--- mensaje hacia o desde la linea (colas del manejador) ---*/
//RAQUEL revisar este datos el tamaño para el p48
typedef struct mensavb
{
   char           error[24];
   char           opcion;
   char           fichero[8];
   char           clave[100];
   char           longitud[4];
   char           datos[2048];
} mensavb;

/* mide 2185 que está definido en cnetplus.h como MAX_MENSA_VB*/


typedef struct mensaje_lin
{
   long           tipo;
   char           mensaje_linea[MAX_MENSA_VB];
} mensaje_lin;



/***  ---------------   mensaje recibido   -------------   */
   mensaje_lin    msglin;

