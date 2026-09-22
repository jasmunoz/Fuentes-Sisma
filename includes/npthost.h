//PRJ0257005 - Revisado tokenizacion
#ifndef NPTHOST_H
#define NPTHOST_H
/*****************************************************************************
 *
 *              <npthost.h>            Longitud = 1035
 *
 *****************************************************************************/


/*    Registro del fichero de mensajes para Cardplus diferidos       */

typedef struct NPSHOST
 {
  char        prioridad[1];
  char        hostnumcross[10];
  char        hostmens[1024];    /*  mensaje completo Host    */
 } NPSHOST;

#endif
