
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[29];
};
static const struct sqlcxp sqlfpn =
{
    28,
    "/sf/mepa/rut/src/filefun.cpp"
};


static unsigned int sqlctx = 926128916;


static struct sqlexd {
   unsigned long  sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
   unsigned char  **sqphsv;
   unsigned long  *sqphsl;
            int   *sqphss;
            short **sqpind;
            int   *sqpins;
   unsigned long  *sqparm;
   unsigned long  **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
            int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
   unsigned int   sqlpfmem;
   unsigned char  *sqhstv[22];
   unsigned long  sqhstl[22];
            int   sqhsts[22];
            short *sqindv[22];
            int   sqinds[22];
   unsigned long  sqharm[22];
   unsigned long  *sqharc[22];
   unsigned short  sqadto[22];
   unsigned short  sqtdso[22];
} sqlstm = {13,22};

/* SQLLIB Prototypes */
extern void sqlcxt (void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlcx2t(void **, unsigned int *,
                    struct sqlexd *, const struct sqlcxp *);
extern void sqlbuft(void **, char *);
extern void sqlgs2t(void **, char *);
extern void sqlorat(void **, unsigned int *, void *);

/* Forms Interface */
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern void sqliem(unsigned char *, signed int *);

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{13,4130,31,8,0,
5,0,0,0,0,0,27,359,0,0,4,4,0,1,0,1,97,0,0,1,10,0,0,1,10,0,0,1,10,0,0,
36,0,0,2,0,0,30,376,0,0,0,0,0,1,0,
51,0,0,0,0,0,62,382,0,0,0,0,0,1,0,
66,0,0,3,0,0,30,393,0,0,0,0,0,1,0,
81,0,0,0,0,0,62,398,0,0,0,0,0,1,0,
96,0,0,4,0,0,29,415,0,0,0,0,0,1,0,
111,0,0,5,0,0,32,427,0,0,0,0,0,1,0,
126,0,0,0,0,0,62,432,0,0,0,0,0,1,0,
141,0,0,6,0,0,31,449,0,0,0,0,0,1,0,
156,0,0,7,20,0,4,488,0,0,0,0,0,1,0,
171,0,0,0,0,0,56,806,0,0,1,1,0,1,0,3,102,0,0,
190,0,0,8,33,0,122,808,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,102,0,0,1,97,0,0,
221,0,0,0,0,0,13,830,0,0,7,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,2,97,0,0,
264,0,0,0,0,0,13,854,0,0,6,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,3,0,0,
303,0,0,0,0,0,13,878,0,0,10,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,3,0,0,2,3,0,0,2,97,0,0,
358,0,0,0,0,0,13,907,0,0,21,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
457,0,0,0,0,0,13,941,0,0,6,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,
496,0,0,0,0,0,13,965,0,0,6,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,
535,0,0,0,0,0,13,993,0,0,6,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,
0,2,97,0,0,
574,0,0,0,0,0,13,1012,0,0,5,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,
609,0,0,0,0,0,15,1034,0,0,1,1,0,1,0,1,102,0,0,
628,0,0,0,0,0,78,1035,0,0,1,1,0,1,0,3,102,0,0,
647,0,0,0,0,0,56,1082,0,0,1,1,0,1,0,3,102,0,0,
666,0,0,9,38,0,122,1085,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,102,0,0,1,
97,0,0,
701,0,0,0,0,0,13,1098,0,0,6,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,
740,0,0,0,0,0,13,1106,0,0,22,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
843,0,0,0,0,0,13,1113,0,0,5,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,
878,0,0,0,0,0,13,1120,0,0,7,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,
921,0,0,0,0,0,13,1133,0,0,13,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,
0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
988,0,0,0,0,0,15,1146,0,0,1,1,0,1,0,1,102,0,0,
1007,0,0,0,0,0,78,1147,0,0,1,1,0,1,0,3,102,0,0,
1026,0,0,0,0,0,56,1354,0,0,1,1,0,1,0,3,102,0,0,
1045,0,0,10,38,0,122,1355,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,102,0,0,1,
97,0,0,
1080,0,0,0,0,0,13,1360,0,0,13,1,0,1,0,1,102,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,
97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,2,97,0,0,
1147,0,0,0,0,0,15,1364,0,0,1,1,0,1,0,1,102,0,0,
1166,0,0,0,0,0,78,1365,0,0,1,1,0,1,0,3,102,0,0,
1185,0,0,11,116,0,2,1442,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,
1220,0,0,12,37,0,122,1462,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,
1255,0,0,13,45,0,3,1525,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1286,0,0,14,77,0,5,1530,0,0,4,4,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1317,0,0,15,79,0,3,1625,0,0,12,12,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,
1380,0,0,16,111,0,5,1634,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,
1415,0,0,17,111,0,5,1643,0,0,5,5,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,
97,0,0,
1450,0,0,18,57,0,3,1685,0,0,7,7,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,
1493,0,0,19,0,0,29,1687,0,0,0,0,0,1,0,
1508,0,0,20,85,0,3,1697,0,0,7,7,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,1,97,0,0,
1551,0,0,21,0,0,29,1699,0,0,0,0,0,1,0,
1566,0,0,22,53,0,3,1734,0,0,6,6,0,1,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,0,0,1,97,
0,0,1,97,0,0,
1605,0,0,23,34,0,122,1804,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
1628,0,0,24,34,0,122,1810,0,0,2,2,0,1,0,1,97,0,0,1,97,0,0,
};


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//PRJ0247167 - Revisado ampliación P48 - cambiado de 2279 a 4000
//PRJ288711 - Adaptación tratamineto diferidos erroneos
/*definicion de funciones */
int InsertarL(void* registro);
void QuitarCaracteres(char* cadena, int longi);

/*añadido para PRICE*/ 
int InsertarC(void* registro);
int InsertarD(void* registro);
int InsertarDF(void* registro);
int InsertarDE(void* registro); //PRJ288711
/*añadido para PRICE*/

/* Cabeceras de SQL Communication Area, para control de errores, etc. */
/* exec sql include sqlca;
 */ 
/*
 * $Header: sqlca.h 24-apr-2003.12:50:58 mkandarp Exp $ sqlca.h 
 */

/* Copyright (c) 1985, 2003, Oracle Corporation.  All rights reserved.  */
 
/*
NAME
  SQLCA : SQL Communications Area.
FUNCTION
  Contains no code. Oracle fills in the SQLCA with status info
  during the execution of a SQL stmt.
NOTES
  **************************************************************
  ***                                                        ***
  *** This file is SOSD.  Porters must change the data types ***
  *** appropriately on their platform.  See notes/pcport.doc ***
  *** for more information.                                  ***
  ***                                                        ***
  **************************************************************

  If the symbol SQLCA_STORAGE_CLASS is defined, then the SQLCA
  will be defined to have this storage class. For example:
 
    #define SQLCA_STORAGE_CLASS extern
 
  will define the SQLCA as an extern.
 
  If the symbol SQLCA_INIT is defined, then the SQLCA will be
  statically initialized. Although this is not necessary in order
  to use the SQLCA, it is a good pgming practice not to have
  unitialized variables. However, some C compilers/OS's don't
  allow automatic variables to be init'd in this manner. Therefore,
  if you are INCLUDE'ing the SQLCA in a place where it would be
  an automatic AND your C compiler/OS doesn't allow this style
  of initialization, then SQLCA_INIT should be left undefined --
  all others can define SQLCA_INIT if they wish.

  If the symbol SQLCA_NONE is defined, then the SQLCA variable will
  not be defined at all.  The symbol SQLCA_NONE should not be defined
  in source modules that have embedded SQL.  However, source modules
  that have no embedded SQL, but need to manipulate a sqlca struct
  passed in as a parameter, can set the SQLCA_NONE symbol to avoid
  creation of an extraneous sqlca variable.
 
MODIFIED
    lvbcheng   07/31/98 -  long to int
    jbasu      12/12/94 -  Bug 217878: note this is an SOSD file
    losborne   08/11/92 -  No sqlca var if SQLCA_NONE macro set 
  Clare      12/06/84 - Ch SQLCA to not be an extern.
  Clare      10/21/85 - Add initialization.
  Bradbury   01/05/86 - Only initialize when SQLCA_INIT set
  Clare      06/12/86 - Add SQLCA_STORAGE_CLASS option.
*/
 
#ifndef SQLCA
#define SQLCA 1
 
struct   sqlca
         {
         /* ub1 */ char    sqlcaid[8];
         /* b4  */ int     sqlabc;
         /* b4  */ int     sqlcode;
         struct
           {
           /* ub2 */ unsigned short sqlerrml;
           /* ub1 */ char           sqlerrmc[70];
           } sqlerrm;
         /* ub1 */ char    sqlerrp[8];
         /* b4  */ int     sqlerrd[6];
         /* ub1 */ char    sqlwarn[8];
         /* ub1 */ char    sqlext[8];
         };

#ifndef SQLCA_NONE 
#ifdef   SQLCA_STORAGE_CLASS
SQLCA_STORAGE_CLASS struct sqlca sqlca
#else
         struct sqlca sqlca
#endif
 
#ifdef  SQLCA_INIT
         = {
         {'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
         sizeof(struct sqlca),
         0,
         { 0, {0}},
         {'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
         {0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
         }
#endif
         ;
#endif
 
#endif
 
/* end SQLCA */


#include "utilidades.h" 

typedef struct NPSERRD //PRJ288711
 {
 	char			eraccion[1];		//  Accion para procedimiento, E = Error, B = Buena
  char      ersubsis[2];    //  Subsistema               (clave)
  char      ersubapl[2];    //  Subaplicacion            (clave)
  char      ernumope[6];    //  Numero de operacion      (clave)
  char			ernumerr[4];		// Numero de error que devuelve SF
} NPSERRD;


/*variables auxiliares*/
/***-------   campos de uso general     ----------***/
char  cad_verif[10];


/*añadido para PRICE*/
/***---------------   tablas de datos trms y aplicacion  -----------------***/
typedef struct formato_tabla_trms
{
 char  trms_emis[3];        /* Código del emisor */
 char  trms_idtm[5];        /* Código de mensaje PRICE */
 char  trms_copr[7];        /* Código de proceso PRICE */
 char  trms_idtv[5];        /* Código de mensaje PRICE de vuelta*/
 char  trms_idsi[5];        /* Código de mensaje SIO */
 char  trms_cosi[7];        /* Código de proceso SIO */
}      formato_tabla_trms;

formato_tabla_trms   tabla_trms [500]; 

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

/***--------------------- tabla de tabla de operaciones --------------------------**/
typedef struct dtbop ttbop;
struct dtbop
{
   char     clave [18];
   char     funcion[20];
   ttbop    *siguiente;
};

ttbop *ptb;
ttbop *ptb_primero;

/*añadido para PRICE*/

//RAQUEL añadido para unificar configuracion y colas para arranque por bbdd

/***---------------   tablas de datos ipcs  -----------------***/
typedef struct formato_tabla_ipcs
{
char  ipcs_indi[3];	
char  ipcs_nsvm[8];
char  ipcs_nomb[5];
char  ipcs_clav[2];
int   ipcs_tama;
}      formato_tabla_ipcs;

formato_tabla_ipcs   tabla_ipcs [50]; 


/***---------------   tablas de datos configuracion  -----------------***/
typedef struct formato_tabla_conf
{
 char conf_indi[3];	/* codigo 00 siempre para cargar toda la tabla con procedimiento CARGA*/
 char conf_cdsu[5];   /* subistema-subaplicaicon */
 char conf_dsej[9];  /* nombre con que se levanta el proceso */
 char conf_para[21]; /* El maximo permitido es 20 */
 char conf_cola[5];     /* nombre de la cola o SVM */
 char conf_tipp[2];        /* Como parar el proceso: Q=cola, S=svm, N=no parar */
 int  conf_nmpr;      /* Numero de posiciones reservadas en svm para proceso, cuantos tenemos que inicializar*/
 int  conf_capr;      /* Numero de copias a arrancar automaticamente */
 char conf_csal[5];  /* Cola de salida para cuando el pamnej no es parte de SISMA */
}      formato_tabla_conf;

formato_tabla_conf   tabla_conf [50]; 

/***---------------   tablas de datos para  -----------------***/
typedef struct formato_tabla_para
{
  char ps_indice[3];  
  char ps_csb000[5];                                                  
  char ps_noment[21];                                                 
  char ps_numope[7];                                                  
  char ps_nument[3];                                                  
  char ps_hcamse[7];                                                  
  char ps_feinse[9];                                                  
  char ps_hoinse[7];                                                  
  char ps_modocs[2];                                                  
  char ps_timesp[7];                                                 
  char ps_numcross[11];                                               
  char ps_cerrando[2];                                                 
  char ps_resolu[3];   
  char ps_monito[3];   
  char ps_cotrms[3];  
  char ps_idered[2];  
  char ps_cltrat[9];  
  char ps_clenvi[5];  
  char ps_clrece[9];                                                                    
  char ps_clvhsm[51];
} formato_tabla_para;

formato_tabla_para   tabla_para [2];


/* Declaracion de variables .huesped", que seran utilizadas en sentencias SQL */
/* EXEC SQL BEGIN DECLARE SECTION; */ 
 

   char *Vent;
   char *Vent2;
   char opcionT[20];
   char numope[7];
   //PRJ0247167 se cambia para que quepa el formato interno nuevo entero
   //char formatoI[2280]; // ATENCIÓN - dejar este campo y asteriscar el inferior si se tiene que probar Preautorizaciones
   char formatoI[4000]; 
   char fecha0[9]; char hora00[7]; char lonumpan[20]; char locomerc[16]; char loclavpr[36];
   char loimporte[13]; char losesion[4]; char loeseven[2]; char loesanul[2]; char lofecses[7];
   char subsisF[3]; char subapl[3];
   char catieabs[11]; char catieesp[11];
   char tabla[8];
   char claveT[200];
   char CodError[4];
   char usuario1[30];
   char registro2[3000];
   char auxpan[20];
   SQL_CURSOR  registro3 ;
   char formatoDE[14]; //PRJ288711 - estructura para llamar al procedimiento
   char accion[1]; //PRJ288711 - opcion de llamada al procedimiento
   char errorn[4]; //PRJ288711 - opcion de llamada al procedimiento
   
   
  struct {
    char            idsubsis[3];
    char            idsubapl[3];
    char            idcomando[51]; //CAMBIAR A 50 CUANDO CAMBIE LA TABLA
    char            idclave1[151];
    char            idclave2[151];
 }estrucIDEN;
 
/* añadido para PRICE*/
  struct {
  char   trmsemis[3];
  char   trmsidtm[5];
  char   trmscopr[7];
  char   trmsidtv[5];
  char   trmsidsi[5];
  char   trmscosi[7];
  }estrucTRMS;

  struct {
  char   donumiso[4];
  char   dopictur[2];
  char   dolongit[5];
  char   dolonmax[4];
  char   dousomac[2];
  }estrucDISO;
 struct {
  char   a2numiso[4];
  char   a2pictur[2];
  char   a2longit[5];
  char   a2lonmax[5];
  char   a2offset[5];
  }estrucDAP2;
 struct {
  char   bmtipmen[5];
  char   bmsentra[2];
  char   bmpervia[2];
  char   bmbima01[193];
  char   bmbima02[193];
  }estrucBIMA;
  struct {
    char tbtabla0[3];
    char tbclave0[13];
    char tbfuncis[21];
    char tbcodent[5];
  }estrucTBOP;
  struct {
    char            pasubsis[3];
    char            pasubapl[3];
    char            panomsub[21];
    char            pasemaf1[2];
    char            pasemaf2[2];
    char            paestado[3];
    char            patrepet[7];
    char            patrerep[7];
    char            patresnf[7];
    char            panumrea[2];
    char            pafecses[7];
    char            panumses[4];
    char            paconcil[2];
    char            panip[16];
    char            paportnume[6];
    char            pacidesti[14];
    char            paciadqui[14];
    char            paenvoff[2];
    char            paenvtst[2];
    char            patisub[2];
    char            paidred[5];
 }estrucPARS;
  struct{
  char      lonumope[7];
  char      lofecha0[9];
  char      lohora00[7];
  char      lonumpan[20];
  char      locomerc[16];
  char      loclavpr[36];
  char      loeseven[2];
  char      loesanul[2];
  char      loimporte[13];
  char      losesion[4];
  char      lofecses[7];
  //PRJ0247167 se cambia tamaño 4000
  //char      loforitn[2280]; // ATENCIÓN - dejar este campo y asteriscar el inferior si se tiene que probar Preautorizaciones
  char      loforitn[4000]; 
 }estrucLOGO;
 struct{
  char      canumope[7];
  char      catieabs[11];
  char      catieesp[11];
  //PRJ0247167 se cambia tamaño 4000
  //char      caforitn[2280];   // ATENCIÓN - dejar este campo y asteriscar el inferior si se tiene que probar Preautorizaciones
  char      caforitn[4000]; 
}estrucCASA;
 struct{
  char      disubsis[3];   
  char      disubapl[3];   
  char      difecha0[9];   
  char      dihora00[7];   
  char      dinumope[7];    
  //PRJ0247167 se cambia tamaño 4000
  //char      diforitn[2280]; // ATENCIÓN - dejar este campo y asteriscar el inferior si se tiene que probar Preautorizaciones
  char      diforitn[4000];
  //char      diclavpr[36]; -- Quitada desde hace tiempo
 }estrucDIFE;
 
/* añadido para PRICE*/

//RAQUEL añadido para arrancar entidades por bbdd
//unificamos el arranque de configuracion y colas
/* añadido para PRICE*/
  struct {
   char  ipcsindi[3];
   char  ipcsnsvm[8];
   char  ipcsnomb[5];
   char  ipcsclav[2];
   int   ipcstama;
  }estrucIPCS;

  struct {
  char confindi[3];
  char confcdsu[5]; 
  char confdsej[9];
  char confpara[21];
  char confcola[5]; 
  char conftipp[2];    
  int  confnmpr;     
  int  confcapr;    
  char confcsal[5];   
  }estrucCONF;
																	 
  struct {       
  char psindice[3];  
  char pscsb000[5];                                                  
  char psnoment[21];                                                 
  char psnumope[7];                                                  
  char psnument[3];                                                  
  char pshcamse[7];                                                  
  char psfeinse[9];                                                  
  char pshoinse[7];                                                  
  char psmodocs[2];                                                  
  char pstimesp[7];                                                 
  char psnumcross[11];                                               
  char pscerrando[2];                                                 
  char psresolu[3];   
  char psmonito[3];   
  char pscotrms[3];  
  char psidered[2];  
  char pscltrat[9];  
  char psclenvi[5];  
  char psclrece[9];                                                                    
  char psclvhsm[51];                                              
  }estrucPARA;                                                    
                                                                  
                                                                  
/* EXEC SQL END DECLARE SECTION; */ 
                                     


////////////////////////// CONEXION A LA BASE DE DATOS /////////////////////////
int conexion_BBDD()
{
    printf("Entrando en conexion_BBDD  filefun 04122022v2.0\n");
   if((Vent=getenv("ORAUID"))==(char *)NULL)
   //recuperar usuario
   {
      printf("No existe variable de entorno ORAUID\n");
      exit(-1);
   }
   if((Vent2=getenv("ORAPWD"))==(char *)NULL)
   //recuperar password 
   {
      printf("No existe variable de entorno ORAPWD\n");
      exit(-1);
   }
   strncpy(usuario1, Vent, strlen(Vent));
   strncpy(usuario1+strlen(Vent), "/",1);
   strncpy(usuario1+strlen(Vent)+1, Vent2,strlen(Vent2));

    /* EXEC SQL CONNECT :usuario1; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 13;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )10;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (unsigned char  *)usuario1;
    sqlstm.sqhstl[0] = (unsigned long )30;
    sqlstm.sqhsts[0] = (         int  )30;
    sqlstm.sqindv[0] = (         short *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned long )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlstm.sqlcmax = (unsigned int )100;
    sqlstm.sqlcmin = (unsigned int )2;
    sqlstm.sqlcincr = (unsigned int )1;
    sqlstm.sqlctimeout = (unsigned int )0;
    sqlstm.sqlcnowait = (unsigned int )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    if (sqlca.sqlcode)
    {
      printf("cadenaconexion %s\n", usuario1);
      printf("Error en conexion de %d\n", sqlca.sqlcode);
      exit(-1);
    }
    fprintf(stdout,"abro conexion bd\n");
    fflush(stdout);
    return(0);
}


/////////////////////////  DESCONEXION DE LA BASE DE DATOS ////////////////////////
int desconexion_BBDD()
{
   fprintf(stdout, "cierro conexion bd\n");
   /* EXEC SQL COMMIT RELEASE; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 13;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )36;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
   if (sqlca.sqlcode != 0)
   {
      printf("sqlcode al cerrar bbdd %d\n",sqlca.sqlcode);
      return(-1);
   }
   /* EXEC SQL CACHE FREE ALL; */ 

{
   struct sqlexd sqlstm;
   sqlstm.sqlvsn = 13;
   sqlstm.arrsiz = 4;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )51;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)4352;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   return(0);
}


//////////////////////////  SALIR DE TRATAR_FICH CON COMMIT  ////////////////////////
int salir_cm(int conecBBDD, int codSalida, char * salTabla, OPCION_FICH opcion)
{
   if(conecBBDD)
   {
      //fprintf(stdout, "commit y cierro conexion bd\n");
      /* EXEC SQL COMMIT RELEASE; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 13;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )66;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
      if (sqlca.sqlcode != 0)
      {
         printf("sqlcode al cerrar bbdd %d\n",sqlca.sqlcode);
      }
      /* EXEC SQL CACHE FREE ALL; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 13;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )81;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   }  
   else
   {
      if(strncmp(salTabla,"NPTDHSM",7)==0 || //pq se le llama desde cobol y la transacción la controlan ellos
          strncmp(salTabla,"NPTIDEN",7)==0 ||
          strncmp(salTabla,"HSMDRIP",7)==0 ||
           opcion == IGUAL ||
           opcion == MAYORIGUAL ||     //* añadido para PRICE*/
		   opcion == PRIMERO ||        //* añadido para PRICE*/
		   opcion == SIGUIENTE)        //* añadido para PRICE*/
      {
          //fprintf(stdout, "no commit tabla >%.7s<\n", salTabla); 
      }
      else
      {       
         fprintf(stdout, "commit tabla >%.7s<\n", salTabla); 
         /* EXEC SQL COMMIT; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 13;
         sqlstm.arrsiz = 4;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )96;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
      }
   }
   return(codSalida);
}

//////////////////////////  SALIR DE TRATAR_FICH CON ROLLBACK  ////////////////////////
int salir_rb(int conecBBDD, int codSalida, char * salTabla, OPCION_FICH opcion)
{
   if(conecBBDD)
   {
      fprintf(stdout, "rollback y cierro conexion bd\n");
      /* EXEC SQL ROLLBACK RELEASE; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 13;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )111;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
      if (sqlca.sqlcode != 0)
      {
         printf("sqlcode al cerrar bbdd %d\n",sqlca.sqlcode);
      }
      /* EXEC SQL CACHE FREE ALL; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 13;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )126;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


   }  
   else
   {
      if(strncmp(salTabla,"NPTDHSM",7)==0 || //pq se le llama desde cobol y la transacción la controlan ellos
          strncmp(salTabla,"NPTIDEN",7)==0 ||
          strncmp(salTabla,"HSMDRIP",7)==0 ||
           opcion == IGUAL ||
           opcion == MAYORIGUAL ||    //* añadido para PRICE*//
		   opcion == PRIMERO ||       //* añadido para PRICE*//
		   opcion == SIGUIENTE)       //* añadido para PRICE*//      
      {
          //fprintf(stdout, "no rollback\n"); //pq se le llama desde cobol y la transacción la controlan ellos
       }
       else
       {
         fprintf(stdout, "rollback\n");
         /* EXEC SQL ROLLBACK; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 13;
         sqlstm.arrsiz = 4;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )141;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

    
      }
   }
   return(codSalida);
}



//////////////////////////////////  TRATAR_FICH //////////////////////////
int tratar_fich(char* ficher, OPCION_FICH opcion, void * registro, void * clave)
{
   	fprintf(stdout,"Dentro del filefun\n");
    fflush(stdout);
    //printf("tratar_fich\n");
    //printf("opcion de llamar en tratar_fich %i\n",opcion);
    fflush(stdout);
    int contador=0;
    int ErrInsertar=0;
    int retorReg=0;  //0=devuelve registro, 1=no devuelve registro
    int abroConexion=0;
    
		//* añadido para PRICE*//
    int err=0;
    char ch05[5];
    char ch04[4];
    int ll=0;
    int indice=0;
    int terminaMal=0;

    ch04[3]='\0';
    ch05[4]='\0';
		//* añadido para PRICE*//
		//inicializamos la clave en cada nueva interaccion RAQUEL
    memset(claveT, '\0', 200); 
	
		// fprintf(stdout,"RAQUEL DENTRO DE TRATAR_FICH opcion %s\n",opcion);
    // fflush(stdout);
	 
    //prueba si hay conexion
    /* EXEC SQL SELECT (1) FROM DUAL; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 13;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "select 1  from DUAL ";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )156;
    sqlstm.selerr = (unsigned short)1;
    sqlstm.sqlpfmem = (unsigned int  )0;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
    if (sqlca.sqlcode == -1012)
    {
    	fprintf(stdout,"Dentro filefun.cpp, abro conexion\n");
      abroConexion=1;
      conexion_BBDD();
    }
    else
    {
    	fprintf(stdout,"Comprueba conexion sqlca.sqlcode >%d<\n", sqlca.sqlcode);   
    }

    //fprintf(stdout,"Dentro filefun.cpp tratar_fich, con opcion >%s< ficher >%s< clave >%s<\n",opcion,ficher,clave); //PRJ0247167
	
		// fprintf(stdout,"RAQUEL ANTES DEL SWITCH  OPCION \n");
    // fflush(stdout);

		switch (opcion)
  	{
// * añadido para PRICE *//

  		case ESCRIBIR :
    		strncpy(tabla, (char *)ficher, 7);
      	strncpy(opcionT, "INSERTAR",8);
	      opcionT[8]='\0';
				//printf("escribir\n");
				//fflush(stdout);
	      if (!strncmp(tabla, "NPTCASA", 7)) //directamente
	      {
	      	ErrInsertar=InsertarC(registro);
	              //return(ErrInsertar);
				  if(ErrInsertar)
				  	return(salir_rb(abroConexion, ErrInsertar, tabla, opcion));
				  else
				  	return(salir_cm(abroConexion, ErrInsertar, tabla, opcion));
	     	}
	     	if (!strncmp(tabla, "NPTLOGO", 7)) //directamente
	      {
	      	ErrInsertar=InsertarL(registro);
	        //return(ErrInsertar);
			    if(ErrInsertar)
			    	return(salir_rb(abroConexion, ErrInsertar, tabla, opcion));
				  else
				  	return(salir_cm(abroConexion, ErrInsertar, tabla, opcion));
	     	}
	      if (!strncmp(tabla, "NPTDSCD", 7)) //directamente
	      {
	      	ErrInsertar=InsertarD(registro);
	        //return(ErrInsertar);
			    if(ErrInsertar)
			    	return(salir_rb(abroConexion, ErrInsertar, tabla, opcion));
				  else
				  	return(salir_cm(abroConexion, ErrInsertar, tabla, opcion));
	    	}
	      if (!strncmp(tabla, "NPTDIFE", 7)) //directamente
	      {
	      	ErrInsertar=InsertarDF(registro);
	        //return(ErrInsertar);
			    if(ErrInsertar)
			    	return(salir_rb(abroConexion, ErrInsertar, tabla, opcion));
				  else
				  	return(salir_cm(abroConexion, ErrInsertar, tabla, opcion));	  
	     	}
				//PRJ288711 - INI
	      if (!strncmp(tabla, "NPTERRD", 7)) //directamente
	      {
	      	printf(". . . . Entrando por NPTERRD de modificacion . . . .\n");
	      	
	      	ErrInsertar=InsertarDE(registro);
	        //return(ErrInsertar);
			    if(ErrInsertar)
			    	return(salir_rb(abroConexion, ErrInsertar, tabla, opcion));
				  else
				  	return(salir_cm(abroConexion, ErrInsertar, tabla, opcion));	  
	     	}
				//PRJ288711 - FIN		   
	      strncpy(claveT, registro, strlen(registro));
	      retorReg=1;
	           
	      break;

		case CARGAR :
    	//con una llamada cargamos el cursor que nos interesa: NPTDISO, NPTDAP2 y NPTBIMA
      // printf("cargar\n");
		   
	    //fprintf(stdout,"RAQUEL opcion cargar \n");
      //fflush(stdout);
      strncpy (claveT, clave, (strlen(clave)));
      strncpy(tabla, (char *)ficher, 7);
		 	//  fprintf(stdout,"RAQUEL opcion cargar claveT %s\n",claveT);
      //  fflush(stdout);
      break;
			//* añadido para PRICE*//           
        
        
    	case REESCRIBIR :
      	//  printf("reescribir\n");
        strncpy(tabla, (char *)ficher, 7);
        if(!strncmp(tabla,"NumeroC",7))
        {
             strncpy (registro, clave, (strlen(clave)));
             strncpy(opcionT, "NUMEROOPE",9);
             opcionT[9]='\0';
             strncpy(claveT, clave, 1);
             claveT[1]='\0';
                retorReg=0;
            }
           else
           {
             strncpy(opcionT, "UPDATE",8);
             opcionT[8]='\0';
             if (!strncmp(tabla, "NPTLOGO", 7)) //directamente
             {
               ErrInsertar=InsertarL(registro);
                  //return(ErrInsertar);
                 if(ErrInsertar)
                    return(salir_rb(abroConexion, ErrInsertar, tabla, opcion));
                 else
                    return(salir_cm(abroConexion, ErrInsertar, tabla, opcion));
             }
             if (!strncmp(tabla, "ANULAOP", 7) || !strncmp(tabla, "HSMDRIP", 7) )
               strncpy (claveT, clave, (strlen(clave)));
             else
               strncpy (claveT, registro, strlen(registro));
                retorReg=1;
             }
           
           break;

			//* añadido para PRICE *//

      case FECMODIF :
           strncpy(tabla, (char *)ficher, 7);
           strncpy(opcionT, "FECMODIF",8);
           opcionT[8]='\0';
           strncpy (claveT, "   ", 3);
           claveT[3]='\0';
           retorReg=0;
          // printf("fecmodif\n");
           break;

      case ACTIVAR :
         //  printf("ACTIVAR\n");
           strncpy(opcionT, "ACTIVA", 6);
           opcionT[6]='\0';
           strncpy(tabla,(char *)ficher, 7);
           retorReg=1; 
           strncpy (claveT, clave, (strlen(clave)));
           break;

      case DESACTIVAR :
         //  printf("DESACTIVAR\n");
           strncpy(opcionT, "DACTIVA", 7);
           opcionT[7]='\0';
           strncpy(tabla,(char *)ficher, 7);
           retorReg=1; 
           strncpy (claveT, clave, (strlen(clave)));
           break;

      case SEMAF :
        //   printf("SEMAF\n");
           strncpy(opcionT, "SEMAF", 5);
           opcionT[5]='\0';
           strncpy(tabla,(char *)ficher, 7);
           retorReg=0; 
           strncpy (claveT, clave, (strlen(clave)));
           break;

      case SEMAF00 :
          // printf("SEMAF00\n");
           strncpy (claveT, clave, (strlen(clave)));
           strncpy(opcionT, "SEMAF00", 7);
           opcionT[7]='\0';
           strncpy(tabla, (char *)ficher, 7);
           retorReg=1;
           break;

      case PACONCIL :
        //   printf("paconcil \n");
           strncpy (claveT, clave, (strlen(clave)));
           strncpy(opcionT, "PACONCI", 7);
           opcionT[7]='\0';
           strncpy(tabla, (char *)ficher, 7);
           retorReg=1;
           break;

      case MONITOR :
         //  printf("monitor \n");
           strncpy(opcionT, "MONITOR", 7);
           opcionT[7]='\0';
           strncpy (claveT, clave, (strlen(clave)));
           strncpy(tabla, (char *)ficher, 7);
           retorReg=0;
           break;


      case TOTAL :
         //  printf("TOTAL\n");
           strncpy(opcionT, "TOTAL", 5);
           opcionT[5]='\0';
           strncpy(tabla, (char *)ficher, 7);
           strncpy (claveT, clave, (strlen(clave)));
           retorReg=1;
           break;

      case IGUALBORRAR :
         //  printf("IGUALBORRAR\n");
           strncpy(opcionT, "IGUALBOR", 8);
           opcionT[8]='\0';
           strncpy(tabla, (char *)ficher, 7);
           strncpy (claveT, registro, (strlen(clave)));
            retorReg=0;
           break;


      case BORRAR :
        //   printf("BORRAR\n");
           strncpy (claveT, clave, (strlen(clave)));
           strncpy(opcionT, "BORRAR", 6);
           opcionT[6]='\0';
           strncpy(tabla, (char *)ficher, 7);
           retorReg=1;
           break;
			//* añadido para PRICE *//

      case IGUAL :
          // printf("IGUAL\n");
           strncpy (claveT, clave, (strlen(clave)));
           strncpy(opcionT, "IGUAL", 5);
           opcionT[5]='\0';
           strncpy(tabla, (char *)ficher, 7);
           retorReg=0;
          break;
          
			//* añadido para PRICE*//
      case MAYORIGUAL :
        //   printf("MAYORIGUAL\n");
           strncpy(tabla, ficher, 7);
           strncpy (claveT, clave,(strlen(clave)));
           strncpy(opcionT, "MAYORIGUAL", 10);
           retorReg=0;
          break;

      case PRIMERO :
         //  printf("PRIMERO\n");
           strncpy(tabla, ficher, 7);
            //printf("recupero primero\n");
            strncpy(opcionT, "PRIMERO", 7 );
            opcionT[7]='\0';
            strncpy(claveT, "00", 2);
            claveT[2]='\0';
            //printf("llamamos oracle %s\n", tabla);
            
            retorReg=0;
            break;

      case SIGUIENTE :
         //  printf("siguiente\n");
           strncpy (claveT, clave, (strlen(clave)));
           strncpy(opcionT, "SIGUIENTE",9);
           opcionT[9]='\0';
           strncpy(tabla, (char *)ficher, 7);
           retorReg=0;
           break;

      case SESIONF :
         //  printf("sesion F \n");
           strncpy (claveT, clave, 13);
           strncpy(opcionT, "SESION", 6);
           opcionT[6]='\0';
           strncpy(tabla, (char *)ficher, 7);
           retorReg=1;
          break;

      case ACTSALDO  :
         //  printf("ACTSALDO\n");
           strncpy(opcionT, "ACTSALDO", 8);
           opcionT[8]='\0';
           strncpy(tabla, (char *)ficher, 7);
           strncpy (claveT, clave,(strlen(clave)));
           retorReg=1;

          break;
      case MODIFESTADO:
         //  printf("modificar estado\n");
           strncpy(opcionT, "MODIFESTADO", 11);
           opcionT[11]='\0';
           strncpy(tabla, (char *)ficher, 7);
           strncpy (claveT, clave, (strlen(clave)));
           retorReg=1;
           break;

      case COMPENS :
        //   printf("netos por tipo compensacion");
           strncpy(opcionT, "COMPENS", 7);
           opcionT[9]='\0';
           strncpy (claveT, clave, (strlen(clave)));
           strncpy(tabla, (char *)ficher, 7);
           retorReg=0;
           break;
				// * añadido para PRICE* //
    }
    tabla[7]='\0';
    claveT[strlen(claveT)]='\0'; 
    //Inicializamos el error
    memset(CodError, '\0', 4); 
 
//* añadido para PRICE*//
   if (opcion == CARGAR )
   {
   		// printf("if (opcion == CARGAR )\n");
	   
	    fprintf(stdout,"RAQUEL dentro if CARGAR \n");
      fflush(stdout);
      printf("tabla %s \n",tabla);
      printf("claveT %s \n",claveT);
			fflush(stdout);
       
      /* EXEC SQL ALLOCATE :registro3; */ 

{
      struct sqlexd sqlstm;
      sqlstm.sqlvsn = 13;
      sqlstm.arrsiz = 4;
      sqlstm.sqladtp = &sqladt;
      sqlstm.sqltdsp = &sqltds;
      sqlstm.stmt = "";
      sqlstm.iters = (unsigned int  )1;
      sqlstm.offset = (unsigned int  )171;
      sqlstm.cud = sqlcud0;
      sqlstm.sqlest = (unsigned char  *)&sqlca;
      sqlstm.sqlety = (unsigned short)4352;
      sqlstm.occurs = (unsigned int  )0;
      sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
      sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
      sqlstm.sqhsts[0] = (         int  )0;
      sqlstm.sqindv[0] = (         short *)0;
      sqlstm.sqinds[0] = (         int  )0;
      sqlstm.sqharm[0] = (unsigned long )0;
      sqlstm.sqadto[0] = (unsigned short )0;
      sqlstm.sqtdso[0] = (unsigned short )0;
      sqlstm.sqphsv = sqlstm.sqhstv;
      sqlstm.sqphsl = sqlstm.sqhstl;
      sqlstm.sqphss = sqlstm.sqhsts;
      sqlstm.sqpind = sqlstm.sqindv;
      sqlstm.sqpins = sqlstm.sqinds;
      sqlstm.sqparm = sqlstm.sqharm;
      sqlstm.sqparc = sqlstm.sqharc;
      sqlstm.sqpadto = sqlstm.sqadto;
      sqlstm.sqptdso = sqlstm.sqtdso;
      sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


       // printf("despues del ALLOCATE \n");
        /* EXEC SQL CALL CARGARTABLA (:tabla, :claveT, :registro3, :CodError); */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 13;
        sqlstm.arrsiz = 4;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "call CARGARTABLA(:b0,:b1,:b2,:b3)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )190;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (unsigned char  *)tabla;
        sqlstm.sqhstl[0] = (unsigned long )8;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (unsigned char  *)claveT;
        sqlstm.sqhstl[1] = (unsigned long )200;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         short *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned long )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (unsigned char  *)&registro3;
        sqlstm.sqhstl[2] = (unsigned long )sizeof(int);
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         short *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned long )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (unsigned char  *)CodError;
        sqlstm.sqhstl[3] = (unsigned long )4;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         short *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned long )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
        
        printf("CodError %s \n", CodError);
       
        if (!strncmp(CodError, "000", 3)) //retorno OK
        {
           /* EXEC SQL WHENEVER NOT FOUND DO break; */ 
 
 
           ++registro3.curocn;
           ll=0;
           if (!strncmp(tabla, "NPTDAP2", 7))
           {
                tabla_apl[0].tipo_fc = '.';
                tabla_apl[0].long_fc = 0;
                tabla_apl[0].max_fc = 0;
                tabla_apl[0].off_fc = 0;
           }
           for (;;)
           {
              //tratar cada tabla
              if (!strncmp(tabla, "NPTTRMS", 7))
              {
                 /* EXEC SQL FETCH :registro3
                     INTO :estrucTRMS; */ 

{
                 struct sqlexd sqlstm;
                 sqlstm.sqlvsn = 13;
                 sqlstm.arrsiz = 7;
                 sqlstm.sqladtp = &sqladt;
                 sqlstm.sqltdsp = &sqltds;
                 sqlstm.iters = (unsigned int  )1;
                 sqlstm.offset = (unsigned int  )221;
                 sqlstm.selerr = (unsigned short)1;
                 sqlstm.sqlpfmem = (unsigned int  )0;
                 sqlstm.cud = sqlcud0;
                 sqlstm.sqlest = (unsigned char  *)&sqlca;
                 sqlstm.sqlety = (unsigned short)4352;
                 sqlstm.occurs = (unsigned int  )0;
                 sqlstm.sqfoff = (         int )0;
                 sqlstm.sqfmod = (unsigned int )2;
                 sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[0] = (         int  )0;
                 sqlstm.sqindv[0] = (         short *)0;
                 sqlstm.sqinds[0] = (         int  )0;
                 sqlstm.sqharm[0] = (unsigned long )0;
                 sqlstm.sqadto[0] = (unsigned short )0;
                 sqlstm.sqtdso[0] = (unsigned short )0;
                 sqlstm.sqhstv[1] = (unsigned char  *)estrucTRMS.trmsemis;
                 sqlstm.sqhstl[1] = (unsigned long )3;
                 sqlstm.sqhsts[1] = (         int  )0;
                 sqlstm.sqindv[1] = (         short *)0;
                 sqlstm.sqinds[1] = (         int  )0;
                 sqlstm.sqharm[1] = (unsigned long )0;
                 sqlstm.sqadto[1] = (unsigned short )0;
                 sqlstm.sqtdso[1] = (unsigned short )0;
                 sqlstm.sqhstv[2] = (unsigned char  *)estrucTRMS.trmsidtm;
                 sqlstm.sqhstl[2] = (unsigned long )5;
                 sqlstm.sqhsts[2] = (         int  )0;
                 sqlstm.sqindv[2] = (         short *)0;
                 sqlstm.sqinds[2] = (         int  )0;
                 sqlstm.sqharm[2] = (unsigned long )0;
                 sqlstm.sqadto[2] = (unsigned short )0;
                 sqlstm.sqtdso[2] = (unsigned short )0;
                 sqlstm.sqhstv[3] = (unsigned char  *)estrucTRMS.trmscopr;
                 sqlstm.sqhstl[3] = (unsigned long )7;
                 sqlstm.sqhsts[3] = (         int  )0;
                 sqlstm.sqindv[3] = (         short *)0;
                 sqlstm.sqinds[3] = (         int  )0;
                 sqlstm.sqharm[3] = (unsigned long )0;
                 sqlstm.sqadto[3] = (unsigned short )0;
                 sqlstm.sqtdso[3] = (unsigned short )0;
                 sqlstm.sqhstv[4] = (unsigned char  *)estrucTRMS.trmsidtv;
                 sqlstm.sqhstl[4] = (unsigned long )5;
                 sqlstm.sqhsts[4] = (         int  )0;
                 sqlstm.sqindv[4] = (         short *)0;
                 sqlstm.sqinds[4] = (         int  )0;
                 sqlstm.sqharm[4] = (unsigned long )0;
                 sqlstm.sqadto[4] = (unsigned short )0;
                 sqlstm.sqtdso[4] = (unsigned short )0;
                 sqlstm.sqhstv[5] = (unsigned char  *)estrucTRMS.trmsidsi;
                 sqlstm.sqhstl[5] = (unsigned long )5;
                 sqlstm.sqhsts[5] = (         int  )0;
                 sqlstm.sqindv[5] = (         short *)0;
                 sqlstm.sqinds[5] = (         int  )0;
                 sqlstm.sqharm[5] = (unsigned long )0;
                 sqlstm.sqadto[5] = (unsigned short )0;
                 sqlstm.sqtdso[5] = (unsigned short )0;
                 sqlstm.sqhstv[6] = (unsigned char  *)estrucTRMS.trmscosi;
                 sqlstm.sqhstl[6] = (unsigned long )7;
                 sqlstm.sqhsts[6] = (         int  )0;
                 sqlstm.sqindv[6] = (         short *)0;
                 sqlstm.sqinds[6] = (         int  )0;
                 sqlstm.sqharm[6] = (unsigned long )0;
                 sqlstm.sqadto[6] = (unsigned short )0;
                 sqlstm.sqtdso[6] = (unsigned short )0;
                 sqlstm.sqphsv = sqlstm.sqhstv;
                 sqlstm.sqphsl = sqlstm.sqhstl;
                 sqlstm.sqphss = sqlstm.sqhsts;
                 sqlstm.sqpind = sqlstm.sqindv;
                 sqlstm.sqpins = sqlstm.sqinds;
                 sqlstm.sqparm = sqlstm.sqharm;
                 sqlstm.sqparc = sqlstm.sqharc;
                 sqlstm.sqpadto = sqlstm.sqadto;
                 sqlstm.sqptdso = sqlstm.sqtdso;
                 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                 if (sqlca.sqlcode == 1403) break;
}

 
                     
                if (ll >=500)
                {
                   //EXEC SQL ROLLBACK RELEASE;
                   //return (101);
				   return(salir_rb(abroConexion, 101, tabla, opcion));
                }

                strncpy(tabla_trms[ll].trms_emis, estrucTRMS.trmsemis , 2);
                strncpy(tabla_trms[ll].trms_idtm, estrucTRMS.trmsidtm , 4);
                strncpy(tabla_trms[ll].trms_copr, estrucTRMS.trmscopr , 6);
                strncpy(tabla_trms[ll].trms_idtv, estrucTRMS.trmsidtv , 4);
                strncpy(tabla_trms[ll].trms_idsi, estrucTRMS.trmsidsi , 4);
                strncpy(tabla_trms[ll].trms_cosi, estrucTRMS.trmscosi , 6); 
                ll++;  
              }
			  
			  //RAQUEL añadido para cargar configuracion y colas entidades unificadas bbdd
			  if (!strncmp(tabla, "NPTIPCS", 7))
              {
								// fprintf(stdout,"RAQUEL FILEFUN ENTRANDO CARGAR TABLA NPTIPCS\n");
                // fflush(stdout);
                 /* EXEC SQL FETCH :registro3
                     INTO :estrucIPCS; */ 

{
                 struct sqlexd sqlstm;
                 sqlstm.sqlvsn = 13;
                 sqlstm.arrsiz = 7;
                 sqlstm.sqladtp = &sqladt;
                 sqlstm.sqltdsp = &sqltds;
                 sqlstm.iters = (unsigned int  )1;
                 sqlstm.offset = (unsigned int  )264;
                 sqlstm.selerr = (unsigned short)1;
                 sqlstm.sqlpfmem = (unsigned int  )0;
                 sqlstm.cud = sqlcud0;
                 sqlstm.sqlest = (unsigned char  *)&sqlca;
                 sqlstm.sqlety = (unsigned short)4352;
                 sqlstm.occurs = (unsigned int  )0;
                 sqlstm.sqfoff = (         int )0;
                 sqlstm.sqfmod = (unsigned int )2;
                 sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[0] = (         int  )0;
                 sqlstm.sqindv[0] = (         short *)0;
                 sqlstm.sqinds[0] = (         int  )0;
                 sqlstm.sqharm[0] = (unsigned long )0;
                 sqlstm.sqadto[0] = (unsigned short )0;
                 sqlstm.sqtdso[0] = (unsigned short )0;
                 sqlstm.sqhstv[1] = (unsigned char  *)estrucIPCS.ipcsindi;
                 sqlstm.sqhstl[1] = (unsigned long )3;
                 sqlstm.sqhsts[1] = (         int  )0;
                 sqlstm.sqindv[1] = (         short *)0;
                 sqlstm.sqinds[1] = (         int  )0;
                 sqlstm.sqharm[1] = (unsigned long )0;
                 sqlstm.sqadto[1] = (unsigned short )0;
                 sqlstm.sqtdso[1] = (unsigned short )0;
                 sqlstm.sqhstv[2] = (unsigned char  *)estrucIPCS.ipcsnsvm;
                 sqlstm.sqhstl[2] = (unsigned long )8;
                 sqlstm.sqhsts[2] = (         int  )0;
                 sqlstm.sqindv[2] = (         short *)0;
                 sqlstm.sqinds[2] = (         int  )0;
                 sqlstm.sqharm[2] = (unsigned long )0;
                 sqlstm.sqadto[2] = (unsigned short )0;
                 sqlstm.sqtdso[2] = (unsigned short )0;
                 sqlstm.sqhstv[3] = (unsigned char  *)estrucIPCS.ipcsnomb;
                 sqlstm.sqhstl[3] = (unsigned long )5;
                 sqlstm.sqhsts[3] = (         int  )0;
                 sqlstm.sqindv[3] = (         short *)0;
                 sqlstm.sqinds[3] = (         int  )0;
                 sqlstm.sqharm[3] = (unsigned long )0;
                 sqlstm.sqadto[3] = (unsigned short )0;
                 sqlstm.sqtdso[3] = (unsigned short )0;
                 sqlstm.sqhstv[4] = (unsigned char  *)estrucIPCS.ipcsclav;
                 sqlstm.sqhstl[4] = (unsigned long )2;
                 sqlstm.sqhsts[4] = (         int  )0;
                 sqlstm.sqindv[4] = (         short *)0;
                 sqlstm.sqinds[4] = (         int  )0;
                 sqlstm.sqharm[4] = (unsigned long )0;
                 sqlstm.sqadto[4] = (unsigned short )0;
                 sqlstm.sqtdso[4] = (unsigned short )0;
                 sqlstm.sqhstv[5] = (unsigned char  *)&estrucIPCS.ipcstama;
                 sqlstm.sqhstl[5] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[5] = (         int  )0;
                 sqlstm.sqindv[5] = (         short *)0;
                 sqlstm.sqinds[5] = (         int  )0;
                 sqlstm.sqharm[5] = (unsigned long )0;
                 sqlstm.sqadto[5] = (unsigned short )0;
                 sqlstm.sqtdso[5] = (unsigned short )0;
                 sqlstm.sqphsv = sqlstm.sqhstv;
                 sqlstm.sqphsl = sqlstm.sqhstl;
                 sqlstm.sqphss = sqlstm.sqhsts;
                 sqlstm.sqpind = sqlstm.sqindv;
                 sqlstm.sqpins = sqlstm.sqinds;
                 sqlstm.sqparm = sqlstm.sqharm;
                 sqlstm.sqparc = sqlstm.sqharc;
                 sqlstm.sqpadto = sqlstm.sqadto;
                 sqlstm.sqptdso = sqlstm.sqtdso;
                 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                 if (sqlca.sqlcode == 1403) break;
}

 
                // fprintf(stdout,"RAQUEL FILEFUN DESPUES EXEC TABLA NPTIPCS\n");  
								// fprintf(stdout,"RAQUEL DESPUES EXEC registro3 %s\n",registro3);  
								// fprintf(stdout,"RAQUEL DESPUES EXEC estrucIPCS %s\n",estrucIPCS); 
								// fflush(stdout);
                if (ll >=100)
                {
                   //EXEC SQL ROLLBACK RELEASE;
                   //return (101);
				   				return(salir_rb(abroConexion, 101, tabla, opcion));
                }

                strncpy(tabla_ipcs[ll].ipcs_indi, estrucIPCS.ipcsindi , 3);
								strncpy(tabla_ipcs[ll].ipcs_nsvm, estrucIPCS.ipcsnsvm , 8);
                strncpy(tabla_ipcs[ll].ipcs_nomb, estrucIPCS.ipcsnomb , 5);   
                strncpy(tabla_ipcs[ll].ipcs_clav, estrucIPCS.ipcsclav , 2);
								tabla_ipcs[ll].ipcs_tama=estrucIPCS.ipcstama;         
				ll++;  
              }
			  if (!strncmp(tabla, "NPTCONF", 7))
              {
				 				//fprintf(stdout,"RAQUEL FILEFUN ENTRANDO CARGAR TABLA NPTCONF\n");
                 //fflush(stdout);
                 /* EXEC SQL FETCH :registro3
                     INTO :estrucCONF; */ 

{
                 struct sqlexd sqlstm;
                 sqlstm.sqlvsn = 13;
                 sqlstm.arrsiz = 10;
                 sqlstm.sqladtp = &sqladt;
                 sqlstm.sqltdsp = &sqltds;
                 sqlstm.iters = (unsigned int  )1;
                 sqlstm.offset = (unsigned int  )303;
                 sqlstm.selerr = (unsigned short)1;
                 sqlstm.sqlpfmem = (unsigned int  )0;
                 sqlstm.cud = sqlcud0;
                 sqlstm.sqlest = (unsigned char  *)&sqlca;
                 sqlstm.sqlety = (unsigned short)4352;
                 sqlstm.occurs = (unsigned int  )0;
                 sqlstm.sqfoff = (         int )0;
                 sqlstm.sqfmod = (unsigned int )2;
                 sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[0] = (         int  )0;
                 sqlstm.sqindv[0] = (         short *)0;
                 sqlstm.sqinds[0] = (         int  )0;
                 sqlstm.sqharm[0] = (unsigned long )0;
                 sqlstm.sqadto[0] = (unsigned short )0;
                 sqlstm.sqtdso[0] = (unsigned short )0;
                 sqlstm.sqhstv[1] = (unsigned char  *)estrucCONF.confindi;
                 sqlstm.sqhstl[1] = (unsigned long )3;
                 sqlstm.sqhsts[1] = (         int  )0;
                 sqlstm.sqindv[1] = (         short *)0;
                 sqlstm.sqinds[1] = (         int  )0;
                 sqlstm.sqharm[1] = (unsigned long )0;
                 sqlstm.sqadto[1] = (unsigned short )0;
                 sqlstm.sqtdso[1] = (unsigned short )0;
                 sqlstm.sqhstv[2] = (unsigned char  *)estrucCONF.confcdsu;
                 sqlstm.sqhstl[2] = (unsigned long )5;
                 sqlstm.sqhsts[2] = (         int  )0;
                 sqlstm.sqindv[2] = (         short *)0;
                 sqlstm.sqinds[2] = (         int  )0;
                 sqlstm.sqharm[2] = (unsigned long )0;
                 sqlstm.sqadto[2] = (unsigned short )0;
                 sqlstm.sqtdso[2] = (unsigned short )0;
                 sqlstm.sqhstv[3] = (unsigned char  *)estrucCONF.confdsej;
                 sqlstm.sqhstl[3] = (unsigned long )9;
                 sqlstm.sqhsts[3] = (         int  )0;
                 sqlstm.sqindv[3] = (         short *)0;
                 sqlstm.sqinds[3] = (         int  )0;
                 sqlstm.sqharm[3] = (unsigned long )0;
                 sqlstm.sqadto[3] = (unsigned short )0;
                 sqlstm.sqtdso[3] = (unsigned short )0;
                 sqlstm.sqhstv[4] = (unsigned char  *)estrucCONF.confpara;
                 sqlstm.sqhstl[4] = (unsigned long )21;
                 sqlstm.sqhsts[4] = (         int  )0;
                 sqlstm.sqindv[4] = (         short *)0;
                 sqlstm.sqinds[4] = (         int  )0;
                 sqlstm.sqharm[4] = (unsigned long )0;
                 sqlstm.sqadto[4] = (unsigned short )0;
                 sqlstm.sqtdso[4] = (unsigned short )0;
                 sqlstm.sqhstv[5] = (unsigned char  *)estrucCONF.confcola;
                 sqlstm.sqhstl[5] = (unsigned long )5;
                 sqlstm.sqhsts[5] = (         int  )0;
                 sqlstm.sqindv[5] = (         short *)0;
                 sqlstm.sqinds[5] = (         int  )0;
                 sqlstm.sqharm[5] = (unsigned long )0;
                 sqlstm.sqadto[5] = (unsigned short )0;
                 sqlstm.sqtdso[5] = (unsigned short )0;
                 sqlstm.sqhstv[6] = (unsigned char  *)estrucCONF.conftipp;
                 sqlstm.sqhstl[6] = (unsigned long )2;
                 sqlstm.sqhsts[6] = (         int  )0;
                 sqlstm.sqindv[6] = (         short *)0;
                 sqlstm.sqinds[6] = (         int  )0;
                 sqlstm.sqharm[6] = (unsigned long )0;
                 sqlstm.sqadto[6] = (unsigned short )0;
                 sqlstm.sqtdso[6] = (unsigned short )0;
                 sqlstm.sqhstv[7] = (unsigned char  *)&estrucCONF.confnmpr;
                 sqlstm.sqhstl[7] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[7] = (         int  )0;
                 sqlstm.sqindv[7] = (         short *)0;
                 sqlstm.sqinds[7] = (         int  )0;
                 sqlstm.sqharm[7] = (unsigned long )0;
                 sqlstm.sqadto[7] = (unsigned short )0;
                 sqlstm.sqtdso[7] = (unsigned short )0;
                 sqlstm.sqhstv[8] = (unsigned char  *)&estrucCONF.confcapr;
                 sqlstm.sqhstl[8] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[8] = (         int  )0;
                 sqlstm.sqindv[8] = (         short *)0;
                 sqlstm.sqinds[8] = (         int  )0;
                 sqlstm.sqharm[8] = (unsigned long )0;
                 sqlstm.sqadto[8] = (unsigned short )0;
                 sqlstm.sqtdso[8] = (unsigned short )0;
                 sqlstm.sqhstv[9] = (unsigned char  *)estrucCONF.confcsal;
                 sqlstm.sqhstl[9] = (unsigned long )5;
                 sqlstm.sqhsts[9] = (         int  )0;
                 sqlstm.sqindv[9] = (         short *)0;
                 sqlstm.sqinds[9] = (         int  )0;
                 sqlstm.sqharm[9] = (unsigned long )0;
                 sqlstm.sqadto[9] = (unsigned short )0;
                 sqlstm.sqtdso[9] = (unsigned short )0;
                 sqlstm.sqphsv = sqlstm.sqhstv;
                 sqlstm.sqphsl = sqlstm.sqhstl;
                 sqlstm.sqphss = sqlstm.sqhsts;
                 sqlstm.sqpind = sqlstm.sqindv;
                 sqlstm.sqpins = sqlstm.sqinds;
                 sqlstm.sqparm = sqlstm.sqharm;
                 sqlstm.sqparc = sqlstm.sqharc;
                 sqlstm.sqpadto = sqlstm.sqadto;
                 sqlstm.sqptdso = sqlstm.sqtdso;
                 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                 if (sqlca.sqlcode == 1403) break;
}

 
								// fprintf(stdout,"RAQUEL FILEFUN DESPUES EXEC TABLA NPTCONF\n");  
								// fprintf(stdout,"RAQUEL DESPUES EXEC registro3 %s\n",registro3);  
								// fprintf(stdout,"RAQUEL DESPUES EXEC estrucCONF %s\n",estrucCONF); 
								// fflush(stdout);
                     
                if (ll >=100)
                {
                   //EXEC SQL ROLLBACK RELEASE;
                   //return (101);
				   return(salir_rb(abroConexion, 101, tabla, opcion));
                }
                strncpy(tabla_conf[ll].conf_indi, estrucCONF.confindi, 3);    
                strncpy(tabla_conf[ll].conf_cdsu, estrucCONF.confcdsu, 5);
                strncpy(tabla_conf[ll].conf_dsej, estrucCONF.confdsej, 11);
                strncpy(tabla_conf[ll].conf_para, estrucCONF.confpara, 21);
                strncpy(tabla_conf[ll].conf_cola, estrucCONF.confcola, 5);
								strncpy(tabla_conf[ll].conf_tipp, estrucCONF.conftipp, 2);
								tabla_conf[ll].conf_nmpr=estrucCONF.confnmpr; 
								tabla_conf[ll].conf_capr=estrucCONF.confcapr;				
                strncpy(tabla_conf[ll].conf_csal, estrucCONF.confcsal, 5); 				
                ll++;  
              }
			  
			  if (!strncmp(tabla, "NPTPARA", 7))
              {
				// fprintf(stdout,"RAQUEL FILEFUN ENTRANDO CARGAR TABLA NPTPARA\n");
                // fflush(stdout);
                 /* EXEC SQL FETCH :registro3
                     INTO :estrucPARA; */ 

{
                 struct sqlexd sqlstm;
                 sqlstm.sqlvsn = 13;
                 sqlstm.arrsiz = 21;
                 sqlstm.sqladtp = &sqladt;
                 sqlstm.sqltdsp = &sqltds;
                 sqlstm.iters = (unsigned int  )1;
                 sqlstm.offset = (unsigned int  )358;
                 sqlstm.selerr = (unsigned short)1;
                 sqlstm.sqlpfmem = (unsigned int  )0;
                 sqlstm.cud = sqlcud0;
                 sqlstm.sqlest = (unsigned char  *)&sqlca;
                 sqlstm.sqlety = (unsigned short)4352;
                 sqlstm.occurs = (unsigned int  )0;
                 sqlstm.sqfoff = (         int )0;
                 sqlstm.sqfmod = (unsigned int )2;
                 sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[0] = (         int  )0;
                 sqlstm.sqindv[0] = (         short *)0;
                 sqlstm.sqinds[0] = (         int  )0;
                 sqlstm.sqharm[0] = (unsigned long )0;
                 sqlstm.sqadto[0] = (unsigned short )0;
                 sqlstm.sqtdso[0] = (unsigned short )0;
                 sqlstm.sqhstv[1] = (unsigned char  *)estrucPARA.psindice;
                 sqlstm.sqhstl[1] = (unsigned long )3;
                 sqlstm.sqhsts[1] = (         int  )0;
                 sqlstm.sqindv[1] = (         short *)0;
                 sqlstm.sqinds[1] = (         int  )0;
                 sqlstm.sqharm[1] = (unsigned long )0;
                 sqlstm.sqadto[1] = (unsigned short )0;
                 sqlstm.sqtdso[1] = (unsigned short )0;
                 sqlstm.sqhstv[2] = (unsigned char  *)estrucPARA.pscsb000;
                 sqlstm.sqhstl[2] = (unsigned long )5;
                 sqlstm.sqhsts[2] = (         int  )0;
                 sqlstm.sqindv[2] = (         short *)0;
                 sqlstm.sqinds[2] = (         int  )0;
                 sqlstm.sqharm[2] = (unsigned long )0;
                 sqlstm.sqadto[2] = (unsigned short )0;
                 sqlstm.sqtdso[2] = (unsigned short )0;
                 sqlstm.sqhstv[3] = (unsigned char  *)estrucPARA.psnoment;
                 sqlstm.sqhstl[3] = (unsigned long )21;
                 sqlstm.sqhsts[3] = (         int  )0;
                 sqlstm.sqindv[3] = (         short *)0;
                 sqlstm.sqinds[3] = (         int  )0;
                 sqlstm.sqharm[3] = (unsigned long )0;
                 sqlstm.sqadto[3] = (unsigned short )0;
                 sqlstm.sqtdso[3] = (unsigned short )0;
                 sqlstm.sqhstv[4] = (unsigned char  *)estrucPARA.psnumope;
                 sqlstm.sqhstl[4] = (unsigned long )7;
                 sqlstm.sqhsts[4] = (         int  )0;
                 sqlstm.sqindv[4] = (         short *)0;
                 sqlstm.sqinds[4] = (         int  )0;
                 sqlstm.sqharm[4] = (unsigned long )0;
                 sqlstm.sqadto[4] = (unsigned short )0;
                 sqlstm.sqtdso[4] = (unsigned short )0;
                 sqlstm.sqhstv[5] = (unsigned char  *)estrucPARA.psnument;
                 sqlstm.sqhstl[5] = (unsigned long )3;
                 sqlstm.sqhsts[5] = (         int  )0;
                 sqlstm.sqindv[5] = (         short *)0;
                 sqlstm.sqinds[5] = (         int  )0;
                 sqlstm.sqharm[5] = (unsigned long )0;
                 sqlstm.sqadto[5] = (unsigned short )0;
                 sqlstm.sqtdso[5] = (unsigned short )0;
                 sqlstm.sqhstv[6] = (unsigned char  *)estrucPARA.pshcamse;
                 sqlstm.sqhstl[6] = (unsigned long )7;
                 sqlstm.sqhsts[6] = (         int  )0;
                 sqlstm.sqindv[6] = (         short *)0;
                 sqlstm.sqinds[6] = (         int  )0;
                 sqlstm.sqharm[6] = (unsigned long )0;
                 sqlstm.sqadto[6] = (unsigned short )0;
                 sqlstm.sqtdso[6] = (unsigned short )0;
                 sqlstm.sqhstv[7] = (unsigned char  *)estrucPARA.psfeinse;
                 sqlstm.sqhstl[7] = (unsigned long )9;
                 sqlstm.sqhsts[7] = (         int  )0;
                 sqlstm.sqindv[7] = (         short *)0;
                 sqlstm.sqinds[7] = (         int  )0;
                 sqlstm.sqharm[7] = (unsigned long )0;
                 sqlstm.sqadto[7] = (unsigned short )0;
                 sqlstm.sqtdso[7] = (unsigned short )0;
                 sqlstm.sqhstv[8] = (unsigned char  *)estrucPARA.pshoinse;
                 sqlstm.sqhstl[8] = (unsigned long )7;
                 sqlstm.sqhsts[8] = (         int  )0;
                 sqlstm.sqindv[8] = (         short *)0;
                 sqlstm.sqinds[8] = (         int  )0;
                 sqlstm.sqharm[8] = (unsigned long )0;
                 sqlstm.sqadto[8] = (unsigned short )0;
                 sqlstm.sqtdso[8] = (unsigned short )0;
                 sqlstm.sqhstv[9] = (unsigned char  *)estrucPARA.psmodocs;
                 sqlstm.sqhstl[9] = (unsigned long )2;
                 sqlstm.sqhsts[9] = (         int  )0;
                 sqlstm.sqindv[9] = (         short *)0;
                 sqlstm.sqinds[9] = (         int  )0;
                 sqlstm.sqharm[9] = (unsigned long )0;
                 sqlstm.sqadto[9] = (unsigned short )0;
                 sqlstm.sqtdso[9] = (unsigned short )0;
                 sqlstm.sqhstv[10] = (unsigned char  *)estrucPARA.pstimesp;
                 sqlstm.sqhstl[10] = (unsigned long )7;
                 sqlstm.sqhsts[10] = (         int  )0;
                 sqlstm.sqindv[10] = (         short *)0;
                 sqlstm.sqinds[10] = (         int  )0;
                 sqlstm.sqharm[10] = (unsigned long )0;
                 sqlstm.sqadto[10] = (unsigned short )0;
                 sqlstm.sqtdso[10] = (unsigned short )0;
                 sqlstm.sqhstv[11] = (unsigned char  *)estrucPARA.psnumcross;
                 sqlstm.sqhstl[11] = (unsigned long )11;
                 sqlstm.sqhsts[11] = (         int  )0;
                 sqlstm.sqindv[11] = (         short *)0;
                 sqlstm.sqinds[11] = (         int  )0;
                 sqlstm.sqharm[11] = (unsigned long )0;
                 sqlstm.sqadto[11] = (unsigned short )0;
                 sqlstm.sqtdso[11] = (unsigned short )0;
                 sqlstm.sqhstv[12] = (unsigned char  *)estrucPARA.pscerrando;
                 sqlstm.sqhstl[12] = (unsigned long )2;
                 sqlstm.sqhsts[12] = (         int  )0;
                 sqlstm.sqindv[12] = (         short *)0;
                 sqlstm.sqinds[12] = (         int  )0;
                 sqlstm.sqharm[12] = (unsigned long )0;
                 sqlstm.sqadto[12] = (unsigned short )0;
                 sqlstm.sqtdso[12] = (unsigned short )0;
                 sqlstm.sqhstv[13] = (unsigned char  *)estrucPARA.psresolu;
                 sqlstm.sqhstl[13] = (unsigned long )3;
                 sqlstm.sqhsts[13] = (         int  )0;
                 sqlstm.sqindv[13] = (         short *)0;
                 sqlstm.sqinds[13] = (         int  )0;
                 sqlstm.sqharm[13] = (unsigned long )0;
                 sqlstm.sqadto[13] = (unsigned short )0;
                 sqlstm.sqtdso[13] = (unsigned short )0;
                 sqlstm.sqhstv[14] = (unsigned char  *)estrucPARA.psmonito;
                 sqlstm.sqhstl[14] = (unsigned long )3;
                 sqlstm.sqhsts[14] = (         int  )0;
                 sqlstm.sqindv[14] = (         short *)0;
                 sqlstm.sqinds[14] = (         int  )0;
                 sqlstm.sqharm[14] = (unsigned long )0;
                 sqlstm.sqadto[14] = (unsigned short )0;
                 sqlstm.sqtdso[14] = (unsigned short )0;
                 sqlstm.sqhstv[15] = (unsigned char  *)estrucPARA.pscotrms;
                 sqlstm.sqhstl[15] = (unsigned long )3;
                 sqlstm.sqhsts[15] = (         int  )0;
                 sqlstm.sqindv[15] = (         short *)0;
                 sqlstm.sqinds[15] = (         int  )0;
                 sqlstm.sqharm[15] = (unsigned long )0;
                 sqlstm.sqadto[15] = (unsigned short )0;
                 sqlstm.sqtdso[15] = (unsigned short )0;
                 sqlstm.sqhstv[16] = (unsigned char  *)estrucPARA.psidered;
                 sqlstm.sqhstl[16] = (unsigned long )2;
                 sqlstm.sqhsts[16] = (         int  )0;
                 sqlstm.sqindv[16] = (         short *)0;
                 sqlstm.sqinds[16] = (         int  )0;
                 sqlstm.sqharm[16] = (unsigned long )0;
                 sqlstm.sqadto[16] = (unsigned short )0;
                 sqlstm.sqtdso[16] = (unsigned short )0;
                 sqlstm.sqhstv[17] = (unsigned char  *)estrucPARA.pscltrat;
                 sqlstm.sqhstl[17] = (unsigned long )9;
                 sqlstm.sqhsts[17] = (         int  )0;
                 sqlstm.sqindv[17] = (         short *)0;
                 sqlstm.sqinds[17] = (         int  )0;
                 sqlstm.sqharm[17] = (unsigned long )0;
                 sqlstm.sqadto[17] = (unsigned short )0;
                 sqlstm.sqtdso[17] = (unsigned short )0;
                 sqlstm.sqhstv[18] = (unsigned char  *)estrucPARA.psclenvi;
                 sqlstm.sqhstl[18] = (unsigned long )5;
                 sqlstm.sqhsts[18] = (         int  )0;
                 sqlstm.sqindv[18] = (         short *)0;
                 sqlstm.sqinds[18] = (         int  )0;
                 sqlstm.sqharm[18] = (unsigned long )0;
                 sqlstm.sqadto[18] = (unsigned short )0;
                 sqlstm.sqtdso[18] = (unsigned short )0;
                 sqlstm.sqhstv[19] = (unsigned char  *)estrucPARA.psclrece;
                 sqlstm.sqhstl[19] = (unsigned long )9;
                 sqlstm.sqhsts[19] = (         int  )0;
                 sqlstm.sqindv[19] = (         short *)0;
                 sqlstm.sqinds[19] = (         int  )0;
                 sqlstm.sqharm[19] = (unsigned long )0;
                 sqlstm.sqadto[19] = (unsigned short )0;
                 sqlstm.sqtdso[19] = (unsigned short )0;
                 sqlstm.sqhstv[20] = (unsigned char  *)estrucPARA.psclvhsm;
                 sqlstm.sqhstl[20] = (unsigned long )51;
                 sqlstm.sqhsts[20] = (         int  )0;
                 sqlstm.sqindv[20] = (         short *)0;
                 sqlstm.sqinds[20] = (         int  )0;
                 sqlstm.sqharm[20] = (unsigned long )0;
                 sqlstm.sqadto[20] = (unsigned short )0;
                 sqlstm.sqtdso[20] = (unsigned short )0;
                 sqlstm.sqphsv = sqlstm.sqhstv;
                 sqlstm.sqphsl = sqlstm.sqhstl;
                 sqlstm.sqphss = sqlstm.sqhsts;
                 sqlstm.sqpind = sqlstm.sqindv;
                 sqlstm.sqpins = sqlstm.sqinds;
                 sqlstm.sqparm = sqlstm.sqharm;
                 sqlstm.sqparc = sqlstm.sqharc;
                 sqlstm.sqpadto = sqlstm.sqadto;
                 sqlstm.sqptdso = sqlstm.sqtdso;
                 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                 if (sqlca.sqlcode == 1403) break;
}

 
                     
                if (ll >2)
                {
                   //EXEC SQL ROLLBACK RELEASE;
                   //return (101);
				   				return(salir_rb(abroConexion, 101, tabla, opcion));
                }
								strncpy(tabla_para[ll].ps_indice, estrucPARA.psindice,3);  
								strncpy(tabla_para[ll].ps_csb000, estrucPARA.pscsb000,5);         
								strncpy(tabla_para[ll].ps_noment, estrucPARA.psnoment,21);        
								strncpy(tabla_para[ll].ps_numope, estrucPARA.psnumope,7);         
								strncpy(tabla_para[ll].ps_nument, estrucPARA.psnument,3);         
								strncpy(tabla_para[ll].ps_hcamse, estrucPARA.pshcamse,7);         
								strncpy(tabla_para[ll].ps_feinse, estrucPARA.psfeinse,9);         
								strncpy(tabla_para[ll].ps_hoinse, estrucPARA.pshoinse,7);         
								strncpy(tabla_para[ll].ps_modocs, estrucPARA.psmodocs,2);         
								strncpy(tabla_para[ll].ps_timesp, estrucPARA.pstimesp,7);         
								strncpy(tabla_para[ll].ps_numcross, estrucPARA.psnumcross,11);      
								strncpy(tabla_para[ll].ps_cerrando, estrucPARA.pscerrando,2);       
								strncpy(tabla_para[ll].ps_resolu, estrucPARA.psresolu,3);   
								strncpy(tabla_para[ll].ps_monito, estrucPARA.psmonito,3);   
								strncpy(tabla_para[ll].ps_cotrms, estrucPARA.pscotrms,3);  
								strncpy(tabla_para[ll].ps_idered, estrucPARA.psidered,2);  
								strncpy(tabla_para[ll].ps_cltrat, estrucPARA.pscltrat,9);  
								strncpy(tabla_para[ll].ps_clenvi, estrucPARA.psclenvi,5);  
								strncpy(tabla_para[ll].ps_clrece, estrucPARA.psclrece,9);         
								strncpy(tabla_para[ll].ps_clvhsm, estrucPARA.psclvhsm,51);		
                ll++;  				
              }
              
              if (!strncmp(tabla, "NPTDISO", 7))
              {
                 /* EXEC SQL FETCH :registro3
                     INTO :estrucDISO; */ 

{
                 struct sqlexd sqlstm;
                 sqlstm.sqlvsn = 13;
                 sqlstm.arrsiz = 21;
                 sqlstm.sqladtp = &sqladt;
                 sqlstm.sqltdsp = &sqltds;
                 sqlstm.iters = (unsigned int  )1;
                 sqlstm.offset = (unsigned int  )457;
                 sqlstm.selerr = (unsigned short)1;
                 sqlstm.sqlpfmem = (unsigned int  )0;
                 sqlstm.cud = sqlcud0;
                 sqlstm.sqlest = (unsigned char  *)&sqlca;
                 sqlstm.sqlety = (unsigned short)4352;
                 sqlstm.occurs = (unsigned int  )0;
                 sqlstm.sqfoff = (         int )0;
                 sqlstm.sqfmod = (unsigned int )2;
                 sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[0] = (         int  )0;
                 sqlstm.sqindv[0] = (         short *)0;
                 sqlstm.sqinds[0] = (         int  )0;
                 sqlstm.sqharm[0] = (unsigned long )0;
                 sqlstm.sqadto[0] = (unsigned short )0;
                 sqlstm.sqtdso[0] = (unsigned short )0;
                 sqlstm.sqhstv[1] = (unsigned char  *)estrucDISO.donumiso;
                 sqlstm.sqhstl[1] = (unsigned long )4;
                 sqlstm.sqhsts[1] = (         int  )0;
                 sqlstm.sqindv[1] = (         short *)0;
                 sqlstm.sqinds[1] = (         int  )0;
                 sqlstm.sqharm[1] = (unsigned long )0;
                 sqlstm.sqadto[1] = (unsigned short )0;
                 sqlstm.sqtdso[1] = (unsigned short )0;
                 sqlstm.sqhstv[2] = (unsigned char  *)estrucDISO.dopictur;
                 sqlstm.sqhstl[2] = (unsigned long )2;
                 sqlstm.sqhsts[2] = (         int  )0;
                 sqlstm.sqindv[2] = (         short *)0;
                 sqlstm.sqinds[2] = (         int  )0;
                 sqlstm.sqharm[2] = (unsigned long )0;
                 sqlstm.sqadto[2] = (unsigned short )0;
                 sqlstm.sqtdso[2] = (unsigned short )0;
                 sqlstm.sqhstv[3] = (unsigned char  *)estrucDISO.dolongit;
                 sqlstm.sqhstl[3] = (unsigned long )5;
                 sqlstm.sqhsts[3] = (         int  )0;
                 sqlstm.sqindv[3] = (         short *)0;
                 sqlstm.sqinds[3] = (         int  )0;
                 sqlstm.sqharm[3] = (unsigned long )0;
                 sqlstm.sqadto[3] = (unsigned short )0;
                 sqlstm.sqtdso[3] = (unsigned short )0;
                 sqlstm.sqhstv[4] = (unsigned char  *)estrucDISO.dolonmax;
                 sqlstm.sqhstl[4] = (unsigned long )4;
                 sqlstm.sqhsts[4] = (         int  )0;
                 sqlstm.sqindv[4] = (         short *)0;
                 sqlstm.sqinds[4] = (         int  )0;
                 sqlstm.sqharm[4] = (unsigned long )0;
                 sqlstm.sqadto[4] = (unsigned short )0;
                 sqlstm.sqtdso[4] = (unsigned short )0;
                 sqlstm.sqhstv[5] = (unsigned char  *)estrucDISO.dousomac;
                 sqlstm.sqhstl[5] = (unsigned long )2;
                 sqlstm.sqhsts[5] = (         int  )0;
                 sqlstm.sqindv[5] = (         short *)0;
                 sqlstm.sqinds[5] = (         int  )0;
                 sqlstm.sqharm[5] = (unsigned long )0;
                 sqlstm.sqadto[5] = (unsigned short )0;
                 sqlstm.sqtdso[5] = (unsigned short )0;
                 sqlstm.sqphsv = sqlstm.sqhstv;
                 sqlstm.sqphsl = sqlstm.sqhstl;
                 sqlstm.sqphss = sqlstm.sqhsts;
                 sqlstm.sqpind = sqlstm.sqindv;
                 sqlstm.sqpins = sqlstm.sqinds;
                 sqlstm.sqparm = sqlstm.sqharm;
                 sqlstm.sqparc = sqlstm.sqharc;
                 sqlstm.sqpadto = sqlstm.sqadto;
                 sqlstm.sqptdso = sqlstm.sqtdso;
                 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                 if (sqlca.sqlcode == 1403) break;
}

 

                strncpy(cad_verif , estrucDISO.donumiso , 3);
                cad_verif[3] = '\0';
                indice = atoi(cad_verif) - 1;
                if (indice >=128)
                {
                   //EXEC SQL ROLLBACK RELEASE;
                   //return (101);
				   return(salir_rb(abroConexion, 101, tabla, opcion));
                }

                tabla_iso[indice].tipo_fs=estrucDISO.dopictur[0];

                strncpy(ch05, estrucDISO.dolongit, 4);
                tabla_iso[indice].long_fs = atoi(ch05);
                strncpy(ch04, estrucDISO.dolonmax, 3); //longitud maxima
                tabla_iso[indice].max_fs = atoi(ch04);

                tabla_iso[indice].mac_fs=estrucDISO.dousomac[0];
              }
              if (!strncmp(tabla, "NPTDAP2", 7))
              {
                   /* EXEC SQL FETCH :registro3
                     INTO :estrucDAP2; */ 

{
                   struct sqlexd sqlstm;
                   sqlstm.sqlvsn = 13;
                   sqlstm.arrsiz = 21;
                   sqlstm.sqladtp = &sqladt;
                   sqlstm.sqltdsp = &sqltds;
                   sqlstm.iters = (unsigned int  )1;
                   sqlstm.offset = (unsigned int  )496;
                   sqlstm.selerr = (unsigned short)1;
                   sqlstm.sqlpfmem = (unsigned int  )0;
                   sqlstm.cud = sqlcud0;
                   sqlstm.sqlest = (unsigned char  *)&sqlca;
                   sqlstm.sqlety = (unsigned short)4352;
                   sqlstm.occurs = (unsigned int  )0;
                   sqlstm.sqfoff = (         int )0;
                   sqlstm.sqfmod = (unsigned int )2;
                   sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                   sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                   sqlstm.sqhsts[0] = (         int  )0;
                   sqlstm.sqindv[0] = (         short *)0;
                   sqlstm.sqinds[0] = (         int  )0;
                   sqlstm.sqharm[0] = (unsigned long )0;
                   sqlstm.sqadto[0] = (unsigned short )0;
                   sqlstm.sqtdso[0] = (unsigned short )0;
                   sqlstm.sqhstv[1] = (unsigned char  *)estrucDAP2.a2numiso;
                   sqlstm.sqhstl[1] = (unsigned long )4;
                   sqlstm.sqhsts[1] = (         int  )0;
                   sqlstm.sqindv[1] = (         short *)0;
                   sqlstm.sqinds[1] = (         int  )0;
                   sqlstm.sqharm[1] = (unsigned long )0;
                   sqlstm.sqadto[1] = (unsigned short )0;
                   sqlstm.sqtdso[1] = (unsigned short )0;
                   sqlstm.sqhstv[2] = (unsigned char  *)estrucDAP2.a2pictur;
                   sqlstm.sqhstl[2] = (unsigned long )2;
                   sqlstm.sqhsts[2] = (         int  )0;
                   sqlstm.sqindv[2] = (         short *)0;
                   sqlstm.sqinds[2] = (         int  )0;
                   sqlstm.sqharm[2] = (unsigned long )0;
                   sqlstm.sqadto[2] = (unsigned short )0;
                   sqlstm.sqtdso[2] = (unsigned short )0;
                   sqlstm.sqhstv[3] = (unsigned char  *)estrucDAP2.a2longit;
                   sqlstm.sqhstl[3] = (unsigned long )5;
                   sqlstm.sqhsts[3] = (         int  )0;
                   sqlstm.sqindv[3] = (         short *)0;
                   sqlstm.sqinds[3] = (         int  )0;
                   sqlstm.sqharm[3] = (unsigned long )0;
                   sqlstm.sqadto[3] = (unsigned short )0;
                   sqlstm.sqtdso[3] = (unsigned short )0;
                   sqlstm.sqhstv[4] = (unsigned char  *)estrucDAP2.a2lonmax;
                   sqlstm.sqhstl[4] = (unsigned long )5;
                   sqlstm.sqhsts[4] = (         int  )0;
                   sqlstm.sqindv[4] = (         short *)0;
                   sqlstm.sqinds[4] = (         int  )0;
                   sqlstm.sqharm[4] = (unsigned long )0;
                   sqlstm.sqadto[4] = (unsigned short )0;
                   sqlstm.sqtdso[4] = (unsigned short )0;
                   sqlstm.sqhstv[5] = (unsigned char  *)estrucDAP2.a2offset;
                   sqlstm.sqhstl[5] = (unsigned long )5;
                   sqlstm.sqhsts[5] = (         int  )0;
                   sqlstm.sqindv[5] = (         short *)0;
                   sqlstm.sqinds[5] = (         int  )0;
                   sqlstm.sqharm[5] = (unsigned long )0;
                   sqlstm.sqadto[5] = (unsigned short )0;
                   sqlstm.sqtdso[5] = (unsigned short )0;
                   sqlstm.sqphsv = sqlstm.sqhstv;
                   sqlstm.sqphsl = sqlstm.sqhstl;
                   sqlstm.sqphss = sqlstm.sqhsts;
                   sqlstm.sqpind = sqlstm.sqindv;
                   sqlstm.sqpins = sqlstm.sqinds;
                   sqlstm.sqparm = sqlstm.sqharm;
                   sqlstm.sqparc = sqlstm.sqharc;
                   sqlstm.sqpadto = sqlstm.sqadto;
                   sqlstm.sqptdso = sqlstm.sqtdso;
                   sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                   if (sqlca.sqlcode == 1403) break;
}

 

                  strncpy(cad_verif, estrucDAP2.a2numiso , 3);
                  cad_verif[3] = '\0';
                  indice = atoi(cad_verif) - 1;
                  if (indice >=128)
                  {
                     //EXEC SQL ROLLBACK RELEASE;
					 //return (101);
					 return(salir_rb(abroConexion, 101, tabla, opcion));
                  } 

                  tabla_apl[indice].tipo_fc = estrucDAP2.a2pictur[0];
                  tabla_apl[indice].long_fc = atoi(estrucDAP2.a2longit);
                  tabla_apl[indice].max_fc  = atoi(estrucDAP2.a2lonmax);
                  tabla_apl[indice].off_fc  = atoi(estrucDAP2.a2offset);
              }
              if (!strncmp(tabla, "NPTBIMA", 7))
              {
                if (ll>=100)
                {
                   printf("ll > 100\n");
                   //EXEC SQL ROLLBACK RELEASE;
                   //return(101);
				   return(salir_rb(abroConexion, 101, tabla, opcion));
                }

                  /* EXEC SQL FETCH :registro3
                    INTO :estrucBIMA; */ 

{
                  struct sqlexd sqlstm;
                  sqlstm.sqlvsn = 13;
                  sqlstm.arrsiz = 21;
                  sqlstm.sqladtp = &sqladt;
                  sqlstm.sqltdsp = &sqltds;
                  sqlstm.iters = (unsigned int  )1;
                  sqlstm.offset = (unsigned int  )535;
                  sqlstm.selerr = (unsigned short)1;
                  sqlstm.sqlpfmem = (unsigned int  )0;
                  sqlstm.cud = sqlcud0;
                  sqlstm.sqlest = (unsigned char  *)&sqlca;
                  sqlstm.sqlety = (unsigned short)4352;
                  sqlstm.occurs = (unsigned int  )0;
                  sqlstm.sqfoff = (         int )0;
                  sqlstm.sqfmod = (unsigned int )2;
                  sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                  sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                  sqlstm.sqhsts[0] = (         int  )0;
                  sqlstm.sqindv[0] = (         short *)0;
                  sqlstm.sqinds[0] = (         int  )0;
                  sqlstm.sqharm[0] = (unsigned long )0;
                  sqlstm.sqadto[0] = (unsigned short )0;
                  sqlstm.sqtdso[0] = (unsigned short )0;
                  sqlstm.sqhstv[1] = (unsigned char  *)estrucBIMA.bmtipmen;
                  sqlstm.sqhstl[1] = (unsigned long )5;
                  sqlstm.sqhsts[1] = (         int  )0;
                  sqlstm.sqindv[1] = (         short *)0;
                  sqlstm.sqinds[1] = (         int  )0;
                  sqlstm.sqharm[1] = (unsigned long )0;
                  sqlstm.sqadto[1] = (unsigned short )0;
                  sqlstm.sqtdso[1] = (unsigned short )0;
                  sqlstm.sqhstv[2] = (unsigned char  *)estrucBIMA.bmsentra;
                  sqlstm.sqhstl[2] = (unsigned long )2;
                  sqlstm.sqhsts[2] = (         int  )0;
                  sqlstm.sqindv[2] = (         short *)0;
                  sqlstm.sqinds[2] = (         int  )0;
                  sqlstm.sqharm[2] = (unsigned long )0;
                  sqlstm.sqadto[2] = (unsigned short )0;
                  sqlstm.sqtdso[2] = (unsigned short )0;
                  sqlstm.sqhstv[3] = (unsigned char  *)estrucBIMA.bmpervia;
                  sqlstm.sqhstl[3] = (unsigned long )2;
                  sqlstm.sqhsts[3] = (         int  )0;
                  sqlstm.sqindv[3] = (         short *)0;
                  sqlstm.sqinds[3] = (         int  )0;
                  sqlstm.sqharm[3] = (unsigned long )0;
                  sqlstm.sqadto[3] = (unsigned short )0;
                  sqlstm.sqtdso[3] = (unsigned short )0;
                  sqlstm.sqhstv[4] = (unsigned char  *)estrucBIMA.bmbima01;
                  sqlstm.sqhstl[4] = (unsigned long )193;
                  sqlstm.sqhsts[4] = (         int  )0;
                  sqlstm.sqindv[4] = (         short *)0;
                  sqlstm.sqinds[4] = (         int  )0;
                  sqlstm.sqharm[4] = (unsigned long )0;
                  sqlstm.sqadto[4] = (unsigned short )0;
                  sqlstm.sqtdso[4] = (unsigned short )0;
                  sqlstm.sqhstv[5] = (unsigned char  *)estrucBIMA.bmbima02;
                  sqlstm.sqhstl[5] = (unsigned long )193;
                  sqlstm.sqhsts[5] = (         int  )0;
                  sqlstm.sqindv[5] = (         short *)0;
                  sqlstm.sqinds[5] = (         int  )0;
                  sqlstm.sqharm[5] = (unsigned long )0;
                  sqlstm.sqadto[5] = (unsigned short )0;
                  sqlstm.sqtdso[5] = (unsigned short )0;
                  sqlstm.sqphsv = sqlstm.sqhstv;
                  sqlstm.sqphsl = sqlstm.sqhstl;
                  sqlstm.sqphss = sqlstm.sqhsts;
                  sqlstm.sqpind = sqlstm.sqindv;
                  sqlstm.sqpins = sqlstm.sqinds;
                  sqlstm.sqparm = sqlstm.sqharm;
                  sqlstm.sqparc = sqlstm.sqharc;
                  sqlstm.sqpadto = sqlstm.sqadto;
                  sqlstm.sqptdso = sqlstm.sqtdso;
                  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                  if (sqlca.sqlcode == 1403) break;
}

 

 
                strncpy(tabla_bim[ll].tipo_bm , estrucBIMA.bmtipmen , 4);
                tabla_bim[ll].sent_bm = estrucBIMA.bmsentra[0];

                if(estrucBIMA.bmpervia[0] == '1' || estrucBIMA.bmpervia[0] == '2')
                   tabla_bim[ll].perv_bm = estrucBIMA.bmpervia[0] == '1' ? 'S' : 'N';
                else
                   tabla_bim[ll].perv_bm = estrucBIMA.bmpervia[0] == 'S' ? 'S' : 'N';

                strncpy(tabla_bim[ll].bim1_bm , estrucBIMA.bmbima01 , 192);
                strncpy(tabla_bim[ll].bim2_bm , estrucBIMA.bmbima02 , 192);
                ll++;
              }

              if (!strncmp(tabla, "NPTTBOP", 7))
              {
                 /* EXEC SQL FETCH :registro3
                    INTO :estrucTBOP; */ 

{
                 struct sqlexd sqlstm;
                 sqlstm.sqlvsn = 13;
                 sqlstm.arrsiz = 21;
                 sqlstm.sqladtp = &sqladt;
                 sqlstm.sqltdsp = &sqltds;
                 sqlstm.iters = (unsigned int  )1;
                 sqlstm.offset = (unsigned int  )574;
                 sqlstm.selerr = (unsigned short)1;
                 sqlstm.sqlpfmem = (unsigned int  )0;
                 sqlstm.cud = sqlcud0;
                 sqlstm.sqlest = (unsigned char  *)&sqlca;
                 sqlstm.sqlety = (unsigned short)4352;
                 sqlstm.occurs = (unsigned int  )0;
                 sqlstm.sqfoff = (         int )0;
                 sqlstm.sqfmod = (unsigned int )2;
                 sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[0] = (         int  )0;
                 sqlstm.sqindv[0] = (         short *)0;
                 sqlstm.sqinds[0] = (         int  )0;
                 sqlstm.sqharm[0] = (unsigned long )0;
                 sqlstm.sqadto[0] = (unsigned short )0;
                 sqlstm.sqtdso[0] = (unsigned short )0;
                 sqlstm.sqhstv[1] = (unsigned char  *)estrucTBOP.tbtabla0;
                 sqlstm.sqhstl[1] = (unsigned long )3;
                 sqlstm.sqhsts[1] = (         int  )0;
                 sqlstm.sqindv[1] = (         short *)0;
                 sqlstm.sqinds[1] = (         int  )0;
                 sqlstm.sqharm[1] = (unsigned long )0;
                 sqlstm.sqadto[1] = (unsigned short )0;
                 sqlstm.sqtdso[1] = (unsigned short )0;
                 sqlstm.sqhstv[2] = (unsigned char  *)estrucTBOP.tbclave0;
                 sqlstm.sqhstl[2] = (unsigned long )13;
                 sqlstm.sqhsts[2] = (         int  )0;
                 sqlstm.sqindv[2] = (         short *)0;
                 sqlstm.sqinds[2] = (         int  )0;
                 sqlstm.sqharm[2] = (unsigned long )0;
                 sqlstm.sqadto[2] = (unsigned short )0;
                 sqlstm.sqtdso[2] = (unsigned short )0;
                 sqlstm.sqhstv[3] = (unsigned char  *)estrucTBOP.tbfuncis;
                 sqlstm.sqhstl[3] = (unsigned long )21;
                 sqlstm.sqhsts[3] = (         int  )0;
                 sqlstm.sqindv[3] = (         short *)0;
                 sqlstm.sqinds[3] = (         int  )0;
                 sqlstm.sqharm[3] = (unsigned long )0;
                 sqlstm.sqadto[3] = (unsigned short )0;
                 sqlstm.sqtdso[3] = (unsigned short )0;
                 sqlstm.sqhstv[4] = (unsigned char  *)estrucTBOP.tbcodent;
                 sqlstm.sqhstl[4] = (unsigned long )5;
                 sqlstm.sqhsts[4] = (         int  )0;
                 sqlstm.sqindv[4] = (         short *)0;
                 sqlstm.sqinds[4] = (         int  )0;
                 sqlstm.sqharm[4] = (unsigned long )0;
                 sqlstm.sqadto[4] = (unsigned short )0;
                 sqlstm.sqtdso[4] = (unsigned short )0;
                 sqlstm.sqphsv = sqlstm.sqhstv;
                 sqlstm.sqphsl = sqlstm.sqhstl;
                 sqlstm.sqphss = sqlstm.sqhsts;
                 sqlstm.sqpind = sqlstm.sqindv;
                 sqlstm.sqpins = sqlstm.sqinds;
                 sqlstm.sqparm = sqlstm.sqharm;
                 sqlstm.sqparc = sqlstm.sqharc;
                 sqlstm.sqpadto = sqlstm.sqadto;
                 sqlstm.sqptdso = sqlstm.sqtdso;
                 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                 if (sqlca.sqlcode == 1403) break;
}



                 ttbop *p = (ttbop *)malloc(sizeof(ttbop));
                 if (!p)   /*** insuficiente memoria   ***/
                 {
                    printf( "Memoria insuficiente en nuevo_tb\n");
                    //EXEC SQL ROLLBACK RELEASE;
					//return (101);
					return(salir_rb(abroConexion, 101, tabla, opcion));
					
                 }
                 ptb=p;
                 memset(ptb->clave, '\0', sizeof(ptb->clave));
                 strncpy(ptb->clave, estrucTBOP.tbcodent,4);
                 strncat(ptb->clave, estrucTBOP.tbtabla0,2);
                 strncat(ptb->clave, estrucTBOP.tbclave0,12);
                 strncpy(ptb->funcion, estrucTBOP.tbfuncis,20);
                 ptb->siguiente = ptb_primero;
                 ptb_primero = ptb;
              }
           }//for
           /* EXEC SQL CLOSE :registro3; */ 

{
           struct sqlexd sqlstm;
           sqlstm.sqlvsn = 13;
           sqlstm.arrsiz = 21;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.iters = (unsigned int  )1;
           sqlstm.offset = (unsigned int  )609;
           sqlstm.cud = sqlcud0;
           sqlstm.sqlest = (unsigned char  *)&sqlca;
           sqlstm.sqlety = (unsigned short)4352;
           sqlstm.occurs = (unsigned int  )0;
           sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
           sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
           sqlstm.sqhsts[0] = (         int  )0;
           sqlstm.sqindv[0] = (         short *)0;
           sqlstm.sqinds[0] = (         int  )0;
           sqlstm.sqharm[0] = (unsigned long )0;
           sqlstm.sqadto[0] = (unsigned short )0;
           sqlstm.sqtdso[0] = (unsigned short )0;
           sqlstm.sqphsv = sqlstm.sqhstv;
           sqlstm.sqphsl = sqlstm.sqhstl;
           sqlstm.sqphss = sqlstm.sqhsts;
           sqlstm.sqpind = sqlstm.sqindv;
           sqlstm.sqpins = sqlstm.sqinds;
           sqlstm.sqparm = sqlstm.sqharm;
           sqlstm.sqparc = sqlstm.sqharc;
           sqlstm.sqpadto = sqlstm.sqadto;
           sqlstm.sqptdso = sqlstm.sqtdso;
           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
           /* EXEC SQL FREE :registro3; */ 

{
           struct sqlexd sqlstm;
           sqlstm.sqlvsn = 13;
           sqlstm.arrsiz = 21;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.stmt = "";
           sqlstm.iters = (unsigned int  )1;
           sqlstm.offset = (unsigned int  )628;
           sqlstm.cud = sqlcud0;
           sqlstm.sqlest = (unsigned char  *)&sqlca;
           sqlstm.sqlety = (unsigned short)4352;
           sqlstm.occurs = (unsigned int  )0;
           sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
           sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
           sqlstm.sqhsts[0] = (         int  )0;
           sqlstm.sqindv[0] = (         short *)0;
           sqlstm.sqinds[0] = (         int  )0;
           sqlstm.sqharm[0] = (unsigned long )0;
           sqlstm.sqadto[0] = (unsigned short )0;
           sqlstm.sqtdso[0] = (unsigned short )0;
           sqlstm.sqphsv = sqlstm.sqhstv;
           sqlstm.sqphsl = sqlstm.sqhstl;
           sqlstm.sqphss = sqlstm.sqhsts;
           sqlstm.sqpind = sqlstm.sqindv;
           sqlstm.sqpins = sqlstm.sqinds;
           sqlstm.sqparm = sqlstm.sqharm;
           sqlstm.sqparc = sqlstm.sqharc;
           sqlstm.sqpadto = sqlstm.sqadto;
           sqlstm.sqptdso = sqlstm.sqtdso;
           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
           
           if (!strncmp(tabla, "NPTBIMA", 7) )
             registro=(char*)tabla_bim;
           if (!strncmp(tabla, "NPTDAP2", 7) )
              registro=(char*)tabla_apl;
           if (!strncmp(tabla, "NPTDISO", 7) )
             registro=(char*)tabla_iso;
           if (!strncmp(tabla, "NPTTBOP", 7) )
             registro=(char*)ptb_primero;
		   if (!strncmp(tabla, "NPTTRMS", 7) )
               registro=(char*)tabla_trms;
		   if (!strncmp(tabla, "NPTIPCS", 7) )
               registro=(char*)tabla_ipcs;
		   if (!strncmp(tabla, "NPTCONF", 7) )
               registro=(char*)tabla_conf;
		   if (!strncmp(tabla, "NPTPARA", 7) )
               registro=(char*)tabla_para;

        }//if no error
        else
        {
           printf("se sale por el else de cargar\n");
           //EXEC SQL ROLLBACK RELEASE;
		   //return (101);
		   return(salir_rb(abroConexion, 101, tabla, opcion));
        } 
   }
   else
   {
// * añadido para PRICE*//
   // printf (" else if (opcion == CARGAR ) \n");
    if (((!strncmp(tabla, "NPTPARS", 7))  && (strncmp(opcionT,"UPDATE",6) && strncmp(opcionT,"FECMODIF",8)
                                          &&  strncmp(opcionT,"MODIFESTADO",11) && strncmp(opcionT, "SEMAF", 5) 
                                          &&  strncmp(opcionT, "SESION", 6) && strncmp(opcionT, "PACONCI", 7))) ||
        ((!strncmp(tabla, "NPTCASA", 7)) && (!strncmp(opcionT, "IGUAL",5))) ||
        ((!strncmp(tabla, "NPTDIFE", 7)) && (strncmp(opcionT, "BORRAR",5))) ||
        ((!strncmp(tabla, "PREVIAA", 7)) && (!strncmp(opcionT, "IGUAL",5))) ||
        ((!strncmp(tabla, "NPTIDEN", 7)) && (!strncmp(opcionT, "IGUAL",5))) //||
	//	((!strncmp(tabla, "NPTPARA", 7)) && (!strncmp(opcionT, "IGUAL",5)))    //añadido para arranque por bbdd
       )  //cursores
     {
         fprintf(stdout,"cursores llamadaProc2 claveT>%s< tabla %s, opcion %s\n", claveT, (char*)tabla, (char*)opcionT);
         fflush(stdout);

          contador=0;
        //  printf ("antes del allocate\n");
          /* EXEC SQL ALLOCATE :registro3; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 13;
          sqlstm.arrsiz = 21;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.stmt = "";
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )647;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
          sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
          sqlstm.sqhsts[0] = (         int  )0;
          sqlstm.sqindv[0] = (         short *)0;
          sqlstm.sqinds[0] = (         int  )0;
          sqlstm.sqharm[0] = (unsigned long )0;
          sqlstm.sqadto[0] = (unsigned short )0;
          sqlstm.sqtdso[0] = (unsigned short )0;
          sqlstm.sqphsv = sqlstm.sqhstv;
          sqlstm.sqphsl = sqlstm.sqhstl;
          sqlstm.sqphss = sqlstm.sqhsts;
          sqlstm.sqpind = sqlstm.sqindv;
          sqlstm.sqpins = sqlstm.sqinds;
          sqlstm.sqparm = sqlstm.sqharm;
          sqlstm.sqparc = sqlstm.sqharc;
          sqlstm.sqpadto = sqlstm.sqadto;
          sqlstm.sqptdso = sqlstm.sqtdso;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


       //   printf ("despues del allocate\n");
       //   printf ("antes del call llamadaProc2\n");
          /* EXEC SQL CALL llamadaProc2 (:opcionT, :tabla, :claveT, :registro3, :CodError); */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 13;
          sqlstm.arrsiz = 21;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.stmt = "call llamadaProc2(:b0,:b1,:b2,:b3,:b4)";
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )666;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlstm.sqhstv[0] = (unsigned char  *)opcionT;
          sqlstm.sqhstl[0] = (unsigned long )20;
          sqlstm.sqhsts[0] = (         int  )0;
          sqlstm.sqindv[0] = (         short *)0;
          sqlstm.sqinds[0] = (         int  )0;
          sqlstm.sqharm[0] = (unsigned long )0;
          sqlstm.sqadto[0] = (unsigned short )0;
          sqlstm.sqtdso[0] = (unsigned short )0;
          sqlstm.sqhstv[1] = (unsigned char  *)tabla;
          sqlstm.sqhstl[1] = (unsigned long )8;
          sqlstm.sqhsts[1] = (         int  )0;
          sqlstm.sqindv[1] = (         short *)0;
          sqlstm.sqinds[1] = (         int  )0;
          sqlstm.sqharm[1] = (unsigned long )0;
          sqlstm.sqadto[1] = (unsigned short )0;
          sqlstm.sqtdso[1] = (unsigned short )0;
          sqlstm.sqhstv[2] = (unsigned char  *)claveT;
          sqlstm.sqhstl[2] = (unsigned long )200;
          sqlstm.sqhsts[2] = (         int  )0;
          sqlstm.sqindv[2] = (         short *)0;
          sqlstm.sqinds[2] = (         int  )0;
          sqlstm.sqharm[2] = (unsigned long )0;
          sqlstm.sqadto[2] = (unsigned short )0;
          sqlstm.sqtdso[2] = (unsigned short )0;
          sqlstm.sqhstv[3] = (unsigned char  *)&registro3;
          sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
          sqlstm.sqhsts[3] = (         int  )0;
          sqlstm.sqindv[3] = (         short *)0;
          sqlstm.sqinds[3] = (         int  )0;
          sqlstm.sqharm[3] = (unsigned long )0;
          sqlstm.sqadto[3] = (unsigned short )0;
          sqlstm.sqtdso[3] = (unsigned short )0;
          sqlstm.sqhstv[4] = (unsigned char  *)CodError;
          sqlstm.sqhstl[4] = (unsigned long )4;
          sqlstm.sqhsts[4] = (         int  )0;
          sqlstm.sqindv[4] = (         short *)0;
          sqlstm.sqinds[4] = (         int  )0;
          sqlstm.sqharm[4] = (unsigned long )0;
          sqlstm.sqadto[4] = (unsigned short )0;
          sqlstm.sqtdso[4] = (unsigned short )0;
          sqlstm.sqphsv = sqlstm.sqhstv;
          sqlstm.sqphsl = sqlstm.sqhstl;
          sqlstm.sqphss = sqlstm.sqhsts;
          sqlstm.sqpind = sqlstm.sqindv;
          sqlstm.sqpins = sqlstm.sqinds;
          sqlstm.sqparm = sqlstm.sqharm;
          sqlstm.sqparc = sqlstm.sqharc;
          sqlstm.sqpadto = sqlstm.sqadto;
          sqlstm.sqptdso = sqlstm.sqtdso;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
       //   printf ("despues del call llamadaProc2\n");
          /* EXEC SQL WHENEVER NOT FOUND DO break; */ 
 
  
         ++registro3.curocn;
         for (;;){
           //  printf("dentro del for \n");
           //  printf("contador dentro del for%i\n", contador);
           //  printf("dentro del for claveT>%s< tabla %s, opcion %s\n", claveT, (char*)tabla, (char*)opcionT);
             fflush(stdout);
             
           if (!strncmp(tabla, "NPTIDEN", 7))
           {
             /* EXEC SQL FETCH :registro3
             INTO :estrucIDEN; */ 

{
             struct sqlexd sqlstm;
             sqlstm.sqlvsn = 13;
             sqlstm.arrsiz = 21;
             sqlstm.sqladtp = &sqladt;
             sqlstm.sqltdsp = &sqltds;
             sqlstm.iters = (unsigned int  )1;
             sqlstm.offset = (unsigned int  )701;
             sqlstm.selerr = (unsigned short)1;
             sqlstm.sqlpfmem = (unsigned int  )0;
             sqlstm.cud = sqlcud0;
             sqlstm.sqlest = (unsigned char  *)&sqlca;
             sqlstm.sqlety = (unsigned short)4352;
             sqlstm.occurs = (unsigned int  )0;
             sqlstm.sqfoff = (         int )0;
             sqlstm.sqfmod = (unsigned int )2;
             sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
             sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
             sqlstm.sqhsts[0] = (         int  )0;
             sqlstm.sqindv[0] = (         short *)0;
             sqlstm.sqinds[0] = (         int  )0;
             sqlstm.sqharm[0] = (unsigned long )0;
             sqlstm.sqadto[0] = (unsigned short )0;
             sqlstm.sqtdso[0] = (unsigned short )0;
             sqlstm.sqhstv[1] = (unsigned char  *)estrucIDEN.idsubsis;
             sqlstm.sqhstl[1] = (unsigned long )3;
             sqlstm.sqhsts[1] = (         int  )0;
             sqlstm.sqindv[1] = (         short *)0;
             sqlstm.sqinds[1] = (         int  )0;
             sqlstm.sqharm[1] = (unsigned long )0;
             sqlstm.sqadto[1] = (unsigned short )0;
             sqlstm.sqtdso[1] = (unsigned short )0;
             sqlstm.sqhstv[2] = (unsigned char  *)estrucIDEN.idsubapl;
             sqlstm.sqhstl[2] = (unsigned long )3;
             sqlstm.sqhsts[2] = (         int  )0;
             sqlstm.sqindv[2] = (         short *)0;
             sqlstm.sqinds[2] = (         int  )0;
             sqlstm.sqharm[2] = (unsigned long )0;
             sqlstm.sqadto[2] = (unsigned short )0;
             sqlstm.sqtdso[2] = (unsigned short )0;
             sqlstm.sqhstv[3] = (unsigned char  *)estrucIDEN.idcomando;
             sqlstm.sqhstl[3] = (unsigned long )51;
             sqlstm.sqhsts[3] = (         int  )0;
             sqlstm.sqindv[3] = (         short *)0;
             sqlstm.sqinds[3] = (         int  )0;
             sqlstm.sqharm[3] = (unsigned long )0;
             sqlstm.sqadto[3] = (unsigned short )0;
             sqlstm.sqtdso[3] = (unsigned short )0;
             sqlstm.sqhstv[4] = (unsigned char  *)estrucIDEN.idclave1;
             sqlstm.sqhstl[4] = (unsigned long )151;
             sqlstm.sqhsts[4] = (         int  )0;
             sqlstm.sqindv[4] = (         short *)0;
             sqlstm.sqinds[4] = (         int  )0;
             sqlstm.sqharm[4] = (unsigned long )0;
             sqlstm.sqadto[4] = (unsigned short )0;
             sqlstm.sqtdso[4] = (unsigned short )0;
             sqlstm.sqhstv[5] = (unsigned char  *)estrucIDEN.idclave2;
             sqlstm.sqhstl[5] = (unsigned long )151;
             sqlstm.sqhsts[5] = (         int  )0;
             sqlstm.sqindv[5] = (         short *)0;
             sqlstm.sqinds[5] = (         int  )0;
             sqlstm.sqharm[5] = (unsigned long )0;
             sqlstm.sqadto[5] = (unsigned short )0;
             sqlstm.sqtdso[5] = (unsigned short )0;
             sqlstm.sqphsv = sqlstm.sqhstv;
             sqlstm.sqphsl = sqlstm.sqhstl;
             sqlstm.sqphss = sqlstm.sqhsts;
             sqlstm.sqpind = sqlstm.sqindv;
             sqlstm.sqpins = sqlstm.sqinds;
             sqlstm.sqparm = sqlstm.sqharm;
             sqlstm.sqparc = sqlstm.sqharc;
             sqlstm.sqpadto = sqlstm.sqadto;
             sqlstm.sqptdso = sqlstm.sqtdso;
             sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
             if (sqlca.sqlcode == 1403) break;
}

 
           //  printf("estructura estrucIDEN %s\n", estrucIDEN);
           }
//* añadido para PRICE*//
           else
             if (!strncmp(tabla, "NPTPARS", 7))
             {
                /* EXEC SQL FETCH :registro3
                INTO :estrucPARS; */ 

{
                struct sqlexd sqlstm;
                sqlstm.sqlvsn = 13;
                sqlstm.arrsiz = 22;
                sqlstm.sqladtp = &sqladt;
                sqlstm.sqltdsp = &sqltds;
                sqlstm.iters = (unsigned int  )1;
                sqlstm.offset = (unsigned int  )740;
                sqlstm.selerr = (unsigned short)1;
                sqlstm.sqlpfmem = (unsigned int  )0;
                sqlstm.cud = sqlcud0;
                sqlstm.sqlest = (unsigned char  *)&sqlca;
                sqlstm.sqlety = (unsigned short)4352;
                sqlstm.occurs = (unsigned int  )0;
                sqlstm.sqfoff = (         int )0;
                sqlstm.sqfmod = (unsigned int )2;
                sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                sqlstm.sqhsts[0] = (         int  )0;
                sqlstm.sqindv[0] = (         short *)0;
                sqlstm.sqinds[0] = (         int  )0;
                sqlstm.sqharm[0] = (unsigned long )0;
                sqlstm.sqadto[0] = (unsigned short )0;
                sqlstm.sqtdso[0] = (unsigned short )0;
                sqlstm.sqhstv[1] = (unsigned char  *)estrucPARS.pasubsis;
                sqlstm.sqhstl[1] = (unsigned long )3;
                sqlstm.sqhsts[1] = (         int  )0;
                sqlstm.sqindv[1] = (         short *)0;
                sqlstm.sqinds[1] = (         int  )0;
                sqlstm.sqharm[1] = (unsigned long )0;
                sqlstm.sqadto[1] = (unsigned short )0;
                sqlstm.sqtdso[1] = (unsigned short )0;
                sqlstm.sqhstv[2] = (unsigned char  *)estrucPARS.pasubapl;
                sqlstm.sqhstl[2] = (unsigned long )3;
                sqlstm.sqhsts[2] = (         int  )0;
                sqlstm.sqindv[2] = (         short *)0;
                sqlstm.sqinds[2] = (         int  )0;
                sqlstm.sqharm[2] = (unsigned long )0;
                sqlstm.sqadto[2] = (unsigned short )0;
                sqlstm.sqtdso[2] = (unsigned short )0;
                sqlstm.sqhstv[3] = (unsigned char  *)estrucPARS.panomsub;
                sqlstm.sqhstl[3] = (unsigned long )21;
                sqlstm.sqhsts[3] = (         int  )0;
                sqlstm.sqindv[3] = (         short *)0;
                sqlstm.sqinds[3] = (         int  )0;
                sqlstm.sqharm[3] = (unsigned long )0;
                sqlstm.sqadto[3] = (unsigned short )0;
                sqlstm.sqtdso[3] = (unsigned short )0;
                sqlstm.sqhstv[4] = (unsigned char  *)estrucPARS.pasemaf1;
                sqlstm.sqhstl[4] = (unsigned long )2;
                sqlstm.sqhsts[4] = (         int  )0;
                sqlstm.sqindv[4] = (         short *)0;
                sqlstm.sqinds[4] = (         int  )0;
                sqlstm.sqharm[4] = (unsigned long )0;
                sqlstm.sqadto[4] = (unsigned short )0;
                sqlstm.sqtdso[4] = (unsigned short )0;
                sqlstm.sqhstv[5] = (unsigned char  *)estrucPARS.pasemaf2;
                sqlstm.sqhstl[5] = (unsigned long )2;
                sqlstm.sqhsts[5] = (         int  )0;
                sqlstm.sqindv[5] = (         short *)0;
                sqlstm.sqinds[5] = (         int  )0;
                sqlstm.sqharm[5] = (unsigned long )0;
                sqlstm.sqadto[5] = (unsigned short )0;
                sqlstm.sqtdso[5] = (unsigned short )0;
                sqlstm.sqhstv[6] = (unsigned char  *)estrucPARS.paestado;
                sqlstm.sqhstl[6] = (unsigned long )3;
                sqlstm.sqhsts[6] = (         int  )0;
                sqlstm.sqindv[6] = (         short *)0;
                sqlstm.sqinds[6] = (         int  )0;
                sqlstm.sqharm[6] = (unsigned long )0;
                sqlstm.sqadto[6] = (unsigned short )0;
                sqlstm.sqtdso[6] = (unsigned short )0;
                sqlstm.sqhstv[7] = (unsigned char  *)estrucPARS.patrepet;
                sqlstm.sqhstl[7] = (unsigned long )7;
                sqlstm.sqhsts[7] = (         int  )0;
                sqlstm.sqindv[7] = (         short *)0;
                sqlstm.sqinds[7] = (         int  )0;
                sqlstm.sqharm[7] = (unsigned long )0;
                sqlstm.sqadto[7] = (unsigned short )0;
                sqlstm.sqtdso[7] = (unsigned short )0;
                sqlstm.sqhstv[8] = (unsigned char  *)estrucPARS.patrerep;
                sqlstm.sqhstl[8] = (unsigned long )7;
                sqlstm.sqhsts[8] = (         int  )0;
                sqlstm.sqindv[8] = (         short *)0;
                sqlstm.sqinds[8] = (         int  )0;
                sqlstm.sqharm[8] = (unsigned long )0;
                sqlstm.sqadto[8] = (unsigned short )0;
                sqlstm.sqtdso[8] = (unsigned short )0;
                sqlstm.sqhstv[9] = (unsigned char  *)estrucPARS.patresnf;
                sqlstm.sqhstl[9] = (unsigned long )7;
                sqlstm.sqhsts[9] = (         int  )0;
                sqlstm.sqindv[9] = (         short *)0;
                sqlstm.sqinds[9] = (         int  )0;
                sqlstm.sqharm[9] = (unsigned long )0;
                sqlstm.sqadto[9] = (unsigned short )0;
                sqlstm.sqtdso[9] = (unsigned short )0;
                sqlstm.sqhstv[10] = (unsigned char  *)estrucPARS.panumrea;
                sqlstm.sqhstl[10] = (unsigned long )2;
                sqlstm.sqhsts[10] = (         int  )0;
                sqlstm.sqindv[10] = (         short *)0;
                sqlstm.sqinds[10] = (         int  )0;
                sqlstm.sqharm[10] = (unsigned long )0;
                sqlstm.sqadto[10] = (unsigned short )0;
                sqlstm.sqtdso[10] = (unsigned short )0;
                sqlstm.sqhstv[11] = (unsigned char  *)estrucPARS.pafecses;
                sqlstm.sqhstl[11] = (unsigned long )7;
                sqlstm.sqhsts[11] = (         int  )0;
                sqlstm.sqindv[11] = (         short *)0;
                sqlstm.sqinds[11] = (         int  )0;
                sqlstm.sqharm[11] = (unsigned long )0;
                sqlstm.sqadto[11] = (unsigned short )0;
                sqlstm.sqtdso[11] = (unsigned short )0;
                sqlstm.sqhstv[12] = (unsigned char  *)estrucPARS.panumses;
                sqlstm.sqhstl[12] = (unsigned long )4;
                sqlstm.sqhsts[12] = (         int  )0;
                sqlstm.sqindv[12] = (         short *)0;
                sqlstm.sqinds[12] = (         int  )0;
                sqlstm.sqharm[12] = (unsigned long )0;
                sqlstm.sqadto[12] = (unsigned short )0;
                sqlstm.sqtdso[12] = (unsigned short )0;
                sqlstm.sqhstv[13] = (unsigned char  *)estrucPARS.paconcil;
                sqlstm.sqhstl[13] = (unsigned long )2;
                sqlstm.sqhsts[13] = (         int  )0;
                sqlstm.sqindv[13] = (         short *)0;
                sqlstm.sqinds[13] = (         int  )0;
                sqlstm.sqharm[13] = (unsigned long )0;
                sqlstm.sqadto[13] = (unsigned short )0;
                sqlstm.sqtdso[13] = (unsigned short )0;
                sqlstm.sqhstv[14] = (unsigned char  *)estrucPARS.panip;
                sqlstm.sqhstl[14] = (unsigned long )16;
                sqlstm.sqhsts[14] = (         int  )0;
                sqlstm.sqindv[14] = (         short *)0;
                sqlstm.sqinds[14] = (         int  )0;
                sqlstm.sqharm[14] = (unsigned long )0;
                sqlstm.sqadto[14] = (unsigned short )0;
                sqlstm.sqtdso[14] = (unsigned short )0;
                sqlstm.sqhstv[15] = (unsigned char  *)estrucPARS.paportnume;
                sqlstm.sqhstl[15] = (unsigned long )6;
                sqlstm.sqhsts[15] = (         int  )0;
                sqlstm.sqindv[15] = (         short *)0;
                sqlstm.sqinds[15] = (         int  )0;
                sqlstm.sqharm[15] = (unsigned long )0;
                sqlstm.sqadto[15] = (unsigned short )0;
                sqlstm.sqtdso[15] = (unsigned short )0;
                sqlstm.sqhstv[16] = (unsigned char  *)estrucPARS.pacidesti;
                sqlstm.sqhstl[16] = (unsigned long )14;
                sqlstm.sqhsts[16] = (         int  )0;
                sqlstm.sqindv[16] = (         short *)0;
                sqlstm.sqinds[16] = (         int  )0;
                sqlstm.sqharm[16] = (unsigned long )0;
                sqlstm.sqadto[16] = (unsigned short )0;
                sqlstm.sqtdso[16] = (unsigned short )0;
                sqlstm.sqhstv[17] = (unsigned char  *)estrucPARS.paciadqui;
                sqlstm.sqhstl[17] = (unsigned long )14;
                sqlstm.sqhsts[17] = (         int  )0;
                sqlstm.sqindv[17] = (         short *)0;
                sqlstm.sqinds[17] = (         int  )0;
                sqlstm.sqharm[17] = (unsigned long )0;
                sqlstm.sqadto[17] = (unsigned short )0;
                sqlstm.sqtdso[17] = (unsigned short )0;
                sqlstm.sqhstv[18] = (unsigned char  *)estrucPARS.paenvoff;
                sqlstm.sqhstl[18] = (unsigned long )2;
                sqlstm.sqhsts[18] = (         int  )0;
                sqlstm.sqindv[18] = (         short *)0;
                sqlstm.sqinds[18] = (         int  )0;
                sqlstm.sqharm[18] = (unsigned long )0;
                sqlstm.sqadto[18] = (unsigned short )0;
                sqlstm.sqtdso[18] = (unsigned short )0;
                sqlstm.sqhstv[19] = (unsigned char  *)estrucPARS.paenvtst;
                sqlstm.sqhstl[19] = (unsigned long )2;
                sqlstm.sqhsts[19] = (         int  )0;
                sqlstm.sqindv[19] = (         short *)0;
                sqlstm.sqinds[19] = (         int  )0;
                sqlstm.sqharm[19] = (unsigned long )0;
                sqlstm.sqadto[19] = (unsigned short )0;
                sqlstm.sqtdso[19] = (unsigned short )0;
                sqlstm.sqhstv[20] = (unsigned char  *)estrucPARS.patisub;
                sqlstm.sqhstl[20] = (unsigned long )2;
                sqlstm.sqhsts[20] = (         int  )0;
                sqlstm.sqindv[20] = (         short *)0;
                sqlstm.sqinds[20] = (         int  )0;
                sqlstm.sqharm[20] = (unsigned long )0;
                sqlstm.sqadto[20] = (unsigned short )0;
                sqlstm.sqtdso[20] = (unsigned short )0;
                sqlstm.sqhstv[21] = (unsigned char  *)estrucPARS.paidred;
                sqlstm.sqhstl[21] = (unsigned long )5;
                sqlstm.sqhsts[21] = (         int  )0;
                sqlstm.sqindv[21] = (         short *)0;
                sqlstm.sqinds[21] = (         int  )0;
                sqlstm.sqharm[21] = (unsigned long )0;
                sqlstm.sqadto[21] = (unsigned short )0;
                sqlstm.sqtdso[21] = (unsigned short )0;
                sqlstm.sqphsv = sqlstm.sqhstv;
                sqlstm.sqphsl = sqlstm.sqhstl;
                sqlstm.sqphss = sqlstm.sqhsts;
                sqlstm.sqpind = sqlstm.sqindv;
                sqlstm.sqpins = sqlstm.sqinds;
                sqlstm.sqparm = sqlstm.sqharm;
                sqlstm.sqparc = sqlstm.sqharc;
                sqlstm.sqpadto = sqlstm.sqadto;
                sqlstm.sqptdso = sqlstm.sqtdso;
                sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                if (sqlca.sqlcode == 1403) break;
}

 
            //    printf("estructura estrucPARS %s\n", estrucPARS);
             }
             else
               if (!strncmp(tabla, "NPTCASA", 7))
               {
                 /* EXEC SQL FETCH :registro3
                 INTO :estrucCASA; */ 

{
                 struct sqlexd sqlstm;
                 sqlstm.sqlvsn = 13;
                 sqlstm.arrsiz = 22;
                 sqlstm.sqladtp = &sqladt;
                 sqlstm.sqltdsp = &sqltds;
                 sqlstm.iters = (unsigned int  )1;
                 sqlstm.offset = (unsigned int  )843;
                 sqlstm.selerr = (unsigned short)1;
                 sqlstm.sqlpfmem = (unsigned int  )0;
                 sqlstm.cud = sqlcud0;
                 sqlstm.sqlest = (unsigned char  *)&sqlca;
                 sqlstm.sqlety = (unsigned short)4352;
                 sqlstm.occurs = (unsigned int  )0;
                 sqlstm.sqfoff = (         int )0;
                 sqlstm.sqfmod = (unsigned int )2;
                 sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                 sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                 sqlstm.sqhsts[0] = (         int  )0;
                 sqlstm.sqindv[0] = (         short *)0;
                 sqlstm.sqinds[0] = (         int  )0;
                 sqlstm.sqharm[0] = (unsigned long )0;
                 sqlstm.sqadto[0] = (unsigned short )0;
                 sqlstm.sqtdso[0] = (unsigned short )0;
                 sqlstm.sqhstv[1] = (unsigned char  *)estrucCASA.canumope;
                 sqlstm.sqhstl[1] = (unsigned long )7;
                 sqlstm.sqhsts[1] = (         int  )0;
                 sqlstm.sqindv[1] = (         short *)0;
                 sqlstm.sqinds[1] = (         int  )0;
                 sqlstm.sqharm[1] = (unsigned long )0;
                 sqlstm.sqadto[1] = (unsigned short )0;
                 sqlstm.sqtdso[1] = (unsigned short )0;
                 sqlstm.sqhstv[2] = (unsigned char  *)estrucCASA.catieabs;
                 sqlstm.sqhstl[2] = (unsigned long )11;
                 sqlstm.sqhsts[2] = (         int  )0;
                 sqlstm.sqindv[2] = (         short *)0;
                 sqlstm.sqinds[2] = (         int  )0;
                 sqlstm.sqharm[2] = (unsigned long )0;
                 sqlstm.sqadto[2] = (unsigned short )0;
                 sqlstm.sqtdso[2] = (unsigned short )0;
                 sqlstm.sqhstv[3] = (unsigned char  *)estrucCASA.catieesp;
                 sqlstm.sqhstl[3] = (unsigned long )11;
                 sqlstm.sqhsts[3] = (         int  )0;
                 sqlstm.sqindv[3] = (         short *)0;
                 sqlstm.sqinds[3] = (         int  )0;
                 sqlstm.sqharm[3] = (unsigned long )0;
                 sqlstm.sqadto[3] = (unsigned short )0;
                 sqlstm.sqtdso[3] = (unsigned short )0;
                 sqlstm.sqhstv[4] = (unsigned char  *)estrucCASA.caforitn;
                 sqlstm.sqhstl[4] = (unsigned long )4000;
                 sqlstm.sqhsts[4] = (         int  )0;
                 sqlstm.sqindv[4] = (         short *)0;
                 sqlstm.sqinds[4] = (         int  )0;
                 sqlstm.sqharm[4] = (unsigned long )0;
                 sqlstm.sqadto[4] = (unsigned short )0;
                 sqlstm.sqtdso[4] = (unsigned short )0;
                 sqlstm.sqphsv = sqlstm.sqhstv;
                 sqlstm.sqphsl = sqlstm.sqhstl;
                 sqlstm.sqphss = sqlstm.sqhsts;
                 sqlstm.sqpind = sqlstm.sqindv;
                 sqlstm.sqpins = sqlstm.sqinds;
                 sqlstm.sqparm = sqlstm.sqharm;
                 sqlstm.sqparc = sqlstm.sqharc;
                 sqlstm.sqpadto = sqlstm.sqadto;
                 sqlstm.sqptdso = sqlstm.sqtdso;
                 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                 if (sqlca.sqlcode == 1403) break;
}

 
            //     printf("estructura estrucCASA %s\n", estrucCASA);
               }
               else
                 if (!strncmp(tabla, "NPTDIFE", 7))
                 {
                    /* EXEC SQL FETCH :registro3
                    INTO :estrucDIFE; */ 

{
                    struct sqlexd sqlstm;
                    sqlstm.sqlvsn = 13;
                    sqlstm.arrsiz = 22;
                    sqlstm.sqladtp = &sqladt;
                    sqlstm.sqltdsp = &sqltds;
                    sqlstm.iters = (unsigned int  )1;
                    sqlstm.offset = (unsigned int  )878;
                    sqlstm.selerr = (unsigned short)1;
                    sqlstm.sqlpfmem = (unsigned int  )0;
                    sqlstm.cud = sqlcud0;
                    sqlstm.sqlest = (unsigned char  *)&sqlca;
                    sqlstm.sqlety = (unsigned short)4352;
                    sqlstm.occurs = (unsigned int  )0;
                    sqlstm.sqfoff = (         int )0;
                    sqlstm.sqfmod = (unsigned int )2;
                    sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                    sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                    sqlstm.sqhsts[0] = (         int  )0;
                    sqlstm.sqindv[0] = (         short *)0;
                    sqlstm.sqinds[0] = (         int  )0;
                    sqlstm.sqharm[0] = (unsigned long )0;
                    sqlstm.sqadto[0] = (unsigned short )0;
                    sqlstm.sqtdso[0] = (unsigned short )0;
                    sqlstm.sqhstv[1] = (unsigned char  *)estrucDIFE.disubsis;
                    sqlstm.sqhstl[1] = (unsigned long )3;
                    sqlstm.sqhsts[1] = (         int  )0;
                    sqlstm.sqindv[1] = (         short *)0;
                    sqlstm.sqinds[1] = (         int  )0;
                    sqlstm.sqharm[1] = (unsigned long )0;
                    sqlstm.sqadto[1] = (unsigned short )0;
                    sqlstm.sqtdso[1] = (unsigned short )0;
                    sqlstm.sqhstv[2] = (unsigned char  *)estrucDIFE.disubapl;
                    sqlstm.sqhstl[2] = (unsigned long )3;
                    sqlstm.sqhsts[2] = (         int  )0;
                    sqlstm.sqindv[2] = (         short *)0;
                    sqlstm.sqinds[2] = (         int  )0;
                    sqlstm.sqharm[2] = (unsigned long )0;
                    sqlstm.sqadto[2] = (unsigned short )0;
                    sqlstm.sqtdso[2] = (unsigned short )0;
                    sqlstm.sqhstv[3] = (unsigned char  *)estrucDIFE.difecha0;
                    sqlstm.sqhstl[3] = (unsigned long )9;
                    sqlstm.sqhsts[3] = (         int  )0;
                    sqlstm.sqindv[3] = (         short *)0;
                    sqlstm.sqinds[3] = (         int  )0;
                    sqlstm.sqharm[3] = (unsigned long )0;
                    sqlstm.sqadto[3] = (unsigned short )0;
                    sqlstm.sqtdso[3] = (unsigned short )0;
                    sqlstm.sqhstv[4] = (unsigned char  *)estrucDIFE.dihora00;
                    sqlstm.sqhstl[4] = (unsigned long )7;
                    sqlstm.sqhsts[4] = (         int  )0;
                    sqlstm.sqindv[4] = (         short *)0;
                    sqlstm.sqinds[4] = (         int  )0;
                    sqlstm.sqharm[4] = (unsigned long )0;
                    sqlstm.sqadto[4] = (unsigned short )0;
                    sqlstm.sqtdso[4] = (unsigned short )0;
                    sqlstm.sqhstv[5] = (unsigned char  *)estrucDIFE.dinumope;
                    sqlstm.sqhstl[5] = (unsigned long )7;
                    sqlstm.sqhsts[5] = (         int  )0;
                    sqlstm.sqindv[5] = (         short *)0;
                    sqlstm.sqinds[5] = (         int  )0;
                    sqlstm.sqharm[5] = (unsigned long )0;
                    sqlstm.sqadto[5] = (unsigned short )0;
                    sqlstm.sqtdso[5] = (unsigned short )0;
                    sqlstm.sqhstv[6] = (unsigned char  *)estrucDIFE.diforitn;
                    sqlstm.sqhstl[6] = (unsigned long )4000;
                    sqlstm.sqhsts[6] = (         int  )0;
                    sqlstm.sqindv[6] = (         short *)0;
                    sqlstm.sqinds[6] = (         int  )0;
                    sqlstm.sqharm[6] = (unsigned long )0;
                    sqlstm.sqadto[6] = (unsigned short )0;
                    sqlstm.sqtdso[6] = (unsigned short )0;
                    sqlstm.sqphsv = sqlstm.sqhstv;
                    sqlstm.sqphsl = sqlstm.sqhstl;
                    sqlstm.sqphss = sqlstm.sqhsts;
                    sqlstm.sqpind = sqlstm.sqindv;
                    sqlstm.sqpins = sqlstm.sqinds;
                    sqlstm.sqparm = sqlstm.sqharm;
                    sqlstm.sqparc = sqlstm.sqharc;
                    sqlstm.sqpadto = sqlstm.sqadto;
                    sqlstm.sqptdso = sqlstm.sqtdso;
                    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                    if (sqlca.sqlcode == 1403) break;
}

 
              //      printf("estructura estrucDIFE %s\n", estrucDIFE);
                 }
             //    else
	         //   	 if (!strncmp(tabla, "NPTPARA", 7))
             //        {
             //           EXEC SQL FETCH :registro3
             //           INTO :estrucPARA; 
             // //      printf("estructura estrucDIFE %s\n", estrucPARa);
             //        }
                     else 
				     {
                         /* EXEC SQL FETCH :registro3
                         INTO :estrucLOGO; */ 

{
                         struct sqlexd sqlstm;
                         sqlstm.sqlvsn = 13;
                         sqlstm.arrsiz = 22;
                         sqlstm.sqladtp = &sqladt;
                         sqlstm.sqltdsp = &sqltds;
                         sqlstm.iters = (unsigned int  )1;
                         sqlstm.offset = (unsigned int  )921;
                         sqlstm.selerr = (unsigned short)1;
                         sqlstm.sqlpfmem = (unsigned int  )0;
                         sqlstm.cud = sqlcud0;
                         sqlstm.sqlest = (unsigned char  *)&sqlca;
                         sqlstm.sqlety = (unsigned short)4352;
                         sqlstm.occurs = (unsigned int  )0;
                         sqlstm.sqfoff = (         int )0;
                         sqlstm.sqfmod = (unsigned int )2;
                         sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
                         sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
                         sqlstm.sqhsts[0] = (         int  )0;
                         sqlstm.sqindv[0] = (         short *)0;
                         sqlstm.sqinds[0] = (         int  )0;
                         sqlstm.sqharm[0] = (unsigned long )0;
                         sqlstm.sqadto[0] = (unsigned short )0;
                         sqlstm.sqtdso[0] = (unsigned short )0;
                         sqlstm.sqhstv[1] = (unsigned char  *)estrucLOGO.lonumope;
                         sqlstm.sqhstl[1] = (unsigned long )7;
                         sqlstm.sqhsts[1] = (         int  )0;
                         sqlstm.sqindv[1] = (         short *)0;
                         sqlstm.sqinds[1] = (         int  )0;
                         sqlstm.sqharm[1] = (unsigned long )0;
                         sqlstm.sqadto[1] = (unsigned short )0;
                         sqlstm.sqtdso[1] = (unsigned short )0;
                         sqlstm.sqhstv[2] = (unsigned char  *)estrucLOGO.lofecha0;
                         sqlstm.sqhstl[2] = (unsigned long )9;
                         sqlstm.sqhsts[2] = (         int  )0;
                         sqlstm.sqindv[2] = (         short *)0;
                         sqlstm.sqinds[2] = (         int  )0;
                         sqlstm.sqharm[2] = (unsigned long )0;
                         sqlstm.sqadto[2] = (unsigned short )0;
                         sqlstm.sqtdso[2] = (unsigned short )0;
                         sqlstm.sqhstv[3] = (unsigned char  *)estrucLOGO.lohora00;
                         sqlstm.sqhstl[3] = (unsigned long )7;
                         sqlstm.sqhsts[3] = (         int  )0;
                         sqlstm.sqindv[3] = (         short *)0;
                         sqlstm.sqinds[3] = (         int  )0;
                         sqlstm.sqharm[3] = (unsigned long )0;
                         sqlstm.sqadto[3] = (unsigned short )0;
                         sqlstm.sqtdso[3] = (unsigned short )0;
                         sqlstm.sqhstv[4] = (unsigned char  *)estrucLOGO.lonumpan;
                         sqlstm.sqhstl[4] = (unsigned long )20;
                         sqlstm.sqhsts[4] = (         int  )0;
                         sqlstm.sqindv[4] = (         short *)0;
                         sqlstm.sqinds[4] = (         int  )0;
                         sqlstm.sqharm[4] = (unsigned long )0;
                         sqlstm.sqadto[4] = (unsigned short )0;
                         sqlstm.sqtdso[4] = (unsigned short )0;
                         sqlstm.sqhstv[5] = (unsigned char  *)estrucLOGO.locomerc;
                         sqlstm.sqhstl[5] = (unsigned long )16;
                         sqlstm.sqhsts[5] = (         int  )0;
                         sqlstm.sqindv[5] = (         short *)0;
                         sqlstm.sqinds[5] = (         int  )0;
                         sqlstm.sqharm[5] = (unsigned long )0;
                         sqlstm.sqadto[5] = (unsigned short )0;
                         sqlstm.sqtdso[5] = (unsigned short )0;
                         sqlstm.sqhstv[6] = (unsigned char  *)estrucLOGO.loclavpr;
                         sqlstm.sqhstl[6] = (unsigned long )36;
                         sqlstm.sqhsts[6] = (         int  )0;
                         sqlstm.sqindv[6] = (         short *)0;
                         sqlstm.sqinds[6] = (         int  )0;
                         sqlstm.sqharm[6] = (unsigned long )0;
                         sqlstm.sqadto[6] = (unsigned short )0;
                         sqlstm.sqtdso[6] = (unsigned short )0;
                         sqlstm.sqhstv[7] = (unsigned char  *)estrucLOGO.loeseven;
                         sqlstm.sqhstl[7] = (unsigned long )2;
                         sqlstm.sqhsts[7] = (         int  )0;
                         sqlstm.sqindv[7] = (         short *)0;
                         sqlstm.sqinds[7] = (         int  )0;
                         sqlstm.sqharm[7] = (unsigned long )0;
                         sqlstm.sqadto[7] = (unsigned short )0;
                         sqlstm.sqtdso[7] = (unsigned short )0;
                         sqlstm.sqhstv[8] = (unsigned char  *)estrucLOGO.loesanul;
                         sqlstm.sqhstl[8] = (unsigned long )2;
                         sqlstm.sqhsts[8] = (         int  )0;
                         sqlstm.sqindv[8] = (         short *)0;
                         sqlstm.sqinds[8] = (         int  )0;
                         sqlstm.sqharm[8] = (unsigned long )0;
                         sqlstm.sqadto[8] = (unsigned short )0;
                         sqlstm.sqtdso[8] = (unsigned short )0;
                         sqlstm.sqhstv[9] = (unsigned char  *)estrucLOGO.loimporte;
                         sqlstm.sqhstl[9] = (unsigned long )13;
                         sqlstm.sqhsts[9] = (         int  )0;
                         sqlstm.sqindv[9] = (         short *)0;
                         sqlstm.sqinds[9] = (         int  )0;
                         sqlstm.sqharm[9] = (unsigned long )0;
                         sqlstm.sqadto[9] = (unsigned short )0;
                         sqlstm.sqtdso[9] = (unsigned short )0;
                         sqlstm.sqhstv[10] = (unsigned char  *)estrucLOGO.losesion;
                         sqlstm.sqhstl[10] = (unsigned long )4;
                         sqlstm.sqhsts[10] = (         int  )0;
                         sqlstm.sqindv[10] = (         short *)0;
                         sqlstm.sqinds[10] = (         int  )0;
                         sqlstm.sqharm[10] = (unsigned long )0;
                         sqlstm.sqadto[10] = (unsigned short )0;
                         sqlstm.sqtdso[10] = (unsigned short )0;
                         sqlstm.sqhstv[11] = (unsigned char  *)estrucLOGO.lofecses;
                         sqlstm.sqhstl[11] = (unsigned long )7;
                         sqlstm.sqhsts[11] = (         int  )0;
                         sqlstm.sqindv[11] = (         short *)0;
                         sqlstm.sqinds[11] = (         int  )0;
                         sqlstm.sqharm[11] = (unsigned long )0;
                         sqlstm.sqadto[11] = (unsigned short )0;
                         sqlstm.sqtdso[11] = (unsigned short )0;
                         sqlstm.sqhstv[12] = (unsigned char  *)estrucLOGO.loforitn;
                         sqlstm.sqhstl[12] = (unsigned long )4000;
                         sqlstm.sqhsts[12] = (         int  )0;
                         sqlstm.sqindv[12] = (         short *)0;
                         sqlstm.sqinds[12] = (         int  )0;
                         sqlstm.sqharm[12] = (unsigned long )0;
                         sqlstm.sqadto[12] = (unsigned short )0;
                         sqlstm.sqtdso[12] = (unsigned short )0;
                         sqlstm.sqphsv = sqlstm.sqhstv;
                         sqlstm.sqphsl = sqlstm.sqhstl;
                         sqlstm.sqphss = sqlstm.sqhsts;
                         sqlstm.sqpind = sqlstm.sqindv;
                         sqlstm.sqpins = sqlstm.sqinds;
                         sqlstm.sqparm = sqlstm.sqharm;
                         sqlstm.sqparc = sqlstm.sqharc;
                         sqlstm.sqpadto = sqlstm.sqadto;
                         sqlstm.sqptdso = sqlstm.sqtdso;
                         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
                         if (sqlca.sqlcode == 1403) break;
}

  
					 }

//* añadido para PRICE*//

           fprintf(stdout,"Contador >%d< CodError >%s< sqlca.sqlcode >%d<\n", contador, CodError, sqlca.sqlcode);
           contador++;
           if (!strncmp(opcionT, "IGUAL",5))
              break;
         }
       //  printf("salgo del for \n");
       //  printf("contador %i\n", contador);
          /* EXEC SQL CLOSE :registro3; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 13;
          sqlstm.arrsiz = 22;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )988;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
          sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
          sqlstm.sqhsts[0] = (         int  )0;
          sqlstm.sqindv[0] = (         short *)0;
          sqlstm.sqinds[0] = (         int  )0;
          sqlstm.sqharm[0] = (unsigned long )0;
          sqlstm.sqadto[0] = (unsigned short )0;
          sqlstm.sqtdso[0] = (unsigned short )0;
          sqlstm.sqphsv = sqlstm.sqhstv;
          sqlstm.sqphsl = sqlstm.sqhstl;
          sqlstm.sqphss = sqlstm.sqhsts;
          sqlstm.sqpind = sqlstm.sqindv;
          sqlstm.sqpins = sqlstm.sqinds;
          sqlstm.sqparm = sqlstm.sqharm;
          sqlstm.sqparc = sqlstm.sqharc;
          sqlstm.sqpadto = sqlstm.sqadto;
          sqlstm.sqptdso = sqlstm.sqtdso;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

  
          /* EXEC SQL FREE :registro3; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 13;
          sqlstm.arrsiz = 22;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.stmt = "";
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )1007;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
          sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
          sqlstm.sqhsts[0] = (         int  )0;
          sqlstm.sqindv[0] = (         short *)0;
          sqlstm.sqinds[0] = (         int  )0;
          sqlstm.sqharm[0] = (unsigned long )0;
          sqlstm.sqadto[0] = (unsigned short )0;
          sqlstm.sqtdso[0] = (unsigned short )0;
          sqlstm.sqphsv = sqlstm.sqhstv;
          sqlstm.sqphsl = sqlstm.sqhstl;
          sqlstm.sqphss = sqlstm.sqhsts;
          sqlstm.sqpind = sqlstm.sqindv;
          sqlstm.sqpins = sqlstm.sqinds;
          sqlstm.sqparm = sqlstm.sqharm;
          sqlstm.sqparc = sqlstm.sqharc;
          sqlstm.sqpadto = sqlstm.sqadto;
          sqlstm.sqptdso = sqlstm.sqtdso;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
        //  printf("despues del EXEC SQL CLOSE Y FREE registro3 \n");

         ///////////// Si el proceso ha abierto la conexion la libera, si no no
         if ((strncmp(CodError,"000",3)) || (contador==0))
         {
            fprintf(stdout,"Contador >%d< CodError >%s< sqlca.sqlcode >%d<\n", contador, CodError, sqlca.sqlcode);
               return(salir_rb(abroConexion, 101, tabla, opcion));

         }
         else
         {
//* añadido para PRICE *//

         if (!strncmp(tabla, "NPTPARS", 7))
         {
            memset(registro, '\0', sizeof(NPSPARS));
            strcat(registro, estrucPARS.pasubsis);
            strcat(registro, estrucPARS.pasubapl);
            strcat(registro, estrucPARS.panomsub);
            strcat(registro, estrucPARS.pasemaf1);
            strcat(registro, estrucPARS.pasemaf2);
            strcat(registro, estrucPARS.paestado);
            strcat(registro, estrucPARS.patrepet);
            strcat(registro, estrucPARS.patrerep);
            strcat(registro, estrucPARS.patresnf);
            strcat(registro, estrucPARS.panumrea);
            strcat(registro, estrucPARS.pafecses);
            strcat(registro, estrucPARS.panumses);
            strcat(registro, estrucPARS.paconcil);
            strcat(registro, estrucPARS.panip);
            strcat(registro, estrucPARS.paportnume);
            strcat(registro, estrucPARS.pacidesti);
            strcat(registro, estrucPARS.paciadqui);
            strcat(registro, estrucPARS.paenvoff);
            strcat(registro, estrucPARS.paenvtst);
            strcat(registro, estrucPARS.patisub);
            strcat(registro, estrucPARS.paidred);
         }
         else
         {
          if (!strncmp(tabla, "NPTCASA", 7))
          {
          	NPSCASA casa;
          	int despla=0;
          	
            memset(registro, '\0', sizeof(NPSCASA));
            
            //PRJ0247167: se modifica la concatenacion de string por una copia de un numero de posiciones fija debido a la
            //            diferencia de medida entre los campos reales de NPTCASA y la estructura "formato_interno"
            /*strcat(registro, estrucCASA.canumope);
            strcat(registro, estrucCASA.catieabs);
            strcat(registro, estrucCASA.catieesp);
            strcat(registro, estrucCASA.caforitn);
            */
            
            memcpy(registro, estrucCASA.canumope,sizeof(casa.canumope));
            despla=sizeof(casa.canumope);
            memcpy(registro+despla, estrucCASA.catieabs, sizeof(casa.catieabs));
            despla=despla+sizeof(casa.catieabs);
            memcpy(registro+despla, estrucCASA.catieesp, sizeof(casa.catieesp));
            despla=despla+sizeof(casa.catieesp);
            memcpy(registro+despla, estrucCASA.caforitn, sizeof(casa.caforitn));
            
            // FIN modificacion PRJ0247167   
          }
          else
          {
           if(!strncmp(tabla, "NPTDIFE", 7))
           {
          	 NPSDIFE dife;
          	 int despla=0;
          	 
          	 printf ("JFL - Paso por la nueva modificacio del filefun de NPSDIFE\n");
             memset(registro, '\0', sizeof(NPSDIFE));
            //PRJ0247167: se modifica la concatenacion de string por una copia de un numero de posiciones fija debido a la
            //            diferencia de medida entre los campos reales de NPSDIFE y la estructura "formato_interno"
            /*strcat(registro, estrucDIFE.disubsis);
             strcat(registro, estrucDIFE.disubapl);
             strcat(registro, estrucDIFE.difecha0);
             strcat(registro, estrucDIFE.dihora00);
             strcat(registro, estrucDIFE.dinumope);
             strcat(registro, estrucDIFE.diforitn);*/
            
			//strcat(registro, estrucDIFE.diclavpr);

            memcpy(registro, estrucDIFE.disubsis, sizeof(dife.disubsis));
            despla=sizeof(dife.disubsis);
            memcpy(registro+despla, estrucDIFE.disubapl, sizeof(dife.disubapl));
            despla=despla+sizeof(dife.disubapl);
            memcpy(registro+despla, estrucDIFE.difecha0, sizeof(dife.difecha0));
            despla=despla+sizeof(dife.difecha0);
            memcpy(registro+despla, estrucDIFE.dihora00, sizeof(dife.dihora00));
            despla=despla+sizeof(dife.dihora00);
            memcpy(registro+despla, estrucDIFE.dinumope, sizeof(dife.dinumope));
            despla=despla+sizeof(dife.dinumope);
            memcpy(registro+despla, estrucDIFE.diforitn, sizeof(dife.diforitn)); 

            // FIN modificacion PRJ0247167   


           }
           else
           {
 //* añadido para PRICE *//              
            if(!strncmp(tabla, "NPTIDEN", 7))
            {
              memset(registro, '\0', sizeof(NPSIDEN));
              strcat(registro, estrucIDEN.idsubsis);
              strcat(registro, estrucIDEN.idsubapl);
              strcat(registro, estrucIDEN.idcomando);
              strcat(registro, estrucIDEN.idclave1);                                                      
              strcat(registro, estrucIDEN.idclave2);                                                      
            }                                                                                             
//* añadido para arranque sisma por bbdd *//                                                              
																									      
            else                                                                                          
            {                                                                                             
			 // if(!strncmp(tabla, "NPTPARA", 7))                                                          
			 // {                                                                                          
			 //   memset(registro, '\0', sizeof(NPSPARA)); 
             //   strcat(registro, estrucPARA.psindice);				 
			 //   strcat(registro, estrucPARA.pscsb000);                                                   
			 //   strcat(registro, estrucPARA.psnoment);                                      
			 //   strcat(registro, estrucPARA.psnumope);                                      
             //   strcat(registro, estrucPARA.psnument);				                         
			 //   strcat(registro, estrucPARA.pshcamse);                                      
			 //   strcat(registro, estrucPARA.psfeinse);                                      
             //   strcat(registro, estrucPARA.pshoinse);                                      
             //   strcat(registro, estrucPARA.psmodocs);                                      
             //   strcat(registro, estrucPARA.pstimesp);                                      
             //   strcat(registro, estrucPARA.psnumcross); 				                         
             //   strcat(registro, estrucPARA.pscerrando);                                         
             //   strcat(registro, estrucPARA.psresolu);       
			//strcat(registro, estrucPARA.psmonito);
			//strcat(registro, estrucPARA.pscotrms);
			//strcat(registro, estrucPARA.psidered);
			//strcat(registro, estrucPARA.pscltrat);
			//strcat(registro, estrucPARA.psclenvi);
			//strcat(registro, estrucPARA.psclrece);
             //   strcat(registro, estrucPARA.psclvhsm);                                              
			 // }                                                                                     
			   
//* añadido para PRICE *//			   
			 //  else                                                                  
			 //  {                                                                     
			      NPSLOGO logo;
          	int despla=0;
          	
          	memset(registro, '\0', sizeof(NPSLOGO));     
			                            
            //PRJ0247167: se modifica la concatenacion de string por una copia de un numero de posiciones fija debido a la
            //            diferencia de medida entre los campos reales de NPTLOGO y la estructura "formato_interno"
            /*
                 strcat(registro, estrucLOGO.lonumope);                              
                 strcat(registro, estrucLOGO.lofecha0);                              
                 strcat(registro, estrucLOGO.lohora00);
                 strcat(registro, estrucLOGO.lonumpan);
                 strcat(registro, estrucLOGO.locomerc);
                 strcat(registro, estrucLOGO.loclavpr);
                 strcat(registro, estrucLOGO.loeseven);
                 strcat(registro, estrucLOGO.loesanul);
                 strcat(registro, estrucLOGO.loimporte);
                 strcat(registro, estrucLOGO.losesion);
                 strcat(registro, estrucLOGO.lofecses);
                 strcat(registro, estrucLOGO.loforitn);
            */     
            memcpy(registro, estrucLOGO.lonumope, sizeof(logo.lonumope));
            despla=sizeof(logo.lonumope);
            memcpy(registro+despla, estrucLOGO.lofecha0, sizeof(logo.lofecha0));
            despla=despla+sizeof(logo.lofecha0);            
            memcpy(registro+despla, estrucLOGO.lohora00, sizeof(logo.lohora00));
            despla=despla+sizeof(logo.lohora00);
            memcpy(registro+despla, estrucLOGO.lonumpan, sizeof(logo.lonumpan));
            despla=despla+sizeof(logo.lonumpan);
            memcpy(registro+despla, estrucLOGO.locomerc, sizeof(logo.locomerc));
            despla=despla+sizeof(logo.locomerc);
            memcpy(registro+despla, estrucLOGO.loclavpr, sizeof(logo.loclavpr));
            despla=despla+sizeof(logo.loclavpr);
            memcpy(registro+despla, estrucLOGO.loeseven, sizeof(logo.loeseven));
            despla=despla+sizeof(logo.loeseven);
            memcpy(registro+despla, estrucLOGO.loesanul, sizeof(logo.loesanul));
            despla=despla+sizeof(logo.loesanul);
            memcpy(registro+despla, estrucLOGO.loimporte, sizeof(logo.loimporte));
            despla=despla+sizeof(logo.loimporte);
            memcpy(registro+despla, estrucLOGO.losesion, sizeof(logo.losesion));
            despla=despla+sizeof(logo.losesion);
            memcpy(registro+despla, estrucLOGO.lofecses, sizeof(logo.lofecses));
            despla=despla+sizeof(logo.lofecses);
            memcpy(registro+despla, estrucLOGO.loforitn, sizeof(logo.loforitn));
            
            // FIN modificacion PRJ0247167   
                 
			   //}//else npslpara
			 } //else iden
            }//else dife
           }//else casa
          }//else pars 
         }//else coderor
       } // if cursores 

     else
     {
       if (!strncmp(tabla, "NPTLOGO", 7)) //cursores
       {
         //printf("NPTLOGO llamadaProc2\n"); 
         contador=0;
         /* EXEC SQL ALLOCATE :registro3; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 13;
         sqlstm.arrsiz = 22;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1026;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
         sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         short *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned long )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqphsv = sqlstm.sqhstv;
         sqlstm.sqphsl = sqlstm.sqhstl;
         sqlstm.sqphss = sqlstm.sqhsts;
         sqlstm.sqpind = sqlstm.sqindv;
         sqlstm.sqpins = sqlstm.sqinds;
         sqlstm.sqparm = sqlstm.sqharm;
         sqlstm.sqparc = sqlstm.sqharc;
         sqlstm.sqpadto = sqlstm.sqadto;
         sqlstm.sqptdso = sqlstm.sqtdso;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

  
         /* EXEC SQL CALL llamadaProc2 (:opcionT, :tabla, :claveT, :registro3, :CodError); */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 13;
         sqlstm.arrsiz = 22;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "call llamadaProc2(:b0,:b1,:b2,:b3,:b4)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1045;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (unsigned char  *)opcionT;
         sqlstm.sqhstl[0] = (unsigned long )20;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         short *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned long )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (unsigned char  *)tabla;
         sqlstm.sqhstl[1] = (unsigned long )8;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         short *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned long )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (unsigned char  *)claveT;
         sqlstm.sqhstl[2] = (unsigned long )200;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         short *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned long )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (unsigned char  *)&registro3;
         sqlstm.sqhstl[3] = (unsigned long )sizeof(int);
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         short *)0;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned long )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (unsigned char  *)CodError;
         sqlstm.sqhstl[4] = (unsigned long )4;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         short *)0;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned long )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqphsv = sqlstm.sqhstv;
         sqlstm.sqphsl = sqlstm.sqhstl;
         sqlstm.sqphss = sqlstm.sqhsts;
         sqlstm.sqpind = sqlstm.sqindv;
         sqlstm.sqpins = sqlstm.sqinds;
         sqlstm.sqparm = sqlstm.sqharm;
         sqlstm.sqparc = sqlstm.sqharc;
         sqlstm.sqpadto = sqlstm.sqadto;
         sqlstm.sqptdso = sqlstm.sqtdso;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
         /* EXEC SQL WHENEVER NOT FOUND DO break; */ 
 

         ++registro3.curocn;
         for (;;){
            /* EXEC SQL FETCH :registro3
                INTO :estrucLOGO; */ 

{
            struct sqlexd sqlstm;
            sqlstm.sqlvsn = 13;
            sqlstm.arrsiz = 22;
            sqlstm.sqladtp = &sqladt;
            sqlstm.sqltdsp = &sqltds;
            sqlstm.iters = (unsigned int  )1;
            sqlstm.offset = (unsigned int  )1080;
            sqlstm.selerr = (unsigned short)1;
            sqlstm.sqlpfmem = (unsigned int  )0;
            sqlstm.cud = sqlcud0;
            sqlstm.sqlest = (unsigned char  *)&sqlca;
            sqlstm.sqlety = (unsigned short)4352;
            sqlstm.occurs = (unsigned int  )0;
            sqlstm.sqfoff = (         int )0;
            sqlstm.sqfmod = (unsigned int )2;
            sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
            sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
            sqlstm.sqhsts[0] = (         int  )0;
            sqlstm.sqindv[0] = (         short *)0;
            sqlstm.sqinds[0] = (         int  )0;
            sqlstm.sqharm[0] = (unsigned long )0;
            sqlstm.sqadto[0] = (unsigned short )0;
            sqlstm.sqtdso[0] = (unsigned short )0;
            sqlstm.sqhstv[1] = (unsigned char  *)estrucLOGO.lonumope;
            sqlstm.sqhstl[1] = (unsigned long )7;
            sqlstm.sqhsts[1] = (         int  )0;
            sqlstm.sqindv[1] = (         short *)0;
            sqlstm.sqinds[1] = (         int  )0;
            sqlstm.sqharm[1] = (unsigned long )0;
            sqlstm.sqadto[1] = (unsigned short )0;
            sqlstm.sqtdso[1] = (unsigned short )0;
            sqlstm.sqhstv[2] = (unsigned char  *)estrucLOGO.lofecha0;
            sqlstm.sqhstl[2] = (unsigned long )9;
            sqlstm.sqhsts[2] = (         int  )0;
            sqlstm.sqindv[2] = (         short *)0;
            sqlstm.sqinds[2] = (         int  )0;
            sqlstm.sqharm[2] = (unsigned long )0;
            sqlstm.sqadto[2] = (unsigned short )0;
            sqlstm.sqtdso[2] = (unsigned short )0;
            sqlstm.sqhstv[3] = (unsigned char  *)estrucLOGO.lohora00;
            sqlstm.sqhstl[3] = (unsigned long )7;
            sqlstm.sqhsts[3] = (         int  )0;
            sqlstm.sqindv[3] = (         short *)0;
            sqlstm.sqinds[3] = (         int  )0;
            sqlstm.sqharm[3] = (unsigned long )0;
            sqlstm.sqadto[3] = (unsigned short )0;
            sqlstm.sqtdso[3] = (unsigned short )0;
            sqlstm.sqhstv[4] = (unsigned char  *)estrucLOGO.lonumpan;
            sqlstm.sqhstl[4] = (unsigned long )20;
            sqlstm.sqhsts[4] = (         int  )0;
            sqlstm.sqindv[4] = (         short *)0;
            sqlstm.sqinds[4] = (         int  )0;
            sqlstm.sqharm[4] = (unsigned long )0;
            sqlstm.sqadto[4] = (unsigned short )0;
            sqlstm.sqtdso[4] = (unsigned short )0;
            sqlstm.sqhstv[5] = (unsigned char  *)estrucLOGO.locomerc;
            sqlstm.sqhstl[5] = (unsigned long )16;
            sqlstm.sqhsts[5] = (         int  )0;
            sqlstm.sqindv[5] = (         short *)0;
            sqlstm.sqinds[5] = (         int  )0;
            sqlstm.sqharm[5] = (unsigned long )0;
            sqlstm.sqadto[5] = (unsigned short )0;
            sqlstm.sqtdso[5] = (unsigned short )0;
            sqlstm.sqhstv[6] = (unsigned char  *)estrucLOGO.loclavpr;
            sqlstm.sqhstl[6] = (unsigned long )36;
            sqlstm.sqhsts[6] = (         int  )0;
            sqlstm.sqindv[6] = (         short *)0;
            sqlstm.sqinds[6] = (         int  )0;
            sqlstm.sqharm[6] = (unsigned long )0;
            sqlstm.sqadto[6] = (unsigned short )0;
            sqlstm.sqtdso[6] = (unsigned short )0;
            sqlstm.sqhstv[7] = (unsigned char  *)estrucLOGO.loeseven;
            sqlstm.sqhstl[7] = (unsigned long )2;
            sqlstm.sqhsts[7] = (         int  )0;
            sqlstm.sqindv[7] = (         short *)0;
            sqlstm.sqinds[7] = (         int  )0;
            sqlstm.sqharm[7] = (unsigned long )0;
            sqlstm.sqadto[7] = (unsigned short )0;
            sqlstm.sqtdso[7] = (unsigned short )0;
            sqlstm.sqhstv[8] = (unsigned char  *)estrucLOGO.loesanul;
            sqlstm.sqhstl[8] = (unsigned long )2;
            sqlstm.sqhsts[8] = (         int  )0;
            sqlstm.sqindv[8] = (         short *)0;
            sqlstm.sqinds[8] = (         int  )0;
            sqlstm.sqharm[8] = (unsigned long )0;
            sqlstm.sqadto[8] = (unsigned short )0;
            sqlstm.sqtdso[8] = (unsigned short )0;
            sqlstm.sqhstv[9] = (unsigned char  *)estrucLOGO.loimporte;
            sqlstm.sqhstl[9] = (unsigned long )13;
            sqlstm.sqhsts[9] = (         int  )0;
            sqlstm.sqindv[9] = (         short *)0;
            sqlstm.sqinds[9] = (         int  )0;
            sqlstm.sqharm[9] = (unsigned long )0;
            sqlstm.sqadto[9] = (unsigned short )0;
            sqlstm.sqtdso[9] = (unsigned short )0;
            sqlstm.sqhstv[10] = (unsigned char  *)estrucLOGO.losesion;
            sqlstm.sqhstl[10] = (unsigned long )4;
            sqlstm.sqhsts[10] = (         int  )0;
            sqlstm.sqindv[10] = (         short *)0;
            sqlstm.sqinds[10] = (         int  )0;
            sqlstm.sqharm[10] = (unsigned long )0;
            sqlstm.sqadto[10] = (unsigned short )0;
            sqlstm.sqtdso[10] = (unsigned short )0;
            sqlstm.sqhstv[11] = (unsigned char  *)estrucLOGO.lofecses;
            sqlstm.sqhstl[11] = (unsigned long )7;
            sqlstm.sqhsts[11] = (         int  )0;
            sqlstm.sqindv[11] = (         short *)0;
            sqlstm.sqinds[11] = (         int  )0;
            sqlstm.sqharm[11] = (unsigned long )0;
            sqlstm.sqadto[11] = (unsigned short )0;
            sqlstm.sqtdso[11] = (unsigned short )0;
            sqlstm.sqhstv[12] = (unsigned char  *)estrucLOGO.loforitn;
            sqlstm.sqhstl[12] = (unsigned long )4000;
            sqlstm.sqhsts[12] = (         int  )0;
            sqlstm.sqindv[12] = (         short *)0;
            sqlstm.sqinds[12] = (         int  )0;
            sqlstm.sqharm[12] = (unsigned long )0;
            sqlstm.sqadto[12] = (unsigned short )0;
            sqlstm.sqtdso[12] = (unsigned short )0;
            sqlstm.sqphsv = sqlstm.sqhstv;
            sqlstm.sqphsl = sqlstm.sqhstl;
            sqlstm.sqphss = sqlstm.sqhsts;
            sqlstm.sqpind = sqlstm.sqindv;
            sqlstm.sqpins = sqlstm.sqinds;
            sqlstm.sqparm = sqlstm.sqharm;
            sqlstm.sqparc = sqlstm.sqharc;
            sqlstm.sqpadto = sqlstm.sqadto;
            sqlstm.sqptdso = sqlstm.sqtdso;
            sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
            if (sqlca.sqlcode == 1403) break;
}

 
            contador++;
         }
          /* EXEC SQL CLOSE :registro3; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 13;
          sqlstm.arrsiz = 22;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )1147;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
          sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
          sqlstm.sqhsts[0] = (         int  )0;
          sqlstm.sqindv[0] = (         short *)0;
          sqlstm.sqinds[0] = (         int  )0;
          sqlstm.sqharm[0] = (unsigned long )0;
          sqlstm.sqadto[0] = (unsigned short )0;
          sqlstm.sqtdso[0] = (unsigned short )0;
          sqlstm.sqphsv = sqlstm.sqhstv;
          sqlstm.sqphsl = sqlstm.sqhstl;
          sqlstm.sqphss = sqlstm.sqhsts;
          sqlstm.sqpind = sqlstm.sqindv;
          sqlstm.sqpins = sqlstm.sqinds;
          sqlstm.sqparm = sqlstm.sqharm;
          sqlstm.sqparc = sqlstm.sqharc;
          sqlstm.sqpadto = sqlstm.sqadto;
          sqlstm.sqptdso = sqlstm.sqtdso;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
          /* EXEC SQL FREE :registro3; */ 

