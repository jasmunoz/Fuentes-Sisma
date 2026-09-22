/******************************************************************
  Modifica el estado del semaforo.             
******************************************************************/
//int modifica_estado(NPSSOCK * preg_sock, char * estado)
int modifica_estado(char* preg_sock, char * estado)
{
 int error =0;
 char lee_clave[6];
 
  //solo lo modifica si es distinto al que ya tiene
  if(strncmp((psvm+posic_subsis)->str_svm.svmpars.paestado, estado,2)!=0)
  {
    strncpy(lee_clave, preg_sock, 2);
    strncpy(lee_clave + 2, preg_sock+2, 2);
    strncpy(lee_clave + 4, estado, 2);
    lee_clave[6]='\0';
  	
    fprintLog( nomProg, "Se modifica el estado de clave >%.4s< en el estado >%.2s<\n", lee_clave, lee_clave+4); 
   
     error=tratar_fich("NPTPARS",MODIFESTADO, NULL, lee_clave);

     fprintLog(nomProg, " despues de modifcar el semaforo \n");
    
    if(error)
    { 
        fprintLog(nomProg, "Error al Modificar NPTSOCK >%d<\n", error);
    }
    //modificar el SVN
    if (posic_subsis < 0)
    {
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr,"***** ERROR: no encuentra subsistema >%s< en svm : %d\n",
           lee_clave, posic_subsis); 
#endif
        return(1);
     }
     strncpy((psvm+posic_subsis)->str_svm.svmpars.paestado, estado, 2);    
   } 
   return(error);
}

