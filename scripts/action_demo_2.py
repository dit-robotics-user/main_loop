#!/usr/bin/env python
import math
import copy


class Action:
    def __init__(self, name, preconditions, effects, child_action, position, degree, speed, cost, mode, speed_mode_radius, type_number, cup_number, wait):
        self.name = name
        self.preconditions = preconditions
        self.effects = effects
        self.child_action = child_action
        self.cost = cost
        self.position = position
        self.wait = wait
        self.degree = degree
        self.type_number = type_number
        self.mode = mode  # speed mode or position mode
        self.speed_mode_radius = speed_mode_radius
        self.speed = speed
        self.cup_number = cup_number
        self.tangent_position = []
        self.tangent_angle = []
        self.path = []
        self.priority = 0
        self.required_world_state = []
        self.result_world_state = []

    def __gt__(self, other):
        return self.priority > other.priority

    def print_stuff(self):
        print("Name: " + self.name)
        print(self.preconditions)
        print(self.effects)

    def calculate_priority(self, my_pos):
        if self.name == 'cup_hold':
            self.priority = 0  # though priority here is 0, but after calculating the leftover actions, priority will be compensated
        else:
            self.priority = len(self.required_world_state) + self.cost + math.sqrt(((my_pos[0]-self.position[0])
                            * (my_pos[0]-self.position[0]) + (my_pos[1]-self.position[1])*(my_pos[1]-self.position[1])))
            #print(self.priority)
        '''print('----')
        print(len(self.required_world_state))
        print(self.cost)
        print(((my_pos[0]-self.position[0])*(my_pos[0]-self.position[0]) +
               (my_pos[1]-self.position[1])*(my_pos[1]-self.position[1])))
        print('----')'''

    def req_world_state_change(self, cws, former_required_ws, goal):  # renews required world state
        self.required_world_state = copy.deepcopy(former_required_ws)
        for precondition in self.preconditions:
            if precondition not in self.required_world_state:
                self.required_world_state.append(precondition)
        for state in cws:
            if state in self.required_world_state:
                self.required_world_state.remove(state)
        for effect in self.effects:
            if effect in self.required_world_state:
                self.required_world_state.remove(effect)
        if goal[0] in self.required_world_state:
            self.required_world_state.remove(goal[0])

        '''copy_rws = former_required_ws.copy()
        copy_cws = cws.copy()
        for effect in self.effects:
            if effect in copy_rws:
                copy_rws.remove(effect)
        for precondition in self.preconditions:
            if precondition not in copy_rws:
                copy_rws.append(precondition)
        for state in copy_cws:
            if state in copy_rws:
                copy_rws.remove(state)
        self.required_world_state = copy_rws'''

    def refresh(self):
        self.path = []
        self.priority = 0
        self.required_world_state = []
        self.result_world_state = []
