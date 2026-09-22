
#include "cforitno.h" 

 typedef struct NPSDSCD
 {
   char   ddsubsis[2];     /*  Subsistema                          */
   char   ddsubapl[2];     /*  Subaplicacion                       */
   char   ddfecha0[8];     /*  Fecha de la operacion               */
   char   ddhora00[6];     /*  Hora de la operacion                */
   char   ddnumope[8];     /*  Numero de la operacion              */
   char   ddmensaj[sizeof(formato_interno)];
 } NPSDSCD;
