//PRJ0247167 - Revisado ampliación P48
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "cforitno.h"
/*-----------------------------------------------------------------------------
        Constantes y tablas de LOG, conexion e incidencias
-----------------------------------------------------------------------------*/
#define CIFRADO         "1"
#define DESCIFRADO      "0"
#define RsaMec          "1"


#define LONG_MSG 4096
#define LONGMAX     12288       /* Maxima longitud del texto a cifrar o descifrar */
#define LONGMIN         8       /* Minima longitud del texto a cifrar o descifrar */

// Opciones de comandos
#define TEST          "1401"
#define PIN_CIFRADO   "0601"
#define CALCULA_PIN   "0601"
#define CIFRAR_PIN    "0611"
#define HASH_DATO     "2000"
#define DESCIFRA_DATO "0901"
#define LISTADO       "0103"
#define IMPORTACION   "0202"
#define EXPORTA       "0303"
#define IMPRIME_PIN   "0609"
#define RSA_DATO      "1103"
#define CALCULA_MAC   "0801"
#define CALCULA_CMAC  "0802" //PRJ0258542
#define ALTA_CLAVE    "0101" //PRJ0258542
#define BAJA_CLAVE    "0102" //PRJ0258542
#define IMPORTAR_TR31 "0206" //PRJ0258542
#define EXPORTAR_TR31 "0304" //PRJ0258542
#define CALCULA_KCV   "0404" //PRJ0258542
#define CALCULA_OFF   "0618" //PRJ0258542
#define TRANSLATE_PIN "0603" //PRJ0378949
#define EXPORT_PIN    "0605" //PRJ0378949
#define GENERA_PIN    "0619" //PRJ0378949
#define CIFRAR_PIN    "0611" //PRJ0378949
#define CIFRADO903    "0903" //PRJ0378949

#define CRIPTOGRAMA   "0503"
#define CALCULA_OFFSET "0604"
#define DESCIFRA_DATO_AES "0903"

// Opciones de busqueda de identificador de claves internas
#define PIN_CIF         "02"
#define CIF_PIN         "03"
#define CAL_PIN         "04"
#define DESCIFRA        "07"
#define CF_HASH         "30"
#define CF_RSA          "31"
#define CIFRADO_EXTERNO "D00"
#define CIFRADO_INTERNO "N00"
#define ALMACEN_EXTERNO "D"
#define ALMACEN_INTERNO "S"
#define SIN_KCV         "00"
//añadido para PRICE
#define ARPC            "06"
#define MAC            	"08"

/*los max MAX_* son los numero tal cual y los FMAX son +1 */
#define TAM_PAN   19   // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a tamaño total del PAN
#define FTAM_PAN  TAM_PAN + 1
#define DIRTRTUX "/sf/tux/logs/"

//añadido para PRICE
// Constantes para la composicion de los comandos relacionados con el criptograma
#define VERIF_ARQC      "0"
#define GENERA_ARPC     "2"
#define ESQUEMA_VISA    "0"
#define BLOQUE_ISO_0    "0"

typedef struct NPSLOGO
 {
  char      lonumope[6];   /*  Numero de operacion CRO      (clave) */
  char      lofecha0[8];   /*  Fecha CRO                    (clave) */
  char      lohora00[6];   /*  Hora  CRO                    (clave) */
  char      lonumpan[TAM_PAN]; /*  Numero de tarjeta            (clave) */  // PRJ0202598 cambio bin de 6 posiciones 8 no afecta a tamaño total del PAN
  char      locomerc[15];  /*  Identificacion comercio      (clave) */
  char      loclavpr[35];  /*  clave para recuperar operacion (clave) */
  char      loeseven[1];   /*  Estado evento generado               */
  char      loesanul[1];   /*  Estado de Anulacion                  */
  char      loimporte[12];   /*  importe de la operación sin decimales x100 completado con ceros por la izquierda */
  char      losesion[3];      
  char      lofecses[6];
  //PRJ0247167 - Revisado ampliación P48 no es necesario cambiarle el tamaño
  char      loforitn[sizeof(formato_interno)];    /*  formato interno de la operacion, pero grabado como char */
 } NPSLOGO;

