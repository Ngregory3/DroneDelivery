from djitellopy import tello
import re

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

        

def main():
    print("running...")
    drone = tello.Tello()
    drone.connect()
    flying = True
    input_pattern = "^\s*-?\d+\s*,\s*-?\d+\s*,\s*-?\d+\s*$"

    while flying:
        #Check battery and temperature
        battery_percent = drone.get_battery()

        if (battery_percent > 5):
            print("Drone battery {}%".format(battery_percent))
        else:
            print("Terminating script, battery at {}%".format(battery_percent))
            flying = False
            break
            
        temperature = drone.get_temperature()

        if (temperature < 37):
            print("Drone temperature: {} C".format(temperature))
        else:
            print("Drone temperature too hot({}C), cool off the drone".format(temperature))
            flying = False
            break

        #Prompt user for input
        valid_point = False
        inp = input('Input the final relative position in "x,y,z"\n')
        inp = inp.strip()
        if inp == "end":
            flying = False
            break
        elif re.search(input_pattern, inp) is not None:
            input_list = inp.split(',')
            if len(input_list) == 3:
                x = int(input_list[0])
                y = int(input_list[1])
                z = int(input_list[2])
                valid_point = True
        else:
            print("error parsing")
        
        #If valid input, get functions to execute
        if valid_point:
            move_stack = generate_func_list(x,y,z)
            print(move_stack)
            success = execute_funcs(move_stack)
            if not success:
                print("Failed to execute path")
                flying = False
        
    drone.land()
    print("Finished, drone landed")

main()
