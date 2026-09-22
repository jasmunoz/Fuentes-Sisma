#include "cnetplus.h"
#include <ctype.h>
void  rtrim(char *bufer,char c);

/*****************************************************************************
 *   Funcion que valida si un campo es numerico                              * 
 *                               devuelve  0 - Campo no numerico             *
 *                                         1 - Campo numerico                *
 *****************************************************************************/
int campo_numerico (char * campo)
{
     int           i = 0;
    
     rtrim(campo, ' ');
     
     for (i=0; i< strlen(campo); i++)
         if (!isdigit((char) campo[i]))
            return(0);

     return(1);
}


/*****************************************************************************
 * Funcion que valida si los n primeros caracteres de un campo son numericos * 
 *                               devuelve  0 - Campo no numerico             *
 *                                         1 - Campo numerico                *
 *****************************************************************************/
int ncampo_numerico (char *c,int n)
{
    while (((*c == '0') || (*c == '1') || (*c == '2') || (*c == '3') ||
            (*c == '4') || (*c == '5') || (*c == '6') || (*c == '7') ||
            (*c == '8') || (*c == '9'))  &&  (n>0))
    {   c++;
        n--;
    };
    if (n>0)
       return(0); 
    else
       return(1);
}

