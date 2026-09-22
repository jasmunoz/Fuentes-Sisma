#define movmem(src, dst, count) memmove ((dst), (src), (count))

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "deffich.h"
#include "cforitno.h"

#ifdef SEPARA
#endif

extern char nomProg[50]; 

void fprintLog(char ficheroPlantilla[50], char* format, ...);

/*****************************************************************************
*  - Obtiene el numero de operacion del fichero de Parametros del Sistema    *
******************************************************************************/

int fnumope (char *numope, char * numcross, char Indice)
{
  typedef struct CruceOper
  {
    char numoper[10];
    char numcruce[10];
  }CruceOper;

  int cod_err;
  char clave[1];
  
 CruceOper registro;  
 cod_err = 0;

 //Recuperamos el número de cruce y el de operación de la tabla

 clave[0]=Indice;
 clave[1]='\0';
//11.feb.11 Quitar trazas de Sisma
// fprintLog(nomProg, "Indice de la tabla para el numero de operacion: %s\n",clave);
//11.feb.11 Fin
 if (clave[0] != '1' && clave[0] != '2' && clave [0] != '3' && clave [0] != '4' && clave [0] != '5' &&
     clave[0] != '6' && clave[0] != '7' && clave [0] != '8' && clave [0] != '9' && clave [0] != '0')
 {
        clave[0]='0';
        clave[1]='\0';
        fprintLog(nomProg, "nuevo indice de la tabla para el numero de operacion: %s\n",clave);
 }
 
 cod_err=tratar_fich("NumeroCruceYOperacion", REESCRIBIR, &registro, clave);
 
 if(!cod_err)
 {
   strncpy(numope, registro.numoper+4, 6);
  //strncpy(numcross, registro.numcruce,10);
 
  //fprintLog(nomProg, "numcross es : >%.10s<\n",numcross);
  numope[6]='\0';
  fprintLog(nomProg, "numope >%.6s<\n",numope);  
 }
 else
  fprintLog(nomProg, "--->>>error al leer  %d\n",cod_err);
 
 return cod_err;
}

int fjuliana(fecha *fechag, juliana *fechaj)
  {

  int resto,sumagj;
  int total;
  int dias[]={0,31,59,90,120,151,181,212,243,273,304,334};

  total=fechag->aa+100*fechag->ss;
  if ((total % 4 == 0) && (total % 100 != 0) || (total % 400 == 0))
     resto = 0;
  else
     resto = 1;

  if ((fechag->mm > 12) || (fechag->mm < 1))
     return(1);

  switch (fechag->mm)
    {
    case 4:
    case 6:
    case 9:
    case 11:
            if (fechag->dd > 30)
               return(3);
            break;
    case 2:
               if (fechag->dd > 29)
                  return(3);
               if (resto && fechag->dd > 28)
                  return(3);
    default:
            if (fechag->dd > 31)
               return(3);
    }

  fechaj->anio = total;
  fechaj->dia  = fechag->dd;
  sumagj       = dias [fechag->mm - 1]; 

  if ((resto == 0) && (fechag->mm > 2))
     fechaj->dia  += (1 + sumagj);
  else
     fechaj->dia += sumagj;

  if (resto == 0)
     return(2);
  else
     return(0);

  }
