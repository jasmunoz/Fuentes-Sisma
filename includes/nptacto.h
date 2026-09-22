#ifndef NPTACTO_H
#define NPTACTO_H
/*****************************************************************************
 *
 *              <nptacto.h>            Longitud = 34    
 *
 *****************************************************************************/


/*    Registro del fichero de tabla de acumulados para totales    */


typedef struct NPSACTO
 {
  char atnumope[6];     /* Numero de operacion         (clave)  */
  char atcodfin[3];     /* Código de finalización               */
  char atiso000[4];     /* Tipo de operación                    */
  char atdig12[2];      /* Código de proceso 12                 */
  char atimporte[12];   /* Importe                              */
  char atredes[4];      /* subsistema y subapli de la red con la que se concilia (3001 adq 3002 emisor)  */
  char atsesion[3];     /* sesion, pero no esta en la tabla     */
  char atimpcuod[12];  /* importe de la cuota signo -d          */
  char atimpcuoc[12];  /* importe de la cuota signo +c          */
  char atresol[2];      /* resolutor de la operaciones   (03)   */
  char atcompe[1];      /* compensacion, P48.26                 */
  char atfecses[6];     /* fecha sesion                         */
 } NPSACTO;

#endif
