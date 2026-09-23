/**********************************************/

tbine *nuevo_tbin();
/**********************************************/
/***   funcion: carga bines  ***/
/**********************************************/
int fcarga_bines()
{
  int err,salir,cont=0;
   // PRJ0202598 cambio bin de 6 posiciones 8 cambia el tamaño total del registro NPSBINE y de la estructura tbine 
  NPSBINE regbine;
  tbine *pbi;
 // PRJ0202598 cambio bin de 6 posiciones 8 
 // char claveB[7];
 char claveB[8];

 pbi_binprimero = NULL;

 err=tratar_fich("NPTBINE",PRIMERO,&regbine,NULL);
 if (err)
 {
  if (err != 101)
  {
   fprintLog(nomProg, "Error >%d< en PRIMERO Fichero bine\n", err);
   //Añadido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Carga NPTBINE codigo de error >%i< en PRIMERO\n", err);
  
  }
  else
  {
   fprintLog(nomProg, "Fichero de bines vacio\n");
   //Añadido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Carga NPTBINE codigo de error >%i< fichero de BINES vacio\n", err);
  }
  salir = -2;
 }


 while(err == 0)
 {
   cont ++;
   
   fprintLog(nomProg, "leido >%d< %.*s %.*s %.*s %.*s\n",
   cont,
   sizeof(regbine.biclave0), regbine.biclave0,
   sizeof(regbine.birnginf), regbine.birnginf,
   sizeof(regbine.birngsup), regbine.birngsup,
   sizeof(regbine.bidescri), regbine.bidescri
   );

  pbi = nuevo_tbin();
  if (!pbi)
  {
   salir = 1;
   break;
  }

  memset(pbi->t_biclave0, ' ', sizeof(pbi->t_biclave0));
  strncpy(pbi->t_biclave0, regbine.biclave0,sizeof(regbine.biclave0));
  strncpy(pbi->t_birnginf, regbine.birnginf,sizeof(regbine.birnginf));
  strncpy(pbi->t_birngsup, regbine.birngsup,sizeof(regbine.birngsup));
  strncpy(pbi->t_bitboper, regbine.bitboper,sizeof(regbine.bitboper));
  strncpy(pbi->t_birespri, regbine.birespri,sizeof(regbine.birespri));
  strncpy(pbi->t_bicodent, regbine.bicodent,sizeof(regbine.bicodent));
  strncpy(pbi->t_biparau1, regbine.biparau1,sizeof(regbine.biparau1)); //PTASK000023003

  pbi->bin_siguiente = pbi_binprimero;
  pbi_binprimero = pbi;

  // PRJ0202598 cambio bin de 6 posiciones 8

  strncpy(claveB, regbine.biclave0,8);
  claveB[8]='\0';
  err=tratar_fich("NPTBINE",SIGUIENTE,&regbine,claveB); 

  if (err)
  {
   if (err == 101)
   {
      fprintLog(nomProg,"Ya no se encuentran mas registros NPTBINE\n");
      salir = 0;
	}
   else
   {
     fprintLog(nomProg, "Error >%d< en SIGUIENTE Fichero bine\n", err); 
     //Añadido para Monitorizacion SISMA 
     fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Carga NPTBINE codigo de error >%i< en SIGUIENTE\n", err);

     salir = -3;
   }
  }
 }   /***   fin del bucle   ***/

 return(salir);
}

/*******************************************************/
/*** crea un elemento nuevo en tabla de bines  ***/
/*******************************************************/

// PRJ0202598 cambio bin de 6 posiciones 8 cambia el tamaño total de la estructura tbine 
tbine *nuevo_tbin()
{
 tbine *p = (tbine *)malloc(sizeof(tbine));

 if (!p)   /*** insuficiente memoria   ***/ 
 {
  strcpy(coderror, "001");
  
   //Añadido para Monitorizacion SISMA 
  fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Update NPTTBINE codigo de error >%.3s< memoria insuficiente\n", coderror);

  strcpy(reginci.innomfic, "NPTBINE");     
  return (p);  
 }
 else
  return(p);
}



/**********************************************/