typedef   struct {
    char            idsubsis[2];        //  codigo de subsistema (clave)     
    char            idsubapl[2];        //  subaplicacion        (clave) 
    char            idcomando[50];      //  comando              (clave)
    char            idclave1[150];      //  clave zona 1
    char            idclave2[150];      //  clave zona 2
}  NPSIDEN;

//    Registro del fichero de incidencias

typedef struct NPSINCI
{
 //   char    inpcs000[4];     // Entidad
    char    infecha0[8];     // Fecha
    char    inhora00[6];     // Hora
    char    innompro[8];     // Proceso
    char    innomfic[8];     // Fichero o elemento
    char    innumope[6];     // Número de operación
    char    incoderr[6];     // Código de error
    char    inestado[1];     // Estado de la incidencia P: pendiente, T: tratada
} NPSINCI;

typedef   struct {
  char    dhcodent[4];      /*  codigo de entidad           pk   */
  char    dhtiempo[4];      /*  tiempo de respuesta del hsm      */
  char    dhcabece[10];     /*  cabecera del HSM                 */ 
  char    dhnumip[15];      /*  IP del HSM                       */
  char    dhpuerto[4];      /*  puerto del HSM     */
  char    dhprodt[1];       /*  produccion 1- utilzamos hsm 0- no utilizamos hsm. */
  char    dhnumip2[15];     /*  IP secundaria */
	char		dhsemafhsm1[1];		/* Semaforo del HSM principal 0 = KO - 1 = OK */ //PRJ0258542
	char		dhsemafhsm2[1];		/* Semaforo del HSM secundario 0 = KO - 1 = OK */  //PRJ0258542
	char		dhparenov[2];			/* Control renovacion - 99 = No renueva, 98 = error hsm, xx = clave que se esta renovando */  //PRJ0258542
} NPSDHSM;

typedef   struct {
    char            pasubsis[2];        /*  codigo de subsistema             */
    char            pasubapl[2];        /*  subaplicacion (SEMP)    Numero de ILT para SIO   */
    char            panomsub[20];       /*  nombre subsistema                */
    char            pasemaf1[1];        /*  Semaforo1 de Estado (PASEMAFO EN b.a.)   */
                                        /*     0 - DES                       */
                                        /*     1 - SNN                       */
                                        /*     2 - SUS                       */
                                        /*     3 - SNF                       */
    char            pasemaf2[1];        /*  Semaforo2 de Estado ( PAESTANT en b.a.)    */
                                        /*     0 - INI                       */
                                        /*     1 - SNN                       */
                                        /*     2 - SNN-SUS                   */
                                        /*     3 - SNF                       */
                                        /*     4 - SNF-SUS                   */
    char            paestado[2];        /* Estado del socket                 */
                                        /*  00 Estado proceso                */
                                        /*  01 Estado cerrado                */
                                        /*  02 Estado Erróneo                */

    char            patrepet[6];        /*  Tiempo de Respuesta peticiones   */
    char            patrerep[6];        /*  Tiempo de Respuesta repeticiones */
    char            patresnf[6];        /*  Tiempo de Respuesta SNF          */
    char            panumrea[1];        /*  numero de repeticiones           */
    struct        redef_datses
    {
        char        pafecses[6];       /*  fecha de la sesion               */ 
        char        panumses[3];       /*  numero de la sesion              */
        char        paconcil[1];       /*  flag de conciliacion :
                      1: conciliacion ha cuadrado
                      2: conciliacion ha descuadrado
                      3: conciliacion no disponible
                      0: sesion en curso               */
    } rdfses;
    char            panip[15];          /*  IP del socket                    */
    char            paportnume[5];      /*  puerto                 */
    char            pacidesti[13];       /*  Identificador destino            */
    char            paciadqui[13];       /*  Identificador adquirente         */

    char            paenvoff[1];       /*  Opcion envio off       */
    char            paenvtst[1];       /*  Opcion envio test
                                               0: Solo se envian test si semaforo cerrado
                                               1: Se envian test periodicamente */
    char            patipsub[1]; /*tipo resolutor*/
    char            paidred[4];
	
} NPSPARS;

typedef struct 
{
 char     canumope[6];          /*  Numero de operacion    (clave)    */
 char     catieabs[10];          /*  Tiempo absoluto cuando caduca     */
 char     catieesp[10];          /*  Tiempo de espera de operacion     */
 //PRJ0247167 - Revisado ampliación P48 no es necesario cambiarle el tamaño
 formato_interno caforitn;      /*  Formato interno operacion         */
} NPSCASA;


