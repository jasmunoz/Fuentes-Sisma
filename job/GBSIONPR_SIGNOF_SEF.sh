echo "-------------------------------------"
echo "Iniciando GBSIONPR SIGN-OFF SEF..."
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh 

#---------------------------------------------------------------------------#
/sf/sisma/obj/GBSIONPR SNF4002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR SIGN-OFF SEF"
echo "------------------------------------------"
exit $status
