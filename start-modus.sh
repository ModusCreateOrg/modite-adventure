cd /home/retrofw/test
#pid=`ps | grep gmenu2x | grep -v grep | awk '{print $1}'`
#kill -9 $pid
gdb -batch -ex run -ex bt --args ./modus*
reboot