import socket

TCP_IP = '100.70.2.244' #Local Host
TCP_PORT = 3007            #Listening Port
BUFFER_SIZE = 4096

SERVER_IP = '100.70.25.100'  #Arduino IP
SERVER_PORT = 23            #Server Port

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP,TCP_PORT))
s.listen(1)

(conn,addr) = s.accept()
print('Connection address: ',addr)
while True:
    print ('Sending message')
    byteSent = s.sendto('ECHO'.encode(),(SERVER_IP,SERVER_PORT))
    print (byteSent)
    print ('Waiting for response')
    data,addr = s.recvfrom(BUFFER_SIZE)
    if not data: break
    print ('received data: ',data.decode())
    print ('data recieved from: ',addr)

s.close()
