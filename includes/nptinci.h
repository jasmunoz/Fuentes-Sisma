#ifndef NPTINCI_H
#define NPTINCI_H
/***********************************************************************
 *
 *
 *      (C) IINDRA S.A   Septiembre, 2019.  
 *     
 *              <nptinci.h>            Longitud =
 *
 ***********************************************************************/

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


typedef struct NPSINIS  // Secuencial para monitorización
{
    char    ilfechin[6];
    char    isfechin[8];    //  Fecha de la incidencia AAAAMMDD
    char    ilhorain[6];
    char    ishorain[9];    //  Hora de la incidencia  HHMMSSCCC
    char    ilnompro[6];
    char    isnompro[8];    //  Programa que grabo la incidencia
    char    ilnomfic[6];
    char    isnomfic[12];   //  Fichero que ocasiono la incidencia
    char    ilcodinc[6];
    char    iscodinc[3];    //  Codigo de incidencia
    char    ilerror0[6];
    char    iserror0[3];    //  Codigo de error
    char    ilnumope[6];
    char    isnumope[6];    // Numero de operación en Netplus (ira en blanco si no es posible)
    char    iliso000[6];
    char    isiso000[4];    // Tipo de mensaje (blancos si no es posible)
    char    ilclavpr[6];
    char    isclavpr[35];   // Clave operación unica, para controlar
                            // Operaciones repetidas (blancos si no es posible)
    char    ilmsginc[6];
    char    ismsginc[50];   // Texto de la incidencia
} NPSINIS;

typedef struct NPSINCP  // Incidencias para grabar en CardPlus ATTHINCI
{
    // longitud registro 169
    char     codent[4];     // 1465
    char     proces[6];     // SISMA
    char     numope[6];     // cod_con_nci
    char     servicio[20];  // cod_tar_nci
    char     coderr[3];     // cod_err_nci
    char     descri[100];   // des_cri_nci
    char     fecinc[8];     // fec_inc_nci
    char     horinc[6];     // hor_inc_nci
    char     estado[1];     // cod_inc_nci
    char     nodoip[15];    // nod_oip_nci
} NPSINCP;


#endif
