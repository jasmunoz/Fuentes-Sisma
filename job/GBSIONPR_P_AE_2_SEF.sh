echo "--------------------------------------------------"
echo "Iniciando GBSIONPR PET ZAK 2 y ZEK 2 SEF..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sf/sisma/obj/GBSIONPR P084002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZAK 2 y ZEK 2 SEF"
echo "------------------------------------------------"
exit $status
