from djitellopy import tello
import time

#Max flight seems to be 180cm with a single command

def fly_drone():
    #Create var to control drone
    drone = tello.Tello()

    #Connect and take off
    drone.connect()

    #Check for battery life
    battery_percent = drone.get_battery()

    if (battery_percent > 5):
        print("Drone battery {}%".format(battery_percent))
    else:
        print("Terminating script, battery at {}%".format(battery_percent))
        return 1
    

    drone.takeoff()

    #Move forward
    drone.move("forward",500)
    drone.move_back(30)
    drone.move_left(50)
    drone.move_right(50)
    drone.move_up(20)

    #Get Height
    print("Height:")
    print(drone.get_height())

    #Get Distance
    print("Distance:")
    print(drone.get_distance_tof())

    #drone.flip_back()


    #Land the drone
    drone.land()

    

def get_data():
    #Create var to control drone
    drone = tello.Tello()

    #Connect and take off
    drone.connect()

    #Check for battery life
    battery_percent = drone.get_battery()

    print("Drone battery {}%".format(battery_percent))

def fly_away():
    drone = tello.Tello()
    drone.connect()

    drone.move_forward(20)
    drone.move_forward(20)
    drone.move_forward(20)
    drone.move_forward(20)
    drone.move_forward(20)
    drone.move_forward(20)

    drone.land()


#fly_drone()
get_data()