#Matthew Metzger and Seth Barnard
#Solves Mastermind puzzles
#10/7/2012

import copy
import random
import sys

def checkGuess(state, final):
  x = getCP(state, final)
  y = getColor(state, final)
  return (x, y-x)

def getCP(state, final):
  count = 0
  for x in xrange(4):
    if state[x] == final[x]:
      count += 1
  return count

def getColor(state, final):
  count = 0
  temp = copy.deepcopy(state)
  for x in final:
    if x in temp:
      count += 1
      temp.remove(x)
  return count

def remove(possible, guess, answer):
  temp = copy.deepcopy(possible)
  for x in temp:
    if answer != checkGuess(guess, x):
      possible.remove(x)

#Get the lowest possible score of a guess using all combinations of right color/locaitons
def getScore(guess, possible):
  leastScore = 99999999

  #Create all combinations of color/location pegs
  for x in xrange(0,5):
    for y in xrange(0,(5-x)):

      #Assign current color/location to answer
      answer = (x,y)

      #Do not consider special case (3,1)
      if answer != (3,1):

        removed = 0

        #Count the number of possible guesses that will be removed
        #using this color/locaiton combo
        for x in possible:
          if answer != checkGuess(guess, x):
              removed+=1              

        #Save the least number of items that will be removed given any color/locaiton
        if removed<leastScore:
          leastScore = removed

  return leastScore

#Get the next guess
#This will be the guess with the highest assigned score
def getNextGuess(possible, colors):
  highScore = -1
  nextGuess = []

  #Get the score for each guess
  #Save the guess with the highest score and return it
  #for a in colors:
  #  for b in colors:
  #    for c in colors:
  #      for d in colors:
  for x in possible:
          score = getScore(x, possible)
          if(score>highScore):
            highScore = score
            nextGuess = x
  
  return nextGuess

  

def PlayGame(colors, pegs):
  guess1 = [colors[0], colors[0], colors[1], colors[1]]
  guess = []
  possible = []
  final = []
  answer = (0,0) #initialize answer
  n = 0

  #generate all possible configurations
  for a in colors:
    for b in colors:
      for c in colors:
        for d in colors:
          possible.append([a,b,c,d])

  #get color configuration from user
  print "Colors are 'Red', 'Orange', 'Green', 'Blue', 'Yellow', 'Brown'"
  while n < pegs:
    foo = raw_input("Enter color %d " % ((n+1)))
    if foo in colors:
      n += 1
      final.append(foo)
    else:
      print "%s is not an available color" % (foo)

     
  #remove states that aren't consisitent with guess1
  answer = checkGuess(guess1, final)
  print "\nAnswers are in format: (Color & Position correct, Colors correct)"
  print "Guess 1: %s, %s, %s, %s - \t" % (guess1[0], guess1[1], guess1[2], guess1[3]), answer
  if answer == (4,0):
    return 1
  remove(possible, guess1, answer)

  guesses = 1

  #guess first item in possible, get an answer and remove inconsisitent states until
  #a correct answer is generated
  while answer != (4,0):
    #It gets the next guess for the getNextGuess function

    guess = getNextGuess(possible, colors)
    answer = checkGuess(guess, final)
    guesses += 1
    print "Guess %d: %s, %s, %s, %s - \t" % (guesses, guess[0], guess[1], guess[2], guess[3]), answer
    remove(possible, guess, answer)

  return guesses
  
#main
def mainProgram():
	Colors = ['Red', 'Orange', 'Green', 'Blue', 'Yellow', 'Brown']
	pegs = 4

	option = raw_input("Press s to start a new game or q to quit ")

	while(option != 'q'):
  	  if option == 's':
    	    n = PlayGame(Colors, pegs)
    	    print "This game took %d guesses" % (n)
  	  elif option == 'q':
    	    sys.exit(0)

          option = raw_input("Press s to start a new game or q to quit ")
 
if __name__ == '__main__':
  mainProgram()
