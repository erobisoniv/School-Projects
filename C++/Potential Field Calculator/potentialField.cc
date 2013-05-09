//Author: Matthew Metzger
//Instructor: Dr. Li
//Description: Computes repulsive vector
//Date: 2/21/2013

#include <iostream>
#include <math.h>
using namespace std;

struct Vector
{
	double direction;
	double magnitude;
};

Vector SumVectors(Vector one, Vector two);

int main()
{
	Vector sensor[6];
	double degrees[6] = {45, 30, -30, -45, -130, 130};
	int MAX_DISTANCE = 10;
	Vector turn;
	
	for(int i = 0; i < 6; i++)
	{
		double reading;
		cout << "Enter sensor reading " << (i+1) << " : ";
		cin >> reading;
		
		if(reading < MAX_DISTANCE)
		{
			sensor[i].direction = degrees[i] - 180;
			sensor[i].magnitude = (MAX_DISTANCE - reading)/MAX_DISTANCE;
		}
		else
		{
			sensor[i].direction = 0.0;
			sensor[i].magnitude = 0.0;
		}
	}
	
	for(int i = 1; i < 6; i++)
	{
		turn.direction = sensor[0].direction;
		turn.magnitude = sensor[0].magnitude;
		
		turn = SumVectors(turn, sensor[i]);
	}
	
	cout << "\nThe robot should turn " << turn.direction << " degrees with a magnitude of " << turn.magnitude << endl;
	return 0;
}
Vector SumVectors(Vector one, Vector two)
{
	Vector answer;
	
	answer.direction = atan( (one.magnitude * sin(one.direction) + two.magnitude * sin(two.direction) ) / (one.magnitude * cos(one.direction) + two.magnitude * cos(two.direction) ) );
	answer.magnitude = sqrt(one.magnitude*one.magnitude + two.magnitude*two.magnitude);
	
	return answer;
};