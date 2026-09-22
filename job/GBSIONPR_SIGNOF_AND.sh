echo "-------------------------------------"
echo "Iniciando GBSIONPR SIGN-OFF AND..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfabank/sisma/obj/GBSIONPR SNF2002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR SIGN-OFF AND"
echo "------------------------------------------"
exit $status
