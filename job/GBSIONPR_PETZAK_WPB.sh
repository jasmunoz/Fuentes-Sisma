echo "-------------------------------------"
echo "Iniciando GBSIONPR PETZAK WPB..." 
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfwpb/sisma/obj/GBSIONPR P131002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PETZAK WPB"
echo "------------------------------------------"
exit $status
