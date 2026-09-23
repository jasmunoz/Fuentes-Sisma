#ifndef NPTBINE_H

#define NPBINE_H

/*****************************************************************************
 *
 *
 *      (C) INDRA, S.A   Junio 2000
 *              <nptbine.h>            Longitud =xxx    
 *
 *****************************************************************************/


/*    Registro del fichero de Bines */


typedef struct NPSBINE {
 // PRJ0202598 cambio bin de 6 posiciones 8 cambia el tamaño total de NPSBINE
  char   biclave0[8];     /*  Bin                    (clave)  */
  char   birnginf[11];    /*  Rango inferior         (clave)  */ 
  char   birngsup[11];    /*  Rango superior                  */
  char   bicodent[4];     /*  Entidad                                    */
  char   bidescri[25];    /*  Descripcion                                */
  char   bitboper[2];     /*  Tabla de operaciones                       */
  char   biparau1[1];     /*  Parametros de autorizacion 1               */
  char   biparau2[1];     /*  Parametro autorizacion 2                   */
  char   birespri[2];     /*  Resolutor primario                         */
  char   biimport[8];     /*  Importe                                    */
  char   biopmxof[3];     /*  Numero maximo de operaciones diarias en off */
 } NPSBINE;

#endif
