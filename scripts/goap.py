#!/usr/bin/env python
from Queue import PriorityQueue
from setting import *
import copy


def get_successors_goal(goal, action_list):
    successors = []
    for action in action_list:
        for g in goal:
            if g in action.effects:
                successors.append(action)
    return successors


def get_successors(required_states, action_list):
    successors = []
    for action in action_list:
        for state in required_states:
            if state in action.effects:
                successors.append(action)

    return successors


def change_current_world_state(path, state, goal, action_list):
    for action in action_list:
        if action.name == path[-1]:
            for eff in action.effects:
                if eff not in state and eff != goal[0]:
                    state.append(eff)


def distance(pos1, pos2):
    x = math.sqrt((pos2[0]-pos1[0])*(pos2[0]-pos1[0]) + (pos2[1]-pos1[1])*(pos2[1]-pos1[1]))
    return x


def goap(goal, cws, my_pos, a_list):
    temp_cws = cws.copy()
    print('goal:')
    print(goal)
    goal_expands = []
    available_paths = []
    for successor in get_successors_goal(goal, a_list):
        copy_goal = goal.copy()
        successor.world_state_change(temp_cws, copy_goal)
        successor.calculate_priority(my_pos)
        if len(successor.required_world_state) == 0:
            successor.path.append(successor.name)
            #change_current_world_state(successor.path, cws, goal, a_list)
            # print(successor.priority)
            available_paths.append(successor)
        else:
            goal_expands.append(successor)
    for expands in goal_expands:
        small_queue = PriorityQueue()
        small_queue.put((expands.priority, expands))
        while small_queue.empty() is False:
            top = small_queue.get()[1]
            top.path.append(top.name)
            for successor in get_successors(top.required_world_state, a_list):
                copy_top_rws = top.required_world_state.copy()
                successor.world_state_change(temp_cws, copy_top_rws)
                successor.priority = top.priority + distance(successor.position, top.position)
                successor.path = top.path
                if len(successor.required_world_state) == 0:
                    successor.path.append(successor.name)
                    successor.path.reverse()
                    #change_current_world_state(successor.path, cws, goal, a_list)
                    output_successor = copy.deepcopy(successor)  # deepcopy is done to prevent change of path from above
                    available_paths.append(output_successor)
                else:
                    small_queue.put((successor.priority, successor))

    if len(available_paths) > 0:
        available_paths.sort()
        print('path:')
        print(available_paths[0].path)

        for a in a_list:
            if a.name is available_paths[0].path[-1]:
                last_action = a
                break
        for eff in last_action.effects:
            if eff in cws:
                cws.remove(eff)
                #print("remove " + eff)
            elif eff not in cws and eff != goal[0]:
                cws.append(eff)
                #print("add " + eff)
            if available_paths[0].iscup is True:
                for pre in last_action.preconditions:
                    if pre in cws:
                        cws.remove(pre)
        return available_paths[0].path
    else:
        print('cant find path')
        null = []
        return null
