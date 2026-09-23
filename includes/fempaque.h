/***   el orden de los bits aparece aqui de derecha a izquierda (SCO),  
                                         de izquierda a derecha (HPUX),
       pero esto es algo variable dependiente de la maquina   ***/
#if defined (SCO_NETPLUS) || defined (EC_NT)
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
#else
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
#endif

   char caracter;
/***   convierte el tipo char de la variable caracter 
       a tipo ochobits mediante la variable workbyte   ***/
   ochobits *workbyte = (ochobits*) &caracter;

   ochobits workbyte2;
/***   convierte el tipo ochobits de la variable workbyte2
       a tipo char  mediante la variable caracter2   ***/
   char *caracter2 = (char*) &workbyte2;


