"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
# coding=utf-8
from action import *
from mission import *
from operator import attrgetter
#mode = 1
#(name, preconditions, effects, child_action, position, degree, speed, cost, mode, speed_mode_radius, type_number, cup_number, wait):
#mode1 = position mode ; mode2 = speed mode

def setting(mode, action_list, current_world_state, mission_list, my_pos, my_degree, cup_color, direction):
    if mode is 1:
        a1 = Action('hand_close', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a2 = Action('hand_open', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a3 = Action('wrist_mid', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a4 = Action('wrist_down', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a5 = Action('wrist_cup', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a6 = Action('wrist_up', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a7 = Action("finger_close", [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a8 = Action('finger_open', [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a9 = Action('finger_open_green1', [], [1,3,5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a10 = Action('finger_open_red1', [], [2,4], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a11 = Action('finger_open_green2', [], [2,4], [], (0, 0), 0, 0, 0, 0, 3, 0, 0, True)
        a12 = Action('finger_open_red2', [], [1,3,5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a13 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
        a14 = Action('windsock_hand_up', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
        a15 = Action('windsock_hand_down', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
        a16 = Action('flag', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 6, 0, True)

        #   get cup
        c1 = Action('cup_prepare', ['no_cups'], ['cup_prepare'], [a6, a2, a8], (300, 300), 90, 150, 3, 1, 15, 1, 4, True)
        c2 = Action('cup_get1', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (400, 1800), 180, 150, 3, 1, 15, 4, 4, True)
        c3 = Action('cup_get2', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (400, 500), 180, 150, 3, 1, 15, 4, 4, True)
        c4 = Action('cup_get3', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (1000, 600), 90, 150, 3, 1, 15, 4, 4, True)
        c5 = Action('cup_hold', ['cup_get'],  ['get_points', 'have_cups'], [a5], (0, 0), 90, 150, 3, 1, 15, 1, 4, True)

        #   activate lighthouse
        c6 = Action('lighthouse', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a13], (400, 2000), 90, 150, 3, 1, 15, 4, 4, True)

        #   put first round cup
        c7 = Action('put_green1', ['have_cups', 'first_time'], ['green1_put'], [a4, a9, a6], (1100, 2200), 0, 150, 3, 1, 15, 4, 4, True)
        c8 = Action('put_red1', ['green1_put'], ['red1_put'], [a4, a10], (500, 2200), 0, 150, 3, 1, 15, 4, 4, True)
        c9 = Action('put_initial1', ['red1_put'], ['get_points', 'no_cups', 'second_time'], [a13, a5], (500, 2000), 270, 150, 3, 1, 15, 4, 4, True)

        #   put second round cup
        c10 = Action('put_set2', ['have_cups', 'second_time'], ['cup_set'], [a4, a12, a6], (900, 2100), 270, 150, 3, 1, 15, 4, 4, True)
        c11 = Action('put_green2', ['cup_set'], ['green2_put'], [a4, a11, a6], (1100, 2200), 0, 150, 3, 1, 15, 4, 4, True)
        c12 = Action('put_leftover2', ['green2_put'], ['leftover2_put'], [a4, a7, a6], (900, 2100), 270, 150, 3, 1, 15, 4, 4, True)
        c13 = Action('put_red2', ['leftover2_put'], ['red2_put'], [a4, a12, a6], (800, 2200), 180, 150, 3, 1, 15, 4, 4, True)
        c14 = Action('put_initial2', ['red2_put'], ['get_points', 'no_cups', 'home_full'], [a13, a5], (900, 1800), 270, 150, 3, 1, 15, 4, 4, True)

        #   push windsock
        c15 = Action('hand_prepare', ['hand_down', 'windsocks_unpushed'], ['hand_up'], [a14], (400, 1000), 85, 150, 3, 1, 15, 4, 4, True)
        c16 = Action('hand_push', ['hand_up'], ['get_points', 'windsocks_pushed', 'hand_down'], [a15], (800, 1250), 85, 150, 3, 1, 15, 4, 4, True)

        #  go home
        c17 = Action('go_home', [], ['at_home'], [a13], (700, 2200), 270, 150, 3, 1, 15, 4, 4, True)
        c18 = Action('raise_flag', ['at_home'], ['go_home', 'flag_raised'], [a16], (700, 2200), 270, 150, 3, 1, 15, 4, 4, True)

        action_list = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18]
        action_list = [c1, c2, c3, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c17, c18]

        current_world_state = ['no_cups', 'hand_down', 'first_time', 'lighthouse_unactivated', 'windsocks_unpushed']

        m1 = Mission('get_points', 1, 0)
        m2 = Mission('go_home', 1, 1)

        mission_list = [m1, m2]

        '''if direction:
            c17.position = ()  # <---
        else:
            c17.position = ()  # <---
            
        for color in cup_color:
            i = 1
            if color == green:  # <---
                c11.effects.append(i)
            elif color == red:  # <---
                c12.effects.append(i)
            else:
                print('color determine error')
            i += 1'''

    return action_list, current_world_state, mission_list




