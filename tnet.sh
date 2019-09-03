while true; do 
  echo "Checking for ${LDK} `date`"

  ping 169.254.1.1  -c 1 -W 0 -i 200 >/dev/null 2>&1
  RC=$?

  if [[ ${RC} -eq 0 ]]; then
  	telnet ${LDK}
  	sleep 1
  fi
done

