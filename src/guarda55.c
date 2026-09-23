/**********************************************/ 
/*** guarda55: Calcular Tags de respuesta  ***/   
/**********************************************/
//PTASK000024654 - añade comando a la llamada
int guarda55(formato_interno * pforitn, char * comando)
{
  int Long55_gd=0;
  char Long55alf[3];
  char datos_criptograma[250];
  char arpc_calculado[4096];  //tamaño maximo de la respuesta para que no machaque nada 
  int long_arpc = 0;
  int retorno = 0;
  int tipo_cla = 0; //PTASK000024654
  NPSPARA  *reg_para; //PTASK000024654
  int ll = 0; //PTASK000024654
  char comandoI[50]; //PTASK000024654
  char clave_iden[5]; //PTASK000024654
 	char clave1[151]; //PTASK000024654
  char clave2[151]; //PTASK000024654
  char codResp[3]; //PTASK000024654
  char total_91_emp[32];
  //char claveARPC[16];
  char claveARPC[151]; //PTASK000024654
  NPSDHSM reghsm;

//antes de machacar los datos del tag, guardamos los necesarios.

  memset(datos_criptograma, '\0', sizeof(datos_criptograma));   
  memset(arpc_calculado, '\0', sizeof(arpc_calculado)); 

  strncpy(claveARPC,"S0005",5);
  strncpy(claveARPC+5, (char *)pforitn->fidatpro.fimacarq.idarqc,5);
  strncpy(claveARPC+10, "N0000", 5);
  claveARPC[15]='\0';
  long_arpc = 0;

  // PRJ0202598 cambio bin de 6 posiciones 8 no afecta al tamaño de finumPAN
  //PTASK000024654 - se require la tarjeta completa para el ARQC
  //strncpy(datos_criptograma + long_arpc, (char *)pforitn->fidatiso.fiisoaut.finumPAN.datos + 2 ,14);
  strncpy(datos_criptograma + long_arpc, (char *)pforitn->fidatiso.fiisoaut.finumPAN.datos ,16);
  //long_arpc = long_arpc + 14;
	long_arpc = long_arpc + 16;
			
  //PTASK000024654 - se requiren los 3 campos completos del P23
  //strncpy(datos_criptograma + long_arpc,(char *)pforitn->fidatiso.fiisoaut.fiaut023 + 1 ,2);
  strncpy(datos_criptograma + long_arpc,(char *)pforitn->fidatiso.fiisoaut.fiaut023 ,3);
  //long_arpc = long_arpc + 2;
  long_arpc = long_arpc + 3;
  
  strncpy(datos_criptograma + long_arpc,(char *)pforitn->fidatiso.fiisoaut.fiaut055.datos.tag55.tag_9F26,16);
  long_arpc = long_arpc + 16;
#if defined (TRAZA) && (TRAZA > 10)   
  fprintf(stdout,"datos_criptograma >%.*s< codfin ->%s<-\n", long_arpc, datos_criptograma, pforitn->fidatpro.ficodfin);
  fflush(stdout);
#endif  

//montamos el bit 55, cada TAG con su longitud
//TAG 71 y 72 si son necesarios
  //fprintf(stdout, "montamos tag 71\n");
  memset(pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_71,'\0',
  sizeof(pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_71));
		
  //fprintf(stdout, "montamos tag 72\n");
  memset(pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_72,'\0',
			sizeof(pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_72));
	
//TAG 8A	
  //fprintf(stdout, "montamos tag 8A\n");
  pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A[0] = 2;
  Long55_gd++;
  if(!strncmp(pforitn->fidatpro.ficodfin,"000",3))
  {
		pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A[1] = 0x30;
		pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A[2] = 0x30;
		strncpy(datos_criptograma + long_arpc,"3030",4);
		long_arpc = long_arpc + 4;
  }
  else
  {
		pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A[1] = 0x35;
		pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A[2] = 0x38;
		strncpy(datos_criptograma + long_arpc,"3538",4);
		long_arpc = long_arpc + 4;
    //fprintf(stdout,"datos_criptograma >%.*s<\n",long_arpc,datos_criptograma);
    //fflush(stdout);
  }
  Long55_gd=Long55_gd+2;

	
  //TAG 91: CALCULO DEL ARPC
  // datos necesarios para realizar la llamada
  //fprintf(stdout, "datos para tag 91, llamamos a manejhsm\n");
		
  memset(pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91,'\0',
	sizeof(pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91));

  datos_criptograma[long_arpc]='\0';
  
#if defined (TRAZA) && (TRAZA > 10)
  fprintf(stdout,"Para tag 91 datos_criptograma >%.*s<\n",long_arpc,datos_criptograma);
  fflush(stdout);
#endif  
	
//Llamada a la libreria de hsm para calculo de ARPC, por ahora no se llama ya que no tenemos esta ddll
  //PTASK000024654 - No se ha ampliado el área de entrada en la llamada a la funcion manejhsm y ahora esta fallando - INI
  //retorno = manejhsm(claveARPC, "05", datos_criptograma, long_arpc, sfd_hsm, arpc_calculado); 
  strncpy(clave_iden, pforitn->fidatpro.fiorigen, 2);
  strncpy(clave_iden + 2, pforitn->fidatpro.fisubori, 2); 
  clave_iden[4]='\0';
  fprintf(stdout,"Llamada conexhsm_C\n");
  fflush(stdout);
 	for (ll=0 ; ll<2 ; ll++) 
  {
  	strncpy(comandoI, comando, 50);
  	comandoI[50]='\0';
  	conexhsm_C("0001",comando, sockssl, codResp);
   	retorno = claveshsm(comando, clave_iden, "000000", clave1, clave2);
		break;
  }
	strncpy(claveARPC,clave1, strlen(clave1));
	claveARPC[strlen(clave1)]='\0';
  fprintf(stdout,"Datos de entrada >%s< >%d< >%s< >%d< >%s<\n",claveARPC, tipo_cla, datos_criptograma, long_arpc, arpc_calculado);
  fflush(stdout);
  tipo_cla = 0;
	retorno = manejhsm(claveARPC, "05", tipo_cla, datos_criptograma, long_arpc, sockssl, arpc_calculado); 	  
	//PTASK000024654 - FIN
	
//PTASK000024654 - FIN
//  //////////////// si da error por conexion se reintenta
//  if(retorno == -32 || retorno == -145)
//  {
//#if defined (TRAZA) && (TRAZA > 1)  
//      fprintLog(nomProg,"Intenta reconectar\n"); 
//#endif	  
//      memset(&reghsm, ' ', sizeof(NPSDHSM));
//      sfd_hsm=conexhsm("0001", reghsm);//por ahora la ponemos a pelo
//	  if (sfd_hsm > 0)
//	    retorno = manejhsm(claveARPC, "05", datos_criptograma, long_arpc, sfd_hsm, arpc_calculado); 
//	  else
//	    retorno = sfd_hsm; 
// }
//PTASK000024654 - FIN
         
  if (retorno != 16)
  {
#if defined (TRAZA) && (TRAZA > 1)  
	fprintf(stdout,"Error en el calculo del ARPC\n");
#endif	
  }
  else
  {		
    //fprintLog(nomProg,"no hay error en criptograma\n");
	long_arpc=strlen(arpc_calculado)/2 + 2;

	pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91[0] = long_arpc;
	Long55_gd++;
	
#if defined (TRAZA) && (TRAZA > 10)  	
	fprintf(stdout,"manejhsm retorno>%d<, calculo >%s< tamaño ARPC >%d<\n",retorno,
												arpc_calculado,long_arpc);
#endif												
	
	fempapin(strlen(arpc_calculado), total_91_emp, arpc_calculado);    
	
#if defined (TRAZA) && (TRAZA > 10)  	
	fhexdump(stdout, total_91_emp,8);
#endif	
	           
	//strncpy(total_91_emp + strlen(arpc_calculado)/2, pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A + 1, 2);
	// 20160202: no puede ser strncpy pq trabajamos con una comprimida que puede tener caracteres null que cortan la cadena
	memcpy(total_91_emp + strlen(arpc_calculado)/2, pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_8A + 1, 2);
	memcpy(pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91 + 1, total_91_emp,long_arpc );
	Long55_gd = Long55_gd + long_arpc;
			
#if defined (TRAZA) && (TRAZA > 10)  	    
	fhexdump(stdout, pforitn->fidatiso.fiisoaut.fiaut055.datos.resp55.tag_91, long_arpc+1);   
#endif	
		
  }
//guardar tamaño del 55	
  memset(Long55alf,'0',3);
  sprintf(Long55alf,"%03d",Long55_gd); 
  strncpy(pforitn->fidatiso.fiisoaut.fiaut055.longitud,Long55alf,3);

   return 0;

}/* fin funcion*/

