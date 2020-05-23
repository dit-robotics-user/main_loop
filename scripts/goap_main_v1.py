#!/usr/bin/env python

from main_loop.srv import *
import rospy
from setting_demo import *

class MyClass:
	action_done = False  # <----
	my_pos = (3, 3)  # <----
	output = [0]*15  # --->
	output_speed = 0  # --->
	output_mode = -1  # --->
	output_degree = -1  # --->
	output_position = (0, 0)  # --->
	output_wait = True  # --->
	output_mission_name = "start"
	last_action_done_ = False
	input_name = " "
	

demo_path = setting(1)
goal = []
path_done = False
left_side = 0
right_side = 0

'''
1 2  3  4
5 6  7  8
9 10 11 12
'''
def output_processor(output_action, left_side, right_side):
    output = [-1]*15
    if output_action.iscup is True and output_action.grab_mode is 1:
        for p in output_action.preconditions:
            if p is '1':
                claw_num = 2*left_side + 0  # use layer 2 - left_side + '1'
                output[claw_num] = 1
            elif p is '2':
                claw_num = 2*left_side + 1  # use layer 2 - left_side + '2'
                output[claw_num] = 1
            elif p is'3':
                claw_num = 2*right_side + 6  # use layer 2 - right_side + '3'
                output[claw_num] = 1
            else:
                claw_num = 2*right_side + 7  # use layer 2 - right_side + '4'
                output[claw_num] = 1
    if output_action.iscup is True and output_action.grab_mode is 2:
        for p in output_action.preconditions:
            if p is '1':
                claw_num = 2*left_side + 0  # use layer 2 - left_side + '1'
                output[claw_num] = 2
            elif p is '2':
                claw_num = 2*left_side + 1  # use layer 2 - left_side + '2'
                output[claw_num] = 2
            elif p is'3':
                claw_num = 2*right_side + 6  # use layer 2 - right_side + '3'
                output[claw_num] = 2
            else:
                claw_num = 2*right_side + 7  # use layer 2 - right_side + '4'
                output[claw_num] = 2

    elif output_action.number is 6:  # lower 6
        if right_side is 0:
            output[12] = 0
            output[13] = 0
        if right_side is 1:
            output[12] = 0
            output[13] = 0
        elif right_side is 2:
            output[12] = 1
            output[13] = 1
    elif output_action.number is 2:  # lift 2
        if right_side is 0:
            output[12] = 1
            output[13] = 1
        if right_side is 1:
            output[12] = 2
            output[13] = 2
        elif right_side is 2:
            output[12] = 2
            output[13] = 2
    elif output_action.number is 4:  # hand down 4
        output[14] = 0
    elif output_action.number is 3:  # hand up 3
        output[14] = 2
    if output_action.number is 7:  # open 7
        for p in output_action.preconditions:
            if p is '1':
                claw_num = 2 * left_side + 0  # use layer 2 - left_side + '1'
                output[claw_num] = 0
            elif p is '2':
                claw_num = 2 * left_side + 1  # use layer 2 - left_side + '2'
                output[claw_num] = 0
            elif p is '3':
                claw_num = 2 * right_side + 6  # use layer 2 - right_side + '3'
                output[claw_num] = 0
            else:
                claw_num = 2 * right_side + 7  # use layer 2 - right_side + '4'
                output[claw_num] = 0
    return output

            
def handle_return_to_main(req):
	MyClass.action_done = req.action_done  # <----
	MyClass.my_pos = (req.pos[0],req.pos[1])
	MyClass.input_name = req.mission_name
	return [MyClass.output_degree,MyClass.output_speed,MyClass.output_mode,MyClass.output_position,MyClass.output,MyClass.output_wait,MyClass.output_mission_name]

def add_two_ints_server():
	global demo_path
	global left_side
	global right_side
	rospy.init_node('goap_test')
	rospy.Service('goap_test_v1', goap_demo, handle_return_to_main)
	while True:

		path_done = False
		path = demo_path[0]
		#if path.iscup is True:
			#if path.grab_mode is 2:  # speed mode
				#path.tangent_point_calculation(MyClass.my_pos, stretch_factor=5)
		MyClass.output = output_processor(path, left_side, right_side)
		MyClass.output_degree = path.degree
		MyClass.output_speed = path.speed
		MyClass.output_mode = path.grab_mode
		MyClass.output_position = path.position
		MyClass.output_wait = path.iswait
		MyClass.output_mission_name = path.name

		if MyClass.action_done is True and demo_path[0].name == MyClass.input_name:
			if path.number is 6:  # lower
				left_side -= 1
				right_side -= 1
			elif path.number is 2:  # lift
				left_side += 1
				right_side += 1
			if len(demo_path) > 1:
				demo_path.remove(demo_path[0])
			
		

if __name__ == "__main__":
	add_two_ints_server()
