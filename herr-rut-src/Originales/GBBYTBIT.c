
/********************
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


void fhexdump(FILE * pf, void * vaddr, int len);
void COMPRIMECHEQUE(char *Longitud, char *pbyte_exp, char *pbyte_com);

/** COMPRIMECHEQUE2:
    Longitud: Cadena de 8 posiciones justificada con 8 por la izquierda. Ej: 00004608 
    pbyte_exp: Cadena de entrada. La longitud la marca el campo longitud 
    pbyte_com: Cadena de salida. La longitud deberia ser la del campo londigud entre 8 (pasamos de 1 byte a 1 bit **/
void COMPRIMECHEQUE2 (char *Longitud, char *pbyte_exp, char *pbyte_com);

void COMPRIMECHEQUE(char *strLongitud, char *pbyte_exp, char *pbyte_com)
{
   int PosCom;
   int PosExp;
   int Long=0;
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

   printf("***   **************************   ***\n");   //JFL
   printf("***   GBBYTBIT.c versio 30/08/23   ***\n");   //JFL
   printf("***   **************************   ***\n");   //JFL
   
   printf("pbyte_com \n");
   printf("Longitud strLongitud <%s>\n", strLongitud);
   printf("Longitud <%d> pbyte_exp <%s>\n", Long, pbyte_exp);
   PosCom = 0;
   PosExp = 0;
   while (1) //for (i=0;i<long;++)
   {
      //printf("pbyte com <%d> <%c%c%c%c%c%c%c%c>\n", PosExp,pbyte_exp[0+PosExp*8],pbyte_exp[1+PosExp*8],pbyte_exp [2+PosExp*8],pbyte_exp [3+PosExp*8],pbyte_exp [4+PosExp*8],pbyte_exp [5+PosExp*8],pbyte_exp [6+PosExp*8],pbyte_exp [7+PosExp*8]);
      workbyte2.bit_0 = ((pbyte_exp [0+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_1 = ((pbyte_exp [1+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_2 = ((pbyte_exp [2+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_3 = ((pbyte_exp [3+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_4 = ((pbyte_exp [4+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_5 = ((pbyte_exp [5+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_6 = ((pbyte_exp [6+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_7 = ((pbyte_exp [7+PosExp*8] ) == '1') ? 1 : 0;
   
      //printf("caracter2 <%s>\n",caracter2);   JFL
      //printf("workbyte2 <%u%u%u%u%u%u%u%u>\n",workbyte2.bit_7,workbyte2.bit_6,workbyte2.bit_5,workbyte2.bit_4,workbyte2.bit_3,workbyte2.bit_2,workbyte2.bit_1,workbyte2.bit_0);               //JFL
   	  pbyte_com[PosCom]= *caracter2;
   	  PosExp ++;
   	  PosCom ++; 
   	  if (PosExp *8 >= Long)
   	     break;
   }
   
   printf("antes de fhexdump pbyte_com >%s<\n",pbyte_com); 		//JASM
   fhexdump(stdout, pbyte_com, PosCom);
   printf("despues de fhexdump pbyte_com >%s<\n",pbyte_com); 		//JASM

   printf("\n\n");               //JFL
   return;
}

void COMPRIMECHEQUE2(char *strLongitud, char *pbyte_exp, char *pbyte_com)
{
   int PosCom;
   int PosExp;
   int Long=0;
   typedef struct ochobits
   {  unsigned  int bit_0  :  1;
      unsigned  int bit_1  :  1;
      unsigned  int bit_2  :  1;
      unsigned  int bit_3  :  1;
      unsigned  int bit_4  :  1;
      unsigned  int bit_5  :  1;
      unsigned  int bit_6  :  1;
      unsigned  int bit_7  :  1;
   } ochobits;

   char caracter;
/***   convierte el tipo char de la variable caracter 
       a tipo ochobits mediante la variable workbyte   ***/
   ochobits *workbyte = (ochobits*) &caracter;

   ochobits workbyte2;
/***   convierte el tipo ochobits de la variable workbyte2
       a tipo char  mediante la variable caracter2   ***/
   char *caracter2 = (char*) &workbyte2;
   strLongitud[8]='\0';
   Long = atoi(strLongitud);
   unsigned char  caraprin;

   printf("***   ********************************************   ***\n");   //JFL
   printf("***   GBBYTBIT.c - COMPRIMECHEQUE2 versio 30/08/23   ***\n");   //JFL
   printf("***   ********************************************   ***\n");   //JFL
   
   printf("pbyte_com \n");
   printf("Longitud strLongitud <%s>\n", strLongitud);
   printf("Longitud <%d> pbyte_exp <%s>\n", Long, pbyte_exp);
   PosCom = 0;
   PosExp = 0;
   while (1) //for (i=0;i<long;++)
   {
      //printf("pbyte exp <%d> <%c%c%c%c%c%c%c%c>\n", PosExp,pbyte_exp[0+PosExp*8],pbyte_exp[1+PosExp*8],pbyte_exp [2+PosExp*8],pbyte_exp [3+PosExp*8],pbyte_exp [4+PosExp*8],pbyte_exp [5+PosExp*8],pbyte_exp [6+PosExp*8],pbyte_exp [7+PosExp*8]);
      workbyte2.bit_0 = ((pbyte_exp [0+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_1 = ((pbyte_exp [1+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_2 = ((pbyte_exp [2+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_3 = ((pbyte_exp [3+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_4 = ((pbyte_exp [4+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_5 = ((pbyte_exp [5+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_6 = ((pbyte_exp [6+PosExp*8] ) == '1') ? 1 : 0;
      workbyte2.bit_7 = ((pbyte_exp [7+PosExp*8] ) == '1') ? 1 : 0;
   
      //printf("caracter2 <%s>\n",caracter2);   JFL
      //printf("workbyte2 <%u%u%u%u%u%u%u%u>\n",workbyte2.bit_0,workbyte2.bit_1,workbyte2.bit_2,workbyte2.bit_3,workbyte2.bit_4,workbyte2.bit_5,workbyte2.bit_6,workbyte2.bit_7);               //JFL
      //printf("PosCom: <%d>\n", PosCom);  // JFL
      
   	  pbyte_com[PosCom]= *caracter2;
      //printf("pbyte_com >%s< \n", pbyte_com);   // JFL

   	  PosExp ++;
   	  PosCom ++; 
   	  if (PosExp *8 >= Long)
   	     break;
   }
   
   fhexdump(stdout, pbyte_com, PosCom);

   printf("\n\n");               //JFL
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
