#ifndef NPTTARJ_H
#define NPTTARJ_H
/*****************************************************************************
 *
 *
 *      (C) INDRA     
 * 
 *              <npttarj.h>            Longitud = 
 *
 *****************************************************************************/


/*    Registro del fichero de tarjetas                   */


typedef struct NPSTARJ 
 {
  // PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
  char    tanumpan[TAM_PAN]; /*  Numero de tarjeta         (clave)           */
  char    taperfil[8];  /*  Perfil asociado al tipo de tarjeta          */
  char    tacudeb1[20]; /*  Numero de primera   cuenta de debito        */
  char    tacudeb2[20]; /*  Numero de segunda cuenta de debito          */
  char    tacuetar[10]; /*  Numero de cuenta de la tarjeta              */
  char    tafeccad[4];  /*  Fecha de caducidad (AAMM)                   */
  char    taclaope[3];  /*  Clave de operatividad:                      */
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
                              /*  N3 = Bloqueo preventivo                     */
                              /*  N2 = Tarjeta deteriorada                    */
                              /*  NC = Tarjeta no recibida por Titular.CAPTURAR*/
  char    taclatar[2];  /*  Clave tarjeta:                              */
                              /*  01 = Debito-T                               */
                              /*  02 = Credito-T                              */
                              /*  03 = Debito-T/Credito-T                     */
                              /*  11 = Debito-C                               */
                              /*  12 = Credito-C                              */
                              /*  13 = Debito-C/Credito-C                     */
                              /*  17 = Debito-T/Credito-T                     */
                              /*       Debito-C/Credito-C                     */
                              /*  18 = Debito-T/Debito-C                      */
                              /*  19 = Credito-T/Credito-C                    */
  char    tamoneda[4];  /*  Moneda '0978'                               */
  char    tanomtit[30]; /*  Nombre del titular                          */
  char    tadnitit[9];  /*  DNI del titular                             */
  char    talpdica[8];  /*  Limite importe diario cajero              */
  char    tadpdica[8];  /*  Disponible importe diario cajero          */
  char    tadpdicas[1]; /*  Signo Disponible importe diario cajero    */
  char    tafinica[8];  /*  Fecha inicio diario cajero                */

  char    talpdbca[8];  /*  Limite importe periodo debito cajero        */
  char    tadpdbca[8];  /*  Disponible importe periodo debito cajero    */
  char    tadpdbcas[1]; /*  Signo Disponible importe periodo debito cajero    */
  char    tafindca[8];  /*  Fecha inicio periodo debito cajero          */
  char    taclpdca[2];  /*  Periodo debito cajero:                      */
                              /*  00 = No existe periodo                      */
                              /*  01 a 70 = Numero dias que dura periodo      */
                              /*  80 = Periodo de una semana                  */
                              /*  81 = Periodo de dos semanas                 */
                              /*  82 = Periodo comienza el 1 y 15 del mes     */
                              /*  83 = Periodo de un mes                      */
                              /*  84 = Periodo de tres meses                  */
                              /*  85 = Periodo de seis meses                  */
                              /*  86 = Periodo de un anio                     */

  char    talpcrca[8];  /*  Limite importe periodo credito cajero       */
  char    tadpcrca[8];  /*  Disponible importe periodo credito cajero   */
  char    tadpcrcas[1]; /*  Signo Disponible importe periodo credito cajero   */
  char    tafincca[8];  /*  Fecha inicio periodo credito cajero         */
  char    taclpcca[2];  /*  Clave duracion periodo credito cajero       */
 
  char    talpdico[8];  /*  Limite importe diario comercio              */
  char    tadpdico[8];  /*  Disponible importe diario comercio          */
  char    tadpdicos[1]; /*  Signo Disponible importe diario comercio          */
  char    tafinico[8];  /*  Fecha inicio diario comercio                */

  char    talpdbco[8];  /*  Limite importe periodo debito comercio      */
  char    tadpdbco[8];  /*  Disponible importe periodo debito comercio  */
  char    tadpdbcos[1]; /*  Signo Disponible importe periodo debito comercio  */
  char    tafindco[8];  /*  Fecha inicio periodo debito comercio        */
  char    taclpdco[2];  /*  Clave duracion periodo debito comercio      */

  char    talpcrco[8];  /*  Limite importe periodo credito comercio     */
  char    tadpcrco[8];  /*  Disponible importe periodo credito comercio */
  char    tadpcrcos[1]; /*  Signo Disponible importe periodo credito comercio */
  char    tafincco[8];  /*  Fecha inicio periodo credito comercio       */
  char    taclpcco[2];  /*  Clave duracion periodo credito comercio     */

  char    talpdian[8];  /*  Limite importe diario anticipo              */
  char    tadpdian[8];  /*  Disponible importe diario anticipo          */
  char    tadpdians[1]; /*  Signo Disponible importe diario anticipo          */
  char    tafinian[8];  /*  Fecha inicio diario anticipo                */

  char    talpdban[8];  /*  Limite importe periodo debito anticipo      */
  char    tadpdban[8];  /*  Disponible importe periodo debito anticipo  */
  char    tadpdbans[1]; /*  Signo Disponible importe periodo debito anticipo  */
  char    tafindan[8];  /*  Fecha inicio periodo debito anticipo        */
  char    taclpdan[2];  /*  Clave duracion periodo debito anticipo      */
 
  char    talpcran[8];  /*  Limite importe periodo credito anticipo     */
  char    tadpcran[8];  /*  Disponible importe periodo credito anticipo */
  char    tadpcrans[1]; /*  Signo Disponible importe periodo credito anticipo */
  char    tafincan[8];  /*  Fecha inicio periodo credito anticipo       */
  char    taclpcan[2];  /*  Clave duracion periodo credito anticipo     */

  char    tacomwww[1];  /*  Permiso comercio  correo, telefono, www         */

  char    taestact[1];  /*  estado de activacion de tarjeta                 */
                              /*            0 activas AMBAS                       */
                              /*            1 inactiva Nueva   activa antigua     */
                              /*            2 activa   Nueva inactiva antigua     */
                              /*            3 inactivas AMBAS                     */
  char    tafcante[4];  /*  fecha caducidad anterior                        */

  char    talpcrgl[8];  /*  Limite importe periodo credito global           */
  char    tadpcrgl[8];  /*  Disponible importe periodo credito global       */
  char    tadpcrgls[1]; /*  Signo Disponible importe periodo credito global */
  char    tafincgl[8];  /*  Fecha inicio periodo credito global             */
  char    taclpcgl[2];  /*  Clave duracion periodo credito global           */
 } NPSTARJ;


#endif
