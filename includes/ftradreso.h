//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
#ifndef FTRADRESO_H
#define FTRADRESO_H

#include "msg_nucl.h"

/*** 
   FICHERO: FTRADRESO.H
   DESCRIPCION: Fichero con la estructura de comunicacion 
   LLeva tamaño total del mensaje y la division del mensaje
   en los distintos campos para la copy mepa
                
***/

char subsistema_general[3];
 typedef struct mensaje_comunicacion
{
    long    tipo;
    char    long_mensaje[4];
	//PRJ0247167 - Revisado ampliación P48
    //DMND0037271 el mensaje no cambia de tamaño porque es suficiente 
	//a pesar de la ampliacion del p48, se cambian los campos de las tablas
	//NPTCASA, NPTLOGO y NPTDIFE para que graben los 4000
    //char    mensaje_linea[2000];
    char    mensaje_linea[4000];
} mensaje_comunicacion;

typedef struct idtrans03
{
     char    FECHA [8];
     char    HORA [6];
     char    NUMOPE[6];
} idtrans03;

typedef struct fechor
{
     char    AAAA[4];
     char    MM[2];
     char    DD[2];
     char    HH[2];
     char    MI[2];
     char    SS[2];
} fechor;

typedef struct infoprivada
{
     char    IMP_OPFI[12];       //IMPORTE DE LA operacion financiada  
     char    NUM_CTS_OPFI[3];    //NUMERO DE CUOTAS
     char    IMP_CTA_OPFI[12];   //IMPORTE DE LA CUOTA
     char    FEC_PRMR_VCMVTO[8]; //FECHA PRIMER VENCIMIENTO
     char    POR_INTRS[7];       //INTERES
     char    POR_TAE[7];         //TAE
     char    CMSN_ESTDO[12];     //COMISION DE ESTUDIO
     char    POR_INTRS_MORA[7];  //INTERES DE MORA
     char    POR_TAE_MORA[7];    //TAE DE MORA
     char    CMSN_MORA[12];      //COMISION MORATORIA
     char    IMP_TTL_SGRO[12];   //SEGURO
     char    IMP_TTL_OPFI[12];   //IMPORTE TOTAL DE OPERACION
     char    IND_CMPRA_GRTS[1];  //INDICADOR COMPRA GRATIS
} infoprivada;

typedef struct p48saldos
{
   char MON_CRED [3];
   char SIGNO_CRED [1];
   char IMP_SLD_CRED [12];
   char MON_DEB [3];
   char SIGNO_DEB [1];
   char IMP_SLD_DEB [12];
   char MON_DIA [3];
   char SIGNO_DIA [1];
   char IMP_SLD_DIA [12];
} p48saldos;

typedef struct p48mvmtos
{
   char MONEDA [3];
   char MVMTOS [6][37];
   // aammddC999999999999compra en el super
   // fecha de 6
   // signo de 1
   // importe de 12
   // descripcion de 18
   
} p48mvmtos;

 
typedef struct datosP48
{
     char OFFSET [12];
     char IDIOMA [2];
     char NEW_OFFSET [12];
     char NEW_IDIOMA [2];
     char CVV [3];
     char TIP_COMPEN [1];
     char NUM_TFNO [9];
     char COD_BLOQMVT [1]; //si es consulta de movimiento estar� entre 1 y 5
     p48saldos DAT_SALDOS;
     p48mvmtos DAT_MOVIMIENTOS;
     char IMP_NETOS [180]; //desglose de importes netos P48.15
} datosP48;

typedef struct excneg
{
    char COD_EXCNEG[3];
    char COD_LN[1];
    char FCAD_EXCNG[4]; //AAMM
    char FACT_EXCNG[12]; //AAMMDDhhmmss
    char FACT_LN[12]; //AAMMDDhhmmss
    char TIP_DOC_TITU[3]; //DNI
    char NUM_DOC_TITU[12]; //
    char CSB_COMUNICANTE[4];
}excneg;

typedef struct datosP4873
{
     char IND_CONTA[1];
     char contacLES[20];
} datosP4873;

