"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action_demo_2 import *

# name, preconditions, effects, child_action, position, degree, speed, cost, mode, speed_mode_radius, type_number, cup_number, wait
def setting(mode, cup_color):
    demo_path = []
    if mode is 1:
        a1 = Action('hand1_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a2 = Action('hand1_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a3 = Action('hand1_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a4 = Action('hand1_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a5 = Action('hand1_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a6 = Action('hand1_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)

        a7 = Action('hand2_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a8 = Action('hand2_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a9 = Action('hand2_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a10 = Action('hand2_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a11 = Action('hand2_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a12 = Action('hand2_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)

        a13 = Action('hand3_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a14 = Action('hand3_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a15 = Action('hand3_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a16 = Action('hand3_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a17 = Action('hand3_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a18 = Action('hand3_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)

        a19 = Action('hand4_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a20 = Action('hand4_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a21 = Action('hand4_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a22 = Action('hand4_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a23 = Action('hand4_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a24 = Action('hand4_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)

        a25 = Action('suck1_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
        a26 = Action('suck1_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)

        a27 = Action('suck2_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 6, 0, True)
        a28 = Action('suck2_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 6, 0, True)

        a29 = Action('suck3_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
        a30 = Action('suck3_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)

        a31 = Action('suck4_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 8, 0, True)
        a32 = Action('suck4_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 8, 0, True)

        a33 = Action('stepper_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
        a34 = Action('stepper_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)

        a35 = Action('flag_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 10, 0, True)

        a36 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 11, 0, True)

        #   hand 1 get cup
        c1 = Action('hand1_get_cup', ['n'], ['c'], [a1, a34, a25, a33, a5, a34, a26, a33, a6], (400, 400), 90, 150, 3, 1, 15, 1, 4, True)


        #   hand 2 get cup
        c2 = Action('hand2_get_cup', ['h', 'e'], ['t'], [a7, a34, a27, a33, a11, a34, a28, a33, a12], (700, 800), 85, 150, 3, 1, 15, 4, 4, True)


        #   hand 3 get cup
        c3 = Action('hand3_get_cup', ['n'], ['c'], [a13, a34, a29, a33, a17, a34, a30, a33, a18], (750, 350), 90, 150, 3, 1, 15, 1, 4, True)


        #   hand 4 get cup
        c4 = Action('hand4_get_cup', ['l'], ['d'], [a19, a34, a31, a33, a23, a34, a32, a33, a24], (530, 300), 85, 150, 3, 1, 15, 4, 4, True)

        #   hand 1 release
        c5 = Action('hand1_release', ['h'], ['p'], [a5, a34, a25, a33, a4, a34, a26, a33, a6], (480, 500), 85, 150, 3, 1, 15, 4, 4, True)


        #   hand 2 release
        c6 = Action('hand2_release', ['h'], ['p'], [a11, a34, a27, a33, a10, a34, a28, a33, a12], (480, 500), 85, 150, 3, 1, 15, 4, 4, True)

        #   hand 3 release
        c7 = Action('hand3_release', ['h'], ['p'], [a17, a34, a29, a33, a16, a34, a30, a33, a18], (480, 500), 85, 150, 3, 1, 15, 4, 4, True)

        #   hand 4 release
        c8 = Action('hand4_release', ['h'], ['p'], [a23, a34, a31, a33, a22, a34, a32, a33, a24], (480, 500), 85, 150, 3, 1, 15, 4, 4, True)

        #  go home
        c9 = Action('go_home', [], ['at_home'], [a36], (300, 700), 85, 150, 3, 1, 15, 4, 4, True)
        c10 = Action('raise_flag', [], ['at_home'], [a36], (300, 700), 85, 150, 3, 1, 15, 4, 4, True)

        child_action_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15]
        action_path = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10]
        go_home_path = [c18, c19]

        for child in child_action_list:
            green = 5
            red = 3
            if child.name == 'finger_open_green2':
                for i in range(len(cup_color)):
                    if cup_color[i] == green:
                        child.effects.append(i+1)
            elif child.name == 'finger_open_red2':
                for i in range(len(cup_color)):
                    if cup_color[i] == red:
                        child.effects.append(i+1)

    return action_path, go_home_path
