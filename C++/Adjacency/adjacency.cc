#include <iostream>

using namespace std;

const int Max_Nodes = 10; //maximum number of nodes

int adjacency[Max_Nodes][Max_Nodes];
//displays menu
void printMenu();
//displaysj matrix
void matrix(int matrix[Max_Nodes][Max_Nodes], int);
//determines if there are any isolated nodes and displays them
void isolated(int matrix[Max_Nodes][Max_Nodes], int);
//determines the degree of a particular node
void degree(int matrix[Max_Nodes][Max_Nodes], int);
//determines if a euler path exists
void euler(int matrix[Max_Nodes][Max_Nodes], int);
//determines if one node is adjacent to another
void adjacent(int matrix[Max_Nodes][Max_Nodes], int);

int main()
{
	int nodes;
	int input;
	int row;
	int column;
	char trash;
	
	cout << "Please input the number of nodes: ";
	cin >> nodes;
	
	//sets all values in adjacency matrix to 0
	for(int i = 0; i < nodes; i++)
		for(int j = 0; j < nodes; j++)
			adjacency[i][j] = 0;
	
	cout << "Please input the adjacency relation: " << endl;
	cin >> trash;
	
	while(trash != '}')
	{
		cin >> trash;
		cin >> row;
		cin >> trash;
		cin >> column;
		cin >> trash;
		cin >> trash;
		
		adjacency[row - 1][column -1] ++;
	}
	
	printMenu();
	cin >> input;
	while(input)
	{
		switch(input)
		{
			case 1: 
				matrix(adjacency, nodes);
				printMenu();
				break;
			case 2:
				isolated(adjacency, nodes);
				printMenu();
				break;
			case 3:
				degree(adjacency, nodes);
				printMenu();
				break;
			case 4:
				euler(adjacency, nodes);
				printMenu();
				break;
			case 5:
				adjacent(adjacency, nodes);
				printMenu();
				break;
			case 6:
				cout << "Goodbye" << endl;
				return 0;
		};
		cin >> input;
	}
	
}

//displays menu
void printMenu()
{
	cout << "Choose from the following" << endl;
	cout << "1. Print the adjacency matrix" << endl;
	cout << "2. Determine if there are any isolated nodes" << endl;
	cout << "3. Determine the degreee of a node" << endl;
	cout << "4. Determine if an Euler path exists" << endl;
	cout << "5. Determine if one node is adjacent to another" << endl;
	cout << "6. Quit" << endl;
}

//displaysj matrix
void matrix(int matrix[Max_Nodes][Max_Nodes], int nodes)
{
	cout << "The Adjacency matrix is:" << endl;
	
	for(int i = 0; i < nodes; i++)
	{
		for(int j = 0; j < nodes; j++)
			cout << matrix[i][j] << " ";
	
		cout << endl;
	}
	
	cout << endl;
};

//determines if there are any isolated nodes and displays them
void isolated(int matrix[Max_Nodes][Max_Nodes], int nodes) 
{
	int sum;
	int isolated = 0;
	
	for(int i = 0; i < nodes; i++)
	{
		sum = 0;
		for(int j =0; j < nodes; j++)
			sum += matrix[i][j];
		if(sum == 0)
		{
			cout << i + 1 << " is an isolated node" << endl;
			isolated++;
		}
	}
	if(isolated == 0)
		cout << "There are no isolated nodes." << endl;
	cout << endl;
};

//determines the degree of a particular node
void degree(int matrix[Max_Nodes][Max_Nodes], int nodes)
{
	int node_interest;
	int sum = 0;
	
	cout << "Which node are you interested in? " << endl;
	cin >> node_interest;
	
	for(int i = 0; i < nodes; i++)
	{
		if((node_interest - 1) == i)
			sum = sum + (2 * matrix[node_interest  -1][i]);
		else
			sum += matrix[node_interest - 1][i];
	}
	cout << "The degree of node " << node_interest << " is " << sum << endl << endl;
};

//determines if a euler path exists
void euler(int matrix[Max_Nodes][Max_Nodes], int nodes)
{
	int sum;
	int odd = 0;
	
	for(int i = 0; i < nodes; i++)
	{
		sum = 0;
		for(int j =0; j < nodes; j++)
			if(i == j)
				sum = sum + (2 * matrix[i][j]);
			else
				sum += matrix[i][j];
		if(sum%2 != 0)
			odd++;
	}
	
	if(odd == 0 || odd == 2)
		cout << "An Euler path does exist in the graph" << endl << endl;
	else
		cout << "An Euler path does NOT exist in the graph" << endl << endl;
};

//determines if one node is adjacent to another
void adjacent(int matrix[Max_Nodes][Max_Nodes], int nodes)
{
	int node1;
	int node2;
	
	cout << "Which two nodes are you interested in?" << endl;
	cin >> node1;
	cin >> node2;
	
	if(matrix[node1 -1][node2 - 1] > 0)
		cout << "Node " << node1 << " is adjacent to node " << node2 << endl << endl;
	else
		cout << "Node " << node1 << " is NOT adjacent to node " << node2 << endl << endl;
};
