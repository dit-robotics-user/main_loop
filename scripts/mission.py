#!/usr/bin/env python
class Mission:
    def __init__(self, name, cost, priority_function_number):
        self.name = name
        self.cost = cost
        self.priority = 0
        self.priority_function_number = priority_function_number

    def __gt__(self, other):
        return self.priority > other.priority

    def calculate_priority(self, cws, left_claw, right_claw, my_pos):
        if self.priority_function_number is 0:
            self.priority = 1  # should be a heuristic function here
        elif self.priority_function_number is 1:
            if '1' not in cws and '2' not in cws:
                self.priority = 0
            else:
                self.priority = 2
            #self.priority = 2
        elif self.priority_function_number is 2:
            if '3' not in cws and '4' not in cws:
                self.priority = 0
            else:
                self.priority = 2
            #self.priority = 2
        elif self.priority_function_number is 3:
            if left_claw is 3 and right_claw is 3:
                self.priority = 0
            else:
                self.priority = 3
        else:
            print('calculate priority error')