{
          struct sqlexd sqlstm;
          sqlstm.sqlvsn = 13;
          sqlstm.arrsiz = 22;
          sqlstm.sqladtp = &sqladt;
          sqlstm.sqltdsp = &sqltds;
          sqlstm.stmt = "";
          sqlstm.iters = (unsigned int  )1;
          sqlstm.offset = (unsigned int  )1166;
          sqlstm.cud = sqlcud0;
          sqlstm.sqlest = (unsigned char  *)&sqlca;
          sqlstm.sqlety = (unsigned short)4352;
          sqlstm.occurs = (unsigned int  )0;
          sqlstm.sqhstv[0] = (unsigned char  *)&registro3;
          sqlstm.sqhstl[0] = (unsigned long )sizeof(int);
          sqlstm.sqhsts[0] = (         int  )0;
          sqlstm.sqindv[0] = (         short *)0;
          sqlstm.sqinds[0] = (         int  )0;
          sqlstm.sqharm[0] = (unsigned long )0;
          sqlstm.sqadto[0] = (unsigned short )0;
          sqlstm.sqtdso[0] = (unsigned short )0;
          sqlstm.sqphsv = sqlstm.sqhstv;
          sqlstm.sqphsl = sqlstm.sqhstl;
          sqlstm.sqphss = sqlstm.sqhsts;
          sqlstm.sqpind = sqlstm.sqindv;
          sqlstm.sqpins = sqlstm.sqinds;
          sqlstm.sqparm = sqlstm.sqharm;
          sqlstm.sqparc = sqlstm.sqharc;
          sqlstm.sqpadto = sqlstm.sqadto;
          sqlstm.sqptdso = sqlstm.sqtdso;
          sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
         if ((strncmp(CodError,"000",3)) || (contador==0))
         {
            printf("Contador >%d< CodError >%s< sqlca.sqlcode >%d< llamdaProc2 NPTLOGO\n", contador, CodError, sqlca.sqlcode);
            //EXEC SQL ROLLBACK RELEASE;
            //return  (101);
						return(salir_rb(abroConexion, 101, tabla, opcion));
         }
         else
         {
			      NPSLOGO logo;
          	int despla=0;
          	
            memset(registro, '\0', sizeof(NPSLOGO));
            
            //PRJ0247167: se modifica la concatenacion de string por una copia de un numero de posiciones fija debido a la
            //            diferencia de medida entre los campos reales de NPTLOGO y la estructura "formato_interno"    
            /*
            strcat(registro, estrucLOGO.lonumope);
            strcat(registro, estrucLOGO.lofecha0);
            strcat(registro, estrucLOGO.lohora00);
            strcat(registro, estrucLOGO.lonumpan);
            strcat(registro, estrucLOGO.locomerc);
            strcat(registro, estrucLOGO.loclavpr);
            strcat(registro, estrucLOGO.loeseven);
            strcat(registro, estrucLOGO.loesanul);
            strcat(registro, estrucLOGO.loimporte);
            strcat(registro, estrucLOGO.losesion);
            strcat(registro, estrucLOGO.lofecses);
            strcat(registro, estrucLOGO.loforitn);
            */
            
            memcpy(registro, estrucLOGO.lonumope, sizeof(logo.lonumope));
            despla=sizeof(logo.lonumope);
            memcpy(registro+despla, estrucLOGO.lofecha0, sizeof(logo.lofecha0));
            despla=despla+sizeof(logo.lofecha0);            
            memcpy(registro+despla, estrucLOGO.lohora00, sizeof(logo.lohora00));
            despla=despla+sizeof(logo.lohora00);
            memcpy(registro+despla, estrucLOGO.lonumpan, sizeof(logo.lonumpan));
            despla=despla+sizeof(logo.lonumpan);
            memcpy(registro+despla, estrucLOGO.locomerc, sizeof(logo.locomerc));
            despla=despla+sizeof(logo.locomerc);
            memcpy(registro+despla, estrucLOGO.loclavpr, sizeof(logo.loclavpr));
            despla=despla+sizeof(logo.loclavpr);
            memcpy(registro+despla, estrucLOGO.loeseven, sizeof(logo.loeseven));
            despla=despla+sizeof(logo.loeseven);
            memcpy(registro+despla, estrucLOGO.loesanul, sizeof(logo.loesanul));
            despla=despla+sizeof(logo.loesanul);
            memcpy(registro+despla, estrucLOGO.loimporte, sizeof(logo.loimporte));
            despla=despla+sizeof(logo.loimporte);
            memcpy(registro+despla, estrucLOGO.losesion, sizeof(logo.losesion));
            despla=despla+sizeof(logo.losesion);
            memcpy(registro+despla, estrucLOGO.lofecses, sizeof(logo.lofecses));
            despla=despla+sizeof(logo.lofecses);
            memcpy(registro+despla, estrucLOGO.loforitn, sizeof(logo.loforitn));
         	            
            // FIN modificacion PRJ0247167   
         }
       }
       else
       {
        if (!strncmp(tabla, "NPTDIFE", 7) && !strncmp(opcionT, "BORRAR", 6))
        {
            strncpy(subsisF, claveT, 2);
            subsisF[2]='\0';
            strncpy(subapl, claveT+2, 2);
            subapl[2]='\0';
            strncpy(fecha0, claveT+4, 8);
            fecha0[8]='\0';
            strncpy(hora00, claveT+12, 6);
            hora00[6]='\0';
            strncpy(numope, claveT+18, 6);
            numope[6]='\0';
            
            contador = 0;
            while (contador == 0)
            {
              /* EXEC SQL DELETE NPTDIFE WHERE disubsis = :subsisF and disubapl = :subapl and difecha0 = :fecha0 and 
                                         dihora00 = :hora00 and dinumope = :numope; */ 

{
              struct sqlexd sqlstm;
              sqlstm.sqlvsn = 13;
              sqlstm.arrsiz = 22;
              sqlstm.sqladtp = &sqladt;
              sqlstm.sqltdsp = &sqltds;
              sqlstm.stmt = "delete  from NPTDIFE  where ((((disubsis=:b0 an\
d disubapl=:b1) and difecha0=:b2) and dihora00=:b3) and dinumope=:b4)";
              sqlstm.iters = (unsigned int  )1;
              sqlstm.offset = (unsigned int  )1185;
              sqlstm.cud = sqlcud0;
              sqlstm.sqlest = (unsigned char  *)&sqlca;
              sqlstm.sqlety = (unsigned short)4352;
              sqlstm.occurs = (unsigned int  )0;
              sqlstm.sqhstv[0] = (unsigned char  *)subsisF;
              sqlstm.sqhstl[0] = (unsigned long )3;
              sqlstm.sqhsts[0] = (         int  )0;
              sqlstm.sqindv[0] = (         short *)0;
              sqlstm.sqinds[0] = (         int  )0;
              sqlstm.sqharm[0] = (unsigned long )0;
              sqlstm.sqadto[0] = (unsigned short )0;
              sqlstm.sqtdso[0] = (unsigned short )0;
              sqlstm.sqhstv[1] = (unsigned char  *)subapl;
              sqlstm.sqhstl[1] = (unsigned long )3;
              sqlstm.sqhsts[1] = (         int  )0;
              sqlstm.sqindv[1] = (         short *)0;
              sqlstm.sqinds[1] = (         int  )0;
              sqlstm.sqharm[1] = (unsigned long )0;
              sqlstm.sqadto[1] = (unsigned short )0;
              sqlstm.sqtdso[1] = (unsigned short )0;
              sqlstm.sqhstv[2] = (unsigned char  *)fecha0;
              sqlstm.sqhstl[2] = (unsigned long )9;
              sqlstm.sqhsts[2] = (         int  )0;
              sqlstm.sqindv[2] = (         short *)0;
              sqlstm.sqinds[2] = (         int  )0;
              sqlstm.sqharm[2] = (unsigned long )0;
              sqlstm.sqadto[2] = (unsigned short )0;
              sqlstm.sqtdso[2] = (unsigned short )0;
              sqlstm.sqhstv[3] = (unsigned char  *)hora00;
              sqlstm.sqhstl[3] = (unsigned long )7;
              sqlstm.sqhsts[3] = (         int  )0;
              sqlstm.sqindv[3] = (         short *)0;
              sqlstm.sqinds[3] = (         int  )0;
              sqlstm.sqharm[3] = (unsigned long )0;
              sqlstm.sqadto[3] = (unsigned short )0;
              sqlstm.sqtdso[3] = (unsigned short )0;
              sqlstm.sqhstv[4] = (unsigned char  *)numope;
              sqlstm.sqhstl[4] = (unsigned long )7;
              sqlstm.sqhsts[4] = (         int  )0;
              sqlstm.sqindv[4] = (         short *)0;
              sqlstm.sqinds[4] = (         int  )0;
              sqlstm.sqharm[4] = (unsigned long )0;
              sqlstm.sqadto[4] = (unsigned short )0;
              sqlstm.sqtdso[4] = (unsigned short )0;
              sqlstm.sqphsv = sqlstm.sqhstv;
              sqlstm.sqphsl = sqlstm.sqhstl;
              sqlstm.sqphss = sqlstm.sqhsts;
              sqlstm.sqpind = sqlstm.sqindv;
              sqlstm.sqpins = sqlstm.sqinds;
              sqlstm.sqparm = sqlstm.sqharm;
              sqlstm.sqparc = sqlstm.sqharc;
              sqlstm.sqpadto = sqlstm.sqadto;
              sqlstm.sqptdso = sqlstm.sqtdso;
              sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
              if (sqlca.sqlcode == 1403) break;
}


              contador++;
           }
           if (sqlca.sqlcode != 0)
           {
               printf("sqlcode tras el borrado NPTDIFE %d\n",sqlca.sqlcode);
			  //EXEC SQL ROLLBACK RELEASE;
              //return(101);
			  return(salir_rb(abroConexion, 101, tabla, opcion));
           }
           //printf("borrado\n");
        }
			//* añadido para PRICE *//        
     else
     {
         fprintf(stdout,"LLamadaProc opcion %s,  tabla %s, clave %.25s\n",  (char*)opcionT, (char*)tabla, (char*)claveT);
         fflush(stdout);
         memset(registro2, '\0', 3000);

         /* EXEC SQL CALL LlamadaProc (:opcionT, :tabla, :claveT, :registro2, :CodError); */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 13;
         sqlstm.arrsiz = 22;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "call LlamadaProc(:b0,:b1,:b2,:b3,:b4)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1220;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (unsigned char  *)opcionT;
         sqlstm.sqhstl[0] = (unsigned long )20;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         short *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned long )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (unsigned char  *)tabla;
         sqlstm.sqhstl[1] = (unsigned long )8;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         short *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned long )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (unsigned char  *)claveT;
         sqlstm.sqhstl[2] = (unsigned long )200;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         short *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned long )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (unsigned char  *)registro2;
         sqlstm.sqhstl[3] = (unsigned long )3000;
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         short *)0;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned long )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (unsigned char  *)CodError;
         sqlstm.sqhstl[4] = (unsigned long )4;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         short *)0;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned long )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqphsv = sqlstm.sqhstv;
         sqlstm.sqphsl = sqlstm.sqhstl;
         sqlstm.sqphss = sqlstm.sqhsts;
         sqlstm.sqpind = sqlstm.sqindv;
         sqlstm.sqpins = sqlstm.sqinds;
         sqlstm.sqparm = sqlstm.sqharm;
         sqlstm.sqparc = sqlstm.sqharc;
         sqlstm.sqpadto = sqlstm.sqadto;
         sqlstm.sqptdso = sqlstm.sqtdso;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}

 
         
         if (sqlca.sqlcode != 0) 
         {
           if ((sqlca.sqlcode != -1405) || 
               ((sqlca.sqlcode == -1405) && (retorReg==0)))
           {
             printf("error en el acceso LlamadaProc %i\n", sqlca.sqlcode );
           }
         } 
         
         ///////// Si el proceso ha abierto la conexion la libera, si no no
         if  (strncmp(CodError,"000",3))
         {
             printf("CodError >%s< sqlca.sqlcode >%d< en LlamadaProc\n", CodError, sqlca.sqlcode);
             return(salir_rb(abroConexion, 101, tabla, opcion));
         }
         else
            if (retorReg==0)
            {
              strncpy (registro, (char*)registro2, (strlen(registro2)));
            }
          printf("BBDD OK\n");
        }
       }
     }
   }
   
     return(salir_cm(abroConexion,0, tabla, opcion));
     
}

