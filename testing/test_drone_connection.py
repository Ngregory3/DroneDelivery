from djitellopy import tello

def get_battery() -> str:
    #Create var to control drone
    drone = tello.Tello()
    drone.connect()

    #Check for battery life
    battery_percent = drone.get_battery()

    message = "Drone battery {}%".format(battery_percent)
    print(message)
    return message

get_battery()
    