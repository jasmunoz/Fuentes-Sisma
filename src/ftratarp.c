//PRJ0257005 - Revisado tokenizacion
//PRJ0247167 - Revisado ampliación P48
/***
   FUNCION: FTRATARP.C 
   DESCRIPCION: Tratamiento de repeticiones origen Nodo 
***/

/**********************************************/
/***   funcionRP: Repeticiones        ***/
/**********************************************/
int funcionrp(formato_interno * pforitn)
{
   int retorno=0;
   int i=0;

  int msg_original;
  NPSLOGO reg_logo;


  tpclav02 claprice;   /* clave previa de price */ 
  char * clave_pr;
  int num_origen;
  fecha fecha_actual;
  hora hora_actual;
  fecha * pfecha_ope;
  fecha fech_aux;
  hora hor_aux;
  hora * phora_ope;
  char aux[9];
  int segundos_op;
  int segundos_actual;
  int genera_timeout;
  formato_interno *auxlogforitn;


#ifdef TRAZA
fprintLog(nomProg,"\n\n**************\n");
fprintLog(nomProg,"*** FTRATARP ***\n");
fprintLog(nomProg,"****************\n\n"); 
#endif

fverfi(*pforitn);

 pforitn->fidatpro.fiorigen[2]='\0';
 num_origen = atoi(pforitn->fidatpro.fiorigen);
 pforitn->fiisocom.fiiso000[4]='\0';
 tipomsg = atoi(pforitn->fiisocom.fiiso000); 
 msg_original = tipomsg;

 fprintLog(nomProg,"origen ->%d<-\n", num_origen);
 fprintLog(nomProg,"tipomsg ->%d<-\n", tipomsg);
/* 
//PTASK000024640 - INI - Quitado para no tocar el mensaje original
 if (pforitn->fiisocom.fiiso000[3] == '1'
 || pforitn->fiisocom.fiiso000[3] == '5')
 {
     msg_original = tipomsg - 1;
 }
//PTASK000024640 - FIN - Quitado para no tocar el mensaje original
*/
 	 
 sprintf(claprice.lotipopr, "%04d", msg_original);  
 fprintLog(nomProg,"msg_original ->%.*s<-\n", 4, claprice.lotipopr);
 
 ////
 strcpy(claprice.lofehopr,pforitn->fiisocom.fifechor.aa);
 strcpy(claprice.lofehopr+2,pforitn->fiisocom.fifechor.mm);
 strcpy(claprice.lofehopr+4,pforitn->fiisocom.fifechor.dd);
 strcpy(claprice.lofehopr+6,pforitn->fiisocom.fifechor.hh);
 strcpy(claprice.lofehopr+8,pforitn->fiisocom.fifechor.mi);
 strncpy(claprice.lofehopr+10,pforitn->fiisocom.fifechor.ss,2);

 strncpy(claprice.lonumepr, pforitn->fiisocom.fiidtran, sizeof(claprice.lonumepr));

 strncpy(claprice.loadqupr,pforitn->fiisocom.fi032094.longitud,2);
 strncpy(claprice.loadqupr+2,pforitn->fiisocom.fi032094.datos,11);
 clave_pr = (char *)&claprice;

 fprintLog(nomProg,"clave_pr ->%.*s<-\n", sizeof(claprice), clave_pr);

 retorno =0;
 retorno = tratar_fich("PREVIAA",IGUAL,&reg_logo,clave_pr);
 fprintLog(nomProg,"IGUALPREVIAA retorno=%d\n",retorno);
 
 /* la operacion no existe */
 /* se trata como nueva */
 if (retorno)
 {
     fprintLog(nomProg,"No existe una op previa a la repetición espero 1seg y vuelvo a intentar\n");
	 		sleep(1);
     retorno =0;
     retorno = tratar_fich("PREVIAA",IGUAL,&reg_logo,clave_pr);
     fprintLog(nomProg,"IGUALPREVIAA retorno=%d\n",retorno);
}

 	//PRJ288711 - INI - Lo que queremos es que se vuelva a enviar el menaje de totales a tuxedo
 	if ((strcmp(claprice.lotipopr,"1524")) && (retorno == 0))
 	{
 		fprintLog(nomProg,"Ha encontrado previa pero es msg_original ->%.*s<-\n", 4, claprice.lotipopr);
 		retorno = 101;
 		fprintLog(nomProg,"Genero un 101 forzado a retorno = %d\n",retorno);
 	}
 	//PRJ288711 - FIN - Lo que queremos es que se vuelva a enviar el menaje de totales a tuxedo


 if (retorno)
 {	 
     fprintLog(nomProg,"No existe una op previa a la repetición en el segundo intento\n");
	 if(strncmp(pforitn->fiisocom.fiiso000,"1421",4)!=0) // 20160624 -si es 1421 no quiero que cambie el tipo operacion pq se trata distinto en ftratapr
	 {
        sprintf(pforitn->fiisocom.fiiso000, "%04d", msg_original);
	 }
     strncpy(pforitn->fidatpro.ficlatab + 6 , "20", 2);
     fverfi(*pforitn);
	 //vuelve a buscar la tabla de operaciones
     return(99);
 }
 else
 {
    // se encuentra la operacion original: auxlogforitn
    auxlogforitn=(formato_interno*)reg_logo.loforitn;
    fprintLog(nomProg, "reglogo.loforitn >%.30s<\n", reg_logo.loforitn);
    fprintLog(nomProg,"operación original numope>%.6s< codfin>%.3s<\n"
			  ,auxlogforitn->fidatpro.finumope
			  ,auxlogforitn->fidatpro.ficodfin);
	    
    // se guarda la operacion previa
    sprintf(pforitn->fidatpro.finumprv,"%.6s", auxlogforitn->fidatpro.finumope);
		
    if(nspaces(auxlogforitn->fidatpro.ficodfin, 3))
    {
      //operacion en vuelo
      fprintLog(nomProg,"operacion en vuelo, no se responde\n");
    }
    else
    {
      //Operacion resuelta, copiar los datos de respuesta y enviar al origen con return 98
	  // ponemos la marca de que no totaliza porque la original ya habrá totalizado
	  pforitn->fidatpro.finototal[0] = 'N';
	  
      fprintLog(nomProg,"operacion resuelta copiar respuesta y enviar al origen\n");
	  //20160630 - si se respondio ok, pongo ficodfin 015
	  //           si se respondió ko, pongo el mismo que tuviera la original
	  //sprintf(pforitn->fidatpro.ficodfin ,"%.3s" ,auxlogforitn->fidatpro.ficodfin);
	  if(strncmp(pforitn->fidatpro.ficodfin,"000",3)==0)
	  {
	     sprintf(pforitn->fidatpro.ficodfin ,"%.3s" ,"015"); //codfin 015-repe respusal 900-ok
	  }
	  else
	  {
	     sprintf(pforitn->fidatpro.ficodfin ,"%.3s" ,auxlogforitn->fidatpro.ficodfin);
	  }
	  sprintf(pforitn->fidatpro.firesfin , "%.2s", auxlogforitn->fidatpro.firesfin);
	  sprintf(pforitn->fiisocom.respusal ,"%.3s" , auxlogforitn->fiisocom.respusal);
	  sprintf(pforitn->fidatpro.ficomisi,"%.8s", auxlogforitn->fidatpro.ficomisi);
	  sprintf(pforitn->fidatpro.fitipcom,"%.2s", auxlogforitn->fidatpro.fitipcom);
	  sprintf(pforitn->fidatpro.ficomadi,"%.5s", auxlogforitn->fidatpro.ficomadi);
	  	  
	  // 
	  // datos de respuesta de las operaciones financiadas
	  if(strncmp(auxlogforitn->fiprivado.numcuotas,"   ",3)!=0
	  && strncmp(auxlogforitn->fiprivado.numcuotas,"000",3)!=0)
	  {
	      strncpy(pforitn->fiprivado.impopfi     ,auxlogforitn->fiprivado.impopfi, 12);
	      strncpy(pforitn->fiprivado.numcuotas   ,auxlogforitn->fiprivado.numcuotas, 3);
	      strncpy(pforitn->fiprivado.impcuota    ,auxlogforitn->fiprivado.impcuota, 12);
	      strncpy(pforitn->fiprivado.fecprmvto   ,auxlogforitn->fiprivado.fecprmvto, 8);
	      strncpy(pforitn->fiprivado.porintrs    ,auxlogforitn->fiprivado.porintrs, 7);
		 strncpy(pforitn->fiprivado.portae      ,auxlogforitn->fiprivado.portae, 7);
		 strncpy(pforitn->fiprivado.impcomestd  ,auxlogforitn->fiprivado.impcomestd, 12);
		 strncpy(pforitn->fiprivado.porintrsmor ,auxlogforitn->fiprivado.porintrsmor, 7);
		 strncpy(pforitn->fiprivado.portaemor   ,auxlogforitn->fiprivado.portaemor, 7);
		 strncpy(pforitn->fiprivado.impcommor   ,auxlogforitn->fiprivado.impcommor, 12);
		 strncpy(pforitn->fiprivado.impseguro   ,auxlogforitn->fiprivado.impseguro, 12);
		 strncpy(pforitn->fiprivado.imptotopfi  ,auxlogforitn->fiprivado.imptotopfi, 12);
		 strncpy(pforitn->fiprivado.indcmpragrts,auxlogforitn->fiprivado.indcmpragrts, 1);
	  }
	  // si la original es una peticion de act fichero excep negativa
	  if(strncmp(auxlogforitn->fiisocom.fiiso000,"1334",4)==0)
	  {
		     strncpy(pforitn->fidatiso.fiisofic.fific072.longitud,
			    auxlogforitn->fidatiso.fiisofic.fific072.longitud,2);
		     pforitn->fidatiso.fiisofic.fific072.longitud[2] = '\0';
			 
		 strncpy(pforitn->fidatiso.fiisofic.fific072.datos,
			    auxlogforitn->fidatiso.fiisofic.fific072.datos,
				sizeof(auxlogforitn->fidatiso.fiisofic.fific072.datos));
	  }
	  // si la original es peticion de saldo
	  if(strncmp(auxlogforitn->fidatiso.fiisoaut.fiaut003.dig12,"30",2)==0)
	  {
		 sprintf(pforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas,"%.1s",
			auxlogforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas); 
            for (i = 0; i<4; i++)
	        {
	         sprintf(pforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat[i],"%.24s",
		      auxlogforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat);
            }  
      }

	  // si la original es solicitud en cajero y lleva saldos en la respuesta
	  if((strncmp(auxlogforitn->fiisocom.fiiso000,"11",2) ==0 || strncmp(auxlogforitn->fiisocom.fiiso000,"12",2)==0)
	     && strncmp(auxlogforitn->fidatiso.fiisoaut.fiaut003.dig12,"01",2)==0)
	  {
  
		 sprintf(pforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas,"%.1s",
			auxlogforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.num_ctas);  
            for (i = 0; i<4; i++)  
	        {
	         sprintf(pforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat[i],"%.24s",
		      auxlogforitn->fidatiso.fiisoaut.fiaut048.augen.saldos.saldo_dat);
            } 
       }
	   
          // si la original es peticion de movimientos
       if(strncmp(auxlogforitn->fidatiso.fiisoaut.fiaut003.dig12,"31",2)==0)	   
       {
	    sprintf(pforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.moneda, "%.3s" ,
	    auxlogforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.moneda); 
            for (i = 0; i<5; i++)
	        {
	         sprintf(pforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.movimi[i],"%.37s",
		      auxlogforitn->fidatiso.fiisoaut.fiaut048.movim.sub04806.movimi); 
            }
       }
	  
           return(98); // retorna al tratami y envia al origen 
    }//finalizada
 }//encontrada
} /* fin funcion */