int bloquear(short tipobloq)
{
    return(0);
}

int transaccion(int transmodo)
{
  return(0);
}

//* añadido para PRICE *//
int InsertarC(void* registro)
{
    int cont=0;
    int errSQL=0;

    QuitarCaracteres(registro,sizeof(NPSCASA));  

    strncpy(numope, registro, 6);
    numope[6]='\0';
    strncpy(catieabs, (char*)registro+6, 10);
    catieabs[10]='\0';
    strncpy(catieesp, (char*)registro+16, 10);
    catieesp[10]='\0';
    strncpy(formatoI, (char*)registro+26, sizeof(formatoI)-1);
    formatoI[sizeof(formatoI)-1]='\0';

    while (cont==0)
    {
	   cont++;
	   
       /* EXEC SQL INSERT INTO NPTCASA VALUES (:numope,:catieabs,:catieesp,:formatoI ); */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 13;
       sqlstm.arrsiz = 22;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "insert into NPTCASA  values (:b0,:b1,:b2,:b3)";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )1255;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)numope;
       sqlstm.sqhstl[0] = (unsigned long )7;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)catieabs;
       sqlstm.sqhstl[1] = (unsigned long )11;
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqhstv[2] = (unsigned char  *)catieesp;
       sqlstm.sqhstl[2] = (unsigned long )11;
       sqlstm.sqhsts[2] = (         int  )0;
       sqlstm.sqindv[2] = (         short *)0;
       sqlstm.sqinds[2] = (         int  )0;
       sqlstm.sqharm[2] = (unsigned long )0;
       sqlstm.sqadto[2] = (unsigned short )0;
       sqlstm.sqtdso[2] = (unsigned short )0;
       sqlstm.sqhstv[3] = (unsigned char  *)formatoI;
       sqlstm.sqhstl[3] = (unsigned long )4000;
       sqlstm.sqhsts[3] = (         int  )0;
       sqlstm.sqindv[3] = (         short *)0;
       sqlstm.sqinds[3] = (         int  )0;
       sqlstm.sqharm[3] = (unsigned long )0;
       sqlstm.sqadto[3] = (unsigned short )0;
       sqlstm.sqtdso[3] = (unsigned short )0;
       sqlstm.sqphsv = sqlstm.sqhstv;
       sqlstm.sqphsl = sqlstm.sqhstl;
       sqlstm.sqphss = sqlstm.sqhsts;
       sqlstm.sqpind = sqlstm.sqindv;
       sqlstm.sqpins = sqlstm.sqinds;
       sqlstm.sqparm = sqlstm.sqharm;
       sqlstm.sqparc = sqlstm.sqharc;
       sqlstm.sqpadto = sqlstm.sqadto;
       sqlstm.sqptdso = sqlstm.sqtdso;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
       if (sqlca.sqlcode == 1403) break;
}

 
       
	   // si da error por clave duplicada hacemos update
	   if(sqlca.sqlcode == -1)
	   {
           /* EXEC SQL UPDATE NPTCASA set        
           CATIEABS = :catieabs,
		   CATIEESP = :catieesp,
		   CAFORITN = :formatoI
           where CANUMOPE=:numope; */ 

{
           struct sqlexd sqlstm;
           sqlstm.sqlvsn = 13;
           sqlstm.arrsiz = 22;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.stmt = "update NPTCASA  set CATIEABS=:b0,CATIEESP=:b1,CAFO\
RITN=:b2 where CANUMOPE=:b3";
           sqlstm.iters = (unsigned int  )1;
           sqlstm.offset = (unsigned int  )1286;
           sqlstm.cud = sqlcud0;
           sqlstm.sqlest = (unsigned char  *)&sqlca;
           sqlstm.sqlety = (unsigned short)4352;
           sqlstm.occurs = (unsigned int  )0;
           sqlstm.sqhstv[0] = (unsigned char  *)catieabs;
           sqlstm.sqhstl[0] = (unsigned long )11;
           sqlstm.sqhsts[0] = (         int  )0;
           sqlstm.sqindv[0] = (         short *)0;
           sqlstm.sqinds[0] = (         int  )0;
           sqlstm.sqharm[0] = (unsigned long )0;
           sqlstm.sqadto[0] = (unsigned short )0;
           sqlstm.sqtdso[0] = (unsigned short )0;
           sqlstm.sqhstv[1] = (unsigned char  *)catieesp;
           sqlstm.sqhstl[1] = (unsigned long )11;
           sqlstm.sqhsts[1] = (         int  )0;
           sqlstm.sqindv[1] = (         short *)0;
           sqlstm.sqinds[1] = (         int  )0;
           sqlstm.sqharm[1] = (unsigned long )0;
           sqlstm.sqadto[1] = (unsigned short )0;
           sqlstm.sqtdso[1] = (unsigned short )0;
           sqlstm.sqhstv[2] = (unsigned char  *)formatoI;
           sqlstm.sqhstl[2] = (unsigned long )4000;
           sqlstm.sqhsts[2] = (         int  )0;
           sqlstm.sqindv[2] = (         short *)0;
           sqlstm.sqinds[2] = (         int  )0;
           sqlstm.sqharm[2] = (unsigned long )0;
           sqlstm.sqadto[2] = (unsigned short )0;
           sqlstm.sqtdso[2] = (unsigned short )0;
           sqlstm.sqhstv[3] = (unsigned char  *)numope;
           sqlstm.sqhstl[3] = (unsigned long )7;
           sqlstm.sqhsts[3] = (         int  )0;
           sqlstm.sqindv[3] = (         short *)0;
           sqlstm.sqinds[3] = (         int  )0;
           sqlstm.sqharm[3] = (unsigned long )0;
           sqlstm.sqadto[3] = (unsigned short )0;
           sqlstm.sqtdso[3] = (unsigned short )0;
           sqlstm.sqphsv = sqlstm.sqhstv;
           sqlstm.sqphsl = sqlstm.sqhstl;
           sqlstm.sqphss = sqlstm.sqhsts;
           sqlstm.sqpind = sqlstm.sqindv;
           sqlstm.sqpins = sqlstm.sqinds;
           sqlstm.sqparm = sqlstm.sqharm;
           sqlstm.sqparc = sqlstm.sqharc;
           sqlstm.sqpadto = sqlstm.sqadto;
           sqlstm.sqptdso = sqlstm.sqtdso;
           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
           if (sqlca.sqlcode == 1403) break;
}

 	   
	   }
       if (sqlca.sqlcode != 0)
       {
         errSQL = sqlca.sqlcode;
         printf("sqlcode tras el insert NPTCASA >%d<\n",sqlca.sqlcode);
       } 
    }
	
    return(errSQL);
}
//* añadido para PRICE *//

