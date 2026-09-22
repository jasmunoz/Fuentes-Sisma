#ifndef NPTTOTO_H
#define NPTTOTO_H
/*****************************************************************************
 *
 *
 *      (C) INDRA  Marzo, 2019.     
 *
 *              <npttoto.h>            Longitud = 149   
 *
 *****************************************************************************/


/*    Registro del fichero de tabla de operaciones       */


typedef struct NPSTOTO
{

  char tosubsis[2];     /* Subsistema totalizado              (clave)  */
  char tosubapl[2];    /*  Subaplic. con la que se totaliza   (clave)  */
  char tosesion[3];    /*  Sesion de totalizacion             (clave)  */
  char toorides[2];    /*  Totales de Origen/Destino          (clave)  */
  char tofecses[6];    /*  Fecha de Conciliacion              (clave)  */

  char tonuabon[10];   /*  Numero de abonos                            */
  char tonuanab[10];   /*  Numero de abonos debidos a anulacion        */
  char tonucarg[10];   /*  Numero de cargos                            */
  char tonuanca[10];   /*  Numero de cargos debidos a anulacion        */


  char toimabon[16];   /*  Importe de abonos                           */
  char toimanab[16];   /*  Importe de abonos debidos a anulacion       */
  char toimcarg[16];   /*  Importe de cargos                           */
  char toimanca[16];   /*  Importe de cargos debidos a anulacion       */
  char toimabcu[16];   /*  Importe abono de cuotas                     */
  char toimcacu[16];   /*  Importe cargo de cuotas                     */

} NPSTOTO;

#endif
