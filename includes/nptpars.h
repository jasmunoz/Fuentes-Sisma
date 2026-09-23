#ifndef NPTPARS_H
#define NPTPARS_H
/*****************************************************************************
 *
 *
 *      (C) ERITEL, S.A   Septiembre, 1994.
 *      
 *              <nptpars.h>            Longitud = 340
 *
 *****************************************************************************/


/*    Registro del fichero de Parametros de Subsistema       */

typedef   struct {
    char            pasubsis[2];        /*  codigo de subsistema             */
    char            pasubapl[2];        /*  subaplicacion (SEMP)    Numero de ILT para SIO   */
    char            panomsub[20];       /*  nombre subsistema                */
    char            pasemaf1[1];        /*  Semaforo1 de Estado (PASEMAFO EN b.a.)   */
                                        /*     0 - DES                       */
                                        /*     1 - SNN                       */
                                        /*     2 - SUS                       */
                                        /*     3 - SNF                       */
    char            pasemaf2[1];        /*  Semaforo2 de Estado ( PAESTANT en b.a.)    */
                                        /*     0 - INI                       */
                                        /*     1 - SNN                       */
                                        /*     2 - SNN-SUS                   */
                                        /*     3 - SNF                       */
                                        /*     4 - SNF-SUS                   */
    char            paestado[2];        /* Estado del socket                 */
                                        /*  00 Estado proceso                */
                                        /*  01 Estado cerrado                */
                                        /*  02 Estado Erróneo                */

    //no esta en tablachar            pacodccd[3];        /*  Caracteristicas del dialogo      */
    char            patrepet[6];        /*  Tiempo de Respuesta peticiones   */
    char            patrerep[6];        /*  Tiempo de Respuesta repeticiones */
    char            patresnf[6];        /*  Tiempo de Respuesta SNF          */
    char            panumrea[1];        /*  numero de repeticiones           */
    struct        redef_datses
    {
        char        pafecses[6];       /*  fecha de la sesion               */ 
        char        panumses[3];       /*  numero de la sesion              */
        char        paconcil[1];       /*  flag de conciliacion :
                      1: conciliacion ha cuadrado
                      2: conciliacion ha descuadrado
                      3: conciliacion no disponible
                      0: sesion en curso               */
    } rdfses;
    char            panip[15];          /*  IP del socket                    */
    char            paportnume[5];      /*  puerto                 */
    char            pacidesti[13];       /*  Identificador destino            */
    char            paciadqui[13];       /*  Identificador adquirente         */

    char            paenvoff[1];       /*  Opcion envio off       */
    char            paenvtst[1];       /*  Opcion envio test
                                               0: Solo se envian test si semaforo cerrado
                                               1: Se envian test periodicamente */
    //char            pausofutu[13];
    char            patipsub[1]; /*tipo resolutor*/
    char            paidred[4];
	
} NPSPARS;


#endif
/* NPTPARS_H */
