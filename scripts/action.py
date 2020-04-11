import math


def calculate_unit_vector(start, goal):  # give two points and calculates the unit vector
    x = goal[0] - start[0]
    y = goal[1] - start[1]
    length = math.sqrt(x*x + y*y)
    unit_vector = (x/length, y/length)
    return unit_vector


def calculate_degree(start, goal):  # from vector then calculates the degree from 0-360
    num = goal[1] - start[1]
    den = goal[0] - start[0]
    pi = 3.1415926
    radius = 0
    if num != 0 and den != 0:
        if num > 0 and den > 0:  # 1 quadrant
            radius = math.atan(num / den)
        elif num > 0:  # 2, 3 quadrant
            radius = pi + math.atan(num / den)
        else:  # 4 quadrant
            radius = 2 * pi + math.atan(num / den)
    elif num == 0 and den > 0:
        radius = 0  # slope is zero
    elif num == 0 and den < 0:
        radius = pi  # slope is zero
    elif num > 0 and den == 0:
        radius = pi / 2  # slope is infinity
    elif num < 0 and den == 0:
        radius = 3 * pi / 2  # slope is infinity
    else:
        '''print('start:')
        print(start)
        print('goal')
        print(goal)
        print('error')'''
    degree = int(radius * 180 / pi)  # switches radius to degree
    #print('degree:', degree)
    return degree


class Action:
    def __init__(self, name, preconditions, effects, position, cost, degree, speed, mode, speed_mode_radius, number, iscup, iswait):
        self.name = name
        self.preconditions = preconditions
        self.effects = effects
        self.path = []
        self.priority = 0
        self.required_world_state = []
        self.current_world_state = []
        self.cost = cost
        self.position = position
        self.iswait = iswait
        self.degree = degree
        self.number = number
        self.grab_mode = mode  # speed mode or position mode
        self.radius = speed_mode_radius
        self.speed = speed
        self.circular_position = []
        self.circular_angle = []
        self.iscup = iscup

    def __gt__(self, other):
        return self.priority > other.priority

    def print_stuff(self):
        print("Name: " + self.name)
        print(self.preconditions)
        print(self.effects)

    def calculate_priority(self, my_pos):
        self.priority = len(self.required_world_state) + self.cost + math.sqrt(((my_pos[0]-self.position[0])
                        * (my_pos[0]-self.position[0]) + (my_pos[1]-self.position[1])*(my_pos[1]-self.position[1])))
        '''print('----')
        print(len(self.required_world_state))
        print(self.cost)
        print(((my_pos[0]-self.position[0])*(my_pos[0]-self.position[0]) +
               (my_pos[1]-self.position[1])*(my_pos[1]-self.position[1])))
        print('----')'''

    def world_state_change(self, cws, rws):
        for precondition in self.preconditions:
            if precondition not in rws:
                rws.append(precondition)
        for state in cws:
            if state in rws:
                rws.remove(state)
        for effect in self.effects:
            if effect in rws:
                rws.remove(effect)
        self.required_world_state = rws

    def refresh(self):
        self.path = []
        self.priority = 0
        self.required_world_state = []
        self.current_world_state = []

    def tangent_point_calculation(self, my_pos, stretch_factor):  # is_right should be included in cup actions
        cx = self.position[0]
        cy = self.position[1]
        x = my_pos[0]
        y = my_pos[1]
        r = self.radius
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

        degree1 = calculate_degree(my_pos, pos1)
        degree2 = calculate_degree(my_pos, pos2)

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