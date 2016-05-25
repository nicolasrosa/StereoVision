#! /bin/bash
ifconfig wlan0 up
iwconfig wlan0 essid "Union Dearborn WiFi"
dhclient wlan0
ping 8.8.8.8
