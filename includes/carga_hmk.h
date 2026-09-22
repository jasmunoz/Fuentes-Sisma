#define __CARGA_HMK_H__

/*-----------------------------------------------------------------------------
        Constantes 
-----------------------------------------------------------------------------*/
#define CIFRADO         "1"
#define DESCIFRADO      "0"
//#define ECB             2       /* Modo de operacion ECB */    
#define CBC             "00"       /* Modo de operacion CBC */

#define TAMANO       4096
#define LONGMAX     12288       /* Maxima longitud del texto a cifrar o descifrar */
#define LONGMIN         8       /* Minima longitud del texto a cifrar o descifrar */

#define TRANSPORTE_ZONA  1
#define TRANSPORTE       0

#define DES_ERROK      90       /* No error */ 
#define ERRLENGTH      91       /* Longitud de texto a cifrar o descifrar
				 * invalida */
#define ERRCKDS        92       /* Error en la apertura del fichero CKDS o CRCACLV*/
#define ERRREAD        93       /* Error de lectura en el CKDS */
#define ERRWRITE       94       /* Error de escritura en el CKDS */
#define ERRSEEK        95       /* Error de busqueda en el CKDS */
//#define ERRSYS         96       /* Error del sistema en lectura del reloj */
#define ERRLABEL       97       /* Error por label no contenido en CKDS */

//#define LICENSE_ERROR  98       /* Error por licencia incorrecta */
// Opciones de comandos
#define TEST          "1401"
#define CALCULA_MAC   "0801"
#define CALCULA_CMAC  "0802" //PRJ0258542
#define ALTA_CLAVE    "0101" //PRJ0258542
#define BAJA_CLAVE    "0102" //PRJ0258542
#define IMPORTAR_TR31 "0206" //PRJ0258542
#define EXPORTAR_TR31 "0304" //PRJ0258542
#define CALCULA_KCV   "0404" //PRJ0258542
#define CALCULA_OFF   "0618" //PRJ0258542
#define TRANSLATE_PIN "0603" //PRJ0378949
#define EXPORT_PIN    "0605" //PRJ0378949
#define GENERA_PIN    "0619" //PRJ0378949
#define CIFRAR_PIN    "0611" //PRJ0378949
#define CIFRADO903    "0903" //PRJ0378949

#define PIN_CIFRADO   "0601"
#define CALCULA_PIN   "0601"
#define CALCULA_CVV   "0701"
#define CRIPTOGRAMA   "0503"
#define CALCULA_PVV   "0608" 
#define DESCIFRA_DATO "0901"
#define LISTADO	      "0103"
#define IMPORTACION   "0202"
#define BORRADO       "0102"
//#define DIVERSIFICA "1601"
#define EXPORTA	      "0303"

// Opciones de busqueda de identificador de claves internas
#define TRANSPORT       "02"
#define CIF_PIN         "03"
#define CAL_PIN         "04"
#define CVV             "05"
#define ARPC            "06"
#define DESCIFRA        "07"
#define PVV             "04" //La clave interna esta bajo LMK4, igual que la de calculo de PIN
#define MAC            	"08"
#define CIFRADO_EXTERNO	"D00"
#define CIFRADO_INTERNO	"N00"
#define ALMACEN_EXTERNO	"D"
#define ALMACEN_INTERNO	"S"
#define SIN_KCV         "00"
//#define ALGORIT_DIVERSI "12"

// Constantes para la composicion de los comandos relacionados con el criptograma
#define VERIF_ARQC      "0"
#define GENERA_ARPC     "2"
#define ESQUEMA_VISA    "0"
#define BLOQUE_ISO_0    "0"

#define LONG_MSG 4096


#define HEX_KEY_SIZE     16
