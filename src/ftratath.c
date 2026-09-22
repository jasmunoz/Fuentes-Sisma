/***********************************************************************
  FUNCION   funcionTH

  DESCRIPCION : tratamiento de mensajes de cierre para el host  
  // EN TRIODOS DE MOMENTO NO SE UTILIZA ///////
***********************************************************************/
#include "nptpara.h"

//int fgraba_diferidos(formato_interno *pforitn);
int fgradife(formato_interno * pforitn);

formato_interno paux_diforitn;
NPSPARA  *reg_para;

int funcionth(formato_interno *pforitn)
{
 char opcenvoff ='0';

/*** me falta añadir la funcion que graba en difer que la cogere de lanza_cierre_host
   y mirar la carga de totales  y añadir la tabla operaciones tother** **/

    int graba_fichero_difer03(formato_interno * ci_foritn);
    int fci_carga_totales03(formato_interno *pforitn);

    int retorno;
    int tipomsg;
	int ll;
	
    formato_interno pforitn_aux;
	

    fprintLog(nomProg,"****************\n");
    fprintLog(nomProg,"*** FTRATATH *** %.6s\n", pforitn->fidatpro.finumope);
    fprintLog(nomProg,"****************\n");

    /* comprobamos si hemos cuadrado o no */
    fprintLog(nomProg," respusal >%.3s<\n",pforitn->fiisocom.respusal);

    // GRABA ALERTA INFORMATIVA POR DESCUADRE SIO.
    if (!strncmp(pforitn->fiisocom.respusal,"501",3))
           fgraba_incidencia_cp(pforitn->fiisocom.respusal, pforitn->fidatpro.finetpri, NULL, NULL);

    memcpy(&pforitn_aux, pforitn, sizeof(formato_interno));

    tipomsg = atoi(pforitn->fiisocom.fiiso000);
    fprintLog(nomProg, "tipomsg >%d<\n",tipomsg);
    fprintLog(nomProg, "Totales Origen fidatpro.fiorigen >%s<\n",pforitn_aux.fidatpro.fiorigen);

    switch(tipomsg)
    {
    case 1524:
        strncpy(pforitn_aux.fiisocom.fiiso000,"1524",4);
		//RAQUEL CAMBIAR POR CAMPO DE LA TABLA NPTPARA
 
         for (ll=0 ; ll<2 ; ll++) 
         {  
			if (strncmp(pforitn_aux.fidatpro.fiorigen,reg_para[ll].psnument,2)==0)
		    {
    	      strncpy(pforitn_aux.fidatpro.firespri, reg_para[ll].psresolu,2); 
              break;
			}
         }
        strncpy(pforitn_aux.fidatpro.firesfin, "  ",2);

     // realizamos el cierre en npttoto
        retorno = fci_carga_totales03(&pforitn_aux);

        strcpy(pforitn_aux.fidatiso.fiisotot.monedafac, "0978");
        strcpy(pforitn_aux.fidatiso.fiisotot.monedacon, "0978");

      /**************controlar el error de thnptpars  grabar incidencia ***/
      // DOS NUEVOS CODIGOS DE ERROR. 088, 089
        if (retorno != 88)
        {
            retorno=graba_fichero_difer03(&pforitn_aux);
            fprintLog(nomProg, "el retorno de enviar_diferido es>%d<\n",retorno);
            if (retorno == 89)
            {
                fgraba_incidencia_cp("089", pforitn_aux.fidatpro.finumope, "trataTH", NULL);
            }
        }
        break;
    default:
        fprintLog(nomProg, "recibido mensaje no contemplado :%d\n",tipomsg);
        strcpy(coderror, "280");
         //Añadido para Monitorizacion SISMA 
         fprintLog(nomProg,"TIPO FUNCIONAL – TIPO DE MENSAJE NO CONTEMPLADO: Tipo mensaje >%d< codigo de error >%.3s<\n",tipomsg,coderror);

        strcpy(pforitn->fidatpro.ficodfin, "280");
        strcpy(pforitn->fidatpro.firesfin, "00");
        retorno = -1;
    }

    if(retorno)
        return (finalizar(98));

    return (finalizar(11));
}

