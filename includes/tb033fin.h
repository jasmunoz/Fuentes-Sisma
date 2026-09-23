/*****************************************************************************
 * 
 *        TB033FIN.H      
 *
 *   Tabla para pasar de las respuestas de Cardplus en "respusal" a "ficodfin"
 *
 *****************************************************************************/

typedef struct tabla_033fin
{
   int tab_033;
   int tab_fin;
} tabla_033fin;

tabla_033fin  datos_033fin[ ] = {
                            {12,280},
                            {51,229},
                            {61,232},
                            {56,234},
                            {67,239},
                            {72,322},
                            {73,122},
                            {76,261},
                            {77,917},
                            {78,323},
                            {79,314},
                            {82,290},

        /***   la ultima linea siempre debera llevar {0,261},   ***/
                            {0,261},
                             };
