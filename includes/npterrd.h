//PRJ288711 - Tratamiento diferidos erroneos
#ifndef NPTERRD_H
#define NPTERRD_H
/*****************************************************************************
 *
 *
 *      (C) INDRA, S.A   MARZO, 2024. 
 *
 *
 *****************************************************************************/

/*    Registro de la tabla de diferidos erroneos    */

#include "cforitno.h"

typedef struct NPSERRD
 {
 	char			eraccion[1];		//  Accion para procedimiento, E = Error, B = Buena
  char      ersubsis[2];    //  Subsistema               (clave)
  char      ersubapl[2];    //  Subaplicacion            (clave)
  char      ernumope[6];    //  Numero de operacion      (clave)
  char			ernumerr[4];		// Numero de error que devuelve SF
} NPSERRD;

#endif
