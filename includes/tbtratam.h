#ifndef TBTRATAMIENTOS_H
#define TBTRATAMIENTOS_H
/*****************************************************************************
 *
 *
 *    
 *
 *              <tbtratamientos.h>  
 *
 *      Contiene los tratamienots y sus punteros
 *      Es la que va a redirigir al tratami segun la clave NPTTBOP hacia
 *      cada uno de estas funciones 
 *
 *****************************************************************************/

typedef struct tratami
{
   char simbolo[3];
   int  (*pfunc)(formato_interno *pforitn);
} tratami;

tratami    tratamientos[ ] = {
                             {"EO",funcioneo},
                             {"PR",funcionpr},
                             {"ER",funcioner},
                             {"DP",funciondp},
                             {"CO",funcionco},
                             {"MS",funcionms},
                             {"RP",funcionrp},
                             {"RD",funcionrd},
                             {"ED",funcioned},
                             {"CH",funcionch},
                             {"TO",funcionto},
                             {"SS",funcionss},
                             {"AC",funcionac},
                             {"TH",funcionth},
                             {"DI",funciondi},
						     {"VH",funcionvh}, // validaciones hsm
						     {"LG",funcionlg},
                             {"xx",NULL},
                             };
#endif
/* TBTRATAMIENTOS_H */
