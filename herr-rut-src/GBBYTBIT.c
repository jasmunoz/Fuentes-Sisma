
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
	DESCOMPRIMECHEQUE - Pasandole una cadena Hexadecimal devuelve una cadena binaria
	Longitud: cadena de 8 posicones justificada con ceros por la izquierda
	pbyte_com: cadena comprimida de entrada marcada por la longitud del campo anterior
	pbyte_exp: cadena expandida de salida, que sera multiplicada por 8, ya que expande
	
	DESCOMPRIMECHEQUE2 - Pasandole una cadena Hexadecimal devuelve una cadena hexadecimal expandidad
	Longitud: cadena de 8 posicones justificada con ceros por la izquierda
	pbyte_com: cadena comprimida de entrada marcada por la longitud del campo anterior
	pbyte_exp: cadena expandida de salida, que sera multiplicada por 2, ya que expande

	int hex_to_int(char c);
*/
void DESCOMPRIMECHEQUE(char *Longitud, char *pbyte_com, char *pbyte_exp);
void DESCOMPRIMECHEQUE2(char *Longitud, char *pbyte_com, char *pbyte_exp);
int fHexa_bina(char * cadena, int long_cadena, char *resultado);
int hex_to_int(char c);
int fhexdexp(FILE * pf, void * vaddr, int len, char * salida);
void fHex_bin(char * cadena, char *long_cadena, char *resultado);
void fBin_hex(char * cadena, char *long_cadena, char *resultado);
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

   printf("\n");
   printf("**************************************************\n");  
   printf("***   GBBYTBIT.c version 03/24 COMPRIMECHEQUE  ***\n");  
   printf("**************************************************\n");  

  if (Long != strlen(pbyte_exp) || pbyte_exp[0] == ' ')
  {
		printf("Longitud indicada <%i> y longitud cadena entrada <%i> no coinciden\n", Long, strlen(pbyte_exp));
		printf("o la cadena esta vacia <%.*s>\n", 4, pbyte_exp);
		//return;
  }

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
   printf("**********************************\n"); 
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
   printf("***   GBBYTBIT.c - COMPRIMECHEQUE2 version 30/08/23  ***\n");   //JFL
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
  char cadena;
  int longE=0;
  int longS=0;
  int longI=0;
  int i = 0;
   
  printf("\n");
  printf("******************************************************\n"); 
  printf("***  GBBYTBIT.c version 03/24 - DESCOMPRIMECHEQUE  ***\n"); 
  printf("******************************************************\n"); 
   
  strLongitud[8]='\0';
  longE = atoi(strLongitud);
  pbyte_com[longE] = '\0';

  if (longE != strlen(pbyte_com) || pbyte_com[0] == ' ')
  {
		printf("Longitud indicada <%i> y longitud cadena entrada <%i> no coinciden\n", longE, strlen(pbyte_com));
		printf("o la cadena esta vacia <%.*s>\n", 4, pbyte_com);
	//	return;
  }

  printf("Longitud <%i> cadena entrada <%s>\n", longE, pbyte_com);
  longS = fhexdexp(stdout, pbyte_com, longE, cadenaS);
  //printf("Intermedio cadenaS >%s< y longS >%i<\n", cadenaS, longS);
	
  longI = fHexa_bina(cadenaS, longS, cadenaE);
  //printf("Longitud <%i> cadenaE <%s>\n", longI, cadenaE);
	cadena = *cadenaE;

	for (i = 0; i < longI; i++) pbyte_exp[i] = cadenaE[i];
	pbyte_exp[longI] = '\0';
	printf("Longitud <%i> cadena salida <%s>\n", longI, pbyte_exp);

  strncpy(pbyte_exp+longI,"       .",8); 
  printf("**********************************\n"); 
  printf("\n");
  return;
}

void DESCOMPRIMECHEQUE2(char *strLongitud, char *pbyte_com, char *pbyte_exp)
{
  char *cadenaE;
  char *cadenaS;
  char cadena;
  int longE=0;
  int longS=0;
  int longI=0;
  int i = 0;
   
  printf("\n");
  printf("*******************************************************\n"); 
  printf("***  GBBYTBIT.c version 03/24 - DESCOMPRIMECHEQUE2  ***\n"); 
  printf("*******************************************************\n"); 
   
  strLongitud[8]='\0';
  longE = atoi(strLongitud);
  pbyte_com[longE] = '\0';
  cadenaS = (char*) malloc(longE*4);
/*
  if (longE != strlen(pbyte_com) || pbyte_com[0] == ' ')
  {
		printf("Longitud indicada <%i> y longitud cadena entrada <%i> no coinciden\n", longE, strlen(pbyte_com));
		printf("o la cadena esta vacia <%.*s>\n", 4, pbyte_com);
	//	return;
  }
*/
  printf("Longitud <%i> cadena entrada <%.*s>\n", longE, longE, pbyte_com);
  longS = fhexdexp(stdout, pbyte_com, longE, cadenaS);
  //printf("Intermedio cadenaS >%s< y longS >%i<\n", cadenaS, longS);
	
  //longI = fHex_bin(cadenaS, longS, cadenaE);
  //printf("Longitud <%i> cadenaE <%s>\n", longI, cadenaE);
	//cadena = *cadenaE;

	for (i = 0; i < longS; i++) pbyte_exp[i] = cadenaS[i];
	pbyte_exp[longS] = '\0';
	printf("Longitud <%i> cadena salida <%s>\n", longS, pbyte_exp);

  strncpy(pbyte_exp+longS,"       .",8); 
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
 	//salida[newlen+1] = '\0';
 	
	while(nn<len)
 	{
  	cc= *(addr+nn);
  	sprintf(linea+offset, "%02X", cc);
  	offset+=2;
  	//printf("Linea intermedia >%s< y valor nn >%i< y valor mm >%i<\n", linea, nn, mm);
  	sprintf(newlinea+mm,"%2s", linea); 
  	//printf("Linea nueva >%s< y valor nn >%i< y valor mm >%i<\n", newlinea, nn, mm);
  	
  	offset = 0;
  	nn += 1;
  	mm += 2;
	}
 	strncpy(salida, newlinea, mm);
	//salida[mm+1] = '\0';
	//printf("Linea final >%s< y valor mm >%s<\n", newlinea, mm);
 	//printf("Linea salida >%s< y valor mm >%i<\n", salida, mm);
 	return mm;
}