typedef struct formato03
{
     char    INDCOM[1];           //Indica si hay que hacer COMMIT (=0) o ROLLBACK (=1)
//importante, este dato esta puesto a pelo en el sergener y pmanejTux, si cambia de sitio habr� que modificar.
     char    CODENT[4];			 // código de la entidad emisora (nuemprce)
     char    LONG_TJT [2];       // longitud de la tarjeta
     // PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
     char    PAN [19];           // número de tarjeta con espacios detras  
     char    COD_PRCSO[6];       // código de proceso
     char    IMPRTE [12];        // importe en moneda original con ceros por delante.
     // RAQUEL MODIFICADO idtrans03 NUM_IDNTFCDR_TRNSCCN; // identificador de la transaccion
     char    NUM_IDNTFCDR_TRNSCCN [6]; // identificador de la transaccion
     fechor  FECHA_HORA;         // fecha y hora de la transacción
     char    fec_cad[4];         // fecha de caducidad de la tarjeta
     char    num_ssn[3];         // num sesión con ceros por delante
     char    cod_orgn[6];        // ej datafono, valores pendientes de definir
     char    cod_act[4];         // codigo de actividad
     char    bit22[12];          // bit 22
     char    cod_fncn[3];        // código de función
     char    cod_rzn[4];         // código de razón
     char    ent_adqrnte[11];    // entidad adquirente p32
     char    cod_red[4];         // código de red. ej 0444 S4B
     char    NUM_RFRNCA[23];     // bit 31
     char    COD_ATRZCN[6];      // código de autorización (dato de salida salvo en op. off
     char    COD_RSPSTA[3];      // código de respuesta (dato de salida)
     char    COMERCIO [15];      // comercio P42
     char    TERMINAL[8];        // terminal P41
     char    TARIFARIO[4];       // tarifario 
     char    FORMA_PAGO [4];     // forma de pago 
     char    COD_MNDA[3];        // codigo de moneda original P49
     idtrans03  DAT_PRVA;        // identificacion de la operación previa.
	 // en universal solo se informa en anulaciones y es con numope
	 // en privada se informa en confirmaciones, devoluciones y anulaciones y es con num autorizacion
     char    FEC_CONT[8];        // fecha de la sesión   
     infoprivada RSPSTA_DTFNO;   // respuesta al datafono op privada financiada, datos de salida
     char    COD_MNDA_CONCI[3];  //CODIGO DE MONEDA CONCILIACIÓN P50 
     char    IMP_MNDA_CONCI[12]; // IMPORTE EN MON CONCILIACION (P05)
     char    TSA_MNDA_CONCI[8];  // TASA CONVERSION MONEDA CONCILIACION (P09)
     char    COD_MNDA_TITU[3];   //CODIGO DE MONEDA DEL TITULAR (P51)
     char    IMP_MNDA_TITU[12];  // IMPORTE EN MON DEL TITULAR (P06)
     char    TSA_MNDA_TITU[8];   // TASA CONVERSION MONEDA TITULAR (P10)
     char    IMP_CASH[12];       // IMPORTE CASH EN MONEDA p4
	 char    TASA_ATM[9];        // TASA ACCESO ATM (ACCESS FEE) Signo + importe de 8
     char    NOM_TITU[30];       //NOMBRE DEL TITULAR (dato de salida)
     char    IMP_CMSN[8];        //IMPORTE DE LA comision de la operacion
	 char    TIP_COM_AD[1];      //TIPO DE COMISION ADICIONAL 1exacta 2calculada
	 char    IMP_COM_AD[5];      //IMPORTE DE LA COMISION ADICIONAL  RDCAJEROS
     char    NOM_LOC_ESTB[40];   //nombre y localizacin del establecimiento (P43)
	 char    TASAS[44];          // importe de cuotas por las tasas que aplica la red (P46)
	 char    COD_OPERACION[2];    // 01-1100 preautorizacion  //DMND0036176
	                              // 02-1220 confi de preaut
								  // 03 - si llega informado el telefono p48.24
								  // 04 - si el codproceso=90 cambio de pin
     char    CTA_OPERATORIA[24];  // (4)IBAN (20) CCC
     excneg  DTS_EXCNEG;
     datosP48 DTS_ADCNLS;    //DATOS ADICIONALES P48  ////PRJ0247167 no se modifica
	 char    COD_CONCILIA[1]; //1-CONCILIADO 2-NO CONCILIADO
     char    IND_AUTO_PARCIAL[1]; // 1-FLAG DE ADMITE AUTORIZACION PARCIAL
     char    IMP_PARCIAL[12]; // IMPORTE PARCIAL AUTORIZADO (P48.93)	 
     datosP4873 DTS_COTALS;    //DATOS CONTADORS DEL CONTALES
    
}formato03;

