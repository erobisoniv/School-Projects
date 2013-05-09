#!/usr/bin/env python

#==========================
#Seth barnard
#Matt LastName
#Date: 11/28/12
#Class: AI
#==========================

import subprocess
from subprocess import PIPE
import sys
import os
import threading
import time
import shelve
import mastermindGameModule

#Global Variables
colors = ['Yellow','Green','Orange','Red','Blue','Brown']
totalPlays = 0
totalGuesses = 0
minGuesses = 999
maxGuesses = 0
finished = False

#Function for thread processing
def process(prog, start):
  global totalPlays
  global totalGuesses
  global minGuesses
  global maxGuesses

  #Setup where each thread starts and ends
  astart = start[0][0]
  aend = start[0][1]
  bstart = start[1][0]
  bend = start[1][1]

  #Run the subprogram for each color combination assigned
  for a in colors[astart:aend+1]:
    for b in colors[bstart:bend+1]:
      for c in colors:
        for d in colors:
          prog.stdin.write('s\n')
          prog.stdin.flush()
          prog.stdin.write(a+'\n')
          prog.stdin.flush()
          prog.stdin.write(b+'\n')
          prog.stdin.flush()
          prog.stdin.write(c+'\n')
          prog.stdin.flush()
          prog.stdin.write(d+'\n')

          line = ''
          while not line.startswith("This game took"):
            line = prog.stdout.readline()
          
          #Collect data for statistics
          guesses = int(line.split()[3])
          totalGuesses+= guesses
          totalPlays+=1
          if guesses<minGuesses:
            minGuesses = guesses
          if guesses>maxGuesses:
            maxGuesses = guesses
  
  #End subprogram and flush its pipe
  prog.stdin.write('q\n')
  prog.stdin.flush()

#Function for a thread to display progress of program
def percentComplete():
  global finished

  while not finished:
    print ("%.2f" % (float(totalPlays)/1296.0*100.0)),
    print "percent complete"
    time.sleep(8)


#Function that creates threads and calculates statistics for subprogram
def calcAverage():
  global finished
  global totalGuesses
  global totalPlays
  global maxGuesses
  global minGuesses

  #Time execution
  timer = time.time
  startTime = timer()

  #Create all subprograms
  prog1 = subprocess.Popen(["python", "mastermindGameModule.py"], stdin=PIPE, stdout=PIPE)
  prog2 = subprocess.Popen(["python", "mastermindGameModule.py"], stdin=PIPE, stdout=PIPE)
  prog3 = subprocess.Popen(["python", "mastermindGameModule.py"], stdin=PIPE, stdout=PIPE)
  prog4 = subprocess.Popen(["python", "mastermindGameModule.py"], stdin=PIPE, stdout=PIPE)
  
  #Create all threads
  t1 = threading.Thread(target=process, args=(prog1,([0,2],[0,2])))
  t2 = threading.Thread(target=process, args=(prog2,([0,2],[3,5])))
  t3 = threading.Thread(target=process, args=(prog3,([3,5],[0,2])))
  t4 = threading.Thread(target=process, args=(prog4,([3,5],[3,5])))
  t5 = threading.Thread(target=percentComplete)

  #Start all threads and wait until completion
  t1.start()
  t2.start()
  t3.start()
  t4.start()
  t5.start()

  t1.join()
  t2.join()
  t3.join()
  t4.join()
  finished = True
  t5.join()
  endTime = timer()

  #Store results in a file
  db = shelve.open("data")
  db["totalGuesses"] = totalGuesses
  db["totalPlays"] = totalPlays
  db["time"] = endTime-startTime
  db["maxGuesses"] = maxGuesses
  db["minGuesses"] = minGuesses
  db.close()

#Function that prints the main menu for the driver
def printMenu():
  db = shelve.open("data")
  lastTime = db["time"]
  db.close()
  print "\n                                 Menu"
  print "========================================================================\n"
  print "1: Show Stats From Previous Run\n"
  print "2: Run all possibilities to Get Stats"
  print "   (Previous processing time",
  print ("%.2f" % (lastTime/60.0)),
  print "minutes)\n"
  print "3: Run the game\n"
  print "4: Quit\n"
  print "========================================================================\n\n"
 

#Function that prints the statistics stored in file
def printStats():
  db = shelve.open("data")
  
  print "                   Statistics From Previous Data Run"
  print "========================================================================"
  print "========================================================================"
  print "***", "Total Number of Plays Processed:", db["totalPlays"], "                           ***"
  print "***                                                                  ***"
  print "***", "Total Number of Guesses Processed:", db["totalGuesses"], "                         ***"
  print "***                                                                  ***"
  print "***", "Average Number of Guesses Per Play:", 
  print "%.2f" % (float(db["totalGuesses"])/db["totalPlays"]), "                        ***"
  print "***                                                                  ***"
  print "***", "Maximum Number of Guesses Made Per Play:", db["maxGuesses"], "                      ***"
  print "***                                                                  ***"
  print "***", "Minimum Number of Guesses Made Per Play:", db["minGuesses"], "                      ***"
  print "***                                                                  ***"
  print "***", "Total Amount of Time Elapsed for Processing:",
  print "%.2f" % (db["time"]/60.0),
  print "Minutes       ***"
  for x in xrange(5):
     print "***                                                                  ***"
  print "========================================================================"
  print "========================================================================"
  db.close()

#Main Program
if __name__ == "__main__":
  choice = 0
  
  #Continue program until user decides to quit
  while choice!=4:
    printMenu() 
    choice = int(raw_input("Enter a choice(1, 2, 3, or 4):"))
    if choice==1:
      print "\n"
      printStats()
      print "\n"
    elif choice==2:
      print "\n"
      calcAverage()
      print "\n"
    elif choice==3:
      print "\n"
      mastermindGameModule.mainProgram()
      print "\n"
    elif choice==4:
      print
    else:
      print choice, "is not a valid option"

