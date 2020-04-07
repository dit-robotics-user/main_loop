from goap import *
import copy
'''
my_pos = (30, 30)
my_degree = 0
action_list = []
current_world_state = []
mission_list = []
action_list, current_world_state, mission_list = setting(1, action_list, current_world_state, mission_list, my_pos, my_degree)


def action_done(path):
    number = 0
    for action in action_list:
        if action.name is path[-1]:
            if action.iscup is False:
                continue
            number = action.number
    for i in range(len(action_list)-1, -1, -1):
        if action_list[i].number is number:
            action_list.remove(action_list[i])

while len(current_world_state) is not 0:
    for actions in action_list:
        actions.refresh()
    mission_list = calculate_mission_priority(mission_list, current_world_state)
    #new_current_world_state = current_world_state.copy()
    p = goap([mission_list[0].name], current_world_state, my_pos, action_list)
    print('cws:')
    print(current_world_state)
    action_done(p)
    print()'''
'''for actions in action_list:
        print(actions.name)'''

'''



def replan_path(action_name, a_list):
    penalty = 500
    for action in a_list:
        if action.name is action_name:
            action.priority += penalty


def kill_mission(mission_name, m_list):
    for mission in m_list:
        if mission.name is mission_name:
            m_list.remove(mission)


def kill_same_action_number(path, a_list):
    number = 0
    for action in a_list:
        if action.name is path[-1]:
            number = action.number
    for i in range(len(a_list) - 1, -1, -1):
        if a_list[i].number is number:
            a_list.remove(a_list[i])


action_list = []
current_world_state = []
mission_list = []
action_done = True  #< - STM
replan = False      #< - MAIN
my_pos = (3, 3)     #< - MAIN
my_degree = 0
kill_cup = False    #< - MAIN < - CAMERA
setting_mode = 1    #< - MAIN
action_list, current_world_state, mission_list = setting(setting_mode, action_list, current_world_state, mission_list, my_pos, my_degree)
while True:
    path_done = False

    #if kill_cup:

    for actions in action_list:
        actions.refresh()
    mission_list = calculate_mission_priority(mission_list, current_world_state)
    new_current_world_state = current_world_state.copy()
    path = goap([mission_list[0].name], new_current_world_state, my_pos, action_list)
    goal = path[-1]

    while path_done is False:
        if replan is True:  # means that that cup is currently occupied?
            replan_path(path[0], action_list)
            path_done = True
            continue
        if action_done is True:
            if len(path) > 1:
                path.pop()
            else:
                current_world_state = new_current_world_state
                if goal is 'lighthouse':
                    kill_mission('lighthouse', action_list)
                if goal is 'wind':
                    kill_mission('wind', action_list)
                path_done = True
                kill_same_action_number(path, action_list)
                continue
            output = path[0]
'''


def replan_path(action_name, a_list):
    penalty = 500
    for action in a_list:
        if action.name is action_name:
            action.priority += penalty


def kill_mission(mission_name, m_list):
    for mission in m_list:
        if mission.name is mission_name:
            m_list.remove(mission)


def kill_same_action_number(goal, a_list):
    number = 0
    for action in a_list:
        if action.name is goal:
            if action.iscup is False:
                continue
            number = action.number
    for i in range(len(a_list) - 1, -1, -1):
        if a_list[i].number is number:
            a_list.remove(a_list[i])


def name_to_action(name, a_list):
    for a in a_list:
        if a.name is name:
            return a




replan = False
action_done = True
my_pos = (3, 3)
my_degree = 0

left_side = 0
right_side = 0
lift_left_deleted = False
lift_right_deleted = False
action_list = []
current_world_state = []
mission_list = []
action_list, current_world_state, mission_list = setting(1, action_list, current_world_state, mission_list, my_pos, my_degree)
dont_change_action_list = action_list.copy()
output = 0
goal = []
path_done = False

while 1:
    path_done = False
    for actions in action_list:
        actions.refresh()
    new_current_world_state = current_world_state.copy()
    mission_list = calculate_mission_priority(mission_list, new_current_world_state, left_side, right_side, my_pos)
    path = goap([mission_list[0].name], new_current_world_state, my_pos, action_list)
    if len(path) > 0:
        goal = path[-1]
    else:
        print('no path found')
        continue
    print('cws:')
    print(new_current_world_state)
    print()
    #action_done(p)

    while path_done is False:
        if replan is True:  # means that that cup is currently occupied?
            replan_path(path[0], action_list)
            path_done = True
            continue

        if action_done is False:
            output = name_to_action(path[0], dont_change_action_list)
        else:
            if len(path) > 1:
                path.pop(0)
                output = name_to_action(path[0], dont_change_action_list)
            else:
                current_world_state = new_current_world_state
                if goal is 'lighthouse':
                    kill_mission('lighthouse', action_list)
                elif goal is 'wind':
                    kill_mission('wind', action_list)
                elif goal is 'lift_left':
                    left_side += 1
                    if left_side >= 3:
                        for action in action_list:
                            if action.name is 'lift_left':
                                action_list.remove(action)
                                lift_left_deleted = True
                elif goal is 'lift_right':
                    right_side += 1
                    if right_side >= 3:
                        for action in action_list:
                            if action.name is 'lift_right':
                                action_list.remove(action)
                                lift_right_deleted = True
                elif goal is 'put_cup':
                    if lift_left_deleted is True:
                        append_list(dont_change_action_list, 'lift_left', action_list)
                    if lift_right_deleted is True:
                        append_list(dont_change_action_list, 'lift_right', action_list)
                path_done = True
                kill_same_action_number(goal, action_list)
                continue