int InsertarL(void* registro)
{

    int cont=0;
    int errSQL=0;
    NPSLOGO logo;

		//printf("...... Llamada al procedimiento InsertarL .-.-.- \n"); // temporal
		//printf(".... Cadena de entrada registro 1 >%s< \n", registro);       // temporal

    //PRJ0247167: se modifica la concatenacion de string por una copia de un numero de posiciones fija debido a la
    //            diferencia de medida entre los campos reales de NPTLOGO y la estructura "formato_interno"    
    //QuitarCaracteres(registro, sizeof(NPSLOGO));
    QuitarCaracteres(registro, sizeof(logo));
		//printf(".... Cadena de entrada registro 2 >%s< \n", registro);			 // temporal

    strncpy(numope,registro, 6);
    numope[6]='\0';

    strncpy(fecha0, (char*)registro+6, 8);
    fecha0[8]='\0';

    strncpy(hora00, (char*)registro+14, 6);
    hora00[6]='\0';

    strncpy(lonumpan, (char*)registro+20, 19);
    lonumpan[19]='\0';
    
    strncpy(locomerc, (char*)registro+39, 15);
    locomerc[15]='\0';

    strncpy(loclavpr, (char*)registro+54, 35);
    loclavpr[35]='\0';

    strncpy(loeseven, (char*)registro+89, 1);
    loeseven[1]='\0';

    strncpy(loesanul, (char*)registro+90, 1);
    loesanul[1]='\0';

    strncpy(loimporte, (char*)registro+91, 12);
    loimporte[12]='\0';

    strncpy(losesion, (char*)registro+103, 3);
    losesion[3]='\0';

    strncpy(lofecses, (char*)registro+106, 6);
    lofecses[6]='\0';

	
    //PRJ0247167: se modifica la concatenacion de string por una copia de un numero de posiciones fija debido a la
    //            diferencia de medida entre los campos reales de NPTLOGO y la estructura "formato_interno"    
    //strncpy(formatoI, (char*)registro+112, sizeof(formatoI)-1);
    strncpy(formatoI, (char*)registro+112, sizeof(logo.loforitn)-1);
    
    formatoI[sizeof(formatoI)-1]='\0';
	
	//BIMSF-3087
	printf("filefun.cpp antes de el insert NPTLOGO fecha0 >%s<\n",fecha0);
	printf("filefun.cpp antes de el insert NPTLOGO hora00 >%s<\n",hora00);
	printf("filefun.cpp antes de el insert NPTLOGO numope >%s<\n",numope);
	printf("filefun.cpp antes de el insert NPTLOGO lonumpan >%s<\n",lonumpan);
	printf("filefun.cpp antes de el insert NPTLOGO locomerc >%s<\n",locomerc);
	printf("filefun.cpp antes de el insert NPTLOGO loclavpr >%s<\n",loclavpr);
	printf("filefun.cpp antes de el insert NPTLOGO loeseven >%s<\n",loeseven);
	printf("filefun.cpp antes de el insert NPTLOGO loesanul >%s<\n",loesanul);
	printf("filefun.cpp antes de el insert NPTLOGO loimporte >%s<\n",loimporte);
	printf("filefun.cpp antes de el insert NPTLOGO formatoI >%s<\n",formatoI);
	printf("filefun.cpp antes de el insert NPTLOGO losesion >%s<\n",losesion);
	printf("filefun.cpp antes de el insert NPTLOGO lofecses >%s<\n",lofecses);
 
 //* añadido para PRICE * //
     while (cont==0)  // si no lo meto en un bucle no compila
    {
      cont ++;
      if (!strncmp(opcionT, "INSERTAR",8 ))
      {
	
         /* EXEC SQL INSERT INTO NPTLOGO VALUES (:fecha0, :hora00,:numope
            ,:lonumpan
            ,:locomerc, :loclavpr, :loeseven, :loesanul, :loimporte,:formatoI, :losesion, :lofecses); */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 13;
         sqlstm.arrsiz = 22;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "insert into NPTLOGO  values (:b0,:b1,:b2,:b3,:b4,:b5\
,:b6,:b7,:b8,:b9,:b10,:b11)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1317;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (unsigned char  *)fecha0;
         sqlstm.sqhstl[0] = (unsigned long )9;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         short *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned long )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (unsigned char  *)hora00;
         sqlstm.sqhstl[1] = (unsigned long )7;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         short *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned long )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (unsigned char  *)numope;
         sqlstm.sqhstl[2] = (unsigned long )7;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         short *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned long )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (unsigned char  *)lonumpan;
         sqlstm.sqhstl[3] = (unsigned long )20;
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         short *)0;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned long )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (unsigned char  *)locomerc;
         sqlstm.sqhstl[4] = (unsigned long )16;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         short *)0;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned long )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqhstv[5] = (unsigned char  *)loclavpr;
         sqlstm.sqhstl[5] = (unsigned long )36;
         sqlstm.sqhsts[5] = (         int  )0;
         sqlstm.sqindv[5] = (         short *)0;
         sqlstm.sqinds[5] = (         int  )0;
         sqlstm.sqharm[5] = (unsigned long )0;
         sqlstm.sqadto[5] = (unsigned short )0;
         sqlstm.sqtdso[5] = (unsigned short )0;
         sqlstm.sqhstv[6] = (unsigned char  *)loeseven;
         sqlstm.sqhstl[6] = (unsigned long )2;
         sqlstm.sqhsts[6] = (         int  )0;
         sqlstm.sqindv[6] = (         short *)0;
         sqlstm.sqinds[6] = (         int  )0;
         sqlstm.sqharm[6] = (unsigned long )0;
         sqlstm.sqadto[6] = (unsigned short )0;
         sqlstm.sqtdso[6] = (unsigned short )0;
         sqlstm.sqhstv[7] = (unsigned char  *)loesanul;
         sqlstm.sqhstl[7] = (unsigned long )2;
         sqlstm.sqhsts[7] = (         int  )0;
         sqlstm.sqindv[7] = (         short *)0;
         sqlstm.sqinds[7] = (         int  )0;
         sqlstm.sqharm[7] = (unsigned long )0;
         sqlstm.sqadto[7] = (unsigned short )0;
         sqlstm.sqtdso[7] = (unsigned short )0;
         sqlstm.sqhstv[8] = (unsigned char  *)loimporte;
         sqlstm.sqhstl[8] = (unsigned long )13;
         sqlstm.sqhsts[8] = (         int  )0;
         sqlstm.sqindv[8] = (         short *)0;
         sqlstm.sqinds[8] = (         int  )0;
         sqlstm.sqharm[8] = (unsigned long )0;
         sqlstm.sqadto[8] = (unsigned short )0;
         sqlstm.sqtdso[8] = (unsigned short )0;
         sqlstm.sqhstv[9] = (unsigned char  *)formatoI;
         sqlstm.sqhstl[9] = (unsigned long )4000;
         sqlstm.sqhsts[9] = (         int  )0;
         sqlstm.sqindv[9] = (         short *)0;
         sqlstm.sqinds[9] = (         int  )0;
         sqlstm.sqharm[9] = (unsigned long )0;
         sqlstm.sqadto[9] = (unsigned short )0;
         sqlstm.sqtdso[9] = (unsigned short )0;
         sqlstm.sqhstv[10] = (unsigned char  *)losesion;
         sqlstm.sqhstl[10] = (unsigned long )4;
         sqlstm.sqhsts[10] = (         int  )0;
         sqlstm.sqindv[10] = (         short *)0;
         sqlstm.sqinds[10] = (         int  )0;
         sqlstm.sqharm[10] = (unsigned long )0;
         sqlstm.sqadto[10] = (unsigned short )0;
         sqlstm.sqtdso[10] = (unsigned short )0;
         sqlstm.sqhstv[11] = (unsigned char  *)lofecses;
         sqlstm.sqhstl[11] = (unsigned long )7;
         sqlstm.sqhsts[11] = (         int  )0;
         sqlstm.sqindv[11] = (         short *)0;
         sqlstm.sqinds[11] = (         int  )0;
         sqlstm.sqharm[11] = (unsigned long )0;
         sqlstm.sqadto[11] = (unsigned short )0;
         sqlstm.sqtdso[11] = (unsigned short )0;
         sqlstm.sqphsv = sqlstm.sqhstv;
         sqlstm.sqphsl = sqlstm.sqhstl;
         sqlstm.sqphss = sqlstm.sqhsts;
         sqlstm.sqpind = sqlstm.sqindv;
         sqlstm.sqpins = sqlstm.sqinds;
         sqlstm.sqparm = sqlstm.sqharm;
         sqlstm.sqparc = sqlstm.sqharc;
         sqlstm.sqpadto = sqlstm.sqadto;
         sqlstm.sqptdso = sqlstm.sqtdso;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
         if (sqlca.sqlcode == 1403) break;
}

 
		 //BIMSF-3087
	     printf("filefun.cpp tras el insert NPTLOGO sqlcode >%d<\n",sqlca.sqlcode);
		 printf("filefun.cpp tras el insert NPTLOGO sqlerrm >%s<\n",sqlca.sqlerrm);
	    // si da error por clave duplicada hacemos update
	    if(sqlca.sqlcode == -1)
	    {
           /* EXEC SQL UPDATE NPTLOGO set        
           datos_forminterno = :formatoI,
           loclavpr          = :loclavpr
           where lofecha0=:fecha0 and lohora00=:hora00  and lonumope=:numope; */ 

{
           struct sqlexd sqlstm;
           sqlstm.sqlvsn = 13;
           sqlstm.arrsiz = 22;
           sqlstm.sqladtp = &sqladt;
           sqlstm.sqltdsp = &sqltds;
           sqlstm.stmt = "update NPTLOGO  set datos_forminterno=:b0,loclavpr\
=:b1 where ((lofecha0=:b2 and lohora00=:b3) and lonumope=:b4)";
           sqlstm.iters = (unsigned int  )1;
           sqlstm.offset = (unsigned int  )1380;
           sqlstm.cud = sqlcud0;
           sqlstm.sqlest = (unsigned char  *)&sqlca;
           sqlstm.sqlety = (unsigned short)4352;
           sqlstm.occurs = (unsigned int  )0;
           sqlstm.sqhstv[0] = (unsigned char  *)formatoI;
           sqlstm.sqhstl[0] = (unsigned long )4000;
           sqlstm.sqhsts[0] = (         int  )0;
           sqlstm.sqindv[0] = (         short *)0;
           sqlstm.sqinds[0] = (         int  )0;
           sqlstm.sqharm[0] = (unsigned long )0;
           sqlstm.sqadto[0] = (unsigned short )0;
           sqlstm.sqtdso[0] = (unsigned short )0;
           sqlstm.sqhstv[1] = (unsigned char  *)loclavpr;
           sqlstm.sqhstl[1] = (unsigned long )36;
           sqlstm.sqhsts[1] = (         int  )0;
           sqlstm.sqindv[1] = (         short *)0;
           sqlstm.sqinds[1] = (         int  )0;
           sqlstm.sqharm[1] = (unsigned long )0;
           sqlstm.sqadto[1] = (unsigned short )0;
           sqlstm.sqtdso[1] = (unsigned short )0;
           sqlstm.sqhstv[2] = (unsigned char  *)fecha0;
           sqlstm.sqhstl[2] = (unsigned long )9;
           sqlstm.sqhsts[2] = (         int  )0;
           sqlstm.sqindv[2] = (         short *)0;
           sqlstm.sqinds[2] = (         int  )0;
           sqlstm.sqharm[2] = (unsigned long )0;
           sqlstm.sqadto[2] = (unsigned short )0;
           sqlstm.sqtdso[2] = (unsigned short )0;
           sqlstm.sqhstv[3] = (unsigned char  *)hora00;
           sqlstm.sqhstl[3] = (unsigned long )7;
           sqlstm.sqhsts[3] = (         int  )0;
           sqlstm.sqindv[3] = (         short *)0;
           sqlstm.sqinds[3] = (         int  )0;
           sqlstm.sqharm[3] = (unsigned long )0;
           sqlstm.sqadto[3] = (unsigned short )0;
           sqlstm.sqtdso[3] = (unsigned short )0;
           sqlstm.sqhstv[4] = (unsigned char  *)numope;
           sqlstm.sqhstl[4] = (unsigned long )7;
           sqlstm.sqhsts[4] = (         int  )0;
           sqlstm.sqindv[4] = (         short *)0;
           sqlstm.sqinds[4] = (         int  )0;
           sqlstm.sqharm[4] = (unsigned long )0;
           sqlstm.sqadto[4] = (unsigned short )0;
           sqlstm.sqtdso[4] = (unsigned short )0;
           sqlstm.sqphsv = sqlstm.sqhstv;
           sqlstm.sqphsl = sqlstm.sqhstl;
           sqlstm.sqphss = sqlstm.sqhsts;
           sqlstm.sqpind = sqlstm.sqindv;
           sqlstm.sqpins = sqlstm.sqinds;
           sqlstm.sqparm = sqlstm.sqharm;
           sqlstm.sqparc = sqlstm.sqharc;
           sqlstm.sqpadto = sqlstm.sqadto;
           sqlstm.sqptdso = sqlstm.sqtdso;
           sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
           if (sqlca.sqlcode == 1403) break;
}

 	   
	    } 
	  }	
      else
      {
         //printf("loclavpr %.35s\n", loclavpr);
         /* EXEC SQL UPDATE NPTLOGO set        
         datos_forminterno = :formatoI,
         loclavpr          = :loclavpr
         where lofecha0=:fecha0 and lohora00=:hora00  and lonumope=:numope; */ 

{
         struct sqlexd sqlstm;
         sqlstm.sqlvsn = 13;
         sqlstm.arrsiz = 22;
         sqlstm.sqladtp = &sqladt;
         sqlstm.sqltdsp = &sqltds;
         sqlstm.stmt = "update NPTLOGO  set datos_forminterno=:b0,loclavpr=:\
b1 where ((lofecha0=:b2 and lohora00=:b3) and lonumope=:b4)";
         sqlstm.iters = (unsigned int  )1;
         sqlstm.offset = (unsigned int  )1415;
         sqlstm.cud = sqlcud0;
         sqlstm.sqlest = (unsigned char  *)&sqlca;
         sqlstm.sqlety = (unsigned short)4352;
         sqlstm.occurs = (unsigned int  )0;
         sqlstm.sqhstv[0] = (unsigned char  *)formatoI;
         sqlstm.sqhstl[0] = (unsigned long )4000;
         sqlstm.sqhsts[0] = (         int  )0;
         sqlstm.sqindv[0] = (         short *)0;
         sqlstm.sqinds[0] = (         int  )0;
         sqlstm.sqharm[0] = (unsigned long )0;
         sqlstm.sqadto[0] = (unsigned short )0;
         sqlstm.sqtdso[0] = (unsigned short )0;
         sqlstm.sqhstv[1] = (unsigned char  *)loclavpr;
         sqlstm.sqhstl[1] = (unsigned long )36;
         sqlstm.sqhsts[1] = (         int  )0;
         sqlstm.sqindv[1] = (         short *)0;
         sqlstm.sqinds[1] = (         int  )0;
         sqlstm.sqharm[1] = (unsigned long )0;
         sqlstm.sqadto[1] = (unsigned short )0;
         sqlstm.sqtdso[1] = (unsigned short )0;
         sqlstm.sqhstv[2] = (unsigned char  *)fecha0;
         sqlstm.sqhstl[2] = (unsigned long )9;
         sqlstm.sqhsts[2] = (         int  )0;
         sqlstm.sqindv[2] = (         short *)0;
         sqlstm.sqinds[2] = (         int  )0;
         sqlstm.sqharm[2] = (unsigned long )0;
         sqlstm.sqadto[2] = (unsigned short )0;
         sqlstm.sqtdso[2] = (unsigned short )0;
         sqlstm.sqhstv[3] = (unsigned char  *)hora00;
         sqlstm.sqhstl[3] = (unsigned long )7;
         sqlstm.sqhsts[3] = (         int  )0;
         sqlstm.sqindv[3] = (         short *)0;
         sqlstm.sqinds[3] = (         int  )0;
         sqlstm.sqharm[3] = (unsigned long )0;
         sqlstm.sqadto[3] = (unsigned short )0;
         sqlstm.sqtdso[3] = (unsigned short )0;
         sqlstm.sqhstv[4] = (unsigned char  *)numope;
         sqlstm.sqhstl[4] = (unsigned long )7;
         sqlstm.sqhsts[4] = (         int  )0;
         sqlstm.sqindv[4] = (         short *)0;
         sqlstm.sqinds[4] = (         int  )0;
         sqlstm.sqharm[4] = (unsigned long )0;
         sqlstm.sqadto[4] = (unsigned short )0;
         sqlstm.sqtdso[4] = (unsigned short )0;
         sqlstm.sqphsv = sqlstm.sqhstv;
         sqlstm.sqphsl = sqlstm.sqhstl;
         sqlstm.sqphss = sqlstm.sqhsts;
         sqlstm.sqpind = sqlstm.sqindv;
         sqlstm.sqpins = sqlstm.sqinds;
         sqlstm.sqparm = sqlstm.sqharm;
         sqlstm.sqparc = sqlstm.sqharc;
         sqlstm.sqpadto = sqlstm.sqadto;
         sqlstm.sqptdso = sqlstm.sqtdso;
         sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
         if (sqlca.sqlcode == 1403) break;
}

 
      }
	}
 // * añadido para PRICE *//
 
    errSQL = sqlca.sqlcode;


    return(errSQL);
}

