#ifndef TRAEETIQ_H
#define TRAEETIQ_H

#define NUM_ETIQUETAS_REG 100

typedef struct r_etiqueta 
{
 char etiqueta[4];
 int l_campo;
 char * p_campo;
} r_etiqueta;

typedef struct  petiquetas
{
 int n_etiquetas;
 r_etiqueta etiqueta[NUM_ETIQUETAS_REG];
} petiquetas;

void trae_etiquetas(petiquetas * salida, int max_etiquetas, char * regent);  
void fetiquetas_A_fi(formato_interno * pfi, petiquetas * str_etiq);

#endif
