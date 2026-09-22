/*****************************************************************************************************
   FUNCION....: ftratass.c
   DESCRIPCION: ESTA FUNCION RECUPERA EL NUMERO Y LA FECHA DE SESION Y LO
                Si la sesion la llevamos nosotros (pribvada, cajeros):
                 CARGA EN EL FIAUT28 Y FIAUT29. SE LEE DE LAS VARIABLES EN MEMORIA SVM DEL SUBSISTEMA.
                 PARA GARANTIZAR QUE ESTAS VARIABLES ESTAN ACTUALIZADAS HAY QUE RECARGARLAS
                 EN EL CAMBIO DE SESION. EL CONTROL DE LA SESION ES DEL SUBSISTEMA ORIGEN.
                Si es REDSYS, la sesion la llevan ellos.
                 SI FIAUT28 ha cambiado, actualizamos la tabla con la nueva e incrementamos la sesion.
                 Quiere decir, que REDSYS ha cerrado la sesion.
    20/12/2023 - Se detecta que en el paso de la sesión 999 a la 001, en el envio de 
                totales se salta a la sesion 000 cuando resta 1 a las 001 y graba en la npttoto con 
                000 y sin informacion porque no encuentra registros 000 en la nptactoX correspondiente
******************************************************************************************************/

#include "cforitno.h"
#include "svm.h" 
#include <stdio.h>

