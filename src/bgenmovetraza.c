/*************************************************************************************************************
     bgenmovetraza.c
       programa que mueve las trazas del directorio  homenet/dat/trazas    a   homenet/dat_his/trazas/tr...
**************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "datagen.h"
#include "netwin.h"
#include "cforitno.h"
#include "deffich.h"


struct tm_dec *tm;
struct tm_dec vtm;

fecha   fecgre;
juliana fecjul_act;
juliana fecjul_cad;

char    fechacad[9];
char    fechafich[9];

FILE  *pflista;

char   reg[300];

char   comando[300];
char   flista[200];
char   fichero[200];

char dirtraza[25];

int  retorno;

/***   numero de dias a guardar los ficheros   ***/
int guardar_enLocal = 30;
int guardar_enHist = 60;/*guardar las trazas 1 mes */
int fcompara_fechas (fecha *fecha1, fecha *fecha2);
int fsemanal(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio);
int funoyquince(fecha *pfecha_operacion, fecha *pfecha_inicio);
int fmeses(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio);
int fdiaria(int periodo, fecha *pfecha_operacion, fecha *pfecha_inicio);
int fdiferencia_en_dias(fecha *pfecha_operacion, fecha *pfecha_inicio);
void fponemeses(int meses,fecha *fechag, int inidia);
int borra_lista(char * fichero, int tipo, char * directorio, int guardar);
int limpia_USOS(fecha * fecgre);
int limpia_NPTCASA();
int limpia_NPTCA03();
void fprintLog(char* format, ...);
int fjuliana(fecha *fechag,juliana *fechaj);
//int mueve_trazas(char * flista, char * dirtraza1);
int mueve_trazas();


#include "fcaldisp.c"


main()
{

 GetLocalTime(&tt);
 fprintLog("BGENMOVETRAZA\n");

 retorno=mueve_trazas(); // mueve a datos/trazas creando una carpeta datada y con la ip incluida en el nombre

 fprintLog("FIN\n");
 exit(0);

}


///////////////////////////////////////////////////////////////////////////////////////
//int mueve_trazas(char * flista,char * dirtraza1)
int mueve_trazas()
{
    SYSTEMTIME tt;
    char ddmmaaaa_hh[11];
    char fechaFichero[11];
    char dirtrazaFecha[100];
    char trazaFecha[100];// nuevo
    int i=0;
    int posicionPunto=0;
    int posicionFin=0;
    int retorno=0;
    char ip[20];
    int hora =0; //nuevo


    int saca_ip_local(char *ip);
    void fEliminar_Blancos (char * cadena_entrada, int long_entrada);

    /////////////////////////////////////////////////////////////////////////////

    fprintLog("MOVER TRAZAS DE LA SESION AL DIRECTORIO DATOS-TRAZAS\n");
    
    
 // incluido para recuperar la variable de entorno y crear las trazas en su entorno correspondiente
// segun el usuario que arranque

  char* HOME;
  HOME = getenv ("SF_HOME");

    retorno=saca_ip_local(ip);
    fEliminar_Blancos(ip,sizeof(ip));
    fprintLog("strlen ip >%d<\n", strlen(ip));

GetLocalTime(&tt);

    hora = tt.wHour;

    fprintLog(" hora en el instante >%d<\n",hora);

    /*  restar hora */

     if (hora == 0)
     {
         hora = 23;
     }else{
     hora = hora --;
     }


     fprintLog("menos una hora >%d<\n",hora);

   sprintf(dirtrazaFecha, DIRDATRZ"tr%d%02d%02d_%.*s",tt.wYear, tt.wMonth, tt.wDay,strlen(ip),ip);
   sprintf(trazaFecha,"tr%d%02d%02d_%.*s",tt.wYear, tt.wMonth, tt.wDay,strlen(ip),ip);

   fprintLog("dirtrazaFecha >%s<\n",dirtrazaFecha);
   fprintLog("trazaFecha >%s<\n",trazaFecha);


    //if not exist C:\SismaDes\datos\trazas\tr20080723 mkdir C:\SismaDes\datos\trazas\tr20080723
    sprintf(comando,"if not exist %s mkdir %s",dirtrazaFecha, dirtrazaFecha);
    fprintLog("comando->%s<-\n",comando);
    retorno=system(comando);

    /** nuevo  **/
    fprintLog("retorno de crear directorio >%d<\n",retorno);

    sprintf(comando,"move %s%s*_%02d.traza %s", HOME,DIRTRAZA,hora,dirtrazaFecha);


   fprintLog("comando al mover ->%s<-\n",comando);
        retorno=system(comando);
  /** fin del nuevo **/


      fprintLog ("trazaFecha 1>%s<\n",trazaFecha);

      sprintf(comando,"%s%sbmovetraza %s> %s%smovetraza.traza", HOME,DIRSCRIP,trazaFecha,HOME,DIRTRAZA); // mueve de datos/trazas al disco E
     //sprintf(comando,"%sbmovetraza %s > %smovetraza.traza", DIRSCRIP,dirtraza, DIRTRAZA); // mueve de datos/trazas al disco E
     fprintLog("comando>%s<\n", comando);
     retorno=system(comando);
     if (retorno)
         fprintLog("Error moviendo las trazas a la maquina HISTORICOS\n");
	 
    return(0);
}

