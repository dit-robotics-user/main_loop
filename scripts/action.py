from cup import *
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

    def tangent_point_calculation(self, my_pos, stretch_factor):  # is_right should be included in cup actions
        cx = self.position[0]
        cy = self.position[1]
        x = my_pos[0]
        y = my_pos[1]
        r = self.speed_mode_radius
        is_right = False
        if (cx - x)*(cx - x) + (cy - y)*(cy - y) <= r*r:
            print('error input, my pos is inside/on the radius')
            return 0
        #  am^2 + bm + c = 0
        a = cx*cx - 2*cx*x + x*x - r*r
        b = 2*cx*y - 2*cx*cy - 2*x*y + 2*x*cy
        c = y*y - 2*cy*y + cy*cy - r*r
        if a != 0 and c != 0:
            m1 = (-b + math.sqrt(b*b - 4*a*c))/2/a  # solve root 1
            if m1 != 0:
                x1 = (m1 * m1 * x - y * m1 + cx + cy * m1) / (m1 * m1 + 1)
                y1 = m1 * (x1 - x) + y
            else:
                x1 = cx
                y1 = y
            m2 = (-b - math.sqrt(b*b - 4*a*c))/2/a  # solve root 2
            if m2 != 0:
                x2 = (m2 * m2 * x - y * m2 + cx + cy * m2) / (m2 * m2 + 1)
                y2 = m2 * (x2 - x) + y
            else:
                x2 = cx
                y2 = y
        elif a == 0 and c != 0:
            m1 = -c/b
            x1 = (m1 * m1 * x - y * m1 + cx + cy * m1) / (m1 * m1 + 1)
            y1 = m1 * (x1 - x) + y
            x2 = x
            y2 = cy
        elif a == 0 and c == 0:
            x1 = cx
            y1 = y
            x2 = x
            y2 = cy
        else:
            m1 = -b/a
            x1 = (m1 * m1 * x - y * m1 + cx + cy * m1) / (m1 * m1 + 1)
            y1 = m1 * (x1 - x) + y
            x2 = x
            y2 = cy
        pos1 = (x1, y1)
        pos2 = (x2, y2)

        print('pos1:')
        print(pos1)
        degree1 = calculate_degree(my_pos, pos1)
        print('pos2:')
        print(pos2)
        degree2 = calculate_degree(my_pos, pos2)
        print('pos_cen:')
        print(self.position)

        center_degree = calculate_degree(my_pos, self.position)

        # determines using left or right claw
        if '1' in self.preconditions or '2' in self.preconditions:
            is_right = True
        if is_right is True:  # use right claw
            self.degree = min(degree1, degree2, center_degree)
            # extend the goal along vector
            if self.degree == degree1:
                self.position = (pos1[0] + stretch_factor * calculate_unit_vector(my_pos, pos1)[0], pos1[1] +
                                 stretch_factor * calculate_unit_vector(my_pos, pos1)[1])
            else:
                self.position = (pos2[0] + stretch_factor * calculate_unit_vector(my_pos, pos2)[0], pos2[1] +
                                 stretch_factor * calculate_unit_vector(my_pos, pos2)[1])
        else:  # use left claw
            self.degree = max(degree1, degree2, center_degree)
            # extend the goal along vector
            if self.degree == degree1:
                self.position = (pos1[0] + stretch_factor * calculate_unit_vector(my_pos, pos1)[0], pos1[1] +
                                 stretch_factor * calculate_unit_vector(my_pos, pos1)[1])
            else:
                self.position = (pos2[0] + stretch_factor * calculate_unit_vector(my_pos, pos2)[0], pos2[1] +
                                 stretch_factor * calculate_unit_vector(my_pos, pos2)[1])