int funcionss(formato_interno * pforitn)
{
    int retorno, origen, posic_subsis =0; 
    char auxsub[5];
    char clavep[14];
	char fecsesRed[7];
	char fecsesFi[7];
	int sesion;

#if defined (TRAZA) && (TRAZA > 5)     
 	fprintLog(nomProg,"==========================\n");
 	fprintLog(nomProg,"= Funcion SS 29072025    =\n");
 	fprintLog(nomProg,"= Verificacion de sesion =\n");
 	fprintLog(nomProg,"==========================\n");
	fverfi(*pforitn);
#endif 
	
	if(strncmp(pforitn->fiisocom.fiiso000,"16",2)==0 || strncmp(pforitn->fiisocom.fiiso000,"18",2)==0) //PRJ0258542 - Fase 3 - Cambio 11052026
	{
		fprintLog(nomProg," Operativa con iso000 %.4s que no necesita sesion\n",pforitn->fiisocom.fiiso000);
		return(finalizar(11));
	}  //PRJ0258542 - Fase 3 - Cambio 11052026
	
		strncpy(auxsub, pforitn->fidatpro.fiorigen,2);
    strncpy(clavep, pforitn->fidatpro.fiorigen,2);
    strncpy(auxsub + 2, pforitn->fidatpro.fisubori, 2);
    strncpy(clavep + 2, pforitn->fidatpro.fisubori, 2);
	
#if defined (TRAZA) && (TRAZA > 10)  	
    fprintLog(nomProg,"Sesion del subsistema >%.4s<\n", auxsub);	
#endif	
    posic_subsis= encuentra_svm(psvm,auxsub,TIP_SUBSIS);

    if (posic_subsis < 0)
    {
#if defined (TRAZA) && (TRAZA > 1)  	
	     fprintLog(nomProg,"No se encuentra el subsistema en SVM\n");
#endif		 
         return(finalizar(98));
    }    
	
	// las operaciones de conciliacion tienen la fecha de sesion en otra zona del fi
	if(strncmp(pforitn->fiisocom.fiiso000,"15",2)==0)
	{
      /* fecha de sesion en fiaut028 */
#if defined (TRAZA) && (TRAZA > 10)  	  
	  fprintLog(nomProg,"fecha sesion redsys >%.2s%.2s%.2s<\n",pforitn->fidatiso.fiisotot.fitot028.aa,
                                                               pforitn->fidatiso.fiisotot.fitot028.mm,
                                                               pforitn->fidatiso.fiisotot.fitot028.dd);
#endif															   
	  sprintf(fecsesRed,"%.2s%.2s%.2s",pforitn->fidatiso.fiisotot.fitot028.aa,
                                       pforitn->fidatiso.fiisotot.fitot028.mm,
                                       pforitn->fidatiso.fiisotot.fitot028.dd);	
	}
	else
	{
		if (strncmp(pforitn->fiisocom.fiiso000,"18",2)==0) //PRJ0258542 - Se mete el IF
		{
			fprintLog(nomProg,"NO llega fecha sesion, operacion claves\n");
	  	sprintf(fecsesRed,"%6s","      ");
		}
		else
		{
    	/* fecha de sesion en fiaut028 */
#if defined (TRAZA) && (TRAZA > 10)  	  
	  	fprintLog(nomProg,"fecha sesion redsys >%.2s%.2s%.2s<\n",pforitn->fidatiso.fiisoaut.fiaut028.aa,
                                                               pforitn->fidatiso.fiisoaut.fiaut028.mm,
                                                               pforitn->fidatiso.fiisoaut.fiaut028.dd);
#endif															   
	  	sprintf(fecsesRed,"%.2s%.2s%.2s",pforitn->fidatiso.fiisoaut.fiaut028.aa,
                                       pforitn->fidatiso.fiisoaut.fiaut028.mm,
                                       pforitn->fidatiso.fiisoaut.fiaut028.dd);
		}
	}
	fprintLog(nomProg,"*** ftratass.c - Inicio de auxsub >%.4s< sesion >%.3s< (psvm + posic_subsis)->str_svm.svmpars.panumses >%.3s< \n", auxsub, sesion, (psvm + posic_subsis)->str_svm.svmpars.panumses );								   
  //// OPERACIONES CONTABLES CON LA FECHA DE SESION INFORMADA
	if (strncmp(fecsesRed, "000000", 2) !=0 
	&&  strncmp(fecsesRed, "      ", 2) !=0) 
  {    		      
		//// SI ES LA MISMA SESION
		if (strncmp(fecsesRed, (psvm + posic_subsis)->str_svm.svmpars.pafecses, 6)==0)
    {		
       strncpy(auxsub,(psvm + posic_subsis)->str_svm.svmpars.panumses,3);
       auxsub[3]='\0';
       sesion = atoi(auxsub);		
       //fprintLog(nomProg,"*** ftratass.c - Paso 1 de auxsub >%.4s< sesion >%.3s< \n", auxsub, sesion);								   
		}
		else  //// SI ES DISTINTA
		{
		  /// SI ES MAYOR HACEMOS CAMBIO DE SESION
		  if (strncmp(fecsesRed, (psvm + posic_subsis)->str_svm.svmpars.pafecses, 6) > 0)
          { //ha cambiado la fecha de sesion
             strncpy((psvm + posic_subsis)->str_svm.svmpars.pafecses, fecsesRed, 6); 
			 //y el numero de sesion
             strncpy(auxsub,(psvm + posic_subsis)->str_svm.svmpars.panumses,3);
             auxsub[3]='\0';
             sesion = atoi(auxsub);
             //fprintLog(nomProg,"*** ftratass.c - Paso 2 de auxsub >%.4s< sesion >%.3s<\n", auxsub, sesion);								   
             
             if (sesion == 999)
               sesion = 1; //no pongo 0 pq da problemas al validar si esta informado
             else
               sesion++;
#if defined (TRAZA) && (TRAZA > 10)  		
		     fprintLog(nomProg,"cambio a la sesion >%03d<\n", sesion);  
			 //fprintLog(nomProg,"*** ftratass.c - Paso 3 de auxsub >%.4s< sesion >%.3s<\n", auxsub, sesion);

#endif			 
         
		     //actualiza SVM
             sprintf((psvm + posic_subsis)->str_svm.svmpars.panumses, "%03d", sesion);
		   
             //actualizar PARS
             strncpy(clavep + 4, (psvm + posic_subsis)->str_svm.svmpars.pafecses, 6 );
             strncpy(clavep + 10, (psvm + posic_subsis)->str_svm.svmpars.panumses, 3 );
             clavep[13]='\0';
#if defined (TRAZA) && (TRAZA > 10)  			 
             fprintLog(nomProg,"llamamos al filefun con clave %s<\n", clavep);	
#endif			 
             retorno = tratar_fich("NPTPARS",SESIONF,NULL, clavep);
             if (retorno != 0 )
             {
#if defined (TRAZA) && (TRAZA > 1)  			 
	           fprintLog(nomProg,"error al cambiar la sesion en el PARS\n");
#endif			   
               return(finalizar(98));
             }
          }
		  else  ////// SI ES MENOR ES QUE ES UNA OPERACION DE LA SESION ANTERIOR TODAVÍA ABIERTA
		  {
           //restamos uno a la sesion de pars
           strncpy(auxsub,(psvm + posic_subsis)->str_svm.svmpars.panumses,3);
           auxsub[3]='\0';
           sesion = atoi(auxsub) -1; 
           //20/12/2023 - controlar que si esta en la 001 no se pase a la 000, tiene que volver
           //               a la sesion 999 - Inicio cambio
           if (sesion == 0)
           {
            sesion = 999;
#if defined (TRAZA) && (TRAZA > 1)  			 
            fprintLog(nomProg,"Cambio sesion 000 a 999 \n");
#endif			   
           }
           //20/12/2023 - controlar que si esta en la 001 no se pase a la 000, tiene que volver
           //               a la sesion 999 - Fin cambio
		  }		  
   	}
   		/// INFORMAMOS LA SESION EN EL FORMATO INTERNO
		if(strncmp(pforitn->fiisocom.fiiso000,"15",2)==0)
		{
		   sprintf(pforitn->fidatiso.fiisotot.fitot029,"%03d",sesion);
		   pforitn->fidatiso.fiisotot.fitot029[3] = '\0';
           //fprintLog(nomProg,"*** ftratass.c - Paso 5 de pforitn->fidatiso.fiisotot.fitot029 >%.3s< \n", pforitn->fidatiso.fiisotot.fitot029);								   
           
		   
		}
		else
		{
		   sprintf(pforitn->fidatiso.fiisoaut.fiaut029,"%03d",sesion);
       pforitn->fidatiso.fiisoaut.fiaut029[3] = '\0';
       //fprintLog(nomProg,"*** ftratass.c - Paso 6 de pforitn->fidatiso.fiisoaut.fiaut029 >%.3s< \n", pforitn->fidatiso.fiisoaut.fiaut029);								   

		}

  }	  
 	else ///// OPERACIONES NO CONTABLES
  {//llevamos nosotros la sesion
   	fprintLog(nomProg,"*** ftratass.c - Se asigna a auxsub >%.4s< sesion >%.3s< y fecha >%.6s<\n", auxsub, (psvm + posic_subsis)->str_svm.svmpars.panumses, (psvm + posic_subsis)->str_svm.svmpars.pafecses);								   
		if (strncmp(pforitn->fiisocom.fiiso000,"18",2)==0) //PRJ0258542 - Se mete el IF
		{
			strncpy(pforitn->fidatiso.fiisocla.ficla028 ,(psvm + posic_subsis)->str_svm.svmpars.pafecses, 6);
			pforitn->fidatiso.fiisocla.ficla028[6] = '\0';
			strncpy(pforitn->fidatiso.fiisocla.ficla029 ,(psvm + posic_subsis)->str_svm.svmpars.panumses, 3);
			pforitn->fidatiso.fiisocla.ficla029[3] = '\0';
		}
		else
		{
			strncpy(pforitn->fidatiso.fiisoaut.fiaut028.aa ,(psvm + posic_subsis)->str_svm.svmpars.pafecses, 2);
			pforitn->fidatiso.fiisoaut.fiaut028.mm[2] = '\0';
			strncpy(pforitn->fidatiso.fiisoaut.fiaut028.mm ,(psvm + posic_subsis)->str_svm.svmpars.pafecses +2, 2);
			pforitn->fidatiso.fiisoaut.fiaut028.dd[2] = '\0';
			strncpy(pforitn->fidatiso.fiisoaut.fiaut028.dd ,(psvm + posic_subsis)->str_svm.svmpars.pafecses +4, 2); 
			pforitn->fidatiso.fiisoaut.fiaut028.dd[2] = '\0';
	  	strncpy(auxsub,(psvm + posic_subsis)->str_svm.svmpars.panumses,3);
    	auxsub[3]='\0';
			sesion = atoi(auxsub);
			strncpy(pforitn->fidatiso.fiisoaut.fiaut029 ,(psvm + posic_subsis)->str_svm.svmpars.panumses, 3);
			pforitn->fidatiso.fiisoaut.fiaut029[3] = '\0';
		}
	}
  return(finalizar(11));

} /* fin funcion */
