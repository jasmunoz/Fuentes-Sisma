#ifndef NPTPARA_H
#define NPTPARA_H
/*****************************************************************************
 *
 *    
 *      (C) INDRA S.A   Septiembre, 2024. //PRJ0258542
 *
 *              <nptpara.h>            Longitud = 169  
 *
 *****************************************************************************/


/*    Registro del fichero de Parametros del Sistema         */ 

typedef   struct {
	char            psindice[3];        /*  Indice para carga tabla 00       */
  char            pscsb000[5];        /*  Codigo CSB de la entidad         */
  char            psnoment[21];       /*  Nombre de la entidad             */
  char            psnumope[7];        /*  Numero de operacion              */
                                      /*   1 - Num. sesion actual          */
                                      /*   5 - Num. operacion en la sesion */
  char            psnument[3];        /*  Numero de entiedad origen        */
  char            pshcamse[7];        /*  Hora de cambio de sesion         */
  char            psfeinse[9];        /*  Fecha de inicio de sesion        */
  char            pshoinse[7];        /*  Hora de inicio de sesion         */
  char            psmodocs[2];        /*  Modalidad cambio sesion          */
                                      /*   0 - Cambio Automatico           */
                                      /*   1 - Cambio Manual               */
                                      /*   2 - Mensaje del HOST            */
  char            pstimesp[7];        /*  Tiempo de activacion del super.  */
  char            psnumcross[11];     /*  Numero de cruce                  */
  char            pscerrando[2];      /*  1 - se esta cerrando             */
	char            psresolu[3];        /*  Rsolutor                         */
	char            psmonito[3];        /*  Control pantallas,monitorizacion */
	char            pscotrms[3];        /*  Emisor trms 00 I, 01 A, 02 S     */
	char            psidered[2];        /*  IDE-CD 5 I, 6 A, 7 S             */
	char            pscltrat[9];        /*  Cola tratami                     */
	char            psclenvi[5];        /*  Cola clxxxx envio                */
	char            psclrece[9];        /*  Cola clxxxx recepcion            */
	char            psclvhsm[51];       /*  Clave para MAC en TDES           */
	char            psclzmka[51];       /*  Clave de transporte activa       */
	char            psclzmki[51];       /*  Clave de transporte inactiva     */
	char            psclzika[51];       /*  Clave inicial de integridad acti */
	char            psclziki[51];       /*  Clave inicial de integridad inac */
	char            psclzpka[51];       /*  Clave cifrado PIN activa         */
	char            psclzpki[51];       /*  Clave cifrado PIN inactiva       */
	char            psclzaka[51];       /*  Clave calculo MAC activa         */
	char            psclzaki[51];       /*  Clave calculo Mac inactiva       */
	char            psclzeka[51];       /*  Clave cifrado datos activa       */
	char            psclzeki[51];       /*  Clave cifrado datos inactiva     */
	char						psclclap[51];				/*  Clave cálculo de cálculo de PIN  */
	char						psclblop[51];				/*  Clave bloqeu de PIN TDES o DES   */
} NPSPARA; //NPTPARA

NPSPARA tabla_para[2];

#endif
/* NPTPARA_H */
