from action import *


def setting(mode):
    if mode is 1:  # name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup
        c1 = Action("cup1",      ['2', '4'],   ['get_cup'], (700, 300), 3, 90, 150, 1, 0, 1, True)
        c2 = Action("cup2",      ['1'],        ['get_cup'], (200, 670), 3, 90, 150, 1, 0, 2, True)
        c3 = Action("cup3",      ['3'],        ['get_cup'], (300, 1010), 3, 60, 150, 1, 0, 3, True)
        c4 = Action('lift',      [],           ['lift_left', '1', '2'], (300, 300), 3, 60, 150, 1, 15, 6, False)
        c5 = Action('hand_down', [],           [], (700, 300), 3, 80, 150, 1, 15, 6, False)
        c6 = Action('hand_up',   [],           [], (1800, 670), 3, 90, 150, 1, 15, 6, False)
        c7 = Action('goto',      [],           [], (1650, 1265), 3, 270, 150, 1, 15, 6, False)
        c8 = Action('lower',     [],           [], (700, 300), 3, 60, 270, 1, 15, 6, False)
        c9 = Action('open',      ['1', '2', '3', '4'], [], (1800, 270), 3, 270, 150, 1, 15, 6, True)
        c10 = Action('goto',      [], [], (1650, 1265), 3, 270, 150, 1, 15, 6, False)
        c11 = Action('goto', [], [], (600, 300), 3, 270, 150, 1, 15, 6, False)

        demo_path = [c5, c1, c3, c4, c1, c6, c7, c8, c9, c10, c11]

    return demo_path
