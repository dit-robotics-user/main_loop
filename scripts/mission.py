# coding=utf-8
import math


def distance(pos1, pos2):  # calculates exact distance
    x = math.sqrt((pos2[0] - pos1[0]) * (pos2[0] - pos1[0]) + (pos2[1] - pos1[1]) * (pos2[1] - pos1[1]))
    return x


class Mission:
    def __init__(self, name, cost, priority_function_number):
        self.name = name
        self.cost = cost
        self.priority = 0
        self.priority_function_number = priority_function_number

    def __gt__(self, other):
        return self.priority > other.priority

    def calculate_priority(self, time, go_home_time):
        if self.priority_function_number is 0:  # get_points
        # 沒杯子就拿杯子
            if time > go_home_time:
                self.priority = 20000
            else:
                self.priority = 0
        elif self.priority_function_number is 1:  # go_home
        # 有杯子就放杯子
            if time > go_home_time:
                self.priority = 0
            else:
                self.priority = 20000
        else:
            print('priority function number invalid')

        '''# ===感覺撞燈塔跟推風標都應該放在put_cut, get_cup中..===
        elif self.priority_function_number is 2:  # activate lighthouse
        # 近就撞燈塔

        elif self.priority_function_number is 3:  # push windsocks
        # 近就推風標'''