typedef struct 
 {
  char      disubsis[2];    /*  Subsistema               (clave)    */
  char      disubapl[2];    /*  Subaplicacion            (clave)    */
  char      difecha0[8];    /*  Fecha procesal           (clave)    */
  char      dihora00[6];    /*  Hora procesal            (clave)    */
  char      dinumope[6];    /*  Numero de operacion      (clave)    */
  char      diforitn[sizeof(formato_interno)];    /*  formato interno de la operacion, pero grabado como char */
  char      diclavpr[35];   //identificación RTS de la operación (clave2 única)

} NPSDIFE;


 typedef struct NPSDSCD
 {
   char   ddsubsis[2];     /*  Subsistema                          */
   char   ddsubapl[2];     /*  Subaplicacion                       */
   char   ddfecha0[8];     /*  Fecha de la operacion               */
   char   ddhora00[6];     /*  Hora de la operacion                */
   char   ddnumope[8];     /*  Numero de la operacion              */
   char   ddmensaj[sizeof(formato_interno)];
 } NPSDSCD;
 
 
 /*    Registro del fichero de Parametros del Sistema         */

typedef   struct {
    char            pscsb000[4];        /*  Codigo CSB de la entidad         */
    char            psnoenti[20];       /*  Nombre de la entidad             */
    char            psnumope[6];        /*  Numero de operacion              */
                                        /*   1 - Num. sesion actual          */
                                        /*   5 - Num. operacion en la sesion */
    char            psnumseh[2];        /*  Numero de entiedad origen  */
    char            pshcamse[6];        /*  Hora de cambio de sesion         */
    char            psfeinse[8];        /*  Fecha de inicio de sesion        */
    char            pshoinse[6];        /*  Hora de inicio de sesion         */
    char            psmodocs[1];        /*  Modalidad cambio sesion          */
                                        /*   0 - Cambio Automatico           */
                                        /*   1 - Cambio Manual               */
                                        /*   2 - Mensaje del HOST            */
    char            pstimesp[6];        /*  Tiempo de activacion del super.  */
    char            psnumcross[10];     /*  Numero de cruce                  */
    char            pscerrando[1];      /*  1 - se esta cerrando             */
	char            psresolu[1];        /*  Rsolutor                         */
	char            psclvhsm[50];       /*  Clave para conexion HSM          */
} NPSPARA;

 

typedef struct tm_dec {         /* see ctime(S) */
 int tm_sec;                    /* time of day, seconds */
 int tm_min;                    /* time of day, minutes */
 int tm_hour;                   /* time of day, hours (24 hour clock) */
 int tm_mday;                   /* day of month (1-31) */
 int tm_mon;                    /* month of year (0-11) */
 int tm_year;                   /* year - 1900 */
 int tm_siglo;                   /* siglo - 1 */
 int tm_mls;                    /* milesimas de segundo */
} tm_dec;

 //PRJ0258542 - se incluyen UNO y DOS
typedef enum
{ABANDONAR,
 ACTIVAR,
 ANTERIOR,
 CARGAR,
 BLOQUEAR,
 BORRAR,
 ESCRIBIR,
 DESACTIVAR,
 FECMODIF,
 IGUAL,
 LIMPIAR,
 MAYORIGUAL,
 MAYOR,
 MENORIGUAL,
 MENOR,
 PRIMERO,
 REESCRIBIR,
 SIGUIENTE,
 SEMAF,
 SEMAF00,
 PACONCIL,
 MONITOR,
 THNPTPARS,
 IGUALBLOQ,
 NSMILT,
 DESBLOQ,
 SESIONF,
 NODOCIERRE,
 DEBITO,
 CREDITO,
 RESPALDO,
 TOTAL,
 IGUALBORRAR,
 MODIFICO,
 TRUNCAR,
 NUMERO,
 THNPTSIOC,
 MODIFESTADO,
 ULTIMO,
 ACTSALDO,
 CIFRATJT,
 DESCIFRATJT,
 COMPENS,
} OPCION_FICH;


void fhexdump(FILE * pf, void * vaddr, int len);

#endif
