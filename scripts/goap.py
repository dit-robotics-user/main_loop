from Queue import PriorityQueue

from setting import *
import copy
import math


'''def get_goal_successors(required_states, action_list):  # linking actions by examining there effects
    successors = []
    for action in action_list:
        for state in required_states:
            if state in action.effects:
                copy_action = copy.deepcopy(action)
                successors.append(copy_action)
    return successors'''


def get_successors(required_states, action_list):  # linking actions by examining there effects
    successors = []
    for action in action_list:
        is_append = True
        for state in required_states:
            if state not in action.effects:
                is_append = False
        if is_append is True:
            copy_action = copy.deepcopy(action)
            successors.append(copy_action)
    return successors


def calculate_successor_priority(top, successor):
    magic_number = 20
    cost_of_cup_movement = 10
    if successor.name == 'cup_prepare':
        successor.priority = top.priority + cost_of_cup_movement + magic_number
    else:
        successor.priority = top.priority + distance(top.position, successor.position) + magic_number


def change_current_world_state(path, state, goal, action_list):  # changing the world state when the action is done
    for action in action_list:
        if action.name == path[-1]:
            for eff in action.effects:
                if eff not in state and eff != goal[0]:
                    state.append(eff)


def distance(pos1, pos2):  # calculates exact distance
    x = math.sqrt((pos2[0]-pos1[0])*(pos2[0]-pos1[0]) + (pos2[1]-pos1[1])*(pos2[1]-pos1[1]))
    return x


def output_path_processor(goal, output_path, cws):
    passed_on_req = copy.deepcopy(cws)
    for i in range(0, len(output_path), 1):
        output_path[i].result_world_state = passed_on_req
        for effect in output_path[i].effects:
            if effect != goal[0] and effect not in passed_on_req:
                output_path[i].result_world_state.append(effect)
        for precondition in output_path[i].preconditions:
            if precondition in passed_on_req:
                output_path[i].result_world_state.remove(precondition)
        passed_on_req = copy.deepcopy(output_path[i].result_world_state)


def dominant_processor(output_path, dominant_name):
    dominant_pos = (0, 0)
    dominant_degree = 0
    dominant_change = False
    shift_rate = 150
    for child in output_path:
        if child.name[:len(dominant_name)] == dominant_name:
            dominant_pos = child.position
            dominant_degree = child.degree
            dominant_change = True
    if dominant_change == True:
        for child in output_path:
            if child.name[:len(dominant_name)] != dominant_name:
                if child.name == 'cup_prepare':
                    child.position = (dominant_pos[0] - shift_rate*int(math.cos(dominant_degree*3.1415926535/180)), dominant_pos[1]
                                      - shift_rate*int(math.sin(dominant_degree*3.1415926535/180)))
                    child.degree = dominant_degree
                elif child.name == 'cup_hold':
                    child.position = (dominant_pos[0] - shift_rate*int(math.cos(dominant_degree*3.1415926535/180)), dominant_pos[1]
                                      - shift_rate*int(math.sin(dominant_degree*3.1415926535/180)))

                    child.degree = dominant_degree
                else:
                    child.position = dominant_pos
                    child.degree = dominant_degree



def child_action_processor(output_path):
    for path in output_path:
        new_child_action_list = []
        for child_action in path.child_action:
            child_action.position = path.position
            child_action.degree = path.degree
            new_child_action_list.append(copy.deepcopy(child_action))
        path.child_action = new_child_action_list


def goap(goal, cws, my_pos, action_list):
    goal_expands = []
    available_paths = []
    p_queue = PriorityQueue()
    search_done = False

    for successor in get_successors(goal, action_list):
        successor.req_world_state_change(cws, goal, goal)
        successor.calculate_priority(my_pos)
        if len(successor.required_world_state) == 0:
            successor.path.append(successor)
            available_paths.append(successor)
        else:
            goal_expands.append(successor)

    for expand in goal_expands:
        p_queue.put((expand.priority, expand))

    while search_done is False:
        if p_queue.empty() is True:
            search_done = True
            continue
        top = copy.deepcopy(p_queue.get()[1])
        top.path.append(top)
        for successor in get_successors(top.required_world_state, action_list):
            successor.req_world_state_change(cws, top.required_world_state, goal)
            calculate_successor_priority(top, successor)
            successor.path = top.path
            '''if 'stop' in successor.required_world_state:
                successor.path.append(successor)
                successor.path.reverse()
                available_paths.append(successor)'''
            if len(successor.required_world_state) == 0:
                successor.path.append(successor)
                successor.path.reverse()
                available_paths.append(successor)
                search_done = True
            else:
                p_queue.put((successor.priority, successor))
        '''if p_queue.empty() is True:
            search_done = True'''

    if len(available_paths) > 0:
        available_paths.sort()
        output_path = available_paths[0].path
        output_path_processor(goal, output_path, cws)

        dominant_processor(output_path, 'cup_get')

        for child in output_path:
            print(child.name)
            print(child.result_world_state)
        print('====================')

        child_action_processor(output_path)
        '''for child in output_path:
            for child_action in child.child_action:
                child_action.position = child.position
                child_action.degree = child.degree'''

        return output_path
    else:
        print('cant find path')
        null = []
        return null

