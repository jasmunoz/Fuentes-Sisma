/*****************************************************************
   FUNCION  FEXPLBIT             llamada por ftraductor

   DESCRIPCION: Funcion que convierte un mapa de bits de 8 bytes a 
                una cadena de 64 ceros y unos, y viceversa  

   AREA DE PASO :
  - entero con el sentido de la expansion :                
          1.- expandir     (traducion nodo-->netplus)       
          2.- comprimir    (traducion netplus-->nodo)       
                                                             
  - puntero a char con mapa de bits expandido (64 bytes)      
                                                             
  - puntero a char con mapa de bits comprimido (8 bytes)     
*****************************************************************/
int fexplbit(int opciex, char *pmapa_exp, char *pmapa_com)
{

/***   el orden de los bits aparece aqui de derecha a izquierda (SCO),
                          de izquierda a derecha (HPUX),
       pero esto es algo variable dependiente de la maquina   ***/
//#ifdef LOW_HIGH
//   typedef struct ochobits
//   {  unsigned  int bit_7  :  1;
//      unsigned  int bit_6  :  1;  
//      unsigned  int bit_5  :  1;
//      unsigned  int bit_4  :  1;
//      unsigned  int bit_3  :  1;
//      unsigned  int bit_2  :  1;
//      unsigned  int bit_1  :  1;
//      unsigned  int bit_0  :  1;
//   } ochobits;
//#else
// typedef struct ochobits
 {  unsigned  int bit_0  :  1;
    unsigned  int bit_1  :  1;
    unsigned  int bit_2  :  1;
    unsigned  int bit_3  :  1;
    unsigned  int bit_4  :  1;
    unsigned  int bit_5  :  1;
    unsigned  int bit_6  :  1;
    unsigned  int bit_7  :  1;
 } ochobits;
//#endif

   char caracter;
/***   convierte el tipo char de la variable caracter 
       a tipo ochobits mediante la variable workbyte   ***/
   ochobits *workbyte = (ochobits*) &caracter;

   ochobits workbyte2;
/***   convierte el tipo ochobits de la variable workbyte2
       a tipo char  mediante la variable caracter2   ***/
   char *caracter2 = (char*) &workbyte2;

  int ll;
#ifdef TRAZABIT
#endif
  int mm;

#if 0
f defined (TRAZA) && (TRAZA > 2) 
#endif

#ifdef TRAZA
#ifdef LOW_HIGH
fprintLog( "Definido LOW_HIGH\n");
#else
fprintLog( "NO Definido LOW_HIGH\n");
#endif
#endif
    
   switch (opciex)
   {  
   case 1:

      for(ll=0;ll<8;ll++)
      {
         caracter = pmapa_com [ll];

    pmapa_exp[8*ll + 0] = workbyte->bit_0 + '0';
    pmapa_exp[8*ll + 1] = workbyte->bit_1 + '0';
    pmapa_exp[8*ll + 2] = workbyte->bit_2 + '0';
    pmapa_exp[8*ll + 3] = workbyte->bit_3 + '0';
    pmapa_exp[8*ll + 4] = workbyte->bit_4 + '0';
    pmapa_exp[8*ll + 5] = workbyte->bit_5 + '0';
    pmapa_exp[8*ll + 6] = workbyte->bit_6 + '0';
    pmapa_exp[8*ll + 7] = workbyte->bit_7 + '0';
      } 
      break;
   case 2:


      for(ll=0;ll<8;ll++)
      {
         workbyte2.bit_0 = ((pmapa_exp [ 8*ll + 0 ] ) == '1') ? 1 : 0;
         workbyte2.bit_1 = ((pmapa_exp [ 8*ll + 1 ] ) == '1') ? 1 : 0;
         workbyte2.bit_2 = ((pmapa_exp [ 8*ll + 2 ] ) == '1') ? 1 : 0;
         workbyte2.bit_3 = ((pmapa_exp [ 8*ll + 3 ] ) == '1') ? 1 : 0;
         workbyte2.bit_4 = ((pmapa_exp [ 8*ll + 4 ] ) == '1') ? 1 : 0;
         workbyte2.bit_5 = ((pmapa_exp [ 8*ll + 5 ] ) == '1') ? 1 : 0;
         workbyte2.bit_6 = ((pmapa_exp [ 8*ll + 6 ] ) == '1') ? 1 : 0;
         workbyte2.bit_7 = ((pmapa_exp [ 8*ll + 7 ] ) == '1') ? 1 : 0;
  
         pmapa_com[ll] = *caracter2; 
      } 
      break;
   } 

  printf("============================ Pintamos MAPA BITS =============================\n");
  for (ll=0;ll<8;ll++)
  {   
	 caraprin = *(pmapa_com + ll);
     //AÑADIDO RAQUEL
     if(caraprin < 16)
       {
        printf(">>>0%X<<< ", caraprin); 
       }
       else
       {
     	   printf(">>%X<<< ", caraprin);  
     
       }
     // FIN -AÑADIDO RAQUEL
  }
  printf("\n============================ Pintamos MAPA BITS =============================\n");
 
#ifdef TRAZABIT
#endif

 return (0);
}
