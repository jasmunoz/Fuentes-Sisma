#ifndef CFORITNO_H
#define CFORITNO_H
/***
   FICHERO: CFORITNO.H
   DESCRIPCION: Fichero con la estructura del formato interno
                de la aplicacion.

***/

#include "cnetplus.h"

typedef struct fecha_6
{
   char     aa[3];
   char     mm[3];
   char     dd[3];
} fecha_6;

typedef struct cripto
{
    char idmac[6];
    char idarqc[6];
}cripto;
 typedef struct datos_procesales
 {
    char          finumope[7];     /* numero de operacion */
    char          fifecha0[8];     /* fecha en la aplicacion */
    char          fihora00[8];     /* hora en la aplicacion */
    char          fiorigen[3]; /* origen */
    char          firesfin[3]; /* resolutor final */
    char          firespri[3]; /* resolutor primario */
    char          finetpri[5]; /* entidad resolutora primaria */
    char          ficodfin[4]; /* codigo de finalicacion */
    char          fitipcla[3]; // tipo de clave de operaciones
                               // 11 - tjt propia
                               // 88 - op sin tarjeta (test y cierre)
                               // 44 - tjt ajena (op de titular de tjt)
                               // 22 - op de abono a comercios propios
    char          ficlatab[13]; /* clave de entrada tabla de ope. */
    char          fitipred[1];          /* Tipo redefinicion de mensaje utilizada */

    char          fitienor[9];     /* tiempo de entrada al origen */
    char          fitisare[9];     /* tiempo de salida del resolutor */
    char          fitienre[9];     /* tiempo de entrada al resolutor */
    char          fitipcue[3];  /* tipo de cuenta    */
    char          fisubres[3];  /* subaplicacion resolutora */
    char          fisubori[3];  /* subaplicacion origen */
    char          fiusofut[31];   /////////// USO FUTURO ////////////////
    char          ficomadi[5];    // añado comision adicional necesario para RDCAJEROS 48.31
    char          fiparaut[3];  /* Parametros por bin */
    char          fitipcom[2];  /* tipo comisión a aplicar 1, exacta, 2, máxima, y 9 no especificada   */
    char          ficomisi[9];   /* imp. comision a aplicar */
    char          fipidori[6]; /*  Se utiliza para: ID de la thread origen (5 posiciones) o ID del proceso origen */
    char          finumprv[7]; // numero de operacion de la operacion original previa [6+1]
    char          finototal[1]; // 'N' indica que esa operacion no totaliza
    char          fioriini[3]; // subsistema origen inicial (para que no se pierda cuando fiorigen pasa a 50 si acaba en diferidos)
    char          firesprs[4]; // respuesta del resolutor (para que no se pierda cuando respusal se traduce en el EO)
    char          fipancif[20]; // pan cifrado longitud igual que iso02
    cripto        fimacarq; /* etiquetas para validar mac,arqc y arpc en HSM */
    char          fiaremac[101]; /* los isos necesarios para calcular el mac */

 } datos_procesales;


 typedef struct iso_002
 {
   char    longitud[3];
   char    datos[20];
  } iso_002;

 typedef struct iso_003
 {
   char    dig12[3]; /* tipo operación*/
   char    dig34[3]; /* cta cargo/cta abono*/
   char    dig56[3];
  } iso_003;

 typedef struct iso_origen
 {
   char    longitud[3];
   char    datos[12];
  } iso_origen;

 typedef struct iso_fecha
 {
   char aa[3];
   char mm[3];
   char dd[3];
   char hh[3];
   char mi[3];
   char ss[3];
  } iso_fecha;


 typedef struct iso_014
 {
   char    aa[3];
   char    mm[3];
  } iso_014;


typedef struct iso_035
{
   char   longitud[3];
   char   datos[38];
} iso_035;

typedef struct iso_044
{
   char   longitud[3];
   char   datos[65];
} iso_044;

typedef struct iso_045
{
   char   longitud[3];
   char   datos[77];
} iso_045;


typedef struct iso_046
{
   char   longitud[3];
   char   datos[45];
} iso_046;


typedef struct iso48contr
{
   char   cardia[9];
   char   ctrlof[3];
} iso48contr;

typedef struct iso48saldos
{
	/*** PRJ0247167 se substituye la estructura iso48saldos por la que hay en cforitno.h de /sf/sisma/includes
   char   resolutor[2];
   char   mcredito [3];
   char   scredito [1];
   char   credito  [12];
   char   mdebito  [3];
   char   sdebito  [1];
   char   debito   [12];
   char   mdiario  [3];
   char   sdiario  [1];
   char   diario   [12];
   char   delimi   [1];//delimitador
  ***/
  
   char   num_ctas [1]; 
   char   saldo_dat[4][24];
   char   delimi   [1];//delimitador
  // FIN MODIFICACION  PRJ0247167
  
} iso48saldos;


