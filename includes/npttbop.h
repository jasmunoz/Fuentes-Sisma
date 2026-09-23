#ifndef NPTTBOP_H
#define NPTTBOP_H
/*****************************************************************************
 *
 *   
 *      (C) ERITEL, S.A   Septiembre, 2019.  
 *
 *              <npttbop.h>            Longitud = 84
 *****************************************************************************/


/*    Registro del fichero de tabla de operaciones       */

typedef   struct {

    char         tbcodent[4];        /*  entidad                      */
    char         tbtabla0[2];        /*  codigo de tabla              */
    char         tbclave0[12];       /*  clave del msg                */
    char         tbdescri[50];       /*  Descripcion                  */
    char         tbfuncis[20];       /*  funciones a aplicar          */
}              NPSTBOP;


#endif
/* NPTTBOP_H */
