//PRJ0247167 - Revisado ampliación P48
#ifndef CFORHOST_H
#define CFORHOST_H
/***
   FICHERO: CFORHOST.H
   DESCRIPCION: Fichero con la estructura del formato mensaje para comunicacion con HOST
***/

#include "cnetplus.h"


 typedef struct datos_comunes
 {
    char          hofuncion[4];     /* Funcionlidad */
    char          homedio00[5];     /* Medio */
 } datos_comunes;


/****************************************************************************/
//     ESTRUCTURAS DE PETICION
/****************************************************************************/


typedef struct reg_05P //Peticion general 100,120,200, 220,5000,5200,5300,700 y comunicaciones de petición.
{
    char          r05p_002[19]; /*SIO-1 tarjeta*/ //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r05p_003[6];  /*SIO-2 c.proceso*/
    char          r05p_006[12]; /*SIO-3 ó SIO-5 importe*/
    char          r05p_011[6];  /* num. de transacion SIO-6 */
    char          r05p_012[12]; /* fecha y hora local SIO-7 + SIO-10 */
    char          r05p_014[4];  /* fecha de caducidad SIO-11*/
    char          r05p_018[4];  /* Código actividad SIO-18*/
    char          r05p_028[6];  /* Fecha sesión con Host */
    char          r05p_029[3];  /* Número sesión con Host*/
    char          r05p_032[11];
    char          r05p_033[11];
    char          r05p_035[37];
    char          r05p_037[12]; /*SIO-40 num. referencia*/
    char          r05p_038[6];  /*SIO-37 num.autorización*/
    char          r05p_041[12]; /*SIO-19 terminal*/
    char          r05p_042[16]; /*SIO-42 ref tpv*/
    char          r05p_043[40]; /*SIO-48 nombre y localidad establec.*/
    char          r05p_047[55]; /*SIO-47 datos adic.long. max, r. telef, p.recibos, c.tarj.int,s.cruz roja y pag.trib*/
    char          r05p_4801[12];
    char          r05p_4802[2];
    char          r05p_4803[12];
    char          r05p_4804[2];
    char          r05p_4810[1];
    char          r05p_4812[2];
    char          r05p_049[4];    /*SIO-8 moneda*/
    char          r05p_ding[4];   /*SIO-16 datos ingreso */
    char          r05p_comis[8];  /*SIO-17 comisiones (tipo + comisión) */
    char          r05p_numita[2]; /*SIO-21 num. miembro tarjeta*/
    char          r05p_cpostal[6];/*SIO-43 c. potal*/
    char          r05p_modaut[4]; /*SIO-50 modo autorización*/
    char          r05p_063[40];    /*SIO-63 D. adic. en op. realizadas en otras redes*/
    char          r05p_origen[2]; /*añadido 19/10/2006 origen de la operacion 30-4B, 00-Netplus */
    char          r05p_cross_num[10]; /*numero de cruce ING-DEBITO **/
    char          r05p_cudeb1[20]; /* cuenta debito **/
    char          r05p_fecha0[6]; /* fecha de autorización SISMA */
    char          r05p_canal[2]; /* Canal SIO */
    char          r05p_nouso[599];
 } reg_05P; //Peticion general



typedef struct reg_07P //Peticion carga chip
{
    char          r07p_002[19];  //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r07p_003[6];
    char          r07p_004[12];
    char          r07p_006[12];
    char          r07p_010[8];
    char          r07p_011[6];     /* num. de transacion ISO-11 */
    char          r07p_012[12];      /* fecha y hora local ISO-12 */
    char          r07p_014[4];      /* fecha de caducidad */
    char          r07p_018[4];      /* codigo de actividad */
    char          r07p_022[12];
    char          r07p_024[3];  /* codigo de funcion  ISO-24 */
    char          r07p_025[4];  /* codigo de funcion  ISO-24 */
    char          r07p_026[4];
    char          r07p_030[24];
    char          r07p_032[11];
    char          r07p_033[11];
    char          r07p_035[37];
    char          r07p_037[12];
    char          r07p_038[6];
    char          r07p_039[3];
    char          r07p_040[3];
    char          r07p_041[8];
    char          r07p_042[15];
    char          r07p_043[40];
    char          r07p_045[76];
    char          r07p_4803[12];
    char          r07p_4804[2];
    char          r07p_4810[1];
    char          r07p_4812[2];
    char          r07p_049[4];
    char          r07p_051[3];
    char          r07p_058[11];
    char          r07p_6201[6];
    char          r07p_6202[8];
    char          r07p_6203[8];
    char          r07p_6204[4];
    char          r07p_6205[4];
    char          r07p_6206[8];
    char          r07p_6207[8];
    char          r07p_6208[8];
    char          r07p_6214[8];
    char          r07p_6215[8];
    char          r07p_6216[28];
    char          r07p_nouso[529];
 } reg_07P; //Peticion carga chip