typedef struct iso48comun01
{
 char   offset[13]; //48.01
 char   idioma[3];  //48.02
 char   cvv000[4];  //48.08
 char   termin[13]; //identificacion comercio o terminal, se manda a Redsys en el 48.09 
 char   fiiso4813[4]; //tipo de tratamiento
 char   fiiso4816[7]; //fecha sesión del establecimiento
 char   fiiso4817[7]; //identificación del establecimiento
////PRJ0247167  se cambia de 3 a 15 siempre se pone uno mas en linux por los '\0'
 ////char   fiiso4820[3]; //calificación de riesgo
 char   fiiso4820[16]; 
 char   fiiso4821[51]; //texto explicativo
 char   flagImpParcial[2]; //48.48 flag importe parcial 
 char   importeParcial[13]; // 48.93 importe parcial
 ///char   usofut480a[38]; //espacio para uso futuro
 char   contadores73[20]; // para guardar los contadores del contacless, lo snuevo del 48.73
 char   usofut480a[18]; //espacio para uso futuro
 char   telefo[10]; //numero de telefono, 48.24
 char   rref[7]; //resto de referencia, para las recargas 48.25
 char   compen[2]; //compensacion 48.26
 char   usofut480b[79]; //espacio para uso futuro
 char   fiiso4833[12]; // referencia transf y trasp /////////////// TRANSFERENCIAS Y TRASPASOS
 char   fiiso4854[10]; // tasa de acceso a atm (access fee) 48.54 
                       // 1 - signo (D,C)
                       // 8 - importe en la misma moneda que p04
                       // 1 - fin de cadena
 char   fiiso4861[10];// gastos al cliente transf y trasp ////////// TRANSFERENCIAS Y TRASPASOS  
} iso48comun01;

typedef struct iso48sb06
{
/*** PRJ0247167 se substituye la estructura iso48sb06 por la que hay en cforitno.h de /sf/sisma/includes
 char   moneda   [3];
 char   movimi[6][37];
 char   delimi   [1];//delimitador
***/
  
 char   fecha    [6];
 char   signo    [1];
 char   importe  [12]; 
 char   moneda   [3];
 char   num_movi [1]; 
 char   movimi[5][37];
 char   delimi   [1];//delimitador
 // FIN MODIFICACION  PRJ0247167

}iso48sb06;

typedef struct iso48movim
{
 iso48comun01 comun01;
 iso48sb06 sub04806;
 char bloqmo[3];
} iso48movim;


typedef struct iso48augen
{
   iso48comun01 comun01; /* deja espacio para campos comunes */
   iso48saldos  saldos;
   char fiiso4869[13];         //tasa de intercambio con decimales de exactitud  
                             //(2 primeros tipo cuota, 1 signo, importe de 3 enteros y 6 decimales)
} iso48augen;

typedef struct iso48noeco
{
   iso48comun01 comun01;
   char   nuevof[13];
   char   nuevid[3];
} iso48noeco;

typedef union iso_048
{
   iso48comun01 comun01; //mensaje 1100, y en la respuesta de recarga
   iso48contr    contr; //mensajes de control
   iso48augen    augen; //autorizacion, incluye los saldos
   iso48noeco    noeco; //cambio pin, cambio idioma
   iso48movim    movim; //consulta de movimientos
} iso_048;

typedef struct iso055Resp
{
    //los datos de salida los guardamos en BCD, ya que es como le interesa a Sermepa
    //montamos los tags con la long y el dato
    char tag_71 [129];
    char tag_72 [129];
    char tag_91 [17];
    char tag_8A [3];
}iso055Resp;

typedef struct iso055Tag
{
//vamos a considerar que en principio NO los queremos en BCD, y los ponemos en el orden de D-1
    char tag_9F02 [12];
    char tag_9F03 [12];
    char tag_9F1A [4];
        char tag_95 [10];
    char tag_5F2A [4];
    char tag_9A [6];
    char tag_9C [2];
    char tag_9F37 [8];
    char tag_82 [4];
    char tag_9F36 [4];
    char tag_9F10 [64];
    char tag_9F17 [1];
    char tag_9F26 [16];
    char tag_9F27 [2];
    char tag_9F33 [6];
    char tag_9F34 [6];
    char tag_9F6E [8]; //esta no se que es pero llega
}iso055Tag;



typedef union p_055
{
    // aqui pongo las etiquetas
    iso055Tag   tag55;  // peticion para 55
    iso055Resp  resp55; // respuesta para 55
    char max055 [257];
}p_055;

