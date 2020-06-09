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

'''def insert_cup_color(open_red, open_green, cup_colors):
    for color in colors:
        if color == red:
            open_red.effect.append(color)
        else:
            open_green.effect.append(color)'''


def setting(mode, action_list, current_world_state, mission_list, my_pos, my_degree, cup_color):
    if mode is 1:
        a1 = Action('hand_close', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a2 = Action('hand_open', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a3 = Action('wrist_mid', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a4 = Action('wrist_down', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a5 = Action('wrist_cup', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a6 = Action('wrist_up', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a7 = Action("finger_close", [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
        a8 = Action('finger_open', [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a9 = Action('finger_open_green1', [], [], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a10 = Action('finger_open_red1', [], [], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a11 = Action('finger_open_green2', [], [], [], (0, 0), 0, 0, 0, 0, 3, 0, 0, True)
        a12 = Action('finger_open_red2', [], [], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
        a13 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
        a14 = Action('windsock_hand_up', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a15 = Action('windsock_hand_down', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)

        #   get cup
        c1 = Action('cup_prepare', ['no_cups'], ['cup_prepare'], [a6, a2, a8], (600, 600), 90, 150, 3, 1, 15, 1, 4, True)
        c2 = Action('cup_get1', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (700, 700), 90, 150, 3, 1, 15, 4, 4, True)
        c3 = Action('cup_get2', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (800, 800), 90, 150, 3, 1, 15, 4, 4, True)
        c4 = Action('cup_get3', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (690, 400), 90, 150, 3, 1, 15, 4, 4, True)
        c5 = Action('cup_hold', ['cup_get'],  ['get_points', 'have_cups'], [a5], (300, 300), 90, 150, 3, 1, 15, 1, 4, True)

        #   activate lighthouse
        c6 = Action('lighthouse', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a13], (530, 300), 85, 150, 3, 1, 15, 4, 4, True)

        #   put first round cup
        c7 = Action('put_green1', ['have_cups', 'first_time'], ['green1_put'], [a4, a9, a6], (300,450), 85, 150, 3, 1, 15, 4, 4, True)
        c8 = Action('put_red1', ['green1_put'], ['red1_put', 'no_cups', 'second_time'], [a4, a10], (600, 500), 85, 150, 3, 1, 15, 4, 4, True)
        c9 = Action('put_initial1', ['red1_put'], ['get_points'], [a13, a5], (700, 900), 85, 150, 3, 1, 15, 4, 4, True)

        #   put second round cup
        c10 = Action('put_set2', ['have_cups', 'second_time'], ['cup_set'], [a4, a12, a6], (380, 340), 85, 150, 3, 1, 15, 4, 4, True)
        c11 = Action('put_green2', ['cup_set'], ['green2_put'], [a4, a11, a6], (600, 800), 85, 150, 3, 1, 15, 4, 4, True)
        c12 = Action('put_leftover2', ['green2_put'], ['leftover2_put'], [a4, a7, a6], (1000, 750), 85, 150, 3, 1, 15, 4, 4, True)
        c13 = Action('put_red2', ['leftover2_put'], ['red2_put', 'no_cups', 'home_full'], [a4, a12, a6], (800, 830), 85, 150, 3, 1, 15, 4, 4, True)
        c14 = Action('put_initial2', ['red2_put'], ['get_points'], [a13, a5], (700, 850), 85, 150, 3, 1, 15, 4, 4, True)

        #   push windsock
        c15 = Action('hand_prepare', ['hand_down', 'windsocks_unpushed'], ['hand_up'], [a14], (600, 430), 85, 150, 3, 1, 15, 4, 4, True)
        c16 = Action('hand_push', ['hand_up'], ['get_points', 'windsocks_pushed', 'hand_down'], [a15], (900, 470), 85, 150, 3, 1, 15, 4, 4, True)

        #  go home
        c17 = Action('go_home', [], ['go_home', 'at_home'], [a13], (800, 670), 85, 150, 3, 1, 15, 4, 4, True)

        child_action_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15]
        action_list = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17]
        #action_list = [c1, c2, c3, c4, c5, c7, c8, c9, c10, c11, c12, c13, c14, c17]

        current_world_state = ['no_cups', 'hand_down', 'first_time', 'lighthouse_unactivated', 'windsocks_unpushed']

        m1 = Mission('get_points', 1, 0)
        m2 = Mission('go_home', 1, 1)

        mission_list = [m1, m2]

        for child in child_action_list:
            green = 0
            red = 1
            if child.name == 'finger_open_green2':
                for i in range(len(cup_color)):
                    if cup_color[i] == green:
                        child.effects.append(i + 1)
            elif child.name == 'finger_open_red2':
                for i in range(len(cup_color)):
                    if cup_color[i] == red:
                        child.effects.append(i + 1)

    return action_list, current_world_state, mission_list




