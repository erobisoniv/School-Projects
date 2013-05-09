//Name: Matthew Metzger
//Date: 12/2/2011
//Purpose: Program decodes sent packets using hamming codes
//		 and conversion of gray code to binary using a key.

#include<iostream>
#include<fstream>
#include <cmath>
#include<stdlib.h>

using namespace std;

const int MAX_CODES = 30;
const int PACKET_LENGTH = 9;
const int CODE_LENGTH = 5;

int main()
{
	ifstream ifs;
	char temp;
	char code[MAX_CODES];
	int packet[PACKET_LENGTH];
	int gray[CODE_LENGTH];
	int binary[CODE_LENGTH];
	int bit;
	int count;
	int i, j;
	char checks;
	ifs.open("chars.dat"); //open key file
	
	ifs >> noskipws >> temp; // priming read
	//read in key
	for(i = 0; i < MAX_CODES; i++)
	{
		code[i] = temp;
		ifs >> noskipws >> temp;
	}
	ifs.close(); //close key file
	
	ifs.open("packets.dat"); //open encoded message file
	
	ifs >> temp; //priming read
	//read in encoded message
	while(ifs)
	{
		int check = 0;
		int answer = 0;
		
		//read in encoded packets
		for(i = 0; i < 9; i++)
		{
			packet[i] = temp - '0';
			ifs >> temp;
			if(temp == '\n')
				ifs >> temp;
			//cout << packet[i]; - debugging
		}
		//cout << endl; - debugging
		
		//check if errors occured
		bit = packet[7] + packet[8];
		if((bit % 2) != 0)
			check += 8;
		
		bit = packet[3] + packet[4] + packet[5] + packet[6];
		if((bit % 2) != 0)
			check += 4;
		
		bit = packet[1] + packet[2] + packet[5] + packet[6];
		if((bit % 2) != 0)
			check += 2;
		
		bit = packet[0] + packet[2] + packet[4] + packet[6] + packet[8];
		if((bit % 2) != 0)
			check += 1;
		
		//change bit where error occured
		if(check != 0) 
		{
			if(packet[check - 1] == 1)
				packet[check - 1] = 0;
			else
				packet[check -1] = 1;
		}
		
		
		//get gray code (sent message)
		gray[0] = packet[2];
		gray[1] = packet[4];
		gray[2] = packet[5];
		gray[3] = packet[6];
		gray[4] = packet[8];
		
		/*for debug purposes
		cout << "gray: ";
		for(i = 0; i < CODE_LENGTH; i++)
			cout << gray[i] << " ";
		*/
		
		//convert gray code to binary
		binary[0] = gray[0];
		for(i = 1; i < CODE_LENGTH; i++)
		{
			count = 0;
			for(j = 0; j < i; j++)
			{
				count += gray[j];
			}
			if((count % 2) == 1)
			{
				if(gray[i] == 1)
					binary[i] = 0;
				else
					binary[i] = 1;
			}
			else
				binary[i] = gray[i];
		}
		
		/*for debug purposes
		cout << "bin: ";
		for(i = 0; i < CODE_LENGTH; i++)
			cout << binary[i];
		cout << endl;
		*/
		
		//convert binary to decimal
		for(i = 0, j = 4; i < CODE_LENGTH, j >= 0; i++, j--)
		{
			answer += pow(2, j) * binary[i];;
		}
		
		//print character
		cout << code[answer - 1];
		
		
	}
	cout << endl;
	
}