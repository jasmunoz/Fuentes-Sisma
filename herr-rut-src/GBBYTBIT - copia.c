
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

//PRJ270893 - 03-2024
/*
	DESCOMPRIMECHEQUE
	Longitud: cadena de 8 posicones justificada con ceros por la izquierda
	pbyte_com: cadena comprimida de entrada marcada por la longitud del campo anterior
	pbyte_exp: cadena expandida de salida, que sera multiplicada por 8, ya que expande
	
	int hex_to_int(char c);
*/
void DESCOMPRIMECHEQUE(char *Longitud, char *pbyte_com, char *pbyte_exp);
int fHexa_binario(char * cadena, int long_cadena, char *resultado);
int hex_to_int(char c);
int fhexdexp(FILE * pf, void * vaddr, int len, char * salida);
//PRJ270893 - 03-2024

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
   strLongitud[8]='\0';
   Long = atoi(strLongitud);
   unsigned char  caraprin;
   pbyte_exp[Long] = '\0';

   printf("**************************************************\n");  
   printf("***   GBBYTBIT.c versio 03/24 COMPRIMECHEQUE   ***\n");  
   printf("**************************************************\n");  
   
   printf("Longitud <%d> cadena entrada <%s>\n", Long, pbyte_exp);
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
   
   //printf("antes de fhexdump pbyte_com >%s<\n",pbyte_com); 		//JASM
   //fhexdump(stdout, pbyte_com, PosCom);
   //printf("despues de fhexdump pbyte_com >%s<\n",pbyte_com); 		//JASM
   pbyte_com[PosExp] = '\0';
   printf("Longitud <%d> cadena salida <%s>\n", PosExp, pbyte_com);

   printf("\n");               //JFL
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


//**************************************************************************************
//PRJ270893 - 03-2024
void DESCOMPRIMECHEQUE(char *strLongitud, char *pbyte_com, char *pbyte_exp)
{
  char *cadenaE;
  char *cadenaS;
  int longE=0;
  int longS=0;
   
  printf("\n");
  printf("******************************************************\n"); 
  printf("***  GBBYTBIT.c version 03/24 - DESCOMPRIMECHEQUE  ***\n"); 
  printf("******************************************************\n"); 
   
  strLongitud[8]='\0';
  longE = atoi(strLongitud);
  pbyte_com[longE] = '\0';
  printf("Longitud <%i> cadena entrada <%s>\n", longE, pbyte_com);
  longS = fhexdexp(stdout, pbyte_com, longE, cadenaS);
  printf("Intermedio cadenaS >%s< y longS >%i<\n", cadenaS, longS);
	
  longS = fHexa_binario(cadenaS, longS, pbyte_exp);
  
  printf("Longitud <%i> cadena salida <%s>\n", longS, pbyte_exp);
  strncpy(pbyte_exp,"11111111",8); 
  //strncpy(pbyte_exp,pbyte_exp,8); 
  //strncpy(pbyte_exp,cadenaF,256); 
  printf("**********************************\n"); 
  printf("\n");
  return;
}

int fhexdexp(FILE * pf, void * vaddr, int len, char * salida)
{
 	int ii,nn,doble=2,mm=0;
 	int newlen = len * doble;
 	unsigned char cc;
 	char linea[len+1];
 	char newlinea[newlen+1];
 	char * addr;
 	int offset;
 	addr = vaddr;
 	offset = 0;
 	nn=0;
 	
	while(nn<len)
 	{
  	cc= *(addr+nn);
  	sprintf(linea+offset, "%02X", cc);
  	offset+=2;
  	//printf("Linea intermedia >%s< y valor nn >%i< y valor mm >%i<\n", linea, nn, mm);
  	//newlinea+mm = &linea;
  	sprintf(newlinea+mm,"%2s", linea); 
  	//printf("Linea nueva >%s< y valor nn >%i< y valor mm >%i<\n", newlinea, nn, mm);
  	
  	offset = 0;
  	nn += 1;
  	mm += 2;
	}
 	strncpy(salida, newlinea, mm);
	//newlinea[mm] = '\0';
	salida[mm] = '\0';
	//printf("Linea final >%s< y valor mm >%s<\n", newlinea, mm);
 	//printf("Linea salida >%s< y valor mm >%i<\n", salida, mm);
 	return mm;
}

