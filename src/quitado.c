	////AQUI - QUITAR
	fprintLog(nomProg,"------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	fprintLog(nomProg,"******DENTRO DE PARAQUITAR*******\n");

/*
	retorno = 0;
	fprintLog(nomProg,"Conecta al 1 >%.15s<\n", ip_conex1);
	conexhsm_1(ip_conex1, puerto, tiempo, cabecera, sockssl, codResp, 1);
*/
/*
	fprintLog(nomProg,"****************************************\n");
	fprintLog(nomProg,"Ejecuto 0802 Iso0\n");
	strncpy(claveCMac, " ", 150);
	strncpy(claveCMac, "Z000130010000009185240019403F23277332C3C8C470C109000000080000480020924B022FD2C0973283966E7E30336E85F29AABA9B226B140D20FD106EB95B7F2D6F92E", 137);
	strncpy(area_machsm, "3530363735373235303630343134353430323831363831303036303232343635303230323234363532323035303033323034303030303030", 112);
	long_hsm = 112;
	retorno = manejhsm(claveCMac, "07", 5, area_machsm, long_hsm, sockssl, mac_calculado);
	fprintLog(nomProg,"Salida 0802 >%s<\n", mac_calculado);
	fprintLog(nomProg,"****************************************\n");
	return(4);
*/
/*
 	// --- Esto es para meter claves manualmente  -QUITAR cuando funcione todo, esto es solo de pruebas
	//Segun Utimaco, para descifrar las claves TR-31 tenemos que tener una clave de tipo 27 como ZMK para descifrar dichas claves
  fprintLog(nomProg,"Llamada para meter clave Interna de cifrado TR31\n");
	retorno = manejhsm(claveCMac, "25", 27, area_machsm, long_hsm, sockssl, mac_calculado);
	retorno = manejhsm(claveCMac, "25", 27, area_machsm, long_hsm, sockssl, mac_calculado);
	//Meto una llamada de prueba para generar una clave de tranporte interna AES para que me sirva para poder probar
  fprintLog(nomProg,"Llamada para meter clave Interna de transporte\n");
	retorno = manejhsm(claveCMac, "25", 2, area_machsm, long_hsm, sockssl, mac_calculado);
  fprintLog(nomProg,"Llamada para meter clave Interna de CMAC\n");
	retorno = manejhsm(claveCMac, "25", 8, area_machsm, long_hsm, sockssl, mac_calculado);
  fprintLog(nomProg,"Llamada para meter clave Interna de BloqP\n");
	retorno = manejhsm(claveCMac, "25", 3, area_machsm, long_hsm, sockssl, mac_calculado);
	retorno = manejhsm(claveCMac, "25", 3, area_machsm, long_hsm, sockssl, mac_calculado);
	return(4);
  fprintLog(nomProg,"Llamada para meter clave Interna de Datos\n");
	retorno = manejhsm(claveCMac, "25", 7, area_machsm, long_hsm, sockssl, mac_calculado);
	retorno = manejhsm(claveCMac, "25", 7, area_machsm, long_hsm, sockssl, mac_calculado);
	//Meto una llamada de prueba para generar una clave de CMAC interna AES para que me sirva para poder probar
	// --- Esto es para meter claves manualmente  -QUITAR cuando funcione todo, esto es solo de pruebas
*/
/*
	fprintLog(nomProg,"****************************************\n");
	fprintLog(nomProg,"Ejecuto 0619 Iso0\n");
	strncpy(claveExpor, " ", 150);
	strncpy(entrada0619, "S000500028N00065A8283                                                                                                                                 D00320294C4A9A955B7E895EE63947A6A4649L03063D11EB                                                                                                      52993300167850080009", 320);
	long0619 = 320;
	retorno = manejhsm(claveExpor, "39", 0, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"Salida 0619 >%s<\n", salida0619);
	fprintLog(nomProg,"****************************************\n");
	fprintLog(nomProg,"Ejecuto 0619 Iso1\n");
	strncpy(claveExpor, " ", 150);
	strncpy(entrada0619, "S000500028N00065A8283                                                                                                                                 D00320294C4A9A955B7E895EE63947A6A4649L03063D11EB                                                                                                      52993300167850080009", 320);
	long0619 = 320;
	retorno = manejhsm(claveExpor, "39", 1, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"Salida 0619 >%s<\n", salida0619);
	fprintLog(nomProg,"****************************************\n");
	fprintLog(nomProg,"Ejecuto 0619 Iso2\n");
	strncpy(claveExpor, " ", 150);
	strncpy(entrada0619, "S000500028N00065A8283                                                                                                                                 D00320294C4A9A955B7E895EE63947A6A4649L03063D11EB                                                                                                      52993300167850080009", 320);
	long0619 = 320;
	retorno = manejhsm(claveExpor, "39", 2, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"Salida 0619 >%s<\n", salida0619);
	fprintLog(nomProg,"****************************************\n");
	fprintLog(nomProg,"Ejecuto 0619 Iso3\n");
	strncpy(claveExpor, " ", 150);
	strncpy(entrada0619, "S000500028N00065A8283                                                                                                                                 D00320294C4A9A955B7E895EE63947A6A4649L03063D11EB                                                                                                      52993300167850080009", 320);
	long0619 = 320;
	retorno = manejhsm(claveExpor, "39", 3, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"Salida 0619 >%s<\n", salida0619);
	fprintLog(nomProg,"****************************************\n");
	fprintLog(nomProg,"Ejecuto 0619 Iso4\n");
	strncpy(claveExpor, " ", 150);
	strncpy(entrada0619, "S000500028N00065A8283                                                                                                                                 D00320294C4A9A955B7E895EE63947A6A4649L03063D11EB                                                                                                      52993300167850080009", 320);
	long0619 = 320;
	retorno = manejhsm(claveExpor, "39", 7, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"Salida 0619 >%s<\n", salida0619);
	return(4);
*/
/*
	fprintLog(nomProg,"1 Ejecuto 0901 \n");
	strncpy(claveExpor, "D0032203E9A3F1388067004527C459E48EE5BL07063D11EB", 48);
	strncpy(entrada0619, "04B19ADA3C2D53F6E", 17);
	long0619 = 17;
	retorno = manejhsm(claveExpor, "20", 0, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"1 Salida 0901 >%s<\n", salida0619);
	fprintLog(nomProg,"2 Ejecuto 0901 \n");
	strncpy(claveExpor, "D0032203E9A3F1388067004527C459E48EE5BL07063D11EB", 48);
	strncpy(entrada0619, "02EB1A160193B83FB", 17);
	long0619 = 17;
	retorno = manejhsm(claveExpor, "20", 0, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"2 Salida 0901 >%s<\n", salida0619);
*/
/*
	fprintLog(nomProg,"1 Ejecuto 0903 \n");
	strncpy(claveExpor, "D0032203E9A3F1388067004527C459E48EE5BL07063D11EB", 48);
	claveExpor[48]='\0';
	strncpy(entrada0619, "EA7B615F381AA0AC", 16);
	long0619 = 16;
	retorno = manejhsm(claveExpor, "37", 0, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"1 Salida 0903 >%s<\n", salida0619);

	fprintLog(nomProg,"2 Ejecuto 0903 \n");
	strncpy(claveExpor, "D0032203E9A3F1388067004527C459E48EE5BL07063D11EB", 48);
	strncpy(entrada0619, "2EB1A160193B83FB", 16);
	long0619 = 16;
	retorno = manejhsm(claveExpor, "37", 0, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"2 Salida 0903 >%s<\n", salida0619);
	return(4);
*/

/*
	fprintLog(nomProg,"1 Ejecuto 0603 \n");
	strncpy(claveExpor, "D00320294C4A9A955B7E895EE63947A6A4649L03063D11EB", 48); //DES
	//strncpy(claveExpor, "Z000130010000006127691139B3D2C72DF5797F757D5D750900000003000041001040B2D72E6C0728C4CC9CC8841F23932B4779035D17169A1A2B83A6595A6467DEFE8546", 161); //AES
	claveExpor[48]='\0';
	strncpy(entrada0619, "2EB1A160193B83FB5299330016785008", 32);
	long0619 = 32;
	retorno = manejhsm(claveExpor, "36", 0, entrada0619, long0619, sockssl, salida0619);
	fprintLog(nomProg,"1 Salida 0603 >%s<\n", salida0619);
	return(4);
*/
	//Prueba para descubrir el bloque de PIN de una tarjeta y llegar a sacar un PIN en claro - FIN
/*
	//Para exportar de forma interna, una misma clave lmk03 - zpk, en los dos HSM y luego poderla borrar
	retorno = 0;
	fprintLog(nomProg,"Conecta al 1 >%.15s<\n", ip_conex1);
	conexhsm_1(ip_conex1, puerto, tiempo, cabecera, sockssl, codResp, 1);
	strncpy(claveExpor, "Z0001300100000053560B0FCF9F3521949D7A9FBC57AD93090000001B000047002074B79776CBAF79461F82DA0D44CA60889388F11CDCDDB121F8BFF760F6C750E61F02E1", 137);
	strncpy(claveTr31, "D0112P0AB00S0000C0C6ED6EA5C94FBBAA00934371CFAB7B63D4A4C9B5FE2D8BE776CB130B10763BE259675E4010836630B8804648268C22", 112);
	longExpor = 112;
	fprintLog(nomProg,"Ejecuto expor 1 \n");
	retorno = manejhsm(claveExpor, "08", 3, claveTr31, longExpor, sockssl, salidaexpor);
	fprintLog(nomProg,"Salida expor 1 >%s<\n", salidaexpor);
	fprintLog(nomProg,"Conecta al 2 >%.15s<\n", ip_conex2);
	conexhsm_1(ip_conex2, puerto, tiempo, cabecera, sockssl, codResp, 2);
	strncpy(claveExpor, "Z0001300100000053560B0FCF9F3521949D7A9FBC57AD93090000001B000047002074B79776CBAF79461F82DA0D44CA60889388F11CDCDDB121F8BFF760F6C750E61F02E1", 137);
	strncpy(claveTr31, "D0112P0AB00S0000C0C6ED6EA5C94FBBAA00934371CFAB7B63D4A4C9B5FE2D8BE776CB130B10763BE259675E4010836630B8804648268C22", 112);
	longExpor = 112;
	fprintLog(nomProg,"Ejecuto expor 2 \n");
	retorno = manejhsm(claveExpor, "08", 3, claveTr31, longExpor, sockssl, salidaexpor);
	fprintLog(nomProg,"Salida expor 1 >%s<\n", salidaexpor);
	return(4);
	//Para exportar de forma interna, una misma clave lmk03 - zpk, en los dos HSM y luego poderla borrar
*/
	//Para exportar de forma externa, una misma clave en lmk03 y en lmk07, porque la necesitamos para probar el bloque de PIN - INI
/*
	retorno = 0;
	fprintLog(nomProg,"Conecta al 1 >%.15s<\n", ip_conex1);
	conexhsm_1(ip_conex1, puerto, tiempo, cabecera, sockssl, codResp, 1);
	strncpy(claveExpor, "Z00013001000000DD10840BA41B501409F196A84B873D11090000001B0000490020222CB2DBD12BB18DB7A06035B29C006CEE2B1551491A43AC5B51DAFE8F6B5DCEA8B63C", 137);
	strncpy(claveTr31, "D0112P0TB00S00004D71D761DF34C3CC9D22C00CF9CF11A84AAD911855E4D05D93BF527BD5B59E2CBFAD555734BF01A2934BDD30225274A0", 112);
	//strncpy(claveTr31, "D0112P0AB00S0000C0C6ED6EA5C94FBBAA00934371CFAB7B63D4A4C9B5FE2D8BE776CB130B10763BE259675E4010836630B8804648268C22", 112);
	longExpor = 112;
	fprintLog(nomProg,"Ejecuto expor 1 \n");
	strncpy(salidaexpor, clave_iden, 4);
	retorno = manejhsm(claveExpor, "12", 3, claveTr31, longExpor, sockssl, salidaexpor);
	fprintLog(nomProg,"Salida expor 1 >%s<\n", salidaexpor);
	strncpy(claveExpor, "Z00013001000000DD10840BA41B501409F196A84B873D11090000001B0000490020222CB2DBD12BB18DB7A06035B29C006CEE2B1551491A43AC5B51DAFE8F6B5DCEA8B63C", 137);
	strncpy(claveTr31, "D0112D0TB00S00005C50CDF8146E506EDB97288A6FFD3CFCE450DE5B27DE51EF7A0FDEDD73996D25B9C923A2601AA5EDA25A6F22E066E0EC", 112);
	//strncpy(claveTr31, "D0112P0AB00S0000C0C6ED6EA5C94FBBAA00934371CFAB7B63D4A4C9B5FE2D8BE776CB130B10763BE259675E4010836630B8804648268C22", 112);
	longExpor = 112;
	fprintLog(nomProg,"Ejecuto expor 2 \n");
	strncpy(salidaexpor, clave_iden, 4);
	retorno = manejhsm(claveExpor, "12", 7, claveTr31, longExpor, sockssl, salidaexpor);
	fprintLog(nomProg,"Salida expor 1 >%s<\n", salidaexpor);
	return(4);
*/
	//Para exportar de forma externa, una misma clave en lmk03 y en lmk07, porque la necesitamos para probar el bloque de PIN - FIN
	
/*
	//Para dar de baja claves --- INI
	retorno = 0;
	fprintLog(nomProg,"Conecta al 1 >%.15s<\n", ip_conex1);
	conexhsm_1(ip_conex1, puerto, tiempo, cabecera, sockssl, codResp, 1);
	strncpy(claveBaja, "Z00013001000000615C24A1C676A8907669C02774832D55090000000300004A0010C322771E742D477BDF4BB96C90FFD957EFE7EFBEF2B4A4F6D7D468F0F0F9ECD8F4BEB7", 137);
	longBaja = 137;
	fprintLog(nomProg,"Ejecuto baja 1 \n");
	strncpy(salidabaja, clave_iden, 4);
	retorno = manejhsm(claveBaja, "24", 5, claveBaja, longBaja, sockssl, salidabaja);
	if (retorno == 0)
	{
		fprintLog(nomProg,"Conecta al 2 >%.15s<\n", ip_conex2);
		conexhsm_1(ip_conex2, puerto, tiempo, cabecera, sockssl, codResp, 2);
		strncpy(claveBaja, "Z00013001000000615C24A1C676A8907669C02774832D55090000000300004A0010C322771E742D477BDF4BB96C90FFD957EFE7EFBEF2B4A4F6D7D468F0F0F9ECD8F4BEB7", 137);
											//Z00013001000000A591F8F4106F750646EF54A76F0A7F73090000000300004A0010C32277BBA8DBDFED0568A0484238023BFF257258DC37740DC6EBD1F6C20C3641D673C4
		longBaja = 137;
		fprintLog(nomProg,"Ejecuto baja 2 \n");
		strncpy(salidabaja, clave_iden, 4);
		retorno = manejhsm(claveBaja, "24", 5, claveBaja, longBaja, sockssl, salidabaja);
	}
	//Para dar de baja claves --- FIN
*/
	fprintLog(nomProg,"******SALGO DE PARAQUITAR*******\n");
	fprintLog(nomProg,"------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	////AQUI - QUITAR
