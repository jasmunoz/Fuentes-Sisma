/* -------------------- test for zeroes  ----------------- */
/*  devuelve 0               = cadena distinta de zeroes   */
/*  devuelve  mayor que 0 si = cadena igual a zeroes       */
/*---------------------------------------------------------*/
int zeroes (char *c)
{
    while (*c == '0') 
        c++;  
    return (!*c);
}

/* -------------------- test for nzeroes  ---------------- */
/*     devuelve :   0 .- n primeros caracteres de cadena   */
/*                       distintos de zeroes               */
/*                  1 .- n primeros caracteres de cadena   */
/*                       iguales a zeroes                  */
/*---------------------------------------------------------*/
int nzeroes (char *c,int n)
{
    while ((*c == '0') && (n>0))
    {   
        c++;
        n--;
    };
    if (n>0)
       return(0); 
    else
       return(1);
}

