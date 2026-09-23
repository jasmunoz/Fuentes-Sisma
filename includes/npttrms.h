#ifndef NPTTRMS_H
#define NPTTRMS_H
/*****************************************************************************
 *
 *
 *      (C) INDRA GROUP   Julio, 2026.   
 *
 *              <npttrms.h>            Longitud = 28
 *
 *****************************************************************************/


/*    Registro del fichero de Codigos de Proceso        */
/*    Se creo para traducir los codigos de price a sio  */


typedef   struct {
    char            trmsemis[3];        /* Código del emisor */
    char            trmsidtm[5];        /* Código de mensaje PRICE */
    char            trmscopr[7];        /* Código de proceso PRICE */
    char            trmsidtv[5];        /* Código de mensaje PRICE de vuelta*/
    char            trmsidsi[5];        /* Código de mensaje SIO */
    char            trmscosi[7];        /* Código de proceso SIO */
    char						trmsoper[2];				/* Tipo de operativa B = Bizum, T = Tarjeta */ //PRJ0387060
} NPSTRMS;

NPSTRMS tabla_trms [500];

#endif
/* NPTTRMS_H */
