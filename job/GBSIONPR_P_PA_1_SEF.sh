echo "--------------------------------------------------"
echo "Iniciando GBSIONPR PET ZPK 1 y ZAK 1 SEF..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sf/sisma/obj/GBSIONPR P034002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZPK 1 y ZAK 1 SEF"
echo "------------------------------------------------"
exit $status
