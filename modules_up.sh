#!/bin/sh

#---------------------------------------------
# Script used for upload the PWM and GPIO pins
#---------------------------------------------

#Carrega módulos de PWM
cd /sys/devices/bone_capemgr.9
echo am33xx_pwm > slots
echo bone_pwm_P9_21 > slots
echo bone_pwm_P9_14 > slots
echo bone_pwm_P8_19 > slots
echo bone_pwm_P9_42 > slots
#echo bone_pwm_P8_13 > slots
#echo bone_pwm_P9_22 > slots
#echo bone_pwm_P9_16 > slots
echo "Carregou PWM Pins!"

#Habilita pinos GPIO como saída
cd /sys/class/gpio
echo 30 > /sys/class/gpio/export
echo 60 > /sys/class/gpio/export
echo 66 > /sys/class/gpio/export
echo 67 > /sys/class/gpio/export
sleep 1
echo "out" > /sys/class/gpio30/direction
echo "out" > /sys/class/gpio60/direction
echo "out" > /sys/class/gpio66/direction
echo "out" > /sys/class/gpio67/direction
echo "Carregou GPIO Pins!"
