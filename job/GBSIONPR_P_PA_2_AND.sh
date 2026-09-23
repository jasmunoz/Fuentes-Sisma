echo "--------------------------------------------------"
echo "Iniciando GBSIONPR PET ZPK 2 y ZAK 2 AND..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfabank/sisma/obj/GBSIONPR P042002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZPK 2 y ZAK 2 AND"
echo "------------------------------------------------"
exit $status
