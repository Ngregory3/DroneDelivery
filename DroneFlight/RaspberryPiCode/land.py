from djitellopy import tello
    
def land():
    drone = tello.Tello()
    drone.connect()

    drone.land()


land()
