echo "--------------------------------------------------"
echo "Iniciando GBSIONPR PET ZPK 1, ZAK 1 y ZEK 1 WPB..." 
date

#Carga de variables
. ${SF_HOME}/mepa/bat/job/vamepash.sh

#---------------------------------------------------------------------------#
/sfwpb/sisma/obj/GBSIONPR P011002
#---------------------------------------------------------------------------#
status=$?
if  [ $status -ne 0 ]
then
    echo "$0 - Error : $status"
    exit $status
fi

date
echo "Finalizado GBSIONPR PET ZPK 1, ZAK 1 y ZEK 1 WPB"
echo "------------------------------------------------"
exit $status
