/*************************************
*   funcion de finalizacion
**************************************/
#include "cforitno.h"
#include "deffich.h"
#include "nptinci.h"
#include "svm.h"

extern NPSINCI reginci; 
extern SVM *psvm;
extern int rtrata_svm;
extern char coderror[];
extern int sfd_hsm;

int fgraba_incidencia_cp(char *coderror, char *numope, char *nompro, char *nomfic);
int desconexion_BBDD();

int finalizar(int codigo)
{
   int err;

    if(coderror==0)
    {
        strncpy(coderror,reginci.incoderr, sizeof(reginci.incoderr));
    }
	
    switch(codigo)
    {
	     // SUPERVIS NO INTENTA REARRANCAR
        case -1: 
            fgraba_incidencia_cp(coderror, reginci.innumope,  reginci.innompro, reginci.innomfic);
			(psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
			(psvm+rtrata_svm)->estado=PARADA;			
			desconexion_BBDD(); //desconecto la bbdd al finalizar el servicio
            break;
			
		// SUPERVIS INTENTA REARRANCAR
        case -2: 
            fgraba_incidencia_cp(coderror, reginci.innumope,  reginci.innompro, reginci.innomfic);
			(psvm+rtrata_svm)->estado=RECARGAR; //indico rearrancar al supervis 
			//el pid permanece para que el supervis lo arranque
			desconexion_BBDD(); //desconecto la bbdd al finalizar el servicio
            break;
			
        case 0:  /* orden de cierre al proceso */
		    if(strncmp(coderror,"   ",3) ==0)
			     strncpy(coderror, "999", 3);
            fgraba_incidencia_cp(coderror, reginci.innumope,  reginci.innompro, reginci.innomfic);
            (psvm+rtrata_svm)->str_svm.svmproc.pid=-1;
            (psvm+rtrata_svm)->estado=NOUSO;
			desconexion_BBDD(); //desconecto la bbdd al finalizar el servicio
            break;
        case 11:  /* continuar procesando el mensaje */
#if defined (TRAZA) && (TRAZA > 10)  
            fprintf(stdout,"Finaliza con valor 11 continua...\n"); //PRJ0258542
#endif
            break;
        case 98: /* responder al origen   */
            if (strncmp(coderror,"   ",3)!=0)
               fgraba_incidencia_cp(coderror, reginci.innumope,  reginci.innompro, reginci.innomfic); 
            break;
        default:  /*error no contemplado */
            fgraba_incidencia_cp(coderror, reginci.innumope,  reginci.innompro, reginci.innomfic);
    }
    return(codigo);
}
