#ifndef FCIFPAN_H
#define FCIFPAN_H

/*-----------------------------------------------------------------------------
        Constants
-----------------------------------------------------------------------------*/
#define CIFRADO_HSM         "1"
#define DESCIFRADO_HSM      "0"
//#define ECB             2       /* Modo de operacion ECB */
#define CBC_HSM             "00"       /* Modo de operacion CBC */  

// Opciones de comandos
#define DESCIFRA_DATO "0901"


// Opciones de busqueda de identificador de claves internas
#define DESCIFRA_HSM    "07"
#define CIFRADO_EXTERNO "D00"
#define CIFRADO_INTERNO "N00"
#define ALMACEN_EXTERNO "D"
#define ALMACEN_INTERNO "S"
#define SIN_KCV         "00"


//#define FICH_INI_HSM      "Sisma.ini"  
#define LONG_MSG 4096


typedef struct datos_cifrado
    {
    char  entrada[20];
    char  cabecera[10];
    char  identificador_cifrado[20];
    int aux_sfd;
    } datos_cifrado;

#endif
/* De FCIFPAN_H */