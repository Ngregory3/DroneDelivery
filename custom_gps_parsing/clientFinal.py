import socket

# Arduino's IP and Port
SERVER_IP = '0.0.0.0'  # This is the IP of your Arduino.
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

s.bind((SERVER_IP, SERVER_PORT))

# Connect to the Arduino
#s.connect((SERVER_IP, SERVER_PORT))

# Send a message to the Arduino (Optional)
#s.sendall(b'Hello, Arduino!')

s.listen()
client_socket, client_address = s.accept()

while True:
    data = client_socket.recv(1024).decode('utf-8')

    #time.sleep(1)

    curr = str(data)
    print(curr)
    if (curr == "start:"): continue
    #if (len(curr) < 7): continue

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
