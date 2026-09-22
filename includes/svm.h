#ifndef SVM_H
#define SVM_H
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "cforitno.h"
#include "svmbines.h"


#define CIERRE        'C'
#define NOUSO         '0'
#define PARADA        'P'
#define TRABAJO       'T'
#define RECARGAR      'R'
#define ESPERA        'W'
#define CAIDA_LINEA   'X'

/* Para buscar procesos por diferentes criterios */
#define B_NOMBRE    1
#define B_COLA      2
#define B_PID       3

#define TIPO_PARADA   1
#define PID_PDTE      -1963
#define E_NOSVM       1  /* Error al conseguir la SVM */
#define E_CONSVM      2  /* Error al conectar a la SVM */
#define E_NOKEY       3  /* Error al conseguir la clave */
#define E_NOIPC       4  /* Error no existe la referencia a la IPC */
#define NUM_SUBSIS    60
#define NUM_PROCES    400
#define MAX_REG_SVM   ((NUM_SUBSIS+NUM_PROCES)*10)
#define TAM_SVM       ((MAX_REG_SVM+1)*sizeof(SVM))
#define TIP_SUPER     'S'
#define NO_TIPO       '0'
#define NO_TIPO_INT    NO_TIPO - '0'

#define TIP_SUBSIS    '1'
#define TIP_SUBSIS_INT TIP_SUBSIS - '0'

#define TIP_PROCES    '2'
#define TIP_PROCES_INT TIP_PROCES - '0'

#define TIP_BLOQ      '3'

#define SEM_ABIERTO   1

#define NUM_TIPOS     TIP_PROCES_INT

#define CREAR_IPC     IPC_CREAT|0666   // para el servidor
#define CONEC_IPC     0666   // para un cliente
#define NO_EXIS_SVM   -32000
#define NO_EXIS_CONF  -32001

//#define TIMEWAIT      8
#define TIMEWAIT      10
#define MAXCODS 15

#define MAX_REG_TBBINE   5000    //Comprobado que no afecta para PRJ0202598 cambio bin de 6 posiciones 8
#define TAM_TBBINE       ((MAX_REG_TBBINE + 1)*sizeof(tbbine)) //Comprobado que no afecta para PRJ0202598 cambio bin de 6 posiciones 8


/* tabla que estara en svm  **/
typedef struct
{
   time_t t_exp;
   long   numope;
} SVMtimer;
/**** tabla en svm  **/
typedef struct
{
   pid_t  pid;
   fecha  farran;
   hora   harran;
   char   modocs;
   char   ACsvmtibi;
   char   ACsvmbine;
   char   ACsvmreso;
   char   ACsvmsock;
   short  numses;
   int  timesp;
   int  IDsvmbine;    //Comprobado que no afecta para PRJ0202598 cambio bin de 6 posiciones 8
   int  IDsvmsock;
   int  NumBines;     //Comprobado que no afecta para PRJ0202598 cambio bin de 6 posiciones 8
} SVMsuper;

typedef struct
{
    int  pasemsub;
    char  paestado[2];
    int  patrepet;
    int  patrerep;
    int  patresnf;
    char panip[15];
    char paportnume[5];
//  char pacipres[13];
    char pacidest[13];
//  char paclamac[2];
//  char paetiMAC[10];
//  char paalgcif[2];
//  char pausarEMV[2];
//  char papuerto[20];
    char paciadqui[13];
    char paidred[4];
//  char pausconf[1];
    char pafecses[6];
    char panumses[3];
} SVMpars;

typedef struct
{
    pid_t  pid;
    char proceso[11];
    int  idcolaIn;
    int  idcolaOut;
    int  Traza;
} SVMproc;

typedef union
 {
      SVMsuper  svmsupe;
      SVMpars   svmpars;
      SVMproc   svmproc;
 } unionSVM;

typedef struct
 {
    char    svmtip; // posibles valores: TIP_SUPER (supervisor), TIP_PROCES(2), TIP_SUBSIS(1), NO_TIPO
    char    estado; //parada, espera, trabajo, nouso,
    char    tippar; //Como parar el proceso: Q=cola, S=svm, N=no parar
    int     senal;
    char    pasubsis[5];
    unionSVM  str_svm;    //campos propios segun el tipo que sea 
    SVMtimer  tb_timer03; //tiempos segun el subsistema
    SVMtimer  tb_timer05;
    SVMtimer  tb_timer07;
	SVMtimer  tb_timer10;
    SVMtimer  tb_timer30;
    SVMtimer  tb_timer20;
    SVMtimer  tb_timer40;
 } SVM;

#ifdef SVMTBOP
typedef struct dtbop ttbop;
struct dtbop
{
   char     clave [14];
   char     funcion[20];
   ttbop      *siguiente;
};

typedef struct
{
   char     clave [14];
   char     funcion[20];
} SVMtbop;
#endif /* SVMTBOP */

#define AcSvmBine (psvm->str_svm.svmsupe.ACsvmbine)
#define AcSvmSock (psvm->str_svm.svmsupe.ACsvmsock)
#define IdSvmBine (psvm->str_svm.svmsupe.IDsvmbine)
#define IdSvmSock (psvm->str_svm.svmsupe.IDsvmsock)
#define SESION    (psvm->str_svm.svmsupe.numses)
#define NBinesSvm   str_svm.svmsupe.NumBines
#define NBinTieSvm  str_svm.svmsupe.NumBinTie

#endif