/* expasion de bytes etiqueta 95 y 9F10*/

typedef struct bytes_tag55
{
  char fiibit1_95[9];/* BYTES TAG 95 */
  char fiibit2_95[9];
  char fiibit3_95[9];
  char fiibit4_95[9];
  char fiibit5_95[9];
  char fiibit6_95[9];
  char fiibit7_95[9];
  char fiibit8_95[9];
  char fiibit1_9F[9];/* BYTES TAG 9F10 */
  char fiibit2_9F[9];
  char fiibit3_9F[9];
  char fiibit4_9F[9];
  char fiibit5_9F[9];
  char fiibit6_9F[9];
  char fiibit7_9F[9];
  char fiibit8_9F[9];
}bytes_tag55;

typedef  struct iso_055
{
   char  longitud[4];
   char  carcontrol[2];
   p_055   datos;
   bytes_tag55 bytes_55;
} iso_055;

typedef struct iso_056
{
   char   longitud[3];
   char   datos[36]; /*SIO-6 + SIO-7+ SIO-10 + SIO-19*/
 } iso_056;

typedef  char iso_mac[9] ;

typedef  struct iso_072
{
  char  longitud[4];
  char  datos[257];
} iso_072;

typedef  struct iso_101
{
   char  longitud[3];
   char  datos[18];
} iso_101;

typedef  struct iso_031
{
   char  longitud[3];
   char  datos[24];
} iso_031;

typedef  struct iso_102
{
   char  longitud[3];
   char  datos[29];
} iso_102; 


typedef struct iso_comunes
{
    char          fiiso000[5];
    char          fiidtran[7];     /* num. de transacion (ISO-11) (SIO-6) */
    iso_fecha       fifechor;     /* fecha y hora local AAMMDDHHMMSS (ISO-12) (SIO-7 + SIO-10)*/
    iso_origen      fi032094;     /* adquirente/ origen*/
    char          fiiso024[4];     /* código función*/
    char          respusal[4]; /* codigo de respuesta a enviar (ISO-39) (SIO-33) */
 } iso_comunes;


 typedef struct iso_autorizaciones
 {
    iso_002            finumPAN; /*Tarjeta (ISO-002) (SIO-001) */
    iso_003       fiaut003; /*Código proceso (ISO-003) (SIO-002) */
    char             fiaut004[13]; /*Importe transacción SIO-3 */
    char             fiaut005[13]; /*Importe transaccion en msj 0100 SIO-5 */
    char             fiaut006[13]; /*Importe transaccion */
    char             fiaut009[9];
    char             fiaut010[9];
    iso_014            fifeccad; /*Fecha Caducidad (ISO-014) (SIO-011)*/
    char             fiaut018[5]; /*Código actividad SIO-18*/
    char             fiaut019[4]; /* Codigo Pais */
    char             finumita[3]; /*Número miembro tarjeta SIO-21*/
    char             fiaut022[13];
    char             fiaut023[4];/* numero de secuencia de la tarjeta EMV*/
    char             fiaut025[5]; /*Código razón de anulación SIO-20*/
    char             fiaut026[5];
    char             fiaut027;
    fecha_6       fiaut028;         /*fecha de sesion */    
    char             fiaut029[4]; /*número de sesion con el host*/
    char             fiaut030[25]; //12 + 12 +fin de cadena //importes originales transaccion y conciliación
    iso_031             fiaut031; // referencia de clearing
    iso_origen       fiaut033; // presentador de la operación    
    iso_035          fiaut035;
    char             fiaut037[13]; /*Número referencia SIO-40 */
    char             fiaut038[7];  /*Número autorización SIO-37*/
    char             fiaut040[4];
    char             fiaut041[13]; /*Id. terminal y la Red SIO-19*/
    char             fiaut042[16]; /*Establecimiento*/ 
    char             fiaut043[41]; /*Nombre y localidad del Establecimiento SIO-48*/
    iso_044            fiaut044;
    iso_045            fiaut045;
    iso_046            fiaut046;
    iso_048            fiaut048;
    char             fiaut049[4]; /*Moneda transacción SIO-8*/
    char             fiaut050[4];
    char             fiaut051[4];
    char             fiaut052[17]; //PRJ0258542 - pasa de 8 a 16- aquí se añade uno mas al cuenteo
    char             fiaut053[17]; /*Posiciones 9 y 10 son clave cifrado SIO-53 */
    iso_055            fiaut055; /* datos de aplicaciones sobre EMV */
    char             fiaut055_host[259]; /*para el host*/
    iso_056            fiaut056; /*Elemento de datos originales SIO-36 = SIO-6 + SIO-7 + SIO-10 + SIO-19*/
    iso_origen         fiaut058;
    iso_mac            fiaut064; /*MAC SIO-52 */
	////////////// TRANSFERENCIAS Y TRASPASOS ///////////////
    iso_102            fiaut102; //cuenta origen
    iso_102            fiaut103; //cuenta destino con el mismo formato que la origen
    iso_mac            fiaut128; //mac si hay 2º mapa de bits 
    
} iso_autorizaciones;

 typedef struct iso_ficheros
 {
    iso_002          fific002; /* Identificador de la tarjeta SIO-1*/
    char           fific053[17]; /*Posiciones 9 y 10 identificador de la clave cifrado SIO-53 */
    iso_072          fific072; /* registro de datos, para ficheros */ 
    iso_origen       fific093;
    iso_101          fific101; /*Campo usado para le respuesta NETplus->Cardplus a un msj. 9304(9314) de act.ficheros*/
    iso_mac          fific128; 
 } iso_ficheros;

 typedef struct iso_administrativos
 {
    iso_072          fiadm072;
    iso_origen       fiadm093;

 } iso_administrativos;

 typedef struct iso_control
 {
    char             codmcd[3]; /* Código de MCD SIO-1*/
    char             codopcion[2]; /* Código de opción SIO-1 -> 0 Al recuperarse la comunicacion, reanudar envio por linea de los movimientos off*/
                                                //       -> 1 Al recuperarse la comunicacion NO reanudar envio por linea de los movimientos off
    char             dispocontrol[10]; /* Campo de mensajes de control de rearranque SIO -> en principio envio lo mismo que me envian y si parte
                                             origen  netplus  lo relleno a ceros */
    fecha_6          fictr028;
    char           fictr029[4];
    iso_origen       fictr093;
    iso_048          fictr048;
 } iso_control;

 
 typedef struct iso_totales
 {
    char               monedafac[4]; /*Moneda facturación SIO-8 */
    char               monedacon[4]; /*Moneda contable SIO-44 */
    fecha_6            fitot028; /* Fecha de referencia de la totalización por el banco SIO-60 */
    char             fitot029[4];
    char             fitot074[11]; /*número, abonos  SIO-26*/
    char             fitot075[11]; /*abonos, número de anulaciones SIO-30*/
    char             fitot076[11]; /*cargos, número  SIO-24*/
    char             fitot077[11]; /*cargos, número anulaciones SIO-28*/
    char             fitot086[17]; /*abonos, importe  SIO-25 */
    char             fitot087[17]; /*abonos, importe anulación SIO-29*/
    char             fitot088[17]; /*cargos, importe SIO-23*/
    char             fitot089[17]; /*cargos, importe anulaciones SIO-27 */
    char             fitot097[18]; /* valor neto */
    char             fitot109[17]; /*abono, importe de cuotas */
    char             fitot110[17]; /*cargo, importe de cuotas*/
    char             fitot061[3];  /*indicador cuadre totales SIO-61 */
    char             fitot4815[181]; // desglose de importes netos por tipo de compensacion 
                                     // 10 elementos de 18 posiciones
                                     // cada elemento 1 - tipo compen
                                     //               1 - signo
                                     //               16- importe  
 } iso_totales;



