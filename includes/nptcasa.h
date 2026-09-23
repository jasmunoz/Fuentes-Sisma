//PRJ0247167 - Revisado ampliación P48
#ifndef NPTCASA_H
#define NPTCASA_H

#include "cforitno.h"
/*****************************************************************************
 *
 *
 *      (C) ERITEL, S.A   Septiembre, 2019.
 *
 *              <nptdife.h>            Longitud =
 *
 *****************************************************************************/


/*    Registro del fichero de log de operaciones       */

typedef struct NPSCASA
{
 char     canumope[6];          /*  Numero de operacion    (clave)    */
 char     catieabs[10];          /*  Tiempo absoluto cuando caduca     */
 char     catieesp[10];          /*  Tiempo de espera de operacion     */
 //PRJ0247167 - Revisado ampliación P48 no es necesario cambiarle el tamaño
 formato_interno caforitn;      /*  Formato interno operacion         */
} NPSCASA;

#endif
