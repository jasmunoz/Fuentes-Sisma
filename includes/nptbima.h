#ifndef NPTBIMA_H

#define NPBIMA_H

/*****************************************************************************
 *
 *       
 *      (C) INDRA, S.A   Enero, 1995.
 *   
 *              <nptbima.h>            Longitud =xxx
 *
 *****************************************************************************/


/*    Registro del fichero de Bitmpas */

typedef struct NPSBIMA {
  char   bmredext[2];    /*  Red externa del mensaje           (clave)  */
  char   bmtipmen[4];    /*  Tipo de mensaje                   (clave)  */
  char   bmsentra[1];    /*  Sentido de traduccion             (clave)  */
  char   bmpervia[1];    /*  Permiso de viajar en este sentido          */
  char   bmbima01[192];  /*  Mapa de bits primario                      */
  char   bmbima02[192];  /*  Mapa de bits secundario                    */
  char   bmveriso[7];    /*  Version ISO                                */
  char   bmverpri[3];    /*  Version APL                                */
  char   bmfechal[8];    /*  Fecha de alta del mensaje                  */
  char   bmhoraal[4];    /*  Hora de alta del mensaje                   */
 } NPSBIMA;

#endif
