#ifndef SVMBINES_H
#define SVMBINES_H

typedef struct tbtibi_bin tbtibi2; 
struct tbtibi_bin
{
   char     bincodent[4];
   // PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN y cambia el tamaño total de tbtibi_bin
   char     bin000[TAM_BIN]; 
   char     tiptar[TAM_TIPTAR];
   char     tboper[2];
   char     parau1[2];
   char     respri[8];
   int      limoff;
   tbtibi2   *sigbin;
};

typedef struct
{
   char     tipo; 
   tbtibi2    b;
} tbtibi3;


typedef struct
{
   char     tienda[TAM_ESTA];
   char     termin[TAM_TERM];
   char     tiptar[TAM_TIPTAR];
   char     tboper[2];
   char     parau1[2];
   char     codres[TAM_RESO];
   int      limoff;
   float    comisi;
   int      NumResBala;
   char     TB_RESOBALA[MAX_RES_BALA+1][TAM_RESO];
} tbtibi;

typedef struct
{
   // PRJ0202598 cambio bin de 6 posiciones 8 se cambia TAM_BIN y cambia el tamaño total de tbbine
   char     ficbin[TAM_BIN];  
   char     birnginf[11]; 
   char     birngsup[11];
   char     fictar[TAM_TIPTAR];
   char     idcuad[1];
} tbbine;

#endif