typedef union resto_iso
{
  iso_autorizaciones  fiisoaut;
  iso_ficheros        fiisofic;
  iso_administrativos fiisoadm;
  iso_control         fiisoctr;
  iso_totales         fiisotot;
} resto_iso;

typedef struct datos_privados
{
     char tarifario[4];    // NUMERO DE TARIFARIO
     char formapago[4];    // NUMERO DE LA FORMA DE PAGO
     char impopfi[12];     //IMPORTE DE LA operación financiada  
     char numcuotas[3];    //NUMERO DE CUOTAS
     char impcuota[12];    //IMPORTE DE LA CUOTA
     char fecprmvto[8];    //FECHA PRIMER VENCIMIENTO
     char porintrs[7];     //INTERES
     char portae[7];       //TAE
     char impcomestd[12];  //COMISION DE ESTUDIO
     char porintrsmor[7];  //INTERES DE MORA
     char portaemor[7];    //TAE DE MORA
     char impcommor[12];   //COMISION MORATORIA
     char impseguro[12];   //SEGURO
     char imptotopfi[12];  //IMPORTE TOTAL DE OPERACION
     char indcmpragrts[1]; //INDICADOR COMPRA GRATIS
     char nomtitu[30];     // NOMBRE DEL TITULAR
      
} datos_privados;

 typedef struct formato_interno
 {
    datos_procesales fidatpro;
    iso_comunes      fiisocom;
    resto_iso        fidatiso;
    datos_privados   fiprivado;
 } formato_interno;


#endif
/* De CFORITNO_H */

