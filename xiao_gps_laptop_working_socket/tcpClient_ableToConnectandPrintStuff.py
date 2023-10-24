import socket

# Arduino's IP and Port
SERVER_IP = '100.70.28.117'  # This is the IP of your Arduino.
#Run clientServer and read:
# Connecting
# WL_IDLE_STATUS
# WL_DISCONNECTED
# WL_CONNECTED
# Connected to the WiFi network
# Local ESP32 IP: 100.70.25.10 <-this is the SERVER_IP

SERVER_PORT = 25

# Create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the Arduino
s.connect((SERVER_IP, SERVER_PORT))

# Send a message to the Arduino (Optional)
s.sendall(b'Hello, Arduino!')

# Receive a response from the Arduino
# data = s.recv(1024)
# print('Received', repr(data))

# data = s.recv(1024)
# print('Received', repr(data))

while True:
    data = s.recv(1024)
    if data:
        print('Received', repr(data))
        received_data = repr(data).split()
        fix = received_data[0]
        latitude = received_data[1][:-2]
        longitude = received_data[2][:-2]


'''
BUFFER_SIZE = 4096
while True:
    print ('Sending message')
    byteSent = s.sendto('ECHO'.encode(),(SERVER_IP,SERVER_PORT))
    print (byteSent)
    print ('Waiting for response')
    data,addr = s.recvfrom(BUFFER_SIZE)
    if not data: break
    print ('received data: ',data.decode())
    print ('data recieved from: ',addr)
'''

# Close the socket
s.close()


'''
import socket
import threading

def receive_data(sock):
    while True:
        data = sock.recv(1024)
        if not data:
            break
        print('Received', repr(data))

SERVER_IP = '100.70.25.100'
SERVER_PORT = 23

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((SERVER_IP, SERVER_PORT))

# Start a thread to continuously receive data
threading.Thread(target=receive_data, args=(s,)).start()

# This is just for the demonstration. In a real-world application, you would have
# some other tasks or logic here. For this example, we're just sending a message.
s.sendall(b'Hello, Arduino!')

# Here, we'll just keep the application running. In a real-world scenario, you'd have
# more logic or an event loop, etc.
try:
    while True:
        pass
except KeyboardInterrupt:
    s.close()
'''