//* añadido para PRICE *//
int InsertarDF(void* registro)
{
    int cont=0;
    int errSQL=0;


    QuitarCaracteres(registro,sizeof(NPSDIFE));  

    strncpy(subsisF, registro, 2);
    subsisF[2]='\0';
    strncpy(subapl, (char*)registro+2, 2);
    subapl[2]='\0';
    strncpy(fecha0, (char*)registro+4, 8);
    fecha0[8]='\0';
    strncpy(hora00, (char*)registro+12, 6);
    hora00[6]='\0';
    strncpy(numope, (char*)registro+18, 6); 
    numope[6]='\0';
    strncpy(formatoI, (char*)registro+24, sizeof(formatoI)-1);
    formatoI[sizeof(formatoI)-1]='\0';
    strncpy(loclavpr, (char*)registro + 24 + sizeof(formatoI)-1, 35);
    loclavpr[35]='\0';
	

    while (cont==0)
    {
    		printf("Hace el insert de NPTDIFE\n");
        /* EXEC SQL INSERT INTO NPTDIFE VALUES (:subsisF, :subapl, :fecha0, :hora00, :numope, :formatoI, :loclavpr); */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 13;
        sqlstm.arrsiz = 22;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "insert into NPTDIFE  values (:b0,:b1,:b2,:b3,:b4,:b5,\
:b6)";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1450;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (unsigned char  *)subsisF;
        sqlstm.sqhstl[0] = (unsigned long )3;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (unsigned char  *)subapl;
        sqlstm.sqhstl[1] = (unsigned long )3;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         short *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned long )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (unsigned char  *)fecha0;
        sqlstm.sqhstl[2] = (unsigned long )9;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         short *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned long )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (unsigned char  *)hora00;
        sqlstm.sqhstl[3] = (unsigned long )7;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         short *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned long )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (unsigned char  *)numope;
        sqlstm.sqhstl[4] = (unsigned long )7;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         short *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned long )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (unsigned char  *)formatoI;
        sqlstm.sqhstl[5] = (unsigned long )4000;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         short *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned long )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (unsigned char  *)loclavpr;
        sqlstm.sqhstl[6] = (unsigned long )36;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         short *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned long )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode == 1403) break;
}

 
				printf("sqlcode tras el insert NPTDIFE %d\n",sqlca.sqlcode);
				/* EXEC SQL COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 13;
    sqlstm.arrsiz = 22;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1493;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


				printf("sqlcode tras el commit NPTDIFE %d\n",sqlca.sqlcode);
        cont++;
        if (sqlca.sqlcode != 0)
        {
           errSQL = sqlca.sqlcode;
           printf("error sqlcode tras el insert NPTDIFE %d\n",sqlca.sqlcode);
        }
    
    		printf("Hace el insert de NPTERRD\n");
        /* EXEC SQL INSERT INTO NPTERRD VALUES (:subsisF, :subapl, :fecha0, :hora00, :numope, SUBSTR(:formatoI,-35,35), '0000', :formatoI, 'P'); */ 