int fHexa_bina (char * cadena, int long_cadena, char * resultado)
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
  return longitud;
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

void fHex_bin (char * cadena, char * long_cadena, char * resultado)
{

	int c = 0; // Contador para recorrer cadena
 	int s;
 	int resto, cociente;
 	int varA;
 	char auxD[4];
 	int longitud = 0;
 
  printf("\n");
  printf("*********************************************\n"); 
  printf("***  GBBYTBIT.c version 03/24 - fHex_bin  ***\n"); 
  printf("*********************************************\n"); 

  long_cadena[4]='\0';
  longitud=atoi(long_cadena);
  cadena[longitud] = '\0';

  if (longitud != strlen(cadena) || cadena[0] == ' ')
  {
		printf("Longitud indicada <%i> y longitud cadena entrada <%i> no coinciden\n", longitud, strlen(cadena));
		printf("o la cadena esta vacia <%.*s>\n", 4, cadena);
		//return;
  }

	printf("Longitud <%i> cadena entrada <%s>\n",longitud,cadena);
	while (cadena[c]!=' ' && c < longitud)
  {
	 
  	varA=3;
    strncpy(auxD,"0000",4);
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
  printf("Longitud <%i> cadena salida <%s>\n", longitud, resultado);
  printf("**********************************\n"); 
  printf("\n");
  return;
}

void fBin_hex (char * cadena, char * long_cadena, char * resultado)
{

	int c = 0; // Contador para recorrer cadena
 	int s = 0;
 	int varA;
 	char auxD[4];
 	int longE = 0;
 	int longS = 0;
 
  printf("\n");
  printf("*********************************************\n"); 
  printf("***  GBBYTBIT.c version 03/24 - fBin_hex  ***\n"); 
  printf("*********************************************\n"); 

  long_cadena[4]='\0';
  longE=atoi(long_cadena);
  cadena[longE] = '\0';
	//printf("Longitud <%i> cadena entrada 21/03/2024 13:36 <%s>\n",long,cadena);
  //if (longE != strlen(cadena) || cadena[0] == ' ')
  //{
	//	printf("Longitud indicada <%i> y longitud cadena entrada <%i> no coinciden ", longE, strlen(cadena));
	//	printf("o la cadena esta vacia <%.*s>\n", 4, cadena);
	//	//return;
  //}

	printf("Longitud <%i> cadena entrada <%s>\n",longE,cadena);
	longE=atoi(long_cadena);
	while (c < longE)
  {
  	strncpy(auxD,cadena + c,4);
  	auxD[4]='\0';
  	//printf("Trozo >%s< valor origen %i y valor destino %i y resultado %i\n",auxD,c,s,atoi(auxD));
  	switch (atoi(auxD))
  	{
  		case 0:
  			strncpy(resultado + s,"0",1);
  			break;
     	case 1:
        strncpy(resultado + s,"1",1);
        break;
    	case 10:
        strncpy(resultado + s,"2",1);
        break;
    	case 11:
        strncpy(resultado + s,"3",1);
        break;
    	case 100:
        strncpy(resultado + s,"4",1);
        break;
    	case 101:
        strncpy(resultado + s,"5",1);
        break;
    	case 110:
        strncpy(resultado + s,"6",1);
        break;
    	case 111:
        strncpy(resultado + s,"7",1);
        break;
    	case 1000:
        strncpy(resultado + s,"8",1);
        break;
    	case 1001:
        strncpy(resultado + s,"9",1);
        break;
    	case 1010:
        strncpy(resultado + s,"A",1);
        break;
    	case 1011:
        strncpy(resultado + s,"B",1);
        break;
    	case 1100:
        strncpy(resultado + s,"C",1);
        break;
    	case 1101:
        strncpy(resultado + s,"D",1);
        break;
    	case 1110:
        strncpy(resultado + s,"E",1);
        break;
    	case 1111:
        strncpy(resultado + s,"F",1);
        break;
    }
  	s++;
  	c+=4;
  	//printf("Resultado %.*s y valor de c %i\n", s, resultado, c);
 	}
  resultado[s] = '\0';
   
  longS=strlen(resultado);
  //printf("Cadena de salida >%s<, de longitud de salida >%i< \n",resultado,longS);
  printf("Longitud <%i> cadena salida <%s>\n", longS, resultado);
  printf("**********************************\n"); 
  printf("\n");
  return;
}

//PRJ270893 - 03-2024
