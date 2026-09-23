cd /${SF_HOME}/sisma/exec
./paradaW

ps -fe | grep "pmanejTux 5525" | grep -v "grep" | while read usuario pid resto
do
  echo $usuario cat $pid cat $resto
  kill -9 $pid
done
cd /${SF_HOME}/sisma/exec
./sismaKill.sh 

