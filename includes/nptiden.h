#ifndef NPTIDEN_H 
#define NPTIDEN_H 
/*****************************************************************************
 *              <nptiden.h>            Longitud = 13   
 *
 *****************************************************************************/


/*    Registro del fichero de Identificadores para claves de MAC y EMV     */

typedef   struct {
    char            idsubsis[2];        //  codigo de subsistema (clave)     
    char            idsubapl[2];        //  subaplicacion        (clave)     	
		char            idcomando[2];       //  comando              (clave)
    char            idclave1[150];      //  clave zona 1
    char            idclave2[150];      //  clave zona 2	
}  NPSIDEN;   


#endif
