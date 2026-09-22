echo "--------------------------------------------------"
echo "Iniciando GBSIONPR PET ZPK 2 y ZEK 2 INV..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sf/sisma/obj/GBSIONPR P063002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZPK 2 y ZEK 2 INV"
echo "------------------------------------------------"
exit $status
