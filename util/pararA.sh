cd /${SF_HOME}/sisma/exec
./paradaA 

ps -fe | grep "pmanejTux 5524" | grep -v "grep" | while read usuario pid resto
do
  echo $usuario cat $pid cat $resto
  kill -9 $pid
done
cd /${SF_HOME}/sisma/exec
./sismaKill.sh

