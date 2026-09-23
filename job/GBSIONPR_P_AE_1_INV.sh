echo "--------------------------------------------------"
echo "Iniciando GBSIONPR PET ZAK 1 y ZEK 1 INV..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sf/sisma/obj/GBSIONPR P073002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZAK 1 y ZEK 1 INV"
echo "------------------------------------------------"
exit $status
