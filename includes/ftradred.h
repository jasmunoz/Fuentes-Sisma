//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
//BIZUM      - Revisado modificacion P48 sub 102 y 103 a 34 pos --25/04/2024 de momento se mantiene a 28
/****************************************************************
   Campos y definiciones de uso por el traductor de PRICE   
****************************************************************/

#include "cnetplus.h"
#include "cforitno.h"
#include "msg_nucl.h"
/*   definicion de valores para las condiciones de cada bit   */
#define  DESCONOCIDO  -1
#define  NO_USADO      0
#define  PROHIBIDO     1
#define  OBLIGATORIO   2
#define  OPCIONAL      3
#define  CONDICIONAL   4

/*   definicion de valores para los tipos de mensajes    */
#define  PETICION_NO_CONTROL 1
#define  PETICION_CONTROL    2
#define  RESPUESTAS_NUCLEO   3
#define  RESPUESTAS_OTROS    4
#define  PETICION_CIERRE 9
#define  RESPUESTA_CIERRE 10
#define  ADMINISTRATIVO 11



/***-------   campos del formato intermedio     ----------***/
//RAQUEL aqui vienen todos los bits que tenemos en uso ahora en unos u otros mensajes
// hay que cambiar el tamaño del bit 48 y si apareciera algun bit nuevo en uso 
//ej: el 15 que ahora no esta, habria que añadirlo aqui

