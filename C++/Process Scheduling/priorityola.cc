#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

struct process
{
	int pid;
	int arrival;
	int burst;
	int priority;
};

//First Come First Server Priority
void FCFS(process data[10], int i);
//Shortest Job First
void SJF(process data[10], int i);
//Shortest Job First preemptively
void SRTF(process data[10], int i);
//Non-preemptive
void NPP(process data[10], int i);
//Preemptive
void PP(process data[10], int i);
//Round Robin
void RR(process data[10], int i);

int main()
{
	
	process data[10];
	process copy[10];
	int i = 0;
	ifstream ifs;
			
	ifs.open("process.dat"); //open file
	
	//priming read
	ifs >> data[i].pid >> data[i].arrival >> data[i].burst >> data[i].priority; 
	
	//read in process data into array
	while(ifs)
	{
		i++;
		ifs >> data[i].pid >> data[i].arrival >> data[i].burst >> data[i].priority;
	}
	
	/*verify data was read in correctly
	for(int k = 0; k < i; k++)
	{
		cout << data[k].pid << " " << data[k].arrival << " "; 
		cout << data[k].burst << " " << data[k].priority << endl;
	}*/
	
	//make sure data is sorted by arrival time
	for(int h = 0; h < i; h++)
		for(int j = 0; j < i - 1; j++)
			if(data[j].arrival > data[j + 1].arrival)
				swap(data[j], data[j + 1]);
			
	//call FSFS
	FCFS(data, i);
	//call SJF
	SJF(data, i);
	
	//make a copy of data so SRTF won't change data array
	for(int j = 0; j < i; j++)
	{
		copy[j].pid = data[j].pid;
		copy[j].arrival = data[j].arrival;
		copy[j].burst = data[j].burst;
		copy[j].priority = data[j].priority;
	}
	//call SRTF
	SRTF(copy, i);
	NPP(data, i);
	
	//make a copy of data so PP won't change data array
	for(int j = 0; j < i; j++)
	{
		copy[j].pid = data[j].pid;
		copy[j].arrival = data[j].arrival;
		copy[j].burst = data[j].burst;
		copy[j].priority = data[j].priority;
	}
	//call PP
	PP(copy, i);
	
	//call RR
	RR(data, i);
	
	return 0;
}

//First Come First Serve Priority
void FCFS(process data[10], int processes)
{
	cout << "FCFS ";
	for(int index = 0; index < processes; index++)
		cout << data[index].pid << " ";
	cout << endl;
}

//Shortest Job First
void SJF(process data[10], int i)
{
	process queue[i];
	process running[1];	
	int time = 0;
	int count = 1;
	int time_max = 0;
	int queued = 0;

	//finds total time to run all processes
	for(int j = 0; j < i; j++)
		time_max += data[j].burst;
	
	running[0] = data[0]; //send 1st job to queue
	cout << "SJF " << running[0].pid << " ";
	
	while(count <= time_max)
	{
		//check if current time is <= to burst time of current process
		if(time < running[0].burst)
		{
			//check if all other processes have been queued or not
			if(queued < i - 1)
			{
				queue[count - 1] = data[count];
				queued++;
			}
			time++;
			count++;
		}
		else
		{
			//sorts queue by priority
			for(int k = 0; k < queued; k++)
				for(int l = 0; l < queued-1; l++)
					if(queue[l].burst > queue[l + 1].burst)
						swap(queue[l], queue[l + 1]);
			/*check queue
			cout << endl << "queue is  " << endl;
			for(int z = 0; z <queued; z++)
				{
					cout << queue[z].pid << " " << queue[z].arrival << " "; 
					cout << queue[z].burst << " " << queue[z].priority << endl;
				}*/

			running[0] = queue[0]; //assigns 1st in queue to run
					
			//display pid of running process
			cout << running[0].pid << " ";
			time = 0; //reinitialize time to start over
			
			//get rid of process that was just told to run from array
			for(int m = 0; m < queued - 1; m++)
				swap(queue[m], queue[m+1]);
			queued--;
			i--;
		}
	}
	cout << endl;
}

//Shortest Job first w/ preemptive queueing
void SRTF(process data[10], int i)
{
	process queue[i];
	process running[1];
	int time_max = 0;
	int queued = 0;
	int time = 1;

	for(int j = 0; j < i -1 ; j++)
		time_max += data[j].burst;
	
	running[0] = data[0]; //send 1st job to queue
	for(int n = 0; n < i - 1; n++)
		swap(data[n], data[n+1]);
	i--;
	cout << "SRTF " << running[0].pid << " ";
	
	for(time; time < time_max; time++)
	{	
		//check if all other processes have been queued or not
		if(i > 0)
		{
			queue[queued] = data[0];
			//get rid of process in data array that was sent to queue
			for(int n = 0; n < i - 1; n++)
				swap(data[n], data[n+1]);
			i--;
			queued++;
		}
		
		running[0].burst--;
	
		//sorts queue by burst
		for(int k = 0; k < queued; k++)
			for(int l = 0; l < queued-1; l++)
				if(queue[l].burst > queue[l + 1].burst)
					swap(queue[l], queue[l + 1]);
				
		//determine if burst of running process has more time left than process in queue
		//with the smallest burst time
		if(running[0].burst > queue[0].burst)
		{
			swap(running[0], queue[0]);
			cout << running[0].pid << " "; //display pid of running proces
		}
		
		//sorts queue by burst
		for(int k = 0; k < queued; k++)
			for(int l = 0; l < queued-1; l++)
				if(queue[l].burst > queue[l + 1].burst)
					swap(queue[l], queue[l + 1]);
		
		//determine if process running is through or not
		if(running[0].burst <= 0)
		{
			//run next item in queue
			running[0] = queue[0]; 
			
			cout << running[0].pid << " "; //display pid of newly running process
			
			//get rid of process in queue that was just told to run
			for(int m = 0; m < queued - 1; m++)
				swap(queue[m], queue[m+1]);
			
			queued--;
		}
	}
	cout << endl;
};

