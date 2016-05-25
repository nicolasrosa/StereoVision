# Sharing internet using Network-Over-USB in BeagleBone Black
# Configures the Network at Target (BBB)
ifconfig usb0 192.168.7.2
route add default gw 192.168.7.1
