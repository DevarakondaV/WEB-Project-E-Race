import socket
import time
from machine import Pin,I2C

pin = Pin(5,Pin.OUT)
#########################   I2C BUS CODE    #############################
i2c = I2C(scl=Pin(15),sda=Pin(13),freq=200000) #I2C bus. Com in 15
com_ID = 15 #com ID for i2c bus
print('Communicate With:',i2c.scan());
##########################################################################

#################   POSSIBLE CONNECTION ENDING USER INPUTS  ##############
terminate = 'Terminate'     #Ends communication and closes socket
end = 'end'     #Ends current communication
##########################################################################

#########################   SERVER CODE     ##############################
host = '0.0.0.0'  
port = 80
print (host, port)
##########################################################################

######################### Blocking for Arduino COMM ######################
while (i2c.scan()==[]):
    print('Waiting For Arduino');
##########################################################################

##########################################################################
if (i2c.scan() == [15]):
    pin.value(1)
    time.sleep(5)
    pin.value(0)
    


address = socket.getaddrinfo(host,port)[0][-1]  #Server address
print('Listening on',address)
server_soc = socket.socket()    #Creating Socket Object
server_soc.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
server_soc.bind(address)    #Binding server address to socket
#server_soc.settimeout(60)   #socket times out in 60 seconds
server_soc.listen(1)    #Opening socket to listen for connections. 1 connection at a time


client_addr = 0;
client_soc, client_addr = server_soc.accept()   #Binds clinet_soc to clinet socket, client_addr to connection address
print('Client connected from:',client_addr)

while True:
    #client_soc, client_addr = server_soc.accept()   #Binds clinet_soc to clinet socket, client_addr to connection address
    if client_addr == 0:
	client_soc,client_addr = server_soc.accept()
	print('Client connected from:',client_addr)
    else:
	client_message = client_soc.recv(1024).decode('utf-8');
	if client_message==end:
		client_soc.send('Ending Connection')
		print('Client sent \'end\'....Closing Client Socket')
		client_addr = 0
		client_soc.close()
		#break;
	elif client_message==terminate:
		client_soc.send('Ending Connection')
		print('Client send \'terminate\'...Closing Client Socket')
		client_soc.close()
		break;
	else:
		print('Client sent:',client_message)
		client_soc.send('I recieved your message')
		i2c.writeto(com_ID,client_message)        
print('While loop broken')
server_soc.close()
    
    
    


