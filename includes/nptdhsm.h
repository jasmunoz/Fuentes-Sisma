#ifndef NPTDHSM_H
#define NPTDHSM_H
/*****************************************************************************
 * 
 *              <nptdhsm.h>            Longitud = 340
 *
 *****************************************************************************/


/*   copy para conexion HSM **/

typedef   struct {
	char    dhcodent[4];      /*  codigo de entidad           pk   */
	char    dhtiempo[4];      /*  tiempo de respuesta del hsm      */
	char    dhcabece[10];     /*  cabecera del HSM                 */ 
	char    dhnumip[15];      /*  IP del HSM                       */
	char    dhpuerto[4];      /*  puerto del HSM 	*/
	char    dhprodt[1];       /*  produccion 1- utilzamos hsm 0- no utilizamos hsm. */
	char    dhnumip2[15];     /*  IP secundaria */
	char		dhsemafhsm1[1];		/* Semaforo del HSM principal 0 = KO - 1 = OK */ //PRJ0258542
	char		dhsemafhsm2[1];		/* Semaforo del HSM secundario 0 = KO - 1 = OK */  //PRJ0258542
	char		dhparenov[2];			/* Control renovacion - 99 = No renueva, 98 = error hsm, xx = clave que se esta renovando */  //PRJ0258542
} NPSDHSM;

   
#endif
/* NPTDHSM_H */
