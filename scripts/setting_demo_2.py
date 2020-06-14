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
        a1 = Action('hand_close', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a2 = Action('hand_open', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a3 = Action('wrist_mid', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a4 = Action('wrist_down', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a5 = Action('wrist_cup', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a6 = Action('wrist_up', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a7 = Action("finger_close", [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a8 = Action('finger_open', [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a9 = Action('finger_open_green1', [], [2, 4], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a10 = Action('finger_open_red1', [], [1, 3, 5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a11 = Action('finger_open_green2', [], [], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a12 = Action('finger_open_red2', [], [], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a13 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
        a14 = Action('windsock_hand_up', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a15 = Action('windsock_hand_down', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)

        #   get cup 1
        c1 = Action('cup_prepare1', ['no_cups'], ['cup_prepare'], [a6, a2, a8], (1600, 500), 270, 150, 3, 1, 15, 1, 4, True)
        c2 = Action('cup_get1', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (1600, 400), 270, 180, 3, 1, 15, 4, 4, True)
        c3 = Action('cup_hold1', ['cup_get'],  ['get_points', 'have_cups'], [a5], (1600, 600), 0, 150, 3, 1, 15, 1, 4, True)

        #   put first round cup
        c4 = Action('put_red1', ['green1_put'], ['red1_put', 'no_cups', 'second_time'], [a4, a10], (1000, 300), 0, 150, 3,
                    1, 15, 4, 4, True)
        c5 = Action('put_green1', ['have_cups', 'first_time'], ['green1_put'], [a4, a9, a6], (500, 300), 0, 150, 3, 1, 15,
                    4, 4, True)
        c6 = Action('put_initial1ize1', ['red1_put'], ['get_points'], [a13, a5], (500, 400), 180, 150, 3, 1, 15, 4, 4, True)
		
		#   activate lighthouse
        c7 = Action('lighthouse', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a13], (400, 400), 180, 150, 3, 1, 15, 4, 4, True)
        
        #   get cup 2
        c8 = Action('cup_prepare2', ['no_cups'], ['cup_prepare'], [a6, a2, a8], (500, 800), 180, 150, 3, 1, 15, 1, 4, True)
        c9 = Action('cup_get2', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (400, 800), 180, 150, 3, 1, 15, 4, 4, True)
        c10 = Action('cup_hold2', ['cup_get'], ['get_points', 'have_cups'], [a5], (500, 800), 0, 150, 3, 1, 15, 1, 4, True)

        #   put second round cup
        c13 = Action('put_set2', ['have_cups', 'second_time'], ['cup_set'], [a4, a12, a6], (800, 400), 90, 150, 3, 1, 15, 4, 4, True)
        c14 = Action('put_green2', ['cup_set'], ['green2_put'], [a4, a11, a6], (800, 400), 180, 150, 3, 1, 15, 4, 4, True)
        c15 = Action('put_leftover2', ['green2_put'], ['leftover2_put'], [a4, a7, a6], (800, 400), 90, 150, 3, 1, 15, 4, 4, True)
        c16 = Action('put_red2', ['leftover2_put'], ['red2_put', 'no_cups', 'home_full'], [a4, a12, a6], (800, 400), 0, 150, 3, 1, 15, 4, 4, True)
        c17 = Action('put_initialize2', ['red2_put'], ['get_points'], [a13, a5], (800, 1000), 270, 150, 3, 1, 15, 4, 4, True)

        #  go home
        c18 = Action('go_home', [], ['at_home'], [a13], (300, 700), 90, 150, 3, 1, 15, 4, 4, True)

        child_action_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15]
        action_path = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c13, c14, c15, c16, c17]
        go_home_path = [c18]

        for child in child_action_list:
            green = 0
            red = 1
            if child.name == 'finger_open_green2':
                for i in range(len(cup_color)):
                    if cup_color[i] == green:
                        child.effects.append(i+1)
            elif child.name == 'finger_open_red2':
                for i in range(len(cup_color)):
                    if cup_color[i] == red:
                        child.effects.append(i+1)

    return action_path, go_home_path