typedef struct reg_09P //Peticion anulacion  420,440,5020,720 y comunicación de anulación
{
    char          r09p_002[19]; /*SIO-1 tarjeta*/  //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r09p_003[6];  /*SIO-2 c. proceso*/
    char          r09p_006[12]; /*SIO-3 ó SIO-5 importe*/
    char          r09p_011[6];  /* SIO-6 num. de transacion */
    char          r09p_012[12]; /* SIO-7 + SIO-10 fecha y hora local */
    char          r09p_014[4];  /* SIO-11 fecha caducidad */
    char          r09p_018[4];  /* SIO-18 código actividad*/
    char          r09p_025[4];  /*SIO-20 C.razón */
    char          r09p_028[6];  /*Fecha sesión con Host */
    char          r09p_029[3];  /*Número sesión con Host */
    char          r09p_037[12];  /*SIO-40 num. referencia*/
    char          r09p_038[6];   /*SIO-37 num autorización*/
    char          r09p_041[12];  /*SIO-19 terminal*/
    char          r09p_043[40];  /*SIO-48 nombre y localidad establecim.*/
    char          r09p_049[4];   /*SIO-8 moneda*/
    char          r09p_056[35];  /*SIO-36 (SIO-6 + SIO-7 + SIO-10 + SIO-19)*/
    char          r09p_6216[28];
    char          r09p_comis[8];  /*SIO-17 comisiones (tipo+ comisiones) */
    char          r09p_numtar[2]; /*SIO-21 número miembro tarjeta*/
    char          r09p_cpostal[6];   /*SIO-43 c. postal*/
    char          r09p_ptransf[32];   /*SIO-32 inf.adic.pregunta transfere*/
    char          r09p_rtransf[42];   /*SIO-35 inf.adic. respuesta transf */
    char          r09p_origen[2];   /* añadido 29-07-2005 origen de la operacion 30 4B
                                                                   00 Netplus */
    char          r09p_cross_num[10]; /*numero de cruce para ING-DEBITO **/
    char          r09p_prv_cross_num[10]; /* numero de cruce de la operacion previa ING-DEBITO***/
    char          r09p_cudeb1[20]; /* cuenta debito **/
    char          r09p_fecha0[6]; /* fecha de autorización SISMA */
    char          r09p_canal[2];  /* Canal SIO */
    char          r09p_nouso[661];/* SI SE AÑADE NUEVO CAMPO AVISAR A CARDPLUS 13/12/2007*/
 } reg_09P; //Peticion anulacion  y comunicación


typedef struct reg_11P //Peticion ficheros cambio estado tarjeta 304
{
    char          r11p_002[19]; /* SIO-1 tarjeta*/ //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r11p_011[6];  /* SIO-6 num. de transacion */
    char          r11p_012[12]; /* SIO-7 y SIO-10 fecha y hora local  */
    char          r11p_039[3];
    char          r11p_072[16];  /*SIO-15 + SIO-19 + SIO-21*/
    char          r11p_093[11];
    char          r11p_094[11];
    char          r11p_101[17];
    char          r11p_cross_num[10]; /* numero de cruce */
    char          r11p_canal[2]; /* Canal SIO */
    char          r11p_nouso[907];
 } reg_11P; //Peticion ficheros  1304,1324,1325


typedef struct reg_26P //Peticion transferencias contables 260
{
    char          r26p_003[6];  /* SIO-2 cod. proceso */
    char          r26p_006[12]; /* SIO-5 importe */
    char          r26p_011[6];  /* SIO-6 número transacción  */
    char          r26p_012[12];  /* SIO-7 + SIO-10 fecha y hora */
    char          r26p_028[6];  /* Fecha sesión con Host */
    char          r26p_029[3];  /* Número sesión con Host */
    char          r26p_037[12];  /* SIO-40 número referencia  */
    char          r26p_041[12];  /* SIO-19 número terminal  */
    char          r26p_048[42];  /* SIO-35 datos transferencia */
    char          r26p_049[4]; /* SIO-8 moneda  */
    char          r26p_056[35]; /* SIO-36 datos previa (SIO-6 + SIO-7 + SIO-10 + SIO-19) */
    char          r26p_adtrans[32];  /*SIO-32 Inf. adicionla pregunta transferencia*/
    char          r26p_cross_num[10]; /*numero de cruce  ING-DEBITO **/
    char          r26p_cudeb1[20]; /* cuenta debito **/
    char          r26p_fecha0[6]; /* fecha de autorización SISMA */
    char          r26p_canal[2];  /* Canal SIO */
    char          r26p_nouso[704];

 } reg_26P; //Peticion transferencias contables 260


