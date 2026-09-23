//PRJ0247167 - Revisado ampliación P48
#ifndef NPTDIFE_H
#define NPTDIFE_H
/*****************************************************************************
 *
 *
 *      (C) ERITEL, S.A   Septiembre, 1994. 
 *
 *              <nptdife.h>            Longitud =
 *
 *****************************************************************************/


/*    Registro del fichero de diferidos    */

//RAQUEL PARA EL CAMBIO DEL P48 COMPROBAR SI EL CAMPO DE LA TABLA DIFORITN 
//HAY QUE AMPLIARLO

#include "cforitno.h"

typedef struct NPSDIFE
 {
  char      disubsis[2];    /*  Subsistema               (clave)    */
  char      disubapl[2];    /*  Subaplicacion            (clave)    */
  char      difecha0[8];    /*  Fecha procesal           (clave)    */
  char      dihora00[6];    /*  Hora procesal            (clave)    */
  char      dinumope[6];    /*  Numero de operacion      (clave)    */
  //PRJ0247167 - Revisado ampliación P48 no es necesario cambiarle el tamaño
  char      diforitn[sizeof(formato_interno)];    /*  formato interno de la operacion, pero grabado como char */
  char      diclavpr[35];   //identificación RTS de la operación (clave2 única)

} NPSDIFE;

#endif
