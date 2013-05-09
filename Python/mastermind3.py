import copy
import random
import sys
from random import choice

def checkGuess(state, final):
  x = getCP(state, final)
  y = getColor(state, final)
  return (x, y-x)

def getCP(state, final):
  count = 0
  for x in range(4):
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

def PlayGame(colors, pegs):
  print ("\nColors are: %s %s %s %s %s %s" % (colors[0], colors[1], colors[2], colors[3], colors[4], colors[5]))
  print ("Answers are in form (Color & Position correct, Color correct)")
  final = [choice(colors), choice(colors), choice(colors), choice(colors)]
  answer = (0,0) #initialize answer
  number_guess = 0
  number_color = 0

  while answer!= (4,0):
    guess = []
    number_guess+=1

    if number_guess > 12:
      print ("You have run out of guesses, you lose :(")
      print ("Correct answer is: %s %s %s %s" % (final[0], final[1], final[2], final[3]))
      break
      
    print ("Guess %d" % (number_guess))
    number_color = 0

    while number_color<pegs:
      foo = input("Enter color %d or enter 'Give Up' to quit: " % (number_color+1))

      if foo == 'Give Up':
        print ("Correct answer is: %s %s %s %s" % (final[0], final[1], final[2], final[3]))
        guess = final
        break

      if foo in colors:
        guess.append(foo)
        number_color+=1
      else:
        print ("%s is not an available color" % (foo))

    answer = checkGuess(guess, final)
    print ("\nGuess %d: %s, %s, %s, %s - " % (number_guess, guess[0], guess[1], guess[2], guess[3]), answer)

  print ("This game took you %d turns" % (number_guess))

#main
Colors = ['Red', 'Orange', 'Green', 'Blue', 'Yellow', 'Brown']
pegs = 4

option = input("Press s to start a new game or q to quit ")

while(option != 'q'):
  if option == 's':
    PlayGame(Colors, pegs)
  elif option == 'q':
    sys.exit(0)

  option = input("Press s to start a new game or q to quit ")
