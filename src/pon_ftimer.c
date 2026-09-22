/***************************************************************************
   PROGRAMA:  FPON_TIMER.C
   DESCRIPCION: Funcion encargada de llamar a ftimer y controlar la respuesta.    
    Comun para todos los subsistemas.
****************************************************************************/
int pon_ftimer(formato_interno * pforitn, int pid_ptimer, int segundos, int tipo_tiempo)
{
  int retorno;
  int tiempo_espera;
  int t_absoluto = 0;
  NPSCASA reg_casa;
  char auxChar[11];

  tiempo_espera = segundos;
//#if defined (TRAZA) && (TRAZA > 1)
  fprintLog(nomProg,"[pon_ftimer] tiempo espera >%d<\n", tiempo_espera);
//#endif

  memset(&reg_casa, '\0', sizeof(NPSCASA));
 
  if(tiempo_espera < 0)
     tiempo_espera = 0;

     t_absoluto=ftimer(pforitn, pid_ptimer, tiempo_espera, tipo_tiempo);
     memcpy(reg_casa.canumope, pforitn->fidatpro.finumope, 6);

     if(retorno)
        return(1);

//#if defined (TRAZA) && (TRAZA > 1)
        fprintLog(nomProg,"[pon_ftimer] t_absoluto >%d<\n", t_absoluto);
//#endif
		sprintf(auxChar,"%010d",t_absoluto);
		strncpy(reg_casa.catieabs, auxChar,10);
		sprintf(auxChar,"%010d",tiempo_espera);
		strncpy(reg_casa.catieesp, auxChar,10);
	  fprintLog(nomProg,"pforitn->fiisocom.fiiso000 >%.4s< \n", pforitn->fiisocom.fiiso000); //PRJ0258542 - Fase 3 - Cambio 11052026
	  if (strncmp(pforitn->fiisocom.fiiso000, "1644", 4) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
	  {
	  	fprintLog(nomProg,"Mensaje 1644, no grabar en NPTCASA \n");
	  	return(0);
	  } //PRJ0258542 - Fase 3 - Cambio 11052026
    printf("tamanyos reg_casa.pforitn <%d>,  formato_interno <%d>\n", sizeof(reg_casa.caforitn), sizeof(formato_interno));
    memcpy(&reg_casa.caforitn,pforitn,sizeof(formato_interno));
    retorno = tratar_fich("NPTCASA", ESCRIBIR, &reg_casa, NULL);
  if(retorno)
  {
#if defined (TRAZA) && (TRAZA > 1)
   perror("ESCRIBIR >>>>>>");
   fprintLog(nomProg,"[pon_ftimer] error CASAR: %d\n",errno);
   fprintLog(nomProg, "retorno CASAR: >%d<\n", retorno); 
   fprintLog(nomProg, "canumope: >%.6s<\n", reg_casa.canumope);
   fprintLog(nomProg, "catieabs: >%d<\n", reg_casa.catieabs);
   fprintLog(nomProg, "catieesp: >%d<\n", reg_casa.catieesp);
#endif
   strcpy(reginci.innomfic, "QCASAR");
   strcpy(coderror, "203");
   //Añadido para Monitorizacion SISMA 
   fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Tipo Insert tabla NPTCASA codigo de error >%.3s<\n",coderror); 

   fgraba_incidencia_cp(coderror, NULL,NULL, reginci.innomfic);
  }
  else
  {        
   fprintLog(nomProg,"Escrito en NPTCASA %i\n", reg_casa.canumope);
  }
 
 return(retorno);
}
