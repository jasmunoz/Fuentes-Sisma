
/*******************
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <process.h>  

int segundos, milesimas; 

/*********************
*********************/
void _CRTAPI1 main(int argc, char **argv)
{

    segundos = atoi(argv[1]); 

    printf("segundos >%d<\n",segundos);  

    milesimas = segundos * 1000;  

    Sleep (milesimas);

    printf("despues de Sleep segundos >%d<\n",segundos); 
}//fin main