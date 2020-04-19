#!/usr/bin/env python
"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action import *

# name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup, iswait
def setting(mode):
    if mode is 1:
        c1 = Action('goto',         [], [], (1650, 865), 3, 270, 150, 1, 15, 5, False, True)
        c2 = Action("wrist_up1",     [1], [], (500,2000), 3, 180, 150, 1, 0, 1, True, True)
        c14 = Action("wrist_up2",     [1], [], (500,2000), 3, 0, 150, 1, 0, 1, True, True)
        c3 = Action("wrist_mid1",    [2], [], (300,2000), 3, 180, 150, 1, 0, 1, True, True)
        c4 = Action("wrist_down1",   [3], [], (950,2200), 3, 0, 150, 1, 0, 1, True, True)
        c12 = Action("wrist_down2",   [3], [], (400,2200), 3, 0, 150, 1, 0, 1, True, True)
        c5 = Action("wrist_setup1",  [4], [], (300,2000), 3, 180, 150, 1, 0, 1, True, True)
        c11 = Action("wrist_setup2",  [4], [], (950,2200), 3, 0, 150, 1, 0, 1, True, True)
        c6 = Action("hand_open",    [2], [], (500,2000), 3, 180, 150, 1, 0, 2, True, True)
        c7 = Action("hand_close",   [1], [], (200, 270), 3, 90, 150, 1, 0, 2, True, True)
        c8 = Action("finger_open",  ['1', '2', '3', '4', '5'], [], (500,2000), 3, 180, 150, 1, 0, 3, True, True)
        c10 = Action("finger_open_G",  ['1', '3', '5'], [], (950,2200), 3, 0, 150, 1, 0, 3, True, True)
        c13 = Action("finger_open_R",  ['2', '4'], [], (400,2200), 3, 0, 150, 1, 0, 3, True, True)
        c9 = Action("finger_close", ['1', '2', '3', '4', '5'], [], (300,2000), 3, 180, 150, 1, 0, 4, True, True)


        demo_path = [c2, c6, c8, c3, c9, c5, c4, c10, c11, c12, c13]

    return demo_path




