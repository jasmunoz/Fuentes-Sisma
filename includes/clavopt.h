#ifndef CLAVOPT_H
#define CLAVOPT_H
/*****************************************************************************
 *
 *
 *      (C) ERITEL, S.A   Septiembre, 1994. NO SE USA
 *    
 *              <clavopt.h>
 *
 *      Contiene las claves de operatividad asociadas a la tarjeta
 *      empleadas en la aplicacion con su correspondiente literal
 *
 *****************************************************************************/


char   *   clave_opt[ ][2] = {
                             {"AA ","Tarjeta operativa         "},
                             {"F1 ","Lista negra.Rob. o perd. C"},
                             {"F2 ","Lista negra.Rob. o perd C."},
                             {"F3 ","Lista negra.Rob. Activam. "},
                             {"F6 ","Lista negra.Falsificada  C"},
                             {"ND ","Lista gris DENEGAR        "},
                             {"AI ","Solicitar D.N.I.          "},
                             {"NC ","Cliente Moroso. Capturar  "},
                             {"NM ","Cliente Moroso. RAI. CAPT "},
                             {"N9 ","Cliente Moroso.NO RAI.CAPT"},
                             {NULL,NULL                         },
                             };


#endif