typedef struct reg_60P //Peticion msj. función transparente 600 y comunicación 620
{
    char          r60p_002[19];  /* SIO-1 tarjeta */ //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r60p_003[6];   /* SIO-2 cod. proceso */
    char          r60p_011[6];   /* SIO-6 número transacción  */
    char          r60p_012[12];  /* SIO-7 + SIO-10 fecha y hora */
    char          r60p_014[4];   /* SIO-11 fecha caducidad */
    char          r60p_028[6];  /* Fecha sesión con Host */
    char          r60p_029[3];  /* Número sesión con Host */
    char          r60p_037[12];  /* SIO-40 número referencia  */
    char          r60p_041[12];  /* SIO-19 número terminal  */
    char          r60p_numita[2]; /* SIO-21 número miembro tarjeta  */
    char          r60p_cross_num[10]; /*numero de cruce par ING-DEBITO **/
    char          r60p_cudeb1[20]; /* cuenta debito **/
    char          r60p_047[102];
    char          r60p_canal[2];  /* Canal SIO */
    char          r60p_nouso[798]; //[827];

 } reg_60P; //Peticion msj. función transparente 600 y comunicación 620


typedef struct reg_50P //Totalización 0500
{
    char          r50p_012[12];   /* SIO-7 + SIO-10 fecha y hora */
    char          r50p_028[6];    /* Fecha sesión Host */
    char          r50p_029[3];    /* Sesión con Host */
    char          r50p_074[10];   /* Número, abonos SIO-26*/
    char          r50p_075[10];   /* Abonos, número de anulaciones  SIO-30*/
    char          r50p_076[10];   /* Cargos, número SIO-24*/
    char          r50p_077[10];   /* Cargos, número anulaciones SIO-28*/
    char          r50p_086[16];   /* Abonos, importe SIO-25*/
    char          r50p_087[16];   /* Abonos, importe anulación SIO-29*/
    char          r50p_088[16];   /* Cargos, importe SIO-23*/
    char          r50p_089[16];   /* Cargos, importe anulaciones SIO-27*/
    char          r50p_054[12];   /* Total traspasos SIO-54*/
    char          r50p_055[10];   /* Número traspasos SIO-55*/
    char          r50p_056[12];   /* Total anulaciones traspasos SIO-56*/
    char          r50p_057[10];   /* Número anulaciones traspasos SIO-57*/
    char          r50p_monedafac[4];   /* Código moneda facturación SIO-8 */
    char          r50p_monedacon[4];   /* Código moneda contable SIO-44 */
    char          r50p_conciadqui[3];/* Resultado de la conciliacion con adquirente (4B)*/
    char          r50p_cross_num[10]; /* numero de cruce para CardPlus */
    char          r50p_nouso[824];
 } reg_50P; //Totalización 0500



/*************************/
typedef struct reg_18P
{
    char          r18p_011[6];   /* num. de transacion ISO-11 */
    char          r18p_012[12];   /* fecha y hora local ISO-12 */
    char          r18p_024[3];   /* codigo de funcion ISO-24 */
    char          r18p_028[6];   /* Fecha sesión con Host */
    char          r18p_029[3];   /* Número sesión con Host */
    char          r18p_093[11];
    char          r18p_094[11];
    char          r18p_cross_num[10]; /*numero de cruce par ING-DEBITO **/
    char          r18p_nouso[952];

} reg_18P;
/*************************/

typedef struct reg_coP
{
    char          rcop_resp[3]; /*código de finalización a una comunicación de fichero */
    char          rcop_desc[17]; /*Descripción del posible campo erróneo del fichero */
} reg_coP;





