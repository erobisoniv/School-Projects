//Name: Matthew Metzger
//CSCI 3250
//Date: April 21, 2012
//Purpose: Simulates FCFS, Look, and SSTF disk scheduling algorithms.

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cmath>
using namespace std;

const int MAX_TRACKS = 4;
const int MAX_SECTORS = 8;
const int MAX_CYLINDERS = 64;
const int MAX_REQUESTS = 100;

//cylinder structure
struct cylinder
{
	char track_sector[MAX_TRACKS][MAX_SECTORS];
};

//data of cylinder, track and sector
struct reads
{
	int clndr, track, sector;
};

// Sorts the items in an array into decending order.
void bubbleSort(reads theArray[], int);
void SSTFSort(reads theArray[], int);

int main(int argc, char* argv[])
{
	struct cylinder disk; //one cylinder in disk
	struct reads disks[MAX_REQUESTS]; 
	ifstream ifs; //input file stream
	FILE *f; //file pointer
	int n_cylinder, n_track, n_sector; //data to be read from indata file
	int n_cylinder_prev = 63; //holds previous read data to calculate seek distance
	int i, j, k;
	int seek_distance = 0;
	int distance_short = 0;
	int distance_cur = 0;
	int distance;
	int dtrack_shrt, dtrack_cur, prev_index, dsect_shrt, dsect_cur;
	int count = 0;
	int shortest; //index of read closest to current cylinder
	
	ifs.open(argv[2]); //open file
	f = fopen(argv[1], "r"); //open file for reading

	cout << "FCFS: ";
	
	ifs >> n_cylinder >> n_track >> n_sector;
	distance = n_cylinder - n_cylinder_prev;
	seek_distance += abs(float(distance));
	count ++;
	fseek(f, sizeof(struct cylinder) * n_cylinder, SEEK_CUR);
	fread(&disk, sizeof(struct cylinder), 1, f);
	printf("%c", disk.track_sector[n_track][n_sector]);
	n_cylinder_prev = n_cylinder;

	//read in cylinder and print out character using FCFS
	while(ifs)
	{
		ifs >> n_cylinder >> n_track >> n_sector;
		distance = n_cylinder - n_cylinder_prev;
		seek_distance += abs(float(distance));
		distance--;
		count ++;
		fseek(f, sizeof(struct cylinder) * distance, SEEK_CUR);
		fread(&disk, sizeof(struct cylinder), 1, f);
		printf("%c", disk.track_sector[n_track][n_sector]);
		n_cylinder_prev = n_cylinder;
	}

	cout << endl << "FCFS: Average seek distance: ";
	printf("%2.2f", (float(seek_distance) / float(count - 1)));
	cout << endl << endl;
	ifs.close(); //close file
	ifs.open(argv[2]); //open file
	
	i = 0; //initialize index

	//reads in requests and places them in array of reads
	while(ifs)
	{
		ifs >> n_cylinder >> n_track >> n_sector;	
		disks[i].clndr = n_cylinder;
		disks[i].track = n_track;
		disks[i].sector = n_sector;
		i++;
	}
	
	i--;//sets index back 1 to be used for array reference

	//puts requests in inverted sorting
	bubbleSort(disks, i);
	
	cout << "Look: ";
	n_cylinder_prev = 63; // initialize starting cylinder to 63
	seek_distance = 0; // initialize seek_distance to 0
	count = 0; //initialize count to 0
	distance = disks[0].clndr - n_cylinder_prev;
	seek_distance += abs(float(distance));
	count ++;
	fseek(f, sizeof(struct cylinder) * disks[0].clndr, SEEK_SET);
	fread(&disk, sizeof(struct cylinder), 1, f);
	printf("%c", disk.track_sector[(disks[0].track)][(disks[0].sector)]);
	n_cylinder_prev = disks[0].clndr;

	//read in cylinder and print out character using LOOK
	for(j = 1; j < i; j++)
	{
		distance = disks[j].clndr - n_cylinder_prev;
		seek_distance += abs(float(distance));
		distance --;
		count ++;
		fseek(f, sizeof(struct cylinder) * distance, SEEK_CUR);
		fread(&disk, sizeof(struct cylinder), 1, f);
		printf("%c", disk.track_sector[(disks[j].track)][(disks[j].sector)]);
		n_cylinder_prev = disks[j].clndr;
	}

	cout << endl << "Look: Average seek distance: ";
	printf("%2.2f", (float(seek_distance) / float(count)));
	
	cout << endl;
	
	ifs.close(); //close file
	ifs.open(argv[2]); //open file
	
	disks[0].clndr = 32;
	disks[0].track = 0;
	disks[0].sector = 0;
	i = 1; //initialize index

	//reads in requests and places them in array of reads
	while(ifs)
	{
		ifs >> n_cylinder >> n_track >> n_sector;	
		disks[i].clndr = n_cylinder;
		disks[i].track = n_track;
		disks[i].sector = n_sector;
		i++;
	}
	
	i--;//sets index back 1 to be used for array reference

	SSTFSort(disks, i);
	
	for(int l = 0; l < i; l++)
	{
		cout << disks[l].clndr << " " << disks[l].track << " " << disks[l].sector << endl;
	}
	
	cout << "SSTF: ";

	
	seek_distance = 0; //initialize seek_distance to 0
	count = 1; //initialize count to 1
	distance = disks[1].clndr - disks[0].clndr;
	seek_distance += abs(float(distance));
	distance--;
	fseek(f, sizeof(struct cylinder) * disks[1].clndr , SEEK_SET);
	fread(&disk, sizeof(struct cylinder), 1, f);
	printf("%c", disk.track_sector[(disks[1].track)][(disks[1].sector)]);
	n_cylinder_prev = disks[1].clndr;
	//read in cylinder and print out character using LOOK
	for(j = 2; j < i; j++)
	{
		distance = disks[j].clndr - n_cylinder_prev;
		seek_distance += abs(float(distance));
		distance--;
		count ++;
		fseek(f, sizeof(struct cylinder) * distance, SEEK_CUR);
		fread(&disk, sizeof(struct cylinder), 1, f);
		printf("%c", disk.track_sector[(disks[j].track)][(disks[j].sector)]);
		n_cylinder_prev = disks[j].clndr;
	}
	cout << endl << "SSTF: Average seek distance: ";
	printf("%2.2f", (float(seek_distance) / float(count)));
	
	fclose(f); //close file 
	ifs.close(); //close file
	
	cout << endl;
	return 0;
}
	
