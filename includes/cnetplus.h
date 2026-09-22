//PRJ0257005 - Revisado tokenizacion
#ifndef CNETPLUS_H
#define CNETPLUS_H
#include <string.h>
/***
   FICHERO: CNETPLUS.H
   DESCRIPCION: Fichero con las constantes y datos generales
                de la aplicacion.
				Vemos los codigos de operacion redsys, sus correspondientes en sio
				las rutas de trazas, ....

***/

//////// Nuevas Constantes PEMAMP ////

// Tipo de operacion
#define PETI_AUTO "1100" 		// en SIO 0100, 0700, 5000 consultas de saldo y mov, //DMND0036176
#define RESP_PETI_AUTO "1110"	// en SIO 0110, 0710, 5010 resp consultas de saldo y mov, //DMND0036176
#define COMU_AUTO "1120"		// en SIO 0120, 0720, 5020 anul consultas de saldo y  mov //DMND0036176
#define REPE_COMU_AUTO "1121"	//DMND0036176
#define COMU_AUTO_TCAP "1124"	//DMND0036176
#define REPE_COMU_AUTO_TCAP "1125"	//DMND0036176
#define ANUL_TCAP "1424"	// 
#define REPE_ANUL_TCAP "1425"	// 
#define RESP_COMU_AUTO "1130"	// en SIO 0130, 0730, 5030 resp anul consultas de saldo y mov //DMND0036176
#define RESP_COMU_AUTO_TCAP "1134"	// 
#define OFF_AUTO "1140"		// //DMND0036176
#define PETI_CONT "1200" 		// en SIO 0200
#define RESP_PETI_CONT "1210"	// en SIO 0210
#define COMU_CONT "1220"		// en SIO 0220
#define REPE_COMU_CONT "1221"	// en SIO
#define COMU_CONT_TCAP "1224"	// 
#define REPE_COMU_CONT_TCAP "1225"	// 
#define RESP_COMU_CONT "1230"	// en SIO 0230
#define RESP_COMU_CONT_TCAP "1234"	// 
#define OFF_CONT "1240"		// en SIO
#define ANUL_PETI "1420"		// en SIO 0420
#define REPE_ANUL_PETI "1421"	// 
#define RESP_ANUL_PETI "1430"	// en SIO 0430
#define RESP_ANUL_TCAP "1434"	// 
#define ANUL_OFF "1440"		// en SIO 0440
#define CONCILIA "1524"		// en SIO 0500
#define REPE_CONC "1525"		// 
#define RESP_CONC "1534"		// en SIO 0510
#define CONC_OFF "1544"		// 
#define ADMINISTR "1644"		// 
#define PETI_DIAL "1804"		// 
#define RESP_DIAL "1814"		// 


//////// Nuevas Constantes PEMAMP ////



#define TAM_PREF  7

#ifndef LOW_HIGH
#define LOW_HIGH 
#endif

/////////////
///////////// HAY QUE CAMBIARLO SEGUN DONDE ESTE INSTALADA LA APLICACION ///////////////////
#define HOMENET  "/sisma/"  
/////////////

#define DIRDATRZ  HOMENET"dat_his/" //trazas para el rranque y supervis
#define DIRDTSIS  "/sisma/"   //trazas para guardar en el proceso diario bfindedia

/////////////
//
#define DIRDATOS  HOMENET"dat/"
#define DIRCAMSE  HOMENET"dat\\cambsesi\\"
#define DIREXECS  HOMENET"obj/"
#define DIRSCRIP  HOMENET"script/\\"
#define DIRTEMPO  HOMENET"temporal/\\"
#define DIRTRAZA  HOMENET"dat/trazas/"
#define DIRCONEX  HOMENET"Conex/\\"
#define DIRTRTUX  HOMENET"dat/trazas/"

#define ESSNDCOL  3
#define TMPRP_MAX  10       /** Numero de segundos a los que se considera  **/
                            /** caducada una operacio a la que la repeticion **/
                            /** encuentra en vuelo **/

#define PRIPUC      1  /* Primer codigo de subaplicacion PUC */
#define ULTPUC      30 /* Ultimo codigo de subaplicacion PUC */

/* Ojo el define ULTPUC influye en el fichero NPTESTA y su tratamiento */
#define MAXRES19    ULTPUC - PRIPUC + 1

/* Este + 1 para contemplar Fimestic */
#define TOTRES      MAXRES19 + 1 /* Ojo con las pantallas, minimo 10 */

// PRJ0202598 cambio bin de 6 posiciones 8
//#define MAXBINE     6
  #define MAXBINE     8    //RCS puesto display en rtratami.c y rtrataan.c porque se usan alli para ocmprobar que no se modifica nada raro
//mensaje maximo de la cola
#define MAX_MENSA_VB              4096


#define LONG_FIJA_ENV 1024 /*longuitud maxima que se envia ing, cardplus **/

/* MACROS DE VECES DE INTENTO PARA COMPROBAR SI UN FICHERO ESTA BLOQUEADO */
#define MAXSEG3 30
#define MAXSEG6 60


#define MAX_LISTEN 10    //maximo conexiones que permite el socket.

#define NUCLEO "0000"
#define TODO   "????"

/*los max MAX_* son los numero tal cual y los FMAX son +1 */
#define TAM_PAN   19  //// PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total del pan
#define FTAM_PAN  TAM_PAN + 1

#define TAM_ESTA 15
#define TAM_TERM 8
// PRJ0202598 cambio bin de 6 posiciones 8
//#define TAM_BIN 6
#define TAM_BIN 8
#define TAM_RESO 10
#define MAX_RES_BALA 5
#define TAM_TIPTAR  3


#endif
