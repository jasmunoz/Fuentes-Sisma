################################
# directorio
################################

echo "PWD" `pwd`
DIR=""
export DIR
DIR=`pwd | cut -c 1-15`
echo "DIR1" $DIR
export DIR

if [[ $DIR = "/sf/preprod/sf" ]]
then  echo "PREPROD"
      DIR="/sf/preprod/sf"
       export preprod=2
else 
      echo "SF"
      DIR="/sf"
      export preprod=1
fi 
echo "preprod" $preprod
echo "DIR2" $DIR

MP_SISMA=$DIR/sisma
export MP_SISMA

echo "MP_SISMA" $MP_SISMA

 cc -m64 -c -O -I$MP_SISMA/includes $MP_SISMA/src/pmanejTux.c  -o $MP_SISMA/obj/pmanejTux.o -I/opt/tuxedo/tuxedo12.2.2.0.0/include


 buildclient -o $MP_SISMA/exec/pmanejTux -f $MP_SISMA/obj/pmanejTux.o  -f $MP_SISMA/obj/libsisma.a

