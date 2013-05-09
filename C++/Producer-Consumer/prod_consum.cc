//Author:  Matthew Metzger
//Due Date:  10/3/2011
//Purpose:  Producer-Consumer program where the producer adds 6 items
//to the queue.  The queue can hold up to 3 items.  The Consumer consumes items,
//removing them from the queue.  Program ends when all 6 items have been produced
//and consumed.
//Note:  Strange indentation to show locations of locks and matching
//		unlocks
//
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
using namespace std;

const int Queue_Size = 3;
int jobqueue[Queue_Size]; //array to hold items in queue
int job_queued; //holds number of items in queue
int job_consumed; //number of items that have been consumed
int job2consume;//which element needs to be consumed next
pthread_mutex_t job_queue_lock = PTHREAD_MUTEX_INITIALIZER;

void * producer (void * ignored)
{
	int myjobs[6] = {8, 2, 1, 7, 20, 42};
	int i = 0;
	
	while (i < 6)
	{
	    pthread_mutex_lock(&job_queue_lock);
		if(job_queued < 3)
		//there is a between 0 and 2 jobs in queue
		{
			//assigns the first element in myjobs[] to the the first free elements in jobqueue[] that has not been consumed
			jobqueue[job_queued] = myjobs[i]; 
			cout<<"producing job "<<i<<" with value "<<jobqueue[job_queued]<<" in queue\n";
		pthread_mutex_unlock(&job_queue_lock);
			job_queued++;
			i++;
		}
		else
		//there are 3  or more jobs in queue
		{
	    pthread_mutex_unlock(&job_queue_lock);
			cout<<"queue full can't produce\n";
			sleep(1); //if queue is full, sleep so consumer has time to run
		}
	}

	pthread_exit(0);
}

void * consumer (void * ignored)
{
	int count = 0;
	
	while (count < 6 )
	{
	    pthread_mutex_lock(&job_queue_lock);
	      if(job_queued > 0)
	       //there is at least one job in queue
   	      {
		        job2consume = Queue_Size % job_queued; //finds the first element that needs to be consumed
			cout<<"consuming job "<<count<<" with value "<<jobqueue[job2consume]<<"\n";
		pthread_mutex_unlock(&job_queue_lock);
		        job_consumed++;
			job_queued--;
			count++;
	      }
	      else
		// there are no jobs in queue
	      {
	    pthread_mutex_unlock(&job_queue_lock);
		cout<<"queue empty can't consume\n";
		sleep(1); //if queue is empty, sleep so producer has time to run
	      }
	}

	pthread_exit(0);
}

int main()
{
	pthread_t produce, consume;

	job_queued = 0;
	job_consumed = 0;
	pthread_create(&produce, NULL, producer, NULL);
	pthread_create(&consume, NULL, consumer, NULL);
	pthread_join(produce,NULL);
	pthread_join(consume,NULL);

	return 0;
}
