
#include "datagen.h"
#include "cnetplus.h"
#include <sys/stat.h>
#include <errno.h>
#include "deffich.h"

char nomProg[20];

/*********************
ejecutable para hacer las tareas necesarias despues del cierre de sesión
- mover las trazas del tux a otro directorio
- mover las trazas del resto de sisma a otro directorio
- lanzar las estadisticas
*********************/
int main(int argc, char *argv[])
{

tm_dec tm;
void fechorsys(struct tm_dec *tmPtr);
char fechanum[13]; //aaaammddhhmm
char comando[200];
int retorno = 0;
int retTraza=0;
int retEsta=0;
char clave_est[9];

// incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

	strncpy(nomProg, "bfindedia", 9);
	nomProg[9]=0;
	
	//fprintLog(nomProg,"PROCESO bfindedia\n");
	
	fechorsys(&tm);
		
    ////////////////////////////////////////////////////////////////////////////////////////	
	//crear el directorio de trazas /sisma/trazas a /sisma/datos/trazas
	////////////////////////////////////////////////////////////////////////////////////////	
    sprintf(fechanum, "%04d%02d%02d%02d%02d%",tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min);
    sprintf(comando,"%s%s/tr%s",HOME,DIRDTSIS,fechanum);
    errno=0;
	
	
    retTraza=mkdir(comando,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
	
    if (retTraza)
	{
      //fprintLog(nomProg,"Error >%d< en mkdir(%s,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)\n",errno,comando);
	  fprintf(stdout,"Error >%d< en mkdir(%s,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH)\n",errno,comando);
	  fflush(stdout);
	}  
	else
	{
	  //fprintLog(nomProg,"creado directorio %s\n", comando);
	  fprintf(stdout,"creado directorio %s\n", comando);
	  fflush(stdout);
	} 
	  
    ////////////////////////////////////////////////////////////////////////////////////////	
	//mover las trazas del directorio /sisma/trazas 
	//                        y del   /sisma/tux a /sisma/datos/trazas
	//cuya fecha no sea hoy
	////////////////////////////////////////////////////////////////////////////////////////	    	

    sprintf(comando,"%s%smovetraza %s%s tr%s",HOME,HOMENET,HOME,HOMENET,fechanum);
	fprintf(stdout, "Ejecuto >%s<\n",comando);
    system(comando);  
		
	/////////////////////////////////////////////////////////////////////////////////////
	// comprimirlas
	/////////////////////////////////////////////////////////////////////////////////////	
	sprintf(comando, "gzip -9 %s%s/tr%s/*", HOME,DIRDTSIS,fechanum);
	system(comando);
		
	/////////////////////////////////////////////////////////////////////////////////////
	// lanzar el procedimiento de estadísticas
	/////////////////////////////////////////////////////////////////////////////////////
    strncpy(clave_est, fechanum, 8); //aaaammdd
    clave_est[8]='\0';
	fechorsys(&tm);
	fprintf(stdout,"%04d%02d%02d-%02d:%02d:%02d  clave_est >%s<\n",  
	         tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec, clave_est);
		
    retEsta=tratar_fich("NPTESTA", TOTAL, NULL, clave_est);
	//retEsta=0;
	
	fechorsys(&tm);
    fprintf(stdout,"%04d%02d%02d-%02d:%02d:%02d retorno de ESTADISTICAS :>%d<\n",
	           tm.tm_year,tm.tm_mon,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec,retorno);
    fflush(stdout);			   
    if(retEsta)
    {
      retorno=fgraba_incidencia_cp("203", NULL,"NPTESTA", "NPTESTA");
	  //fprintLog(nomProg, "retorno de grabar incidencia >%i<\n", retorno);
	  fprintf(stdout,"retorno de grabar incidencia >%i<\n", retorno); 
	  fflush(stdout);
    }
	if(retEsta || retTraza)
	   return(-1); // si algo falla devuelvo -1 para que lo detecte el autosis 
	else
	   return(0);
			
}//fin main
