## elegimos si la variable graba se activa o no 
export graba="SI" 
##export graba="NO"
 
## ejecutamos primero sisma y luego arrancamos pmanjeTux
cd /${SF_HOME}/sisma/exec
./arranqueA
./pmanejTux 5524 & 
