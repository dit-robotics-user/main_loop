from setting import *

action_done = True  # <----
my_pos = (3, 3)  # <----

demo_path = []
demo_path = setting(1, demo_path)
output = 0
goal = []
path_done = False

while len(demo_path) > 0:

    path_done = False
    path = demo_path[0]
    if path.iscup is True:
        if path.grab_mode is 2:  # speed mode
            path.tangent_point_calculation(my_pos, stretch_factor=5)

    while path_done is False:
        if action_done is False:
            output = path
        else:
            print(path.name)
            print(path.position)
            print(path.degree)
            print()
            path_done = True
            demo_path.remove(demo_path[0])