typedef struct formato_intermedio
{
/***----------------------------------------  tipo de mensaje  ------***/
   char        adipre000     [4];
/***----------------------------------------------------  PAN  ------***/
   char        adi002l       [2];
   char        adi002d       [19];
/***--------------------------------------  codigo de proceso  ------***/
   char        adipre003     [6];
/***-----------------------------  importe moneda transaccion  ------***/
   char        adipre004     [12];
/***-----------------------------  importe moneda conciliacion  ------***/
   char        adipre005     [12];
/***-----------------------------  importe moneda titular        ----***/
   char        adipre006     [12];
/***-----------------------------  tasa de conversion a moneda conciliacion  ------***/
   char        adipre009     [8];
/***-----------------------------  tasa de conversion a moneda titular  ------***/
   char        adipre010     [8];


/***----------------  numero de identificacion de transaccion  ------***/
   char        adipre011     [6];
/***---------  fecha y hora local(en terminal) de transaccion  ------***/
   char        adipre012     [12];

/***-------------------------------------  fecha de caducidad  ------***/
   char        adipre014     [4];

/***-------------------------------------  fecha de conversion a moneda de conciliacion  -***/
   char        adipre016     [4];
/***-------------------------------------  fecha de captura  ------***/
   char        adipre017     [4];

/***-------------------------------------  codigo actividad establecimiento ------***/
   char        adipre018     [4];
/***-------------------------------------  codigo de pais de adquirente ------***/
   char        adipre019     [3];
/***--------------------------------  datos punto de servicio  ------***/
   char        adipre022     [12];
/***--------------------------------------  numero de secuencia de la tarjeta  ------***/
   char        adipre023     [3];
/***--------------------------------------  codigo de funcion  ------***/
   char        adipre024     [3];
/***-----------------------------  codigo de razon de mensaje  ------***/
   char        adipre025     [4];
/***-----------------------  codigo actividad internacional establecimiento ------***/
   char        adipre026     [4];
/***-----------------------  longitud del numero de autorizacion------***/
   char        adipre027     [1];
/***----------------------------------  fecha de conciliacion  ------***/
   char        adipre028     [6];
/***---------------------------------  numero de conciliacion  ------***/
// De momento no las quitamos
   char        adipre029     [3];
/***---------------------------  importe original transaccion  ------***/
   char        adipre030     [24];
/***-----------------------  Cave de referencia de adquirente  ------***/
   char        adi031l       [2];
   char        adi031d       [23];
/***-----------------------  codigo identificacion adquirente  ------***/
   char        adi032l       [2];
   char        adi032d       [11];
/***---------------------------  codigo de identificacion del presentador ---***/
   char        adi033l       [2];
   char        adi033d       [11];
/***------------------------------------------  datos pista 2  ------***/
   char        adi035l       [2];
   char        adi035d       [37];
/***-----------------------------------  numero de referencia  ------***/
   char        adipre037     [12];
/***---------------------------------  numero de autorizacion  ------***/
   char        adipre038     [6];
/***---------------------------------------  codigo de accion  ------***/
   char        adipre039     [3];
/***---------------------------------------  codigo de servicio  ------***/
   char        adipre040     [3];
/***--------------------------------  identificacion terminal  ------***/
// De momento no las quitamos
   char        adipre041     [8];
/***-------------------------  identificacion establecimiento  ------***/
   char        adipre042     [15];
/***---------------------------nombre y localidad del establecimiento -***/
   char        adipre043     [40];
/***----------------------------  datos adicionales respuesta  ------***/
// De momento no las quitamos
   char        adi044l       [2];
   char        adi044d       [64];
/***----------------------------  datos pista 1  ------***/
   char        adi045l       [2];
   char        adi045d       [76];
/***----------------------------  datos adicionales respuesta  ------***/
   char        adi046l       [2];
   char        adi046d       [44];

//
/***-----------------------------  datos privados adicionales  ------***/
   char        adi048l       [3];
  //PRJ0247167 - Revisado ampliación P48
  // char        adi048d       [256];
   char        adi048d       [999]; //PRJ0257005 - Revisado tokenizacion

/***---------------------------------------  codigo de moneda transaccion  ------***/
   char        adipre049     [3];
/***---------------------------------------  codigo de moneda conciliacion ------***/
   char        adipre050     [3];
/***---------------------------------------  codigo de moneda titular  ------***/
   char        adipre051     [3];
/***---------------------------------------------  bloque PIN  ------***/
   char        adipre052     [16]; //PRJ0258542 - cambia de 8 a 16
/***---------------------  informacion de control y seguridad  ------***/
   char        adipre053     [24]; //PRJ0258542 - cambia de 16 a 24 - en mensajes normales sera de 16 en mensajes de intercambio de claves sera de 24
/***-----------------------------  datos de aplicaciones sobre microprocesador ICC ------***/
   char        adi055l       [3];
   char        adi055d       [256];
/***--------------------------  elementos de datos originales  ------***/
   char        adi056l       [2];
   char        adi056d       [35];
/***-----------------------  periodo de preautorizacion  ------***/
   char        adipre057     [3];
/***-----------------------  codigo de identificacion del autorizador  ------***/
   char        adi058l       [2];
   char        adi058d       [11];

/***--------------------------  datos monedero electronico  ------***/
   char        adi062l       [3];
   char        adi062d       [256]; //PRJ0257005 - Revisado tokenizacion
/***-------------  codigo de autentificacion del mensaje(MAC)  ------***/
   char        adipre064     [8];
/***-------------  numero de mensaje en el lote  ------***/
   char        adipre071     [8];

/***--------------------------------------  registro de datos  ------***/
   char        adi072l       [3];
   char        adi072d       [256];
/***---------------------------------------  numero de abonos  ------***/
   char        adipre074     [10];
/***-----------------------  numero de abonos por anulaciones  ------***/
   char        adipre075     [10];
/***---------------------------------------  numero de cargos  ------***/
   char        adipre076     [10];
/***-----------------------  numero de cargos por anulaciones  ------***/
   char        adipre077     [10];

/***-----------------------  transacciones de servicio, numero  ------***/
   char        adipre080     [10];
/***-----------------------  autorizaciones, numero  ------***/
   char        adipre081     [10];
/***-----------------------  cobros pagos, numero  ------***/
   char        adipre085     [10];
/***--------------------------------------  importe de abonos  ------***/
   char        adipre086     [16];

/***----------------------  importe de abonos por anulaciones  ------***/
   char        adipre087     [16];
/***--------------------------------------  importe de cargos  ------***/
   char        adipre088     [16];
/***----------------------  importe de cargos por anulaciones  ------***/
   char        adipre089     [16];
/***-----------------------  codigo de identificacion destino  ------***/
   char        adi093l       [2];
   char        adi093d       [11];
/***------------------------  codigo de identificacion origen  ------***/
   char        adi094l       [2];
   char        adi094d       [11];
/***------------------------  datos de referencia de emisor  ------***/
   char        adi095l       [2];
   char        adi095d       [56];
//PRJ0258542 - INI
/***------------------------  datos de las nuevas claves  ------***/
 	 char				 adi096l			 [3];
 	 char				 adi096d			 [360];
//PRJ0258542 - FIN
/***------------------------------------------  posicion neta  ------***/
   char        adipre097     [17];
/***--------------------------------------  nombre de fichero  ------***/
   char        adi101l       [2];
   char        adi101d       [17];
/***--------------------------------------  identificacion de cuenta origen ------***/
   char        adi102l       [2];
   char        adi102d       [28];
   
/***--------------------------------------  identificacion de cuenta destino ------***/
   char        adi103l       [2];
   char        adi103d       [28];

/***-------------------------------------- texto de la transaccion ------***/
   char        adi104l       [3];
   char        adi104d       [256];
/***------------------------------------------ abonos, importe de retrocesion **/
   char        adipre105     [16];
/***------------------------------------------ cargos, importe de retrocesion **/
   char        adipre106     [16];

/***------------------------------------------ abonos, numero de retrocesion **/
   char        adipre107     [10];
/***------------------------------------------ cargos, numero de retrocesion **/
   char        adipre108     [10];

/***----------------------------importe de cuotas a abonar ------***/
    char        adipre109     [16];
/***----------------------------importe de cuotas a cargar ------***/
    char        adipre110     [16];
/***-------------  codigo de autentificacion del mensaje(MAC)  ------***/
   char        adipre128     [8];
}  formato_intermedio;

