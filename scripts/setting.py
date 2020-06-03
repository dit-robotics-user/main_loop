"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
#!/usr/bin/env python
#coding=utf-8
from action import *
from mission import *
from operator import attrgetter
#mode = 1
#(name, preconditions, effects, child_action, position, degree, speed, cost, mode, speed_mode_radius, type_number, cup_number, wait):
#mode1 = position mode ; mode2 = speed mode

def setting(mode, action_list, child_list, current_world_state, mission_list, my_pos, my_degree, cup_color, direction):
    if mode is 1:
        my_pos = (700, 300)
        a1 = Action('cup_close_1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a2 = Action('cup_close_2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a3 = Action('cup_close_3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a4 = Action('cup_close_4', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a5 = Action('cup_close_5', [], [1, 3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a6 = Action('cup_close_6', [], [2, 4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a7 = Action('cup_close_all', [], [1, 2, 3, 4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a8 = Action('cup_open_1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a9 = Action('cup_open_2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a10 = Action('cup_open_3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a11 = Action('cup_open_4', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a12 = Action('cup_open_5', [], [1, 3], [], (0, 0), 0, 0, 0, 0, 2, 0, 0, True)
        a13 = Action('cup_open_6', [], [2, 4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a14 = Action('cup_open_all', [], [1, 2, 3, 4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a15 = Action('windsock_hand_up', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
        a16 = Action('windsock_hand_down', [], [0], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
        a17 = Action('flag', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 6, 0, True)
        a18 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
        a19 = Action('lift_left', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 8, 0, True)
        a20 = Action('lift_right', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
        a21 = Action('lower_left', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 10, 0, True)
        a22 = Action('lower_right', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 11, 0, True)
        a23 = Action('lower_both', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
        a24 = Action('auto_grab_1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
        a25 = Action('auto_grab_2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
        a26 = Action('auto_grab_3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
        a27 = Action('auto_grab_4', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)

        child_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27]

        #   activate lighthouse
        c1 = Action('lighthouse', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a18], (500, 500), 85, 150, 3, 1, 15, 4, 4, True)

        #   push windsock
        c2 = Action('hand_prepare', ['hand_down', 'windsocks_unpushed'], ['hand_up'], [a15], (700, 300), 85, 150, 3, 1, 15, 4, 4, True)
        c3 = Action('hand_push', ['hand_up'], ['get_points', 'windsocks_pushed', 'hand_down'], [a16], (400, 400), 85, 150, 3, 1, 15, 4, 4, True)

        #   go home
        c4 = Action('go_home', [], ['at_home'], [a18], (300, 300), 85, 150, 3, 1, 15, 4, 4, True)
        c5 = Action('raise_flag', ['at_home'], ['flag_raised', 'go_home'], [a17], (300, 300), 85, 150, 3, 1, 15, 4, 4, True)

        #   lift left
        c6 = Action('lift_left_layer', ['left_layer_full'], ['1', '2', 'get_points'], [a19], my_pos, my_degree, 150, 3, 1, 15, 1, 4, False)

        #   lift right
        c7 = Action('lift_right_layer', ['right_layer_full'], ['3', '4', 'get_points', 'right_layer_up'], [a20], my_pos, my_degree, 150, 3, 1, 15, 1, 4, False)

        #   put cups
        c8 = Action('put_prepare', ['have_cups'], ['put_prepare'], [a18], (400, 400), 85, 150, 3, 1, 15, 4, 4, True)
        c9 = Action('put_cups', ['put_prepare'], ['cups_out', '1', '2', '3', '4'], [], (600, 600), 85, 150, 3, 1, 15, 4, 4, True)
        c10 = Action('put_done', ['cups_out'], ['get_points'], [a18], (400, 400), 85, 150, 3, 1, 15, 4, 4, True)

        #   speed mode grab
        c11 = Action('cup_prepare', ['grab_available'], ['cup_prepare'], [a24], my_pos, my_degree, 150, 0, 1, 15, 1, 0, True)
        c12 = Action('cup_speed_mode_1', ['cup_prepare', '2'], ['get_points', 'get_cups', 'have_cups'], [a1], (800, 800), 0,
                    150, 0, 2, 15, 1, 0, True)

        #   pos mode grab
        c13 = Action('cup_pos_mode_1', ['grab_available', '1', '3'], ['get_points', 'get_cups', 'have_cups'], [a5],
                    (300, 300), 0, 150, 0, 1, 15, 2, 1, True)
        c14 = Action('cup_pos_mode_2', ['grab_available', '1'], ['get_points', 'get_cups', 'have_cups'], [a1],
                     (400, 400), 0, 150, 0, 1, 15, 2, 1, True)
        c15 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a2],
                     (500, 500), 0, 150, 0, 1, 15, 2, 1, True)

        action_list = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15]
        #action_list = [ c11, c12]

        current_world_state = ['1', '2', '3', '4', 'grab_available', 'hand_down', 'lighthouse_unactivated', 'windsocks_unpushed']

        m1 = Mission('get_points', 1, 0)
        m2 = Mission('go_home', 1, 1)

        mission_list = [m1, m2]

    return action_list, current_world_state, mission_list, child_list, my_pos




