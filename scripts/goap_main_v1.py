#!/usr/bin/env python

from main_loop.srv import *
import rospy
from setting import *

class MyClass:
	action_done = False  # <----
	my_pos = (3, 3)  # <----
	output = [-1]*7  # --->
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
def output_processor(output_action):
    output = [-1]*7
    if output_action.number is 1:  # wrist
        output[0] = output_action.preconditions[0]
    elif output_action.number is 2:  # hand
        output[1] = output_action.preconditions[0]
    elif output_action.number is 3:  # finger open
        for p in output_action.preconditions:
            output[int(p)+1] = 1
    elif output_action.number is 4:  # finger open
        for p in output_action.preconditions:
            output[int(p)+1] = 0
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
	rospy.Service('goap_test_v1', goap_, handle_return_to_main)
	while True:

		path_done = False
		path = demo_path[0]

		MyClass.output_mission_name = path.name
		MyClass.output = output_processor(path)
		MyClass.output_degree = path.degree
		MyClass.output_speed = path.speed
		MyClass.output_mode = path.grab_mode
		MyClass.output_position = path.position
		MyClass.output_wait = path.iswait

		if MyClass.action_done is True and demo_path[0].name == MyClass.input_name:
			if len(demo_path) > 1:
				demo_path.remove(demo_path[0])
			
		

if __name__ == "__main__":
	add_two_ints_server()
