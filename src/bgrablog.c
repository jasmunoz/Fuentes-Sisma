//PRJ0257005 - Revisado tokenizacion
/***
   FUNCION: BGRABLOG.C 
   DESCRIPCION: Función para grabar las operaciones en el log de Sisma 
***/
int fgraba_log(formato_interno * pforitn) 
{

  NPSLOGO reglogo;
  char char03[3];
  NOMFICH fich_log;
  tpclav02 * claprice;   /* clave previa de price */      

    fprintLog(nomProg,"     ****************\n");
    fprintLog(nomProg,"%.6s *** BGRABLOG ***\n", pforitn->fidatpro.finumope);
    fprintLog(nomProg,"     ****************\n\n");   

 memset(&reglogo, ' ', sizeof(reglogo));
 strncpy(reglogo.lonumpan, pforitn->fidatiso.fiisofic.fific002.datos,19); 

 strncpy(reglogo.lonumpan+19, "  " , 2);
 strncpy(reglogo.locomerc, "               ",15);

 fprintLog("fgraba_log finumope: %s\n", pforitn->fidatpro.finumope);
 strncpy(reglogo.lonumope, pforitn->fidatpro.finumope, 6);

 strncpy(reglogo.lofecha0, pforitn->fidatpro.fifecha0, 8);
 strncpy(reglogo.lohora00, pforitn->fidatpro.fihora00, 6);

 if ((strncmp(pforitn->fiisocom.fiiso000,"1420",4)==0) || 
     (strncmp(pforitn->fiisocom.fiiso000,"1421",4)==0) ||
     (strncmp(pforitn->fiisocom.fiiso000,"1430",4)==0))
 {
      strncpy(reglogo.loesanul,"1", 1);
 } 
 else 
 {
      strncpy(reglogo.loesanul,"0", 1);
 }     
          
 memcpy(reglogo.loforitn, (char *)pforitn, sizeof(reglogo.loforitn));

   /* en funcion del numero de sesion se graba en un log u otro */

 claprice = (tpclav02 *) reglogo.loclavpr;
 strncpy(claprice->lotipopr, pforitn->fiisocom.fiiso000, 4); 

 strncpy(claprice->lofehopr,pforitn->fiisocom.fifechor.aa, 2);
 strncpy(claprice->lofehopr+2,pforitn->fiisocom.fifechor.mm, 2);
 strncpy(claprice->lofehopr+4,pforitn->fiisocom.fifechor.dd, 2);
 strncpy(claprice->lofehopr+6,pforitn->fiisocom.fifechor.hh, 2);
 strncpy(claprice->lofehopr+8,pforitn->fiisocom.fifechor.mi, 2);
 strncpy(claprice->lofehopr+10,pforitn->fiisocom.fifechor.ss,2);

 strncpy(claprice->lonumepr, pforitn->fiisocom.fiidtran, sizeof(claprice->lonumepr));
 strncpy(claprice->loadqupr, pforitn->fiisocom.fi032094.longitud, 2);
 strncpy(claprice->loadqupr+2, pforitn->fiisocom.fi032094.datos, 11);

 retorno = tratar_fich(NPTLOGO,ESCRIBIR,&reglogo,NULL); 
 fprintLog( "Retorno al escribir LOG   %d\n",retorno);  

 return(retorno);
}
