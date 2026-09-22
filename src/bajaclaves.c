		for (ll=0 ; ll<2 ; ll++) 
	  {
	  	if (strncmp(pfi->fidatpro.fiorigen,reg_para[ll].psnument,2)==0)
			{	
	  		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
				if (strncmp(identifica,"00",2) == 0) //ZMK y ZIK
				{
					strncpy(comandoB1, reg_para[ll].psclzmki, 50);
					strncpy(comandoB2, reg_para[ll].psclziki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"01",2) == 0) //ZPK1 y ZAK1 y ZEK1
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					strncpy(comandoB2, reg_para[ll].psclzaki, 50);
					strncpy(comandoB3, reg_para[ll].psclzeki, 50);
					numerobajas = 3;
					unoodos = 1;
				}
				if (strncmp(identifica,"02",2) == 0) //ZPK2 y ZAK2 y ZEK2
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					strncpy(comandoB2, reg_para[ll].psclzaki, 50);
					strncpy(comandoB3, reg_para[ll].psclzeki, 50);
					numerobajas = 3;
					unoodos = 2;
				}
				if (strncmp(identifica,"03",2) == 0) //ZPK1 y ZAK1
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					strncpy(comandoB2, reg_para[ll].psclzaki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"04",2) == 0) //ZPK2 y ZAK2
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					strncpy(comandoB2, reg_para[ll].psclzaki, 50);
					numerobajas = 2;
					unoodos = 2;
				}
				if (strncmp(identifica,"05",2) == 0) //ZPK1 y ZEK1
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					strncpy(comandoB2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"06",2) == 0) //ZPK2 y ZEK2
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					strncpy(comandoB2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 2;
				}
				if (strncmp(identifica,"07",2) == 0) //ZAK1 y ZEK1
				{
					strncpy(comandoB1, reg_para[ll].psclzaki, 50);
					strncpy(comandoB2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"08",2) == 0) //ZAK2 y ZEK2
				{
					strncpy(comandoB1, reg_para[ll].psclzaki, 50);
					strncpy(comandoB2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 2;
				}
				if (strncmp(identifica,"11",2) == 0) //ZPK1
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"12",2) == 0) //ZPK2
				{
					strncpy(comandoB1, reg_para[ll].psclzpki, 50);
					numerobajas = 1;
					unoodos = 2;
				}
				if (strncmp(identifica,"13",2) == 0) //ZAK1
				{
					strncpy(comandoB1, reg_para[ll].psclzaki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"14",2) == 0) //ZAK2
				{
					strncpy(comandoB1, reg_para[ll].psclzaki, 50);
					numerobajas = 1;
					unoodos = 2;
				}
				if (strncmp(identifica,"15",2) == 0) //ZEK1
				{
					strncpy(comandoB1, reg_para[ll].psclzeki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"16",2) == 0) //ZEK2
				{
					strncpy(comandoB1, reg_para[ll].psclzeki, 50);
					numerobajas = 1;
					unoodos = 2;
				}
				if (strncmp(identifica,"21",2) == 0) //ZMK
				{
					strncpy(comandoB1, reg_para[ll].psclzmki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"22",2) == 0) //ZIK
				{
					strncpy(comandoB1, reg_para[ll].psclziki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				break;
			} //FIN de if = pnnument
		} //FIN del for

		long_bla = 50;
		while (long_bla > 0 && comandoB1[long_bla - 1] == ' ')
			long_bla--;
		comandoB1[long_bla] = '\0';
		long_bla = 50;
		while (long_bla > 0 && comandoB2[long_bla - 1] == ' ')
			long_bla--;
		comandoB2[long_bla] = '\0';
		long_bla = 50;
		while (long_bla > 0 && comandoB3[long_bla - 1] == ' ')
			long_bla--;
		comandoB3[long_bla] = '\0';
		fprintLog(nomProg,"1 Comandos BCK >%s< >%s< >%s<\n",comandoB1,comandoB2,comandoB3);
		fflush(nomProg);
		//PRJ0258542 - Fase 3 - Cambio 11052026 - INI
		if (comandoB1[0] != '\0')
		{

	    fprintLog(nomProg, "DENTRO DEL IF\n");
	    fflush(nomProg);
	    fprintLog(nomProg, "Entra 1 >%ld<\n", long_bla);
	    fflush(nomProg);

	    snprintf(comandoBCK1, 51, "%s_BCK", comandoB1);

	    int len = strlen(comandoBCK1);
	    if (len < 50)
	    {
	        memset(comandoBCK1 + len, ' ', 50 - len);
	        comandoBCK1[50] = '\0';
	    }

	    fprintLog(nomProg, "Entra 2\n");
	    fflush(nomProg);

	    fprintLog(nomProg, "Resultado >%s<\n", comandoBCK1);
	    fflush(nomProg);

	    fprintLog(nomProg, "Entra 3\n");
	    fflush(nomProg);
		}
		else
		{
			fprintLog(nomProg, "NO ENTRA EN EL IF\n");
	    fflush(nomProg);
	  }//fin if
		if (comandoB2[0] != '\0')
		{
	    snprintf(comandoBCK2, 51, "%s_BCK", comandoB2);
	    int len = strlen(comandoBCK2);
	    if (len < 50)
	    {
	        memset(comandoBCK2 + len, ' ', 50 - len);
	        comandoBCK2[50] = '\0';
	    }
	    fprintLog(nomProg, "Resultado >%s<\n", comandoBCK2);
	    fflush(nomProg);
		}//fin if
		if (comandoB3[0] != '\0')
		{
	    snprintf(comandoBCK3, 51, "%s_BCK", comandoB3);
	    int len = strlen(comandoBCK3);
	    if (len < 50)
	    {
	        memset(comandoBCK3 + len, ' ', 50 - len);
	        comandoBCK3[50] = '\0';
	    }
	    fprintLog(nomProg, "Resultado >%s<\n", comandoBCK3);
	    fflush(nomProg);
		}//fin if
		//PRJ0258542 - Fase 3 - Cambio 11052026 - FIN
  	if (numerobajas == 1)
		{
			fprintLog(nomProg,"clave_iden bajas 1>%.4s<\n", clave_iden);
			fflush(nomProg);
    	retorno = claveshsm(comandoBCK1, clave_iden, "000000", clave1, clave2); //PRJ0258542 - Fase 3 - Cambio 11052026 - cambiar c
    	if (unoodos == 1)
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave principal ----------------- -----------------------------------------\n");
				fprintLog(nomProg,"Baja al 1 ip_conex1 >%.15s<\n", ip1);
				fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
				fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
				fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
				fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
				fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
				fflush(nomProg);
				conexhsm_1(ip1, puerto, tiempo, cabecera, sockssl, codResp, 1); //PRJ0258542 - Fase 3
				long_hsm = strlen(clave1);
				if (long_hsm > 0)
				{
					//Antes de dar de baja guardamos copia en TR31 de la clave
					tipcla = atoi(identifica);
	   			fprintLog(nomProg,"1--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
					fflush(nomProg);
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					retorno = manejhsm(clave1, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
	   			fprintLog(nomProg,"1--------------------- Damos de baja la clave ----------------------------------\n");
					fflush(nomProg);
					//Antes de dar de baja guardamos copia en TR31 de la clave
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
					fflush(nomProg);
	    		retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
	    	}
    		retorno = 0;
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave principal ----------------- -----------------------------------------\n");
					fprintLog(nomProg,"Baja al 2 ip_conex2 >%.15s<\n", ip2);
					fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
					fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
					fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
					fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
					fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
					conexhsm_1(ip2, puerto, tiempo, cabecera, sockssl, codResp, 2); //PRJ0258542 - Fase 3
					long_hsm = strlen(clave1);
					if (long_hsm > 0)
					{
						//Antes de dar de baja guardamos copia en TR31 de la clave
						tipcla = atoi(identifica);
		   			fprintLog(nomProg,"2--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						retorno = manejhsm(clave1, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
		   			fprintLog(nomProg,"2--------------------- Damos de baja la clave ----------------------------------\n");
						//Antes de dar de baja guardamos copia en TR31 de la clave
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		//20260521--retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    	else
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave secundaria ---------------- -----------------------------------------\n");
				fprintLog(nomProg,"Baja al 1 ip_conex1 >%.15s<\n", ip1);
				fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
				fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
				fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
				fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
				fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
				conexhsm_1(ip1, puerto, tiempo, cabecera, sockssl, codResp, 1); //PRJ0258542 - Fase 3
				long_hsm = strlen(clave2);
				if (long_hsm > 0)
				{
					//Antes de dar de baja guardamos copia en TR31 de la clave
					tipcla = atoi(identifica);
	   			fprintLog(nomProg,"3--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					retorno = manejhsm(clave2, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
	   			fprintLog(nomProg,"3--------------------- Damos de baja la clave ----------------------------------\n");
					//Antes de dar de baja guardamos copia en TR31 de la clave
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		//20260521--retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave secundaria ---------------- -----------------------------------------\n");
					fprintLog(nomProg,"Baja al 2 ip_conex2 >%.15s<\n", ip2);
					fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
					fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
					fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
					fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
					fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
					conexhsm_1(ip2, puerto, tiempo, cabecera, sockssl, codResp, 2); //PRJ0258542 - Fase 3
					long_hsm = strlen(clave2);
					if (long_hsm > 0)
					{
						//Antes de dar de baja guardamos copia en TR31 de la clave
						tipcla = atoi(identifica);
		   			fprintLog(nomProg,"4--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						retorno = manejhsm(clave2, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
		   			fprintLog(nomProg,"4--------------------- Damos de baja la clave ----------------------------------\n");
						//Antes de dar de baja guardamos copia en TR31 de la clave
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		//20260521--retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    }//fin if numerobajas == 1
  	if (numerobajas == 2)
		{
			fprintLog(nomProg,"clave_iden bajas 2>%.4s<\n", clave_iden);
    	retorno = claveshsm(comandoBCK2, clave_iden, "000000", clave1, clave2);
    	if (unoodos == 1)
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave principal ----------------- -----------------------------------------\n");
				fprintLog(nomProg,"Baja al 1 ip_conex1 >%.15s<\n", ip1);
				fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
				fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
				fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
				fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
				fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
				conexhsm_1(ip1, puerto, tiempo, cabecera, sockssl, codResp, 1); //PRJ0258542 - Fase 3
				long_hsm = strlen(clave1);
				if (long_hsm > 0)
				{
					//Antes de dar de baja guardamos copia en TR31 de la clave
					tipcla = atoi(identifica);
	   			fprintLog(nomProg,"5--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					retorno = manejhsm(clave1, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
	   			fprintLog(nomProg,"5--------------------- Damos de baja la clave ----------------------------------\n");
					//Antes de dar de baja guardamos copia en TR31 de la clave
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		//20260521--retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave principal ----------------- -----------------------------------------\n");
					fprintLog(nomProg,"Baja al 2 ip_conex2 >%.15s<\n", ip2);
					fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
					fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
					fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
					fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
					fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
					conexhsm_1(ip2, puerto, tiempo, cabecera, sockssl, codResp, 2); //PRJ0258542 - Fase 3
					long_hsm = strlen(clave1);
					if (long_hsm > 0)
					{
						//Antes de dar de baja guardamos copia en TR31 de la clave
						tipcla = atoi(identifica);
		   			fprintLog(nomProg,"6--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						retorno = manejhsm(clave1, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
		   			fprintLog(nomProg,"6--------------------- Damos de baja la clave ----------------------------------\n");
						//Antes de dar de baja guardamos copia en TR31 de la clave
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		//20260521--retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    	else
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave secundaria ---------------- -----------------------------------------\n");
				fprintLog(nomProg,"Baja al 1 ip_conex1 >%.15s<\n", ip1);
				fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
				fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
				fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
				fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
				fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
				conexhsm_1(ip1, puerto, tiempo, cabecera, sockssl, codResp, 1); //PRJ0258542 - Fase 3
				long_hsm = strlen(clave2);
				if (long_hsm > 0)
				{
					//Antes de dar de baja guardamos copia en TR31 de la clave
					tipcla = atoi(identifica);
	   			fprintLog(nomProg,"7--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					retorno = manejhsm(clave2, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
	   			fprintLog(nomProg,"7--------------------- Damos de baja la clave ----------------------------------\n");
					//Antes de dar de baja guardamos copia en TR31 de la clave
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		//20260521--retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave secundaria ---------------- -----------------------------------------\n");
					fprintLog(nomProg,"Baja al 2 ip_conex2 >%.15s<\n", ip2);
					fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
					fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
					fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
					fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
					fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
					conexhsm_1(ip2, puerto, tiempo, cabecera, sockssl, codResp, 2); //PRJ0258542 - Fase 3
					long_hsm = strlen(clave2);
					if (long_hsm > 0)
					{
						//Antes de dar de baja guardamos copia en TR31 de la clave
						tipcla = atoi(identifica);
		   			fprintLog(nomProg,"8--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						retorno = manejhsm(clave2, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
		   			fprintLog(nomProg,"8--------------------- Damos de baja la clave ----------------------------------\n");
						//Antes de dar de baja guardamos copia en TR31 de la clave
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		//20260521--retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    }//fin if numerobajas == 2
  	if (numerobajas == 3)
		{
			fprintLog(nomProg,"clave_iden bajas 3>%.4s<\n", clave_iden);
    	retorno = claveshsm(comandoBCK3, clave_iden, "000000", clave1, clave2);
    	if (unoodos == 1)
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave principal ----------------- -----------------------------------------\n");
				fprintLog(nomProg,"Baja al 1 ip_conex1 >%.15s<\n", ip1);
				fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
				fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
				fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
				fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
				fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
				conexhsm_1(ip1, puerto, tiempo, cabecera, sockssl, codResp, 1); //PRJ0258542 - Fase 3
				long_hsm = strlen(clave1);
				if (long_hsm > 0)
				{
					//Antes de dar de baja guardamos copia en TR31 de la clave
					tipcla = atoi(identifica);
	   			fprintLog(nomProg,"9--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					retorno = manejhsm(clave1, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
	   			fprintLog(nomProg,"9--------------------- Damos de baja la clave ----------------------------------\n");
					//Antes de dar de baja guardamos copia en TR31 de la clave
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		//20260521--retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave principal ----------------- -----------------------------------------\n");
					fprintLog(nomProg,"Baja al 2 ip_conex2 >%.15s<\n", ip2);
					fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
					fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
					fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
					fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
					fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
					conexhsm_1(ip2, puerto, tiempo, cabecera, sockssl, codResp, 2); //PRJ0258542 - Fase 3
					long_hsm = strlen(clave1);
					if (long_hsm > 0)
					{
						//Antes de dar de baja guardamos copia en TR31 de la clave
						tipcla = atoi(identifica);
		   			fprintLog(nomProg,"10--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						retorno = manejhsm(clave1, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
		   			fprintLog(nomProg,"10--------------------- Damos de baja la clave ----------------------------------\n");
						//Antes de dar de baja guardamos copia en TR31 de la clave
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		//20260521--retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    	else
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave secundaria ---------------- -----------------------------------------\n");
				fprintLog(nomProg,"Baja al 1 ip_conex1 >%.15s<\n", ip1);
				fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
				fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
				fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
				fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
				fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
				conexhsm_1(ip1, puerto, tiempo, cabecera, sockssl, codResp, 1); //PRJ0258542 - Fase 3
				long_hsm = strlen(clave2);
				if (long_hsm > 0)
				{
					//Antes de dar de baja guardamos copia en TR31 de la clave
					tipcla = atoi(identifica);
	   			fprintLog(nomProg,"11--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					retorno = manejhsm(clave2, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
	   			fprintLog(nomProg,"11--------------------- Damos de baja la clave ----------------------------------\n");
					//Antes de dar de baja guardamos copia en TR31 de la clave
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		//20260521--retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave secundaria ---------------- -----------------------------------------\n");
					fprintLog(nomProg,"Baja al 2 ip_conex2 >%.15s<\n", ip2);
					fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
					fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
					fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
					fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
					fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
					conexhsm_1(ip2, puerto, tiempo, cabecera, sockssl, codResp, 2); //PRJ0258542 - Fase 3
					long_hsm = strlen(clave2);
					if (long_hsm > 0)
					{
						//Antes de dar de baja guardamos copia en TR31 de la clave
						tipcla = atoi(identifica);
		   			fprintLog(nomProg,"12--------------------- Exporta la clave a TR31 antes de borrar ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						retorno = manejhsm(clave2, "35", tipcla, claveZMK, 150, sockssl, salidabaja);
		   			fprintLog(nomProg,"12--------------------- Damos de baja la clave ----------------------------------\n");
						//Antes de dar de baja guardamos copia en TR31 de la clave
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		//20260521--retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
			}
		}//fin if numerobajas == 3
	