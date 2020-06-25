"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action_demo_2 import *

# name, preconditions, effects, child_action, position, degree, speed, cost, mode, speed_mode_radius, type_number, cup_number, wait
def setting(mode, cup_color):
	print(cup_color)
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

		c1 = Action('goto_1', ['no_cups'], ['cup_prepare'], [a13], (939, 796), 330, 150, 3, 1, 15, 1, 4, True)
		#   get cup 1
		c2 = Action('cup_prepare1', ['no_cups'], ['cup_prepare'], [a6, a2, a8], (1598, 256), 270, 150, 3, 1, 15, 1, 4, True)
		c3 = Action('cup_get1', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (1598, 106), 270, 180, 3, 1, 15, 4, 4, True)
		c4 = Action('cup_hold1', ['cup_get'],  ['get_points', 'have_cups'], [a5], (1598, 223), 0, 150, 3, 1, 15, 1, 4, True)

		#   put first round cup
		c5 = Action('put_red1', ['green1_put'], ['red1_put', 'no_cups', 'second_time'], [a4, a10, a6], (909, 223), 0, 150, 3,
					1, 15, 4, 4, True)
		c6 = Action('put_green1', ['have_cups', 'first_time'], ['green1_put'], [a4, a9, a6], (342, 223), 0, 150, 3, 1, 15,
					4, 4, 	True)

		#   activate lighthouse
		c7 = Action('lighthouse_setup', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a13], (323, 288), 180, 150, 3, 1, 15, 4, 4, True)
		c8 = Action('lighthouse_bump', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a3], (250, 288), 180, 150, 3, 1, 15, 4, 4, True)
		c9 = Action('leave_lighthouse', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a13], (329, 255), 90, 150, 3, 1, 15, 4, 4, True)

		#   get cup 2
		c10 = Action('cup_prepare2', ['no_cups'], ['cup_prepare'], [a6, a2, a8], (315, 852), 180, 150, 3, 1, 15, 1, 4, True)
		c11 = Action('cup_get2', ['cup_prepare'], ['cup_get'], [a3, a7, a6], (100, 865), 180, 150, 3, 1, 15, 4, 4, True)
		c12 = Action('cup_hold2', ['cup_get'], ['get_points', 'have_cups'], [a5], (296, 852), 180, 150, 3, 1, 15, 1, 4, True)

		#   put second round cup
		c13 = Action('put_set2', ['have_cups', 'second_time'], ['cup_set'], [a4, a12, a6], (808, 268), 90, 150, 3, 1, 15, 4, 4, True)
		c14 = Action('put_greenset2', ['have_cups', 'second_time'], ['cup_set'], [a13], (808, 206), 180, 150, 3, 1, 15, 4, 4, True)
		c15 = Action('put_green2', ['cup_set'], ['green2_put'], [a4, a11, a6], (680, 206), 180, 150, 3, 1, 15, 4, 4, True)
		c16 = Action('put_leftoverset2', ['have_cups', 'second_time'], ['cup_set'], [a13], (680, 206), 90, 150, 3, 1, 15, 4, 4, True)
		c17 = Action('put_leftover2', ['green2_put'], ['leftover2_put'], [a4, a7, a6], (808, 268), 90, 150, 3, 1, 15, 4, 4, True)
		c18 = Action('put_redset2', ['have_cups', 'second_time'], ['cup_set'], [a13], (808, 206), 0, 150, 3, 1, 15, 4, 4, True)
		c19 = Action('put_red2', ['leftover2_put'], ['red2_put', 'no_cups', 'home_full'], [a4, a12, a6], (914, 206), 0, 150, 3, 1, 15, 4, 4, True)
		c20 = Action('put_initialize2', ['red2_put'], ['get_points'], [a13, a5], (909, 988), 270, 150, 3, 1, 15, 4, 4, True)

		#  go home
		c21= Action('go_home', [], ['at_home'], [a13], (300, 700), 90, 150, 3, 1, 15, 4, 4, True)

		child_action_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15]
		action_path = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18, c19, c20]
		go_home_path = [c21]

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