// Sorts the items in an array into decending order.
void bubbleSort (reads theArray[], int n)
{
   bool sorted = false;  // false when swaps occur

   for (int pass = 1; (pass < n) && !sorted; ++pass)
   {  
	sorted = true;  // assume sorted
	for (int index = 0; index < n-pass; ++index)
	{  
		int nextIndex = index + 1;
		if(theArray[index].clndr == theArray[nextIndex].clndr)
		//cylinders are the same -> sort by track
		{
			if(theArray[index].track == theArray[nextIndex].track)
			//tracks are the same -> sort by sector
			{
				if(theArray[index].sector < theArray[nextIndex].sector)
				{
					swap(theArray[index], theArray[nextIndex]);
					sorted = false;
				}
			}
			else if(theArray[index].track < theArray[nextIndex].track)
			{
				swap(theArray[index], theArray[nextIndex]);
				sorted = false;
			}
		}
		else if (theArray[index].clndr < theArray[nextIndex].clndr)
		{  
			swap(theArray[index], theArray[nextIndex]);
			sorted = false;
		}
	}  
   } 
} 

// Sorts the items in array using SSTF algorithm
void SSTFSort(reads theArray[], int n)
{
	int distance_short = 0;
	int distance_cur = 0;
	int dtrack_shrt, dtrack_cur, prev_index, dsect_shrt, dsect_cur;
	
	for(int j = 1; j < n; j++)
	{
		int shortest = j;
		for(int k = j+1; k < n; k++)
		{
			distance_short = abs(float(theArray[shortest].clndr) - float(theArray[j - 1].clndr));
			distance_cur = abs(float(theArray[k].clndr) - float(theArray[j - 1].clndr));
			if(distance_short > distance_cur)
			{
				shortest = k;
				distance_short = distance_cur;
			}
			else if(distance_short == distance_cur)
			{
				dtrack_shrt = abs(float(theArray[shortest].track) - float(theArray[j - 1].track));
				dtrack_cur = abs(float(theArray[k].track) - float(theArray[j - 1].track));
				if(dtrack_shrt > dtrack_cur)
				{
					shortest = k;
					distance_short = distance_cur;
				}
				else if(dtrack_shrt == dtrack_cur)
				{
					dsect_shrt = abs(float(theArray[shortest].sector) - float(theArray[j - 1].sector));
					dsect_cur = abs(float(theArray[k].sector) - float(theArray[j - 1].sector));
					if(dsect_shrt > dsect_cur)
					{
						shortest = k;
						distance_short = distance_cur;
					}
				}
			}
		}
		swap(theArray[shortest], theArray[j]);
	}
}