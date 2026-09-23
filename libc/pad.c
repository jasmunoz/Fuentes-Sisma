#include <stdlib.h>
#include <string.h>

void rpad(char *bufer,char c,int lon)
{
 int i=0;
     for (i=strlen(bufer);i<lon;i++) bufer[i]=c;
     bufer[i]='\0';
}

void lpad(char *bufer,char c,int lon)
{
 char *bu; 
 int tam,i=0;
 tam=strlen(bufer);
 if (lon>tam)
   {
     bu=(char *) malloc(lon+1);

     for (i=0;i<lon-tam;i++)
        {
           bu[i]=c;  
        }

     for (i=lon-tam;i<lon;i++)
      {
        bu[i]=bufer[i-(lon-tam)];  
      }
     bu[lon]='\0';

     strcpy(bufer, bu);
     free(bu);
   }
}
