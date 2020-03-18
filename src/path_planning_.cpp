#include "ros/ros.h"
#include "std_msgs/String.h"
#include "main_loop/path.h"
#include "main_loop/AddTwoInts.h"
#include <cstdlib>

#include <iostream>
#include <queue>
#include <vector>
#include <cmath>

using namespace std;

#define mazeSizeX 41
#define mazeSizeY 61
#define robotSize 9

void build_maze(int maze[mazeSizeX][mazeSizeY])   //build the maze
{
    // maze field
    for(int j = 0; j < mazeSizeY; j++)
    {
        for(int i = 0; i < mazeSizeX; i++)
        {
            maze[i][j] = 1;
        }
    }
    // adding walls
    // top and bottom walls
    for(int i = 0; i < mazeSizeX; i++)
    {
        maze[i][0] = 0;
        maze[i][1] = 0;
        maze[i][2] = 0;
        maze[i][mazeSizeY-1] = 0;
        maze[i][mazeSizeY-2] = 0;
        maze[i][mazeSizeY-3] = 0;
    }
    //left and right walls
    for(int i = 0; i < mazeSizeY; i++)
    {
        maze[0][i] = 0;
        maze[1][i] = 0;
        maze[2][i] = 0;
        maze[mazeSizeX-1][i] = 0;
        maze[mazeSizeX-2][i] = 0;
        maze[mazeSizeX-3][i] = 0;
    }
}

void build_obstacles(int middlePoint[2], int wallThick, int maze[mazeSizeX][mazeSizeY], int weight){
    int middlePointX = middlePoint[0];
    int middlePointY = middlePoint[1];
    int halfWallThick = (wallThick-1)/2;
    for(int j = middlePointY-halfWallThick; j <= middlePointY+halfWallThick; j++)
    {
        for(int i = middlePointX-halfWallThick; i <= middlePointX + halfWallThick; i++)
        {
            if(i>0&&i<mazeSizeX&&j>0&&j<mazeSizeY)
            {
                maze[i][j] = weight;
            }
        }
    }
}

class PosNode   //used to store path;
{
public:
    int pos[2];
};

class Node
{
public:
    Node()
    {
        g = 0;
        h = 0;
        f = 0;
    }
    double g;
    double h;
    double f;
    double move_cost;
    int priority_stamp;
    vector<PosNode> path;

    int PosX()
    {
        return pos[0];
    }
    int PosY()
    {
        return pos[1];
    }
    void PrintPos()
    {
        cout << "Pos: (" << pos[0] << ", " << pos[1] << "\n";
    }
    void GivePos(int new_pos[2])
    {
        pos[0] = new_pos[0];
        pos[1] = new_pos[1];
    }
private:
    int pos[2];
};

void Up(int* new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[1] -= 1;
}
void UpRight(int* new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[0] += 1;
    new_pos[1] -= 1;
}
void Right(int* new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[0] += 1;
}
void DownRight(int* new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[0] += 1;
    new_pos[1] += 1;
}
void Down(int* new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[1] += 1;
}
void DownLeft(int* new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[0] -= 1;
    new_pos[1] += 1;
}
void Left(int*new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[0] -= 1;
}
void UpLeft(int* new_pos, int pos[2])
{
    new_pos[0] = pos[0];
    new_pos[1] = pos[1];
    new_pos[0] -= 1;
    new_pos[1] -= 1;
}

