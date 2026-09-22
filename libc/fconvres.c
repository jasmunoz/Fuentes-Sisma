/****************************************************************
 FCONVRES.C
     funcion que transforma el iso000 de un formato interno a su
     correspondiente respuesta

 AREA DE PASO :
 - puntero a un campo tipo formato interno, que a la vuelta de la
   funcion quedara con el iso000 convertido en respuesta
********************************************************************/
#include "cforitno.h"
#include <stdio.h>
#include <stdlib.h>

void fprintLog(char ficheroPlantilla[50], char* format, ...);

extern char nomProg[50];

int fconvres(formato_interno *pfi)
{
   int tipomsg;

 tipomsg = atoi(pfi->fiisocom.fiiso000);
 
 //DMND0036176 añadidas las preautorizaciones
 switch (tipomsg)
 {
 //price
  case 1121:
  case 1125:
  case 1221:
  case 1225:
  case 1241:
  case 1245:
  case 1305:
  case 1325: 
  case 1421:
  case 1425:
  case 1521:
  case 1525:
  case 1805: //PRJ0258542
  case 1825: //PRJ0258542
  
   sprintf(pfi->fiisocom.fiiso000, "%04d", tipomsg+9);
   break;
  //price
  case   1100:
  case   1200:
  case   1120:
  case   1124:
  case   1220:
  case   1224:
  case   1304:  
  case   1324:
  case   1420:
  case   1424:
  case   1524:
  case   1804:
  case   1824:
  
   sprintf(pfi->fiisocom.fiiso000, "%04d", tipomsg+10); 
   break; 
  default:
   //el resto de mensajes consideramos que no son petición luego no obtenemos el tipo respuesta 
   return(1);
 }
 return(0);
}

