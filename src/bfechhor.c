#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "netwin.h" 
main()
{
 SYSTEMTIME tt;
 char tmpbuf[200];
 FILE * pftmp;
 int dia;

// incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

 sprintf(tmpbuf, "%s%stmpbat.bat", HOME,DIRTEMPO); 
 pftmp = fopen(tmpbuf, "w");
 if(!pftmp)
 {
  printf("error abriendo %s cod->%d<-\n", tmpbuf, errno);
  exit(-1);
 }

 GetLocalTime(&tt);
 fprintf(stdout,"=========%02d/%02d/%02d/ %02d:%02d:%02d:%03d\n",tt.wYear,tt.wMonth,tt.wDay,tt.wHour,tt.wMinute,tt.wSecond,tt.wMilliseconds);

 sprintf(tmpbuf, "%04d%02d%02d", tt.wYear, tt.wMonth, tt.wDay);
 fprintf(pftmp, "set NWDATE=%s\n", tmpbuf);
 dia=(int)tt.wDay-1;
 fprintf(pftmp,"set NWDATE1=%d\n",dia);
 sprintf(tmpbuf, "%02d%02d%02d", tt.wHour, tt.wMinute, tt.wSecond);    
 fprintf(pftmp, "set NWTIME=%s\n", tmpbuf);

 fclose(pftmp);
}