/****************************************************************************/
//     ESTRUCTURAS DE RESPUESTA
/****************************************************************************/

 typedef struct reg_06R //Respuesta carga chip
{
    char          r06r_002[19]; //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r06r_003[6];
    char          r06r_004[12];
    char          r06r_006[12];
    char          r06r_010[8];
    char          r06r_011[6];     /* num. de transacion ISO-11 */
    char          r06r_012[12];    /* fecha y hora local ISO-12 */
    char          r06r_030[24];
    char          r06r_032[11];
    char          r06r_033[11];
    char          r06r_038[6];
    char          r06r_039[3];
    char          r06r_042[15];
    char          r06r_044[64];
    char          r06r_4805[41];
    char          r06r_4806[222];
    char          r06r_049[4];
    char          r06r_051[3];
    char          r06r_057[3];
    char          r06r_6213[60];
    char          r06r_nouso[472];

 } reg_06R; //Respuesta carga chip


typedef struct reg_08R //Respuesta general y de comunicaciones de autorización
{

    char          r08r_002[19];   /* SIO-1 tarjeta */ //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r08r_003[6];
    char          r08r_006[12];   /* SIO-3 ó 5 importe */
    char          r08r_011[6];       /* SIO-6 num. transacción */
    char          r08r_comis[8];      /* SIO-17 (2 de tipo y 6 de coimisión) comisiones*/
    char          r08r_018[4];    /* Código actividad SIO-18 */
    char          r08r_028[6]; /*Fecha sesión con host */
    char          r08r_029[3]; /*Sesión con Host*/
    char          r08r_038[6]; /*SIO-37 número autorización*/
    char          r08r_039[3]; /*SIO-33 Respuesta salida*/
    char          r08r_049[4];    /* SIO-8 moneda */
    char          r08r_4835[42];  /*SIO-35 respuesta transferencias */
    char          r08r_4838[118];  /*SIO-38 respuesta saldos long+num.ctas+ 4(t.cta+ fecha sald+ signo + saldo + moneda)*/
    char          r08r_4839[222];  /*SIO-39 respuesta saldos y movimi. 30 saldos + num. mov + 5(38)*/
    char          r08r_4858[28];  /*SIO-58 respuesta saldos */
    char          r08r_cross_num[10]; /* numero de cruce*/
    char          r08r_nouso[517];
 } reg_08R; //Respuesta general y de comunicaciones de autorización


typedef struct reg_51R //Respuesta Totalización 0510
{
    char          r51r_012[12];   /* SIO-7 + SIO-10 fecha y hora */
    char          r51r_028[6];    /* Fecha sesión Host */
    char          r51r_029[3];    /* Sesión con Host */
    char          r51r_074[10];   /* Número, abonos SIO-26*/
    char          r51r_075[10];   /* Abonos, número de anulaciones  SIO-30*/
    char          r51r_076[10];   /* Cargos, número SIO-24*/
    char          r51r_077[10];   /* Cargos, número anulaciones SIO-28*/
    char          r51r_086[16];   /* Abonos, importe SIO-25*/
    char          r51r_087[16];   /* Abonos, importe anulación SIO-29*/
    char          r51r_088[16];   /* Cargos, importe SIO-23*/
    char          r51r_089[16];   /* Cargos, importe anulaciones SIO-27*/
    char          r51r_054[12];   /* Total traspasos SIO-54*/
    char          r51r_055[10];   /* Número traspasos SIO-55*/
    char          r51r_056[12];   /* Total anulaciones traspasos SIO-56*/
    char          r51r_057[10];   /* Número anulaciones traspasos SIO-57*/
    char          r51r_061[2];   /* Indicador cuadre de totales SIO-61*/
    char          r51r_monedafac[4];   /* Código moneda facturación SIO-8 */
    char          r51r_monedacon[4];   /* Código moneda contable SIO-44 */
    char          r51r_cross_num[10]; /*numero de cruce*/
    char          r51r_nouso[815];

 } reg_51R; // Respuesta Totalización 0510





typedef struct reg_10R //Respuesta anulaciones y a comunicaciones de anulación 420
{
    char          r10r_002[19]; /* SIO-1 tarjeta */ //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r10r_003[6];
    char          r10r_006[12]; /* SIO-3 ó 5 importe */
    char          r10r_011[6];   /* SIO-6 num. transacción */
    char          r10r_018[4];   /* SIO-18 Cód. actividad */
    char          r10r_028[6];   /* Fecha sesión con Host  */
    char          r10r_029[3];   /* Sesión con Host */
    char          r10r_comis[8];  /* SIO-17 comisiones*/
    char          r10r_039[3]; /* SIO-33 cod finalización */
    char          r10r_049[4]; /* SIO-8 moneda */
    char          r10r_cross_num[10]; /* numero de cruce*/
    char          r10r_nouso[933];
 } reg_10R; //Respuesta anulaciones y a comunicaciones de anulación 420


