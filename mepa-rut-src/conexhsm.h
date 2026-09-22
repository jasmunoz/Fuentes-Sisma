#ifndef CONEXHSM_H
#define CONEXHSM_H


char literal_enti[31] = {"ENTIDAD NO CONTESTA"};
char literal_erro[31] = {"ERROR COMUNICACION"};
char literal_conf[31] = {"ERROR CONFIGURACION"};
char lit_lect_ckd[31] = {"CKDS.DAT NO EXISTE"};
char lit_lec_ckd[31] = {"CKDS.DES NO EXISTE"};
char lit_crcacvl[31] = {"CRCACLV NO EXISTE"};
char lit_extr_clv[31] = {"ERROR EXTRAC. CLAVES"};
char lit_conf_hsm[31] = {"ERROR CONFIGURAC.HSM"};
char lite_cifrado[31] = {"ERROR LBLAUXCLAVESCI"};
char lite_calcpin[31] = {"ERROR IMP.OSVISAKP01"};
char lite_cifrpin[31] = {"ERROR IMP.OSVISAKP02"};
char literal_coma[31] = {"COMANDO HSM NO INFORMADO"};
char literal_pin[31] =  {"ERROR EN MENSAJE DE PIN"};
char literal_fic[31] =  {"ERROR EN CARGA DE FICHERO"};
char literal_carg[31] =  {"CARGA DE CLAVES COMPLETADA"};
char literal_diver[31] = {"ERROR EN DIVERSIF.CLAVE"};
char literal_export[31] =  {"ERROR EN MENSAJE DE EXP.CLAVE"};
char literal_pinta[31] =   {"ERROR EN ACCESO A IDENT.HSM"};
char literal_cif[31] =  {"ERROR EN MENSAJE DE CIF"};
char literal_descif[31] =  {"ERROR EN MENSAJE DESCIF"};
//char tabla_decimal[16] = {"0123456789012345"};
char tabla_decimal[16] = {"849F79EE1F2945A3"};
char vector_inicial[16] = {"0000000000000000"};
char vector_inicial_aes[32] = {"00000000000000000000000000000000"}; //PRJ0258452
char diversificador[16] = {"0004820038584030"}; //PRJ037849 -- 04062026 cambio de 0000482003858403 a 0004820038584030
char opcion_cifrar[1] = {"1"};
char opcion_descifrar[1] = {"0"};

#endif

