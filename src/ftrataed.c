/***
   FUNCION: FTRATAED.C
   DESCRIPCION: Este fichero contiene las funciones necesarias
                para enviar respuesta a proceso de diferidos. 

***/
int fverfi(formato_interno fi);

/**********************************************/
/***   funcionED: ENVIAR A DIFERIDO        ***/   
/**********************************************/
int funcioned(formato_interno *pforitn)
{
   int retorno;
   fprintLog(nomProg,"******************\n");  
   fprintLog(nomProg,"*** FUNCION ED ***\n");
   fprintLog(nomProg,"******************\n");

    //se mantiene porque diferido siempre sera 50  
    strncpy(pforitn->fidatpro.fiorigen,"50",2);   
	fverfi(*pforitn);

    // modificamos el origen para utilizar la funcion envia_origen
    retorno = fenvia_origen(pforitn);


   return(finalizar(11));
} /* fin funcion */

