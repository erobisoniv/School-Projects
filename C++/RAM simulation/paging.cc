//Name: Matthew Metzger
//Date: Nov 21, 2011
//Purpose: Simulates virtual memory and handling page faults

#include <iostream>
#include <fstream>
using namespace std;

const int TABLE_START = 512;
const int MAX_PHYSICAL = 576;
const int MAX_VIRTUAL = 4096;
const int MAX_FRAMES = 8;

int main()
{
	int primary[MAX_PHYSICAL]; //physical memory
	int fifo_primary[8];//index 
	int fifo_secondary[8]; //page numbers in secondary memory
	char secondary[MAX_VIRTUAL]; //virtual memory
	char temp; //holds temp char to read in to secondary mem
	int sec_index = 0; //index of secondary[] 
	int page, offset; //page references
	int i, j, k; //indices
	int output; //index of char to output
	int n_secondary = 1; //number of chars in secondary[]
	int queued = 0; //number of pages that have been queued
	ifstream ifs1, ifs2; //input streams
	int address1; //address locations of pages in primary and secondary
	int free_frame = -1; //frame number that is free
	
	//initialize primary memory
	for(i = 0; i < MAX_PHYSICAL; i++)
		primary[i] = -1;
	
	ifs1.open("secondary.txt"); //open file
	ifs1>> temp; //priming read
	
	while(ifs1)
	//read chars and store in array till eof
	{
		secondary[sec_index] = temp;
		ifs1 >> noskipws >> temp;
		sec_index++;
		n_secondary++;
	}
	
	/*test to make sure data is read in properly
	for(j = 0; i < n_secondary; i++)
		cout << char(secondary[i]);*/
	
	cout << endl;
	
	ifs2.open("reference.txt"); //open file
	ifs2 >> page; //priming read
	
	while(ifs2)
	//run while there are still page references left to read
	{
		free_frame = -1;
		ifs2 >> offset; //read in offset
		address1 = page * 32;
		if(primary[(page + TABLE_START)] == -1)
		//page is not in physical memory
		{
			//determine if there are any free frames and swap if one is found
			for(i = 0; i < TABLE_START; i++)
				if(primary[i] == -1)
				{
					free_frame = i/32;
					i = TABLE_START;
				}
			if(free_frame != -1)
			//a free frame exists
			{
					//copy page from secondary to primary
					for(j = 0; j < 32; j++)
						primary[((free_frame*32) + j)] = secondary[(address1 + j)];
					
					//place frame number into page table
					primary[(TABLE_START + page)] = free_frame;
					fifo_primary[queued] = free_frame; //place frame # into fifo queue
					fifo_secondary[queued] = page; //place page # into queue
					queued++;
					output = primary[TABLE_START + page] * 32 + offset;
					cout << char(primary[output]);
					ifs2 >> page;
			}
			else
			//find which frame to swap out if there are no free frames and swap
			{
				//swap out next page in fifo queue
				for (k = 0; k < 32; k++)
					primary[((fifo_primary[queued % 8] * 32) + k)] = secondary[(address1 + k)];
				
				//change page table to indicate page is in primary memory
				primary[TABLE_START + page] = fifo_primary[(queued % 8)];
				//change entry in page table for page just swapped out to - 1
				primary[(TABLE_START + fifo_secondary[queued % 8])] = -1;
				//place new page into queue
				fifo_secondary[queued % 8] = page;
				
				queued++;
				output = primary[TABLE_START + page] * 32 + offset;
				cout << char(primary[output]);
				ifs2 >> page;
			}
			
		}
		else
		{
			output = primary[TABLE_START + page] * 32 + offset;
			cout << char(primary[output]);
			ifs2 >> page;
		}
	}
	
	cout << endl << endl << endl;
	for(i = 0; i < TABLE_START; i++)
	{
		for(j = 0; j < 32; j++)
		{
			if(char(primary[i]) == '\n')
				cout << '#';
			else
				cout << char(primary[i]);
			i++;
		}
		cout << endl;
	}
	for(i = TABLE_START; i < MAX_PHYSICAL; i++)
	{
		cout << primary[i];
		cout << endl;
	}

	return 0;
};