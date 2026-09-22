/******************************************************************
   mira en parametros si el semaforo esta abierto
******************************************************************/
int fsemaforo_red(int posic_subsis)
{
#if defined (TRAZA) && (TRAZA > 10)  
   //fprintf(stdout,"=============fsemaforo_red pos >%d< semaf >%d< \n",
   //    posic_subsis, (psvm + posic_subsis)->str_svm.svmpars.pasemsub);  
#endif
if (posic_subsis<0) 
  return(0); 
return   ((psvm + posic_subsis)->str_svm.svmpars.pasemsub);    

}

