#!/bin/bash

export SLOTS=/sys/devices/bone_capemgr.9/slots
export PINS=/sys/kernel/debug/pinctrl/44e10800.pinmux/pins
export PINMUX=/sys/kernel/debug/pinctrl/44e10800.pinmux/pinmux-pins

# check if root
if [ $(id -u) -ne 0 ]
then
   echo "must be root"
   exit 1
fi

# check if overlay already applied
grep -q arm_control $SLOTS
if [ $? -eq 1 ]
then # not applied
   echo "applying overlay ..."
   [ -r "/lib/firmware/arm_control-1-00A0.dtbo" ] || exit 2
   echo arm_control-1 > $SLOTS
   echo "GPIO used :"
   egrep "pinmux_motor" $PINMUX
else
   echo "overlay already applied ..."
fi

./server 1337