//Non-preemptive
void NPP(process data[10], int i)
{
	process queue[i];
	process running[1];	
	int time = 0;
	int count = 1;
	int time_max = 0;
	int queued = 0;

	//finds total time to run all processes
	for(int j = 0; j < i; j++)
		time_max += data[j].burst;
	
	running[0] = data[0]; //send 1st job to queue
	cout << "non-preemmptive Priority " << running[0].pid << " ";
	
	while(count <= time_max)
	{
		//check if current time is <= to burst time of current process
		if(time < running[0].burst)
		{
			//check if all other processes have been queued or not
			if(queued < i - 1)
			{
				queue[count - 1] = data[count];
				queued++;
			}
			time++;
			count++;
		}
		else
		{
			//sorts queue by priority
			for(int k = 0; k < queued; k++)
				for(int l = 0; l < queued-1; l++)
					if(queue[l].priority > queue[l + 1].priority)
						swap(queue[l], queue[l + 1]);

			running[0] = queue[0]; //assigns 1st in queue to run
					
			//display pid of running process
			cout << running[0].pid << " ";
			time = 0; //reinitialize time to start over
			
			//get rid of process that was just told to run from array
			for(int m = 0; m < queued - 1; m++)
				swap(queue[m], queue[m+1]);
			queued--;
			i--;
		}
	}
	cout << endl;
};

//Preemptive Priority
void PP(process data[10], int i)
{
	process queue[i - 1];
	process running[1];
	int time_max = 0;
	int queued = 0;
	int time = 1;

	/*check queue
	cout << endl << "data is  " << endl;
	for(int z = 0; z <i; z++)
		{
			cout << data[z].pid << " " << data[z].arrival << " "; 
			cout << data[z].burst << " " << data[z].priority << endl;
		}*/
	
	for(int j = 0; j < i ; j++)
		time_max += data[j].burst;
	
	running[0] = data[0]; //send 1st job to queue
	
	//remove job just sent to running from data
	for(int n = 0; n < i - 1; n++)
		swap(data[n], data[n+1]);
	i--;
	
	cout << "preemptive Priority " << running[0].pid << " "; //display running's pid
	
	for(time; time < time_max; time++)
	{	
		running[0].burst--;
		//check if all other processes have been queued or not
		if(i > 0)
		{
			queue[queued] = data[0]; 
			//get rid of process in data array that was sent to queue
			for(int n = 0; n < i - 1; n++)
				swap(data[n], data[n+1]);
			i--;
			queued++;
	
			//sorts queue by priority
			for(int k = 0; k < queued; k++)
				for(int l = 0; l < queued-1; l++)
					if(queue[l].priority > queue[l + 1].priority)
						swap(queue[l], queue[l + 1]);
		}
		//determine if priority of running process is greater than priority
		//of queued process with greatest priority
		if(running[0].priority > queue[0].priority)
		{
			swap(running[0], queue[0]);
			cout << running[0].pid << " "; //display pid of running proces
		
			//sorts queue by priority
			for(int k = 0; k < queued; k++)
				for(int l = 0; l < queued-1; l++)
					if(queue[l].priority > queue[l + 1].priority)
						swap(queue[l], queue[l + 1]);
		}
		//determine if process running is through or not
		if(running[0].burst <= 0)
		{
			//run next item in queue
			running[0] = queue[0]; 
			
			cout << running[0].pid << " "; //display pid of newly running process
			
			//get rid of process in queue that was just told to run
			for(int m = 0; m < queued - 1; m++)
				swap(queue[m], queue[m+1]);
			
			queued--;
		}
	}
	cout << endl;
};

//Round Robin
void RR(process data[10], int i)
{
	int time_max = 0;
	int time = 0;
	int n = 0;
	
	cout << "RR ";

	/*check data
	cout << endl << "data is  " << endl;
	for(int z = 0; z <i; z++)
		{
			cout << data[z].pid << " " << data[z].arrival << " "; 
			cout << data[z].burst << " " << data[z].priority << endl;
		}*/
	
	for(int j = 0; j < i ; j++)
		time_max += data[j].burst;
		
	cout << "RR "; //display running's pid
	
	while(time_max > time)
	{
		for(int n = 0; n < i; n++)
		{
			cout << data[n].pid << " ";
			for(int cycle = 0; cycle < 2; cycle++)
			{
				data[n].burst--;
				if(i > 0)
				{
					if(data[n].burst <= 0)
					{
						for(int k = n; k < i - 1; k++)
							swap(data[k], data[k+1]);
						if(i > 1)
							cout << data[n].pid << " ";
						i--;
						cycle = -1;
					}
					time++;
				}
			}
		}
	}
	cout << endl;
};
