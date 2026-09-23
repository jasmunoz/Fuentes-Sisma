#ifndef GESTTSAS_H
#define GESTTSAS_H
/*****************************************************************************
*  //PRJ0385382
*              <gesttsas.h>
*
*          Tabla de Gestión de Tarjetas SAS para PCI RETO
*     A los campos se les ha dado el mismo nombre que en la tabla pero sin
*     guiones y se han creado todos de tipo char, aunque haya alguno numerico.
*   Los unicos dos campos que no se han definido, por tamano, son los que van
*    entre los campos oriope y sismaescore, ya que son de tipo CLOB de Oracle.
*   Estos campos son el pet_req_sas_req y el pet_req_sas_res.
* Tampoco se dan de alta los campos fec_reg_bd, fec_ult_act, dato_pers_bd y dato_pers_ult_act
*  ya que no se van a utilizar fuera de la propia base de datos.
*****************************************************************************/


/*    Registro del fichero de tabla de SIAM SIAM_GEST_TSAS    */


typedef struct GESTTSAS //Solamente van a quedar los que se van a utilizar en el procedimiento de salida
 {
  char infcidkeyindex[2];   	/* Indice clave para las operaciones criptograficas   */
  char infcidcifpadtyp[2];    /* Tipo de padeo utilizado en el cifrado del mensaje  */
  char infcidcifiv[32];     	/* Vector inicial de encadenamiento aleatorio         */
  char infcifmsgpan[19];  		/* Numero PAN de la tarjeta               */  
  char infcifmsgmob[15];  		/* Telefono movil asociado a la tarjeta   */
  char infcifmsglan[2];      	/* Idioma con el que el cliente opera     */
  char infcifmsgchavec[4];    /* Clave entidad que facilitara desafio   */
  char infcifmsginivec[32];   /* Vector inicial de 16 bytes aleatorios  */
  char infissuerid[4];				/* CSB de la entidad											*/
  char inftimestamp[35];			/* Fecha y hora de la grabacion						*/
  char infdatstatus[1];				/* Tipo de registro Alta, Baja, Modifica  */
  char sigdatsigtyp[2];				/* 02 = MAC, 06 = CMAC										*/
  char sigdatsig[16];					/* El calculo que se ha realizado					*/
  char infcidcifmsg[512];     /* Mensaje cifrado                        */
 } GESTTSAS;

#endif
