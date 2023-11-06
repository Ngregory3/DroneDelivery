import socket
import time

# Arduino's IP and Port
SERVER_IP = '100.70.16.1'  # This is the IP of your Arduino.
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

while True:
    #data = repr(s.recv(1024))
    data = s.recv(1024).decode('utf-8')
    #print(data)

    #time.sleep(1)

    curr = str(data)
    #if (curr == "start:"): continue
    if (len(curr) < 7): continue

    try:
        curr_list = curr.split()
        myDict = {}

        myDict["fix"] = curr_list[0]
        myDict["latitude"] = curr_list[1]
        myDict["longitude"] = curr_list[2]
        myDict["altitude"] = curr_list[3]

        if (len(myDict["fix"]) > 1): continue
        
        print(myDict)
        
    except Exception as e:
        print("Package has error, keep going.")
        continue
