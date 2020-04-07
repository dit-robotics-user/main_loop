"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action import *


def setting(mode, demo_path):
    if mode is 1:  # name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup
        c1 = Action("cup1_1",      ['1', '3'], ['get_cup'], (200, 20), 3, 90, 150, 2, 0, 1, True)
        c2 = Action("cup1_2",      ['2', '4'], ['get_cup'], (60, 50), 3, 90, 150, 2, 0, 1, True)
        c3 = Action("cup2_1",      ['1'],      ['get_cup'], (12, 3), 3, 70, 150, 2, 0, 2, True)
        c4 = Action('cup2_2',      ['1'],      ['get_cup'], (90, 90), 3, 40, 150, 2, 0, 2, True)
        c5 = Action('cup3_1',      ['2'],      ['get_cup'], (15, 13), 3, 85, 150, 1, 0, 3, True)
        c6 = Action('raise_flag',  ['flag'],   ['get_cup'], (3, 4), 3, 85, 150, 1, 0, 5, False)
        c7 = Action('hand_down',   [],         ['flag'], (5, 5), 3, 85, 150, 1, 0, 5, False)
        c8 = Action('cup4_1',      ['1'],      ['get_cup'], (3, 30), 3, 85, 150, 2, 15, 4, True)
        c9 = Action('lift_left',   [],         ['lift_left', '1', '2'], (5, 5), 3, 85, 150, 2, 15, 6, False)
        c10 = Action('lift_right', [],         ['lift_right', '3', '4'], (5, 5), 3, 85, 150, 2, 15, 6, False)
        c11 = Action('put_cup',    [],         ['1', '2', '3', '4'], (90, 90), 3, 85, 150, 2, 15, 7, False)

        demo_path = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11]

    return demo_path


