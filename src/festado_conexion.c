
/******************************************************************
   mira en NPTSIOC  el estado de la conexión con 4B
******************************************************************/
int festado_conexion(int  opcion,char * canal,char * estado, char opcenvoff)
{

    NPSSIOC reg_sioc;
    char clave_sioc[7];
    char Estado_Conexion[3];

    memset(clave_sioc,' ',6);
    strncpy(clave_sioc,"0000",4);
    strncpy(clave_sioc +4,canal,2);
    clave_sioc[6]='\0';

    fprintLog("Estado_Conexion  opcenvoff >%c< \n", opcenvoff); 
    fprintLog("EL CANAL >%.2s<\n",canal);

    switch(opcion)
    {

    case 1:  // (CONSULTA)
        fprintLog("consulta \n");
        retorno = tratar_fich("NPTSIOC", MAYORIGUAL, &reg_sioc, clave_sioc);
        fprintLog("consulta retorno %d \n",retorno);

        if(retorno)
        {
            fprintLog("Retorna 99\n");
            return(99);
        }
        else
        {
            memset(Estado_Conexion,'\0', sizeof(Estado_Conexion));
            strncpy(Estado_Conexion, reg_sioc.scestado, 2);

            fprintLog("Estado_Conexion %.*s\n",2,reg_sioc.scestado);

            opcenvoff= reg_sioc.scenvoff[0];

            fprintLog("Retorna %d\n", atoi(Estado_Conexion));
            return(atoi(Estado_Conexion));
        }
    break;
    case 2:   // (MODIFICACION)

        fprintf(fichsal,"modificacion\n");
        retorno = tratar_fich("NPTSIOC", MAYORIGUAL, &reg_sioc, clave_sioc);
        if(retorno)
        {
            fprintLog("Retorna 99\n");
            return(99);
        }
        else
        {
            strcpy(Estado_Conexion, estado);
            strncpy(reg_sioc.scestado, Estado_Conexion, 2);
            if(opcenvoff!=2)   // la opcion opcenvoff=2 es un testeo del estado de la conexion, y no es una opcion de la situacion
                reg_sioc.scenvoff[0]=opcenvoff;
            retorno = tratar_fich("NPTSIOC", REESCRIBIR, &reg_sioc, NULL);
            if(retorno)
            {
                fprintLog("Retorna 99\n"); 
                return(99);

            }
            else
            { 
                fprintLog("Retorna %d\n", atoi(Estado_Conexion)); 
                return(atoi(Estado_Conexion));   
            }
        }
    }     // de switch  
}
