#ifndef CFORMONI_H   
#define CFORMONI_H                    

 /****************************************************************/
 /* formato de mensaje en cola de rmonitor*/    
 /****************************************************************/

#define parte_032 6
#define Ini_Origen '8'
 typedef struct
 {
    char          fiorigen[3]; /* origen */  
    char          fisubori[3]; /* subaplicacion origen */  
    char          firesfin[3]; /* resolutor final */
    char          firespri[3]; /* resolutor primario */
    char          ficodfin[4]; /* codigo de finalicacion */
    char          fisubres[3];  /* subaplicacion resolutora */
    long          ficomisi;     /* imp. comision a aplicar */
    int           fipidori;         /* PID del proceso origen */
    char          ent_origen[TAM_RESO];
 } mn_datos_procesales;


typedef struct
{
    char          fiiso000[5];
    char          fiiso024[4];  /* codigo de funcion  ISO-24 */
 } mn_iso_comunes;


 typedef struct
 { 
    iso_002            fiaut002;  //PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño total de la tarjeta
    iso_003            fiaut003;
    double           fiaut006;
    fecha_6            fiaut028;
    char             fiaut029[4]; 
    char   resolutor[TAM_RESO];
    char   ClCuadre[TAM_RESO];
    char   IdCuadre;      /* Identificativo de cuadre      */
    fecha_6            or_fiaut028;
    char             or_fiaut029[4];
 } mn_iso_autorizaciones;

typedef union
{
 mn_iso_autorizaciones  fiisoaut;
} mn_resto_iso;


 typedef struct
 {
   mn_datos_procesales fidatpro;
   mn_iso_comunes      fiisocom;
   mn_resto_iso        fidatiso; 
 } mn_formato_interno;

typedef struct
{
   long mntipo;
   mn_formato_interno mnforitn;
} menmoni;

#endif

