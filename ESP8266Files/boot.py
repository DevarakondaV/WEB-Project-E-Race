# On boot device connects to internet
######defining Connected to Wlan functions:######
import time
from machine import Pin

time.sleep(5)
pin = Pin(5,Pin.OUT)

def do_connect():
    import network #importing network
    wlan = network.WLAN(network.STA_IF) #new WLAN 
    wlan.active(True)
    if wlan.isconnected():
        wlan.disconnect()
    if not wlan.isconnected():
        print('Which Network do you want to connect to?: Home, Ethan, UCLA\n')
        print('Now connecting to network...')
        NID = "NETWORKID"  #MODIFY TO WIFI NETWORK ID
        NPW = "Password"    #MODIFY TO WIFI PASSWORD
        wlan.connect(NID,NPW)
        deadline = time.ticks_add(time.ticks_ms(),30000)
        while not (wlan.isconnected() or not (time.ticks_diff(deadline,time.ticks_ms())>0)):
            print('Connecting...')

    print('network config:',wlan.ifconfig())
    if (wlan.isconnected()):
	pin.value(1)
	time.sleep(5)
	pin.value(0)
        return 1;
    else:
        return 0;
    return;
