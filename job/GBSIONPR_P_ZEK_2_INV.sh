echo "---------------------------------------------------"
echo "Iniciando GBSIONPR PET ZEK 2 o solicitud VCC INV..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sf/sisma/obj/GBSIONPR P163002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZEK 2 o solicitud VCC INV"
echo "-------------------------------------------------"
exit $status
