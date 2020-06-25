#!/usr/bin/env python
#coding=utf-8
import rospy
from main_loop.srv import *
from setting_demo_2 import *


def output_processor(output_action, current_left_layer, current_right_layer):
    # 0-11: claws 12: left 13: right 14: windsock hand 15:flag
    output = [-1] * 16
    #print(str(current_right_layer) + ' -=> ' + str(current_left_layer))
    if output_action.type_number is 1 or output_action.type_number is 2 or output_action.type_number is 13:
        for p in output_action.effects:
            if p is 1:
                claw_num = 6 - 2 * current_left_layer   # use layer 2 - left_side + '1'
                if output_action.type_number is 1:
                    output[claw_num] = 1
                elif output_action.type_number is 2:
                    output[claw_num] = 0
                else:
                    output[claw_num] = 2
            elif p is 2:
                claw_num = 7 - 2 * current_left_layer  # use layer 2 - left_side + '2'
                if output_action.type_number is 1:
                    output[claw_num] = 1
                elif output_action.type_number is 2:
                    output[claw_num] = 0
                else:
                    output[claw_num] = 2
            elif p is 3:
                claw_num = 12 - 2 * current_right_layer  # use layer 2 - right_side + '3'
                if output_action.type_number is 1:
                    output[claw_num] = 1
                elif output_action.type_number is 2:
                    output[claw_num] = 0
                else:
                    output[claw_num] = 2
            else:
                claw_num = 13 - 2 * current_right_layer  # use layer 2 - right_side + '4'
                if output_action.type_number is 1:
                    output[claw_num] = 1
                elif output_action.type_number is 2:
                    output[claw_num] = 0
                else:
                    output[claw_num] = 2

    elif output_action.type_number is 10:  # lower left
        if current_left_layer is 3 or current_left_layer is 2:
            output[12] = 0
        elif current_left_layer is 1:
            output[12] = 1
    elif output_action.type_number is 11:  # lower right
        if current_right_layer is 3 or current_right_layer is 2:
            output[13] = 0
        elif current_right_layer is 1:
            output[13] = 1
    elif output_action.type_number is 12:  # lower both
        if current_left_layer is 3 or current_left_layer is 2:
            output[12] = 0
        elif current_left_layer is 1:
            output[12] = 1
        if current_right_layer is 3 or current_right_layer is 2:
            output[13] = 0
        elif current_right_layer is 1:
            output[13] = 1
    elif output_action.type_number is 8:  # lift left
        if current_left_layer is 3:
            output[12] = 1
        elif current_left_layer is 2 or current_left_layer is 1:
            output[12] = 2
    elif output_action.type_number is 9:  # lift right
        if current_right_layer is 3:
            output[13] = 1
        elif current_right_layer is 2 or current_right_layer is 1:
            output[13] = 2
    elif output_action.type_number is 5:  # hand down 4
        output[14] = output_action.effects[0]
    elif output_action.type_number is 6:  # flag
        output[15] = output_action.effects[0]
    return output



class mymain:  #main輸入與輸出參數需在此calss定義
	action_done = False          # <---
	my_pos = (3, 3)              # <---
	cup_color = [5, 3, 5, 5, 3]  # <---
	north_or_south = 2           # <---
	time = 0                     # <---
	name = 0                     # <---
	child_name = 0               # <---	
	my_degree = 0                # <---
	output_speed = 0             # --->
	output_mode = -1             # --->
	output_degree = -1           # --->
	output_position = (0, 0)     # --->
	output = [-1]*15			 # --->
	output_wait = True           # --->
	output_name = "start"        # --->
	output_child_name = "start"  # --->

class set_frommain:
	strategy = 0
	init_pos = (3,3)
	set_finish = 0 
	setting_finish = 0 
	cup_color = [0, 0, 0, 0, 0]  # <---

