
/********************
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//#include "cnetplus.h"
//#include "svm.h"
//#include "Cola_comun.h"
//#include "msgclien.h"
//#include "procesos.h"

void fhexdump(FILE * pf, void * vaddr, int len);
void EXPANDECHEQUE(char *Longitud, char *pbyte_exp, char *pbyte_com);

/**************************************************************/
/*********     MAIN                                      ******/
/**************************************************************/
   
int main(int argc, char *argv[])
{
   char strLongitud[7];
   int Longitud;
   char Mensaje[10000];
   char MsgConvertido[10000];
    
   if (argc != 3)
   {
     printf("Uso del programa : ExpandeCheque  <Longitud> <Mensaje>\n");
     exit(0);
   } else {
     strcpy(strLongitud, argv[1]);
     Longitud = atoi(strLongitud);

     memcpy(Mensaje, argv[2], Longitud);
     
     fprintf(stderr,"mensaje enviado==========================================================\n");
     fhexdump(stderr, &Mensaje, Longitud);
     fprintf(stderr,"mensaje enviado==========================================================\n");
     fflush(stderr);
   }

      EXPANDECHEQUE(strLongitud, Mensaje, MsgConvertido);
   return(0);
	
	
}//fin mainnnnn


/**********************************************************************************************************/
void EXPANDECHEQUE(char *strLongitud, char *pbyte_com, char *pbyte_exp)
{
   int PosCom;
   int PosExp;
   int Long;
   typedef struct ochobits
   {  unsigned  int bit_7  :  1;
      unsigned  int bit_6  :  1;
      unsigned  int bit_5  :  1;
      unsigned  int bit_4  :  1;
      unsigned  int bit_3  :  1;
      unsigned  int bit_2  :  1;
      unsigned  int bit_1  :  1;
      unsigned  int bit_0  :  1;
   } ochobits;

   char caracter;
/***   convierte el tipo char de la variable caracter 
       a tipo ochobits mediante la variable workbyte   ***/
   ochobits *workbyte = (ochobits*) &caracter;

   ochobits workbyte2;
/***   convierte el tipo ochobits de la variable workbyte2
       a tipo char  mediante la variable caracter2   ***/
   char *caracter2 = (char*) &workbyte2;

     Long = atoi(strLongitud);
unsigned char  caraprin;

printf("pbyte_com \n");
printf("Longitud <%d> pbyte_com <%s>\n", Long, pbyte_com);
PosCom = 0;
PosExp = 0;
while (1) //for (i=0;i<long;++)
{
    printf("pbyte exp <%d> <%c\n", PosCom,pbyte_com[0+PosCom]);
    caracter = pbyte_com [PosCom];
   
    fprintf(stdout, "caracter <%d>\n", caracter);
    pbyte_exp[0 + PosExp *8] = workbyte->bit_0 + '0';
    pbyte_exp[1 + PosExp *8] = workbyte->bit_1 + '0';
    pbyte_exp[2 + PosExp *8] = workbyte->bit_2 + '0';
    pbyte_exp[3 + PosExp *8] = workbyte->bit_3 + '0';
    pbyte_exp[4 + PosExp *8] = workbyte->bit_4 + '0';
    pbyte_exp[5 + PosExp *8] = workbyte->bit_5 + '0';
    pbyte_exp[6 + PosExp *8] = workbyte->bit_6 + '0';
    pbyte_exp[7 + PosExp *8] = workbyte->bit_7 + '0';
    fprintf(stdout, "Mensaje expandido <%c>", pbyte_exp[0] + PosExp *8);
    fprintf(stdout, "<%c>", pbyte_exp[1 + PosExp *8]);
    fprintf(stdout, "<%c>", pbyte_exp[2 + PosExp *8]);
    fprintf(stdout, "<%c>", pbyte_exp[3 + PosExp *8]);
    fprintf(stdout, "<%c>", pbyte_exp[4 + PosExp *8]);
    fprintf(stdout, "<%c>", pbyte_exp[5 + PosExp *8]);
    fprintf(stdout, "<%c>", pbyte_exp[6 + PosExp *8]);
    fprintf(stdout, "<%c>\n", pbyte_exp[7 + PosExp *8]);

	  PosExp ++;
	  PosCom ++; 
	  if (PosCom >= Long)
	     break;
}

fhexdump(stdout, pbyte_exp, PosExp * 8);

 return;
}

/**********************************************************************************************************/


void fhexdump(FILE * pf, void * vaddr, int len)
{
 int ii,nn;
 unsigned char cc;
 char linea[79];
 char * addr;
 int offset;

 addr = vaddr;
 offset = 0;
 nn=0;

 while(nn<len)
 {
  cc= *(addr+nn);
  sprintf(linea, "%7d  >%02X", nn, cc);
  offset+=12;
  for (ii=1; ii < 16; ii++)
  {
   if (nn+ii < len)
   {
    cc = *(addr+nn+ii);
    sprintf(linea+offset, "_%02X", cc);
   }
   else
    sprintf(linea+offset, "   ");
   offset+=3;
  }
  sprintf(linea+offset, "<");
  offset++;
  sprintf(linea+offset, "  >");
  offset+=3;
  for (ii=0; ii < 16; ii++)
  {
   if (nn+ii < len)
   {
    cc = *(addr+nn+ii);
    if( (cc >=32) && (cc<=126) )
     sprintf(linea+offset, "%c", cc);
    else if(!cc)
     sprintf(linea+offset, "~");
    else
     sprintf(linea+offset, "."); 
   }
   else
    sprintf(linea+offset, " "); 
   offset++;
  }
  *(linea+offset) = '<';


  fprintf(pf, "%.*s\n", 78, linea);  
  
  offset = 0;
  nn += 16;
 }
}
