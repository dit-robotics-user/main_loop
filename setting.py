"""
Set all actions, goals and current world states here!!
this .py file is the only place you will need to change
=======================================================
"""
from action import *

# name, preconditions, effects, child_action, position, degree, speed, cost, mode, speed_mode_radius, type_number, cup_number, wait
def setting(mode):#, cup_color):
	demo_path = []
	if mode is 1:
		a1 = Action('hand1_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
		a2 = Action('hand1_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
		a3 = Action('hand1_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)  #get the other hand's cup
		a4 = Action('hand1_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
		a25 = Action('hand1_release2', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
		a5 = Action('hand1_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
		a6 = Action('hand1_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)

		a7 = Action('hand2_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a8 = Action('hand2_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a9 = Action('hand2_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a10 = Action('hand2_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a26 = Action('hand1_release2', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a11 = Action('hand2_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a12 = Action('hand2_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)

		a13 = Action('hand3_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a14 = Action('hand3_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a15 = Action('hand3_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a16 = Action('hand3_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a17 = Action('hand3_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a18 = Action('hand3_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)

		a19 = Action('hand4_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a20 = Action('hand4_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a21 = Action('hand4_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a22 = Action('hand4_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a23 = Action('hand4_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a24 = Action('hand4_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)

		a25 = Action('suck1_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)
		a26 = Action('suck1_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 5, 0, True)

		a27 = Action('suck2_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 6, 0, True)
		a28 = Action('suck2_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 6, 0, True)

		a29 = Action('suck3_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)
		a30 = Action('suck3_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 7, 0, True)

		a31 = Action('suck4_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 8, 0, True)
		a32 = Action('suck4_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 8, 0, True)

		a33 = Action('stepper_pos1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
		a34 = Action('stepper_pos2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
		a35 = Action('stepper_pos3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
		a36 = Action('stepper_pos4', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
		a37 = Action('stepper_pos5', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)
		#        a38 = Action('stepper_pos6', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 9, 0, True)

		a39 = Action('flag_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 10, 0, True)

		a40 = Action('goto', [], [], [], (0, 0), 0, 0, 0, 0, 0, 11, 0, True)

		a41 = Action('hand3+4inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a42 = Action('3+4suck_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
		a43 = Action('hand3+4release1', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a46 = Action('hand3+4release2', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a44 = Action('3+4suck_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
		a45 = Action('hand3+4origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)

        

        #   hand 1 get cup1
		c1 = Action('hand1_get_cup', ['n'], ['c'], [a1, a25, a37, a33, a5, a26], (488, 2875), 215, 160, 3, 1, 15, 1, 4, True)
		c3 = Action('hand1_get_cup', ['n'], ['c'], [a12, a1, a25, a37, a33, a5, a26], (162, 2568), 239, 160, 3, 1, 15, 1, 4, True)

		#   hand 2 get cup1
		c2 = Action('hand2_get_cup', ['h', 'e'], ['t'], [a7, a6, a27, a37, a33, a11, a28], (488, 2875), 215, 150, 3, 1, 15, 4, 4, True)
		c4 = Action('hand2_get_cup', ['h', 'e'], ['t'], [a6, a7, a27, a37, a33, a11, a28], (144, 2323), 291, 150, 3, 1, 15, 4, 4, True)

		#   hand 3 get cup1
		c6 = Action('hand3_get_cup', ['n'], ['c'], [a24, a13, a29, a37, a33, a17, a30], (911, 2003), 188, 150, 3, 1, 15, 1, 4, True)
		c8 = Action('hand3_get_cup', ['n'], ['c'], [a24, a13, a29, a37, a33, a17, a30], (1105, 2756), 335, 150, 3, 1, 15, 1, 4, True)

		#   hand 4 get cup1
		c5 = Action('hand4_get_cup', ['l'], ['d'], [a12, a19, a31, a37, a33, a23, a32], (475, 2112), 144, 150, 3, 1, 15, 4, 4, True)
		c7 = Action('hand4_get_cup', ['l'], ['d'], [a18, a19, a31, a37, a33, a23, a32], (1105, 2756), 335, 150, 3, 1, 15, 4, 4, True)


	#   hand 1 get cup2
		c9 = Action('hand1_get_cup2', ['n'], ['c'], [a18, a2, a25, a37, a33, a5, a26], (1262, 1286), 359, 160, 3, 1, 15, 1, 4, True)

		#   hand 2 get cup2
		c10 = Action('hand2_get_cup2', ['h', 'e'], ['t'], [a6, a8, a27, a37, a33, a11, a28], (1262, 1286), 359, 150, 3, 1, 15, 4, 4, True)

		#   hand 3 get cup2
		c11 = Action('hand3_get_cup2', ['n'], ['c'], [a12, a14, a29, a37, a33, a17, a30], (1585, 1270), 175, 150, 3, 1, 15, 1, 4, True)

		#   hand 4 get cup2
		c12 = Action('hand4_get_cup2', ['l'], ['d'], [a18, a20, a31, a37, a33, a23, a32], (1585, 1270), 175, 150, 3, 1, 15, 4, 4, True)


		#   hand 1 release layer1 (top)
		c13 = Action('hand1_release_cup1', ['h'], ['p'], [a24, a3, a25, a34, a33, a4, a37, a26, a33, a6], (1600, 1800), 0, 150, 3, 1, 15, 4, 4, True)
		#   hand 1 release layer2
		c15 = Action('hand1_release_cup2', ['h'], ['p'], [a3, a25, a35, a33, a25, a37, a26, a33, a6], (1600, 1800), 0, 150, 3, 1, 15, 4, 4, True)
		#   hand 1 release layer3
		c17 = Action('hand1_release_cup3', ['h'], ['p'], [a3, a25, a36, a33, a4, a37, a26, a33, a6], (1600, 1800), 0, 150, 3, 1, 15, 4, 4, True)


		#   hand 2 release layer1
		c14 = Action('hand2_release_cup1', ['h'], ['p'], [a9, a27, a34, a33, a10, a37, a28, a33, a12], (1300, 2550), 100, 150, 3, 1, 15, 4, 4, True)
		#   hand 2 release layer2
		c16 = Action('hand2_release_cup2', ['h'], ['p'], [a9, a27, a35, a33, a26, a37, a28, a33, a12], (1300, 2550), 100, 150, 3, 1, 15, 4, 4, True)
		#   hand 2 release layer3
		c18 = Action('hand2_release_cup3', ['h'], ['p'], [a9, a27, a36, a33, a10, a37, a28, a33, a12], (1300, 2550), 100, 150, 3, 1, 15, 4, 4, True)

		'''        #   hand 3 release layer1
		c13 = Action('hand3_release1', ['h'], ['p'], [a17, a29, a34, a33, a16, a37, a30, a33, a18], (1564, 1255), 351, 150, 3, 1, 15, 4, 4, True)
		#   hand 3 release layer2
		c15 = Action('hand3_release2', ['h'], ['p'], [a17, a29, a35, a33, a16, a37, a30, a33, a18], (1512, 1261), 352, 150, 3, 1, 15, 4, 4, True)
		#   hand 3 release layer3
		c17 = Action('hand3_release3', ['h'], ['p'], [a17, a29, a36, a33, a16, a37, a30, a33, a18], (1460, 1267), 351, 150, 3, 1, 15, 4, 4, True)


		#   hand 4 release layer1
		c14 = Action('hand4_release1', ['h'], ['p'], [a23, a31, a34, a33, a22, a37, a32, a33, a24], (1564, 1255), 351, 150, 3, 1, 15, 4, 4, True)
		#   hand 4 release layer2
		c16 = Action('hand4_release2', ['h'], ['p'], [a23, a31, a35, a33, a22, a37, a32, a33, a24], (1512, 1261), 352, 150, 3, 1, 15, 4, 4, True)
		#   hand 4 release layer3
		c18 = Action('hand4_release3', ['h'], ['p'], [a23, a31, a36, a33, a22, a37, a32, a33, a24], (1460, 1267), 351, 150, 3, 1, 15, 4, 4, True)
		'''
		#   hand 3+4 release layer1 (top)
		c19 = Action('hand3+4_release_cup', ['n'], ['c'], [a41, a42, a34, a33, a46, a37, a44, a33, a45], (520, 2700), 200, 160, 3, 1, 15, 1, 4, True)
		c20 = Action('hand3+4_release_cup', ['n'], ['c'], [a41, a42, a35, a33, a43, a37, a44, a33, a45], (520, 2700), 200, 160, 3, 1, 15, 1, 4, True)
		c21 = Action('hand3+4_release_cup', ['n'], ['c'], [a41, a42, a36, a33, a46, a37, a44, a33, a45], (520, 2700), 200, 160, 3, 1, 15, 1, 4, True)

		#  go home
		c22 = Action('go_home', [], ['at_home'], [a40], (300, 700), 85, 150, 3, 1, 15, 4, 4, True)
		c23 = Action('raise_flag', [], ['at_home'], [a39], (300, 700), 85, 150, 3, 1, 15, 4, 4, True)

		c24 = Action('goto', [], ['at_home'], [a40], (1500, 2250), 135, 150, 3, 1, 15, 4, 4, True)

		child_action_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15]
		action_path = [c1, c2, c3, c4, c5, c6, c24, c7, c8, c9, c10, c11, c12, c13, c14]
		go_home_path = [c17, c18]

        '''for child in child_action_list:
            green = 5
            red = 3
            if child.name == 'finger_open_green2':
                for i in range(len(cup_color)):
                    if cup_color[i] == green:
                        child.effects.append(i+1)
            elif child.name == 'finger_open_red2':
                for i in range(len(cup_color)):
                    if cup_color[i] == red:
                        child.effects.append(i+1)'''

	return action_path, go_home_path
