//PRJ0257005 - Revisado tokenizacion
/*** 
   FUNCION: FTRATAER.C 
   DESCRIPCION: Este fichero contiene las funciones necesarias
                para enviar un mensaje a su resolutor.
  
***/
#define MAX_CENT 30
#include "nptpara.h"
#include "procesos.h" 

int fclase_tipomsg(int tipomsg);
int fenvia_resolutor(formato_interno * pforitn);
int fsemaforo_red(int posic_subsis);
int modifica_semaf_reso(NPSPARS * preg_pars, char estado);
int fverfi(formato_interno fi);

NPSPARA  *reg_para;

/**********************************************/
/***   funcionER: ENVIA AL RESOLUTOR        ***/
/**********************************************/
int funcioner(formato_interno * pforitn)
{
  NPSPARS reg_pars;
  int cont=0;
  char clsubsis[5];
  int posisubs=0;
  int ll;

  int retorno, resolutor,retorno_semaf;
  int clasemsg,tipomsg,msg_online;

#if defined (TRAZA) && (TRAZA > 5)  			  
 fprintLog(nomProg,"*************************\n");
 fprintLog(nomProg,"  %.6s *** FTRATAER ***\n", pforitn->fidatpro.finumope);
 fprintLog(nomProg,"*************************\n");
 fverfi(*pforitn);
#endif 

//PRJ0258542 - Fase 3 - Cambio 11052026 - INI
	if (strncmp(pforitn->fiisocom.fiiso000,"18",2) == 0)
	{
		if (strncmp(pforitn->fiisocom.fiiso024,"811",3) != 0 && strncmp(pforitn->fiisocom.fiiso024,"815",3) != 0 && strncmp(pforitn->fiisocom.fiiso024,"816",3) != 0)
		{
			fprintLog(nomProg, "* * * Mensaje de control * * *\n");
      strncpy(pforitn->fidatiso.fiisoctr.fictr011,pforitn->fiisocom.fiidtran,6);
      strncpy(pforitn->fidatiso.fiisoctr.fictr012.aa,pforitn->fiisocom.fifechor.aa,2);
      strncpy(pforitn->fidatiso.fiisoctr.fictr012.mm,pforitn->fiisocom.fifechor.mm,2);
      strncpy(pforitn->fidatiso.fiisoctr.fictr012.dd,pforitn->fiisocom.fifechor.dd,2);
      strncpy(pforitn->fidatiso.fiisoctr.fictr012.hh,pforitn->fiisocom.fifechor.hh,2);
      strncpy(pforitn->fidatiso.fiisoctr.fictr012.mi,pforitn->fiisocom.fifechor.mi,2);
      strncpy(pforitn->fidatiso.fiisoctr.fictr012.ss,pforitn->fiisocom.fifechor.ss,2);
      strncpy(pforitn->fidatiso.fiisoctr.fictr094.longitud,pforitn->fiisocom.fi032094.longitud,2);
      strncpy(pforitn->fidatiso.fiisoctr.fictr094.datos,pforitn->fiisocom.fi032094.datos,6);
      if (strncmp(pforitn->fidatpro.firespri, "03", 2) == 0)
      {
      	strncpy(pforitn->fidatiso.fiisoctr.fictr094.longitud,"06",2);
      	strncpy(pforitn->fidatiso.fiisoctr.fictr094.datos,"020232",6);
      }
      if (strncmp(pforitn->fidatpro.firespri, "07", 2) == 0)
      {
      	strncpy(pforitn->fidatiso.fiisoctr.fictr094.longitud,"06",2);
      	strncpy(pforitn->fidatiso.fiisoctr.fictr094.datos,"020200",6);
      }
      if (strncmp(pforitn->fidatpro.firespri, "05", 2) == 0)
      {
      	strncpy(pforitn->fidatiso.fiisoctr.fictr094.longitud,"06",2);
      	strncpy(pforitn->fidatiso.fiisoctr.fictr094.datos,"021544",6);
      }
  	}
  	else
  	{
  		fprintLog(nomProg, "* * * Mensaje de claves * * *\n");
  	}
	}
//PRJ0258542 - Fase 3 - Cambio 11052026 - FIN

 strcpy(reginci.innomfic, "trataER"); // Modificacion para que se quede guardado el programa que ha dado el error, y no se pierda el nombre del servicio
 
 resolutor = atoi(pforitn->fidatpro.firespri);
 
  fprintLog(nomProg," * * * Resolutor ->%02d<-\n", resolutor);

//RAQUEL cambiado para coger los datos por bbdd por entidad
    for (ll=0 ; ll<2 ; ll++) 
    {	

	   if (strncmp(pforitn->fidatpro.firespri,reg_para[ll].psresolu,2)==0)
	   {
          fprintLog(nomProg," * * * Subsres ->%s<-\n", pforitn->fidatpro.fisubres);

          //mirar que tipo de mensaje es;
          tipomsg = atoi(pforitn->fiisocom.fiiso000);
          clasemsg = fclase_tipomsg(tipomsg);
         
          // 20160629 - solo miramos semaforo en comunicaciones, en peticiones se intenta enviar
          if (clasemsg == PETICION_NO_CONTROL && pforitn->fiisocom.fiiso000[2] == '2')
          {
	      	strncpy(clsubsis, pforitn->fidatpro.firespri, 2);
	      	strncpy(clsubsis +2, pforitn->fidatpro.fisubres, 2);
	      	clsubsis[4] = '\0'; 
	      	
	      	posisubs= encuentra_svm(psvm, clsubsis, TIP_SUBSIS);
	      	if (posisubs < 0)
	      	{
#if defined (TRAZA) && (TRAZA > 1)		
		       fprintLog(nomProg,"no se encuentra el subsistema >%s< en svm\n",clsubsis); 
#endif		       
               strcpy(coderror, "502");
               strcpy(reginci.innomfic, "svm");
               strcpy(pforitn->fidatpro.ficodfin, "909");
               strcpy(pforitn->fidatpro.firesfin, "00");
               return(finalizar(98));
		    }
		    retorno = fsemaforo_red(posisubs);
            if (retorno !=1)
            {		   
		      // NO RESPALDAMOS  
              strcpy(pforitn->fidatpro.ficodfin, "241");
              strcpy(pforitn->fidatpro.firesfin, "00");		  
              strncpy(pforitn->fidatpro.ficlatab + 8, "50", 2);
#if defined (TRAZA) && (TRAZA > 1)		   
		      fprintLog(nomProg,"semaforo cerrado. Emisor no Disponible %.3s\n",pforitn->fidatpro.ficodfin ); 
#endif		   
		   
              return(99);
            }
          }//peticion no control;
		   break;
	   }

       else 
	   {
		   if (strncmp(pforitn->fidatpro.firespri,reg_para[ll].psnument,2)==0)
		   {
/* ponemos la subaplicacion de destino para que se grabe en el log */
        if(!strncmp(pforitn->fiisocom.fiiso000 , "18", 2))
			  {
          sprintf(pforitn->fidatpro.fisubres, "%.2s", pforitn->fidatiso.fiisoctr.fictr093.datos);
			    break;
			  }			  
		   } 		
	   }
  }

 
 //PRJ0257005 - ftrataer.c
 fprintLog(nomProg, "    ** ** ** Entrando en fgraba_log \n");
 retorno = fgraba_log(pforitn);
 fprintLog(nomProg, "    ** ** ** Saliendo de fgraba_log \n");
 
 if(retorno)
 {
  strcpy(coderror, "222");
  strcpy(reginci.innomfic, "nptlogo");
      //Añadido para Monitorizacion SISMA 
    fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Escritura BBDD tabla NPTLOGO codigo de error 222\n");

  strcpy(pforitn->fidatpro.ficodfin, "909");
  strcpy(pforitn->fidatpro.firesfin, "00");
  return(finalizar(98));
 } /*fin del if */

//RAQUEL CAMBIAR EL SWITCH POR EL CAMPO DE LA TABLA NPTPARA

   for (ll=0 ; ll<2 ; ll++) 
   {    
     if ((resolutor=atoi(reg_para[ll].psnument)) || (resolutor=atoi(reg_para[ll].psresolu)))
     {
				retorno = fenvia_resolutor(pforitn);
				if(retorno)
				{
		          strcpy(coderror, "266");
		          sprintf(reginci.innomfic , "ENVRE%02d", resolutor);
		        
		         //Añadido para Monitorizacion SISMA 
		          fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Envio ENVRE%02d codigo de error >%.3s<\n",resolutor,coderror);
		        
		          strcpy(reginci.incoderr, coderror);
		          //cierra el semaforo cuando no consigue escribir en la PIPE del penvio05.
		          strncpy(reg_pars.pasubsis, pforitn->fidatpro.firespri, 2);
		          strncpy(reg_pars.pasubapl,"00", 2);
		        
		          retorno_semaf = modifica_semaf_reso(&reg_pars, '0');

		          fprintLog(nomProg,"despues de cerrar el semaforo retorno >%d<\n",retorno_semaf);
		          strncpy(pforitn->fidatpro.ficlatab + 8, "40", 2);
		          return(99);
        }
     		break;
     }
   }
   
   if (reg_para[ll].psnument==NULL) 
   {
      // case  0:  /*  debe reentrar al nucleo, y respaldar  semaforo cerrado */
#if defined (TRAZA) && (TRAZA > 1)  
      fprintLog(nomProg,"Resolutor no contemplado >%d<. Respaldar\n",resolutor);
#endif   
	  strncpy(pforitn->fidatpro.ficlatab + 8 , "40" , 2);
      return(99);
   }
// }/* fin  switch */

 return(finalizar(11));

} /* fin funcion */

