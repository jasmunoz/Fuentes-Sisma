#ifndef NPTTACU_H
#define NPTTACU_H
/*****************************************************************************
 *
 *
 *      (C) ERITEL, S.A   Septiembre, 1994. 
 *
 *              <npttacu.h>            Longitud = 
 *
 *****************************************************************************/


/*    Registro del fichero de carga masiva de tarjetas y cuentas             */

/**********************************************************************/

typedef struct tarjeta
 {
  char tanumpan[19];   /*  Numero de tarjeta         (clave)           */
  char tacuetar[10];   /*  Numero de cuenta de la tarjeta              */
  char taclaope[2];    /*  Clave de operatividad:                      */
                       /*  AA = Tarjeta operativa                      */
                       /*  F1 = Lista Negra, Robada o Perdida. CAPT.   */
                       /*  F2 = Lista Negra, Robada o Perdida. CAPT.   */
                       /*  F3 = Lista Negra, Robada o Perdida con      */
                       /*       actividad. CAPTURAR                    */
                       /*  F6 = Lista Negra, Falsificada. CAPTURAR     */
                       /*  ND = Lista Gris. DENEGAR                    */
                       /*  AI = Solicitar D.N.I.                       */
                       /*  NC = Cliente moroso. CAPTURAR               */
                       /*  NM = Cliente moroso. Incluir en RAI. CAPT.  */
                       /*  N9 = Cliente moroso. NO Incluir RAI. CAPT.  */
  char taledico[2];    /*  Limite extraccion diario comercio           */
  char talpdico[7];    /*  Limite importe diario comercio              */
  char talecrco[2];    /*  Limite extraccion periodo credito comercio  */
  char talpcrco[7];    /*  Limite importe periodo credito comercio     */
  char taclpcco[2];    /*  Clave duracion periodo credito comercio     */

 } tarjeta;

/**********************************************************************/
/*    Registro del fichero de cuentas                    */

typedef struct cuenta
 {
  char   cunumcue[10]; /*  Numero de cuenta  (clave)                    */
  char   cuexcper[3];  /*  Exceso permitido  Formato X99                */
                       /*  Valores de X:                                */
                       /*  P = Admite el % de exceso cuant. por 99      */
                       /*  V = V.I.P. 100% de exceso                    */
                       /*  0 a 9 = Permitido exceso equivalente al      */
                       /*          valor de los tres caracteres del     */
                       /*          elemento en miles de pesetas         */
  char   culrecco[7];  /*  Limite respaldo debito comercio              */
  char   cucrecco[2];  /*  Clave duracion periodo respaldo comercio     */
 } cuenta;

/**********************************************************************/
/*----- parte comun a todos los tipos de registro del fichero  */
typedef struct comunes
{
  char   titipreg[1];
  char   titipmov[1];
}  comunes;

/*---- redefinicion de la parte no comun para registro cabecera */
typedef struct cabecera
{
  char   tifecgen[8];
  char   tihorgen[4];
}  cabecera;

/*----  PARTE NO COMUN  */
typedef union resto_tarcu
{
cabecera      tinpvcabe;
tarjeta       tinpvtarj;
cuenta        tinpvcuen;
}  resto_tarcu;

typedef struct NPSTACU
{
comunes       ticomunes;
resto_tarcu   tirestdat;
}  NPSTACU;

#endif

/*****************************************************************/

