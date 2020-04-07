"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
#!/usr/bin/env python
from action import *
from mission import *
from operator import attrgetter
#mode = 1
#(name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup):


def setting(mode, action_list, current_world_state, mission_list, my_pos, my_degree):
    if mode is 1:
        c1 = Action("cup1_1", ['1', '3'], ['get_cup'], (200, 20),   3, 90, 150, 2, 0, 1, True)
        c2 = Action("cup1_2", ['2', '4'], ['get_cup'], (60, 50),   3, 90, 150, 2, 0, 1, True)
        c3 = Action("cup2_1", ['1'],      ['get_cup'], (12, 3),  3, 70, 150, 2, 0, 2, True)
        c4 = Action('cup2_2', ['1'],      ['get_cup'], (90, 90),   3, 40, 150, 2, 0, 2, True)
        c5 = Action('cup3_1', ['2'],      ['get_cup'], (15, 13), 3, 85, 150, 1, 0, 3, True)
        c6 = Action('raise_flag', ['flag'],   ['get_cup'], (3, 4), 3, 85, 150, 1, 0, 5, False)
        c7 = Action('hand_down',   [],    ['flag'], (5, 5), 3, 85, 150, 1, 0, 5, False)
        c8 = Action('cup4_1', ['1'],      ['get_cup'], (3, 30), 3, 85, 150, 2, 15, 4, True)
        c9 = Action('lift_left', [], ['lift_left', '1', '2'], my_pos, my_degree, 85, 150, 2, 15, 6, False)
        c10 = Action('lift_right', [], ['lift_right', '3', '4'], my_pos, my_degree, 85, 150, 2, 15, 6, False)
        c11 = Action('put_cup', [], ['1', '2', '3', '4'], (90, 90), 3, 85, 150, 2, 15, 7, False)

        action_list = [c1, c2, c3, c4, c5, c8, c9, c10, c11]

        current_world_state = ['1', '2', '3', '4']

        m1 = Mission('lift_left', 1, 1)
        m2 = Mission('get_cup', 1, 0)
        m3 = Mission('lift_right', 1, 2)
        m4 = Mission('put_cup', 1, 3)

        mission_list = [m1, m2, m3, m4]
    return action_list, current_world_state, mission_list


def calculate_mission_priority(m_list, cws, left_claw, right_claw, my_pos):
    for m in m_list:
        m.calculate_priority(cws, left_claw, right_claw, my_pos)
    m_list = sorted(m_list, key=attrgetter('priority'))
    return m_list


def append_list(dictionary_list, name_of_action, add_list):
    for l in dictionary_list:
        if name_of_action is l.name:
            add_list.append(l)
