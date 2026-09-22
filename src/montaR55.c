//int pone_055Simple(char * p055_simple, char *  codigo,char * Pcampo, int Lcampo); 

void montaR55(formato_interno *pfi, char *p55, int *Lp55)
{
  int Ldat;
  int L_p55;

#if defined (TRAZA) && (TRAZA > 10)
fprintf(stdout,"Entro en montaR55\n");
fflush(stdout);
#endif

 L_p55=0;

 Ldat=pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_71[0];
 if(Ldat)
 {
    //primero el código del tag en un bit
	p55[L_p55] = 0x71;
	L_p55 = L_p55 +1;
	//segundo la longitud del valor en un bit
	p55[L_p55] = Ldat;
	L_p55 = L_p55 +1;
	//tercero el valor de longitud Ldat
	memcpy(p55 + L_p55, pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_71 + 1, Ldat);
	L_p55 = L_p55 + Ldat;
    //L_p55 += pone_055Simple(p55 + L_p55, "71", pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_71 + 1, Ldat);  
 }


 Ldat=pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_72[0];
 if(Ldat)
 {
    //primero el código del tag en un bit
	p55[L_p55] = 0x72;
	L_p55 = L_p55 +1;
	//segundo la longitud del valor en un bit
	p55[L_p55] = Ldat;
	L_p55 = L_p55 +1;
	//tercero el valor de longitud Ldat
	memcpy(p55 + L_p55, pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_72 + 1, Ldat);
	L_p55 = L_p55 + Ldat;
 
  //L_p55 += pone_055Simple(p55 + L_p55, "72", pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_72 + 1, Ldat);
 }

 Ldat=pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91[0];
 if (Ldat)
 {
    //primero el código del tag en un bit
	p55[L_p55] = 0x91;
	L_p55 = L_p55 +1;
	//segundo la longitud del valor en un bit
	p55[L_p55] = Ldat;
	L_p55 = L_p55 +1;
	//tercero el valor de longitud Ldat
	memcpy(p55 + L_p55, pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91 + 1, Ldat);
	L_p55 = L_p55 + Ldat;
 
  //L_p55 += pone_055Simple(p55 + L_p55, "91", pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91 + 1, Ldat);
  
 #if defined (TRAZA) && (TRAZA > 10)
  fhexdump(stdout, pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91, 16);
  fprintf(stdout,"tag 91 sumado\n");
  fflush(stdout);
#endif 
 }

 Ldat=pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A[0];
 if(Ldat)
 {
    //primero el código del tag en un bit 
	p55[L_p55] = 0x8A;
	L_p55 = L_p55 +1;
	//segundo la longitud del valor en un bit
	p55[L_p55] = Ldat;
	L_p55 = L_p55 +1;
	//tercero el valor de longitud Ldat
	memcpy(p55 + L_p55, pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A + 1, Ldat);
	L_p55 = L_p55 + Ldat;
 
  //L_p55 += pone_055Simple(p55 + L_p55, "8A", pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A + 1, Ldat);
#if defined (TRAZA) && (TRAZA > 10)
 fhexdump(stdout, pfi->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A, 4); 
 fprintf(stdout,"tag 8A sumado\n");
 fflush(stdout);
#endif 
 }
 
#if defined (TRAZA) && (TRAZA > 10)
 fprintf(stdout,"p55 en montaR55:\n"); 
 fhexdump(stdout, p55, L_p55);
 fflush(stdout); 
#endif 

 *Lp55=L_p55;
 return; 
}



