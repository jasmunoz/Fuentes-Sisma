/***
    
   DESCRIPCION: TRATA LOS DIFERIDOS DEL ORIGEN Y LAS CORRECTORAS
                DEL ORIGEN CON TARJETA PROPIA  
***/


int fgradife(formato_interno * pforitn);  
int fverfi(formato_interno fi);

/***************************************************************/
/***   funciondp: autorizacion diferidos con tarjeta propia  ***/
/***************************************************************/

int funciondp(formato_interno * pforitn)
{
int retorno=0;

#if defined (TRAZA) && (TRAZA > 5)     
 	fprintLog(nomProg,"=============================\n");
 	fprintLog(nomProg,"= Funcion DP 29072025       =\n");
 	fprintLog(nomProg,"= Autorizacion del diferido =\n");
 	fprintLog(nomProg,"=============================\n");
	fverfi(*pforitn);
#endif 
     
#if defined (TRAZA) && (TRAZA > 10)
     fprintLog(nomProg,"iso056.longitud >%.*s<\n",
        sizeof(pforitn->fidatiso.fiisoaut.fiaut056.longitud),
        pforitn->fidatiso.fiisoaut.fiaut056.longitud);
     fprintLog(nomProg,"iso056.datos >%.*s<\n",
        sizeof(pforitn->fidatiso.fiisoaut.fiaut056.datos),
        pforitn->fidatiso.fiisoaut.fiaut056.datos);
#endif	

    //si es un diferido que llega ya denegado del origen no lo grabamos para que no se envie al resolutor
	//20160513 - redsys nos ha mandado una operacion de compra internet denegada por autenticacion como 1220 con p39=184
	
  fprintLog(nomProg, "pforitn->fiisocom.respusal >%s<\n", pforitn->fiisocom.respusal);
	if(strncmp(pforitn->fiisocom.respusal,"000",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"001",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"002",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"004",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"007",3)==0	
	|| strncmp(pforitn->fiisocom.respusal,"300",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"400",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"500",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"501",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"900",3)==0
	|| strncmp(pforitn->fiisocom.respusal,"   ",3)==0
	)
	{
	  
     retorno = fgradife(pforitn);
	   switch(retorno)
	   {case 0:  //GRABA BIEN
       
         strcpy(pforitn->fidatpro.firesfin,"00");
         strcpy(pforitn->fidatpro.ficodfin, "000");
		 break;
		 
       case -1:  //REGISTRO DUPLICADO
	   
	     // si da error por clave duplicada al insertar en dife es que esa operacion ya se ha tratado
	     // así que hacemos lo mismo que si hubiera encontrado la previa
         if(retorno== -1) //clave duplicada
	     {    
           //debe ser repetición
	       // ponemos la marca de que no totaliza porque la original ya habrá totalizado
	        pforitn->fidatpro.finototal[0] = 'N';
			//si es 1420 codfin 014 que se traduce a 400 para redsys
			//si es 1120 o 1220 codfin 015 que se traduce a 900 para redsys
			if(strncmp(pforitn->fiisocom.fiiso000,"14",2)==0)
			{
				sprintf(pforitn->fidatpro.ficodfin ,"%.3s" ,"014");
			    fprintLog(nomProg,"operacion repetida, codfin 014 y enviar al origen\n");	
			}
			else
			{
				sprintf(pforitn->fidatpro.ficodfin ,"%.3s" ,"015");
				fprintLog(nomProg,"operacion repetida, codfin 015 y enviar al origen\n");
			}
	        strcpy(pforitn->fidatpro.firesfin,"00");
	        return (finalizar(98)); // retorna al tratami y envia al origen   
	     }
		 break;
		 
       default:  //OTROS
         strcpy(pforitn->fidatpro.firesfin,"00");
         strcpy(pforitn->fidatpro.ficodfin, "909");
		 return (finalizar(98)); // retorna al tratami y envia al origen   
		 break;
	   };
	}
	else
	{
#if defined (TRAZA) && (TRAZA > 5)
     fprintLog(nomProg,"operacion denegada en origen no se graba en diferidos\n");  
	 //184 es error en autenticación de redsys
	 if(strncmp(pforitn->fiisocom.respusal,"184",3)==0) 
	    strncpy(pforitn->fidatpro.ficodfin, "184",3);	   
	 else
	    strncpy(pforitn->fidatpro.ficodfin, "190",3);	   //PRJ0258542 - Se cambia 261 por 190
	 
     strncpy(pforitn->fidatpro.firesfin,pforitn->fidatpro.fiorigen,2); 
#endif		 
	}
    return(finalizar(11));
}



