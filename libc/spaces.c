/* -------------------- test for spaces  ----------------- */
/*     devuelve :   0 .- cadena distinta de spaces         */
/*                !=0 .- cadena igual a spaces             */
/*---------------------------------------------------------*/

int
spaces (char *c)
{
    while (*c == ' ')
        c++; 
    return (!*c);  
}

/* -------------------- test for nspaces  ---------------- */
/*     devuelve :   0 .- n primeros caracteres de cadena   */
/*                       distintos de spaces               */
/*                  1 .- n primeros caracteres de cadena   */
/*                       iguales a spaces                  */
/*---------------------------------------------------------*/
int
nspaces (char *c,int n)
{
    while ((*c == ' ') && (n>0))
    {   c++;
        n--;
    };
    if (n>0)
       return(0); 
    else
       return(1);
}

