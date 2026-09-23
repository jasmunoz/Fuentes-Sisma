cd /sf/sisma/job
/sf/sisma/job/GBSIONPR_SIGNOF.sh

sleep 3

cd /sf/sisma/exec
parada

ps -fe | grep pmanejTux | grep -v "grep" | while read usuario pid resto
do
  echo $usuario cat $pid cat $resto
  kill -9 $pid
done

