/**********************************************************************************************************/
/**********************************************************************************************************/
#include <stdio.h>
#include "cnetplus.h"

extern FILE * fichsal; 

#ifndef TRAZABIT
#define TRAZABIT
#endif

int fexplotabit(int opciex, char *pbyte_exp, char *pbyte_com);

int fexplotabit(int opciex, char *pbyte_exp, char *pbyte_com)
{
/***   el orden de los bits aparece aqui de derecha a izquierda (SCO),
                          de izquierda a derecha (HPUX),
       pero esto es algo variable dependiente de la maquina   ***/ 
//#ifdef LOW_HIGH
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
//#else
// typedef struct ochobits
// {  unsigned  int bit_0  :  1;
//    unsigned  int bit_1  :  1;
//    unsigned  int bit_2  :  1;
//    unsigned  int bit_3  :  1;
//    unsigned  int bit_4  :  1;
//    unsigned  int bit_5  :  1;
//    unsigned  int bit_6  :  1;
//    unsigned  int bit_7  :  1;
// } ochobits;
//#endif

   char caracter;
/***   convierte el tipo char de la variable caracter 
       a tipo ochobits mediante la variable workbyte   ***/
   ochobits *workbyte = (ochobits*) &caracter;

   ochobits workbyte2;
/***   convierte el tipo ochobits de la variable workbyte2
       a tipo char  mediante la variable caracter2   ***/
   char *caracter2 = (char*) &workbyte2;

#ifdef TRAZABIT
unsigned char  caraprin;
#endif

printf("opciex >%d<\n",opciex);


   switch (opciex)
   {  
   case 1: //Expandir
       // caracter = pbyte_com [ll];
	   
	caracter = *pbyte_com;
	
    pbyte_exp[0] = workbyte->bit_0 + '0';
    pbyte_exp[1] = workbyte->bit_1 + '0';
    pbyte_exp[2] = workbyte->bit_2 + '0';
    pbyte_exp[3] = workbyte->bit_3 + '0';
    pbyte_exp[4] = workbyte->bit_4 + '0';
    pbyte_exp[5] = workbyte->bit_5 + '0';
    pbyte_exp[6] = workbyte->bit_6 + '0';
    pbyte_exp[7] = workbyte->bit_7 + '0';

      break;
   case 2:    // comprimir

    workbyte2.bit_0 = ((pbyte_exp [0] ) == '1') ? 1 : 0;
    workbyte2.bit_1 = ((pbyte_exp [1] ) == '1') ? 1 : 0;
    workbyte2.bit_2 = ((pbyte_exp [2] ) == '1') ? 1 : 0;
    workbyte2.bit_3 = ((pbyte_exp [3] ) == '1') ? 1 : 0;
    workbyte2.bit_4 = ((pbyte_exp [4] ) == '1') ? 1 : 0;
    workbyte2.bit_5 = ((pbyte_exp [5] ) == '1') ? 1 : 0;
    workbyte2.bit_6 = ((pbyte_exp [6] ) == '1') ? 1 : 0;
    workbyte2.bit_7 = ((pbyte_exp [7] ) == '1') ? 1 : 0;


	  *pbyte_com= *caracter2; 
      break;
   } 

//#ifdef TRAZABIT
 /*fprintf(fichsal,"--------------------------------------------------\n"); 
 fprintf(fichsal,"bitmaps despues de fexplbit nnnn:\n");
 fflush(fichsal);*/
 printf("bitmaps despues de fexplbit nnnn:\n"); 

 caraprin = *pbyte_com ;
  if(caraprin < 16)
  {
 /*  fprintf(fichsal,">>>0%X<<< ", caraprin);
   fflush(fichsal);*/
   printf("1>>>0%X<<< ", caraprin);
  }
  else
  {
  /* fprintf(fichsal,">>>%X<<< ", caraprin);
      fflush(fichsal);*/
	   printf("2>>%X<<< ", caraprin);
  }
 /*fprintf(fichsal,"\n--------------------------------------------------\n");
 fflush(fichsal);*/
//#endif

 return (0);
}

/**********************************************************************************************************/

