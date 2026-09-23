#ifndef CLAVTAR_H
#define CLAVTAR_H
/*****************************************************************************
 *
 *
 *      (C) ERITEL, S.A   Septiembre, 1994. NO SE USA
 *
 *              <clavtar.h>
 * 
 *      Contiene las claves de  tarjeta  asociadas a la tarjeta
 *      empleadas en la aplicacion con su correspondiente literal      
 *
 *****************************************************************************/
char   *   clave_tar[ ][2] = {
                             {"01","Diario y debito                       "},
                             {"02","Diario y credito                      "},
                             {"03","Diario, debito y credito              "},
                             {"11","Diario y cuenta debito                "},
                             {"12","Diario y cuenta credito               "},
                             {"13","Diario y cuenta debi. y cre.          "}, 
                             {"17","Diario, deb., cre. y cuenta deb. y cre"}, 
                             {"18","Diario, deb. y cuenta deb.            "},
                             {"19","Diario, cred. y cuenta credito        "}, 
                             {NULL,NULL                                    },   
                             };
#endif
/* CLAVTAR_H */
