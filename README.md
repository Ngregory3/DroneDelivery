# DroneDelivery
Code for the implementation of the Drone Delivery System (Senior Design Fall 2023).

As the demand for delivery services increase, autonomous delivery by air proves to be fast in delivery time, not subject to traffic congestion, and also saves costs, as a few drone operators can manage a fleet of drones. Although use of drones still faces regulatory issues, its widespread adoption seems inevitable. Therefore, the technological and logistical aspects of drone delivery should still be explored.​

The main code for the project is in the **Drone Flight** folder. This includes:
1. Code to be installed on the Arduino Xiao to send GPS coordinates
2. Code to be installed on the Raspberry Pi 4 for calling over SSH
3. Code to run on the central server

Many testing scripts can be found within the **testing** folder. These are used to verify the connections between all parts of the infrastructure are working correctly.

The **droneapp** folder contains version 1 of the application that would be used to interact with the system in order to order items via the drones.
