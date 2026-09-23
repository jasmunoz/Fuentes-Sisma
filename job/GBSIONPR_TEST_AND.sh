echo "-------------------------------------"
echo "Iniciando GBSIONPR TEST AND..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfabank/sisma/obj/GBSIONPR TST2002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR TEST AND"
echo "------------------------------------------"
exit $status
