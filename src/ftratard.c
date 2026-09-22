/***
   FUNCION: FTRATARD.C
   DESCRIPCION: envia un diferido a su resolutor primario.  

***/
#include "nptpara.h"
int fverfi(formato_interno fi);
int fenvia_reso_dif(formato_interno * pforitn);
NPSPARA  *reg_para;
/**********************************************/
/***   funcionRD: RESOLUTOR DIFERIDO        ***/
/**********************************************/
int funcionrd(formato_interno *pforitn)
{
    int retorno, resolutor;
	NPSLOGO reg_logo;
	formato_interno auxlogforitn;
	char clave_pr[35];
    int ll;

    fprintLog(nomProg,"****************************\n");
    fprintLog(nomProg, "****** FUNCION RD *********\n");
    fprintLog(nomProg,"****************************\n");

	fverfi(*pforitn);
	pforitn->fidatpro.firespri[2]='\0';
    resolutor = atoi(pforitn->fidatpro.firespri);
    fprintLog(nomProg,"resolutor en RD->%02d<-\n", resolutor);

   //RAQUEL cambiado para coger los datos por bbdd por entidad
    for (ll=0 ; ll<2 ; ll++) 
    { 
	
      if (strncmp(pforitn->fidatpro.firespri, reg_para[ll].psresolu,2)==0)
	  {
			// Si es una confirmacion de preautorizacion
            // o devolucion con previa informada, busco la previa para pasársela al resolutor
			// (si no está ya informada) (es solo informativo si no la encuentra no pasa nada)
			fprintLog(nomProg,"fiaut056.long >%.2s<\n", pforitn->fidatiso.fiisoaut.fiaut056.longitud);
			fprintLog(nomProg,"finumprv >%.6s<\n", pforitn->fidatpro.finumprv);
			fprintLog(nomProg,"fiiso000 >%.4s<\n", pforitn->fiisocom.fiiso000);
			
			pforitn->fidatiso.fiisoaut.fiaut056.longitud[2] = '\0';
	        if(   nspaces(pforitn->fidatpro.finumprv,6)
			   && !strncmp(pforitn->fiisocom.fiiso000,"1220",4)
			   && atoi(pforitn->fidatiso.fiisoaut.fiaut056.longitud)
			   )
            {
               memset(clave_pr, ' ', sizeof(clave_pr));
               memcpy(clave_pr, pforitn->fidatiso.fiisoaut.fiaut056.datos,
                      atoi(pforitn->fidatiso.fiisoaut.fiaut056.longitud));
               clave_pr[35]='\0';
               strncpy(clave_pr + strlen(clave_pr),"               ",(35-strlen(clave_pr)));
               fprintLog(nomProg,"sizeof(clave_pr) %d\n", sizeof(clave_pr));
               fprintLog(nomProg,"clave_pr **********12345678901234567890123456789012345<----\n");
               fprintLog(nomProg,"clave_pr **********%.*s<----\n",sizeof(clave_pr), clave_pr);
			   clave_pr[35]='\0';
			
               retorno = 0;
               retorno = tratar_fich("PREVIAA",IGUAL,&reg_logo,clave_pr);
               fprintLog(nomProg,"IGUALPREVIAA retorno=%d\n",retorno);
			   if (retorno==0)
               { 
  			       memcpy(&auxlogforitn, (formato_interno *)reg_logo.loforitn, sizeof(formato_interno));
                   fprintLog(nomProg,"numope de la previa >%.6s<\n", auxlogforitn.fidatpro.finumope);
                   sprintf(pforitn->fidatpro.finumprv,"%.6s",auxlogforitn.fidatpro.finumope);
			   }
            }			
			
			/////// envía al resolutor   
            retorno = fenvia_resolutor(pforitn);
            if (retorno != 0)
            {
               fgraba_incidencia_cp("266", pforitn->fidatpro.finumope, "trataRD", NULL);
               return(finalizar(11));
            }
            break;
	  }
	  else 
      {
    //  default:
            fgraba_incidencia_cp("265",pforitn->fidatpro.finumope,"trataRD", NULL); 
            return(finalizar(11));
	  }
   }/* fin  switch */ //RAQUEL ahora del for

   return(finalizar(11));
} /* fin funcion */
/*********************************************************/
/***   envia un mensaje  al resolutor correspondiente  ***/
/*********************************************************/
int fenvia_reso_dif(formato_interno * pforitn)
{
 
  int qreso;
  int damecola(SVM *psvm_par,char *proce,char *subsis); 
  char auxReso[3];
  char cola_envio[9];
  
  fprintLog(nomProg,"envia al resolutor >%s<\n", pforitn->fidatpro.firespri);
  sprintf(auxReso,"%.2s", pforitn->fidatpro.firespri);
  
  fprintLog(nomProg,"valor del reso %s\n", auxReso);
  auxReso[2] = '\0';
  fprintLog(nomProg,"atoi del reso %d\n", atoi(auxReso));
 
     //RAQUEL se modifica para arranque unificado bbdd
    strncpy(cola_envio,PENVIO,6);
    strncpy(cola_envio+6,pforitn->fidatpro.firespri,2);
    cola_envio[8] = '\0';
    qreso = damecola(psvm, cola_envio,NULL);
  
   if (qreso== -1)
   {
      fprintLog(nomProg,"Error al obtener la cola\n");
      return(98);
   }
   else
   {
     mensaje_nuc  mensa;
     int long_mensa = sizeof(mensa.fiforitn); //PRJ0258452 - quitado = sizeof(mensa)-sizeof(mensa.tipo);
	 
	 fprintLog(nomProg,"long_mensa %d por la cola %i\n", long_mensa, qreso);
     
     errno=0;
     mensa.tipo=1111;  
     memcpy(&mensa.fiforitn,pforitn,sizeof(mensa.fiforitn));
     //signal(SIGALRM,sigue);
     //alarm(ESSNDCOL); //ESSNDCOL = 3
     if ((msgsnd(qreso, &mensa, long_mensa,0))== -1)
        {
		 			fprintLog(nomProg,"No consigue enviar\n"); 
         	//alarm(0);
         	return(98);
        }
     //alarm(0);   
    return(0);
   }
  }


