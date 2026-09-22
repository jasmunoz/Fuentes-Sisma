echo "---------------------------------------------------"
echo "Iniciando GBSIONPR PET ZEK 2 o solicitud VCC WPB..." 
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfwpb/sisma/obj/GBSIONPR P161002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZEK 2 o solicitud VCC WPB"
echo "-------------------------------------------------"
exit $status
