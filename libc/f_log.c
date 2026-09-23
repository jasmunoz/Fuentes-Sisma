#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cnetplus.h"

void f_log(FILE * pf, char * msg, ...)
{
    
// incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

  char nom_traza[] = HOMEDIRTRAZA"netplus.traza";
  va_list ap;
  int cerrar = 0;
  //SYSTEMTIME tt;

 if(!pf)
 {
  pf=fopen(nom_traza, "a");  
  cerrar = 1;
 }

 if(pf)
 {
  //GetLocalTime(&tt);

  //fprintf(pf, "FLOG-> %.4d/%.2d/%.2d %.2d:%.2d:%.2d:%.3d ", tt.wYear, tt.wMonth, tt.wDay,
  //           tt.wHour, tt.wMinute, tt.wSecond, tt.wMilliseconds);
  va_start(ap, msg);
   vfprintf(pf, msg, ap);  
  va_end(ap);
  if(msg[strlen(msg)-1] != '\n')
   fprintf(pf, "%c", '\n');
  if(cerrar)
   fclose(pf);
 }
}
