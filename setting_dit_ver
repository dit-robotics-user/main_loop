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
		a5 = Action('hand1_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)
		a6 = Action('hand1_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 1, 0, True)

		a7 = Action('hand2_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a8 = Action('hand2_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a9 = Action('hand2_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a10 = Action('hand2_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a11 = Action('hand2_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)
		a12 = Action('hand2_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 2, 0, True)

		a13 = Action('hand3_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a14 = Action('hand3_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a15 = Action('hand3_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a16 = Action('hand3_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a56 = Action('hand3_release2', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a17 = Action('hand3_inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)
		a18 = Action('hand3_origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 3, 0, True)

		a19 = Action('hand4_out_get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a20 = Action('hand4_out_get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a21 = Action('hand4_out_get3', [], [3], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a22 = Action('hand4_release', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
		a57 = Action('hand4_release2', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 4, 0, True)
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

		a41 = Action('hand1+2inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a42 = Action('1+2suck_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
		a43 = Action('hand1+2release1', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a46 = Action('hand1+2release2', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a44 = Action('1+2suck_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 13, 0, True)
		a45 = Action('hand1+2origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a47 = Action('hand1+2get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		a58 = Action('hand1+2get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 12, 0, True)
		
		a48 = Action('hand3+4inside', [], [8], [], (0, 0), 0, 0, 0, 0, 0, 14, 0, True)
		a49 = Action('3+4suck_on', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 15, 0, True)
		a50 = Action('hand3+4release1', [], [4], [], (0, 0), 0, 0, 0, 0, 0, 14, 0, True)
		a51 = Action('hand3+4release2', [], [5], [], (0, 0), 0, 0, 0, 0, 0, 14, 0, True)
		a52 = Action('3+4suck_off', [], [6], [], (0, 0), 0, 0, 0, 0, 0, 15, 0, True)
		a53 = Action('hand3+4origin', [], [9], [], (0, 0), 0, 0, 0, 0, 0, 14, 0, True)
		a54 = Action('hand3+4get1', [], [1], [], (0, 0), 0, 0, 0, 0, 0, 14, 0, True)
		a55 = Action('hand3+4get2', [], [2], [], (0, 0), 0, 0, 0, 0, 0, 14, 0, True)

        #a59 =

        #   hand 1 get cup1
		#c1 = Action('hand1_get_cup', ['n'], ['c'], [a1, a25, a37, a33, a5, a26], (616, 2717), 214, 160, 3, 1, 15, 1, 4, True)
		c1 = Action('hand1_get_cup', ['n'], ['c'], [a47, a42, a37, a33, a41, a44], (603, 2721), 218, 800, 3, 1, 15, 1, 4, True)
		c3 = Action('hand1_get_cup', ['n'], ['c'], [a45, a47, a42, a37, a33, a41, a44], (982, 2719), 322, 800, 3, 1, 15, 1, 4, True)

		#   hand 2 get cup1
		#c2 = Action('hand2_get_cup', ['h', 'e'], ['t'], [a7, a6, a27, a37, a33, a11, a28], (616, 2717), 214, 150, 3, 1, 15, 4, 4, True)
		#c4 = Action('hand2_get_cup', ['h', 'e'], ['t'], [a6, a7, a27, a37, a33, a11, a28], (292, 2200), 271, 800, 3, 1, 15, 4, 4, True)

		#   hand 3 get cup1
		#c6 = Action('hand3_get_cup', ['n'], ['c'], [a24, a13, a29, a37, a33, a17, a30], (1132, 1903), 133, 800, 3, 1, 15, 1, 4, True)
		#c8 = Action('hand3_get_cup', ['n'], ['c'], [a13, a24, a29, a37, a33, a17, a30], (1329, 2466), 317, 150, 3, 1, 15, 1, 4, True)

		#   hand 4 get cup1
		#c5 = Action('hand4_get_cup', ['l'], ['d'], [a12, a19, a31, a37, a33, a23, a32], (616, 1988), 124, 800, 3, 1, 15, 4, 4, True)
		#c7 = Action('hand4_get_cup', ['l'], ['d'], [a18, a19, a31, a37, a33, a23, a32], (1329, 2466), 317, 150, 3, 1, 15, 4, 4, True)
		#c7 = Action('hand4-2_get_cup', ['l'], ['d'], [a18, a24, a54, a49, a37, a33, a48, a52], (1295, 2491), 323, 800, 3, 1, 15, 4, 4, True)

		'''
		#   hand 1 get cup2
		c9 = Action('hand1_get_cup2', ['n'], ['c'], [a18, a2, a25, a37, a33, a5, a26], (1399, 1170), 357, 800, 3, 1, 15, 1, 4, True)

		#   hand 2 get cup2
		c10 = Action('hand2_get_cup2', ['h', 'e'], ['t'], [a8, a6, a27, a37, a33, a11, a28], (1399, 1170), 357, 800, 3, 1, 15, 4, 4, True)
		'''
		'''
		#   hand 1 get cup2
		c9 = Action('hand1_get_cup2', ['n'], ['c'], [a55, a37], (1399, 1190), 181, 800, 3, 1, 15, 1, 4, True)
		
		c10 = Action('hand1_get_cup2', ['n'], ['c'], [a51, a33], (1750, 1190), 181, 800, 3, 1, 15, 1, 4, True)
		'''	
		c9 = Action('hand1_get_cup2', ['n'], ['c'], [a58, a42, a37, a33, a41, a44], (1470, 1175), 1, 800, 3, 1, 15, 1, 4, True)
		#   hand 3 get cup2
		#c11 = Action('hand3_get_cup2', ['n'], ['c'], [a12, a14, a29, a37, a33, a17, a30], (1730, 1151), 180, 150, 3, 1, 15, 1, 4, True)
		c11 = Action('hand3_get_cup2', ['n'], ['c'], [a45, a55, a49, a37, a33, a48, a52], (1807, 1179), 182, 800, 3, 1, 15, 1, 4, True)

		#   hand 4 get cup2
		#c12 = Action('hand4_get_cup2', ['l'], ['d'], [a20, a18, a31, a37, a33, a23, a32], (1730, 1151), 180, 150, 3, 1, 15, 4, 4, True)

		'''
		#   hand 1 release layer1 (top)
		c13 = Action('hand1_release_cup1', ['h'], ['p'], [a24, a3, a25, a34, a33, a4, a37, a26, a33, a6], (1600, 1800), 0, 800, 3, 1, 15, 4, 4, True)
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
		'''	
		#   hand 3 release layer1
		#c13 = Action('hand3_release1', ['h'], ['p'], [a24, a15, a29, a34, a33, a16, a37, a30, a33, a18], (1645, 1195), 181, 800, 3, 1, 15, 4, 4, True)
		#   hand 3 release layer2
		#c15 = Action('hand3_release2', ['h'], ['p'], [a15, a29, a35, a33, a56, a37, a30, a33, a18], (1605, 1178), 181, 800, 3, 1, 15, 4, 4, True)
		#   hand 3 release layer3
		c17 = Action('hand3_release3', ['h'], ['p'], [a15, a29, a36, a33, a16, a37, a30, a33, a18], (1535, 1175), 181, 800, 3, 1, 15, 4, 4, True)


		#   hand 4 release layer1 (top)
		#c14 = Action('hand4_release1', ['h'], ['p'], [a21, a31, a34, a33, a22, a37, a32, a33, a24], (1650, 1195), 181, 800, 3, 1, 15, 4, 4, True)
		#   hand 4 release layer2
		#c16 = Action('hand4_release2', ['h'], ['p'], [a21, a31, a35, a33, a57, a37, a32, a33, a24], (1605, 1195), 181, 800, 3, 1, 15, 4, 4, True)
		#   hand 4 release layer3
		c18 = Action('hand4_release3', ['h'], ['p'], [a21, a31, a36, a33, a22, a37, a32, a33, a24], (1535, 1175), 181, 800, 3, 1, 15, 4, 4, True)

		#   hand 1+2 release layer1 (top)
		'''
		c19 = Action('hand1+2_release_cup', ['n'], ['c'], [a41, a42, a34, a33, a46, a37, a44, a33, a45], (1660, 1195), 0, 800, 3, 1, 15, 1, 4, True)
		c20 = Action('hand1+2_release_cup', ['n'], ['c'], [a41, a42, a35, a33, a43, a37, a44, a33, a45], (1620, 1195), 0, 800, 3, 1, 15, 1, 4, True)
		c21 = Action('hand1+2_release_cup', ['n'], ['c'], [a41, a42, a36, a33, a46, a37, a44, a33, a45], (1580, 1195), 0, 800, 3, 1, 15, 1, 4, True)
		'''
		c19 = Action('hand1+2_release_cup', ['n'], ['c'], [a53, a41, a42, a34, a33, a46, a37, a44, a33, a45], (1735, 1175), 0, 800, 3, 1, 15, 1, 4, True)
		c20 = Action('hand1+2-2_release_cup', ['n'], ['c'], [a53, a41, a42, a35, a33, a4, a10, a37, a44, a33, a45], (1695, 1175), 0, 800, 3, 1, 15, 1, 4, True)
		c21 = Action('hand1+2-3_release_cup', ['n'], ['c'], [a41, a42, a36, a33, a46, a37, a44, a33, a45], (1630, 1175), 0, 800, 3, 1, 15, 1, 4, True)
		
		#  go home
		c22 = Action('go_home1', [], ['at_home'], [a34], (977, 1970), 311, 800, 3, 1, 15, 4, 4, True)
		c23 = Action('raise_flag', [], ['at_home'], [a39], (510, 2580), 153, 800, 3, 1, 15, 4, 4, True)
		
		# go to
		c24 = Action('goto1', [], ['at_home'], [a40], (1380, 1666), 271, 800, 3, 1, 15, 4, 4, True)
		c25 = Action('goto2', [], ['at_home'], [a45], (1458, 1175), 181, 800, 3, 1, 15, 4, 4, True)
		#c26 = Action('goto3', [], ['at_home'], [a34], (1350, 1195), 0, 800, 3, 1, 15, 4, 4, True)
		
		c27 = Action('go_home2', [], ['at_home'], [a33], (382, 2750), 305, 800, 3, 1, 15, 4, 4, True)

		child_action_list = [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15]
		action_path = [c1, c3, c24, c9, c25, c11, c19, c20, c21, c17, c18, c23]
		'''action_path = [c26, c9, c11, c13, c14, c15, c16, c17, c18, c20, c21]'''
		'''action_path = [c19, c20, c21, c13, c14, c15, c16, c17, c18, c22, c27, c23]'''
		go_home_path = [c22, c27, c23]

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
