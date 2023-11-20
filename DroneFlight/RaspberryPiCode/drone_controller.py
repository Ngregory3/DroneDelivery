import threading
import socket
import time
import math
import paramiko

#The program to control all drone actions
#Need to add failsafes for losing connection to Raspberry Pi, losing connectino to Xiao, and Raspberry Pi losing connection to Tello Wifi

#All mathematical functions
def returnRelativeGoal(goalLong, goalLat, goalAlt, currLong, currLat, currAlt):
    #Longitude and Latitude needs to be given in decimal degrees
    KM_TO_CM = 100000
    M_TO_CM = 100
    LAT_TO_KM = 111.32
    EARTH_CIRCUMFERENCE_KM = 40075

    lat_cm = (goalLat - currLat) * LAT_TO_KM * KM_TO_CM
    
    LONG_TO_KM = EARTH_CIRCUMFERENCE_KM * math.cos(goalLat - currLat) / 360
    long_cm = (goalLong - currLong) * LONG_TO_KM * KM_TO_CM

    alt_cm = (goalAlt - currAlt) * M_TO_CM

    return [lat_cm, long_cm, alt_cm]

def convertLatToDecimal(latString, north: bool):
    degree = int(latString[0:2])
    minute = int(latString[2:4])
    second = int(latString[5:])

    degrees = degree + minute/60 + second/3600
    if not north:
        degrees *= -1
    
    return degrees

def convertLongToDecimal(latString, east: bool):
    degree = int(latString[0:2])
    minute = int(latString[2:4])
    second = int(latString[5:])

    degrees = degree + minute/60 + second/3600
    if not east:
        degrees *= -1
    
    return degrees

def convertAltToDecimal(altString):
    return float(altString)


#Create threads for parse GPS
def parseGPS(active: threading.Event, curr_location: dict, location_lock: threading.Lock):
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

    # Close the socket
    s.close()

def main():
    #Event to control parse thread
    print("Beginning program")
    activeEvent = threading.Event()
    activeEvent.set()

    #Current location dictionary of drone and lock
    curr_location = {}
    curr_location["fix"] = 0
    location_mutex = threading.Lock()

    #Create and start parsing thread to get current GPS data
    parse = threading.Thread(target=parseGPS, args=(activeEvent, curr_location, location_mutex))

    #Create SSH connection to raspberry pi
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    pi_server = "192.100.1.1"
    pi_username = "pi"
    pi_password = "raspberry2"
    ssh.connect(pi_server, username=pi_username, password=pi_password)

    print("Setup complete")
    
    while True:
        #Wait for user input
        print("GPS coordinate format: DDMM.SSSSS (D=degrees M=minutes S=seconds)")
        user_input = input("Enter GPS coordinates in the form Lat, Long, Alt\n")
        if (user_input == "end"):
            break
        gps_coords = user_input.split(',')
        print("Inputted goal destination: (lat, long, alt)")
        print(gps_coords[0])
        print(gps_coords[1])
        print(gps_coords[2])

        if len(gps_coords) != 3:
            print("Invalid input, there must be 3 values given separated by commas")
            continue
        
        print("Valid user input given")
        success = True

        '''
        print("Attempting to get coordinates from GPS")
        parse.start()
        threading.Event.wait(10) #Pause to allow us to get GPS data
        try_count = 5
        success = False
        while try_count > 0:
            with location_mutex:
                fix = curr_location["fix"]
            if fix != 0:
                curr_lat = curr_location["lattitude"]
                curr_long = curr_location["longitude"]
                curr_alt = curr_location["altitude"]
                success = True
                break
            else:
                threading.Event.wait(10)
            try_count -= 1

        '''
        print("Getting fake current location")
        #Testing current location
        curr_lat = "0000.0000"
        curr_long = "0000.0000"
        curr_alt = "0000.0000"
        success = True
        north = True
        east = True
        
        
        if success:
            print("Current location data: (lat, long, alt)")
            print(curr_lat)
            print(curr_long)
            print(curr_alt)
            
            relativeGoal = [1000, 0, 50]#returnRelativeGoal(convertLongToDecimal(gps_coords[1], east), convertLatToDecimal(gps_coords[0], north), convertAltToDecimal(gps_coords[2]), convertLongToDecimal(curr_long, east), convertLatToDecimal(curr_lat, north), convertAltToDecimal(curr_alt))
            print("Relative Goal: (x,y,z)")
            print(relativeGoal[0])
            print(relativeGoal[1])
            print(relativeGoal[2])
            
            #Send information to Raspberry Pi through ssh
            test_command = "python ~/Senior_Design/djitelloTest.py"
            flight_command = "python ~/Senior_Design/flyToLocationPi.py {} {} {}".format(relativeGoal[0], relativeGoal[1], relativeGoal[2])
            command = flight_command
            print("Command sent to py: " + command)

            stdin, stdout, stderr = ssh.exec_command(command)
            stdout.channel.recv_exit_status()  #Blocks code and waits for exit status, meaning program as finished
            print("Output from raspberry pi:")
            for line in iter(stdout.readline, ""):
                print(line, end="")
            stdin.close()
            stdout.close()
            stderr.close()
            print("Done executing ssh function")

        else:
            print("Failed to get current GPS coordinates, waiting for new input")
            activeEvent.clear()
            parse.join()
            activeEvent.set()
            continue
            
        print("Finished loop")
            
        
main()
            
        



