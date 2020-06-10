"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action_demo_2 import *

# name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup, iswait
def setting(mode):
    if mode is 1:
        a1 = Action('cup_close_1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a2 = Action('cup_close_2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a3 = Action('cup_close_3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a4 = Action('cup_close_4', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a5 = Action('cup_close_5', [], [1, 3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a6 = Action('cup_close_6', [], [2, 4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a7 = Action('cup_close_7', [], [1, 4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a8 = Action('cup_close_all', [], [1, 2, 3, 4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
        a9 = Action('cup_open_1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a10 = Action('cup_open_2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a11 = Action('cup_open_3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a12 = Action('cup_open_4', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a13 = Action('cup_open_5', [], [1, 3], [], (0, 0), 0, 0, 0, 0, 2, 0, 0, True)
        a14 = Action('cup_open_6', [], [2, 4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a15 = Action('cup_open_all', [], [1, 2, 3, 4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
        a16 = Action('windsock_hand_up', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
        a17 = Action('windsock_hand_down', [], [0], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
        a18 = Action('flag', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 6, 0, True)
        a19 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
        a20 = Action('lift_left', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 8, 0, True)
        a21 = Action('lift_right', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
        a22 = Action('lower_left', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 10, 0, True)
        a23 = Action('lower_right', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 11, 0, True)
        a24 = Action('lower_both', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
        a25 = Action('auto_grab_1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
        a26 = Action('auto_grab_2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
        a27 = Action('auto_grab_3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
        a28 = Action('auto_grab_4', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)

        child_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21,
                      a22, a23, a24, a25, a26, a27, a28]

        #   pos mode grab
        c1 = Action('cup_pos_mode_1', ['grab_available', '1', '3'], ['get_points', 'get_cups', 'have_cups'], [a6],
                     (500, 300), 140, 150, 0, 1, 15, 2, 1, True)
        c2 = Action('cup_pos_mode_2', ['grab_available', '1'], ['get_points', 'get_cups', 'have_cups'], [a1],
                     (400, 650), 90, 150, 0, 1, 15, 2, 1, True)

        #   lift left
        c3 = Action('lift_left_layer', ['left_layer_full'], ['1', '2', 'get_points'], [a20], (400, 650), 90, 150, 3,
                    1, 15, 1, 4, False)

        #   pos mode grab
        c4 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a3],
                     (400, 900), 90, 150, 0, 1, 15, 2, 1, True)

        #   lift right
        c5 = Action('lift_right_layer', ['right_layer_full'], ['3', '4', 'get_points', 'right_layer_up'], [a21],
                    (400, 850), 90, 150, 3, 1, 15, 1, 4, False)

        #   pos mode grab
        c6 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a1],
                     (500, 1800), 45, 150, 0, 1, 15, 2, 1, True)
        c7 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a3],
                     (700, 1900), 20, 150, 0, 1, 15, 2, 1, True)
        c8 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a2],
                     (1000, 1500), 320, 150, 0, 1, 15, 2, 1, True)
        c9 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a4],
                     (1000, 1300), 340, 150, 0, 1, 15, 2, 1, True)

        #   lift left
        c10 = Action('lift_left_layer', ['left_layer_full'], ['1', '2', 'get_points'], [a20], (800, 300), 0, 150, 3,
                    1, 15, 1, 4, False)

        #   lift right
        c11 = Action('lift_right_layer', ['right_layer_full'], ['3', '4', 'get_points', 'right_layer_up'], [a21],
                    (800, 300), 0, 150, 3, 1, 15, 1, 4, False)

        #   pos mode grab
        c12 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a2],
                     (1000, 400), 0, 150, 0, 1, 15, 2, 1, True)
        c13 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a3],
                     (1100, 400), 0, 150, 0, 1, 15, 2, 1, True)

        #   push windsock
        c14 = Action('hand_prepare', ['hand_down', 'windsocks_unpushed'], ['hand_up'], [a16], (1700, 300), 90, 150, 3, 1,
                     15, 4, 4, True)
        c15 = Action('hand_push', ['hand_up'], ['get_points', 'windsocks_pushed', 'hand_down'], [a17], (1700, 700), 90,
                     150, 3, 1, 15, 4, 4, True)

        #   pos mode grab
        c16 = Action('cup_pos_mode_3', ['grab_available', '2'], ['get_points', 'get_cups', 'have_cups'], [a7],
                    (1500, 1900), 0, 150, 0, 1, 15, 2, 1, True)

        #   put cups
        c17 = Action('put_prepare', ['have_cups'], ['put_prepare'], [a19], (1500, 1900), 0, 150, 3, 1, 15, 4, 4, True)
        c18 = Action('put_cups', ['put_prepare'], ['cups_out', '1', '2', '3', '4'], [a15], (1700, 1900), 0, 150, 3, 1, 15,
                    4, 4, True)
        c19 = Action('put_cups', ['put_prepare'], ['cups_out', '1', '2', '3', '4'], [a24, a15], (1600, 1900), 0, 150, 3,
                     1, 15, 4, 4, True)
        c20 = Action('put_cups', ['put_prepare'], ['cups_out', '1', '2', '3', '4'], [a24, a15], (1500, 1900), 0, 150, 3,
                     1, 15, 4, 4, True)
        c21 = Action('put_done', ['cups_out'], ['get_points'], [a19], (1500, 1900), 0, 150, 3, 1, 15, 4, 4, True)

        #   go home
        c22 = Action('go_home', [], ['at_home'], [a19], (700, 300), 90, 150, 3, 1, 15, 4, 4, True)
        c23 = Action('raise_flag', ['at_home'], ['flag_raised', 'go_home'], [a18], (700, 300), 90, 150, 3, 1, 15, 4, 4,
                    True)

        demo_path = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18, c19, c20, c21]
        go_home_path = [c22, c23]

    return demo_path, go_home_path


