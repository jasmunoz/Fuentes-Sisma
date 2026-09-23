#include <stdio.h>
#include <process.h>
#include <tchar.h>

#include "cforhost.h"
#include "cforitno.h"
#include "netwin.h"
#include "deffich.h"
#include "nptinci.h"
#include "nptpars.h"
#include "npttoto.h"
#include "nptlogo.h"

#define SZSERVICENAME        PREFIX"cierr30"

typedef struct mensaje_nucleo
{
    long  tipo;
    formato_interno fiforitn;
}mensaje_nucleo;

mensaje_nucleo   msg_nucleo;
unsigned int long_msgnuc = sizeof(mensaje_nucleo);

SECURITY_ATTRIBUTES     sa;
OVERLAPPED              os;
TCHAR                   szErr[256];



NPSINCI reginci;
NPSPARS reg_pars;

char coderror[4];
char error[3];
int lanza_cierre_Red();
int lanza_control_Red(char * codMCD);


int lanza_control_Red(char * );
int fgraba_incidencia_cp(char *coderror, char *numope, char *descriPar, char *codent, char *nompro, char *nomfic)
int inicfori (int tipomsg, formato_interno *pfi);
int festado_conexion(int  opcion, char * subapl_ilt, char * estado, char opcenvoff);
void fprintLog(char* format, ...);
void fhexdumpLog(void*, long);


