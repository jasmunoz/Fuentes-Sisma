
/***********************************************************************
  FUNCION   funcionMS

  DESCRIPCION : tratamiento de mensajes varios procedentes del NODO   
       (PRICE)
            - ficheros
            - control
            - administrativos
************************************************************************
              Función para tratamiento de los mensajes de
              actualización de ficheros, de control de diálogo
              y administrativos
***********************************************************************/
#include "nptpara.h"
#include "nptiden.h" 
#include "nptcasa.h" //PRJ0258542
#include "ftimer.h" //PRJ0258542
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/opensslv.h>
int ftimer(formato_interno * pforitn, int pid_ptimer, int segundos, int tipo_tiempo); //PRJ0258542
int fms_trata_adminis(formato_interno *pforitn);
int fms_trata_control(formato_interno *pforitn);
int fms_trata_claves(formato_interno *pforitn); //PRJ0258542
int fms_calcula_kcv(formato_interno *pforitn); //PRJ0258542 - Fase 3
int fms_trata_excepci(formato_interno *pforitn);
int calculo_cmac(formato_interno *pforitn, char * clave); //PRJ0258542
int pasar_aAES(formato_interno *pforitn, char * claveE, char * kcvE, char * tipo, char * identi, int longi, char * claveS, char * entidadE, char * tiempoE, char * cabeceraE, char * ip1E, char * puertoE, char * ip2E, char * claveZMK); //PRJ0258542
int busca_previa(formato_interno *pforitn); //PRJ0258542
int sacar_claves(formato_interno *pforitn, char * clave1, char * clave2, char * clave3, char * entidadE, char * tiempoE, char * cabeceraE, char * ip1E, char * puertoE, char * ip2E, char * claveZMKs); //PRJ0258542
int fms_preparaTr31(formato_interno *pforitn); //PRJ0258542
long fQuitar_Blancos (char * cadena_entrada, long long_entrada); //PRJ0258542 - Fase 3 - Cambio 11052026
int comprobarHSM(char * entidadS, char * tiempoS, char * cabeceraS, char * ip1S, char * puertoS, char * ip2S); //PRJ0258542 - Fase 3
extern int tratar_fich2(char* ficher, OPCION_FICH opcion, char * registro, long tamanoE); //PRJ0258542 - Fase 3
int cod_err=0;
char aux_datos[4];
char activacion_aux[3];
long lespacio;
NPSPARA  *reg_para;
char pscerrado[1]; //PRJ0258542 - Fase 3 - Cambio 11052026
int funcionms(formato_interno *pforitn)
{

  int retorno;
  int ll; //PRJ0258542 - Fase 3 - Cambio 11052026
  int tipomsg;
	char subsis[2]; //PRJ0258542
	char subapl[2]; //PRJ0258542
	char clave_iden[5]; //PRJ0258542
  int tiempo_espera = 6; //PRJ0258542
  int t_absoluto = 0; //PRJ0258542
  char auxChar[11]; //PRJ0258542
	NPSCASA reg_casa; //PRJ0258542
	memset(&reg_casa, '\0', sizeof(NPSCASA)); //PRJ0258542
	int long33 = 0; //PRJ0258542 - Fase 3
	int long96 = 0; //PRJ0258542 - Fase 3
	int sentido; //PRJ0258542 - Fase 3 -> 0 = Sentido Entidad a Redsys, 1 = Sentido Redsys a Entidad
	if ((retorno = carga_para("00")) != 0) //PRJ0258542 - Fase 3
	{
	   fprintf(stdout,"**** Error carga tabla NPTPARA ****\n");
	}
	
  fprintLog(nomProg,"======================\n");
  fprintLog(nomProg,"= funcionMS 20241016 =\n");
  fprintLog(nomProg,"======================\n");
	for (ll=0 ; ll<2 ; ll++) 
	{
		if ((strncmp(reg_para[ll].psnument,pforitn->fidatpro.fiorigen,2)==0))
		{
			strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
	  	break;
		}
		if (ll > 4)
			break;
	} //Fin del for
	tipomsg = atoi(pforitn->fiisocom.fiiso000);

 	switch(tipomsg)
 	{
	case 1304:
	case 1324:
	   retorno = fms_trata_excepci(pforitn);
	   break;
  case 1804:
  case 1805: //PRJ0258542
  case 1814: //PRJ0258542
  case 1824:
  case 1825: //PRJ0258542 - cambio retono = 0 por retorno = 1
  	if (strncmp(pforitn->fidatiso.fiisocla.ficla033.longitud, "  ", 2) != 0)
  	{
  		long33 = atoi(pforitn->fidatiso.fiisocla.ficla033.longitud); //PRJ0258542 - Fase 3
  	}
  	if (strncmp(pforitn->fidatiso.fiisocla.ficla096.longitud, "  ", 2) != 0)
  	{
  		long96 = atoi(pforitn->fidatiso.fiisocla.ficla096.longitud); //PRJ0258542 - Fase 3
  	}
	 	if (strncmp(pforitn->fidatiso.fiisocla.ficla024, "811", 3) == 0 || strncmp(pforitn->fidatiso.fiisocla.ficla024, "815", 3) == 0 || strncmp(pforitn->fidatiso.fiisocla.ficla024, "816", 3) == 0 || long33 > 0) 
//	 			|| (strncmp(tipomsg, "1814", 4) == 0 && (strncmp(pforitn->fidatiso.fiisocla.ficla039, "800", 3) == 0 || strncmp(pforitn->fidatiso.fiisocla.ficla039, "801", 3) == 0 || strncmp(pforitn->fidatiso.fiisocla.ficla039, "802", 3) == 0)))  //PRJ0258542
		{
			fprintLog(nomProg,"MENSAJE DE CLAVES >%.4s< \n", pforitn->fiisocom.fiiso000); //Llega en todos
			fprintLog(nomProg,"Firespri, fisubres, fiorigen, fisubori >%.2s< >%.2s< >%.2s< >%.2s< y clave_iden >%.4s<\n", pforitn->fidatpro.firespri, pforitn->fidatpro.fisubres, pforitn->fidatpro.fiorigen, pforitn->fidatpro.fisubori, clave_iden); //Llega en todos
			fprintLog(nomProg,"  Valor del P00          >%.4s< \n", pforitn->fiisocom.fiiso000);
			fprintLog(nomProg,"  Valor del P11          >%.6s< \n", pforitn->fidatiso.fiisocla.ficla011); //MAC - Llega en todos
			fprintLog(nomProg,"  Valor del P12          >%.12s< \n", pforitn->fidatiso.fiisocla.ficla012); //MAC - Llega en todos
			fprintLog(nomProg,"  Valor del P24          >%.3s< \n", pforitn->fidatiso.fiisocla.ficla024); //MAC - solo 1804, 1805, 1824, 1825
			fprintLog(nomProg,"  Valor del P25          >%.4s< \n", pforitn->fidatiso.fiisocla.ficla025); //MAC - solo 1804, 1805, 1824, 1825
			fprintLog(nomProg,"  Valor del P33          >%.2s< >%.11s< \n", pforitn->fidatiso.fiisocla.ficla033.longitud, pforitn->fidatiso.fiisocla.ficla033.datos); //MAC - solo 1804, 1805, 1824, 1825
			fprintLog(nomProg,"  Valor del P39          >%.3s< \n", pforitn->fidatiso.fiisocla.ficla039); //MAC - solo 1814 y 1834
			fprintLog(nomProg,"  Valor del P53          >%.2s%.8s%.2s%.4s%.2s%.6s< \n", pforitn->fidatiso.fiisocla.ficla053.control, pforitn->fidatiso.fiisocla.ficla053.identificador, pforitn->fidatiso.fiisocla.ficla053.algoritmo, 
																																									pforitn->fidatiso.fiisocla.ficla053.longitud, pforitn->fidatiso.fiisocla.ficla053.proteccion, pforitn->fidatiso.fiisocla.ficla053.resto);
			fprintLog(nomProg,"  Valor del S93          >%.2s< >%.11s< \n", pforitn->fidatiso.fiisocla.ficla093.longitud, pforitn->fidatiso.fiisocla.ficla093.datos); // en todos
			fprintLog(nomProg,"  Valor del S94          >%.2s< >%.11s< \n", pforitn->fidatiso.fiisocla.ficla094.longitud, pforitn->fidatiso.fiisocla.ficla094.datos); // en todos
			fprintLog(nomProg,"  Valor del S96          >%.3s< >%.360s< \n", pforitn->fidatiso.fiisocla.ficla096.longitud, pforitn->fidatiso.fiisocla.ficla096.datos); //MAC - 1824 y 1824, ¿1814
			fprintLog(nomProg,"  Valor del S128         >%.8s< \n", pforitn->fidatiso.fiisocla.ficlamac);
//PRJ0258542 - Fase 3 - Cambio 11052026  
			fprintf(stdout,"****** Valor del ficlamac 1 ******\n");
    	fhexdump(stdout, pforitn->fidatiso.fiisocla.ficlamac, 8);
    	fflush(stdout);    
//PRJ0258542 - Fase 3 - Cambio 11052026  
			fprintLog(nomProg,"  Valor semaforo falseo  >%.1s<\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026

			if (memcmp(pscerrado,"4",1) == 0 && (strncmp(pforitn->fiisocom.fiiso000,"1804",4) == 0 || strncmp(pforitn->fiisocom.fiiso000,"1824",4) == 0)) //PRJ0258542 - Fase 3 - Cambio 11052026
			{
				fprintLog(nomProg,"-- Semaforo no responder activo, no se envia contestacion --\n"); 
				retorno = -1;
				break;
			}
			
			if (tipomsg == 1814) //Inicio 1814
			{
			  if ((strncmp(pforitn->fidatpro.fiorigen,"01",2) == 0 || strncmp(pforitn->fidatpro.fiorigen,"02",2) == 0 || strncmp(pforitn->fidatpro.fiorigen,"03",2) == 0 || strncmp(pforitn->fidatpro.fiorigen,"04",2) == 0) 
			  		&& strncmp(pforitn->fidatpro.ficlatab + 8, "50", 2) == 0 &&
						(strncmp(pforitn->fidatpro.firespri,"10",2) == 0 || strncmp(pforitn->fidatpro.firespri,"20",2) == 0 
						|| strncmp(pforitn->fidatpro.firespri,"30",2) == 0 || strncmp(pforitn->fidatpro.firespri,"40",2) == 0))
			  {
			  	fprintLog(nomProg,"--> Sentido Redsys a Entidad\n");
			  	strncpy(clave_iden, pforitn->fidatpro.firespri, 2);
			  	strncpy(clave_iden + 2, pforitn->fidatpro.fisubres, 2); 
			  	clave_iden[4]='\0';
			  	fprintLog(nomProg, "Clave_iden >%.4s< \n", clave_iden);
					strncpy(subsis, pforitn->fidatpro.firespri, 2);
					subsis[2] = '\0';
					strncpy(subapl, pforitn->fidatpro.fisubres, 2);
					subapl[2] = '\0';
					sentido = 1;
			 	}
			 	else
			 	{
			  	fprintLog(nomProg,"--> Sentido Entidad a Redsys\n");
			  	strncpy(clave_iden, pforitn->fidatpro.firespri, 2);
			  	strncpy(clave_iden + 2, pforitn->fidatpro.fisubres, 2); 
			  	clave_iden[4]='\0';
			  	fprintLog(nomProg, "Clave_iden >%.4s< \n", clave_iden);
					strncpy(subsis, pforitn->fidatpro.firespri, 2);
					subsis[2] = '\0';
					strncpy(subapl, pforitn->fidatpro.fisubres, 2);
					subapl[2] = '\0';
					strcpy(coderror,"000"); //Aceptadas las nuevas claves - proceso OK
					strcpy(pforitn->fidatpro.ficodfin, "800");
					strcpy(pforitn->fidatpro.firesfin, "00");
					retorno = 1;
					sentido = 0;
			 	}
			} //Fin 1814
			if (tipomsg == 1824 || tipomsg == 1825 || tipomsg == 1804 || tipomsg == 1805) //Inicio 1824/1825
			{
			  if ((strncmp(pforitn->fidatpro.fiorigen,"10",2) == 0 || strncmp(pforitn->fidatpro.fiorigen,"20",2) == 0 || strncmp(pforitn->fidatpro.fiorigen,"30",2) == 0 || strncmp(pforitn->fidatpro.fiorigen,"40",2) == 0)
			  					&& strncmp(pforitn->fidatpro.fisubori,"02",2) == 0)
			  {
			  	fprintLog(nomProg,"--> Sentido Redsys a Entidad\n");
			  	strncpy(clave_iden, pforitn->fidatpro.fiorigen, 2);
			  	strncpy(clave_iden + 2, pforitn->fidatpro.fisubori, 2); 
			  	clave_iden[4]='\0';
			  	fprintLog(nomProg, "Clave_iden >%.4s< \n", clave_iden);
					strncpy(subsis, pforitn->fidatpro.fiorigen, 2);
					subsis[2] = '\0';
					strncpy(subapl, pforitn->fidatpro.fisubori, 2);
					subapl[2] = '\0';
					sentido = 1;
			 	}
			 	else
			 	{
			  	fprintLog(nomProg,"--> Sentido Entidad a Redsys\n");
			  	strncpy(clave_iden, pforitn->fidatpro.firespri, 2);
			  	strncpy(clave_iden + 2, pforitn->fidatpro.fisubres, 2); 
			  	clave_iden[4]='\0';
			  	fprintLog(nomProg, "Clave_iden >%.4s< \n", clave_iden);
					strncpy(subsis, pforitn->fidatpro.firespri, 2);
					subsis[2] = '\0';
					strncpy(subapl, pforitn->fidatpro.fisubres, 2);
					subapl[2] = '\0';
					strcpy(coderror,"000"); //Aceptadas las nuevas claves - proceso OK
					strcpy(pforitn->fidatpro.ficodfin, "800");
					strcpy(pforitn->fidatpro.firesfin, "00");
					retorno = 1;
					sentido = 0;
			 	}
			} //Fin 1824/1825
			//if (strncmp(pforitn->fidatpro.fiorigen,"01",2) == 0 && strncmp(pforitn->fidatpro.ficlatab + 8, "50", 2) == 0 && 
			//																											(strncmp(pforitn->fidatpro.firespri,"10",2) == 0 || strncmp(pforitn->fidatpro.firespri,"20",2) == 0 
			//																											|| strncmp(pforitn->fidatpro.firespri,"30",2) == 0 || strncmp(pforitn->fidatpro.firespri,"40",2) == 0))
			if (sentido == 1)
			{ 
				retorno = 0;
				retorno = calculo_cmac (pforitn, clave_iden); //Llamada para el calculo de CMAC - Solo para lo que llega de Redsys
				if (retorno == 0)
				{
					if (strncmp(pforitn->fiisocom.fiiso000, "1814", 4) == 0 && strncmp(pforitn->fidatiso.fiisocla.ficla039, "800", 3) == 0 && long33 > 0 && long96 > 0) //PRJ0258542 - Fase 3 - calculo KCV con el recibido para comparar
					{
						retorno = fms_calcula_kcv(pforitn);
					}
					if ((strncmp(pforitn->fiisocom.fiiso000,"1824",4) == 0 || strncmp(pforitn->fiisocom.fiiso000,"1825",4) == 0) && strncmp(pforitn->fidatiso.fiisocla.ficla024,"815",3) == 0)
					{
						retorno = fms_trata_claves(pforitn);
					}
					if ((strncmp(pforitn->fiisocom.fiiso000,"1804",4) == 0 || strncmp(pforitn->fiisocom.fiiso000,"1805",4) == 0) && strncmp(pforitn->fidatiso.fiisocla.ficla024,"816",3) == 0)
					{
						retorno = fms_preparaTr31(pforitn);
					}
				}  //Fin retorno = 0 si MAC correcto
				//PRJ0258542
				//fprintf(stdout,"Valores pforitn 2 39, 53 >%.3s<>%.2s<>%.8s<>%.2s<>%.4s<>%.2s<>%.6s<\n",
				//							pforitn->fidatiso.fiisocla.ficla039,
				//							pforitn->fidatiso.fiisocla.ficla053.control, pforitn->fidatiso.fiisocla.ficla053.identificador, pforitn->fidatiso.fiisocla.ficla053.algoritmo, 
				//							pforitn->fidatiso.fiisocla.ficla053.longitud, pforitn->fidatiso.fiisocla.ficla053.proteccion, pforitn->fidatiso.fiisocla.ficla053.resto);
				switch (retorno)
				{
					case 0:
						strcpy(coderror,"800"); //Aceptadas las nuevas claves - proceso OK
						strcpy(pforitn->fidatpro.ficodfin, "800");
						memcpy(pforitn->fidatiso.fiisocla.ficla039, "800",3);
						strcpy(pforitn->fidatpro.firesfin, "00");
						retorno = 1;
						break;
					case 1:
						strcpy(coderror,"801"); //Rechazadas las nuevas claves - proceso KO
						strcpy(pforitn->fidatpro.ficodfin, "801");
						memcpy(pforitn->fidatiso.fiisocla.ficla039, "801",3);
						strcpy(pforitn->fidatpro.firesfin, "01");
						retorno = 1;
						break;
					case 2:
						strcpy(coderror,"802"); //Error verificacion VCC - proceso KO
						strcpy(pforitn->fidatpro.ficodfin, "802");
						memcpy(pforitn->fidatiso.fiisocla.ficla039, "802",3);
						strcpy(pforitn->fidatpro.firesfin, "02");
						retorno = 1;
						break;
					case 3:
						strcpy(coderror,"904"); //Formato de mensaje erroneo - proceso KO
						strcpy(pforitn->fidatpro.ficodfin, "904");
						memcpy(pforitn->fidatiso.fiisocla.ficla039, "904",3);
						strcpy(pforitn->fidatpro.firesfin, "03");
						retorno = 1; 
						break;
					case 4:
						strcpy(coderror,"909"); //Error de sistema - proceso KO
						strcpy(pforitn->fidatpro.ficodfin, "909");
						memcpy(pforitn->fidatiso.fiisocla.ficla039, "909",3);
						strcpy(pforitn->fidatpro.firesfin, "04");
						retorno = 1;
						break;
					case 5:
						strcpy(coderror,"916"); //MAC erroneo - proceso KO
						strcpy(pforitn->fidatpro.ficodfin, "916");
						memcpy(pforitn->fidatiso.fiisocla.ficla039, "916",3);
						strcpy(pforitn->fidatpro.firesfin, "05");
						retorno = 1;
						break;
				} //FIN del switch
			} //FN del control para recepción de mensajes
		}
		else
		{
			fprintLog(nomProg,"Realiza entrada control por P24 = %.3s\n",pforitn->fiisocom.fiiso024);
			retorno = fms_trata_control(pforitn);
		}
		//PRJ0258542
		//fprintf(stdout,"Valores pforitn 3 39, 53 >%.3s<>%.2s<>%.8s<>%.2s<>%.4s<>%.2s<>%.6s<\n",
		//								pforitn->fidatiso.fiisocla.ficla039,
		//								pforitn->fidatiso.fiisocla.ficla053.control, pforitn->fidatiso.fiisocla.ficla053.identificador, pforitn->fidatiso.fiisocla.ficla053.algoritmo, 
		//								pforitn->fidatiso.fiisocla.ficla053.longitud, pforitn->fidatiso.fiisocla.ficla053.proteccion, pforitn->fidatiso.fiisocla.ficla053.resto);

		break;
  case 1644:
		retorno = fms_trata_adminis(pforitn);
		break;
  default:
		fprintLog(nomProg,"recibido mensaje no contemplado :%d\n",tipomsg);
		strcpy(coderror, "280");
		strcpy(pforitn->fidatpro.ficodfin, "280");
		strcpy(pforitn->fidatpro.firesfin, "00");
		retorno = -1;
 	};
	//fprintf(stdout,"Valor de retorno 2>%d<\n", retorno);
 	if (retorno != 1)
  	return (finalizar(98));

	//PRJ0258542
	//fprintf(stdout,"Valores pforitn 4 39, 53 >%.3s<>%.2s<>%.8s<>%.2s<>%.4s<>%.2s<>%.6s<\n",
	//									pforitn->fidatiso.fiisocla.ficla039,
	//									pforitn->fidatiso.fiisocla.ficla053.control, pforitn->fidatiso.fiisocla.ficla053.identificador, pforitn->fidatiso.fiisocla.ficla053.algoritmo, 
	//									pforitn->fidatiso.fiisocla.ficla053.longitud, pforitn->fidatiso.fiisocla.ficla053.proteccion, pforitn->fidatiso.fiisocla.ficla053.resto);
 	return (finalizar(11));
} //FIN funcionms


/***********************************************************************
  FUNCION   fms_trata_adminis

  DESCRIPCION : tratamiento de mensajes administrativos procedentes del NODO

     retorna :     1 . operacion aceptada
                  -1 . operacion denegada
***********************************************************************/

int fms_trata_adminis(formato_interno *pforitn)
{
	int retorno = 0; //PRJ0258542
	//PRJ0258542 - hacemos desarrollo en el if, el resto sigue igual
	if (strncmp(pforitn->fiisocom.fiiso024,"892",2)==0)
	{
		fprintLog(nomProg,"Trata administrativa por error en claves \n");
		fprintLog(nomProg,"Va a buscar la previa con el P72 >%.100s<\n",pforitn->fidatiso.fiisoadm.fiadm072.datos);
		retorno = busca_previa(pforitn);
		if (retorno == 0)
		{
			fprintLog(nomProg,"*** Previa encontrada de 1644 ***\n"); //PRJ0258542 - Fase 3 - Cambio 11052026
		}
		else
		{
			fprintLog(nomProg,"Error al buscar previa o previa no encontrada, no hace nada\n");
 			strcpy(pforitn->fidatpro.firesfin,"00");
 			strcpy(pforitn->fidatpro.ficodfin,"000");
 			// grabamos una incidencia para saber que no se ha hecho nada
 			fgraba_incidencia_cp("644", pforitn->fidatpro.finumope, "trataADM", NULL);
		}
	}
	else
	{
 		fprintLog(nomProg,"Trata administrativa por otros motivos \n"); //PRJ0258542 - Fase 3 - Cambio 11052026
 		strcpy(pforitn->fidatpro.firesfin,"00");
 		strcpy(pforitn->fidatpro.ficodfin,"000");
 		// grabamos una incidencia para que quede registrado que algo va mal y recibimos administrativos
 		fgraba_incidencia_cp("644", pforitn->fidatpro.finumope, "trataADM", NULL);
 	}
	retorno = fgraba_log(pforitn); //PRJ0258542 - Fase 3 - Cambio 11052026
	if (retorno == 0)
	{
		fprintLog(nomProg,"**** Todo OK al grabar el log 1644 ****\n");
	} //PRJ0258542 - Fase 3 - Cambio 11052026

 return (1);
}


/***********************************************************************
  FUNCION   fms_trata_control

  DESCRIPCION : tratamiento de mensajes de control procedentes de PUC

     retorna :     1 . operacion aceptada
                  -1 . operacion denegada
***********************************************************************/

int fms_trata_control(formato_interno *pfi)
{
  int retorno;
  char auxsub[5];
  int posisubs;

 typedef struct Clave
 {
   char   subsis[2];       /*  Subsistema                                      */
   char   subapl[2];       /*  Subaplicacion                                   */
   char   txtresp[8];      /*  texto para la respuesta del diálogo,
                               “DENEGADO”, “ACEPTADO”, “NOCONOCIDO” o espacios */
   char   codfunc[4];      /*  código de función = fiiso024 + \0               */
 } Clave;

 typedef struct RegistroC
 {
   char   numses[3];       /*  número de sesión                                */
   char   fecses[6];       /*  fecha de sesión en formato aammdd               */
   char   semaf01[1];      /*  fecha de sesión en formato aammdd               */
 } RegistroC;

 Clave clave;
 RegistroC registroc;

 fprintLog(nomProg,"trata_control \n");

 fprintLog(nomProg,"pfi->fidatpro.fiorigen >%.2s<\n", pfi->fidatpro.fiorigen);
 fprintLog(nomProg,"pfi->fidatiso.fiisoctr.fictr093.datos >%.2s<\n", pfi->fidatiso.fiisoctr.fictr093.datos);
 fprintLog(nomProg,"pfi->fidatiso.fiisoctr.fictr094.datos >%.2s<\n", pfi->fidatiso.fiisoctr.fictr094.datos); //PRJ0258542
 fprintLog(nomProg,"pfi->fiisocom.fiiso024 >%.3s<\n", pfi->fiisocom.fiiso024);
 
 
 /*     leer registro en nptpars   */
 strncpy(clave.subsis, pfi->fidatpro.fiorigen, 2);
 strncpy(clave.subapl, pfi->fidatiso.fiisoctr.fictr093.datos,2);
 strncpy(clave.txtresp, "        ",8);
 strncpy(clave.codfunc, pfi->fiisocom.fiiso024, 3);
 clave.codfunc[3]='\0';
 
 retorno = tratar_fich("NPTPARS", SEMAF, &registroc, &clave);

	if (retorno)
	{
 		fprintLog(nomProg,"error en reescribir ->%d<-\n", retorno);
   	return(-1);
 	}

 memcpy(auxsub, clave.subsis, 2);
 memcpy(auxsub + 2, clave.subapl, 2);
 auxsub[4] = '\0';
 posisubs= encuentra_svm(psvm, auxsub, TIP_SUBSIS);
 if (posisubs < 0)
 {
#if defined (TRAZA) && (TRAZA > 1)
  fprintf(stderr,"***** ERROR: no encuentra subsistema >%s< en svm : %d\n",
           auxsub, posisubs);
#endif
  return(1);
 }
 (psvm+posisubs)->str_svm.svmpars.pasemsub=(registroc.semaf01[0]-'0');

 //strncpy (pfi->fidatiso.fiisoctr.fictr029 ,   registroc.numses, 3); //PRJ0258542
 //pfi->fidatiso.fiisoctr.fictr029[3] = '\0'; //PRJ0258542
 //strncpy (pfi->fidatiso.fiisoctr.fictr028.aa, registroc.fecses , 2); //PRJ0258542
 //pfi->fidatiso.fiisoctr.fictr028.aa[2] = '\0'; //PRJ0258542
 //strncpy (pfi->fidatiso.fiisoctr.fictr028.mm, registroc.fecses + 2, 2); //PRJ0258542
 //pfi->fidatiso.fiisoctr.fictr028.mm[2] = '\0'; //PRJ0258542
 //strncpy (pfi->fidatiso.fiisoctr.fictr028.dd, registroc.fecses + 4, 2); //PRJ0258542
 //pfi->fidatiso.fiisoctr.fictr028.dd[2] = '\0'; //PRJ0258542

/***   siempre que se recibe algo de puc/price, si el estado es desconectado
       debe pasar a conectado, y despues variar el estado segun el
       mensaje recibido   ***/

//.pasemaf1 es el semáforo actual
//.pasemaf2 es el semaforo anterior

 strcpy(pfi->fidatpro.firesfin , "00");
 strcpy(pfi->fidatpro.ficodfin , "000");
 fprintLog(nomProg,"resfin >%s< codfin >%s<\n",
 pfi->fidatpro.firesfin, pfi->fidatpro.ficodfin);
 return(1);
}


/***********************************************************************
  FUNCION   fms_trata_excepci

  DESCRIPCION : tratamiento de mensajes de ficheros procedentes de PUC

     retorna :     1 . operacion aceptada
                  -1 . operacion denegada
***********************************************************************/

int fms_trata_excepci(formato_interno *pfi)

{
  int  retorno;
  char auxsub[5];
  int posisubs;
  int ll;

    //RAQUEL modificado para coger por bbdd
    for (ll=0 ; ll<2 ; ll++)  
    {
      if ((strncmp(pfi->fidatpro.fiorigen, reg_para[ll].psnument,2)==0)  
       &&   (strncmp(pfi->fidatiso.fiisofic.fific101.datos,"EXCNEG" ,6) !=0))
       {
          strcpy(coderror, "107");
          strcpy(pfi->fidatpro.ficodfin , "107");
          fprintLog(nomProg,"error en nombre de fichero : %.*s\n",6,pfi->fidatiso.fiisofic.fific101.datos);
          return(-1);
       }
       // si el origen es la entidad 03
	   if (strncmp(pfi->fidatpro.fiorigen, reg_para[ll].psresolu,2)==0)  
	   {
           // informa el nombre del fichero
    		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
        sprintf(pfi->fidatiso.fiisofic.fific101.longitud, "%s","6");
        sprintf(pfi->fidatiso.fiisofic.fific101.datos, "%s","EXCNEG");
         
         // informa los bits 93 y 94 con pars
		   strncpy(auxsub, reg_para[ll].psresolu, 2);
		   strncpy(auxsub+2, "01", 2);
		   auxsub[5]='\0';
		   posisubs= encuentra_svm(psvm, auxsub, TIP_SUBSIS);
           if (posisubs < 0)
           {
#if defined (TRAZA) && (TRAZA > 1)
           fprintf(stderr,"***** ERROR: no encuentra subsistema >%s< en svm\n",auxsub); 
#endif
           return(-1);
           }
          else
          {
             sprintf(pfi->fidatiso.fiisoadm.fiadm093.longitud,"%.2s",
              (psvm+posisubs)->str_svm.svmpars.pacidest);
             sprintf(pfi->fidatiso.fiisoadm.fiadm093.datos,"%.11s",
              (psvm+posisubs)->str_svm.svmpars.pacidest + 2);
             sprintf(pfi->fiisocom.fi032094.longitud, "%.2s",
              (psvm+posisubs)->str_svm.svmpars.paciadqui);  
             sprintf(pfi->fiisocom.fi032094.datos, "%.11s",
              (psvm+posisubs)->str_svm.svmpars.paciadqui + 2);
          }
		  break;
        }
	}
 return(1);
}

//PRJ0258542 - INI - Fase 3
/***********************************************************************
  FUNCION   fms_calcula_kcv

  DESCRIPCION : tratamiento para el cálculo del KCV y compara con el recibido

     retorna :     	0 . operacion aceptada = 800
     								2 . error verificacion VCC = 802
***********************************************************************/

int fms_calcula_kcv(formato_interno *pfi)
{
	int retorno, long96, ll;
	long long_hsm = 0;
	char cadenaTr[144];
	char cadenaFin[151];
	char cabecera[16];
	char comandoE[50];
	char comandoM[50];
	char doblecom[100];
 	char clave1[151];
  char clave2[151];
  char psnument[2];
  char claveExp[150];
  char claveMae[150];
  char clave_iden[5];
  int tipoC;
  char tipoCla[2];
  char kcv[6];

	retorno = 0;
	fprintLog(nomProg,"Entrando en fms_calcula_kcv \n");
	strncpy(clave_iden, pfi->fidatpro.firespri, 2);
	strncpy(clave_iden + 2, pfi->fidatpro.fisubres, 2); 
	clave_iden[4]='\0';
	long96 = atoi(pfi->fidatiso.fiisocla.ficla096.longitud);
	strncpy(psnument, pfi->fidatpro.firespri, 2);
	psnument[2] = '\0';
	strncpy(tipoCla, pfi->fidatiso.fiisocla.ficla053.identificador + 6, 2);
	tipoCla[2] = '\0';
	tipoC = atoi(tipoCla);

	fprintLog(nomProg, "Clave_iden >%.4s< \n", clave_iden);
	fprintLog(nomProg, "Valor de pfi->fidatiso.fiisocla.ficla096.datos >%.*s<\n", long96, pfi->fidatiso.fiisocla.ficla096.datos);
	fprintLog(nomProg, "Valor de pfi->fidatiso.fiisocla.ficla053.identificador >%.8s<\n", pfi->fidatiso.fiisocla.ficla053.identificador);
 	for (ll=0 ; ll<2 ; ll++) 
  {
  	if (strncmp(psnument,reg_para[ll].psnument,2)==0)
		{
  		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
			switch (tipoC)
		  {
		   	case 11:
		   	case 12:
		   		strncpy(comandoE,reg_para[ll].psclzpka,50);
		   		break;
		   	case 13:
		   	case 14:
		   		strncpy(comandoE,reg_para[ll].psclzaka,50);
		   		break;
		   	case 15:
		   	case 16:
		   		strncpy(comandoE,reg_para[ll].psclzeka,50);
		   		break;
		   	case 21:
		   		strncpy(comandoE,reg_para[ll].psclzmka,50);
		   		break;
		   	case 22:
		   		strncpy(comandoE,reg_para[ll].psclzika,50);
		   		break;
			}
	  	comandoE[50] = '\0';
			retorno = claveshsm(comandoE, clave_iden, "000000", clave1, clave2);
			
	  	break;
		}
  	if (ll > 4)
  		break;
  } //Fin del for
  
  if (retorno == 0)
  {
		switch (tipoC)
		{
			case 11:
			case 13:
			case 15:
			case 21:
			case 22:
				strncpy(claveExp,clave1,strlen(clave1));
				claveExp[strlen(clave1)] = '\0';
				break;
			case 12:
			case 14:
			case 16:
				strncpy(claveExp,clave2,strlen(clave2));
				claveExp[strlen(clave2)] = '\0';
				break;
		}
		long_hsm = strlen(claveExp);
		retorno = manejhsm(claveExp, "09", tipoC, claveExp, long_hsm, sockssl, cadenaTr); 
		if (retorno != 8)
		{
			fprintLog(nomProg,"TIPO SISTEMA ACCESO HSM: conexion HSM error al obtener el KCV retorno >%i<\n", retorno);
			retorno = 2;
		}
		else
		{
			//fprintLog(nomProg,"-- Salida de manejhsm >%s<\n",cadenaTr);
			//fprintLog(nomProg,"-- KCV recibido >%.6s<\n",pfi->fidatiso.fiisocla.ficla096.datos);
			strncpy(kcv, cadenaTr + 2, 6);
			if (strncmp(pfi->fidatiso.fiisocla.ficla096.datos, kcv, 6) == 0)
			{
				fprintLog(nomProg,"KCV igual al calculado, recibido >%.6s< y calculado >%.6s<\n",pfi->fidatiso.fiisocla.ficla096.datos,kcv);
				retorno = 0;
				fprintLog(nomProg," --- Valor del semaforo de falseo - %s - ---\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026
	    	if (memcmp(pscerrado,"5",1) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
	    	{
	    		fprintLog(nomProg,"-- Aunque son iguales, se procede al falseo --\n"); 
	    		retorno = 2;
	    	}
			}
			else
			{
				fprintLog(nomProg,"KCV distinto al calculado, recibido >%.6s< y calculado >%.6s<\n",pfi->fidatiso.fiisocla.ficla096.datos,kcv);
				retorno = 2;
			}
		}
  }

	fprintLog(nomProg,"Saliendo de fms_calcula_kcv \n");
  return(retorno);
} //Fin fms_calcula_kcv

/***********************************************************************
  FUNCION   fms_trata_claves

  DESCRIPCION : tratamiento de mensajes de control para intercambio de claves

     retorna :     	0 . operacion aceptada = 800
     								1 . operacion rechazada = 801
     								2 . error verificacion VCC = 802
     								3 . formato de mensaje erroneo = 904
     								4 . error de sistema = 909
     								5 . error de MAC = 916
                   98 . operacion denegada
***********************************************************************/

int fms_trata_claves(formato_interno *pfi)
{
	int retorno, long_cla;
	char subsis[2];
	char subapl[2];
	char entidad[4];
	char cabecera[4];
	char ip1[15];
	char puerto[4];
	char tiempo[4];
	char ip2[15];
	char codResp[3];
	char tipocla1[3];
	char tipocla2[3];
	char tipocla3[3];
	char cadenaP[466]; //PRJ0258542 - Fase 3 - Cambio 11052026 - aumenta a 466
	char cadenaR[154];
	char clave1[150];
	char clave2[150];
	char clave3[150];
	char claveI1[150];
	char claveI2[150];
	char claveI3[150];
	char identifica[3];
	int tipcla;
	char clave_iden[5];
	char claveZMK[151];
	int numerobajas, unoodos, ll;
	char comandoB1[51];
	char comandoB2[51];
	char comandoB3[51];
	char comandoBCK1[51]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char comandoBCK2[51]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char comandoBCK3[51]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char cBCK[4]; //PRJ0258542 - Fase 3 - Cambio 11052026
	strncpy(cBCK, "_BCK", 4); //PRJ0258542 - Fase 3 - Cambio 11052026
	long long_bla = 0; //PRJ0258542 - Fase 3 - Cambio 11052026
	char espacios[51]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char salidabaja[200];
	long long_hsm = 0;
	fprintLog(nomProg,"--------------------- Entrada en rutina fms_trata_claves ----------------------------------------\n");
	strncpy(claveI1,"                                                                                                                                                      ", 150);
	strncpy(claveI2,"                                                                                                                                                      ", 150);
	strncpy(claveI3,"                                                                                                                                                      ", 150);
	strncpy(subsis, pfi->fidatpro.fiorigen, 2);
	strncpy(subapl, pfi->fidatpro.fisubori, 2);
	//pforitn->fidatpro.firespri, pforitn->fidatpro.fisubre
  strncpy(clave_iden, pfi->fidatpro.fiorigen, 2);
  strncpy(clave_iden + 2, pfi->fidatpro.fisubori, 2); 
  fprintLog(nomProg, "Clave_iden >%.4s< \n", clave_iden);
  numerobajas = 0;
  unoodos = 0;
  strncpy(espacios,"                                                  ",50); //PRJ0258542 - Fase 3 - Cambio 11052026
  strncpy(comandoBCK1, espacios, 50);
  strncpy(comandoBCK2, espacios, 50);
  strncpy(comandoBCK3, espacios, 50);
  strncpy(entidad,"    ",4);
	long_cla = 0;
  
	if ((strncmp(pfi->fiisocom.fiiso000,"1824",4) == 0 || strncmp(pfi->fiisocom.fiiso000,"1825",4) == 0) && strncmp(pfi->fiisocom.fiiso024,"815",3) == 0) //Se valida KCV solo en estos casos
	{
		fprintLog(nomProg,"Proceso de cambio de claves con parametrizacion: \n");
		fprintLog(nomProg,"  Iso 000  >%.4s<\n",pfi->fiisocom.fiiso000);
		fprintLog(nomProg,"  Iso 024  >%.3s<\n",pfi->fiisocom.fiiso024);
		fprintLog(nomProg,"  Iso 053  >%.8s<\n",pfi->fidatiso.fiisocla.ficla053.identificador);

	  //Verificacion de que los dos HSM están operativos
	  
	  retorno = comprobarHSM(entidad, tiempo, cabecera, ip1, puerto, ip2); //PRJ0258542 - Fase 3
	  if (retorno != 0) //Se tiene que devolver error para no realizar acciones, 4 = 909 = error de sistema
	  {
	  	return(retorno);
  	}
		//Primero se tiene que importar la clave al sistema y verificar que el KCV es correcto
		retorno = sacar_claves(pfi, claveI1, claveI2, claveI3, entidad, tiempo, cabecera, ip1, puerto, ip2, claveZMK);
		fprintLog(nomProg,"  Retorno sacar_claves  >%d<\n",retorno);
		if (retorno != 0)
		{
			fprintLog(nomProg,"ERROR en la verificacion de KCV o importacion de claves\n");
			return(retorno);
		}
	} //Fin 815 para 1824 y 1825
	if ((strncmp(pfi->fiisocom.fiiso000,"1824",4) == 0 || strncmp(pfi->fiisocom.fiiso000,"1825",4) == 0) && strncmp(pfi->fiisocom.fiiso024,"815",3) == 0) //Se se sigue con el proceso para estos casos
	{
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

		fprintLog(nomProg,"  Iso 053 + 6  >%.2s<\n",identifica);
		fprintLog(nomProg,"  ClaveI1  >%.150s<\n",claveI1);
		fprintLog(nomProg,"  ClaveI2  >%.150s<\n",claveI2);
		fprintLog(nomProg,"  ClaveI3  >%.150s<\n",claveI3);

		fprintLog(nomProg,"--------------------- Comienza tratamiento de actualizacion -------------------------------------\n");
		cadenaP[466] = '\0'; //PRJ0258542 - Fase 3 - Cambio 11052026 - aumenta a 466
		//Si todo va bien mete la clave en el sistema, solo mensajes 1824 y 1825 -- PRUEBA
		// Si la grabacion de claves ha ido correcta, las metemos en la llamada al procedimiento para actualizar
		memcpy(cadenaP, 		subsis, 2);
		memcpy(cadenaP + 2, subapl, 2);
		if (strlen(claveI1) == 0)
		{
			fprintLog(nomProg,"Inicializa espacios claveI1\n");
			strncpy(claveI1,"                                                                                                                                                      ",150);
			claveI1[150]='\0';
		}
		if (strlen(claveI2) == 0)
		{
			fprintLog(nomProg,"Inicializa espacios claveI2\n");
			strncpy(claveI2,"                                                                                                                                                      ",150);
			claveI2[150]='\0';
		}
		if (strlen(claveI3) == 0)
		{
			fprintLog(nomProg,"Inicializa espacios claveI3\n");
			strncpy(claveI3,"                                                                                                                                                      ",150);
			claveI3[150]='\0';
		}
		
		if (strncmp(identifica,"00",2) == 0) //ZMK y ZIK
		{
			strncpy(cadenaP + 4, "12", 2);
			strncpy(cadenaP + 6, "ZMK", 3);
			strncpy(cadenaP + 9, "ZIK", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"01",2) == 0) //ZPK1 y ZAK1 y ZEK1
		{
			strncpy(cadenaP + 4, "13", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "ZAK", 3);
			strncpy(cadenaP + 12, "ZEK", 3);
		}
		if (strncmp(identifica,"02",2) == 0) //ZPK2 y ZAK2 y ZEK2
		{
			strncpy(cadenaP + 4, "23", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "ZAK", 3);
			strncpy(cadenaP + 12, "ZEK", 3);
		}
		if (strncmp(identifica,"03",2) == 0) //ZPK1 y ZAK1
		{
			strncpy(cadenaP + 4, "12", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "ZAK", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"04",2) == 0) //ZPK2 y ZAK2
		{
			strncpy(cadenaP + 4, "22", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "ZAK", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"05",2) == 0) //ZPK1 y ZEK1
		{
			strncpy(cadenaP + 4, "12", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "ZEK", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"06",2) == 0) //ZPK2 y ZEK2
		{
			strncpy(cadenaP + 4, "22", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "ZEK", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"07",2) == 0) //ZAK1 y ZEK1
		{
			strncpy(cadenaP + 4, "12", 2);
			strncpy(cadenaP + 6, "ZAK", 3);
			strncpy(cadenaP + 9, "ZEK", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"08",2) == 0) //ZAK2 y ZEK2
		{
			strncpy(cadenaP + 4, "22", 2);
			strncpy(cadenaP + 6, "ZAK", 3);
			strncpy(cadenaP + 9, "ZEK", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"11",2) == 0) //ZPK1
		{
			strncpy(cadenaP + 4, "11", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"12",2) == 0) //ZPK2
		{
			strncpy(cadenaP + 4, "21", 2);
			strncpy(cadenaP + 6, "ZPK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"13",2) == 0) //ZAK1
		{
			strncpy(cadenaP + 4, "11", 2);
			strncpy(cadenaP + 6, "ZAK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"14",2) == 0) //ZAK2
		{
			strncpy(cadenaP + 4, "21", 2);
			strncpy(cadenaP + 6, "ZAK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"15",2) == 0) //ZEK1
		{
			strncpy(cadenaP + 4, "11", 2);
			strncpy(cadenaP + 6, "ZEK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"16",2) == 0) //ZEK2
		{
			strncpy(cadenaP + 4, "21", 2);
			strncpy(cadenaP + 6, "ZEK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"21",2) == 0) //ZMK
		{
			strncpy(cadenaP + 4, "11", 2);
			strncpy(cadenaP + 6, "ZMK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		if (strncmp(identifica,"22",2) == 0) //ZIK
		{
			strncpy(cadenaP + 4, "11", 2);
			strncpy(cadenaP + 6, "ZIK", 3);
			strncpy(cadenaP + 9, "   ", 3);
			strncpy(cadenaP + 12, "   ", 3);
		}
		strncpy(cadenaP + 15, "A", 1); //PRJ0258542 - Fase 3 - Cambio 11052026 - concatena 
		memcpy(cadenaP + 16, claveI1, 150);
		memcpy(cadenaP + 166, claveI2, 150);
		memcpy(cadenaP + 316, claveI3, 150);
		strcat(cadenaP, cadenaR);
		cadenaP[466] = '\0'; //PRJ0258542 - Fase 3 - Cambio 11052026 - aumenta a 466
		fprintLog(nomProg,"  cadenaP para envio a CAMBIOCLAVES >%.466s<\n",cadenaP);
		retorno = tratar_fich2("NPTPARA",ESCRIBIR,cadenaP,466); 
		if (retorno<0)
		{
			fprintLog(nomProg, "Error al obtener la clave\n");
		  fprintLog(nomProg,"TIPO SISTEMA ACCESO HSM: conexion HSM  error al obtener la clave retorno>%i<\n", retorno);

		  strcpy(coderror, "904");
			strcpy(reginci.innomfic,"HSM");
		  return(3); 
		}

	} //Fin 815 para 1824 y 1825

	//fprintLog(nomProg,"Llega al final y sale con retorno %i\n", retorno);
	fprintLog(nomProg,"--------------------- Salida de rutina fms_trata_claves -----------------------------------------\n");
  return(retorno);
} //Fin fms_trata_claves

/***********************************************************************
  FUNCION   calculo_cmac

  DESCRIPCION : calculo de MAC para mensaje de claves

     retorna :     	0 . todo correcto
                  !=0 . error de HSM
***********************************************************************/

int calculo_cmac(formato_interno *pfi, char * clave)
{
 	NPSIDEN regiden; 
  NPSDHSM reghsm;
 	int tipo_cmac, long_mac, long96;
  int ll;
 	int retorno = 0; 
 	long long_hsm = 0;
 	int ii;
	char longc96[3];
 	unsigned char cc;
 	char linea[4096];
 	char * addr;
 	char comando[50];
 	char clave_iden[5];
 	char clave_2[2];
 	char zona_mac[2];
 	char tipo_mac[2];
 	char area_mac[1024];
 	char area_machsm[4096];
 	char datos_hsm[4096];
 	char mac_desempaq[16];
 	char mac_calculado[4096];
 	char arqc_verificado[4096];  
 	char datos_criptograma[200];
 	char clave1[151];
  char clave2[151];
  char claveCMac[151];
  char claveEMV[151];
  char codResp[3];
  char clavecone[4];
  long96 = 0;
  strncpy(longc96,"000",3);
  fprintLog(nomProg,"--------------------- Entrada en rutina calculo_cmac --------------------------------------------\n");
  
///------------------  
 	//Prepara area para MAC - devolver un 916 si el MAC es erroneo
 	long_mac = 0;
	
	memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla011, 6);
	long_mac = long_mac + 6;
	memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla012, 12);
	long_mac = long_mac + 12;
	if (strncmp(pfi->fidatiso.fiisocla.ficla024, "   ", 3) != 0)
	{
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla024, 3);
		long_mac = long_mac + 3;
	}
	if (strncmp(pfi->fidatiso.fiisocla.ficla025, "    ", 4) != 0)
	{
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla025, 4);
		long_mac = long_mac + 4;
	}
 	memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla033.longitud, 2); //PRJ0258542 - Fase 3
	long_mac = long_mac + 2; //PRJ0258542 - Fase 3
 	memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla033.datos, atoi(pfi->fidatiso.fiisocla.ficla033.longitud));
	long_mac = long_mac + atoi(pfi->fidatiso.fiisocla.ficla033.longitud);
	if (strncmp(pfi->fidatpro.ficlatab + 8, "50", 2) == 0)
	{
 		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla039, 3);
		long_mac = long_mac + 3;
	}
	if (strncmp(pfi->fidatpro.ficlatab + 8, "50", 2) != 0)
	{
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla053.control, 2);
		long_mac = long_mac + 2;
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla053.identificador, 8);
		long_mac = long_mac + 8;
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla053.algoritmo, 2);
		long_mac = long_mac + 2;
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla053.longitud, 4);
		long_mac = long_mac + 4;
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla053.proteccion, 2);
		long_mac = long_mac + 2;
		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla053.resto, 6);
		long_mac = long_mac + 6;
	}

	if (strncmp(pfi->fidatiso.fiisocla.ficla096.longitud, "000", 3) != 0) //PRJ0258542 - Fase 3 - cambio - pongo este control y quito el siguiente que fallo el dia 10/06/2026
	{
 		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla096.longitud, 3);
		long_mac = long_mac + 3;
		strncpy(longc96,pfi->fidatiso.fiisocla.ficla096.longitud,3);
		long96 = atoi(longc96);
 		memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla096.datos, long96);
		long_mac = long_mac + long96;
	}
	//PRJ0258542 - Fase 3 - cambio - quito este control que fallo el dia 10/06/2026 y meto el anterior
	//strncpy(longc96,pfi->fidatiso.fiisocla.ficla096.longitud,3);
	//long96 = atoi(longc96);
	//if (long96 > 0)
	//{
 	//	memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla096.longitud, 3); //PRJ0258542 - Fase 3
	//	long_mac = long_mac + 3; //PRJ0258542 - Fase 3
 	//	memcpy(area_mac + long_mac, pfi->fidatiso.fiisocla.ficla096.datos, long96);
	//	long_mac = long_mac + long96;
	//}
	
	area_mac[long_mac]='\0';

	fprintLog(nomProg,"* - * - * Entro en zona MAC para mensaje claves en entrada * - * - *\n");
	//fprintf(stdout,"Long de CMAC10 %d\n", long_mac);
	//fprintf(stdout,"Area de CMAC >%.*s<\n", long_mac, area_mac);
	fhexdump(stdout,area_mac,long_mac);
	fflush(stdout);
 	memset(mac_calculado,'\0',sizeof(mac_calculado));
 	mac_calculado[4096] = '\0';
  memset(area_machsm,'\0',sizeof(area_machsm));
  memset(datos_hsm,'\0',sizeof(datos_hsm));
  memset(mac_desempaq ,'\0', sizeof(mac_desempaq)); 
  memset(arqc_verificado,'\0',sizeof(arqc_verificado));
  memset(datos_criptograma, '\0', sizeof(datos_criptograma)); 

///------------------ Para obtener ZIK de SIAM_IDEN
	//strncpy(clave_iden, pfi->fidatpro.firespri, 2);
  //strncpy(clave_iden + 2, pfi->fidatpro.fisubres, 2); //PRJ0258542 - Fase 3 - cambio
  strncpy(clave_iden, clave, 4);
  clave_iden[4]='\0';
  strncpy(clave_2, clave, 2);
	fprintLog(nomProg, "Clave_iden >%.4s<\n", clave_iden);
  
 	for (ll=0 ; ll<2 ; ll++) 
  {
  	//if (strncmp(pfi->fidatpro.firespri,reg_para[ll].psnument,2)==0)
  	if (strncmp(clave_2,reg_para[ll].psnument,2)==0)
		{
	  	strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
			strncpy(tipo_mac, pfi->fidatiso.fiisocla.ficla053.algoritmo, 2);
			fprintLog(nomProg,"- Numope >%.6s<\n",pfi->fidatpro.finumope);
			fprintLog(nomProg, "Valor de P53 algoritmo >%.2s< \n ", tipo_mac);
			tipo_cmac = atoi(tipo_mac);
      switch (tipo_cmac)
      {
       	case 00: //TDES sin PIN
       	case 01: //Algoritmo DES
        	strncpy(comando,reg_para[ll].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 02: //Algoritmo 3DES
        case 03: //TDES
        	strncpy(comando,reg_para[ll].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 05: //AES - pagina 16 del comunicado 04/24, se calcula con la ZIK activa
        	strncpy(comando,reg_para[ll].psclzika,50);
        	comando[50]='\0';
        	break;
 			}
 			fprintLog(nomProg, "Comando >%s< \n",comando);
    	retorno = claveshsm(comando, clave_iden, "000000", clave1, clave2);
			break;
    }
  }
	if (retorno<0)
	{
		fprintLog(nomProg, "Error al obtener la clave\n");
	  fprintLog(nomProg,"TIPO SISTEMA ACCESO HSM: conexion HSM  error al obtener la clave retorno>%i<\n", retorno);

	  strcpy(coderror, "904");
		strcpy(reginci.innomfic,"HSM");
	  return(3); 
	}
  strncpy(zona_mac,pfi->fidatiso.fiisocla.ficla053.proteccion, 2);
  zona_mac[2]='\0';
	//fprintLog(nomProg, "Valor de clave ZIK activa >%.151s<\n ", clave1);
	strncpy(pfi->fidatpro.fimacarq.idmac, clave1+5,5);
	strncpy(claveCMac,clave1, strlen(clave1)); //Llevara la clave ZIK activa para este calculo
 	claveCMac[strlen(clave1)]='\0';
	
///------------------ Para obtener ZIK de SIAM_IDEN

  addr = area_mac; 
  for (ii=0; ii < strlen(area_mac); ii++)
  {
		cc = *(addr + ii);
		sprintf(linea+2*ii, "%02X", cc);
  }
  
  strncpy(area_machsm,linea,strlen(area_mac)*2);
  long_hsm = (strlen(area_mac)*2);
  area_machsm[long_hsm]='\0';

  strncpy(datos_hsm, area_machsm, long_hsm);
  datos_hsm[long_hsm]='\0';
  fprintLog(nomProg, "claveCMac >%s< y area_machsm >%s<\n",claveCMac, area_machsm);
  retorno = manejhsm(claveCMac, "07", 5, area_machsm, long_hsm, sockssl, mac_calculado); 
  
  if(retorno == -32 || retorno == -145)
	{
		fprintLog(nomProg,"Intenta reconectar\n"); 
    memset(&reghsm, ' ', sizeof(NPSDHSM));
    conexhsm_C("0001",reg_para[ll].psclvhsm, sockssl, codResp);
    if (!strcmp(codResp, "01")) 
    {
			retorno = manejhsm(claveCMac, "07", tipo_cmac, area_machsm, long_hsm, sockssl, mac_calculado); 
    }
   	else
	   	retorno = 99;
  }

  if (retorno != 32) 
  {
  	fprintLog(nomProg,"Error en respuesta a calculo de cmac\n");
    strcpy(pfi->fidatpro.firesfin, "00");
		strcpy(reginci.innomfic,"MAC"); 
		return(4);
  }
  else
  {
		//fprintf(stdout,"Valores de pfi->fidatiso.fiisocla.ficlamac >%.8s< \n", pfi->fidatiso.fiisocla.ficlamac);
		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
		fprintLog(nomProg," --- Valor del semaforo de falseo - %s - ---\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026
    fdesepin(8,pfi->fidatiso.fiisocla.ficlamac, mac_desempaq);
		fprintLog(nomProg,"- CMAC para Numope >%.6s<\n",pfi->fidatpro.finumope);
    fprintLog(nomProg," MAC calculado en HSM AES: >%s<\n", mac_calculado);   
		fprintLog(nomProg," MAC desempaquetado   AES: >%.16s<\n", mac_desempaq); 
   	if (memcmp(mac_desempaq,"0000000000000000",16) &&  memcmp(mac_desempaq,mac_calculado,16)) // para compararlo tengo en cuenta los 16 caracteres
    {
    	fprintLog(nomProg,"--------------------- Salida de rutina calculo_cmac - son distintos -----------------------------\n");
			strcpy(coderror, "916");
			strcpy(reginci.innomfic,"HSM"); 
			return(5);
    }
    else
    {
    	if (memcmp(pscerrado,"6",1) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
    	{
    		fprintLog(nomProg,"-- Aunque sean MAC iguales, se procede al falseo y se genera error --\n"); 
				strcpy(coderror, "916");
				strcpy(reginci.innomfic,"HSM"); 
				return(5);    	
    	}
    	else
    	{
    		fprintLog(nomProg,"--------------------- Salida de rutina calculo_cmac - son iguales -------------------------------\n");
     		return(0);
     	}
    }
	}
} //FIN calculo_cmac

/***********************************************************************
  FUNCION   sacar_claves

  DESCRIPCION : Calcula el numero de claves que llegan en el S96
  							y las va calculando el CVV y enviando a guardar

     retorna :     	0 . todo correcto
                   98 . error de HSM
***********************************************************************/
int sacar_claves (formato_interno *pforitn, char * clavec1, char * clavec2, char * clavec3, char * entidadE, char * tiempoE, char * cabeceraE, char * ip1E, char * puertoE, char * ip2E, char * claveZMKs)
{
	int longitud, contador, lclav, retorno;
	int uno, dos, tres; //0 = no esta, 1 = si esta
	char longitud_s93[3];
	char conjunto_s93[360];
	char clong[3];
	char entidad[4];
	char cabecera[4];
	char ip1[15];
	char puerto[4];
	char tiempo[4];
	char ip2[15];
	char clave1[150];
	char clave2[150];
	char clave3[150];
	char kcv1[6];
	char kcv2[6];
	char kcv3[6];
	char tipo1[3];
	char tipo2[3];
	char tipo3[3];
	char identifica[2];
	char claveS[150];
	char claveZMK[151];
	memset(claveS, ' ', sizeof(claveS));
	clave1[150] = '\0';
	clave2[150] = '\0';
	clave3[150] = '\0';
	fprintLog(nomProg,"--------------------- Entrada en rutina sacar_claves --------------------------------------------\n");
	strncpy(clavec1,"                                                                                                                                                      ", 150);
	strncpy(clavec2,"                                                                                                                                                      ", 150);
	strncpy(clavec3,"                                                                                                                                                      ", 150);
	strncpy(entidad, entidadE, 4);
	strncpy(cabecera, cabeceraE, 4);
	strncpy(ip1, ip1E, 15);
	strncpy(puerto, puertoE, 4);
	strncpy(tiempo, tiempoE, 4);
	strncpy(ip2, ip2E, 15);
	strncpy(longitud_s93, pforitn->fidatiso.fiisocla.ficla096.longitud, 3);
	longitud_s93[3] = '\0';
	strncpy(conjunto_s93, pforitn->fidatiso.fiisocla.ficla096.datos, 360);
	conjunto_s93[360] = '\0';
	strncpy(identifica,pforitn->fidatiso.fiisocla.ficla053.identificador + 6,2);
	//fprintf(stdout,"Valor de identifica >%.2s<\n", identifica);
	if (strncmp(identifica, "00", 2) == 0)
	{
		strncpy(tipo1, "ZMK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, "ZIK", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "01", 2) == 0 || strncmp(identifica, "02", 2) == 0)
	{
		strncpy(tipo1, "ZPK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, "ZAK", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, "ZEK", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "03", 2) == 0 || strncmp(identifica, "04", 2) == 0)
	{
		strncpy(tipo1, "ZPK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, "ZAK", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "05", 2) == 0 || strncmp(identifica, "06", 2) == 0)
	{
		strncpy(tipo1, "ZPK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, "ZEK", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "07", 2) == 0 || strncmp(identifica, "08", 2) == 0)
	{
		strncpy(tipo1, "ZAK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, "ZEK", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "11", 2) == 0 || strncmp(identifica, "12", 2) == 0)
	{
		strncpy(tipo1, "ZPK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, " ", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "13", 2) == 0 || strncmp(identifica, "14", 2) == 0)
	{
		strncpy(tipo1, "ZAK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, " ", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "15", 2) == 0 || strncmp(identifica, "16", 2) == 0)
	{
		strncpy(tipo1, "ZEK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, " ", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "21", 2) == 0)
	{
		strncpy(tipo1, "ZMK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, " ", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	if (strncmp(identifica, "22", 2) == 0)
	{
		strncpy(tipo1, "ZIK", 3);
		tipo1[3] = '\0';
		strncpy(tipo2, " ", 3);
		tipo2[3] = '\0';
		strncpy(tipo3, " ", 3);
		tipo3[3] = '\0';
	}
	fprintLog(nomProg,"Valor de tipo1, tipo2 y tipo3 >%.3s< >%.3s< >%.3s<\n", tipo1, tipo2, tipo3);
	contador = 0;
	lclav = 0;
	longitud = atoi(longitud_s93);
	uno = 0;
	dos = 1;
	tres = 1;
	fprintLog(nomProg,"Entrada >%.360s<\n",pforitn->fidatiso.fiisocla.ficla096.datos);
	while (contador < longitud)
	{
		if (uno == 1)
		{
			dos = 0;
		}
		if (uno == 1 && dos == 1)
		{
			tres = 0;
		}
		//fprintLog(nomProg," Contador 1 >%d<\n",contador);
		strncpy(clong,pforitn->fidatiso.fiisocla.ficla096.datos + 1 + contador, 4);
		//fprintLog(nomProg," Contador 2 >%d< y longitud leida >%.4s<\n",contador,clong);
		lclav = atoi(clong);
		strncpy(kcv1,"      ", 6);
		strncpy(kcv2,"      ", 6);
		strncpy(kcv3,"      ", 6);
		if (uno == 0)
		{
			strncpy(clave1,pforitn->fidatiso.fiisocla.ficla096.datos + contador, lclav);
			clave1[lclav] = '\0';
			contador = contador + lclav;
			strncpy(kcv1,pforitn->fidatiso.fiisocla.ficla096.datos + contador + 1, 6);
			contador = contador + 7;
			contador = contador + 1;
			uno = 1;
			//fprintLog(nomProg," Contador 3 >%d<\n",contador);

			retorno = pasar_aAES (pforitn, clave1, kcv1, tipo1, identifica, lclav, claveS, entidad, tiempo, cabecera, ip1, puerto, ip2, claveZMK); //Llamada para importar la clave como externa, luego calcular su KCV y luego importar interna
			if (retorno != 0)
			{
				fprintLog(nomProg,"-- Error al pasar a AES clave 1 >%d< \n", retorno);
				if (retorno == 1)
				{
				  strcpy(coderror, "801");
				  strcpy(pforitn->fidatpro.ficodfin, "801");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "801");
				}
				if (retorno == 2)
				{
				  strcpy(coderror, "802");
				  strcpy(pforitn->fidatpro.ficodfin, "802");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "802");
				}
				if (retorno == 3)
				{
				  strcpy(coderror, "904");
				  strcpy(pforitn->fidatpro.ficodfin, "904");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "904");
				}
				if (retorno == 4)
				{
				  strcpy(coderror, "909");
				  strcpy(pforitn->fidatpro.ficodfin, "909");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "909");
				}
				strcpy(reginci.innomfic,"HSM");
			  strcpy(pforitn->fidatpro.firesfin, "00");
			  return(retorno); 
			}
			else
			{
				fprintLog(nomProg,"-- Importada clave 1 OK >%s< \n", claveS);
				strncpy(clave1, claveS, sizeof(claveS));
			}
		}

		if (dos == 0)
		{
			strncpy(clave2,pforitn->fidatiso.fiisocla.ficla096.datos + contador, lclav);
			clave2[lclav] = '\0';
			contador = contador + lclav;
			strncpy(kcv2,pforitn->fidatiso.fiisocla.ficla096.datos + contador + 1, 6);
			contador = contador + 7;
			contador = contador + 1;
			dos = 1;
			//fprintLog(nomProg," Contador 4 >%d<\n",contador);

			retorno = pasar_aAES (pforitn, clave2, kcv2, tipo2, identifica, lclav, claveS, entidad, tiempo, cabecera, ip1, puerto, ip2, claveZMK); //Llamada para importar la clave y luego calcular su KCV
			if (retorno != 0)
			{
				fprintLog(nomProg,"-- Error al pasar a AES clave 2 >%d< \n", retorno);
				if (retorno == 1)
				{
				  strcpy(coderror, "801");
				  strcpy(pforitn->fidatpro.ficodfin, "801");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "801");
				}
				if (retorno == 2)
				{
				  strcpy(coderror, "802");
				  strcpy(pforitn->fidatpro.ficodfin, "802");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "802");
				}
				if (retorno == 3)
				{
				  strcpy(coderror, "904");
				  strcpy(pforitn->fidatpro.ficodfin, "904");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "904");
				}
				if (retorno == 4)
				{
				  strcpy(coderror, "909");
				  strcpy(pforitn->fidatpro.ficodfin, "909");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "909");
				}
				strcpy(reginci.innomfic,"HSM");
			  strcpy(pforitn->fidatpro.firesfin, "00");
			  return(retorno); 
			}
			else
			{
				fprintLog(nomProg,"-- Importada clave 2 OK >%s< \n", claveS);
				strncpy(clave2, claveS, sizeof(claveS));
			}
		}

		if (tres == 0)
		{
			strncpy(clave3,pforitn->fidatiso.fiisocla.ficla096.datos + contador, lclav);
			clave3[lclav] = '\0';
			contador = contador + lclav;
			strncpy(kcv3,pforitn->fidatiso.fiisocla.ficla096.datos + contador + 1, 6);
			contador = contador + 7;
			contador = contador + 1;
			tres = 1;
			//fprintLog(nomProg," Contador 5 >%d<\n",contador);

			retorno = pasar_aAES (pforitn, clave3, kcv3, tipo3, identifica, lclav, claveS, entidad, tiempo, cabecera, ip1, puerto, ip2, claveZMK); //Llamada para importar la clave y luego calcular su KCV
			if (retorno != 0)
			{
				fprintLog(nomProg,"-- Error al pasar a AES clave 3 >%d< \n", retorno);
				if (retorno == 1)
				{
				  strcpy(coderror, "801");
				  strcpy(pforitn->fidatpro.ficodfin, "801");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "801");
				}
				if (retorno == 2)
				{
				  strcpy(coderror, "802");
				  strcpy(pforitn->fidatpro.ficodfin, "802");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "802");
				}
				if (retorno == 3)
				{
				  strcpy(coderror, "904");
				  strcpy(pforitn->fidatpro.ficodfin, "904");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "904");
				}
				if (retorno == 4)
				{
				  strcpy(coderror, "909");
				  strcpy(pforitn->fidatpro.ficodfin, "909");
				  strcpy(pforitn->fidatiso.fiisocla.ficla039, "909");
				}
				strcpy(reginci.innomfic,"HSM");
			  strcpy(pforitn->fidatpro.firesfin, "00");
			  return(retorno); 
			}
			else
			{
				fprintLog(nomProg,"-- Importada clave 3 OK >%s< \n", claveS);
				strncpy(clave3, claveS, sizeof(claveS));
			}
		}
		fprintLog(nomProg,"-- Sacadas Clave 1 >%.150s< Clave 2 >%.150s< Clave 3 >%.150s<\n", clave1, clave2, clave3);
		fprintLog(nomProg,"-- Sacadas KCV 1 >%.6s< KCV 2 >%.6s< KCV 3 >%.6s<\n", kcv1, kcv2, kcv3);
		
	} //Fin del while
	
	strncpy(clavec1, clave1, 150);
	clavec1[150] = '\0';
	strncpy(clavec2, clave2, 150);
	clavec2[150] = '\0';
	strncpy(clavec3, clave3, 150);
	clavec3[150] = '\0';
	strncpy(claveZMKs, claveZMK, 150);
	claveZMKs[150] = '\0';
	fprintLog(nomProg,"--------------------- Salida de rutina sacar_claves ---------------------------------------------\n");
} // FIN sacar_claves

/***********************************************************************
  FUNCION   pasar_aAES

  DESCRIPCION : pasa la clave TR-31 a formato AES - llamada 206 del HSM

     retorna :     	0 . todo correcto
                  !=0 . error de HSM
***********************************************************************/
int pasar_aAES(formato_interno *pforitn, char * claveE, char * kcvE, char * tipo, char * identi, int longi, char * claveS, char * entidadE, char * tiempoE, char * cabeceraE, char * ip1E, char * puertoE, char * ip2E, char * claveZMK)
{

 	NPSIDEN regiden; 
  NPSDHSM reg_hsm;
 	int tipo_aesc;
  int ll;
 	int retorno = 0; 
 	long retornol = 0;
 	long long_hsm = 0;
 	long long_kcv = 0;
 	int ii;

  char claveBaja[151];
  long longBaja = 0;
	char salidabaja[100];
  char claveExpor[151];
  char claveTr31[151];
  long longExpor = 0; //Temporal
	char salidaexpor[1500]; //Temporal
	long long0619 = 0; //Temporal
	char entrada0619[320]; //Temporal
	char salida0619[32]; //Temporal

 	unsigned char cc;
 	char * addr;
 	char comando[50];
 	char zona_aes[2];
 	char tipo_aes[2];
 	char clave_iden[5];
 	char clave1[151];
  char clave2[151];
  char claveIMP[151];
  char codResp[3];
  char clavecone[4];
  char tipoI[3];
  int tipoImp;
 	char claveEntra[233]; //PRJ0258542 - Fase 3 - Cambio 11052026 - cambio de 201 a 233
 	char claveInter[233]; //PRJ0258542 - Fase 3 - Cambio 11052026 - cambio de 201 a 233
	char kcvSale[6];
	char kcvImpor[6];
	char claveExterna[233]; //PRJ0258542 - Fase 3 - Cambio 11052026 - cambio de 201 a 233
	char claveInterna[233]; //PRJ0258542 - Fase 3 - Cambio 11052026 - cambio de 201 a 233
	char claveSale[150];
	char identifica[2];
	char entidad[4];
	char cadena[7];
  char ip_conex1[15]; //PRJ0258542 - Fase 3
  char ip_conex2[15]; //PRJ0258542 - Fase 3
	char tiempo[4]; //PRJ0258542 - Fase 3
	char puerto[4]; //PRJ0258542 - Fase 3
	char cabecera[4]; //PRJ0258542 - Fase 3
	char claveCMac[151];
 	char area_machsm[4096];
 	char datos_hsm[4096];
 	char mac_desempaq[16];
 	char mac_calculado[4096];

  memset(claveEntra,'\0',sizeof(claveEntra));
  memset(claveSale,'\0',sizeof(claveSale));
  memset(claveInter,'\0',sizeof(claveInter));
  memset(claveExterna,'\0',sizeof(claveExterna));
  memset(claveInterna,'\0',sizeof(claveInterna));
  memset(kcvImpor,'\0',sizeof(kcvImpor));
  strncpy(kcvImpor, kcvE, 6);
  kcvImpor[6] = '\0';
  strncpy(identifica, identi, 2);
  identifica[2] = '\0';
  strncpy(entidad, entidadE, 4);
  entidad[4] = '\0';
  strncpy(cadena, "    ", 7);
  cadena[7] = '\0';
  strncpy(ip_conex1, ip1E, 15);
  ip_conex1[15] = '\0';
  strncpy(ip_conex2, ip2E, 15);
  ip_conex2[15] = '\0';
  strncpy(tiempo, tiempoE, 4);
  tiempo[4] = '\0';
  strncpy(puerto, puertoE, 4);
  puerto[4] = '\0';
  strncpy(cabecera, cabeceraE, 4);
  cabecera[4] = '\0';
  fprintLog(nomProg,"--------------------- Entrada en rutina pasar_aAES ----------------------------------------------\n");
  fprintLog(nomProg,"ClaveE >%.144s<\n", claveE);
  fprintLog(nomProg,"kcvE   >%.6s<\n", kcvE);
  fprintLog(nomProg,"tipo   >%.3s<\n", tipo);
  fprintLog(nomProg,"longi  >%d<\n", longi);
  fprintLog(nomProg,"identi >%.2s<\n", identifica);
	fprintLog(nomProg,"entidad>%.4s<\n", entidad);
	fprintLog(nomProg,"ip_conex1 >%.15s<\n", ip_conex1);
	fprintLog(nomProg,"ip_conex2 >%.15s<\n", ip_conex2);
	fprintLog(nomProg,"tiempo  >%.4s<\n", tiempo);
	fprintLog(nomProg,"puerto  >%.4s<\n", puerto);
	fprintLog(nomProg,"cabecera>%.4s<\n", cabecera);

  strncpy(clave_iden, pforitn->fidatpro.fiorigen, 2);
  strncpy(clave_iden + 2, pforitn->fidatpro.fisubori, 2); 
  clave_iden[4]='\0';

  strncpy(tipoI, tipo, 3);
  tipoI[3]='\0';
	if (strncmp(tipoI, "ZMK", 3) == 0)
	{
		tipoImp = 2;
	}
	if (strncmp(tipoI, "ZEK", 3) == 0)
	{
		tipoImp = 7;
	}
	if (strncmp(tipoI, "ZPK", 3) == 0)
	{
		tipoImp = 3;
	}
	if (strncmp(tipoI, "ZIK", 3) == 0 || strncmp(tipoI, "ZAK", 3) == 0)
	{
		tipoImp = 8;
	}
	
 	for (ll=0 ; ll<2 ; ll++) 
  {
  	if (strncmp(pforitn->fidatpro.fiorigen,reg_para[ll].psnument,2)==0)
		{
			strncpy(tipo_aes, pforitn->fidatiso.fiisocla.ficla053.algoritmo, 2);
			//tipo_aesc = atoi(tipo_aes);
			fprintLog(nomProg, "Valor de P53 algoritmo >%.2s< tipoImp >%d< tipo_aesc >%d<\n ", tipo_aes, tipoImp, tipo_aesc);
			strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
			if (strncmp(tipo_aes, "00", 2) == 0 || strncmp(tipo_aes, "03", 2) == 0)
			{
     		strncpy(comando,reg_para[ll].psclvhsm,50);
     	}
     	if (strncmp(tipo_aes, "05", 2) == 0)
			{
				//PRJ0258542 - Fase 3 - Cambio 11052026 - quitar control, siempre con zmka
				//if (tipoImp == 2)
				//{
				//	strncpy(comando,reg_para[ll].psclzmki,50);
				//}
				//else
				//{
					strncpy(comando,reg_para[ll].psclzmka,50);
				//}
     	}     	
			comando[50]='\0';

			/*
      switch (tipo_aesc)
      {
       	case 00: //TDES sin PIN
        	strncpy(comando,reg_para[ll].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 03: //TDES
        	strncpy(comando,reg_para[ll].psclvhsm,50);
        	comando[50]='\0';
        	break;
        case 05: //AES - pagina 16 del comunicado 23/24, se calcula con la ZMK activa
        					// Para el caso de la renovacion de la ZMK, se utiliza la ZMK anterior, no la vigente
        					// por lo que tengo que cambiar para que lea la zmki
        					// Al fallar en las pruebas del 26/05/2026, se refiere a que es con la que tienes, no con la que te llega, por eso dice la anterior
        	//PRJ0258542 - Fase 3 - Cambio 11052026 - quitar control, siempre con zmka
        	//if (tipoImp == 2)
        	//{
        	//	strncpy(comando,reg_para[ll].psclzmki,50);
        	//}
        	//else
        	//{
        		strncpy(comando,reg_para[ll].psclzmka,50);
        	//}
        	comando[50]='\0';
        	break;
 			}
 			*/
 			fprintLog(nomProg, "Clave_iden >%.4s< comando >%.50s<\n", clave_iden, comando);
    	retorno = claveshsm(comando, clave_iden, "000000", clave1, clave2);
			break;
    }
  }
	if (retorno<0)
	{
		fprintLog(nomProg, "Error al obtener la clave\n");
	  fprintLog(nomProg, "TIPO SISTEMA ACCESO HSM: conexion HSM  error al obtener la clave retorno>%i<\n", retorno);

	  strcpy(pforitn->fidatpro.firesfin, "00");
	  return(4); 
	}
  strncpy(zona_aes,pforitn->fidatiso.fiisocla.ficla053.proteccion, 2);
  zona_aes[2]='\0';
	strncpy(pforitn->fidatpro.fimacarq.idmac, clave1+5,5);
	strncpy(claveIMP,clave1, strlen(clave1)); //Llevara la clave ZMK activa para este calculo
 	claveIMP[strlen(clave1)]='\0';
	strncpy(claveZMK,claveIMP, strlen(claveIMP)); //la guarda para usos posteriores
 	claveZMK[strlen(claveIMP)]='\0';
	
	//Primero hace una importacion externa para poder sacar el KCV de la clave
	long_hsm = longi;
	strncpy(claveEntra, claveE, longi);
	claveEntra[longi] = '\0';

	//Llamada a manejhsm con opcion 08 0206
	fprintLog(nomProg,"--------------------- - Realizar la importacion de la clave externamente - ----------------------\n");
	retorno = manejhsm(claveIMP, "12", tipoImp, claveEntra, long_hsm, sockssl, claveExterna);
	fprintLog(nomProg,"  Salida de manejhsm impor extern >%d< >%s<\n",retorno,claveExterna);
	fprintLog(nomProg,"--------------------- - Realizada la importacion de la clave externamente - ---------------------\n");
  if (retorno != 0)
  {
  	//Despues se calcula el KCV con la clave importada
  	//fprintLog(nomProg, "Va a calcular el KCV de la clave Externa\n");
  	//Movemos la clave de salida
  	strncpy(claveInter,claveExterna,strlen(claveExterna));
		claveInter[strlen(claveInter)] = '\0';
		long_kcv = strlen(claveInter);
	}
	else
	{
		fprintLog(nomProg, "Error al importar la clave TR31\n");
		return(4);
	}
	memcpy(claveSale,"   ",144);
	claveSale[144] = '\0';
	fprintLog(nomProg,"--------------------- - Realizar el calculo del KCV de la clave externa - -----------------------\n");
	retorno = manejhsm(claveInter, "09", 5, claveInter, long_kcv, sockssl, claveSale); 
	if (retorno != 8)
	{
	  fprintLog(nomProg,"TIPO SISTEMA ACCESO HSM: conexion HSM error al obtener el KCV retorno >%d<\n", retorno);
	  return(4);
	}
	fprintLog(nomProg,"  Salida de manejhsm calculo kcv >%s<\n",claveSale);
	strncpy(kcvSale, claveSale + 2, 6);
	if (strncmp(kcvImpor, kcvSale, 6) != 0)
	{
		fprintLog(nomProg," KCV calculado y KCV recibido distintos >%.6s< >%.6s<\n",kcvSale, kcvImpor);
		fprintLog(nomProg,"--------------------- - Realizado el calculo del KCV de la clave externa - ----------------------\n");
		return(2);
	}
	fprintLog(nomProg," KCV calculado y KCV recibido iguales >%.6s< >%.6s<\n",kcvSale, kcvImpor);
	fprintLog(nomProg," --- Valor del semaforo de falseo - %s - ---\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026
	if (memcmp(pscerrado,"5",1) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
	{
		fprintLog(nomProg,"-- Aunque sean KCV iguales, se procede al falseo y se genera error 802 --\n"); 
		return(2);
	}
	fprintLog(nomProg,"--------------------- - Realizado el calculo del KCV de la clave externa - ----------------------\n");
	// Si todo ha ido bien se importa de forma interna
	fprintLog(nomProg,"--------------------- - Realizar la importacion de la clave internamente - ----------------------\n");
	//Tiene que actualizar la tabla de control
	strncpy(cadena, entidad, 4);
	strncpy(cadena + 4, "3", 1);
	strncpy(cadena + 5, identifica, 2);
	cadena[7] = '\0';
	retorno = tratar_fich("NPTDHSM", ESCRIBIR, cadena, "0001");
	if (retorno != 0)
	{
		fprintLog(nomProg,"FALLO ACTUALIZANPTDHSM valor >%d<\n",retorno);
		return(4);
	}

//PRJ0258542 - Fase 3 - INI	
	//Llamada a manejhsm con opcion 08 0206
	fprintLog(nomProg,"--------------------- --------------------------------- -----------------------------------------\n");
	fprintLog(nomProg,"Alta al 1 ip_conex1 >%.15s<\n", ip_conex1);
	fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
	fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
	fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
	fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
	fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
	conexhsm_1(ip_conex1, puerto, tiempo, cabecera, sockssl, codResp, 1); //PRJ0258542 - Fase 3
	fprintLog(nomProg, "Valor sockssl hsm 1->sfd_ssl >%i<\n",sockssl->sfd_ssl);
	
	strncpy(claveInterna, clave_iden, 4);
	strncpy(claveInterna + 4, "1", 1);
	fprintLog(nomProg, "  * Valor claveInterna y n HSM >%.5s<\n", claveInterna);
	retorno = manejhsm(claveIMP, "08", tipoImp, claveEntra, long_hsm, sockssl, claveInterna);
	fprintLog(nomProg, " Valor de salida %d \n", retorno);
	if (retorno <= 0)
	{
		strncpy(cadena, entidad, 4);
		strncpy(cadena + 4, "3", 1);
		strncpy(cadena + 5, "99", 2);
		cadena[7] = '\0';
		retorno = tratar_fich("NPTDHSM", ESCRIBIR, cadena, "0001");
		if (retorno != 0)
		{
			fprintLog(nomProg,"FALLO ACTUALIZANPTDHSM valor >%d<\n",retorno);
			fprintLog(nomProg,"--------------------- Salida de rutina pasar_aAES con 4 -----------------------------------------\n");
			return(4);
		}
		fprintLog(nomProg,"--------------------- Salida de rutina pasar_aAES con 1 -----------------------------------------\n");
	  return(1);
	}
	else
	{
		fprintLog(nomProg,"--------------------- --------------------------------- -----------------------------------------\n");  
		fprintLog(nomProg,"Alta al 2 ip_conex2 >%.15s<\n", ip_conex2);
		fprintLog(nomProg,"sfd_ssl   >%d<\n", sockssl->sfd_ssl);
		fprintLog(nomProg,"tiempo    >%.4s<\n", tiempo);
		fprintLog(nomProg,"puerto    >%.4s<\n", puerto);
		fprintLog(nomProg,"cabecera  >%.4s<\n", cabecera);
		fprintLog(nomProg,"codResp   >%.2s<\n", codResp);
		conexhsm_1(ip_conex2, puerto, tiempo, cabecera, sockssl, codResp, 2); //PRJ0258542 - Fase 3
		fprintLog(nomProg, "Valor sockssl hsm 2->sfd_ssl >%i<\n",sockssl->sfd_ssl);
		
		strncpy(claveInterna, clave_iden, 4);
		strncpy(claveInterna + 4, "2", 1);
		fprintLog(nomProg, "  ** Valor claveInterna y n HSM >%.5s<\n", claveInterna);
		retorno = manejhsm(claveIMP, "08", tipoImp, claveEntra, long_hsm, sockssl, claveInterna);
		fprintLog(nomProg, " Valor de salida %d \n", retorno);
		if (retorno <= 0)
		{
			strncpy(cadena, entidad, 4);
			strncpy(cadena + 4, "3", 1);
			strncpy(cadena + 5, "99", 2);
			cadena[7] = '\0';
			retorno = tratar_fich("NPTDHSM", ESCRIBIR, cadena, "0001");
			if (retorno != 0)
			{
				fprintLog(nomProg,"FALLO ACTUALIZANPTDHSM valor >%d<\n",retorno);
				fprintLog(nomProg,"--------------------- Salida de rutina pasar_aAES con 4 -----------------------------------------\n");
			  return(4);
			}
			fprintLog(nomProg,"--------------------- Salida de rutina pasar_aAES con 1 -----------------------------------------\n");
		  return(1);
		}
		else
		{
			strncpy(claveS, claveInterna, retorno);
			strncpy(cadena, entidad, 4);
			strncpy(cadena + 4, "3", 1);
			strncpy(cadena + 5, "99", 2);
			cadena[7] = '\0';
			retorno = tratar_fich("NPTDHSM", ESCRIBIR, cadena, "0001");
			if (retorno != 0)
			{
				fprintLog(nomProg,"FALLO ACTUALIZANPTDHSM valor >%d<\n",retorno);
			  fprintLog(nomProg,"--------------------- Salida de rutina pasar_aAES con 4 -----------------------------------------\n");
			  return(4);
			}
		  fprintLog(nomProg,"--------------------- Salida de rutina pasar_aAES con 0 -----------------------------------------\n");
		  return(0);
		}
	}
	fprintLog(nomProg,"--------------------- Salida de rutina pasar_aAES con 0 -----------------------------------------\n");
  return(0);
//PRJ0258542 - Fase 3 - FIN
} //Fin pasar_aAES

int busca_previa(formato_interno * pforitn)
{
  int retorno;
  char longP[3];
	char clave_log[21];
  NPSLOGO reg_logo;
  tpclav02 * loclavpr;
  iso_claves * fiautorizacion;  
  iso_claves * loautorizacion;
  formato_interno auxlogforitn;
	char iso000[4];
	char iso011[6];
	char iso012[12];
	char iso033[11];
	char entidad[4]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char cabecera[4]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char ip1[15]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char puerto[4]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char tiempo[4]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char ip2[15]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char codResp[3]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char iso024[3]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char iso039[3]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char codfin[3]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char fiorigen[4]; //PRJ0258542 - Fase 3 - Cambio 11052026
	int numerobajas, unoodos, ll;
	char comandoB1[51];
	char comandoB2[51];
	char comandoB3[51];
	char comandoSUP1[51]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char comandoSUP2[51]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char comandoSUP3[51]; //PRJ0258542 - Fase 3 - Cambio 11052026
	char cSUP[4]; //PRJ0258542 - Fase 3 - Cambio 11052026
	strncpy(cSUP, "_SUP", 4); //PRJ0258542 - Fase 3 - Cambio 11052026
	cSUP[4]='\0';
	long long_bla = 0; //PRJ0258542 - Fase 3 - Cambio 11052026
	char salidabaja[200]; //PRJ0258542 - Fase 3 - Cambio 11052026
	long long_hsm = 0; //PRJ0258542 - Fase 3 - Cambio 11052026
	char iso093[11];
	char iso094[11]; 
	int iso000n; 
	char subsis[2];
	char subapl[2];
	char tipocla1[3];
	char tipocla2[3];
	char tipocla3[3];
	char cadenaP[466];
	char cadenaR[154];
	char clave1[150];
	char clave2[150];
	char clave3[150];
	char identifica[2];
	char espacios[50]; //PRJ0258542 - Fase 3 - Cambio 11052026
  char clave_pr[35];
	numerobajas = 0;
	unoodos = 0;
  fprintLog(nomProg,"Entrando en busca_previa\n");
  strncpy(espacios,"                                                  ",50); //PRJ0258542 - Fase 3 - Cambio 11052026
  strncpy(comandoSUP1, espacios, 50);
  strncpy(comandoSUP2, espacios, 50);
  strncpy(comandoSUP3, espacios, 50);
  espacios[50] = '\0'; //PRJ0258542 - Fase 3 - Cambio 11052026
 	loclavpr = (tpclav02 *) &reg_logo.loclavpr;
 	//fiautorizacion = &pforitn->fidatiso.fiisoaut;

 	fverfi(*pforitn);
 
 	//guardamos datos para posible incidencia
 	strcpy(reginci.innomfic, "trataMS"); 
 	strncpy(reginci.innumope, pforitn->fidatpro.finumope, 6);
 
 	memset(clave_pr, ' ', sizeof(clave_pr));
 
 	/***   montamos la clave de log para buscar la previa   ***/
 	strncpy(iso000, pforitn->fidatiso.fiisoadm.fiadm072.datos+3, 4);
 	iso000n = atoi(iso000) - 10;
 	strncpy(iso011, pforitn->fidatiso.fiisoadm.fiadm072.datos+7, 6);
 	strncpy(iso012, pforitn->fidatiso.fiisoadm.fiadm072.datos+13, 12);
 	strncpy(iso094, pforitn->fidatiso.fiisoadm.fiadm072.datos+44, 8);
	fprintLog(nomProg,"Datos previa 00, 11, 12, 94 >%.4s< >%.6s< >%.12s< >%.11s<\n", iso000, iso011, iso012, iso094);
	strncpy(iso000, itoa(iso000n), 4);
 	strncpy(clave_pr, iso000, 4);
 	strncpy(clave_pr + 4, iso011, 6);
 	strncpy(clave_pr + 10, iso012, 12);
 	strncpy(clave_pr + 22, iso094, 8);
 	strncpy(clave_pr + 30, "   ", 3);
		
	clave_pr[35]='\0';
	strncpy(clave_pr + strlen(clave_pr),"               ",(35-strlen(clave_pr)));
	fprintLog(nomProg,"clave_pr para buscar previa ---->%.*s<----\n",	sizeof(clave_pr), clave_pr);
 	
	retorno = 0;

	retorno = tratar_fich("PREVIAA",IGUAL,&reg_logo,clave_pr);

	fprintLog(nomProg,"IGUALPREVIAA retorno=%d\n",retorno);

	if (retorno == 0)
	{
		fprintLog(nomProg,"Previa encontrada, retorno = %d\n",retorno);
		strncpy(codfin, reg_logo.loforitn + 347, 3); //PRJ0258542 - Fase 3 - Cambio 11052026
		strncpy(iso039, reg_logo.loforitn + 390, 3); //PRJ0258542 - Fase 3 - Cambio 11052026
		strncpy(iso024, reg_logo.loforitn + 343, 3); //PRJ0258542 - Fase 3 - Cambio 11052026
		fprintLog(nomProg,"Datos del formato_interno P24 >%.3s< P39 >%.3s< CODFIN >%.3s<\n", iso024, iso039, codfin);
		//if (strncmp(reg_logo.loforitn + 347,"800",3) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
		if (strncmp(iso039,"800",3) == 0 && strncmp(iso024,"815",3) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
		{
			fprintLog(nomProg,"Datos operacion con valor 800 y cambio de claves 815, restaura valores\n");
			fprintLog(nomProg,"Datos del formato_interno P35 cortado >%.8s<\n", reg_logo.loforitn + 395);
			memcpy(clave1, "----                                                                                                                                                  ", 150);
			memcpy(clave2, "----                                                                                                                                                  ", 150);
			memcpy(clave3, "----                                                                                                                                                  ", 150);
			strncpy(identifica, reg_logo.loforitn + 401, 2);
			identifica[2]='\0';
			strncpy(subsis, pforitn->fidatpro.fiorigen, 2);
			subsis[2] = '\0';
			strncpy(subapl, pforitn->fidatpro.fisubori, 2);
			subapl[2] = '\0';
			memcpy(cadenaP, 		subsis, 2);
			memcpy(cadenaP + 2, subapl, 2);
			strncpy(fiorigen, cadenaP, 4); //PRJ0258542 - Fase 3 - Cambio 11052026
			fprintLog(nomProg,"Valor fiorigen >%.4s<\n", fiorigen); //PRJ0258542 - Fase 3 - Cambio 11052026
			strncpy(entidad,"    ",4);
		  retorno = comprobarHSM(entidad, tiempo, cabecera, ip1, puerto, ip2); //PRJ0258542 - Fase 3
		  if (retorno != 0) //Se tiene que devolver error para no realizar acciones, 4 = 909 = error de sistema
		  {
		  	return(retorno);
	  	}
			
			fprintLog(nomProg,"--------------------- Comienza tratamiento de deshacer claves ------------------------------------\n");
			if (strncmp(identifica,"00",2) == 0) //ZMK y ZIK
			{
				memcpy(cadenaP + 4, "12ZMKZIK   ", 11);
			}
			if (strncmp(identifica,"01",2) == 0)
			{
				memcpy(cadenaP + 4, "13ZPKZAKZEK", 11);
			}
			if (strncmp(identifica,"02",2) == 0)
			{
				memcpy(cadenaP + 4, "23ZPKZAKZEK", 11);
			}
			if (strncmp(identifica,"03",2) == 0)
			{
				memcpy(cadenaP + 4, "12ZPKZAK   ", 11);
			}
			if (strncmp(identifica,"04",2) == 0)
			{
				memcpy(cadenaP + 4, "22ZPKZAK   ", 11);
			}
			if (strncmp(identifica,"05",2) == 0)
			{
				memcpy(cadenaP + 4, "12ZPKZEK   ", 11);
			}
			if (strncmp(identifica,"06",2) == 0)
			{
				memcpy(cadenaP + 4, "22ZPKZEK   ", 11);
			}
			if (strncmp(identifica,"07",2) == 0)
			{
				memcpy(cadenaP + 4, "12ZAKZEK   ", 11);
			}
			if (strncmp(identifica,"08",2) == 0)
			{
				memcpy(cadenaP + 4, "22ZAKZEK   ", 11);
			}
			if (strncmp(identifica,"11",2) == 0)
			{
				memcpy(cadenaP + 4, "11ZPK      ", 11);
			}
			if (strncmp(identifica,"12",2) == 0)
			{
				memcpy(cadenaP + 4, "21ZPK      ", 11);
			}
			if (strncmp(identifica,"13",2) == 0)
			{
				memcpy(cadenaP + 4, "11ZAK      ", 11);
			}
			if (strncmp(identifica,"14",2) == 0)
			{
				memcpy(cadenaP + 4, "21ZAK      ", 11);
			}
			if (strncmp(identifica,"15",2) == 0)
			{
				memcpy(cadenaP + 4, "11ZEK      ", 11);
			}
			if (strncmp(identifica,"16",2) == 0)
			{
				memcpy(cadenaP + 4, "21ZEK      ", 11);
			}
			if (strncmp(identifica,"21",2) == 0)
			{
				memcpy(cadenaP + 4, "11ZMK      ", 11);
			}
			if (strncmp(identifica,"22",2) == 0)
			{
				memcpy(cadenaP + 4, "11ZIK      ", 11);
			}
			strncpy(cadenaP + 15, "R", 1); //PRJ0258542 - Fase 3 - Cambio 11052026 - concatena R de retroceso
			memcpy(cadenaP + 16, clave1, 150);
			memcpy(cadenaP + 166, clave2, 150);
			memcpy(cadenaP + 316, clave3, 150);
			cadenaP[466] = '\0';
			fprintLog(nomProg,"  cadenaP  2>%.466s<\n",cadenaP);
			retorno = tratar_fich("NPTPARA",ESCRIBIR,cadenaP,NULL); 
			fprintLog(nomProg,"--------------------- Finaliza tratamiento de deshacer claves ------------------------------------\n");
			fprintLog(nomProg,"--------------------- Comienza tratamiento de baja de claves ---- Identifica %.2s ---------------\n", identifica);
			for (ll=0 ; ll<2 ; ll++) 
		  {
		  	if (strncmp(pforitn->fidatpro.fiorigen,reg_para[ll].psnument,2)==0)
				{	
		  		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
					if (strncmp(identifica,"00",2) == 0) //ZMK y ZIK
					{
						strncpy(comandoSUP1, reg_para[ll].psclzmka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzika, 50);
						numerobajas = 2;
						unoodos = 1;
					}
					if (strncmp(identifica,"01",2) == 0) //ZPK1 y ZAK1 y ZEK1
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzaka, 50);
						strncpy(comandoSUP3, reg_para[ll].psclzeka, 50);
						numerobajas = 3;
						unoodos = 1;
					}
					if (strncmp(identifica,"02",2) == 0) //ZPK2 y ZAK2 y ZEK2
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzaka, 50);
						strncpy(comandoSUP3, reg_para[ll].psclzeka, 50);
						numerobajas = 3;
						unoodos = 2;
					}
					if (strncmp(identifica,"03",2) == 0) //ZPK1 y ZAK1
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzaka, 50);
						numerobajas = 2;
						unoodos = 1;
					}
					if (strncmp(identifica,"04",2) == 0) //ZPK2 y ZAK2
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzaka, 50);
						numerobajas = 2;
						unoodos = 2;
					}
					if (strncmp(identifica,"05",2) == 0) //ZPK1 y ZEK1
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzeka, 50);
						numerobajas = 2;
						unoodos = 1;
					}
					if (strncmp(identifica,"06",2) == 0) //ZPK2 y ZEK2
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzeka, 50);
						numerobajas = 2;
						unoodos = 2;
					}
					if (strncmp(identifica,"07",2) == 0) //ZAK1 y ZEK1
					{
						strncpy(comandoSUP1, reg_para[ll].psclzaka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzeka, 50);
						numerobajas = 2;
						unoodos = 1;
					}
					if (strncmp(identifica,"08",2) == 0) //ZAK2 y ZEK2
					{
						strncpy(comandoSUP1, reg_para[ll].psclzaka, 50);
						strncpy(comandoSUP2, reg_para[ll].psclzeka, 50);
						numerobajas = 2;
						unoodos = 2;
					}
					if (strncmp(identifica,"11",2) == 0) //ZPK1
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						numerobajas = 1;
						unoodos = 1;
					}
					if (strncmp(identifica,"12",2) == 0) //ZPK2
					{
						strncpy(comandoSUP1, reg_para[ll].psclzpka, 50);
						numerobajas = 1;
						unoodos = 2;
					}
					if (strncmp(identifica,"13",2) == 0) //ZAK1
					{
						strncpy(comandoSUP1, reg_para[ll].psclzaka, 50);
						numerobajas = 1;
						unoodos = 1;
					}
					if (strncmp(identifica,"14",2) == 0) //ZAK2
					{
						strncpy(comandoSUP1, reg_para[ll].psclzaka, 50);
						numerobajas = 1;
						unoodos = 2;
					}
					if (strncmp(identifica,"15",2) == 0) //ZEK1
					{
						strncpy(comandoSUP1, reg_para[ll].psclzeka, 50);
						numerobajas = 1;
						unoodos = 1;
					}
					if (strncmp(identifica,"16",2) == 0) //ZEK2
					{
						strncpy(comandoSUP1, reg_para[ll].psclzeka, 50);
						numerobajas = 1;
						unoodos = 2;
					}
					if (strncmp(identifica,"21",2) == 0) //ZMK
					{
						strncpy(comandoSUP1, reg_para[ll].psclzmka, 50);
						numerobajas = 1;
						unoodos = 1;
					}
					if (strncmp(identifica,"22",2) == 0) //ZIK
					{
						strncpy(comandoSUP1, reg_para[ll].psclzika, 50);
						numerobajas = 1;
						unoodos = 1;
					}
					break;
				} //FIN de if = pnnument
			} //FIN del for
			fprintLog(nomProg,"Comandos SUP >%s< >%s< >%s< >%s<\n",comandoSUP1,comandoSUP2,comandoSUP3,cSUP);
			lespacio = fQuitar_Blancos(comandoSUP1,50);
			lespacio = fQuitar_Blancos(comandoSUP2,50);
			lespacio = fQuitar_Blancos(comandoSUP3,50);
			fprintLog(nomProg,"Comandos SUP >%s< >%s< >%s< >%s<\n",comandoSUP1,comandoSUP2,comandoSUP3,cSUP);
			strncat(comandoSUP1, cSUP, sizeof(comandoSUP1) - strlen(comandoSUP1) - 1);
			strncat(comandoSUP2, cSUP, sizeof(comandoSUP2) - strlen(comandoSUP2) - 1);
			strncat(comandoSUP3, cSUP, sizeof(comandoSUP3) - strlen(comandoSUP3) - 1);
			strncat(comandoSUP1, "                              ", sizeof(comandoSUP1) - strlen(comandoSUP1) - 1);
			strncat(comandoSUP2, "                              ", sizeof(comandoSUP2) - strlen(comandoSUP2) - 1);
			strncat(comandoSUP3, "                              ", sizeof(comandoSUP3) - strlen(comandoSUP3) - 1);
			comandoSUP1[50] = '\0';
			comandoSUP2[50] = '\0';
			comandoSUP3[50] = '\0';
			fprintLog(nomProg,"Comandos SUP >%s< >%s< >%s<\n",comandoSUP1,comandoSUP2,comandoSUP3);
	  	if (numerobajas > 0)
			{
	    	retorno = claveshsm(comandoSUP1, fiorigen, "000000", clave1, clave2);
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
						strncpy(salidabaja, fiorigen, 4);
						strncpy(salidabaja + 4, "1", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
							strncpy(salidabaja, fiorigen, 4);
							strncpy(salidabaja + 4, "2", 1);
							salidabaja[5]='\0';
							fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
						strncpy(salidabaja, fiorigen, 4);
						strncpy(salidabaja + 4, "1", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
							strncpy(salidabaja, fiorigen, 4);
							strncpy(salidabaja + 4, "2", 1);
							salidabaja[5]='\0';
							fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
			    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
			    		retorno = 0;
			    	}
	    		}
	    	}
	    } //Fin numerobajas > 0
	  	if (numerobajas > 1)
			{
	    	retorno = claveshsm(comandoSUP2, fiorigen, "000000", clave1, clave2);
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
						strncpy(salidabaja, fiorigen, 4);
						strncpy(salidabaja + 4, "1", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
							strncpy(salidabaja, fiorigen, 4);
							strncpy(salidabaja + 4, "2", 1);
							salidabaja[5]='\0';
							fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
						strncpy(salidabaja, fiorigen, 4);
						strncpy(salidabaja + 4, "1", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
							strncpy(salidabaja, fiorigen, 4);
							strncpy(salidabaja + 4, "2", 1);
							salidabaja[5]='\0';
							fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
			    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
			    		retorno = 0;
			    	}
	    		}
	    	}
	    } //Fin numerobajas > 1
	  	if (numerobajas > 2)
			{
	    	retorno = claveshsm(comandoSUP3, fiorigen, "000000", clave1, clave2);
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
						strncpy(salidabaja, fiorigen, 4);
						strncpy(salidabaja + 4, "1", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
							strncpy(salidabaja, fiorigen, 4);
							strncpy(salidabaja + 4, "2", 1);
							salidabaja[5]='\0';
							fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
						strncpy(salidabaja, fiorigen, 4);
						strncpy(salidabaja + 4, "1", 1);
						salidabaja[5]='\0';
						fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
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
							strncpy(salidabaja, fiorigen, 4);
							strncpy(salidabaja + 4, "2", 1);
							salidabaja[5]='\0';
							fprintLog(nomProg, "Valor fiorigen y n HSM >%.5s<\n", salidabaja);
			    		retorno = manejhsm(clave2, "24", 5, clave2, long_hsm, sockssl, salidabaja);
			    		retorno = 0;
			    	}
	    		}
	    	}
	    } //Fin numerobajas > 2
			fprintLog(nomProg,"--------------------- Finaliza tratamiento de baja de claves ------------------------------------\n");
		}
	}
	else
	{
 		return(finalizar(11));
 	}
 	return(0);
} //FIN busca_previa

