echo "---------------------------------------------------"
echo "Iniciando GBSIONPR PET ZAK 2 o solicitud VCC WPB..." 
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfwpb/sisma/obj/GBSIONPR P141002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZAK 2 o solicitud VCC WPB"
echo "-------------------------------------------------"
exit $status
