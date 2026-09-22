//PRJ0247167 - Revisado ampliación P48
#ifndef NPTLOGO_H
#define NPTLOGO_H 
/*****************************************************************************
 *
 *
 *      (C) INDRA Septiembre, 1994.
 *
 *              <nptlogo.h>            Longitud =
 *
 *****************************************************************************/


/*    Registro del fichero de log de operaciones       */

//RAQUEL para el p48 comprobar si hay que amplicar el campo loforitn de NPTLOGO

#include "cforitno.h"
typedef struct NPSLOGO
 {
  char      lonumope[6];   /*  Numero de operacion CRO      (clave) */
  char      lofecha0[8];   /*  Fecha CRO                    (clave) */
  char      lohora00[6];   /*  Hora  CRO                    (clave) */
  char      lonumpan[TAM_PAN]; /*  Numero de tarjeta            (clave) */
  char      locomerc[15];  /*  Identificacion comercio      (clave) */
  char      loclavpr[35];  /*  clave para recuperar operacion (clave) */
  char      loeseven[1];   /*  Estado evento generado               */
  char      loesanul[1];   /*  Estado de Anulacion                  */
  char      loimporte[12];   /*  importe de la operación sin decimales x100 completado con ceros por la izquierda */
  char      losesion[3];      
  char      lofecses[6];
  //PRJ0247167 - Revisado ampliación P48 no es necesario cambiarle el tamaño
  char      loforitn[sizeof(formato_interno)];    /*  formato interno de la operacion, pero grabado como char */
 } NPSLOGO;

typedef struct
{
 char      loceropr[16];  /*  Filler a ceros                  */
 char      lonumepr[6];   /*  Numero de boleta                */
 char      locodipr[2];   /*  codigo de mercadona? (06)       */
 char      locomepr[4];   /*  codigo comercio                 */
 char      lotermpr[3];   /*  codigo terminal                 */
 char      loespapr[4];   /*  Filler a espacios               */
} tpclav01;
              /* clave busqueda de previa para origen             */
              /*       subsistema 16  (mercadona, carrefour, ...) */

typedef struct
{
 char      lotipopr[4];   /*  Tipo de mensaje                */
 char      lonumepr[6];   /*  Numero de operacion PRICE      */
 char      lofehopr[12];  /*  Fecha y hora PRICE             */
 char      loadqupr[13];  /*  Identificacion adquiriente     */
} tpclav02;
              /* clave busqueda de previa para origen              */
              /*       subsistema 19  (PUC)                        */
              /*       subsistema 20  (PRICE)                      */
              /*       subsistema 22  (fimestic, finaconsa)        */

typedef struct
{
 char      lonumepr[6];   /*  Trace Number (ref. operacion)  */
 char      lofecha0[6];   /*  Fecha operacion                */
 char      lohora00[6];   /*  Hora  operacion                */
 char      loidterm[12];  /*  id. Terminal                   */
 char      loespapr[5];   /*  Filler a espacios              */
} tpclav03;
              /* clave busqueda de previa para origen             */
              /*       subsistema 30  (SIO) */
#endif
