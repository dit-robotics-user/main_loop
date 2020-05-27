#!/usr/bin/env python
# coding=utf-8

import rospy
from main_loop.srv import *
from goap import *
import math

class MyClass:
	action_done = False  # <----
	my_pos = (400, 1800)  # <----
	output = [-1]*7  # --->
	output_speed = 0  # --->
	output_mode = -1  # --->
	output_degree = 0  # --->
	output_position = (0, 0)  # --->
	output_wait = True  # --->
	output_mission_name = "start"
	output_task_name = "start"
	last_action_done_ = False
	input_name = " "
	input_child_name = " "
	replan_mission = False
	kill_mission = True
	task_mission = " " 
	setting_number = 1
	time = 50
	cup_color = 0
	direction = 0
	my_degree = 0
	output_mission_child_name = " start "

# ==REPLAN時給的懲罰==
def penalty(current_action, penalty_cost, penalty_turns, action_list):
    for action in action_list:
        if action.name == current_action.name:
            action.cost += penalty_cost
            penalty_action = action
            break
    return penalty_turns, penalty_action


# ==懲罰回合結束回復==
def lift_penalty(penalty_action, penalty_cost, action_list):
    for action in action_list:
        if action.name == penalty_action.name:
            action.cost -= penalty_cost


# ==將指令傳換成一串輸出給MAIN==
def output_processor_small_chicken(output_action):
    output = [-1]*7
    if output_action.type_number is 1:  # wrist
        output[0] = output_action.effects[0]
    elif output_action.type_number is 2:  # hand
        output[1] = output_action.effects[0]
    elif output_action.type_number is 3:  # finger open
        for p in output_action.effects:
            output[int(p)+1] = 1
    elif output_action.type_number is 4:  # finger open
        for p in output_action.effects:
            output[int(p)+1] = 0
    #elif output_action.type_number is 5:  # windsock hand
        #output[0] = output_action.effects[0]
    '''print(output_action.name)
    print(output)
    print('------------')'''
    return output


def output_processor_big_chicken(output_action, current_left_layer, current_right_layer):
    output = [-1] * 16
    print(str(current_right_layer) + ' -=> ' + str(current_left_layer))
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
    elif output_action.type_number is 4:  # hand down 4
        output[14] = 0
    elif output_action.type_number is 3:  # hand up 3
        output[14] = 2
    elif output_action.type_number is 5:  # windsock hand
        output[15] = output_action.effects[0]
    print(output_action.name)
    print(output)
    print('------------')
    return output


# ==將特定做完的動作刪除==
def kill_action(killed_action, action_list):
    if killed_action.type_number != 1:  # don't delete cup prepare / cup hold
        for action in action_list:
            if action.name == killed_action.name:
                action_list.remove(action)


# ==計算任務優先順序==
def calculate_mission_priority(m_list, time, go_home_time):
    # calculate the missions priority according to current state and return the best mission
    # basically if time allows, get points is prioritized, else we are forced to go home
    for m in m_list:
        m.calculate_priority(time, go_home_time)
    m_list = sorted(m_list, key=attrgetter('priority'))  # sort after calculation
    return m_list


def current_world_state_processor(cws, current_left_layer, current_right_layer):
    if 'get_cups' in cws:
        cws.remove('get_cups')
        if current_left_layer is 1 and current_right_layer is 1 and ('1' and '2' and '3' and '4' not in cws):
            cws.append('whole robot full')
        else:
            cws.append('grab_available')
    if '1' not in cws and '2' not in cws:
        if current_left_layer is 1:
            cws.append('left_side_full')
        else:
            cws.append('left_layer_full')
    if '3' not in cws and '4' not in cws:
        if current_right_layer is 1:
            cws.append('right_side_full')
        else:
            cws.append('right_layer_full')


def get_number_of_cups(cws, current_left_layer, current_right_layer):
    number_of_cups = 2 * (3 - current_right_layer) + 2 * (3 - current_left_layer) + 4
    for state in cws:
        if '1' or '2' or '3' or '4' == state:
            number_of_cups -= 1
    return number_of_cups


def cost_adjuster(cws, current_left_layer, current_right_layer):
    magic_number = 10
    new_cost = magic_number*math.exp(-(get_number_of_cups(cws, current_left_layer, current_right_layer)))
    return new_cost



# ==收MAIN的資料==
action_done = True
replan_mission = False
kill_mission = False
my_pos = (700, 300)
my_degree = 0
setting_number = 1
time = 50
cup_color = 0
direction = 0

# ==GOAP自己的變數==
go_home_time = 2000
action_list = []
child_list = []
current_world_state = []
mission_list = []
output = [-1]*16  # 小雞用(大雞則為[-1]*15)
output_speed = 0
output_mode = -1
output_degree = -1
output_position = (0, 0)
output_wait = True
output_mission_name = "start"
path_done = False
penalty_cost = 2000
penalty_turns = 1
penalty_timer = -1
penalty_undergoing = False
penalty_action = 0
go_home_flag = False
path = []
current_left_layer = 3
current_right_layer = 3