typedef struct datosAdicionales{
     char OFFSET [12];
     char IDIOMA [2];
     char NEW_OFFSET [12];
     char NEW_IDIOMA [2];
     char CVV [3];
     char TIP_COMPEN [1];
     char NUM_TFNO [9];
     char COD_BLOQMVT [1]; //si es consulta de movimiento estar� entre 1 y 5
     p48saldos DAT_SALDOS;
     p48mvmtos DAT_MOVIMIENTOS;
     char IMP_NETOS [180]; //desglose de importes netos P48.15
     char OTROS_DATOS[86];
} datosAdicionales;


//copy sioapl
typedef struct formatoNuevo{
     char    IDE_CD[16];           // identificacion operacion
     char    IDE_CD_ORI[16];       // identificacion del original de la operacion
     char    CAB_CTRL_IOO[6];      // identificacion de ordenador origen
     char    CAB_CTRL_IOD[6];      // identificador de ordenador destino  parte 1 y parte 2
     char    CAB_CTRL_IAP[4];      // identificacion de aplicacion
     char    CAB_CTRL_IOP[6];      // identificador logico de terminal y numero de serie del mensaje
     char    CAB_CTRL_NB[2];       //numero de bloque
     char    CAUR[2];              // codigo de rechazo
     // PRJ0202598 cambio bin de 6 posiciones 8 se usa en el ftrad03.c pero ya era de 8 posiciones
     char    BIN1[8];              // byte 1 de indicadores 
     char    BIN2[8];              // byte 2 de indicadores 
     char    BIN3[8];              // byte 3 de indicadores
     char    IDTM[4];              // identificador tipo de mensaje
     char    MBIT_P[64];           // mapa de bits primario
     char    PAN_L[2];             // número de tarjeta visa o 4b
     // PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
     char    PAN_D[18];            // número de tarjeta con espacios detras 
     char    CODPROC[6];           // código de proceso
     char    IMPORTX[8];           // importe en moneda original con ceros por delante
     char    IDEN_USR_L[2];        // identificacion de usuario: 
     char    IDEN_USR_TIPO[2];     // identificacion de usuario: tipo
     char    IDEN_USR_DATO[30];    // identificacion de usuario: si tipo=01, cuenta en formato BBBB-OOOO-CCCCCCCCCC
     char    IMPOROP[12];          // importe de la operacion
     char    IDTR[6];              // identificacion del mensaje/transaccion original
     char    FECHAL[6];            // fecha de la transaccion (AAMMDD)
     char    CMONTI[4];            // moneda de facturacion
     char    HORAL[6];             // hora de la transaccion (HHMMSS)
     char    FECCAD[4];            // fecha de caducidad de la tarjeta
     char    ACCION_TARJ[2];       // estado de tarjeta o acciones a realizar si hay denegacion
     char    DAT_DEP_EFE[4];       // datos del ingreso en efectivo
     char    COMINF_TIPO[2];            // tipo de comisiones informadas al cliente en operaciones reintegro     
     char    COMINF_IMPO[6];            // importe comisiones informadas al cliente en operaciones reintegro
     char    SECTOR[4];            // codigo de actividad del establecimiento
     char    ID_REDTERM[12];       // identificación de la red y del terminal origen del mensaje
     char    CODRAZ_ANUL[2];       // Codigo de razón de la anulación
     char    NUM_MIEMB[1];         // numero de miembro de la familia
     char    ICARGOS[12];          // total de cargos en la moneda indicada en bit8
     char    NCARGOS[10];          // numero de cargos
     char    IABONOS[12];          // total de abonos en la moneda indicada en bit8
     char    NABONOS[10];          // numero de abonos
     char    I_ANUL_CAR[12];       // total de anulaciones de cargo en la moneda indicada en bit8
     char    NUM_ANUL_CAR[10];     // numero de anulaciones
     char    I_ANUL_ABO[12];       // total de anulaciones de abono en la moneda indicada en bit8
     char    NUM_ANUL_ABO[10];     // numero de anulaciones de abono
     char    DAT_ABO_COM[36];      // datos de abono a comercio
     char    INF_ADIC_PET_TRANSF[32];   // informacion adicional peticion transferencia
     char    CODRES[2];            // Código de respuesta
     char    IDEN_BLOQUE_MOV[12];  // identificacion bloque de movimientos //PRJ0258542 - lo utilizamos para importe en moneda extranjera
     char    ADIC_RESP_TRANSFER[42]; //informacion adicional respuesta transferencias
     char    DATORI[30];           // Campos del mensaje original
     char    NUMAUT[6];            // Número de autorización
     char    INF_SALDO[110];        // informaciones de ssaldos
     char    INF_MOV[222];          // informacion de saldo y 5 ultimos movimientos
     char    NUM_REF[8];           // Número de operación (de referencia)
     char    REF_TPV[16];          // referencia tpv
     char    MON_CTBLE[4];         // moneda contable
     char    TOTAL_TELEBANCO[218]; // datos de totalizacion del telebanco
     char    IMP_TERMINAL[14];     // importe bruto y signo de la totalizacion de terminal  en establecimiento
     char    DAT_ADIC_ENV_TRANSP[404];  // Datos adicionales enviados funcion transparente
     char    NOMEST[40];           // Establecimiento
     char    DAT_ADIC_REC_TRANSP[404]; // datos adicionales recibidos en funcion transparente
     char    MODAUT_TERM[4];       // Modo de autorización
     char    BIT_P_51[974];        // BIZUM codigos operacion
     char    MAC[6];               // Código de autentificación del mensaje (MAC)
     char    CLAVE_MAC[2];         // Identificación de clave de cifrado
     char    TOT_TRASPASOS[12];
     char    NUM_TRANSPASOS[10];
     char    TOT_ANUL_TRASPASOS[12];
     char    NUM_ANUL_TRASPASOS[10];
     char    BIT_P_58[94];
     char    BIT_P_59[186];
     char    FEC_REF_TOTALES[6];
     char    IND_CUADRE_TOT[2];
     char    STATUS_TELEBANCO[20];
     char    INF_OP_OT_REDES[40];   // Datos adicionales en operaciones realizadas en otras redes
     char    PUNT_SERV[12];
     char    EMISOR[2];
     char    CTA_EMISOR[18];
/////////////// nuevos que no se de donde vienen
	char    COD_FUN[3];       //DMND0036176 donde ira el codigo de funcion p24 a 150 para las preautorizaciones    
	char    COD_ACTI[4];        
	char    FEC_SESS[8];         
	char    REF_ADQ[23];         
	char    IDE_ADQ[11];         
	char    IM_CUO[44];          
	char    INF_CTRL_SEG[16];     
	char    IDENT_AUT[11];       
	char    IND_SIO_PRI[1];  
	char    IM_CUO_ABO[12]; 
	char    IM_CUO_CAR[12];
    char    CONCILI[1];
	char    NUMOPER_SISMA[6];
	//PRJ0247167 - Revisado ampliación P48 - Nuevo campo para pasar a MEPA
	char 	VALOR_P48_20[16];
	//PRJ0257005 - Se meten el P48.94 de 6 caracteres y el P62.16 de 30 caracteres - El final baja de 181 a 145
	char	VALOR_P48_94[6]; 	// Identificador de operaciones especiales
	//char	VALOR_P62_16[30]; 	// PAN tokenizado y caducidad de la tarjeta tokenizada
	char	SIOAPL_TARJTOKEN[16]; //Solo se cogerá tarjeta y caducidad porque se ha definido así en MEPA (SIOAPL)
  char  SIOAPL_FECATTOKE[4];
	char	SIOAPL_TIPMOV[1];
	char	SIOAPL_CODIGO_RES[3];
	char	SIOAPL_IBAN_ORDE[34];
	char	SIOAPL_IBAN_BENE[34];
	char  SIOAPL_OFFSET[4]; //PRJ0258542 - para pasar el offset a SF, quito 4 de abajo
	char 	SIOAPL_CODRAZ[4]; //PRJ0350720 - para pasar el codigo de razon a SF, quito 4 de RELLENO_FINAL, que estaba en 69 y se queda en 65
	char  RELLENO_FINAL[65]; // Si quito el de 30 pasa de 145 a 155 - nuevos campos BIZUM 155 -73
 } formatoNuevo;
 
#endif
/* De FTRADRESO_H */

