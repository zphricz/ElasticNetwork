                     
                         2D ELASTIC NETWORK SIM

Overview
--------

This program simulates nodes in an elastic network on a 2D plane. It uses
Euler's method to approximate the movement of the bodies. To run the program,
simply run:

    ./elastic

This will run the program in full screen. You can specify the resolution to
render at with command line arguments:

    ./elastic [Screen_x Screen_y]

You can interact with the simulation by pressing keys on your keyboard:

    1: Halve the timestep
    2: Double the timestep
    3: Turn scaling off
    4: Turn scaling on (will shrink window)
    5: Turn scaling on (will not shrink window)
    6: Make the center of the screen the average position of the bodies
    7: Make the center of the screen the average of the positions of the
       outermost nodes
    8: Decrease the dampening amount
    9: Incrase the dampening amount
    Shift: Switch selected node
    Enter: Toggle between simulating the program and not
    Escape: Stop the program
    Up: Move selected node up
    Down: Move selected node down
    Left: Move selected node left
    Right: Move selected node right

To close the program, press the escape key.

Authors
-------

Zack Hricz

