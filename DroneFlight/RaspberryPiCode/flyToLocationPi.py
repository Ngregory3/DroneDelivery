from djitellopy import tello
import sys

def generate_func_list(x, y, z) -> list:
    move_stack = []
    #For x:
    if x > 0:
        max_func = x // 500
        leftover = x % 500
        for f in range(max_func):
            move_stack.append(["r", 500])
        if leftover != 0:
            move_stack.append(["r", leftover])
    elif x < 0:
        max_func = abs(x) // 500
        leftover = abs(x) % 500
        for f in range(max_func):
            move_stack.append(["l", 500])
        if leftover != 0:
            move_stack.append(["l", leftover])
    
    #For y:
    if y > 0:
        max_func = y // 500
        leftover = y % 500
        for f in range(max_func):
            move_stack.append(["f", 500])
        if leftover != 0:
            move_stack.append(["f", leftover])
    if y < 0:
        max_func = abs(y) // 500
        leftover = abs(y) % 500
        for f in range(max_func):
            move_stack.append(["b", 500])
        if leftover != 0:
            move_stack.append(["b", leftover])

    #For z:
    if z > 0:
        max_func = z // 100
        leftover = z % 100
        for f in range(max_func):
            move_stack.append(["u", 100])
        if leftover != 0:
            move_stack.append(["u", leftover])
    if z < 0:
        max_func = abs(z) // 100
        leftover = abs(z) % 100
        for f in range(max_func):
            move_stack.append(["d", 100])
        if leftover != 0:
            move_stack.append(["d", leftover])

    return move_stack


def execute_funcs(drone: tello.Tello, func_list: list) -> bool:
    try:
        while len(func_list) > 0:
            dir, cm = func_list.pop()
            if dir == "r":
                drone.move_right(cm)
            elif dir == "l":
                drone.move_left(cm)
            elif dir == "f":
                drone.move_forward(cm)
            elif dir == "b":
                drone.move_back(cm)
            elif dir == "u":
                drone.move_up(cm)
            elif dir == "d":
                drone.move_down(cm)
        return True
    except Exception:
        return False
    
def flyToLocation(x,y,z):
    drone = tello.Tello()
    drone.connect()

    #Check battery
    battery_percent = drone.get_battery()

    if (battery_percent > 5):
        print("Drone battery {}%".format(battery_percent))
    else:
        print("Not performing flight, drone battery at less than 5%")
        return

    move_stack = generate_func_list(x,y,z)
    drone.takeoff()
    success = execute_funcs(drone, move_stack)
    if not success:
        print("Failed to execute path")
    else:
        drone.land()
        print("Made it to desired location")


flyToLocation(sys.argv[1], sys.argv[2], sys.argv[3])
