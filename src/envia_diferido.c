/*************************************************************************
   funcion de envio  de diferidos
   retorna -2 si hay error y tiene que dejar de intentarlo
           1 si no es contestada y se debe intentar de nuevo 
		   		 0 si es contestada
**************************************************************************/
int envia_diferido(formato_interno *pforitn)
{
   int queda,retorno,tiempo_respuesta;
   int loc_errno;
   int recibido;
   char auxnumope[7];
	fprintLog(nomProg,"Entrada en envia_diferido con P00 = %.4s\n", pforitn->fiisocom.fiiso000); //PRJ0258542
  retorno = 1;
  while(retorno) 
  {
/**** --------- Limpia la cola---------- ****/
    retorno = msgrcv(qdife, &msg, long_mensaje, PID, IPC_NOWAIT);
    if(retorno == -1)
    {
      if(errno != ENOMSG)
      {
        fprintLog(nomProg,"error leer cola qdife <%d> en limpia>%d<\n", qdife, errno);
        perror("error leer cola qdife");
				fhexdump(stdout, &msg, retorno);
      }
      break;
    }

    if(retorno)
    {
      fprintLog(nomProg,"mensaje residual:\n");
      fhexdump(stdout, &msg, retorno);
    }
  }

  msg.tipo=1111;
  //   envia a rtratami  
  sprintf(pforitn->fidatpro.fipidori, "%d", PID);
  //en  fipidori guardamos el identificador del proceso para que 
  //se devuelva al proceso que lo inicio
   
   
  if (strncmp(pforitn->fiisocom.fiiso000,"1804",4) == 0)
      strncpy(pforitn->fidatpro.ficlatab,"18",2);
  else
      if ((strncmp(pforitn->fiisocom.fiiso000,"1520",4) == 0) ||
          (strncmp(pforitn->fiisocom.fiiso000,"1521",4) == 0))
         strncpy(pforitn->fidatpro.ficlatab,"15",2);
      else
         strncpy(pforitn->fidatpro.ficlatab,"55",2); //tratamiento de diferidos
  			strncpy(pforitn->fidatpro.ficlatab+2,"00",2);
  			strncpy(pforitn->fidatpro.ficlatab+4, pforitn->fidatpro.firespri, 2);
/**** para que nos conserve los codigos
                             07 anulacion por no confirmacion
                             40 anulacion por parada mientras en vuelo
                             60 anulacion retenida por previa en vuelo
*****/

  strncpy(pforitn->fidatpro.ficlatab+6,"00",2);
  strncpy(pforitn->fidatpro.ficlatab+8,"00",2);
  strncpy(pforitn->fidatpro.ficlatab+10,"00",2);
   
   // identificamos que el origen es diferidos
  strncpy(pforitn->fidatpro.fiorigen,"50",2);
  memcpy(&msg.fiforitn,pforitn,
               sizeof(formato_interno));
			   
#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"Envio con pid >%.6s<\n",msg.fiforitn.fidatpro.fipidori);
#endif
/////////////////// ENVIO AL RTRATAMI /////////////////////////
	fprintLog(nomProg,"Dentro de envia_diferido con la clave = %s\n", pforitn->fidatpro.ficlatab); //PRJ0258542
  if ((msgsnd(qtratami, &msg, long_mensaje ,0))== -1)
  {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"error envio  cola qtrata <%d>   %d\n", qtratami, errno);
#endif
     strcpy(reginci.innomfic,"qtrata");
     strcpy(coderror, "244");
     return(-2); //error
  }
  ///////////////////////////////////////////////////////////////
  
   /* lanza temporizador de espera de respuesta  */
   strncpy(auxnumope,pforitn->fidatpro.finumope,6); 
   auxnumope[6]='\0';

