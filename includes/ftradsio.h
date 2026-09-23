
/****************************************************************
   Campos y definiciones de uso por el traductor de SIO
****************************************************************/

#include "cnetplus.h"
#include "cforitno.h"
#include "msg_nucl.h"
#include "fclasmsg.h"

/*   definicion de valores para las condiciones de cada bit   */
#define  DESCONOCIDO  -1
#define  NO_USADO      0
#define  PROHIBIDO     1
#define  OBLIGATORIO   2
#define  OPCIONAL      3
#define  CONDICIONAL   4

//RAQUEL COMPROBAR LAS LONGITUDES DE ESTOS MENSAJES PARA EL P48

/***---   campos de entrada y salida de los raices recsio3 y envsio  ---***/
/***-------   campos de uso general     ----------***/
char  cadena_verif[2000];

/***-------   campos para llamar a ftradsio     ----------***/
char            usotrad;
int             sentido;
int             long_iso;
int             suba_iso;
char            mensaje_sio[2000];
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
int    max_fs;
char   mac_fs;

char   tipo_fc;
int    long_fc;
int    max_fc;
int    off_fc;


/***-------   campos para llamar a fexplbit     ----------***/
char   mapa_exp[65];
char   mapa_com[9];



/***-------   campos para llamar a fcheqbit     ----------***/
int               tipomsg;
char              plantilla[384];
char              bitmapp[64];
char              bitmaps[64];

/***-------   Puntero a los campos de datos de aplicacion  ----------***/
char * pcampo_datapl;
int  l_datosapl;  /*** Longitud de los campos de datos de aplicacion ***/
int  primer_elem; /*** Primer elemento significativo de datos de aplicacion ***/
int  ultimo_elem; /*** ultimo elemento significativo de datos de aplicacion ***/


char              mensaje_interchar[2000];

/***-------   campos para llamar a ftraprev     ----------***/


