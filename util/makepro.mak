# Make 

COMPILER=cc
export COMPILER
COMPILER_C=proc
export COMPILER_C
#aplicación para montar la libreria libmepa
ARLIB=ar -r
export ARLIB

OBJ=$(MP_SISMA)/obj
export OBJ
EXEC=$(MP_SISMA)/exec
export EXEC
INCLUD=$(INCLUDES)
export INCLUD
FUEN1=$(FUENTES)
export FUEN1
LIBNET=$(OBJ)/libsisma.a
export LIBNET
LINKER=cc
export LINKER

#Las variables usadas para los procesos son:
#
#En los programas C
#DUAL: En arranque.c comprueba que no haya otro NETplus en otra maquina (NPI)
#FIMESTIC: Para incluir datafono de FIMESTIC o FINACONSA (A eliminar)
#HIGH_LOW: En ftradpuc.c para el orden de los bits
#MDIGITAL: Para compilar en Digital (Ojo no esta en todos los programas)
#MIGUEL: En ftracarr.c (NPI) (A eliminar al acabar)
#MONOTIEN: En benvfich.c para generar un fichero por tienda
#ORACLE: En supervisor (A eliminar)
#PRUEBAS: Para el MAC (pcalcmac.c)
#SINRESPALDO: En ftrataus.c (NPI)
#TRAZA: Pinta Trazas
#UNIX: Para compilar la x25 

#En includes H
#PRUEBAS: Para arrancar un tratami (arranque.h)

#Antigua antes de modificar
#P= -O -DHIGH_LOW -D$(DESTINO) -DFSTRING -DUNIX $(DEF_COMUN) -DTRAZA19 -I $(INC_COMUN) -I $(INCLUD) -I $(INCLUD_FIC) -I $(FUEN1)  -I $(LIBCRYP)
#cc -m64 -c -g -DHIGH_LOW -DFSTRING -DSUN_OS -DUNIX -DTRAZA=20 -std=gnu99 -I/sf/sisma/includes -D_REENTRANT -lssl -lcrypto -fPIC -Werror -I$ORACLE_HOME/precomp/public -Wimplicit-function-declaration /sf/sisma/libc/crearsvm.c
##-m64 -Werror -Wimplicit-function-declaration -g -I. -I${DIR_APLIC}/include -I${TUXDIR}/include -fPIC 
P= -O -DHIGH_LOW -DFSTRING $(DEF_COMUN) -DTRAZA19 -I$(INCLUD) -I$(FUEN1) 
export P
PROCFLAGS=
include=\($ORACLE_HOME/precomp/public/,$DIRECTORIO\)
export include

LN=curses
export LN
LN2=/opt/oracle/product/12.1.0/client_1/precomp/lib
export LN2
LN3=nsl -lssl -lcrypto 
export LN3
LN6=sisma
export LN6
LN7=/opt/oracle/product/12.1.0/client_1/lib -lclntsh
export LN7

include progrpro.mak

