echo "---------------------------------------------------"
echo "Iniciando GBSIONPR PET ZAK 2 o solicitud VCC AND..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfabank/sisma/obj/GBSIONPR P142002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZAK 2 o solicitud VCC AND"
echo "-------------------------------------------------"
exit $status
