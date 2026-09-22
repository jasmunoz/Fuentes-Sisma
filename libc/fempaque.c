
/***********************************************************************
     FDESEMPA
  Funcion    : desempaqueta una cadena de caracteres empaquetados en BCD
               sobre una cadena de caracteres expandida en codigo ASCII.
               Solo caracteres numericos.

  Parametros : - int con la longitud del dato (la del dato expandido)
               - puntero a char con la cadena a desempaquetar
               - puntero a char con la cadena desempaquetada
**************************************************************************/
#include "cnetplus.h"
#include <stdio.h>
int   fdesepin(int longi, char *sempaq, char *sdesem);  
int   ncampo_numerico (char *c,int n);
int   fempapin(int longi, char *sempaq, char *sdesem);


int fdesempa(int longi,char *sempaq,char *sdesem)
{
  int retorno; 
  int resto;
  int ll;

 resto = longi%2;

 retorno = fdesepin(longi/2 + resto, sempaq, sdesem);
 if(retorno)
  return(retorno);

 if(!ncampo_numerico(sdesem, longi/2 + resto))
  return (-1);

 if(resto)
 {
  for(ll = 0; ll < longi; ll++)
   sdesem[ll] = sdesem[ll + 1];
  sdesem[longi] = ' ';
 }
 return (0);
}


/***********************************************************************
     FEMPAQUE
  Funcion    : empaqueta una cadena de caracteres numericos en ASCII
               sobre una empaquetada con los numeros codificados en BCD
               Solo caracteres numericos.

  Parametros : - int con la longitud del dato (la del dato expandido)
               - puntero a char con la cadena empaquetada
               - puntero a char con la cadena a empaquetar
**************************************************************************/
int fempaque(int longi,char *sempaq,char *sdesem)
{
  int retorno;
  int ll;

 if(!ncampo_numerico(sdesem, longi))
  return(-9);

 retorno = fempapin(longi, sempaq, sdesem);
 if(retorno)
  return(retorno);


 return (0);
}

/****************************************************************************
     FDESEMPAHOST
  Funcion    : desempaqueta una cadena de caracteres empaquetados en BCD,
               (del tamaño que se le indique en el parámetro de entrada)
               sobre una cadena de caracteres expandida del doble de longitud


  Parametros : - puntero a char con la cadena a desempaquetar
               - puntero a char con la cadena desempaquetada
               - integer longitud de la cadena antes de desempaquetar
******************************************************************************/
int fdesempahost(char *sempaq, char *sdesem, int longempaq)
{
    int len;
    int ii,nn;
    unsigned char cc;
    char * addr; 
    int offset;

    offset = 0;
    nn=0;
    len = longempaq; 

    while (nn<len) //desde la posición 0 hasta la len-1
    {
        cc= *(sempaq+nn);
        sprintf(sdesem + offset, "%02X", cc); 
        nn+=1;
        offset+=2;
    }

    return (0);
}
