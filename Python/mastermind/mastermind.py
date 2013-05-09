#Matthew Metzger and Seth Barnard
#Solves Mastermind puzzles
#10/7/2012

import random
import sys

def checkGuess(state, final):
  return (getCP(state, final), getColor(state, final))

def getCP(state, final):
  count = 0
  for x in range(4):
    if state[x] == final[x]:
      count += 1
  return count

def getColor(state, final):
  count = 0
  for x in state:
    if x in final:
      count += 1
  return count - getCP(state, final)

def remove(possible, guess, answer):
  for x in possible:
    if answer != checkGuess(guess, x):
      possible.remove(x)

def PlayGame(colors, pegs):
  guess1 = []
  guess2 = []
  guess3 = []
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
  print "\nAnswers are in format: (Color & Position correct, Colors correct)"
  while n < pegs:
    foo = raw_input("\nEnter color %d " % ((n+1)))
    if foo in colors:
      n += 1
      final.append(foo)
    else:
      print "%s is not an available color" % (foo)

  #set guess1 to be the first 4 colors in the list
  for n in range(pegs):
    guess1.append(colors[n])
  
  #set guess2 to be items 2,3,4,5 from the list of colors
  for n in range(pegs):
    guess2.append(colors[(n+1)])

  #set guess3 to be items 3,4,5,6 from the list of colors
  for n in range(pegs):
    guess3.append(colors[(n+2)])
    
  #remove states that aren't consisitent with guess1
  answer = checkGuess(guess1, final)
  print "\nGuess 1: %s, %s, %s, %s - " % (guess1[0], guess1[1], guess1[2], guess1[3]), answer
  if answer == (4,0):
    return 1
  remove(possible, guess1, answer)

  #remove states that aren't consisitent with guess2
  answer = checkGuess(guess2, final)
  print "\nGuess 2: %s, %s, %s, %s - " % (guess2[0], guess2[1], guess2[2], guess2[3]), answer
  if answer == (4,0):
    return 2
  remove(possible, guess2, answer)
    
  #remove states that aren't consistent with guess3
  answer = checkGuess(guess3, final)
  print "\nGuess 3: %s, %s, %s, %s - " % (guess3[0], guess3[1], guess3[2], guess3[3]), answer
  if answer == (4,0):
    return 3
  remove(possible, guess3, answer)

  guesses = 3

  #guess first item in possible, get an anwer and remove inconsisitent states until
  #a correct answer is generated
  while answer != (4,0):
    #this is where minimax should be implemented
    #so that it doesn't just choose the first element
    guess = possible[0]
    answer = checkGuess(guess, final)
    guesses += 1
    print "\nGuess %d: %s, %s, %s, %s - " % (guesses, guess[0], guess[1], guess[2], guess[3]), answer
    remove(possible, guess, answer)

  return guesses
  
#main
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
  