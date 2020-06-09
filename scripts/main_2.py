#!/usr/bin/env python
#coding=utf-8

import rospy
from main_loop.srv import *
from goap import *
import math


class mymain:
	setting_number = 1
	replan = False				 # <---
	action_done = False          # <---
	kill_mission = False		 # <---
	my_pos = (3, 3)              # <---
	cup_color = [5, 3, 5, 5, 3]  # <---
	north_or_south = 2           # <---
	my_degree = 0				 # <---
	strategy = 0           		 # <---
	time = 0                     # <---
	name = 0                     # <---
	child_name = 0               # <---
	output_speed = 0             # --->
	output_mode = -1             # --->
	output_degree = -1           # --->
	output_position = (0, 0)     # --->
	output = [-1]*16			 # --->
	output_wait = True           # --->
	output_name = "start"        # --->
	output_child_name = "start"  # --->
	

# ==GOAP自己的變數==
go_home_time = 2000
action_list = []
current_world_state = []
mission_list = []
path_done = False
penalty_cost = 2000
penalty_turns = 1
penalty_timer = -1
penalty_undergoing = False
penalty_action = 0
go_home_flag = False
path = []
north = 0
south = 1
north_position = (1, 1)      ######
south_position = (2, 2)      ######
go_home_flag = False

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
    elif output_action.type_number is 2:  # hand and windsock
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

'''def north_or_south(setting_number, action_list, north_or_south):
    if setting_number == 1:
        north_pos = ()
        south_pos = ()
    elif setting_number == 2:
        north_pos = ()
        south_pos = ()
    for action in action_list:
        if action.name == 'go_home':
            if north_or_south == north:
                action.position = tuple(north_pos)
            else:
                action.position = tuple(south_pos)'''
                
# ==設定任務及動作==
action_list, current_world_state, mission_list = setting(mymain.setting_number, action_list, current_world_state, mission_list, mymain.my_pos, mymain.my_degree, mymain.cup_color)

def handle_return_to_main(req):
	mymain.replan = req.replan 
	mymain.action_done = req.action_done  
	mymain.kill_mission = req.kill_mission
	mymain.my_pos = (req.pos[0],req.pos[1])
	mymain.my_pos = (req.pos[0],req.pos[1])
	mymain.name = req.mission_name
	mymain.child_name = req.mission_child_name
	mymain.kill_mission = req.kill_mission 
	mymain.time = req.time
	mymain.my_degree = req.my_degree
	return [mymain.output_degree,mymain.output_speed,mymain.output_mode,mymain.output_position,mymain.output,mymain.output_wait,mymain.output_name,mymain.output_child_name]


def goap_server():
	rospy.init_node('main_2')
	rospy.Service('goap_test_v1', goap_2, handle_return_to_main)
	global mission_list
	global current_world_state
	global penalty_timer
	global penalty_undergoing
	while 1:
		path_done = False
		if mymain.time >= go_home_time and go_home_flag is False:  # switch to go home mode
			for action in action_list:
				if mymain.north_or_south == north:
					action.position = north_position
				elif mymain.north_or_south == south:
					action.position = south_position
			go_home_flag = True
		# ==重製動作參數(好像可刪?)==
		for actions in action_list:
			actions.refresh()
		mission_list = calculate_mission_priority(mission_list, mymain.time, go_home_time)  # <---
		new_current_world_state = copy.deepcopy(current_world_state)  # prevent unwanted overlap or alter
		print()
		path = goap([mission_list[0].name], new_current_world_state, mymain.my_pos, action_list)  # calculate path

		if len(path) == 0:
			continue

		# ==餵PATH==
		while path_done is False:
			# ==MAIN判定重算路徑==
			if mymain.replan is True:
				penalty_timer, penalty_action = penalty(path[0], penalty_cost, penalty_turns, action_list)  # 給予當下被REPLAN的動作一個COST上的懲罰，並懲罰幾回合(任務)
				penalty_undergoing = True
				path_done = True  # 跳出WHILE並重算
				continue

			# ==懲罰回合已到==
			if penalty_timer == 0:
				lift_penalty(penalty_action, penalty_cost, action_list)  # 解除COST上加諸的懲罰
				penalty_undergoing = False

			# ==MAIN判定刪除當下動作(任務?)==
			if mymain.kill_mission is True:
				kill_action(path[0], action_list)
				path_done = True  # 跳出WHILE並重算
				continue

			# ==時間到則終止動作回家(只進入一次以免影響回家的陸續動作)==
			if mymain.time >= go_home_time and go_home_flag is False:
				path_done = True  # 跳出WHILE並重算
				go_home_flag = True
				continue

			# ==若PATH還沒做完==
			if len(path) != 0:
				top_path = path[0]

				# ==小雞應該不會使用到==
				if top_path.mode == 2:
					top_path.tangent_point_calculation(my_pos, 3)

				# ==餵動作裡的子動作==
				while len(top_path.child_action) != 0:
					top_child = top_path.child_action[0]
					mymain.output_name = top_child.name
					mymain.output_degree = top_child.degree
					mymain.output_speed = top_child.speed
					mymain.output_mode = top_child.mode
					mymain.output_position = top_child.position
					mymain.output_wait = top_child.wait
					mymain.output = output_processor_small_chicken(top_child)
					print('-> ' + mymain.output_name + str(mymain.output_position))

					# ==子動作做完==
					if mymain.action_done is True and mymain.output_child_name == mymain.child_name:
						top_path.child_action.remove(top_child)

				current_world_state = top_path.result_world_state  # update current world state status
				kill_action(top_path, action_list)  # remove (specific) finished actions
				path.remove(top_path)  # feed out the next action in path

			# ==PATH做完==
			else:
				if penalty_undergoing is True:  # PENALTY回合減1
					penalty_timer -= 1
				path_done = True




if __name__ == "__main__":
	goap_server()


