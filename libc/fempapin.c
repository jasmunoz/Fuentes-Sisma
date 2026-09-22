/***********************************************************************
     FEMPAPIN
  Funcion    : empaqueta una cadena de 16 caracteres alfanumericos, desde
           desde '0' a '9' y desde 'A' a 'F' 
               sobre una empaquetada con la codificacion en BCD

  Parametros : - int con la longitud del dato (la del dato expandido)
               - puntero a char con la cadena empaquetada
               - puntero a char con la cadena a empaquetar
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cnetplus.h"
int fempapin(int longi, char *sempaq, char *sdesem)
{
  int ll;
  char ch_02[2];
  char ch_aux; 

 if(longi%2)
 {
  if(!isxdigit(sdesem[0]))
   return(-7);
  sempaq[0] = '\0';
  if(isdigit(sdesem[0]))
   ch_aux = sdesem[0] - '0';
  else 
   ch_aux = sdesem[0] + 10 - 'A';
  sempaq[0] = sempaq[0] | ch_aux;
  sdesem++;
  sempaq++;
  longi--;
 } 

 for (ll = 0; ll < longi/2; ll++)
 {
  if(!isxdigit(sdesem[2*ll]) || !isxdigit(sdesem[2*ll+1]))
   return(-8);

  if(isdigit(sdesem[ll*2]))
   ch_aux = sdesem[ll*2] - '0';
  else 
   ch_aux = sdesem[ll*2] + 10 - 'A';
  ch_aux = ch_aux << 4;
  ch_aux = ch_aux & 0xF0;
  sempaq[ll] = ch_aux;

  if(isdigit(sdesem[ll*2 + 1]))
   ch_aux = sdesem[ll*2 + 1] - '0';
  else 
   ch_aux = sdesem[ll*2 + 1] + 10 - 'A';

  sempaq[ll] = sempaq[ll] | ch_aux;
 }

 return(0);
}

/***********************************************************************
     FDESEPIN
  Funcion    : desempaqueta una cadena de 8 caracteres empaquetada
               con la codificacion BCD sobre una de 16 caracteres

  Parametros : - int con la longitud del dato (la del dato empaquetado)
               - puntero a michar con la cadena empaquetada
               - puntero a michar con la cadena desempaquetada
**************************************************************************/
int fdesepin(int longi, char *sempaq, char *sdesem)
{
  int ll;
  int ch_aux;

 for(ll = 0; ll < longi; ll++)
 {
  ch_aux = sempaq[ll];

  ch_aux =  ch_aux >> 4;
  ch_aux =  ch_aux & 0x0F;
  if(ch_aux < 10)
   ch_aux = ch_aux + '0';
  else
   ch_aux = ch_aux - 10 + 'A';

  sdesem[2*ll] = ch_aux;

  ch_aux = sempaq[ll];
  ch_aux =  ch_aux & 0x0F;
  if(ch_aux < 10)
   ch_aux = ch_aux + '0';
  else
   ch_aux = ch_aux - 10 + 'A';

  sdesem[2*ll+1] = ch_aux;
 }
 return(0);
}
/***********************************************************************
     FBIN2HEX
  Funcion    : convierte un numero en una cadena de caracteres hexadecimales.

  Parametros : - puntero a char con la cadena
               - int con el valor numerico
               - puntero a int con la longitud de la cadena
**************************************************************************/
int bin2hex(char * pcadena, int numerico, int * longi)
{
  int resto;
  int cociente;
  int dividendo;
  char ch_08[8];
  int nn;
  char digito;

 if(numerico <= 0)
  return(-1);

 memset(ch_08, '0', 8); 
 dividendo = numerico;
 
 for(nn = 1; nn < 9 && dividendo; nn ++)
 {
  resto = dividendo%16;
  cociente = dividendo/16;

  if(resto >= 10)
   digito = resto - 10 + 'A';  
  else
   digito = resto + '0';
  ch_08[8 - nn] = digito;

  dividendo = cociente;
 }

 if(nn > 8)
  nn = 8;

 strncpy(pcadena, ch_08 + 8 - nn, nn);
 *longi = nn;

 return(1);
}
