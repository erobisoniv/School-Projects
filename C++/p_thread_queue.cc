#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
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

int time = -1;
int processes = 0;
process data[10];
process queue[10];
bool flag = false;
pthread_mutex_t job_queue_lock = PTHREAD_MUTEX_INITIALIZER;

void * scheduler (void * ignored)
{
	
	pthread_exit(0);
}

void * clocks (void * ignored)
{
	if(flag == false)
		
	pthread_mutex_lock(&job_queue_lock);
		flag = true;
		time++;
	pthread_mutex_unlock(&job_queue_lock);
	pthread_cond_wait(&condition, &job_queue_lock)
	
	pthread_exit(0);
}

int main()
{
	pthread_t schedule, clock;
	int queued = 0;
	process data[10];
	ifstream ifs;
			
	ifs.open("process.dat"); //open file
	
	//priming read
	ifs >> data[processes].pid >> data[processes].arrival >> data[processes].burst >> data[processes].priority; 
	
	//read in process data into array
	while(ifs)
	{
		processes++;
		ifs >> data[processes].pid >> data[processes].arrival >> data[processes].burst >> data[processes].priority;
	}
	
	//make sure data is sorted by arrival time (greatest to least)
	for(int h = 0; h < processes; h++)
		for(int j = processes; j > 1; j--)
			if(data[j].arrival > data[j - 1].arrival)
				swap(data[j], data[j + 1]);
			
	pthread_create(&clock, NULL, clocks, NULL);
	pthread_create(&schedule, NULL, scheduler, NULL);
			
	pthread_mutex_lock(&mutex);		
	if(data[processes].arrival <= time)
	{
		queue[queued] = data[processes];
		queued++;
	}
	
	
	pthread_create(&clock, NULL, clocks, NULL);
	pthread_create(&schedule, NULL, scheduler, NULL);
	pthread_join(produce,NULL);
	pthread_join(consume,NULL);

	return 0;
}