#include<iostream>
#include<math.h>
#include<iomanip>
using namespace std;

int main()
{
	double lhs = 1, rhs = 1;
	double temp;
	double phi1 = 0, phi2 = 1;
	double accuracy;
	cout << "How accurate should phi be? ";
	cin >> accuracy;
	while(fabs((phi2-phi1)) > accuracy)
	{
		temp = lhs;
		lhs = rhs;
		rhs += temp;
		phi1 = phi2;
		phi2 = rhs/lhs;
	}
	
	cout << "phi is " << setprecision(10) << phi2 << endl;
}
