/**********************************************/
/***   funcion: carga tabla de operaciones  ***/
/**********************************************/
int fcarga_tbop()
{
  int err,salir,cont=0; 
  char claveTB[19];  

  ptb_primero = NULL;
  strncpy(claveTB,"00",2);  
  claveTB[2]='\0';


 err=tratar_fich("NPTTBOP",CARGAR,&ptb_primero,claveTB); 
 if (err)
 {
   fprintLog(nomProg, "Error al cargaer la tabla de operaciones\n"); 
   //Añadido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Carga NPTTBOP codigo de error >%i<\n", err);  
  
   return(-2);
 }

 return(0);
}

/**********************************************/