{
        struct sqlexd sqlstm;
        sqlstm.sqlvsn = 13;
        sqlstm.arrsiz = 22;
        sqlstm.sqladtp = &sqladt;
        sqlstm.sqltdsp = &sqltds;
        sqlstm.stmt = "insert into NPTERRD  values (:b0,:b1,:b2,:b3,:b4,SUBS\
TR(:b5,(-35),35),'0000',:b5,'P')";
        sqlstm.iters = (unsigned int  )1;
        sqlstm.offset = (unsigned int  )1508;
        sqlstm.cud = sqlcud0;
        sqlstm.sqlest = (unsigned char  *)&sqlca;
        sqlstm.sqlety = (unsigned short)4352;
        sqlstm.occurs = (unsigned int  )0;
        sqlstm.sqhstv[0] = (unsigned char  *)subsisF;
        sqlstm.sqhstl[0] = (unsigned long )3;
        sqlstm.sqhsts[0] = (         int  )0;
        sqlstm.sqindv[0] = (         short *)0;
        sqlstm.sqinds[0] = (         int  )0;
        sqlstm.sqharm[0] = (unsigned long )0;
        sqlstm.sqadto[0] = (unsigned short )0;
        sqlstm.sqtdso[0] = (unsigned short )0;
        sqlstm.sqhstv[1] = (unsigned char  *)subapl;
        sqlstm.sqhstl[1] = (unsigned long )3;
        sqlstm.sqhsts[1] = (         int  )0;
        sqlstm.sqindv[1] = (         short *)0;
        sqlstm.sqinds[1] = (         int  )0;
        sqlstm.sqharm[1] = (unsigned long )0;
        sqlstm.sqadto[1] = (unsigned short )0;
        sqlstm.sqtdso[1] = (unsigned short )0;
        sqlstm.sqhstv[2] = (unsigned char  *)fecha0;
        sqlstm.sqhstl[2] = (unsigned long )9;
        sqlstm.sqhsts[2] = (         int  )0;
        sqlstm.sqindv[2] = (         short *)0;
        sqlstm.sqinds[2] = (         int  )0;
        sqlstm.sqharm[2] = (unsigned long )0;
        sqlstm.sqadto[2] = (unsigned short )0;
        sqlstm.sqtdso[2] = (unsigned short )0;
        sqlstm.sqhstv[3] = (unsigned char  *)hora00;
        sqlstm.sqhstl[3] = (unsigned long )7;
        sqlstm.sqhsts[3] = (         int  )0;
        sqlstm.sqindv[3] = (         short *)0;
        sqlstm.sqinds[3] = (         int  )0;
        sqlstm.sqharm[3] = (unsigned long )0;
        sqlstm.sqadto[3] = (unsigned short )0;
        sqlstm.sqtdso[3] = (unsigned short )0;
        sqlstm.sqhstv[4] = (unsigned char  *)numope;
        sqlstm.sqhstl[4] = (unsigned long )7;
        sqlstm.sqhsts[4] = (         int  )0;
        sqlstm.sqindv[4] = (         short *)0;
        sqlstm.sqinds[4] = (         int  )0;
        sqlstm.sqharm[4] = (unsigned long )0;
        sqlstm.sqadto[4] = (unsigned short )0;
        sqlstm.sqtdso[4] = (unsigned short )0;
        sqlstm.sqhstv[5] = (unsigned char  *)formatoI;
        sqlstm.sqhstl[5] = (unsigned long )4000;
        sqlstm.sqhsts[5] = (         int  )0;
        sqlstm.sqindv[5] = (         short *)0;
        sqlstm.sqinds[5] = (         int  )0;
        sqlstm.sqharm[5] = (unsigned long )0;
        sqlstm.sqadto[5] = (unsigned short )0;
        sqlstm.sqtdso[5] = (unsigned short )0;
        sqlstm.sqhstv[6] = (unsigned char  *)formatoI;
        sqlstm.sqhstl[6] = (unsigned long )4000;
        sqlstm.sqhsts[6] = (         int  )0;
        sqlstm.sqindv[6] = (         short *)0;
        sqlstm.sqinds[6] = (         int  )0;
        sqlstm.sqharm[6] = (unsigned long )0;
        sqlstm.sqadto[6] = (unsigned short )0;
        sqlstm.sqtdso[6] = (unsigned short )0;
        sqlstm.sqphsv = sqlstm.sqhstv;
        sqlstm.sqphsl = sqlstm.sqhstl;
        sqlstm.sqphss = sqlstm.sqhsts;
        sqlstm.sqpind = sqlstm.sqindv;
        sqlstm.sqpins = sqlstm.sqinds;
        sqlstm.sqparm = sqlstm.sqharm;
        sqlstm.sqparc = sqlstm.sqharc;
        sqlstm.sqpadto = sqlstm.sqadto;
        sqlstm.sqptdso = sqlstm.sqtdso;
        sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
        if (sqlca.sqlcode == 1403) break;
}

 
				printf("sqlcode tras el insert NPTERRD %d\n",sqlca.sqlcode);
				/* EXEC SQL COMMIT; */ 