int fHexa_binario (char * cadena, int long_cadena, char * resultado)
{

 	int c = 0; // Contador para recorrer cadena
 	int s;
 	int resto, cociente;
 	int varA;
 	char auxD[4];
 	int longitud = 0;
 
  longitud = long_cadena;

	//printf("Cadena de entrada >%s<, de longitud de entrada >%i< \n",cadena,longitud);
	while (cadena[c]!=' ' && c < longitud)
    {
	 
     varA=3;
     strncpy(auxD,"0000",4);
     //recuperamos el valor del digito
     s=hex_to_int(cadena[c]);
     while (varA > 0)
     {
       resto=s%2;
       cociente=s/2;
       if (resto == 0)
         strncpy(auxD+varA,"0",1);
       else
          strncpy(auxD+varA,"1",1);
       s=cociente;
       varA=varA-1;
     }
     if (s == 0)  
       strncpy(auxD+varA,"0",1);
     else
       strncpy(auxD+varA,"1",1);
 
     strncpy(resultado + c * 4, auxD, 4); 
     c++;
   }
   resultado[c * 4] = '\0';
   
	longitud=strlen(resultado);
	//printf("Cadena de salida >%s<, de longitud de salida >%i< \n",resultado,longitud);
  return;
}

/* ************************************************************************* */
/* Funcion que recibe un caracter hexadecimal y devuelve su valor decimal    */
/* ************************************************************************* */
int hex_to_int(char c)
{
   int numhex;
   switch(c)
   {
    case'0':numhex=0;break;
    case'1':numhex=1;break;
    case'2':numhex=2;break;
    case'3':numhex=3;break;
    case'4':numhex=4;break;
    case'5':numhex=5;break;
    case'6':numhex=6;break;
    case'7':numhex=7;break;
    case'8':numhex=8;break;
    case'9':numhex=9;break;
    case'A':case'a':numhex=10;break;
    case'B':case'b':numhex=11;break;
    case'C':case'c':numhex=12;break;
    case'D':case'd':numhex=13;break;
    case'E':case'e':numhex=14;break;
    case'F':case'f':numhex=15;break;
    }
    return numhex;
}


/* Quitar quitar quitar quitar
   int PosCom;
   int PosExp;
   int Long=0;
   int p=0;
   int s=0;
   unsigned char cc;
   unsigned char c;
   char cadena[4];
   cadena[4] = '\0';

   printf("**********************************\n"); 
   printf("***  GBBYTBIT.c version 03/24  ***\n"); 
   printf("**********************************\n"); 
   
   strLongitud[8]='\0';
   Long = atoi(strLongitud);
   pbyte_com[Long]='\0';
   printf("Entrada Longitud <%d> pbyte_com <%s>\n", Long, pbyte_com);

   printf("Antes de fhexdump pbyte_com >%s<\n",pbyte_com);
   fhexdump(stdout, pbyte_com, Long);
   printf("Despues de fhexdump pbyte_com >%s<\n",pbyte_com);

   PosCom = 0;
   PosExp = 0;
   while (1) 
   {
  		cc = *(pbyte_com+PosCom);
  		c = *(pbyte_com+PosCom);
  		//strncpy(c, cc, 1);
  		p = hex_to_int(c);
  		c = *(pbyte_com+PosCom+1);
  		s = hex_to_int(c);
   		printf("Trozo %d pbyte_com >%2X< p >%d< s >%d<\n", PosCom, cc, p, s);
			switch (s)
			{
			case 0: strncpy(cadena,"0000",4);
			case 1: strncpy(cadena,"0001",4);
			case 2: strncpy(cadena,"0010",4);
			case 3: strncpy(cadena,"0011",4);
			case 4: strncpy(cadena,"0100",4);
			case 5: strncpy(cadena,"0101",4);
			case 6: strncpy(cadena,"0110",4);
			case 7: strncpy(cadena,"0111",4);
			case 8: strncpy(cadena,"1000",4);
			case 9: strncpy(cadena,"1001",4);
			case 10: strncpy(cadena,"1010",4);
			case 11: strncpy(cadena,"1011",4);
			case 12: strncpy(cadena,"1100",4);
			case 13: strncpy(cadena,"1101",4);
			case 14: strncpy(cadena,"1110",4);
			case 15: strncpy(cadena,"1111",4);
			}
			strncpy(pbyte_exp + PosExp * 4, cadena, 4);
   		PosExp ++;
   	  PosCom ++; 
   	  if (PosCom >= Long)
   	     break;
   }
   
   printf("Salida Longitud <%d> pbyte_exp <%s>\n", PosExp + 4, pbyte_exp);
   printf("\n\n");
   return;

*/
//PRJ270893 - 03-2024
