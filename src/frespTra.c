/***************************************************************************
   FUNCION   FRESPONDE_TRATAMI
   DESCRIPCION: Envia un mensaje al subsistema autorizador o nucleo  
                configurando una respuesta para el caso de que la
                peticion no hay podido ser enviada al resolutor
***************************************************************************/
int fresponde_tratami (formato_interno *pfi)
{
    int retorno,tipmen,clase;
    mensaje_nuc msgnuc;
    int long_msgnuc = sizeof(msgnuc.fiforitn); //PRJ0258452 - quitado = sizeof(msgnuc) - sizeof(msgnuc.tipo);


    // Comprueba el tipo de mensaje
    tipmen = atoi(pfi->fiisocom.fiiso000);
    clase = fclase_tipomsg(tipmen);

    if ((clase == RESPUESTAS_NUCLEO) 
     || (clase == RESPUESTAS_OTROS)
     || (clase == RESPUESTA_CIERRE))
    {  /***   si son respuestas, no vuelven al tratami   ***/
        return(0);
    }

    #if defined (TRAZA) && (TRAZA > 10)
    fprintLog(nomProg,"-------FUNCION   FRESPONDE_RTRATAMI----------\n");
    #endif

    strncpy(pfi->fidatpro.ficlatab + 4 , subsistema_general, 2);

    /**** 266 si ya se ha enviado y da Error envio (Hacer correctora) ***/
    if(!strcmp(pfi->fidatpro.ficodfin, "266"))
        strncpy(pfi->fidatpro.ficlatab + 8 , "41" , 2);
	
	// en el rsto de los casos se responde al origen ficlatab50 (no respaldamos ficlatab40)
    else
        //strncpy(pfi->fidatpro.ficlatab + 8 , "40" , 2);  //respaldo	
        /**** 280 Error de formato (Contestar, sin DR) ***/
        //if(!strcmp(pfi->fidatpro.ficodfin, "280"))
           strncpy(pfi->fidatpro.ficlatab + 8 , "50" , 2);
		   

    msgnuc.tipo = 1111;
    msgnuc.fiforitn = *pfi; 

    signal(SIGALRM,sigue);
    alarm(ESSNDCOL); 

    // Envia a rtratami
    if ((msgsnd(qrtratami, &msgnuc , long_msgnuc,0))== -1)
    {
        alarm(0);
        strcpy(coderror, "244");
        strcpy(reginci.innomfic, "QTRATA00"); 
        //Añadido para Monitorizacion SISMA 
        fprintLog(nomProg,"TIPO SISTEMA – ACCESO A LAS COLAS: Tipo Escritura QTRATA00 >%i< codigo de error >%.3s<\n",qrtratami,coderror);

        fgraba_incidencia_cp(coderror, NULL, NULL, reginci.innomfic); 
        return(1); 
    }
    alarm(0); 
    return (0);
}