{
    struct sqlexd sqlstm;
    sqlstm.sqlvsn = 13;
    sqlstm.arrsiz = 22;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )1551;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)4352;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


				printf("sqlcode tras el commit NPTERRD %d\n",sqlca.sqlcode);
        cont++;
        if (sqlca.sqlcode != 0)
        {
           errSQL = sqlca.sqlcode;
           printf("error sqlcode tras el insert NPTERRD %d\n",sqlca.sqlcode);
        }
    }

    return(errSQL);
}

int InsertarD(void* registro)
{
    int cont=0;
    int errSQL=0;

    QuitarCaracteres(registro,sizeof(NPSDSCD));  

    strncpy(subsisF, registro, 2);
    subsisF[2]='\0';
    strncpy(subapl, (char*)registro+2, 2);
    subapl[2]='\0';
    strncpy(fecha0, (char*)registro+4, 8);
    fecha0[8]='\0';
    strncpy(hora00, (char*)registro+12, 6);
    hora00[6]='\0';
    strncpy(numope, (char*)registro+18, 6);
    numope[6]='\0';
    strncpy(formatoI, (char*)registro+24, sizeof(formatoI)-1);
    formatoI[sizeof(formatoI)-1]='\0';

    while (cont==0)
    {
       /* EXEC SQL INSERT INTO NPTDSCD VALUES (:subsisF, :subapl, :fecha0, :hora00, :numope, :formatoI); */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 13;
       sqlstm.arrsiz = 22;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "insert into NPTDSCD  values (:b0,:b1,:b2,:b3,:b4,:b5)";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )1566;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)subsisF;
       sqlstm.sqhstl[0] = (unsigned long )3;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)subapl;
       sqlstm.sqhstl[1] = (unsigned long )3;
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqhstv[2] = (unsigned char  *)fecha0;
       sqlstm.sqhstl[2] = (unsigned long )9;
       sqlstm.sqhsts[2] = (         int  )0;
       sqlstm.sqindv[2] = (         short *)0;
       sqlstm.sqinds[2] = (         int  )0;
       sqlstm.sqharm[2] = (unsigned long )0;
       sqlstm.sqadto[2] = (unsigned short )0;
       sqlstm.sqtdso[2] = (unsigned short )0;
       sqlstm.sqhstv[3] = (unsigned char  *)hora00;
       sqlstm.sqhstl[3] = (unsigned long )7;
       sqlstm.sqhsts[3] = (         int  )0;
       sqlstm.sqindv[3] = (         short *)0;
       sqlstm.sqinds[3] = (         int  )0;
       sqlstm.sqharm[3] = (unsigned long )0;
       sqlstm.sqadto[3] = (unsigned short )0;
       sqlstm.sqtdso[3] = (unsigned short )0;
       sqlstm.sqhstv[4] = (unsigned char  *)numope;
       sqlstm.sqhstl[4] = (unsigned long )7;
       sqlstm.sqhsts[4] = (         int  )0;
       sqlstm.sqindv[4] = (         short *)0;
       sqlstm.sqinds[4] = (         int  )0;
       sqlstm.sqharm[4] = (unsigned long )0;
       sqlstm.sqadto[4] = (unsigned short )0;
       sqlstm.sqtdso[4] = (unsigned short )0;
       sqlstm.sqhstv[5] = (unsigned char  *)formatoI;
       sqlstm.sqhstl[5] = (unsigned long )4000;
       sqlstm.sqhsts[5] = (         int  )0;
       sqlstm.sqindv[5] = (         short *)0;
       sqlstm.sqinds[5] = (         int  )0;
       sqlstm.sqharm[5] = (unsigned long )0;
       sqlstm.sqadto[5] = (unsigned short )0;
       sqlstm.sqtdso[5] = (unsigned short )0;
       sqlstm.sqphsv = sqlstm.sqhstv;
       sqlstm.sqphsl = sqlstm.sqhstl;
       sqlstm.sqphss = sqlstm.sqhsts;
       sqlstm.sqpind = sqlstm.sqindv;
       sqlstm.sqpins = sqlstm.sqinds;
       sqlstm.sqparm = sqlstm.sqharm;
       sqlstm.sqparc = sqlstm.sqharc;
       sqlstm.sqpadto = sqlstm.sqadto;
       sqlstm.sqptdso = sqlstm.sqtdso;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
       if (sqlca.sqlcode == 1403) break;
}

 
        if (sqlca.sqlcode != 0)
        {
           errSQL = sqlca.sqlcode;
           printf("sqlcode tras el insert NPTDSCD %d\n",sqlca.sqlcode);
        }
       cont++;
    }
	
    return(errSQL);
}
//* añadido para PRICE *//