/***************************************
int fci_carga_totales(formato_interno * ci_foritn)
carga los totales para el subsistema 03 tiene que leer el TOTO con
la clave 30,01,sesion_subsistema03,origen00,fechasesion_subsistema03
****************************************/
int fci_carga_totales03(formato_interno * ci_foritn)
{

   NPSTOTO reg_toto;
   int err, posic_subsis;
   char clave_toto[22];
   char clave_thpars[14];
   int sesion;
   char auxses[4];
   int ll;

  fprintLog(nomProg, "dentro de totales\n");
  fprintLog(nomProg, "Carga de totales Origen fidatpro.fiorigen >%s<\n",ci_foritn->fidatpro.fiorigen);
  
  //RAQUEL CAMBIAR EL SWITCH POR EL CAMPO DE LA TABLA NPTPARA
   for (ll=0 ; ll<2 ; ll++) 
   {
   
      if (strncmp(ci_foritn->fidatpro.fiorigen,reg_para[ll].psnument,2)==0)
	  {
	     strncpy(clave_thpars, reg_para[ll].psresolu, 2);
	     strncpy(clave_thpars+2, "01", 2);
		 break;
      }
   }
   clave_thpars[4]='\0';
  fprintLog(nomProg,"Sesion del subsistema >%.4s<\n", clave_thpars);
  posic_subsis= encuentra_svm(psvm,clave_thpars,TIP_SUBSIS);

  if (posic_subsis < 0)
  {
      fprintLog(nomProg,"No se encuentra el subsistema en SVM\n");
      fgraba_incidencia_cp("088", ci_foritn->fidatpro.finumope, "TH_SVM", NULL);
      strcpy(ci_foritn->fidatpro.ficodfin, "909");
      strcpy(ci_foritn->fidatpro.firesfin, "00");
      return(88);
  }

  //cambiar la sesion, la fcha se deja la de la sesion anteriro, por ahora no se usa
  strncpy((psvm + posic_subsis)->str_svm.svmpars.pafecses, ci_foritn->fidatiso.fiisotot.fitot028.aa, 2);
  strncpy((psvm + posic_subsis)->str_svm.svmpars.pafecses+2, ci_foritn->fidatiso.fiisotot.fitot028.mm,2);
  strncpy((psvm + posic_subsis)->str_svm.svmpars.pafecses+4, ci_foritn->fidatiso.fiisotot.fitot028.dd,2);
  strncpy(auxses,ci_foritn->fidatiso.fiisotot.fitot029, 3);
     fprintLog(nomProg,"fecha es %s\n", (psvm + posic_subsis)->str_svm.svmpars.pafecses);
  auxses[3]='\0';
  sesion = atoi(auxses);
  if (sesion == 999)
      sesion = 0;
  else
      sesion++;
  sprintf((psvm + posic_subsis)->str_svm.svmpars.panumses, "%03d", sesion);
  strncpy(clave_thpars + 4, (psvm + posic_subsis)->str_svm.svmpars.pafecses, 6 );
  strncpy(clave_thpars + 10, (psvm + posic_subsis)->str_svm.svmpars.panumses, 3 );
  clave_thpars[13]='\0';
  err = tratar_fich("NPTPARS",SESIONF,NULL, clave_thpars);
 // al ser 03, se actualiza el nptpara para la entidad 0001, pq aqui la entidad viene vacia.
  if (err != 0 )
  {
          fprintLog(nomProg,"error al cambiar la sesion en el PARS\n");
          fgraba_incidencia_cp("088", ci_foritn->fidatpro.finumope, "SESIONF", NULL);
          strcpy(ci_foritn->fidatpro.ficodfin, "909");
          strcpy(ci_foritn->fidatpro.firesfin, "00");
          return(-1);
  }

  memset(&reg_toto,'\0',sizeof(NPSTOTO));

  /* leer totales subsistema  se han grabado como de subsistema resolutor 03* 

    *************************************/
   strncpy(clave_toto, clave_thpars, 4);
   strncpy(clave_toto +4, ci_foritn->fidatiso.fiisotot.fitot029, 3);
   strncpy(clave_toto +7,"00",2); //es SISMA siempre en el rmonitor;
   strncpy(clave_toto +9, ci_foritn->fidatiso.fiisotot.fitot028.aa, 2);
   strncpy(clave_toto +11, ci_foritn->fidatiso.fiisotot.fitot028.mm, 2);
   strncpy(clave_toto +13, ci_foritn->fidatiso.fiisotot.fitot028.dd, 2);
   clave_toto[15]='\0';

   fprintLog(nomProg,"clave_totales   >%s<\n",clave_toto);

   err=tratar_fich("NPTTOTO",MONITOR, &reg_toto, clave_toto);
   if (err == 0)
   {   //vemos si ha cambiado la sesion
         fprintLog(nomProg, "En este momento he tratado todos los datos de la sesion clave_moni>%.20s<\n",clave_toto);

   }
   else
   {
     //Añadido para Monitorizacion SISMA 
    fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Lectura tabla NPTTOTO codigo de error >%d< \n",err);

       fprintLog(nomProg,"Error en el monitor %d\n",err);
   } 

    strncpy(ci_foritn->fidatiso.fiisotot.fitot109,reg_toto.toimabcu, 16); 
    ci_foritn->fidatiso.fiisotot.fitot109[16]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot110,reg_toto.toimcacu, 16);
    ci_foritn->fidatiso.fiisotot.fitot110[16]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot074,reg_toto.tonuabon, 10);
    ci_foritn->fidatiso.fiisotot.fitot074[10]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot075,reg_toto.tonuabon, 10);
    ci_foritn->fidatiso.fiisotot.fitot075[10]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot076,reg_toto.tonucarg, 10);
    ci_foritn->fidatiso.fiisotot.fitot076[10]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot077,reg_toto.tonuanca, 10);
    ci_foritn->fidatiso.fiisotot.fitot077[10]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot086,reg_toto.toimabon, 16);
    ci_foritn->fidatiso.fiisotot.fitot086[16]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot087,reg_toto.toimanab, 16);
    ci_foritn->fidatiso.fiisotot.fitot087[16]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot088,reg_toto.toimcarg, 16);
    ci_foritn->fidatiso.fiisotot.fitot088[16]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot089,reg_toto.toimanca, 16);
    ci_foritn->fidatiso.fiisotot.fitot089[16]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot109,reg_toto.toimabcu, 16);
    ci_foritn->fidatiso.fiisotot.fitot109[16]='\0';
    strncpy(ci_foritn->fidatiso.fiisotot.fitot110,reg_toto.toimcacu, 16);
    ci_foritn->fidatiso.fiisotot.fitot110[16]='\0';
 

    fverfi(*ci_foritn);

    return(0);

}//fin fci_carga_totales

