#ifndef NPTDESC_H

#define NPTDESC_H

/*****************************************************************************
 *
 *
 *      (C) INDRA, S.A   Julio, 2019.     
 *
 *              <nptdesc.h>            Longitud =xxx
 *
 *****************************************************************************/


/*    Registro de descuadres de comercios */



 typedef struct datos_mensaje_desc
 {
   char    longitud[5];
   char    datos[301];
  } datos_mensaje_desc;


typedef struct mensaje_completo
    {
    char          deindacc[4]; // Indicador de accion ([0 pregunta 1 respuesta]; [0 primero 1 repeticion]; [0 no reversion 1 reversion]; [0 aceptacion 1 denegacion 2 rechazo])
    char          decodres[4]; // Codigo de respuesta
    char          deidsesi[2]; // identificador de sesion/canal??
    char          deilt[2];    // identificativo de la ilt
    char          denumster[6]; // número de secuencia de ILT
    char          declvcif[2]; // identificativo de las claves de cifrado
    char          demac[8]; // MAC
    char          deidterm[28]; // Identificativo del terminal
    char          deidusua[10]; // Identificativo del usuario
    char          denumvers[4]; // Identificativo de la version de la transaccion
    char          decodtran[4]; // Codigo del Mensaje Gema Oro
    datos_mensaje_desc   dedatmen; // Datos del Mensaje Gema Oro (acortado, porque no va a ser mas grande de 300)
    } mensaje_completo;


typedef struct NPSDESC {
  char   desubsis[2];     /*  Subsistema                                    */
  char   desubapl[2];     /*  Subaplicacion                             */
  char   defecha0[8];     /*  Fecha de la operacion                     */
  char   dehora00[6];     /*  Hora de la operacion                      */
  char   denumope[8];     /*  Numero de la operacion                        */
  char   demensaj[1652];
} NPSDESC;

#endif
