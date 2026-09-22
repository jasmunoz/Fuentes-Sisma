		strncpy(identifica,pfi->fidatiso.fiisocla.ficla053.identificador + 6,2);
		identifica[2]='\0';
		fprintLog(nomProg,"--------------------- Comienza tratamiento de baja de claves ---- Identifica %.2s ---------------\n", identifica);
		for (ll=0 ; ll<2 ; ll++) 
	  {
	  	if (strncmp(pfi->fidatpro.fiorigen,reg_para[ll].psnument,2)==0)
			{	
	  		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
				if (strncmp(identifica,"00",2) == 0) //ZMK y ZIK
				{
					strncpy(comandoBCK1, reg_para[ll].psclzmki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclziki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"01",2) == 0) //ZPK1 y ZAK1 y ZEK1
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzaki, 50);
					strncpy(comandoBCK3, reg_para[ll].psclzeki, 50);
					numerobajas = 3;
					unoodos = 1;
				}
				if (strncmp(identifica,"02",2) == 0) //ZPK2 y ZAK2 y ZEK2
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzaki, 50);
					strncpy(comandoBCK3, reg_para[ll].psclzeki, 50);
					numerobajas = 3;
					unoodos = 2;
				}
				if (strncmp(identifica,"03",2) == 0) //ZPK1 y ZAK1
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzaki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"04",2) == 0) //ZPK2 y ZAK2
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzaki, 50);
					numerobajas = 2;
					unoodos = 2;
				}
				if (strncmp(identifica,"05",2) == 0) //ZPK1 y ZEK1
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"06",2) == 0) //ZPK2 y ZEK2
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 2;
				}
				if (strncmp(identifica,"07",2) == 0) //ZAK1 y ZEK1
				{
					strncpy(comandoBCK1, reg_para[ll].psclzaki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 1;
				}
				if (strncmp(identifica,"08",2) == 0) //ZAK2 y ZEK2
				{
					strncpy(comandoBCK1, reg_para[ll].psclzaki, 50);
					strncpy(comandoBCK2, reg_para[ll].psclzeki, 50);
					numerobajas = 2;
					unoodos = 2;
				}
				if (strncmp(identifica,"11",2) == 0) //ZPK1
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"12",2) == 0) //ZPK2
				{
					strncpy(comandoBCK1, reg_para[ll].psclzpki, 50);
					numerobajas = 1;
					unoodos = 2;
				}
				if (strncmp(identifica,"13",2) == 0) //ZAK1
				{
					strncpy(comandoBCK1, reg_para[ll].psclzaki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"14",2) == 0) //ZAK2
				{
					strncpy(comandoBCK1, reg_para[ll].psclzaki, 50);
					numerobajas = 1;
					unoodos = 2;
				}
				if (strncmp(identifica,"15",2) == 0) //ZEK1
				{
					strncpy(comandoBCK1, reg_para[ll].psclzeki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"16",2) == 0) //ZEK2
				{
					strncpy(comandoBCK1, reg_para[ll].psclzeki, 50);
					numerobajas = 1;
					unoodos = 2;
				}
				if (strncmp(identifica,"21",2) == 0) //ZMK
				{
					strncpy(comandoBCK1, reg_para[ll].psclzmki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				if (strncmp(identifica,"22",2) == 0) //ZIK
				{
					strncpy(comandoBCK1, reg_para[ll].psclziki, 50);
					numerobajas = 1;
					unoodos = 1;
				}
				break;
			} //FIN de if = pnnument
		} //FIN del for
		lespacio = fQuitar_Blancos(comandoBCK1,50);
		lespacio = fQuitar_Blancos(comandoBCK2,50);
		lespacio = fQuitar_Blancos(comandoBCK3,50);
		strncat(comandoBCK1, cBCK, sizeof(comandoBCK1) - strlen(comandoBCK1) - 1);
		strncat(comandoBCK2, cBCK, sizeof(comandoBCK2) - strlen(comandoBCK2) - 1);
		strncat(comandoBCK3, cBCK, sizeof(comandoBCK3) - strlen(comandoBCK3) - 1);
		strncat(comandoBCK1, "                              ", sizeof(comandoBCK1) - strlen(comandoBCK1) - 1);
		strncat(comandoBCK2, "                              ", sizeof(comandoBCK2) - strlen(comandoBCK2) - 1);
		strncat(comandoBCK3, "                              ", sizeof(comandoBCK3) - strlen(comandoBCK3) - 1);
		comandoBCK1[50] = '\0';
		comandoBCK2[50] = '\0';
		comandoBCK3[50] = '\0';
		fprintLog(nomProg,"Comandos BCK >%s< >%s< >%s<\n",comandoBCK1,comandoBCK2,comandoBCK3);
  	if (numerobajas > 0)
		{
    	retorno = claveshsm(comandoBCK1, clave_iden, "000000", clave1, clave2);
    	if (unoodos == 1)
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave principal HSM 1 ----------- -----------------------------------------\n");
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
	   			fprintLog(nomProg,"1--------------------- Damos de baja la clave ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
					retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
	    	}
    		retorno = 0;
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave principal HSM 2 ----------- -----------------------------------------\n");
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
						fprintLog(nomProg,"2--------------------- Damos de baja la clave ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    	else
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave secundaria HSM 1 ---------- -----------------------------------------\n");
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
					fprintLog(nomProg,"3--------------------- Damos de baja la clave ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave secundaria HSM 2 ---------- -----------------------------------------\n");
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
						fprintLog(nomProg,"4--------------------- Damos de baja la clave ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    } //Fin numerobajas > 0
  	if (numerobajas > 1)
		{
    	retorno = claveshsm(comandoBCK2, clave_iden, "000000", clave1, clave2);
    	if (unoodos == 1)
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave principal HSM 1 ----------- -----------------------------------------\n");
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
					fprintLog(nomProg,"1--------------------- Damos de baja la clave ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
					retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
	    	}
    		retorno = 0;
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave principal HSM 2 ----------- -----------------------------------------\n");
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
						fprintLog(nomProg,"2--------------------- Damos de baja la clave ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    	else
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave secundaria HSM 1 ---------- -----------------------------------------\n");
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
					fprintLog(nomProg,"3--------------------- Damos de baja la clave ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave secundaria HSM 2 ---------- -----------------------------------------\n");
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
						fprintLog(nomProg,"4--------------------- Damos de baja la clave ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    } //Fin numerobajas > 1
  	if (numerobajas > 2)
		{
    	retorno = claveshsm(comandoBCK3, clave_iden, "000000", clave1, clave2);
    	if (unoodos == 1)
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave principal HSM 1 ----------- -----------------------------------------\n");
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
					fprintLog(nomProg,"1--------------------- Damos de baja la clave ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
					retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
	    	}
    		retorno = 0;
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave principal HSM 2 ----------- -----------------------------------------\n");
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
						fprintLog(nomProg,"2--------------------- Damos de baja la clave ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		retorno = manejhsm(clave1, "24", 5, clave1, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    	else
    	{
    		retorno = 0;
    		long_hsm = 0;
   			fprintLog(nomProg,"--------------------- Clave secundaria HSM 1 ---------- -----------------------------------------\n");
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
					fprintLog(nomProg,"3--------------------- Damos de baja la clave ----------------------------------\n");
					strncpy(salidabaja, clave_iden, 4);
					strncpy(salidabaja + 4, "1", 1);
					salidabaja[5]='\0';
					fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
	    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
	    		retorno = 0;
	    	}
    		if (retorno == 0)
    		{
    			long_hsm = 0;
	   			fprintLog(nomProg,"--------------------- Clave secundaria HSM 2 ---------- -----------------------------------------\n");
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
						fprintLog(nomProg,"4--------------------- Damos de baja la clave ----------------------------------\n");
						strncpy(salidabaja, clave_iden, 4);
						strncpy(salidabaja + 4, "2", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor clave_iden y n HSM >%.5s<\n", salidabaja);
		    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
		    		retorno = 0;
		    	}
    		}
    	}
    } //Fin numerobajas > 2

		fprintLog(nomProg,"--------------------- Finaliza tratamiento de baja de claves ------------------------------------\n");
