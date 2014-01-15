#/bin/bash
# motor_test

cd /sys/class/gpio
echo 44 > export
echo 45 > export

# dir
cd gpio44
echo out > direction
echo 1 > value

# steps
cd ../gpio45
echo out > direction

# rotate
while [ 1 ]
do 
   echo 1 > value 
   echo 0 > value
   sleep .001
done
