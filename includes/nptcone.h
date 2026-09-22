#ifndef NPTCONE_H
#define NPTCONE_H
/*****************************************************************************
 *
 *              <nptcone.h>            Longitud = 340     
 *
 *****************************************************************************/


/*   copy con los modulos existentes para alimentar la tabla de conexiones **/

typedef   struct {
    char    cxcodent[4];      /*  codigo de entodad           pk   */
    char    cxsubsis[2];      /*  codigo de subsistema        pk   */
    char    cxsubapl[2];      /*  codigo de subaplicacion          */ 
    char    cxnomsub[8];      /*  nombre procedimiento        pk   */
    char    cxnomPan[20];     /*  nombre que aparece en pantalla   */
    char    cxsemaf[1];       /*  indica si esta o no conctado     */
                              /*     0 - No, en rojo               */
                              /*     1 - Si, en verde              */
    char    cxladPan[2];      /* indica donde se pinta             */
                              /*  AA Arriba, los pmanej            */
                              /*  AM Arriba, comunica centro con pamnej */
                              /*  II mas a la izquierda, el pmanej */
                              /*  IM Izquierda medio, los penv y recep */
                              /*  DD mas a la derecha, el pmanej   */
                              /*  DM derecha medio                 */
                              /*  CN centro, el nucleo             */
                              /*  BB abajo, los pamenj             */
                              /*  BM abajo medio, comunica con centro */
} NPSCONE;


#endif
/* NPTCONE_H */