typedef struct reg_12R //Respuesta ficheros  314
{
    char          r12r_002[19]; /* SIO-1 tarjeta   */ //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r12r_011[6];  /* SIO-6 tarjeta num transacción */
    char          r12r_012[12]; /* SIO-7 + SIO-10 fecha y hora local  */
    char          r12r_039[3];  /* SIO-33 cod. finalización */
    char          r12r_cross_num[10]; /* numero de cruce */
    char          r12r_nouso[964];
 } reg_12R; //Respuesta ficheros  314


typedef struct reg_27R //Respuesta transferencias contables 270
{
    char          r27r_006[12]; /* SIO-5 importe */
    char          r27r_011[6];  /* SIO-6 número transacción  */
    char          r27r_028[6];  /* Fecha sesión Host  */
    char          r27r_029[3];  /* Número sesión Host  */
    char          r27r_038[6];  /* SIO-37 número autorización  */
    char          r27r_039[3];  /* SIO-33 cod. finalización  */
    char          r27r_049[4]; /* SIO-8 moneda  */
    char          r27r_cross_num[10]; /* numero de cruce*/
    char          r27r_nouso[964];

 } reg_27R; //Respuesta transferencias contables 270


typedef struct reg_61R //Respuesta msj. función transparente 610 y comunicación 630
{
    char          r61r_002[19];  /* SIO-1 tarjeta */  //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    char          r61r_011[6];   /* SIO-6 número transacción  */
    char          r61r_028[6];   /* Fecha sesión Host */
    char          r61r_029[3];   /* Número sesión Host  */
    char          r61r_038[6];  /* SIO-37 número autorización  */
    char          r61r_039[3];  /* SIO-33 cod finalización  */
    char          r61r_cross_num[10]; /* numero de cruce*/
    char          r61r_nouso[961];

 } reg_61R; //Respuesta msj. función transparente 610 y comunicación 630


/*************************************/
typedef struct reg_19R
{
    char     r19r_011[6];    /* num. de transacion ISO-11 */
    char     r19r_012[12];   /* fecha y hora local ISO-12 */
    char     r19r_028[6];    /* Fecha sesión Host */
    char     r19r_029[3];    /* Número sesión Host */
    char     r19r_039[3];
    char     r19r_093[11];
    char     r19r_094[11];
    char     r19r_cross_num[10]; /* numero de cruce*/
    char     r19r_nouso[952];
} reg_19R;
/*************************************/

typedef struct reg_com
{
    char     rcom_tipreg[1];    /* Tipo registro, 1->tarjetas, 2->cuentas 3->comisiones, 4->bines*/
    char     rcom_tipmov[1];   /* Tipo movimiento: 1->bajas, 2->altas, 3->modificacion*/
    char     rcom_regtar[570];    /* Registro tarjetas , o cuentas o comisiones*/
    char     rcom_nouso[442];
} reg_com;
/*************************************/




 typedef struct peticion_comunes
 {
    char          r00p_000[4];   //tipo de mensaje
    char          pet_numope[6];

 } peticion_comunes;


 typedef struct respuesta_comunes
 {
    char          res_codres[ 2];
    char          res_codfin[ 2];
    char          res_codlit[ 4];
    char          res_numope[ 6];
    char          res_rghost[ 1];
    char          res_rp_000[ 4];

 } respuesta_comunes;


typedef union peticion_resto
{
  reg_05P          pet_general;
  reg_07P          pet_cargchi;
  reg_09P          pet_anulaci;
  reg_11P          pet_fichero;
  reg_26P          pet_transfe;
  reg_50P          pet_totales;
  reg_60P          pet_ftransp;
  reg_18P          pet_tst;
  reg_coP          pet_comunic;

  } peticion_resto;

typedef union respuesta_resto
{
  reg_06R          res_cargchi;
  reg_08R          res_general;
  reg_10R          res_anulaci;
  reg_12R          res_fichero;
  reg_27R          res_transfe;
  reg_51R          res_totales;
  reg_61R          res_ftransp;
  reg_19R          res_tst;
  reg_com          res_comunic;

} respuesta_resto;


 typedef struct peticion_host
 {
    peticion_comunes     ho_p_comunes;
    peticion_resto       ho_p_resto;

 } peticion_host;

 typedef struct respuest_host
 {
    respuesta_comunes     ho_r_comunes;
    respuesta_resto       ho_r_resto;

 } respuest_host;



typedef union formato_host
{
  peticion_host    vpeticion;
  respuest_host   vrespuesta;

} formato_host;
#endif
/* De CFORHOST_H */