bool isValid(int new_pos[2], int maze[mazeSizeX][mazeSizeY])
{
    if(new_pos[0] <=0 || new_pos[0]>= mazeSizeX-1)
    {
        return false;
    }
    if(new_pos[1] <=0 || new_pos[1]>= mazeSizeY-1)
    {
        return false;
    }
    if(maze[new_pos[0]][new_pos[1]] == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

vector<Node> FindSuccessors(int pos[2], int maze[mazeSizeX][mazeSizeY])
{
    double strait = 1;
    double diagnal = sqrt(2);
    vector<Node> output;
    Node successor;
    int new_pos[2];
    Up(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = strait;
        output.push_back(successor);
    }
    UpRight(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = diagnal;
        output.push_back(successor);
    }
    Right(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = strait;
        output.push_back(successor);
    }
    DownRight(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = diagnal;
        output.push_back(successor);
    }
    Down(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = strait;
        output.push_back(successor);
    }
    DownLeft(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = diagnal;
        output.push_back(successor);
    }
    Left(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = strait;
        output.push_back(successor);
    }
    UpLeft(new_pos, pos);
    if(isValid(new_pos, maze)==true)
    {
        successor.GivePos(new_pos);
        successor.move_cost = diagnal;
        output.push_back(successor);
    }
    return output;
}

double HeuristicFunctionManhattan(Node start, Node goal)
{
    double cost = abs(start.PosX() - goal.PosX()) + abs(start.PosY() - goal.PosY());
    return cost;
}

double HeuristicFunctionDiagnal(Node start, Node goal)
{
    int strait = 1;
    double diagnal = sqrt(2);
    int dx = abs(start.PosX()-goal.PosX());
    int dy = abs(start.PosY()-goal.PosY());
    double cost = strait*(dx + dy) + (diagnal - 2*strait)*min(dx, dy);
    return cost;
}

void PrintPath(vector<PosNode> p, int maze[mazeSizeX][mazeSizeY])
{
    cout << "path: ";
    for(unsigned int i = 0; i < p.size(); i++)
    {
        cout << "(" << p[i].pos[0] << "," << p[i].pos[1] << ") ";
        maze[p[i].pos[0]][p[i].pos[1]] = 8;  //print out path as 8

    }
    cout << "\n";
}

void PrintMaze(int start[2], int goal[2], int maze[mazeSizeX][mazeSizeY])      // print out maze
{
    cout << "\n";
    maze[start[0]][start[1]] = 5;  //set maze start point as 5
    maze[goal[0]][goal[1]] = 9;  //set maze goal point as 9
    char cmaze[mazeSizeX][mazeSizeY];
    //print out the maze
    for(int j = mazeSizeY-1; j >= 0; j--)
    {
        for(int i = 0; i <= mazeSizeX-1 ; i++)
        {
            if(maze[i][j] == 0)
            {
                cmaze[i][j] = 'H';
            }
            else if(maze[i][j] == 8)
            {
                cmaze[i][j] = '#';
            }
            else if(maze[i][j] == 4)
            {
                cmaze[i][j] = '~';
            }
            else if(maze[i][j] == 1)
            {
                cmaze[i][j] = '.';
            }
            else if(maze[i][j] == 5)
            {
                cmaze[i][j] = 'S';
            }
            else if(maze[i][j] == 9)
            {
                cmaze[i][j] = 'G';
            }
            else{
                cmaze[i][j] = '%';
            }
            cmaze[40][60] = '@';
            cmaze[0][0] = '@';
            cout << cmaze[i][j];
            cout << " ";
        }
        cout << "\n";
    }
}



bool operator<(Node const& n1, Node const& n2)
{
    if(abs(n1.f-n2.f)<0.000001){
        return n1.priority_stamp > n2.priority_stamp;
    }
    return n1.f > n2.f;
}



vector<PosNode> AStar(int start_pos[2], int goal_pos[2], int maze[mazeSizeX][mazeSizeY])
{
    int priority_count = 0;
    Node start, goal;
    start.GivePos(start_pos);
    goal.GivePos(goal_pos);
    priority_queue<Node> priority_q;
    vector<PosNode> fail;
    bool visited[mazeSizeX][mazeSizeY] = {0};
    if(maze[goal_pos[0]][goal_pos[1]] == 0)
    {
        cout << "goal unreachable\n";
        return fail;
    }
    if(start_pos[0]<0 || start_pos[0]>mazeSizeX-1 || start_pos[1]<0 || start_pos[1]>mazeSizeY-1 || goal_pos[0]<0 || goal_pos[0]>mazeSizeX-1 || goal_pos[1]<0 || goal_pos[1]>mazeSizeY-1)
    {
        cout << "invalid input\n";
        return fail;
    }
    start.move_cost = 0;
    start.g = 0;
    start.h = HeuristicFunctionDiagnal(start, goal);
    start.f = start.g + start.h;
    priority_count ++;
    start.priority_stamp = priority_count;
    priority_q.push(start);
    while(!priority_q.empty())
    {
        Node top_node = priority_q.top();
        PosNode pos;
        pos.pos[0] = top_node.PosX();
        pos.pos[1] = top_node.PosY();
        if(top_node.PosX() == goal.PosX() && top_node.PosY() == goal.PosY())
        {
            top_node.path.push_back(pos);
            return top_node.path;
        }
        visited[top_node.PosX()][top_node.PosY()] = true;
        priority_q.pop();
        vector<Node> successors = FindSuccessors(pos.pos, maze);
        for(unsigned int i = 0; i < successors.size(); i++)
        {
            Node succ = successors[i];
            succ.path = top_node.path;
            succ.g = succ.move_cost + top_node.g;
            if(!visited[succ.PosX()][succ.PosY()])
            {
                maze[succ.PosX()][succ.PosY()] = 4;
                succ.path.push_back(pos);
                succ.h = HeuristicFunctionDiagnal(succ, goal);
                succ.f = succ.g + succ.h;
                priority_count ++;
                succ.priority_stamp = priority_count;
                priority_q.push(succ);
                visited[succ.PosX()][succ.PosY()] = true;

            }
        }
    }
    cout << "search fail\n";
    return fail;
}

vector<PosNode> bresenhams_line_alg(vector<PosNode> path, int maze[mazeSizeX][mazeSizeY])
{
    PosNode start = path.front();
    vector<PosNode> output;
    for(unsigned int i = 1; i < path.size(); i++)
    {
        PosNode goal = path[i];
        bool steep = false;
        bool walkable = true;
        if(abs(goal.pos[1]-start.pos[1])>abs(goal.pos[0]-start.pos[0]))
        {
            steep = true;
        }
        if(steep)
        {
            swap(start.pos[0], start.pos[1]);
            swap(goal.pos[0], goal.pos[1]);
        }
        if(start.pos[0]>goal.pos[0])
        {
            swap(start.pos[0], goal.pos[0]);
            swap(start.pos[1], goal.pos[1]);
        }
        int delta_x = goal.pos[0] - start.pos[0];
        int delta_y = abs(goal.pos[1] - start.pos[1]);
        int error = delta_x;
        int y_step;
        int y = start.pos[1];
        if(start.pos[1] < goal.pos[1])
        {
            y_step = 1;
        }
        else
        {
            y_step = -1;
        }
        for(int x=start.pos[0]; x<=goal.pos[0]; x++)
        {
            if(steep)
            {
                if(maze[y][x] == 0)
                {
                    walkable = false;
                }
            }
            else
            {
                if(maze[x][y] == 0)
                {
                    walkable = false;
                }
            }
            error -= 2*delta_y;
            if(error <= 0)
            {
                y += y_step;
                error += 2*delta_x;
            }
        }

        if(walkable == false)
        {
            output.push_back(path[i-1]);
            start = path[i-1];
        }
    }
    output.push_back(path.back());
    return output;
}

int get_x(vector<PosNode> path){
    PosNode a = path.front();
    int x = a.pos[0];
    return x;
}

int get_y(vector<PosNode> path){
    PosNode a = path.front();
    int y = a.pos[1];
    return y;
}

void prediction_of_movement(int maze[mazeSizeX][mazeSizeY],int obstacle_pos[2], int move_vector[2]){
    int prediction_ratio = 2;
    int prediction_point[2] = {obstacle_pos[0] + prediction_ratio*move_vector[0], obstacle_pos[1] + prediction_ratio*move_vector[1]};
    int start[2] = {obstacle_pos[0], obstacle_pos[1]};
    int goal[2] = {prediction_point[0], prediction_point[1]};
    PosNode pos;
    vector<PosNode> prediction;
    bool steep = false;
    bool walkable = true;
    cout << "start: " << start[0] << "," << start[1] << " goal: " << goal[0] << "," << goal[1] << "\n";
    if(abs(goal[1]-start[1])>abs(goal[0]-start[0]))
    {
        steep = true;
    }
    if(steep)
    {
        swap(start[0], start[1]);
        swap(goal[0], goal[1]);
    }
    if(start[0]>goal[0])
    {
        swap(start[0], goal[0]);
        swap(start[1], goal[1]);
    }
    int delta_x = goal[0] - start[0];
    int delta_y = abs(goal[1] - start[1]);
    int error = delta_x;
    int y_step;
    int y = start[1];
    if(start[1] < goal[1])
    {
        y_step = 1;
    }
    else
    {
        y_step = -1;
    }
    for(int x=start[0]; x<=goal[0]; x++)
    {
        if(steep)
        {
            if(maze[y][x] == 0)
            {
                walkable = false;
            }
            cout << "x: " << y << " y: " << x << "\n";
            maze[y][x] = 2;
            pos.pos[0] = y;
            pos.pos[1] = x;
            prediction.push_back(pos);
        }
        else
        {
            if(maze[x][y] == 0)
            {
                walkable = false;
            }
            cout << "x: " << x << " y: " << y << "\n";
            maze[x][y] = 2;
            pos.pos[0] = x;
            pos.pos[1] = y;
            prediction.push_back(pos);
        }
        error -= 2*delta_y;
        if(error <= 0)
        {
            y += y_step;
            error += 2*delta_x;
        }
    }
    int j = 100;
    for(unsigned int i = 0; i < prediction.size(); i++)
    {

        build_obstacles(prediction[i].pos, robotSize, maze, 0);
        j+=i*100;
    }
}

float move_degree(int start_pos[2], vector<PosNode> path){
    PosNode start;
    start.pos[0] = start_pos[0];
    start.pos[1] = start_pos[1];
    PosNode goal = path[0];
    float pi = 3.1415926;
    int num = goal.pos[1] - start.pos[1];
    int den = goal.pos[0] - start.pos[0];
    float in_radius = 0;
    float in_degree = 0;
    if (num!=0 && den!=0){
        float radius = atan((float)num / den);
        if (num > 0 && den > 0){
            in_radius = radius;
        }
        else if (num > 0 && den < 0){
            in_radius = pi + radius;
        }
        else if (num < 0 && den < 0){
            in_radius = pi + radius;
        }
        else{
            in_radius = 2*pi + radius;
        }
    }
    else if (num == 0 && den > 0){
        in_radius = 0;
    }
    else if (num == 0 && den < 0){
        in_radius = pi;
    }
    else if (num > 0 and den == 0){
        in_radius = pi/2;
    }
    else if (num < 0 && den == 0){
        in_radius = 3*pi/2;
    }
    else{
        cout << "error\n";
    }
    in_degree = in_radius * 180 / pi;
    cout << "start: (" << start.pos[0] << "," << start.pos[1] << ")\n";
    cout << "goal: (" << goal.pos[0] << "," << goal.pos[1] << ")\n";
    cout << "num: " << num << " den: " << den << " degree: " << in_degree << "\n";
    return in_degree;
}

int big_to_small_maze(int big){
    int small = round((double)big/50);
    return small;
}


bool add(main_loop::path::Request  &req,
         main_loop::path::Response &res)
{

    int maze[mazeSizeX][mazeSizeY];
    build_maze(maze); 
    int obstacle_a[2] = {big_to_small_maze(req.enemy1_x),big_to_small_maze(req.enemy1_y)};//<---from camera
    int obstacle_b[2] = {big_to_small_maze(req.enemy2_x),big_to_small_maze(req.enemy2_y)};//<---from camera
    int obstacle_c[2] = {big_to_small_maze(req.ally_x),big_to_small_maze(req.ally_y)};//<---from camera 
    build_obstacles(obstacle_a, robotSize, maze, 0);
    build_obstacles(obstacle_b, robotSize, maze, 0);
    build_obstacles(obstacle_c, robotSize, maze, 0);
  
    int start_pos[2] = {big_to_small_maze(req.my_pos_x),big_to_small_maze(req.my_pos_y)};//<---my_pos
    int goal_pos[2] = {big_to_small_maze(req.goal_pos_x),big_to_small_maze(req.goal_pos_y)};//<---goap  


    vector<PosNode> a = AStar(start_pos, goal_pos, maze);
    vector<PosNode> b = bresenhams_line_alg(a, maze);//--->output b

    res.next_pos_x = get_x(b)*50 ;//--->for big map 
    res.next_pos_y = get_y(b)*50 ;//--->for big map
    cout << "degree: " << move_degree(start_pos,b) << "\n" ;//--->output degree!!


    return true;

}



int main(int argc, char **argv)
{

    ros::init(argc, argv, "path_planning_");
    ros::NodeHandle nh;
    ros::ServiceServer service = nh.advertiseService("path_plan", add);
    ros::spin();

    return 0;
}
