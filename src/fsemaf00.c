/***********************************************************************
 * 
 * 
 **********************************************************************/

int modifica_semaf_reso(NPSPARS *preg_pars, char estado)
{
  int retorno=0;
  char lee_clave[6];
  int posisubs=0;
  char auxsub[5];
  char auxestado[2];

    strncpy(lee_clave, preg_pars->pasubsis, 2);
    strncpy(lee_clave + 2, preg_pars->pasubapl, 2);
    strncpy(lee_clave + 4, &estado, 1);
    lee_clave[5]='\0';
    sprintf(auxsub,"%.4s",lee_clave);

    fprintLog(nomProg,"Se modifica el semaforo de clave >%.4s< en el estado >%c<\n", lee_clave, estado); 

    retorno=tratar_fich("NPTPARS",SEMAF00, NULL, lee_clave);

    if(retorno)
        fprintLog(nomProg,"Error al Modificar NPTPARS >%d<\n",retorno);
    else
    {
	posisubs= encuentra_svm(psvm, auxsub, TIP_SUBSIS);

        if (posisubs < 0)
        {
           fprintLog(nomProg,"ERROR: no encuentra subsistema >%s<\n", auxsub);
	   retorno = posisubs;
	}
        else
	{
	  sprintf(auxestado,"%c",estado);
          (psvm + posisubs)->str_svm.svmpars.pasemsub = atoi(auxestado);   
	  fprintLog(nomProg,"Modificado el semaforo en SVM >%d<\n",
		         (psvm + posisubs)->str_svm.svmpars.pasemsub ); 
        }
    }
    return(retorno);
}
