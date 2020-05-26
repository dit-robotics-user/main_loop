#!/usr/bin/env python
"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action_demo import *

# name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup, iswait
def setting(mode):
    if mode is 1:
        c1 = Action('goto',   [], [], (1650, 865), 3, 270, 150, 1, 15, 11, False, True)

        c2 = Action("hand1_out_get1",     [1], [], (500,2000), 3, 180, 150, 1, 0, 1, True, True)
        c3 = Action("hand1_out_get2",    [2], [], (300,2000), 3, 180, 150, 1, 0, 1, True, True)
        c4 = Action("hand1_out_get3",   [3], [], (950,2200), 3, 0, 150, 1, 0, 1, True, True)
        c5 = Action("hand1_release",  [4], [], (300,2000), 3, 180, 150, 1, 0, 1, True, True)
        c6 = Action("hand1_inside",    [8], [], (500,2000), 3, 180, 150, 1, 0, 1, True, True)
        c7 = Action("hand1_origin",   [9], [], (200, 270), 3, 90, 150, 1, 0, 1, True, True)

        c8 = Action("hand2_out_get1",     [1], [], (500,2000), 3, 180, 150, 1, 0, 2, True, True)
        c9 = Action("hand2_out_get2",    [2], [], (300,2000), 3, 180, 150, 1, 0, 2, True, True)
        c10 = Action("hand2_out_get3",   [3], [], (950,2200), 3, 0, 150, 1, 0, 2, True, True)
        c11 = Action("hand2_release",  [4], [], (300,2000), 3, 180, 150, 1, 0, 2, True, True)
        c12 = Action("hand2_inside",    [8], [], (500,2000), 3, 180, 150, 1, 0, 2, True, True)
        c13 = Action("hand2_origin",   [9], [], (200, 270), 3, 90, 150, 1, 0, 2, True, True)

        c14 = Action("hand3_out_get1",     [1], [], (500,2000), 3, 180, 150, 1, 0, 3, True, True)
        c15 = Action("hand3_out_get2",    [2], [], (300,2000), 3, 180, 150, 1, 0, 3, True, True)
        c16 = Action("hand3_out_get3",   [3], [], (950,2200), 3, 0, 150, 1, 0, 3, True, True)
        c17 = Action("hand3_release",  [4], [], (300,2000), 3, 180, 150, 1, 0, 3, True, True)
        c18 = Action("hand3_inside",    [8], [], (500,2000), 3, 180, 150, 1, 0, 3, True, True)
        c19 = Action("hand3_origin",   [9], [], (200, 270), 3, 90, 150, 1, 0, 3, True, True)

        c20 = Action("hand4_out_get1",     [1], [], (500,2000), 3, 180, 150, 1, 0, 4, True, True)
        c21 = Action("hand4_out_get2",    [2], [], (300,2000), 3, 180, 150, 1, 0, 4, True, True)
        c22 = Action("hand4_out_get3",   [3], [], (950,2200), 3, 0, 150, 1, 0, 4, True, True)
        c23 = Action("hand4_release",  [4], [], (300,2000), 3, 180, 150, 1, 0, 4, True, True)
        c24 = Action("hand4_inside",    [8], [], (500,2000), 3, 180, 150, 1, 0, 4, True, True)
        c25 = Action("hand4_origin",   [9], [], (200, 270), 3, 90, 150, 1, 0, 4, True, True)
        
        c26 = Action("suck1_on",  [5], [], (500,2000), 3, 180, 150, 1, 0, 5, True, True)
        c27 = Action("suck1_off",  [6], [], (950,2200), 3, 0, 150, 1, 0, 5, True, True)

        c28 = Action("suck2_on",  [5], [], (500,2000), 3, 180, 150, 1, 0, 6, True, True)
        c29 = Action("suck2_off",  [6], [], (950,2200), 3, 0, 150, 1, 0, 6, True, True)

        c30 = Action("suck3_on",  [5], [], (500,2000), 3, 180, 150, 1, 0, 7, True, True)
        c31 = Action("suck3_off",  [6], [], (950,2200), 3, 0, 150, 1, 0, 7, True, True)

        c32 = Action("suck4_on",  [5], [], (500,2000), 3, 180, 150, 1, 0, 8, True, True)
        c33 = Action("suck4_off",  [6], [], (950,2200), 3, 0, 150, 1, 0, 8, True, True)

        c34 = Action("stepper_on",  [5], [], (400,2200), 3, 0, 150, 1, 0, 9, True, True)
        c35 = Action("stepper_off", [6], [], (300,2000), 3, 180, 150, 1, 0, 9, True, True)

        c36 = Action("flag_on", [6], [], (300,2000), 3, 180, 150, 1, 0, 10, True, True)


        demo_path = [c2, c6, c8, c3, c9, c5, c4, c10, c11, c12, c13]

    return demo_path