int main()
{
  int retorno;
  int tiempo_respuesta;
  int t_espera_Red;
  char ch_tmresp[6];
  char char03[3];
  int estado_actual;
  int intentos;
  int intent_recup; //en vez 4 intentos de 30 segundos y luego esperar 1 minuto, ponemos 6 intentos
  int cierre_lanzado;
  char clave_pars[5];

    // lee parametros de subsistema para coger tiempo de espera
	//Raquel creo que no se usa este fuente porque los parametros estan a pelo para inversis

    strncpy(clave_pars, "3001",4);
    clave_pars[4]='\0';
    strncpy(reg_pars.pasubsis,"3001",4);  
    retorno = tratar_fich("NPTPARS", IGUAL, &reg_pars, clave_pars);

    strncpy(ch_tmresp, reg_pars.patrepet, 6);   //hora     
    strncpy(char03, ch_tmresp, 2);
    tiempo_respuesta =  3600 * atoi(char03);
    strncpy(char03, ch_tmresp + 2, 2);          //minutos 00
    tiempo_respuesta += 60 * atoi(char03);
    strncpy(char03, ch_tmresp + 4, 2);          //segundos 30 
    tiempo_respuesta += atoi(char03);

    t_espera_Red=tiempo_respuesta * 1000; /* espera 30 seg */

    /***********/
    cierre_lanzado=0;
    intentos=0;
    intent_recup = 6; // t_recup = 4;


    retorno = lanza_control_Red("03");    // interrupcion

    Sleep(30000); // cambio hos 28/05/2009 //espero medio segundo, por si 4B nos contesta al mensaje 1813

    estado_actual=festado_conexion(1,"00", "00", '2');
    fprintLog("estado_actual >%d<\n",estado_actual);


    if (estado_actual == 29 )
    {
        fprintLog("------------------------------- lanza_cierre ----------------------------------\n");
        retorno = lanza_cierre_Red();
        cierre_lanzado=1; //para que no entre en el while
        Sleep(2000); //damos tiempo a que se ejecute el procedimiento de totalizacion
    }
    else
    {
        retorno = lanza_control_Red("07");    // tst
        Sleep(500);
        estado_actual=festado_conexion(1,"00", "00", '2');
        fprintLog("estado_actual >%d<\n",estado_actual);

        if (estado_actual == 69)
        {
            fprintLog("sesion en recuperacion 1\n");
            Sleep(t_espera_Red);  //damos tiempo para que 4b interrumpa la sesion
                           // ya que ellos tardan 30 seg en interrumpir. 21/03/2007
            retorno = lanza_control_Red("07");    // tst
            Sleep(500); //medio seg para darle tiempo a la respuesta del tst y modificar el estado
                     // en paramertros del subsistema;
            estado_actual=festado_conexion(1,"00", "00", '2');
            fprintLog("estado_actual >%d<\n",estado_actual);
        }

    }//fin del else

        fprintLog("Antes del WHILE intentos = >%d<\n",intentos);

        while(intentos <=5 && cierre_lanzado==0)
        {
            switch (estado_actual)
            {
            case 9://sesion abierta;
                fprintLog("sesion abierta envio interrupcion\n");
                retorno = lanza_control_Red("03");  //interrupcion
                Sleep(30000); //espero medio segundo

                estado_actual=festado_conexion(1,"00", "00", '2');
                fprintLog("estado_actual >%d<\n",estado_actual);
                break;
            case 29://estado de interrupcion porque nos han respondido a la interrupcion;
                fprintLog("--------------------------------lanza_cierre --------------------------------------\n");
                retorno = lanza_cierre_Red(); // retorna 0 si no hay error
                cierre_lanzado=1;//para salir de while
                Sleep(2000); //damos tiempo a que se ejecute el procedimiento de totalizacion
                break;
            case 69://sesion en recuperacion;
                fprintLog("sesion en recuperacion\n");
                while(estado_actual == 69 && intent_recup >0)
                {
                    Sleep(t_espera_Red);  //damos tiempo para que 4b interrumpa la sesion
                                   // ya que ellos tardan 30 seg en interrumpir.
                    fprintLog("dentro del while de sesion en recuperacion\n");
                    retorno = lanza_control_Red("07");    // tst
                    Sleep(500);
                    estado_actual=festado_conexion(1,"00", "00", '2');
                    fprintLog("estado_actual >%d<\n",estado_actual);
                    intent_recup--;
                    fprintLog("intent_recup >%d<\n",intent_recup);
                }
                    if (estado_actual == 69 && intent_recup == 0)
                    {
                        //t_recup=4;
                        Sleep(60000); //1 minuto para volver intentarlo.
                    retorno = lanza_control_Red("07");    // tst
                            Sleep(500);
                      estado_actual=festado_conexion(1,"00", "00", '2');
                    }

                break;
            default:
                fprintLog("antes del Sleep del default \n");
                Sleep(tiempo_respuesta * 3000); // 1 minuto y medio
            }//fin switch

            intentos ++;

            /* por si no contesta al mensaje de interrupcion */
            /****** cierre_lanzado = 1 no hacer tst, porque ya se hecho el cierre con 4B
             y no acepta el tst por estar la sesion cerrada */

            if (cierre_lanzado ==1)
            retorno = lanza_control_Red("07");    // tst

            Sleep(500);
            estado_actual=festado_conexion(1,"00", "00", '2');

            fprintLog("estado_actual >%d<\n",estado_actual);
            fprintLog("dentro del WHILE al FINAL del intento >%d<\n",intentos);

        }//fin de while de intentos general de cierre

        fprintLog("Ha salido del WHILE en el intento >%d<\n",intentos); 

//  } //fin del else

    if (cierre_lanzado==0)
    {
        fprintLog("ERROR no sa ha realizado el cierre con Red\n");
        fgraba_incidencia_cp("312",NULL, "BCIERRed ERROR,No se ha realizado el cierre Red", NULL, NULL, NULL);
    }//fin contador;
    else
    {
        //si se ha lanzado el cierre ahora lanzamos la apertura

        intentos=30;

        //estado_actual=festado_conexion(1,"00", "00", '2');

        fprintLog("Antes del while estado_actual distinto 0 -- estado_actual >%d<\n",estado_actual);

        // para dar tiempo a que se haga el cierre
        // bucle mientras estado sea no cerrada, o no haya esperado el tiempo total
        // la respuesta al cierre recibida de 4B se encarga de modificar la sesion a cerrada.
        while ((intentos > 0) && (estado_actual !=0 ))
        {
            Sleep(500);
            intentos--;
            estado_actual=festado_conexion(1,"00", "00", '2');

           /*** AQUI NOS ESTA DANDO 99 porque NOS HAN CONTESTADO CON ERROR 4B AL CIERRE
         por ello vamos a grabar una incidencia que los de la guardia tienen que llamar
         directamente a los de explotacion 4b, porque no podemos hacer nada. ya que si intentamos
        hacer una reanudacion de la sesion nos dara 99, y en el momento que 4B nos diga que podemos
        realizar el cierre solo tienen que ejecutar el bciersio*/


        if (estado_actual == 99)
        {
            /* ESTO PROBOCA INTERVENCION MANUAL */
            fprintLog("**************************************************\n");
            fprintLog("****** 99 ERROR EN CIERRE CON 4B **********\n");
            fprintLog("**************************************************\n");
        fgraba_incidencia_cp("099",NULL, "BCIERRed No aceptado cierre por 4B", NULL, NULL, NULL);
            return(99);
        }//fin de error de cierre;


        }//fin while
        if (estado_actual==0) //cerrada
            retorno = lanza_control_Red("01");   // apertura

    }

    // Al final comprobamos si la sesión está abierta
    // bucle mientras estado sea no abierta, o no haya esperado el tiempo total
    while ((intentos > 0) && (estado_actual != 9))
    {
        Sleep(500);
        intentos--;
        estado_actual=festado_conexion(1,"00", "00", '2');
    }

/* solo enviamos la reanudación si la sesion esta interrumpida, ya no hace falta reanudar la sesion,
  porque con la apertura de sesion ya va implicitamente
 la reanudacion del trafico*/

    if(estado_actual==29)
        retorno = lanza_control_Red("04");    // reanudacion  


    fprintLog("Fin bciersio\n");
    return (0);

}

#include "fcierre_Red.c"
#include "fcontrol_Red.c"
#include "festado_conexion.c"
#include "funcigen.c"