int fms_preparaTr31(formato_interno *pfi)
{
	
	char cadenaTr[144];
	char cadenaFin[151];
	char cabecera[16];
	char comandoE[50];
	char comandoM[50];
	char doblecom[100];
 	char clave1[151];
  char clave2[151];
  char psnument[2];
  char claveExp[150];
  char claveMae[150];
  char clave_iden[5];
  int tipoC;
  char tipoCla[2];
  char kcv[6];
  int unodos;
  int ll, retorno;
  long long_hsm, long_cla;
  fprintLog(nomProg,"--------------------- Entrada en rutina fms_prepararTr31 ----------------------------------------\n");
  strncpy(clave_iden, pfi->fidatpro.fiorigen, 2);
	strncpy(clave_iden + 2, pfi->fidatpro.fisubori, 2); 
	clave_iden[4]='\0';
	fprintLog(nomProg, "Clave_iden >%.4s< \n", clave_iden);
	strncpy(psnument, pfi->fidatpro.fiorigen, 2);
	psnument[2] = '\0';
	strncpy(tipoCla, pfi->fidatiso.fiisocla.ficla053.identificador + 6, 2);
	tipoCla[2] = '\0';
	tipoC = atoi(tipoCla);
  fprintLog(nomProg,"-- Entrada funcion fms_preparaTr31 -- tipoC %d y clave_iden %.4s\n", tipoC, clave_iden);
	fprintLog(nomProg,"  Valor del P53 >%.2s%.8s%.2s%.4s%.2s%.6s< \n", pfi->fidatiso.fiisocla.ficla053.control, pfi->fidatiso.fiisocla.ficla053.identificador, pfi->fidatiso.fiisocla.ficla053.algoritmo, 
											pfi->fidatiso.fiisocla.ficla053.longitud, pfi->fidatiso.fiisocla.ficla053.proteccion, pfi->fidatiso.fiisocla.ficla053.resto);

	strncpy(pfi->fidatiso.fiisocla.ficla096.longitud," ",3);
	strncpy(pfi->fidatiso.fiisocla.ficla096.datos," ",360);
	cadenaTr[144] = '\0';
	cadenaFin[151] = '\0';
	claveExp[150] = '\0';
	unodos = 0;
	doblecom[100] = '\0';

 	for (ll=0 ; ll<2 ; ll++) 
  {
  	//fprintLog(nomProg," psnumet >%.2s< y reg_para[%d].psnument >%.2s<\n", psnument, ll, reg_para[ll].psnument);
  	if (strncmp(psnument,reg_para[ll].psnument,2)==0)
		{
  		strncpy(pscerrado, reg_para[ll].pscerrando, 1); //PRJ0258542 - Fase 3 - Cambio 11052026
			switch (tipoC)
		  {
		   	case 11:
		   	case 12:
		   		strncpy(comandoE,reg_para[ll].psclzpka,50);
		   		break;
		   	case 13:
		   	case 14:
		   		strncpy(comandoE,reg_para[ll].psclzaka,50);
		   		break;
		   	case 15:
		   	case 16:
		   		strncpy(comandoE,reg_para[ll].psclzeka,50);
		   		break;
		   	case 21:
		   		strncpy(comandoE,reg_para[ll].psclzmka,50);
		   		break;
		   	case 22:
		   		strncpy(comandoE,reg_para[ll].psclzika,50);
		   		break;
			}
	  	comandoE[50] = '\0';
			retorno = claveshsm(comandoE, clave_iden, "000000", clave1, clave2);
			
	  	break;
		}
  	if (ll > 4)
  		break;
  } //Fin del for
  
  if (retorno == 0)
  {
		switch (tipoC)
		{
			case 11:
			case 13:
			case 15:
			case 21:
			case 22:
				strncpy(claveExp,clave1,strlen(clave1));
				claveExp[strlen(clave1)] = '\0';
				break;
			case 12:
			case 14:
			case 16:
				strncpy(claveExp,clave2,strlen(clave2));
				claveExp[strlen(clave2)] = '\0';
				break;
		}
		long_hsm = strlen(claveExp);
		retorno = manejhsm(claveExp, "09", tipoC, claveExp, long_hsm, sockssl, cadenaTr); 
		if (retorno != 8)
		{
			fprintLog(nomProg,"TIPO SISTEMA ACCESO HSM: conexion HSM error al obtener el KCV retorno >%i<\n", retorno);
			return(2);
		}
		fprintLog(nomProg,"-- Salida de manejhsm >%s<\n",cadenaTr);
		strncpy(kcv, cadenaTr + 2, 6);
  }
  else
  {
  	return(1);
  }
	strncpy(cadenaFin, kcv, 6); ///SE SUSTITUYE POR SOLAMENTE EL KCV - 04/12/2024
	long_cla = 6; ///SE SUSTITUYE POR SOLAMENTE EL KCV - 04/12/2024
	cadenaFin[long_cla] = '\0'; ///SE SUSTITUYE POR SOLAMENTE EL KCV - 04/12/2024
	sprintf(pfi->fidatiso.fiisocla.ficla096.longitud, "%03d", long_cla,3);
	pfi->fidatiso.fiisocla.ficla096.longitud[3] = '\0';
	strncpy(pfi->fidatiso.fiisocla.ficla096.datos,cadenaFin,strlen(cadenaFin));
 	for (ll=strlen(cadenaFin) ; ll<360 ; ll++)
  {
  	pfi->fidatiso.fiisocla.ficla096.datos[ll] = ' ';
	}
	pfi->fidatiso.fiisocla.ficla096.datos[360] = '\0';
	fprintLog(nomProg," --- Valor del semaforo de falseo - %s - ---\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026
	if (memcmp(pscerrado,"1",1) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
	{
		fprintLog(nomProg,"-- Aunque sean KCV correcto, se procede al falseo --\n"); 
		strncpy(pfi->fidatiso.fiisocla.ficla096.datos,"000000",6);
	}
	fprintLog(nomProg,"  Valor del S96 >%.3s< >%.360s< \n", pfi->fidatiso.fiisocla.ficla096.longitud, pfi->fidatiso.fiisocla.ficla096.datos);
	fprintLog(nomProg,"--------------------- Salida de rutina fms_preparaTr31 ------------------------------------------\n");
	return(0);
} //FIN fms_preparaTr31

int comprobarHSM(char * entidadS, char * tiempoS, char * cabeceraS, char * ip1S, char * puertoS, char * ip2S) //PRJ0258542 - Fase 3 - Comprueba que los dos HSM's estén operativos y activados
{
	int retorno = 0;
	char entidad[4];
	char cabecera[4];
	char ip1[15];
	char puerto[4];
	char tiempo[4];
	char ip2[15];
	NPSDHSM reg_hsm;
	fprintLog(nomProg,"--------------------- Entrada en rutina comprobarHSM --------------------------------------------\n");
	retorno = tratar_fich("NPTDHSM", IGUAL, &reg_hsm, "0001");
	if (retorno == 0)
	{
		fprintLog(nomProg,"** Datos de configuracion HSMs **\n");
		fprintLog(nomProg,"*  Entidad    >%.4s<\n", reg_hsm.dhcodent);
		fprintLog(nomProg,"*  Tiempo     >%.4s<\n", reg_hsm.dhtiempo);
		fprintLog(nomProg,"*  Cabecera   >%.4s<\n", reg_hsm.dhcabece);
		fprintLog(nomProg,"*  IP HSM1    >%.15s<\n", reg_hsm.dhnumip);
		fprintLog(nomProg,"*  Puerto     >%.4s<\n", reg_hsm.dhpuerto);
		fprintLog(nomProg,"*  Produccion >%.1s<\n", reg_hsm.dhprodt);
		fprintLog(nomProg,"*  IP HSM2    >%.15s<\n", reg_hsm.dhnumip2);
		fprintLog(nomProg,"*  Semaf HSM1 >%.1s<\n", reg_hsm.dhsemafhsm1);
		fprintLog(nomProg,"*  Semaf HSM2 >%.1s<\n", reg_hsm.dhsemafhsm2);
		fprintLog(nomProg,"*  Renovacion >%.2s<\n", reg_hsm.dhparenov);
		if (strncmp(reg_hsm.dhsemafhsm1,"1",1) == 0 && strncmp(reg_hsm.dhsemafhsm2,"1",1) == 0)
		{
			fprintLog(nomProg," --- Valor del semaforo de falseo - %s - ---\n", pscerrado); //PRJ0258542 - Fase 3 - Cambio 11052026
    	if (memcmp(pscerrado,"3",1) == 0) //PRJ0258542 - Fase 3 - Cambio 11052026
    	{
    		fprintLog(nomProg,"-- Semaforo a 3, se simula claves en cambio, se procede al falseo --\n"); 
    		retorno = 1; //Se esta realizando otro cambio de claves
    	}
			else
			{
				if (strncmp(reg_hsm.dhparenov, "99",2) != 0)
				{
					retorno = 1; //Se esta realizando otro cambio de claves
				}
				else
				{
					fprintLog(nomProg," -> Semaforos a 1 y control renovacion a 99\n");
				}
			}
		}
		else
		{
			retorno = 4; //Alguno de los dos HSM no esta operativo
		}
	}
	else
	{
		fprintLog(nomProg,"** Error al obtener configuracion HSMs ** >%d<\n", retorno);
		retorno = 4;
	}
	strncpy(entidad, reg_hsm.dhcodent, 4);
	entidad[4] = '\0';
	strncpy(entidadS, entidad, 4);
	strncpy(tiempo, reg_hsm.dhtiempo, 4);
	tiempo[4] = '\0';
	strncpy(tiempoS, tiempo, 4);
	strncpy(cabecera, reg_hsm.dhcabece, 4);
	cabecera[4] = '\0';
	strncpy(cabeceraS, cabecera, 4);
	strncpy(ip1, reg_hsm.dhnumip, 15);
	ip1[15] = '\0';
	strncpy(ip1S, ip1, 15);
	strncpy(puerto, reg_hsm.dhpuerto, 4);
	puerto[4] = '\0';
	strncpy(puertoS, puerto, 4);
	strncpy(ip2, reg_hsm.dhnumip2, 15);
	ip2[15] = '\0';
	strncpy(ip2S, ip2, 15);
	fprintLog(nomProg,"--------------------- Salida de rutina comprobarHSM ---------------------------------------------\n");
	return (retorno);

} //FIN comprobarHSM

#include "ftimer.c"

// Quita los blancos y devuelve el tamaño de la cadena que se queda en cadena_entrada
long fQuitar_Blancos (char * cadena_entrada, long long_entrada)
{
	//printf(" ** ** ** Entrando rutina fQuitar_Blancos 12/2024 v1.0\n");
  long tam_entrada = 0;
  char cadena_aux[long_entrada];

  memset(cadena_aux, '\0', sizeof(cadena_aux));

  while (cadena_entrada[tam_entrada]!=' ' && tam_entrada < long_entrada)
  {
     sprintf(cadena_aux + tam_entrada, "%c", cadena_entrada[tam_entrada]); 
     tam_entrada++;
  }
  strncpy(cadena_entrada, cadena_aux, tam_entrada);
  cadena_entrada[tam_entrada] = '\0';
  
	//printf(" ** ** ** Saliendo rutina fQuitar_Blancos 12/2024 v1.0\n");
	return (tam_entrada);
}
//PRJ0258542 - FIN

