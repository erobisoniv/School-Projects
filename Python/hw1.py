#!/usr/bin/python
#Name:		Matthew Metzger
#Course: 	CSCI 4350
#Program computes the Pearson correlation and Euclidean distance between two lists of values of equal length

import math

def f1(Data):
   sum = 0
   List1 = [(x*y) for x,y in zip(Data[0], Data[1])]
   for x in List1:
      sum += x
   return sum

def f2(Data, n):
   sum = 0
   for x in Data[n]:
	   sum += x
   return sum

def f3(Data, n):
   sum = 0
   for x in Data[n]:
	sum += pow(x,2)
   return sum
   
def ComputePearson(Data):
	a = len(Data[0]) * f1(Data) - f2(Data, 0) * f2(Data, 1)
	b = math.sqrt((len(Data[0]) * f3(Data, 0) - pow(f2(Data, 0), 2)) * (len(Data[0]) * f3(Data, 1) - pow(f2(Data, 1), 2)))	
	return a/b

def ComputeEuclidean(Data):
   sum = 0
   List1 = [(x-y) for x,y in zip(Data[0], Data[1])]
   for x in List1:
      sum += x*x
   return math.sqrt(sum)	
Data1 = [[2,5,6,9,4,3,9,1],[3,4,5,3,5,4,8,2]]
Data2 = [[1.5,3.5,2.2,4.2,1.3,1.4,1.5,8.5,10,12.5],[2.2,2.6,2.1,3.2,1.5,1.7,2.2,7.6,8.5,15.4]]
#test
#Data3 = [[4,5,10,3,6,9,29,10,5,8],[3,6,9,4,7,6,20,12,4,9]]

print "The Pearson Correlation for Data1 is %5.2f" % ComputePearson(Data1)
print "The Pearson Correlation for Data2 is %5.2f" % ComputePearson(Data2)
print "The Euclidean distance for Data1 is %5.2f" % ComputeEuclidean(Data1)
print "The Euclidean distance for Data2 is %5.2f" % ComputeEuclidean(Data2)
#test
#print "The Pearson Correlation for the Test Data is %5.2f" % ComputePearson(Data3)
#print "The Euclidean distance for the Test Data is %5.2f" % ComputeEuclidean(Data3)





