cd /sf/sisma/exec
parada

ps -fe | grep pmanejTux | grep -v "grep" | while read usuario pid resto
do
  echo $usuario cat $pid cat $resto
  kill -9 $pid
done