goal = []
path_done = False
go_home_time = 20
left_side = 3
right_side = 3
north = 0
south = 1
north_position = (500, 300)      ######
south_position = (1500, 300)      ######
demo_path = []
give_next_action = True
go_home_flag = False
action_path, go_home_path = setting(1)
action_name = 0 
counter = 0


def return_to_main(req):  #main輸入參數與獲得結果存取處(service回調函式)  
	set_frommain.set_finish = 1
	mymain.action_done = req.action_done  # <----
	mymain.my_pos = (req.pos[0],req.pos[1])
	mymain.cup_color = [req.cup_color[0],req.cup_color[1],req.cup_color[2],req.cup_color[3],req.cup_color[4]]
	mymain.north_or_south = req.north_or_south
	mymain.my_degree = req.my_degree
	mymain.time = req.time 
	mymain.name = req.mission_name
	mymain.child_name = req.mission_child_name
	
	return [mymain.output_speed,mymain.output_mode,mymain.output_degree,mymain.output_position,mymain.output,mymain.output_wait,mymain.output_name , mymain.output_child_name]

def setting_strategy(req):
	set_frommain.strategy = req.strategy
	set_frommain.init_pos = (req.init_pos[0],req.init_pos[1])
	set_frommain.cup_color = [req.cup_color[0],req.cup_color[1],req.cup_color[2],req.cup_color[3],req.cup_color[4]]
	set_frommain.set_finish =  req.set_finish
	return [True]
	

def goap_server():
	global give_next_action
	global action_path
	global left_side
	global right_side
	global go_home_flag
	global go_home_time
	global demo_path
	global counter

	rospy.init_node('main_demo_4')
	rospy.Service('goap_test_v1', goap_demo_2,return_to_main)
	while True:
		
		#goap的loop放這:
		if set_frommain.set_finish == 1 :
			if counter == 0:
				action_path, go_home_path = setting(1)
				print('setfinish')
				counter=1		
		
			path_done = False
			if mymain.time >= go_home_time and go_home_flag == False:  # switch to go home mode
				demo_path[:] = []
				for action in go_home_path:
					if mymain.north_or_south == north:
						action.position = north_position
					elif mymain.north_or_south == south:
						action.position = south_position
				action_path = go_home_path
				go_home_flag = True
				give_next_action = True
				
			if give_next_action == True:  # expand action into its child actions
				for c_action in action_path[0].child_action:
					if action_path[0].mode == 2:
						action_path[0].tangent_point_calculation(my_pos, 2)
					c_action.position = action_path[0].position
					c_action.degree = action_path[0].degree
					c_action.speed = action_path[0].speed
					c_action.mode = action_path[0].mode
					c_action.wait = action_path[0].wait
					demo_path.append(copy.deepcopy(c_action))
				action_name = action_path[0].name
				action_path.remove(action_path[0])
				give_next_action = False
			
			path = demo_path[0]
			
			mymain.output_child_name = path.name 
			mymain.output_name = action_name
			mymain.output = output_processor(path, left_side, right_side)
			mymain.output_degree = path.degree
			mymain.output_speed = path.speed
			mymain.output_mode = path.mode
			mymain.output_position = path.position
			mymain.output_wait = path.wait

			if mymain.action_done is True and demo_path[0].name == mymain.child_name and  action_name == mymain.name :
				if path.type_number is 9:  # lift right
					right_side -= 1
				elif path.type_number is 8:  # lift left
					left_side -= 1
				elif path.type_number is 11:  # lower right
					right_side += 1
				elif path.type_number is 10:  # lower left
					left_side += 1
				elif path.type_number is 12:  # lower both
					left_side += 1
					right_side += 1
				print(path.name)
				print(path.position)
				print(path.degree)
				print(mymain.output)
				print()
				if len(demo_path) > 1:
					demo_path.remove(demo_path[0])
				elif len(demo_path) == 1 and len(action_path) >= 1:
					demo_path.remove(demo_path[0])
					give_next_action = True


if __name__ == "__main__":
	goap_server()