/******************************************************************************
   funcion que genera una lista de ficheros, selecciona los que debe borrar
   y los borra. Los parametros que recibe son :
   -  comando que obtiene el listado en un fichero auxiliar 'auxi.lis'
   -  posicion en el fichero obtenido de la parte del nombre del fichero que
      lleva la fecha en formato AAMMDD
   -  Tipo de objetos a borrar :   1 :    Ficheros
                                   2 :    Directorios
******************************************************************************/
int borra_lista(char * flista , int tipo, char * directorio, int guardar)
{
    struct stat est_fichero;
    struct tm * esttm;

    fprintLog("BORRAR LISTA DE FICHEROS\n");

    fecjul_cad.dia = fecjul_act.dia;
    fecjul_cad.anio = fecjul_act.anio;
    fresta_dias(&fecjul_cad, guardar);
    fprintLog("fecjul_cad >>%04d:%03d<<\n",fecjul_cad.anio,fecjul_cad.dia);

    fecgre.ss = 0;
    fecgre.aa = 0;
    fecgre.mm = 0;
    fecgre.dd = 0;

    retorno = fgregoriana(fecjul_cad , &fecgre);

    if(retorno && retorno != 2)
    {
        fprintLog("error en feccad\n");
        fprintLog("fecjul_cad >>%04d:%03d<<\n",fecjul_cad.anio,fecjul_cad.dia);
        exit(-1);
    }

    sprintf(fechacad, "%02d",   fecgre.ss);
    sprintf(fechacad+2, "%02d", fecgre.aa);
    sprintf(fechacad+4, "%02d", fecgre.mm);
    sprintf(fechacad+6, "%02d", fecgre.dd);

    fprintLog(">>>>>>>>>>>>> FECHA DE CADUCIDAD >>>%s<<<\n",fechacad);


    if((pflista = fopen(flista, "r")) == NULL)
    {
        fprintLog("error al abrir flista >*s<\n", flista);
        return(1);
    }

    while(fgets(reg, 300, pflista) != NULL)
    {
        fprintLog( "reg ->%s<-\n", reg);

        if (reg[0] == ' ' || reg[0] == 0x0A)
            continue;

        if(!strncmp(reg, "auxi.lis", 8))
            continue;

        /* Comparamos la fecha de modificacion */
        *strchr(reg, '\n') = '\0';

        sprintf(fichero, "%s%s", directorio, reg);
        retorno = stat(fichero, &est_fichero);

        if(retorno == -1)
        {
            fprintLog( "ERROR: Imposible recuperar %s\n", fichero);
            continue;
        }
        if(tipo == 1 && !est_fichero.st_size)
        {
            fprintLog( "Tam fichero ->%lu<-\n", est_fichero.st_size);
            memset(fechafich, '0', 8);
        }
        else
        {
            esttm = localtime(&est_fichero.st_mtime);
            sprintf(fechafich, "%04d", 1900 + esttm->tm_year);
            sprintf(fechafich+4, "%02d", esttm->tm_mon + 1);
            sprintf(fechafich+6, "%02d", esttm->tm_mday);
        }
        fprintLog( "fechafich ->%.8s<-\n", fechafich);

        if (strncmp(fechafich , fechacad , 8) < 0)
        {
            fprintLog("borrando %s\n", fichero);
            if (tipo == 1)
                sprintf(comando,"del /q %s", fichero);
            if (tipo == 2)
                sprintf(comando,"rmdir /s /q %s", fichero); 
                fprintLog("comando->%s<-\n",comando);
                system(comando);
        }
    }
    fclose(pflista);
    return(0);
}
/* ************************************************************************* */
/* Funcion que elimina los espacios en blanco, colocando el fin de cadena    */
/* ************************************************************************* */
void fEliminar_Blancos (char * cadena_entrada, int long_entrada)
    {

    int tamanio_entrada = 0;
    char cadena_aux[1024];  


    memset(cadena_aux, '\0', sizeof(cadena_aux));

    while (cadena_entrada[tamanio_entrada]!=' ' && tamanio_entrada < long_entrada)
        {
        sprintf(cadena_aux + tamanio_entrada, "%c", cadena_entrada[tamanio_entrada]); 
        tamanio_entrada++;
        } 
    strncpy(cadena_entrada,cadena_aux,tamanio_entrada);
    cadena_entrada[tamanio_entrada] = '\0';
    }
