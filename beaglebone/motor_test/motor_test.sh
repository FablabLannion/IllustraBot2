#/bin/bash
# motor_test

DIR=1
SLEEP=.00025
if [ ${#@} -lt 1 ]
then
   echo "usage: $0 <steps> [dir] [sleep]"
   echo " <steps>: number"
   echo " [dir] : 0, 1 ($DIR)"
   echo " [sleep] : between each step ($SLEEP)"
   exit 1
fi

STEPS=$1
[ ! -z "$2" ] && DIR=$2
[ ! -z "$3" ] && SLEEP=$3

# init gpio
echo 44 > /sys/class/gpio/export
echo 45 > /sys/class/gpio/export

# dir
echo out > /sys/class/gpio/gpio44/direction
echo $DIR > /sys/class/gpio/gpio44/value

# steps
echo out > /sys/class/gpio/gpio45/direction

# rotate
c=0
while [ $c -lt $STEPS ]
do 
   echo 1 > /sys/class/gpio/gpio45/value 
   echo 0 > /sys/class/gpio/gpio45/value
   sleep $SLEEP
   c=$(($c + 1))
done

echo 44 > /sys/class/gpio/unexport
echo 45 > /sys/class/gpio/unexport
