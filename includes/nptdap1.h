#ifndef NPTDAP1_H

#define NPTDAP1_H

/*****************************************************************************
 *
 *
 *      (C) INDRA, S.A    2019.
 *
 *              <nptdap1.h>            Longitud =xxx  
 * 
 *****************************************************************************/


/*    Registro del fichero de datos APLICACION */


typedef struct NPSDAP1 {
  char   apredext[2];    /*  Red externa del mensaje           (clave)  */
  char   apnumiso[3];    /*  Numero del dato ISO               (clave)  */
  char   aptipdat[1];    /*  tipo de dato                      (clave)  */
  char   apnombre[10];   /*  nombre dato                                */
  char   appictur[1];    /*  Picture del dato (X,9,B)                   */
  char   aplongit[4];    /*  Longitud del dato                          */
  char   apoffset[4];    /*  Offset del dato                            */
  char   apfechal[8];    /*  Fecha de alta del dato                     */
  char   aphoraal[6];    /*  Hora de alta del dato                      */
 } NPSDAP1;

#endif
