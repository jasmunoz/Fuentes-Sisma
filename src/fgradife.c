//PRJ0257005 - Revisado tokenizacion
char *itoa_ux(int valor);
void lpad(char *bufer,char c,int lon);
void fechorsys(struct tm_dec *tmPtr);
int  fgradife(formato_interno * pforitn)
{
/****************************************************************/ 
/*--->SE GRABA  EN UN FICHERO DE DIFERIDOS AL HOST              */
/****************************************************************/

#if defined (TRAZA) && (TRAZA > 5)     
 	fprintLog(nomProg,"=======================\n");
 	fprintLog(nomProg,"= Funcion GD 29072025 =\n");
 	fprintLog(nomProg,"= Grabacion diferido  =\n");
 	fprintLog(nomProg,"=======================\n");
#endif 
  fprintLog(nomProg, "-- Variables fgradife.c pforitn %s --\n", pforitn);
  NPSDIFE regdife;
  int numiso000; //tipo de mensaje
  char aux_numiso000[5];//tipo de mensaje  
  tm_dec tmPtr;
  int tipomsg=0;
  char c_tipomsj[5]; 

 formato_interno paux_diforitn;

// fin variables

/****---MODIFICAMOS LA HORA DE LAS CORRECTORAS Y DIFEERIDOS     */
/****---DE SEMP, PARA ASEGURAR LA CORRECTA SECUENCIALIDAD       */
/****---EN EL FICHERO DE DIFERIDOS. Y PARA ASEGURARLA TOTAL-    */
/****---MENTE, LE AÑADIMOS 10 SEGS. MAS QUE LA HORA SISTEMA     */
/****--- NO IMPORTA QUE LOS SEGUNDOS PASEN DE 60                */

 fechorsys(&tmPtr);
 
 sprintf(regdife.disubsis, "%.2s", pforitn->fidatpro.firespri);
 sprintf(regdife.disubapl, "%.2s", pforitn->fidatpro.fisubres); 
 
 sprintf(regdife.difecha0, "%04d", tmPtr.tm_year);
 sprintf(regdife.difecha0+4, "%02d", tmPtr.tm_mon);
 sprintf(regdife.difecha0+6, "%02d", tmPtr.tm_mday);

 sprintf(regdife.dihora00, "%02d", tmPtr.tm_hour);
 sprintf(regdife.dihora00+2, "%02d", tmPtr.tm_min);
 sprintf(regdife.dihora00+4, "%02d", tmPtr.tm_sec+10);

 strncpy(regdife.dinumope, pforitn->fidatpro.finumope, 6);

 memcpy(&paux_diforitn, (formato_interno*)pforitn, sizeof(formato_interno));
 if(!strncmp(paux_diforitn.fiisocom.fiiso000, "14", 2) ||
   !strncmp(paux_diforitn.fiisocom.fiiso000, "04", 2))
 {
	
    fprintLog(nomProg, "fgradife.c iso056.longitud >%.*s<\n",
            sizeof(paux_diforitn.fidatiso.fiisoaut.fiaut056.longitud),
            paux_diforitn.fidatiso.fiisoaut.fiaut056.longitud);
    fprintLog(nomProg, "fgradife.c iso056.datos >%.*s<\n",
            sizeof(paux_diforitn.fidatiso.fiisoaut.fiaut056.datos),
            paux_diforitn.fidatiso.fiisoaut.fiaut056.datos);
 }

  memcpy(regdife.diforitn,&paux_diforitn,sizeof(regdife.diforitn));
  
  // 20160630- grabamos el identificador de la operacion clavpr 
 pforitn->fiisocom.fiiso000[4] = '\0';
 tipomsg = atoi(pforitn->fiisocom.fiiso000);
if (pforitn->fiisocom.fiiso000[3] == '1' ||
    pforitn->fiisocom.fiiso000[3] == '5')
      tipomsg = atoi(pforitn->fiisocom.fiiso000)-1;
	  
 sprintf(c_tipomsj,"%4d",tipomsg);
 
 strncpy(regdife.diclavpr, c_tipomsj, 4);		
 strncpy(regdife.diclavpr + 4, pforitn->fiisocom.fiidtran, 6);		
 strncpy(regdife.diclavpr + 4+6,pforitn->fiisocom.fifechor.aa, 2);	
 strncpy(regdife.diclavpr + 4+6+2,pforitn->fiisocom.fifechor.mm, 2);
 strncpy(regdife.diclavpr + 4+6+2+2,pforitn->fiisocom.fifechor.dd, 2);
 strncpy(regdife.diclavpr + 4+6+2+2+2,pforitn->fiisocom.fifechor.hh, 2);
 strncpy(regdife.diclavpr + 4+6+2+2+2+2,pforitn->fiisocom.fifechor.mi, 2);
 strncpy(regdife.diclavpr + 4+6+2+2+2+2+2,pforitn->fiisocom.fifechor.ss,2);
 strncpy(regdife.diclavpr + 4+6+2+2+2+2+2+2,pforitn->fiisocom.fi032094.longitud, 2);
 strncpy(regdife.diclavpr + 4+6+2+2+2+2+2+2+2, pforitn->fiisocom.fi032094.datos, 11); 
 regdife.diclavpr[35]= '\0';	 
 
 #if defined (TRAZA) && (TRAZA > 10)        
    fprintLog(nomProg, "diclavpr >%s<\n", regdife.diclavpr);
 #endif
 
 
  //PRJ0247167
  fprintLog(nomProg,"*** *** fgradife.c, NPTDIFE - ESCRIBIR\n"); 
  retorno = tratar_fich("NPTDIFE",ESCRIBIR,&regdife,NULL);  
  fprintLog(nomProg,"el retorno de escribir en NPTDIFE es >%d<\n",retorno); 

  return(retorno);
}
