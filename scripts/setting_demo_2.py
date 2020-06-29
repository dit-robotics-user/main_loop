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
		a7 = Action('wrist_lighthouse', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
		a8 = Action("finger_close", [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a9 = Action('finger_open', [], [1, 2, 3, 4, 5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a10 = Action('finger_open_green1', [], [2, 4], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a11 = Action('finger_open_red1', [], [1, 3, 5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a12 = Action('finger_open_green2', [], [], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a13 = Action('finger_open_red2', [], [], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a14 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
		a15 = Action('windsock_hand_up', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a16 = Action('windsock_hand_down', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		
		c1 = Action('goto_1', ['no_cups'], ['cup_prepare'], [a14], (635, 800), 90, 150, 3, 1, 15, 1, 4, True)
		c2 = Action('goto_2', ['no_cups'], ['cup_prepare'], [a14], (1600, 500), 270, 150, 3, 1, 15, 1, 4, True)
		#   get cup 1
		c3 = Action('cup_prepare1', ['no_cups'], ['cup_prepare'], [a6, a2, a9], (1600, 256), 270, 150, 3, 1, 15, 1, 4, True)
		c4 = Action('cup_get1', ['cup_prepare'], ['cup_get'], [a3, a8, a6], (1600, 109), 270, 180, 3, 1, 15, 4, 4, True)
		c5 = Action('cup_hold1', ['cup_get'],  ['get_points', 'have_cups'], [a5], (1600, 223), 0, 150, 3, 1, 15, 1, 4, True)#1600

		#   put first round cup
		c6 = Action('put_red1', ['green1_put'], ['red1_put', 'no_cups', 'second_time'], [a4, a11, a6], (943, 223), 0, 150, 3,
					1, 15, 4, 4, True)
		c7 = Action('put_green1', ['have_cups', 'first_time'], ['green1_put'], [a4, a10, a6], (318, 223), 0, 150, 3, 1, 15,
					4, 4, 	True)

		#   activate lighthouse
		c8 = Action('lighthouse_setup', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a7], (323, 288), 180, 150, 3, 1, 15, 4, 4, True)
		c9 = Action('lighthouse_bump', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a14,a6], (117, 288), 180, 150, 3, 1, 15, 4, 4, True)
		c10 = Action('leave_lighthouse', ['lighthouse_unactivated'], ['get_points', 'lighthouse_activated'], [a14], (329, 255), 90, 150, 3, 1, 15, 4, 4, True)

		#   get cup 2
		c11 = Action('cup_prepare2', ['no_cups'], ['cup_prepare'], [a6, a2, a9], (315, 802), 180, 150, 3, 1, 15, 1, 4, True)#845
		c12 = Action('cup_get2', ['cup_prepare'], ['cup_get'], [a3, a8, a6], (85, 807), 180, 150, 3, 1, 15, 4, 4, True)#845
		c13 = Action('cup_hold2', ['cup_get'], ['get_points', 'have_cups'], [a5], (296, 802), 180, 150, 3, 1, 15, 1, 4, True)

		#   put second round cup
		c14 = Action('put_set2', ['have_cups', 'second_time'], ['cup_set'], [a4, a13, a6], (808, 400), 90, 150, 3, 1, 15, 4, 4, True)
		c15 = Action('put_greenset2', ['have_cups', 'second_time'], ['cup_set'], [a14], (808, 206), 180, 150, 3, 1, 15, 4, 4, True)
		c16 = Action('put_green2', ['cup_set'], ['green2_put'], [a4, a12, a6], (715, 206), 180, 150, 3, 1, 15, 4, 4, True)
		c17 = Action('put_leftoverset2', ['have_cups', 'second_time'], ['cup_set'], [a14], (808, 206), 90, 150, 3, 1, 15, 4, 4, True)
		c18 = Action('put_leftover2', ['green2_put'], ['leftover2_put'], [a4, a8, a6], (808, 395), 92, 150, 3, 1, 15, 4, 4, True)
		c19 = Action('put_redset2', ['have_cups', 'second_time'], ['cup_set'], [a14], (808, 206), 0, 150, 3, 1, 15, 4, 4, True)
		c20 = Action('put_red2', ['leftover2_put'], ['red2_put', 'no_cups', 'home_full'], [a4, a13, a6], (890, 206), 0, 150, 3, 1, 15, 4, 4, True)
		c21 = Action('put_initialize2', ['red2_put'], ['get_points'], [a14, a5], (808, 206), 90, 150, 3, 1, 15, 4, 4, True)

		#  go home north
		#c22= Action('go_home_init_n', [], ['at_home'], [a14], (320, 988), 90, 150, 3, 1, 15, 4, 4, True)
		#c23= Action('go_home_north', [], ['at_home'], [a14], (320, 330), 90, 150, 3, 1, 15, 4, 4, True)
		c22= Action('go_home_init_n', [], ['at_home'], [a14], (808, 206), 180, 150, 3, 1, 15, 4, 4, True)
		c23= Action('go_home_north_put_hand_down', [], ['at_home_hand'], [a9], (630, 206), 180, 150, 3, 1, 15, 4, 4, True)
		
		#  go home south
		c24= Action('go_home_init_s', [], ['at_home'], [a14], (1300, 988), 90, 150, 3, 1, 15, 4, 4, True)
		c25= Action('go_home_south', [], ['at_home'], [a9], (1300, 300), 90, 150, 3, 1, 15, 4, 4, True)

		child_action_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16]
		action_path = [c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25]
		go_home_path_north = [c22, c23]
		go_home_path_south = [c24, c25]

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

	return action_path, go_home_path_north, go_home_path_south
