"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action import *

# name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup, iswait
def setting(mode):
    if mode is 1:
		c1 = Action("cup1",      ['1', '3'],   ['get_cup'], (500, 500), 3, 143, 150, 1, 0, 1, True, True)
		c2 = Action("cup2",      ['1'],        ['get_cup'], (400, 700), 3, 90, 150, 1, 0, 1, True, True)
		c3 = Action("cup3",      ['4'],        ['get_cup'], (400, 900), 3, 60, 150, 1, 0, 1, True, True)
		c4 = Action('lift1',      [],           ['lift_left', '1', '2'], (700,300 ), 3, 90, 150, 1, 15, 2, False, True)
		c5 = Action('lift2',      [], ['lift_left', '1', '2'], (500, 500), 3, 143, 150, 1, 15, 2, False, True)

		c6 = Action('hand_up', [],           [], (1500, 700), 3, 90, 150, 1, 15, 3, False, True)
		c7 = Action('hand_down',   [],           [], (1500, 950), 3, 90, 150, 1, 15, 4, False, True)
		c8 = Action('goto',      [],           [], (1300, 1250), 3, 0, 150, 1, 15, 5, False, True)
		c16 = Action('goto1',      [],           [], (700, 500), 3, 0, 150, 1, 15, 5, False, True)
		c9 = Action('lower1',     [],           [], (1550, 1250), 3, 0, 270, 1, 15, 6, False, True)
		c10 = Action('lower2', [], [],               (1300, 1250), 3, 0, 270, 1, 15, 6, False, True)
		c11 = Action('open1',      ['1', '2', '3', '4'], [], (1700, 1250), 3, 0, 150, 1, 15, 7, True, True)
		c12 = Action('open2',      ['1', '2', '3', '4'], [], (1550, 1250), 3, 0, 150, 1, 15, 7, True, True)

		c13 = Action('hand_up2', [],           [], (1200, 900), 3, 90, 150, 1, 15, 3, False, True)
		c14 = Action("cup4",      ['4'],   ['get_cup'], (500, 500), 3, 143, 150, 1, 0, 1, True, True)     
		c15 = Action('lift3',      [], ['lift_left', '1', '2'], (700, 2000), 3, 143, 150, 1, 15, 2, False, True)
		#c8 = Action('goto',      [],           [], (700, 300), 3, 0, 150, 1, 15, 5, False, True)
		demo_path = [c15,c9]
		demo_path = [c4, c1, c5, c2, c3, c6, c7, c8, c11, c9, c12, c10,c16]

    return demo_path