void QuitarCaracteres(char* cadena, int longi)
{
   int i=0;
   for(i=0; i<longi; i++)
   {
      if (  cadena[i] == '\0'
         || cadena[i] < 41
         || cadena[i] == 44
         )
         cadena[i] = ' ';
   }
}

//PRJ288711 - INI
int InsertarDE(void* registro)
{
    int cont=0;
    int errSQL=0;
		int tamano=0;
		char auxAccion[1];
		NPSERRD regdier;
				
		tamano = sizeof(NPSERRD);
		printf("...... Llamada al procedimiento ACTUALIZANPTERRD .-.-.- \n");
		//printf(".... Cadena de entrada registro 1 >%s< con tamano >%d< \n", registro, tamano);
    //QuitarCaracteres(registro, sizeof(NPSERRD));  
		//QuitarCaracteres(registro, tamano);
		//printf(".... Cadena de entrada registro 2 >%s< con tamano >%d< \n", registro, tamano);

		strncpy(auxAccion, registro, 1);
		auxAccion[1] = '\0';
    strncpy(subsisF, registro+1, 2);
    subsisF[2] = '\0';
		strncpy(subapl, registro+3, 2);
		subapl[2] = '\0';
    strncpy(numope, registro+5, 6);
    numope[6] = '\0';
    strncpy(errorn, registro+11, 4);
    errorn[4] = '\0';
    formatoDE[14]='\0';
        
		printf(".... Cadena a procedimiento con subsisF >%.2s< con subapl >%.2s< con numope >%.6s< con errorn >%.4s<\n", subsisF, subapl, numope, errorn);
		strncpy(accion, auxAccion, 1);
		accion[1] = '\0';
		strncpy(formatoDE, subsisF, 2);
		strncpy(formatoDE+2, subapl, 2);
		strncpy(formatoDE+4, numope, 6);
		strncpy(formatoDE+10, errorn, 4);
    formatoDE[14]='\0';
		printf(".... Cadena a procedimiento con accion >%s< con auxAccion >%s< con formatoDE >%.14s<\n", accion, auxAccion, formatoDE);

    while (cont==0)
    {
    		//EXEC SQL CALL ACTUALIZANPTERRD (:accion, :formatoDE, :CodError);
    		if (strcmp(auxAccion,"B")==0)
    		{
    			printf(".... ¿Entra con valor accion B? >%s<\n", auxAccion);
    			/* EXEC SQL CALL ACTUALIZANPTERRD ('B', :formatoDE, :CodError); */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 13;
       sqlstm.arrsiz = 22;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "call ACTUALIZANPTERRD('B',:b0,:b1)";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )1605;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)formatoDE;
       sqlstm.sqhstl[0] = (unsigned long )14;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)CodError;
       sqlstm.sqhstl[1] = (unsigned long )4;
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqphsv = sqlstm.sqhstv;
       sqlstm.sqphsl = sqlstm.sqhstl;
       sqlstm.sqphss = sqlstm.sqhsts;
       sqlstm.sqpind = sqlstm.sqindv;
       sqlstm.sqpins = sqlstm.sqinds;
       sqlstm.sqparm = sqlstm.sqharm;
       sqlstm.sqparc = sqlstm.sqharc;
       sqlstm.sqpadto = sqlstm.sqadto;
       sqlstm.sqptdso = sqlstm.sqtdso;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    		//	EXEC SQL UPDATE NPTERRD SET ERESTADO = 'T', ERNUMERR = LPAD(TRIM(:errorn),4,'0') WHERE ERSUBSIS = :subsisF AND ERSUBAPL = :subapl AND ERNUMOPE = :numope AND ERESTADO = 'P';
    		}
    		if (strcmp(auxAccion,"E")==0)
    		{
    			printf(".... ¿Entra con valor accion E? >%s<\n", auxAccion);
    			/* EXEC SQL CALL ACTUALIZANPTERRD ('E', :formatoDE, :CodError); */ 

{
       struct sqlexd sqlstm;
       sqlstm.sqlvsn = 13;
       sqlstm.arrsiz = 22;
       sqlstm.sqladtp = &sqladt;
       sqlstm.sqltdsp = &sqltds;
       sqlstm.stmt = "call ACTUALIZANPTERRD('E',:b0,:b1)";
       sqlstm.iters = (unsigned int  )1;
       sqlstm.offset = (unsigned int  )1628;
       sqlstm.cud = sqlcud0;
       sqlstm.sqlest = (unsigned char  *)&sqlca;
       sqlstm.sqlety = (unsigned short)4352;
       sqlstm.occurs = (unsigned int  )0;
       sqlstm.sqhstv[0] = (unsigned char  *)formatoDE;
       sqlstm.sqhstl[0] = (unsigned long )14;
       sqlstm.sqhsts[0] = (         int  )0;
       sqlstm.sqindv[0] = (         short *)0;
       sqlstm.sqinds[0] = (         int  )0;
       sqlstm.sqharm[0] = (unsigned long )0;
       sqlstm.sqadto[0] = (unsigned short )0;
       sqlstm.sqtdso[0] = (unsigned short )0;
       sqlstm.sqhstv[1] = (unsigned char  *)CodError;
       sqlstm.sqhstl[1] = (unsigned long )4;
       sqlstm.sqhsts[1] = (         int  )0;
       sqlstm.sqindv[1] = (         short *)0;
       sqlstm.sqinds[1] = (         int  )0;
       sqlstm.sqharm[1] = (unsigned long )0;
       sqlstm.sqadto[1] = (unsigned short )0;
       sqlstm.sqtdso[1] = (unsigned short )0;
       sqlstm.sqphsv = sqlstm.sqhstv;
       sqlstm.sqphsl = sqlstm.sqhstl;
       sqlstm.sqphss = sqlstm.sqhsts;
       sqlstm.sqpind = sqlstm.sqindv;
       sqlstm.sqpins = sqlstm.sqinds;
       sqlstm.sqparm = sqlstm.sqharm;
       sqlstm.sqparc = sqlstm.sqharc;
       sqlstm.sqpadto = sqlstm.sqadto;
       sqlstm.sqptdso = sqlstm.sqtdso;
       sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


    		//	EXEC SQL UPDATE NPTERRD SET ERESTADO = 'E', ERNUMERR = LPAD(TRIM(:errorn),4,'0') WHERE ERSUBSIS = :subsisF AND ERSUBAPL = :subapl AND ERNUMOPE = :numope AND ERESTADO = 'P';
    		}
 
				//printf("---.- CodError salida ACTUALIZANPTERRD %.3s\n", CodError);
        cont++;
        if (sqlca.sqlcode != 0)
        {
           errSQL = sqlca.sqlcode;
           printf(".-.-.-.-.sqlcode tras el insert NPTERRD >%d<\n", sqlca.sqlcode);
        }
        else
        {
        	printf(".-.-.-.-.sale con error 0 del procedimiento >%d<\n", sqlca.sqlcode);
        }
    }

    return(errSQL);
}
//PRJ288711 - FIN

