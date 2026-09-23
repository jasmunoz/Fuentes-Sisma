
/********************
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "cnetplus.h"
#include "svm.h"
#include "Cola_comun.h"
#include "msgclien.h"
#include "procesos.h"

void fhexdump(FILE * pf, void * vaddr, int len);
int fexplotabit(int opciex, char *pbyte_exp, char *pbyte_com);

/**************************************************************/
/*********     MAIN                                      ******/
/**************************************************************/
   
int main(int argc, char *argv[])
{
   char Sentido[2];
   int IntSentido;
   char strLongitud[7];
   int Longitud;
   char Mensaje[10000];
   char MsgConvertido[10000];
    
   if (argc != 4)
   {
     printf("Uso del programa : ConversionCheques  <Sentido 1 (expandir) / 2 (comprimir) Longitud Mensaje>\n");
     exit(0);
   } else {
     strcpy(Sentido,argv[1]);
     IntSentido = atoi(Sentido);
     strcpy(strLongitud, argv[2]);
     Longitud = atoi(strLongitud);

     memcpy(Mensaje, argv[3], Longitud);
     
     fprintf(stdout,"Sentido <%s> Longitud <%d>\n", Sentido, Longitud);

     fprintf(stderr,"mensaje enviado==========================================================\n");
     fhexdump(stderr, &Mensaje, Longitud);
     fprintf(stderr,"mensaje enviado==========================================================\n");
     fflush(stderr);
   }

   switch (IntSentido)
   {
   case 1: //Expandir
      fexplotabit(IntSentido, MsgConvertido, Mensaje);
      fprintf(stdout, "Mensaje convertido: <%s>\n", MsgConvertido);
      break;
   case 2: //Comprimir
      fexplotabit(IntSentido, Mensaje, MsgConvertido);
      fprintf(stdout, "Mensaje convertido: <%c>\n", MsgConvertido[0]);
      break;
   } 
/*
   while (1)
   {

    retorno = leerCola_prueba(CLIENTE, DatomsgIn, &iLong, getpid(), iTimeOut,Emisor); 

    if (retorno)
    {
	   fprintf(stderr,"error al escribir en la cola %d\n", retorno);  
       fprintf(stderr,"SISMA NO RESPONDE\n");	   
	   return(0);  
    }		
    else
    {
  	 if (strncmp(DatomsgIn,"1804",4) == 0)
  	 {
   	   fprintf(stderr,"Enviado esperando respuesta\n"); 
           retorno= montarrespuesta(DatomsgIn,DatomsgOut,Emisor);
           iLong = 57;
           retorno = EscribirCola_prueba(SERVIDOR, DatomsgOut, iLong, &idHj, iTimeOut,Emisor);
         }
     }
   }	
*/	
	return(0);
	
	
}//fin mainnnnn


int fexplotabit(int opciex, char *pbyte_exp, char *pbyte_com)
{
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

unsigned char  caraprin;

printf("opciex >%d<\n",opciex);
printf("pbyte_com \n");
printf("pbyte_com <%s>\n", pbyte_com);
   switch (opciex)
   {  
   case 1: //Expandir
    caracter = pbyte_com [0];
	   
    //caracter = *pbyte_com;

    fprintf(stdout, "caracter <%d>\n", caracter);
    pbyte_exp[0] = workbyte->bit_0 + '0';
    pbyte_exp[1] = workbyte->bit_1 + '0';
    pbyte_exp[2] = workbyte->bit_2 + '0';
    pbyte_exp[3] = workbyte->bit_3 + '0';
    pbyte_exp[4] = workbyte->bit_4 + '0';
    pbyte_exp[5] = workbyte->bit_5 + '0';
    pbyte_exp[6] = workbyte->bit_6 + '0';
    pbyte_exp[7] = workbyte->bit_7 + '0';
    fprintf(stdout, "Mensaje expandido <%c>", pbyte_exp[0]);
    fprintf(stdout, "<%c>", pbyte_exp[1]);
    fprintf(stdout, "<%c>", pbyte_exp[2]);
    fprintf(stdout, "<%c>", pbyte_exp[3]);
    fprintf(stdout, "<%c>", pbyte_exp[4]);
    fprintf(stdout, "<%c>", pbyte_exp[5]);
    fprintf(stdout, "<%c>", pbyte_exp[6]);
    fprintf(stdout, "<%c>\n", pbyte_exp[7]);
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