//DMND0036176 añadidas las preautorizaciones
   switch(atoi(pforitn->fiisocom.fiiso000))
   {
   case 1121:
   case 1221:
   case 1421:
   case 1525:
   case 1325:
       tiempo_respuesta = 6 * (psvm + subsis_svm)->str_svm.svmpars.patrerep;
	   //cierra el semaforo con patrerep pero antes de mandar una repetición espero 6 veces mas
   break;
   
   case 1804:
       //20160609 - no se por que los signoff tienen otro tiempo
       //if (strcmp(pforitn->fiisocom.fiiso024,"802") == 0)
         tiempo_respuesta = (psvm + subsis_svm)->str_svm.svmpars.patresnf;
       //else
       //  tiempo_respuesta = 3 + (psvm + subsis_svm)->str_svm.svmpars.patrepet;
   break;
   default:
         tiempo_respuesta = 6 * (psvm + subsis_svm)->str_svm.svmpars.patrepet;
		 //cierra el semaforo con patrerep pero antes de mandar una repetición espero 6 veces mas
   break;
   }

   queda=tiempo_respuesta;
   while(queda)
  {
     signal (SIGALRM, sigue);
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"espero %d segundos\n",queda);
#endif
     alarm(queda);

#ifdef ESPERO_SENAL
     fprintLog(nomProg,"ESPERO_SENAL\n");
     pause();
     queda=alarm(0);
     signal(SIGALRM,SIG_IGN);
     recibido = msgrcv(qdife, &msg, long_mensaje ,PID,IPC_NOWAIT);
     loc_errno = errno;
#else
#if defined (TRAZA) && (TRAZA > 10)
     fprintLog(nomProg,"Espero en la cola >%d<\n", qdife);
#endif
     /////////////// RECIBE ////////////////////
     recibido = msgrcv(qdife, &msg, long_mensaje ,PID,0);
     loc_errno = errno;
     signal(SIGALRM,SIG_IGN);
     queda=alarm(0);
     if (queda==0)
       loc_errno = ENOMSG; /* no hay respuesta */
#endif
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"quedaban %d segundos\n",queda);
#endif


    ///////////////// NO HAY RESPUESTA //////////////////
    if (recibido== -1)
    {
      if (loc_errno == ENOMSG)/* no hay respuesta */
      {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"no hay respuesta al >%.6s-%.4s<\n"
	       , pforitn->fidatpro.finumope
	       , pforitn->fiisocom.fiiso000);
#endif
         return(1); //repite
      }
      else   /* error lectura cola  */
      {
#if defined (TRAZA) && (TRAZA > 1)
         fprintLog(nomProg,"error leer cola qdife %d\n",loc_errno);
#endif
        strcpy(reginci.innomfic,"qdife");
        strcpy(coderror, "242");
#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"sleep >%d< segundos\n",queda);
#endif
        sleep(queda);
        return(-2); //error
      }
    }
   
   //////////////////// SI HAY RESPUESTA ///////////
   /*-----verificar que la respuesta es al mensaje------*/
#if defined (TRAZA) && (TRAZA > 1)
   	fprintLog(nomProg,"Dentro envia_diferido, verificar la respuesta recibida\n"); //PRJ0258542
  	fprintLog(nomProg,"finumope respuesta >>>%.*s<<<\n",6, msg.fiforitn.fidatpro.finumope);
    fprintLog(nomProg,"auxnumope peticion >>>%.*s<<<\n",6, auxnumope);   
#endif
   if(!strncmp(msg.fiforitn.fidatpro.finumope, auxnumope, 6))
   {
     if(!strncmp(msg.fiforitn.fidatpro.ficodfin, "297", 3)) //si contesta ptimer con timeout reintenta
     {
#if defined (TRAZA) && (TRAZA > 10)
        fprintLog(nomProg,"codfin 297 timeout\n");
#endif
        return(1);  //repite
     }
     queda = 0;
   }
   else
   {
#if defined (TRAZA) && (TRAZA > 1)
     fprintLog(nomProg,"la respuesta no es la esperada\n"); 
#endif

   }
 }/*  fin del while(queda) */

 memcpy(pforitn,&msg.fiforitn, sizeof(formato_interno)); 

#if defined (TRAZA) && (TRAZA > 1)
 fprintLog(nomProg,"Salida envia_diferido con respuesta codfin >%.3s<\n", msg.fiforitn.fidatpro.ficodfin);
#endif
 return(0); //contestada
}