# ==設定任務及動作==
action_list, current_world_state, mission_list, child_list, MyClass.my_pos = setting(MyClass.setting_number, action_list, child_list, current_world_state, mission_list, MyClass.my_pos, MyClass.my_degree, MyClass.cup_color, MyClass.direction)

def handle_return_to_main(req):
	MyClass.action_done = req.action_done  
	MyClass.my_pos = (req.pos[0],req.pos[1])
	MyClass.input_name = req.mission_name
	MyClass.input_child_name = req.mission_child_name
	MyClass.kill_mission = req.kill_mission 
	MyClass.time = req.time
	return [MyClass.output_degree,MyClass.output_speed,MyClass.output_mode,MyClass.output_position,MyClass.output,MyClass.output_wait,MyClass.output_mission_name,MyClass.output_mission_child_name]

def add_two_ints_server():
	global mission_list
	global penalty_timer
	global new_current_world_state
	global current_world_state
	global penalty_undergoing
	global current_left_layer
	global current_right_layer
	rospy.init_node('main_1')
	rospy.Service('goap_test_v1', goap_, handle_return_to_main)
	while True:
		path_done = False
		# ==重製動作參數(好像可刪?)==
		for actions in action_list:
			actions.refresh()
		mission_list = calculate_mission_priority(mission_list, MyClass.time, go_home_time)  # <---
		new_current_world_state = copy.deepcopy(current_world_state)  # prevent unwanted overlap or alter
		for action in action_list:
			if action.name == 'put_prepare':
				action.cost = cost_adjuster(current_world_state, current_left_layer, current_right_layer)
		print()
		#print('=>' + str(new_current_world_state) + '<=')
		path = goap([mission_list[0].name], new_current_world_state, MyClass.my_pos, action_list, child_list, current_left_layer, current_right_layer)  # calculate path

		if len(path) == 0:
			continue

		# ==餵PATH==
		while path_done is False:
			# ==MAIN判定重算路徑==
			if MyClass.replan_mission is True:
				penalty_timer, penalty_action = penalty(path[0], penalty_cost, penalty_turns,
							action_list)  # 給予當下被REPLAN的動作一個COST上的懲罰，並懲罰幾回合(任務)
				penalty_undergoing = True
				path_done = True  # 跳出WHILE並重算
				continue

			# ==懲罰回合已到==
			if penalty_timer == 0:
				lift_penalty(penalty_action, penalty_cost, action_list)  # 解除COST上加諸的懲罰
				penalty_undergoing = False

			# ==MAIN判定刪除當下動作(任務?)==
			if MyClass.kill_mission is True:
				kill_action(path[0], action_list)
				path_done = True  # 跳出WHILE並重算
				continue

			# ==時間到則終止動作回家(只進入一次以免影響回家的陸續動作)==
			if MyClass.time >= go_home_time and go_home_flag is False:
				path_done = True  # 跳出WHILE並重算
				go_home_flag = True
				continue

			# ==若PATH還沒做完==
			if len(path) != 0:
				top_path = path[0]

				# ==小雞應該不會使用到==
				if top_path.mode == 2:
					top_path.tangent_point_calculation(MyClass.my_pos, 3)

				# ==餵動作裡的子動作==
				while len(top_path.child_action) != 0:
					top_child = top_path.child_action[0]
					MyClass.output_mission_name = top_path.name
					MyClass.output_mission_child_name = top_child.name
					MyClass.output_degree = top_child.degree
					MyClass.output_speed = top_child.speed
					MyClass.output_mode = top_child.mode
					MyClass.output_position = top_child.position
					MyClass.output_wait = top_child.wait
					MyClass.output = output_processor_big_chicken(top_child, current_left_layer, current_right_layer)
					

					# ==子動作做完==
					if MyClass.action_done is True and MyClass.output_mission_child_name == MyClass.input_child_name:
						top_path.child_action.remove(top_child)
						if MyClass.output_mission_name == 'lift_left':
							current_left_layer -= 1
						elif MyClass.output_mission_name == 'lift_right':
							current_right_layer -= 1
						elif MyClass.output_mission_name == 'lower_left':
							current_left_layer += 1
						elif MyClass.output_mission_name == 'lower_right':
							current_right_layer += 1

				current_world_state = top_path.result_world_state  # update current world state status
				kill_action(top_path, action_list)  # remove (specific) finished actions
				path.remove(top_path)  # feed out the next action in path
				current_world_state_processor(current_world_state, current_left_layer, current_right_layer)

			# ==PATH做完==
			else:
				if penalty_undergoing is True:  # PENALTY回合減1
					penalty_timer -= 1
				path_done = True


if __name__ == "__main__":
	add_two_ints_server()
