#!/usr/bin/env python
# coding=utf-8
from Queue import PriorityQueue
from setting import *
import copy
import math


# ==給出可連接的所有動作==
def get_successors(required_states, action_list):  # linking actions by examining there effects
    successors = []
    for action in action_list:
        is_append = True
        for state in required_states:
            if state not in action.effects:  # must satisfy all required world states
                is_append = False
        if is_append is True:
            copy_action = copy.deepcopy(action)
            successors.append(copy_action)
    return successors


# ==計算動作的優先順序==
def calculate_successor_priority(top, successor):
    magic_number = 300  # value due to distance and cost.. found by trial and error
    cost_of_cup_movement = 10  # compensating value for not taking the distance to cups setup pos into account
    if successor.name == 'cup_prepare':
        successor.priority = top.priority + cost_of_cup_movement + magic_number
    else:
        successor.priority = top.priority + distance(top.position, successor.position) + magic_number


# ==算兩點距離==
def distance(pos1, pos2):  # calculates exact distance of two points
    x = math.sqrt((pos2[0]-pos1[0])*(pos2[0]-pos1[0]) + (pos2[1]-pos1[1])*(pos2[1]-pos1[1]))
    return x


# ==將每個動作的RESULT WORLD STATE更新上==
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
    for child in output_path:
        if child.name[:len(dominant_name)] == dominant_name:
            dominant_pos = child.position
            dominant_degree = child.degree
            dominant_change = True
    if dominant_change == True:
        for child in output_path:
            if child.name[:len(dominant_name)] != dominant_name:
                if child.name == 'cup_prepare':
                    child.position = (dominant_pos[0] - 20*int(math.cos(dominant_degree*3.1415926535/180)), dominant_pos[1]
                                      - 20*int(math.sin(dominant_degree*3.1415926535/180)))
                    child.degree = dominant_degree
                elif child.name == 'cup_hold':
                    child.position = (dominant_pos[0] - 20*int(math.cos(dominant_degree*3.1415926535/180)), dominant_pos[1]
                                      - 20*int(math.sin(dominant_degree*3.1415926535/180)))
                    child.degree = dominant_degree
                else:
                    child.position = dominant_pos
                    child.degree = dominant_degree


def child_action_processor(output_path, action_list):
    for path in output_path:
        new_child_action_list = []
        pos = list(copy.deepcopy(path.position))
        counter = 0
        for child_action in path.child_action:
            if path.name == 'put_cups':
                if child_action.name == 'goto':
                    pos[0] += 100
                    counter += 1
                    path.position = tuple(pos)
                    for action in action_list:
                        if action.name == 'put_cups':
                            action.position = path.position
            child_action.position = path.position
            child_action.degree = path.degree
            new_child_action_list.append(copy.deepcopy(child_action))
        path.child_action = new_child_action_list


def put_cup_processor(put_cup_action, cws, child_list, current_left_layer, current_right_layer):
    cll = current_left_layer
    crl = current_right_layer
    lower_left = False
    lower_right = False
    copy_child_list = copy.deepcopy(child_list)
    if ('1' not in cws and '2' not in cws) or ('3' not in cws and '4' not in cws):
        if '1' not in cws and '2' not in cws:
            if cll < 3:
                lower_left = True
                cll += 1
        if '3' not in cws and '4' not in cws:
            if crl < 3:
                lower_right = True
                crl += 1
        if lower_left is True and lower_right is True:
            put_cup_action.child_action.append(copy_child_list[22])
        elif lower_left is True and lower_right is False:
            put_cup_action.child_action.append(copy_child_list[20])  # lower left
        elif lower_left is False and lower_right is True:
            put_cup_action.child_action.append(copy_child_list[21])  # lower right
        else:
            print('something weird in put cup child action')
        lower_left = False
        lower_right = False

    put_cup_action.child_action.append(copy_child_list[13])  # open all
    put_cup_action.child_action.append(copy_child_list[17])  # goto
    while cll < 3 or crl < 3:
        if cll < 3:
            lower_left = True
            cll += 1
        if crl < 3:
            lower_right = True
            crl += 1
        if lower_left is True and lower_right is True:
            put_cup_action.child_action.append(copy_child_list[22])
        elif lower_left is True and lower_right is False:
            put_cup_action.child_action.append(copy_child_list[20])  # lower left
        elif lower_left is False and lower_right is True:
            put_cup_action.child_action.append(copy_child_list[21])  # lower right
        else:
            print('something weird in put cup child action')
        put_cup_action.child_action.append(copy_child_list[13])  # open all
        put_cup_action.child_action.append(copy_child_list[17])  # goto
        lower_left = False
        lower_right = False


def goap(goal, cws, my_pos, action_list, child_list, current_left_layer, current_right_layer):
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
        for object in available_paths:
            print(object.name + '-->' + str(object.priority))
        output_path = available_paths[0].path
        output_path_processor(goal, output_path, cws)

        for act in output_path:
            if act.name == 'put_cups':
                put_cup_processor(act, cws, child_list, current_left_layer, current_right_layer)

        dominant_processor(output_path, 'cup_get')

        for child in output_path:
            print(child.name)
            print(child.result_world_state)
        print('====================')

        child_action_processor(output_path, action_list)

        return output_path
    else:
        print('cant find path')
        null = []
        return null