/*********************************************************/
/***   envia un mensaje  al resolutor correspondiente  ***/
/*********************************************************/
int fenvia_resolutor(formato_interno * pforitn)
{
 
  int qreso;
  int damecola(SVM *psvm_par,char *proce,char *subsis); 
  char auxReso[3];
  char cola_envio[9];
  int aux;
  
#if defined (TRAZA) && (TRAZA > 10)  
  fprintLog(nomProg,"envia al resolutor >%s<\n", pforitn->fidatpro.firespri);
#endif
  
  sprintf(auxReso,"%.2s", pforitn->fidatpro.firespri);
  auxReso[2] = '\0';
 
//RAQUEL SE CAMBIA EL SWITCH  
 
    strncpy(cola_envio,PENVIO,6);
	//aux= atoi(pforitn->fidatpro.firespri);
	//fprintLog(nomProg,"RAQUEL aux para comprobar ftrataer linea 254 %i \n",aux);
    strncpy(cola_envio+6, pforitn->fidatpro.firespri,2);
    cola_envio[8] = '\0';
    qreso = damecola(psvm, cola_envio,NULL);
 
   if (qreso== -1)
   {
#if defined (TRAZA) && (TRAZA > 1)   
      fprintLog(nomProg,"Error al obtener la cola\n");
#endif	  
      return(98);
   }
   else
   {
     mensaje_nuc  mensa;
     int long_mensa = sizeof(mensa.fiforitn); //PRJ0258452 - quitado = sizeof(mensa)-sizeof(mensa.tipo);
	 
#if defined (TRAZA) && (TRAZA > 10)	 
	 fprintLog(nomProg,"long_mensa %d por la cola %i\n", long_mensa, qreso);
#endif     
     errno=0;
     mensa.tipo=1111;  
     memcpy(&mensa.fiforitn,pforitn,sizeof(mensa.fiforitn));
     signal(SIGALRM,sigue);
     alarm(ESSNDCOL); //ESSNDCOL = 3
     if ((msgsnd(qreso, &mensa, long_mensa,0))== -1)
        {
#if defined (TRAZA) && (TRAZA > 1)		
		 fprintLog(nomProg,"No consigue enviar\n");
#endif		 
         alarm(0);
         return(98);
        }
     alarm(0);
     return(0);
   }
}/* fin funcion */
