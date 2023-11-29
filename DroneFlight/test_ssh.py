import paramiko

def main():
    #Event to control parse thread
    print("Beginning program")

    #Create SSH connection to raspberry pi
    ssh = paramiko.SSHClient()
    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    pi_server = "192.100.1.1"
    pi_username = "pi"
    pi_password = "raspberry2"
    ssh.connect(pi_server, username=pi_username, password=pi_password)

    print("Setup complete")
            

    x=int(100.6723933291646)
    y=int(100.5542936241014)
    z=int(0.0)
    #Send information to Raspberry Pi through ssh
    test_command = "python ~/Senior_Design/djitelloTest.py"
    flight_command = "python ~/Senior_Design/flyToLocationPi.py {} {} {}".format(x, y, z)
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
            
        
main()
            
        



