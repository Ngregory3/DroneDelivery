from djitellopy import tello
    
def takeoff():
    drone = tello.Tello()
    drone.connect()

    #Check battery
    battery_percent = drone.get_battery()

    if (battery_percent > 5):
        print("Drone battery {}%".format(battery_percent))
    else:
        print("Not performing flight, drone battery at less than 5%")
        return

    drone.takeoff()


takeoff()
