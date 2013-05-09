#include<iostream>
#include<fstream>
#include<list>
using namespace std;

//data for each block
struct node{
	int value;
	int xvalue;
	int yvalue;
	bool visited;
};

const int MAX_ROWS = 8;
const int MAX_COLS = 16;

//generates map of environment
void generateMap(int goalx, int goaly, node array[][MAX_COLS]);

//used in BFS
void visitNode(int x, int y, list<node>& queue, node array[][MAX_COLS]);

//checks if values are within array
bool inrange(int x, int y);

//finds shortest path from (0,0) to (x,y)
void findPath(int x, int y, node array[][MAX_COLS]);

//returns the node with the lowest value that is adjacent to node at (x,y)
node findLowest(int x, int y, node array[][MAX_COLS]);

int main(int argc, char* argv[])
{
	ifstream myin;
	int endx, endy, temp;
	node map[MAX_ROWS][MAX_COLS];

	myin.open(argv[1]); //get filename from command line and open

	//read in map and set any obstacles to visited
	for(int i = 0; i < MAX_ROWS; i++)
	{
		for(int j = 0; j < MAX_COLS; j++)
		{
			myin >> temp;
			map[i][j].value = temp;
			map[i][j].xvalue = i;
			map[i][j].yvalue = j;

			if(map[i][j].value == 1)
			{	
				map[i][j].visited = true;
			}
			else
				map[i][j].visited = false;
		}
	}
	
	cout << "\nEnter the goal position in the form (x,y): ";
	getchar();
	cin >> endx;
	getchar();
	cin >> endy;
	
	generateMap(endx, endy, map); //generate map
	findPath(endx, endy, map); //find shortest path and display

	myin.close();
	
	return 0;
}

void generateMap(int goalx, int goaly, node map[][MAX_COLS])
{
	list<node> queue; 

	//initialize goal to 2 and set as visited
	map[goalx][goaly].value = 2;
	map[goalx][goaly].visited = true;
	
	//add goal to queue
	queue.push_front(map[goalx][goaly]);

	while(queue.size() != 0) //queue is not empty
	{
		visitNode(queue.front().xvalue - 1, queue.front().yvalue, queue, map); //visit node to north
		visitNode(queue.front().xvalue + 1, queue.front().yvalue, queue, map); //visit node to south
		visitNode(queue.front().xvalue, queue.front().yvalue - 1, queue, map); //visit node to west
		visitNode(queue.front().xvalue, queue.front().yvalue + 1, queue, map); //visit node to east
			
		queue.pop_front(); //remove front node from queue
	}

}

bool inrange(int x, int y)
{
	if( (x < 0) || (x > (MAX_ROWS - 1) ) )
		return false;
	else if( (y < 0) || (y > (MAX_COLS - 1) ) )
		return false;
	else
		return true;
}

void visitNode(int x, int y, list<node>& queue, node map[][MAX_COLS])
{
	node temp;
	temp.xvalue = x;
	temp.yvalue = y;
	
	/*
	if node has not been visited && is within the map
		node.value = parent_node.value + 1
		mark node as visited
		update map with node info
		add node to queue
	*/
	if(map[x][y].visited != true && inrange(x, y) == true)
	{
		temp.value = queue.front().value + 1;
		temp.visited = true;
		map[x][y] = temp;
		queue.push_back(temp);
	}
}

void findPath(int x, int y, node map[][MAX_COLS])
{
	list<node> path; //holds nodes for displaying path
	list<node>::iterator iter;

	path.push_front(map[0][0]); //add start position to list

	/*
	while goal is not in list
		find adjacent node with lowest value and add to path
	*/
	while( (path.back().xvalue != x) || (path.back().yvalue != y))
	{		
		path.push_back(findLowest(path.back().xvalue, path.back().yvalue, map));
	}

	printf("The number of steps from (0, 0) to (%i, %i) is %i\n", x, y, path.size() - 1);
	cout << "The path is: ";

	for(iter = path.begin(); iter != path.end(); ++iter)
	{
		printf("(%i, %i) ", iter->xvalue, iter->yvalue);
	}
	
	cout << endl;

}

node findLowest(int x, int y, node map[][MAX_COLS])
{
	node lowest;
	lowest.value = 100;

	//set lowest to node east of current node
	if(inrange(x+1, y) && map[x+1][y].value != 1)
		lowest = map[x+1][y];
	if(lowest.value > map[x-1][y].value && inrange(x-1, y) && map[x-1][y].value != 1)
		lowest = map[x-1][y];
	if(lowest.value > map[x][y-1].value && inrange(x, y-1) && map[x][y-1].value != 1)
		lowest = map[x][y-1];
	if(lowest.value > map[x][y+1].value && inrange(x, y+1) && map[x][y+1].value != 1)
		lowest = map[x][y+1];
		
	return lowest;
}