/******************************************************
int graba_fichero_difer03(formato_interno *ci_foritn)
*******************************************************/
int graba_fichero_difer03(formato_interno *ci_foritn)
{
    #include "nptdife.h"

    int err=0,retorno;
    NPSDIFE regdife;
    NPSTOTO reg_toto;
    char numoper[6];
    char numcross[10];
    char clave_toto[16];


    memset(&regdife,' ',sizeof(NPSDIFE));

    fprintLog(nomProg, "\n  GENERANDO Cierre HOST\n");

    err = fnumope(numoper,numcross, '0');
    if(err)
    {
        fprintLog(nomProg, "ERROR AL FNUMOPE \n");
        strcpy(coderror, "264");
        return(err);
    }
    else
    {
        strncpy(ci_foritn->fidatpro.finumope, numoper,6);
        ci_foritn->fidatpro.finumope[6] = '\0';
        strncpy(ci_foritn->fiisocom.fiidtran, ci_foritn->fidatpro.finumope,6);
        ci_foritn->fiisocom.fiidtran[6] = '\0';
        fprintLog(nomProg, "finumope es >%s< fiidtran es >%s<\n", ci_foritn->fidatpro.finumope, ci_foritn->fiisocom.fiidtran);
    }

    memcpy(&paux_diforitn, ci_foritn, sizeof(formato_interno));

    tm_dec tm;
    fechorsys(&tm);

    strncpy(paux_diforitn.fidatpro.fifecha0, "20",2);
    sprintf(paux_diforitn.fidatpro.fifecha0+2,"%02d", tm.tm_year - 2000);
    sprintf(paux_diforitn.fidatpro.fifecha0+4,"%02d", tm.tm_mon);
    sprintf(paux_diforitn.fidatpro.fifecha0+6,"%02d", tm.tm_mday);
    sprintf(paux_diforitn.fidatpro.fihora00,"%02d", tm.tm_hour);
    sprintf(paux_diforitn.fidatpro.fihora00+2,"%02d", tm.tm_min);
    sprintf(paux_diforitn.fidatpro.fihora00+4,"%02d", tm.tm_sec);

/*** graba los totales de la red ***/   
   if (!strncmp(ci_foritn->fiisocom.respusal, "501", 3))
   { ///has descuadrado, los totales son los de la red
      strncpy(clave_toto, ci_foritn->fidatpro.fiorigen, 2); //RAQUEL AÑADIDO PARA 3 ENTIDADES
      //ORIGINAL strncpy(clave_toto, "30", 2);
      strncpy(clave_toto +2, ci_foritn->fidatpro.fisubori,  2);
      strncpy(clave_toto +4, ci_foritn->fidatiso.fiisotot.fitot029, 3);
      strncpy(clave_toto +7,ci_foritn->fidatpro.fiorigen,2); //RAQUEL AÑADIDO PARA 3 ENTIDADES
     //ORIGINAL strncpy(clave_toto +7,"30",2); //TOTALES DE LA RED
      strncpy(clave_toto +9, ci_foritn->fidatiso.fiisotot.fitot028.aa, 2);
      strncpy(clave_toto +11, ci_foritn->fidatiso.fiisotot.fitot028.mm, 2);
      strncpy(clave_toto +13, ci_foritn->fidatiso.fiisotot.fitot028.dd, 2);
      clave_toto[15]='\0';

      fprintLog(nomProg,"clave_totales   >%s<\n",clave_toto);

       err = tratar_fich("NPTTOTO",IGUAL,&reg_toto,clave_toto);

       if (err != 0)
        {
           fprintLog(nomProg, "ERROR AL LEER TOTALES %d\n",err);
           //Añadido para Monitorizacion SISMA 
          fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Lectura tabla NPTTOTO codigo de error 311\n");
            fgraba_incidencia_cp("311", ci_foritn->fidatpro.finumope, "trataTH",  NULL);

        }
        strncpy(ci_foritn->fidatiso.fiisotot.fitot109,reg_toto.toimabcu, 16);
        ci_foritn->fidatiso.fiisotot.fitot109[16]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot110,reg_toto.toimcacu, 16);
        ci_foritn->fidatiso.fiisotot.fitot110[16]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot074,reg_toto.tonuabon, 10);
        ci_foritn->fidatiso.fiisotot.fitot074[10]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot075,reg_toto.tonuabon, 10);
        ci_foritn->fidatiso.fiisotot.fitot075[10]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot076,reg_toto.tonucarg, 10);
        ci_foritn->fidatiso.fiisotot.fitot076[10]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot077,reg_toto.tonuanca, 10);
        ci_foritn->fidatiso.fiisotot.fitot077[10]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot086,reg_toto.toimabon, 16);
        ci_foritn->fidatiso.fiisotot.fitot086[16]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot087,reg_toto.toimanab, 16);
        ci_foritn->fidatiso.fiisotot.fitot087[16]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot088,reg_toto.toimcarg, 16);
        ci_foritn->fidatiso.fiisotot.fitot088[16]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot089,reg_toto.toimanca, 16);
        ci_foritn->fidatiso.fiisotot.fitot089[16]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot109,reg_toto.toimabcu, 16);
        ci_foritn->fidatiso.fiisotot.fitot109[16]='\0';
        strncpy(ci_foritn->fidatiso.fiisotot.fitot110,reg_toto.toimcacu, 16); 
        ci_foritn->fidatiso.fiisotot.fitot110[16]='\0';
     }

    retorno = fgraba_log(&paux_diforitn);

    fprintLog(nomProg, "vengo de grabar_log retorno->%d<\n", retorno);
    if(retorno)
    {
        strcpy(coderror, "222");
     //Añadido para Monitorizacion SISMA 
    fprintLog(nomProg,"TIPO SISTEMA – ACCESO A BASE DE DATOS: Escritura BBDD tabla NPTLOGO codigo de error >%.3s<\n",coderror);

        strcpy(ci_foritn->fidatpro.ficodfin, "909");
        strcpy(ci_foritn->fidatpro.firesfin, "00");
        return(-1);
    } /*fin del if */

    //err = fgraba_diferidos(&paux_diforitn);
	err = fgradife(&paux_diforitn);
	
    if(err)
        return(err);  

    fprintLog(nomProg, "\n  GENERADO Cierre HOST\n"); 

    return(0);
}

