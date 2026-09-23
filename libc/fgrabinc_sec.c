#include "deffich.h"
#include "datagen.h"
#include "nptinci.h"

void fprintLog(char ficheroPlantilla[50], char* format, ...);

extern char nomProg[50];

/***********************************************************************
  Funcion para graba incidencia
***********************************************************************/

//Grabar la incidencia
int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic)
{

    NPSINCI reginci;
    int retorno=0;
    char cadena_aux[8];	
	
    // Validar que llegue informado el parámetro coderror, 
    // si no es así termina la función retornando 0.
    if(coderror==0 || coderror ==NULL)
    {
        fprintLog(nomProg, "Fgraba_incidencia_cp - coderror no llega informado\n");
        return(0);
    }
	fprintLog(nomProg,"Graba incidencia coderror >%.3s<\n", coderror);
	
    // Inicializar reginci a espacios
    memset(&reginci, ' ', sizeof(reginci));

    // Obtenemos la fecha del sistema
    tm_dec tmPtr;
    fechorsys(&tmPtr);

	    strncpy(reginci.innompro, nomProg,8);

    strncpy(reginci.innomfic, "        ", 8);
    if(nomfic != NULL)
        strncpy(reginci.innomfic+8-strlen(nomfic), nomfic, strlen(nomfic));

    strncpy(reginci.innumope, "000000", 6);
    if(numope != NULL)
        strncpy(reginci.innumope+6-strlen(numope), numope, strlen(numope));
		

    strncpy(reginci.incoderr, "000000", 6); // Debe estar rellenos de 0 por la izquierda
    strncpy(reginci.incoderr+6-strlen(coderror), coderror, strlen(coderror)); // Debe estar rellenos de 0 por la izquierda
	
	   
	strncpy(reginci.inestado, "P",1); 

    sprintf(cadena_aux, "%04d%02d%02d", tmPtr.tm_year, tmPtr.tm_mon, tmPtr.tm_mday);
    strncpy(reginci.infecha0 ,cadena_aux,8);

    sprintf(cadena_aux, "%02d%02d%02d", tmPtr.tm_hour, tmPtr.tm_min, tmPtr.tm_sec);
    strncpy(reginci.inhora00 ,cadena_aux, 6);

    // Llamar al procedimiento que inserta en NPSINCI con la función tratar_fich
    retorno = tratar_fich("NPTINCI", ESCRIBIR, &reginci, NULL);
    if (retorno)
    {
       //AÃ±adido para Monitorizacion SISMA 
       fprintLog(nomProg,"TIPO SISTEMA - ACCESO A BASE DE DATOS: Tipo Insert NPTINCI codigo de error >%.3s<\n", coderror);

        fprintLog(nomProg, "retorno de grabaNPSINCI >%d<\n",retorno);
    }
 
    // Aunque de error tratar_fich, finalizar la función retornando 0
    return(0);
}



/***********************************************************************
  Nueva funcion para la actualizacion de alguna incidencias que se
  resuelven solas
***********************************************************************/

int factualiza_incidencia_cp(char *coderror, char * nompro, char *nomfic)
{

    // Validar que llegue informado el parámetro coderror,
    // si no es así termina la función retornando 0.
    if(coderror==0)
    {
        fprintLog(nomProg, "Error >%.3s< el campo coderror no llega informado\n",coderror);
        return(0);
    }


    NPSINCI reginci;
    int retorno=0;

    // Inicializar reginci a espacios
    memset(&reginci, ' ', sizeof(reginci));

    if(nompro == NULL)
        strncpy(reginci.innompro, "000000", 8);
    else
        strncpy(reginci.innompro, nompro, 8);

    if(nomfic == NULL)
        strncpy(reginci.innomfic, "000000", 8);
    else
        strncpy(reginci.innomfic, nomfic, 8);

    strncpy(reginci.incoderr, coderror, 6); // Debe estar rellenos de 0 por la izquierda

    strncpy(reginci.inestado, "T",1);

    // Aunque de error tratar_fich, finalizar la función retornando 0
    return(0);
}


