#ifndef COPIACAM_H
#define COPIACAM_H

/*** Movimiento de campos ****/ 
#define AaA 101         /**** Alfanumerico a Alfanumerico ****/ 
#define AaE 103         /**** Alfanumerico a Empaquetado  ****/
#define NaE 203         /**** Numerico a Empaquetado      ****/
#define EaA 301         /**** Empaquetado a Alfanumerico  ****/  
#define EaN 302         /**** Empaquetado a Numerico      ****/

miint fempapin(miint longi, michar *sempaq, michar *sdesem);
miint fdesepin(miint longi, michar *sempaq, michar *sdesem);
miint fempaque(miint longi, michar *sempaq, michar *sdesem);
miint fdesempa(miint longi, michar *sempaq, michar *sdesem);

#endif
