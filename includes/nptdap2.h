#ifndef NPTDAP2_H

#define NPTDAP2_H

/*****************************************************************************
 *
 *              <nptdap2.h>            Longitud =xxx  
 *
 *****************************************************************************/


/*    Registro del fichero de datos APLICACION2*/


typedef struct NPSDAP2 {
  char   a2redext[2];    /*  Red externa del mensaje           (clave)  */
  char   a2numiso[3];    /*  Numero del dato ISO               (clave)  */
  char   a2pictur[1];    /*  Picture del dato (X,9,B,M)                 */
  char   a2longit[4];    /*  Longitud del dato. Si es mayor que 1000,   */
                               /*  la diferencia es la longitud de la cabecera*/
  char   a2lonmax[4];    /*  Longitud maxima si cabecera                */
  char   a2offset[4];    /*  Offset del primer byte                     */
 } NPSDAP2; 

#endif