/***---   campos de entrada y salida de los raices recpuc y envpuc  ---***/
/*--- mensaje hacia o desde la linea (colas del manejador) ---*/
typedef struct mensaje_comunicacion
{
	long           tipo;
	//PRJ0247167 - Revisado ampliación P48
	//DMND0037271 el mensaje no cambia de tamaño porque es suficiente 
	//a pesar de la ampliacion del p48, se cambian los campos de las tablas
	//NPTCASA, NPTLOGO y NPTDIFE para que graben los 4000
	//char    mensaje_linea[2000];
      char           mensaje_linea[4000];
} mensaje_comunicacion;

/***-------   campos de uso general     ----------***/
//DMND0037271 no es necesario cambiarle el tamaño
char  cadena_verif[2000];


/***-------   campos para llamar a ftradpuc     ----------***/
char            usotrad;
int             sentido;
int             long_iso;
int             suba_iso;
//PRJ0247167 - Revisado ampliación P48
//char            mensaje_puc[2000];
char            mensaje_puc[4000];
formato_interno   mensaje_netplus;
int             error_code;
int             error_bit;


/***---------------   tablas de datos iso y aplicacion  -----------------***/
typedef struct formato_tabla_iso
{
char   tipo_fs;
int    long_fs;
int    max_fs;
char   mac_fs;
}      formato_tabla_iso;

formato_tabla_iso   tabla_iso [128];


typedef struct formato_tabla_apl
{
char   tipo_fc;
int    long_fc;
int    max_fc;
int    off_fc;
}      formato_tabla_apl;

formato_tabla_apl   tabla_apl [128];


typedef struct formato_tabla_bim
{
char   tipo_bm[4];
char   sent_bm;
char   perv_bm;
char   bim1_bm[192];
char   bim2_bm[192];
}      formato_tabla_bim;

formato_tabla_bim   tabla_bim [100];




/***---   campos que llenan las funciones flee_datoiso y flee_datoapl ---***/
char   tipo_fs;
int    long_fs;
int    max_fc;
char   mac_fs;
int    max_fs;

char   tipo_fc;
int    long_fc;
int    fc;
int    off_fc;


/***-------   campos para llamar a fexplbit     ----------***/
char   mapa_exp[65];
char   mapa_com[9];



/***-------   campos para llamar a fcheqbit     ----------***/
int               tipomsg;
char              plantilla[384];
char              bitmapp[64];
char              bitmaps[64];

formato_intermedio  mensaje_inter;

//DMND0037271 no es necesario cambiarle el tamaño
char              mensaje_interchar[2000];

/*
#include "fempaque.h"
*/


/***-------   campos para llamar a ftraprev     ----------***/

/***------- subsistema general ----------***/
  char subsistema_general[3];



