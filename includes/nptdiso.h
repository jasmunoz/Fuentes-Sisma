#ifndef NPTDISO_H

#define NPTDISO_H

/*****************************************************************************
 *
 *
 *      (C) INDRA, S.A   Enero, 2018.
 * 
 *              <nptdiso.h>            Longitud =xxx 
 *
 *****************************************************************************/

/*    Registro del fichero de datos ISO */


typedef struct NPSDISO {
  char   doredext[2];    /*  Red externa del mensaje           (clave)  */
  char   donumiso[3];    /*  Numero del dato ISO               (clave)  */
  char   dopictur[1];    /*  Representacion del dato                    */
  char   dolongit[4];    /*  Longitud del dato (si >1000, con cabecera) */
  char   dolonmax[3];    /*  Longitud maxima en caso de ser variable    */
  char   dousopri[1];    /*  Uso del dato en el protocolo               */
  char   dousomac[1];    /*  Uso del dato en el calculo de MAC          */
  char   dofechal[8];    /*  Fecha de alta del dato                     */
  char   dohoraal[4];    /*  Hora de alta del dato                      */
 } NPSDISO;

#endif
