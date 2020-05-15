#!/usr/bin/env python
# coding=utf-8
from main_loop.srv import *
import rospy
from goap import *

class MyClass:
	action_done = False  # <----
	my_pos = (3, 3)  # <----
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
	replan_mission = False
	kill_mission = True
	task_mission = " " 
	setting_number = 1
	time = 50
	cup_color = 0
	direction = 0

# ==收MAIN的資料==
action_done = True
replan_mission = False
kill_mission = True
my_pos = (3, 3)
my_degree = 0
setting_number = 1
time = 100
cup_color = 0
direction = 0

# ==GOAP自己的變數==
go_home_time = 80000
action_list = []
current_world_state = []
mission_list = []
output = [-1]*7  # 小雞用(大雞則為[-1]*15)
output_speed = 0
output_mode = -1
output_degree = -1
output_position = (0, 0)
output_wait = True
output_mission_name = "start"
output_task_name = " "
path_done = False
penalty_cost = 2000
penalty_turns = 1
penalty_timer = -1
penalty_undergoing = False
penalty_action = 0
path = []

# ==設定任務及動作==
action_list, current_world_state, mission_list = setting(setting_number, action_list, current_world_state, mission_list, my_pos, my_degree, cup_color, direction)	

def penalty(current_action, penalty_cost, penalty_turns, action_list):
    for action in action_list:
        if action.name == current_action.name:
            action.cost += penalty_cost
            penalty_action = action
            break
    return penalty_turns, penalty_action


def lift_penalty(penalty_action, penalty_cost, action_list):
    for action in action_list:
        if action.name == penalty_action.name:
            action.cost -= penalty_cost


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


def kill_action(killed_action, action_list):
    if killed_action.type_number != 1:  # don't delete cup prepare / cup hold
        for action in action_list:
            if action.name == killed_action.name:
                action_list.remove(action)


def calculate_mission_priority(m_list,time, go_home_time):
    # calculate the missions priority according to current state and return the best mission
    for m in m_list:
        m.calculate_priority(time, go_home_time)
    m_list = sorted(m_list, key=attrgetter('priority'))
    return m_list
            
def handle_return_to_main(req):
	MyClass.action_done = req.action_done  # <----
	MyClass.my_pos = (req.pos[0],req.pos[1])
	MyClass.input_name = req.mission_name
	MyClass.kill_mission = req.kill_mission 
	MyClass.time = req.time
	return [MyClass.output_degree,MyClass.output_speed,MyClass.output_mode,MyClass.output_position,MyClass.output,MyClass.output_wait,MyClass.output_mission_name,MyClass.output_task_name]

def add_two_ints_server():
	global mission_list
	global penalty_timer
	global new_current_world_state
	global current_world_state
	global penalty_undergoing
	rospy.init_node('main_1')
	rospy.Service('goap_test_v1', goap_, handle_return_to_main)
	while True:
		path_done = False
		for actions in action_list:
			actions.refresh()
		mission_list = calculate_mission_priority(mission_list, MyClass.time, go_home_time)  # <---
		new_current_world_state = copy.deepcopy(current_world_state)
		path = goap([mission_list[0].name], new_current_world_state, MyClass.my_pos, action_list)

		
		if len(path) == 0:
			continue

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

			if len(path) != 0:
				top_path = path[0]

				if top_path.mode == 2:
					top_path.tangent_point_calculation(my_pos, 3)

				while len(top_path.child_action) != 0:
					top_child = top_path.child_action[0]
					MyClass.output_mission_name = top_child.name
					MyClass.output_degree = top_child.degree
					MyClass.output_speed = top_child.speed
					MyClass.output_mode = top_child.mode
					MyClass.output_position = top_child.position
					MyClass.output_wait = top_child.wait
					MyClass.output = output_processor_small_chicken(top_child)
					
					
					print(top_path.name + " " + top_child.name)

					if MyClass.action_done is True and MyClass.output_mission_name == MyClass.input_name:
						top_path.child_action.remove(top_child)

				current_world_state = top_path.result_world_state
				kill_action(top_path, action_list)
				path.remove(top_path)
			else:
				if penalty_undergoing is True:  # PENALTY回合減1
					penalty_timer -= 1
				path_done = True

			
		

if __name__ == "__main__":
	add_two_ints_server()
