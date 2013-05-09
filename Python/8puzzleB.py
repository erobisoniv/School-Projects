from search import *
import math

class EightPuzzle(Problem):
  def __init__(self, nw, n, ne, w, c, e, sw, s, se, initial = None, goal = None):
  	  self.nw = nw
  	  self.n = n
  	  self.ne = ne
  	  self.w = w
  	  self.c = c
  	  self.e = e
  	  self.sw = sw
  	  self.s = s
  	  self.se = se
  	  self.state = [nw, n, ne, w, c, e, sw, s, se]
  	  self.initial = self.state
  	  self.goal = ['1','2','3','8',' ','4','7','6','5']
  
  def successor(self, other):
  	  value = []
  	  if other[0] == ' ':
  	  	  value.append(('Right', self.MoveRight(other)))
  		  value.append(('Down', self.MoveDown(other)))
  	  elif other[1] == ' ':
  		  value.append(('Left', self.MoveLeft(other)))
  		  value.append(('Right', self.MoveRight(other)))
  		  value.append(('Down', self.MoveDown(other)))
  	  elif other[2] == ' ':
  		  value.append(('Left', self.MoveLeft(other)))
  		  value.append(('Down', self.MoveDown(other)))
  	  elif other[3] == ' ':
  		  value.append(('Up', self.MoveUp(other)))
  		  value.append(('Down', self.MoveDown(other)))
  		  value.append(('Right', self.MoveRight(other)))
  	  elif other[4] == ' ':
  		  value.append(('Up', self.MoveUp(other)))
  		  value.append(('Down', self.MoveDown(other)))
  		  value.append(('Right', self.MoveRight(other)))
  		  value.append(('Left', self.MoveLeft(other)))
  	  elif other[5] == ' ':
  		  value.append(('Up', self.MoveUp(other)))
  		  value.append(('Down', self.MoveDown(other)))
  		  value.append(('Left', self.MoveLeft(other)))
  	  elif other[6] == ' ':
  		  value.append(('Up', self.MoveUp(other)))
  		  value.append(('Right', self.MoveRight(other)))
  	  elif other[7] == ' ':
  		  value.append(('Up', self.MoveUp(other)))
  		  value.append(('Right', self.MoveRight(other)))
  		  value.append(('Left', self.MoveLeft(other)))
  	  elif other[8] == ' ':
  		  value.append(('Up', self.MoveUp(other)))
  		  value.append(('Left', self.MoveLeft(other)))
  	  else:
  		  print "invalid board configuration"
  	  return value
  	  
  def goal_test(self, other):
  	  for x in range(9):
  	  	  if other[x] != self.goal[x]:
  	  	  	  return False
  	  return True	  
  def __repr__(self):
  	  return "%s %s %s\n%s %s %s\n%s %s %s" % (self.state[0], self.state[1], self.state[2], self.state[3], self.state[4], self.state[5], self.state[6], self.state[7], self.state[8])

  def MoveLeft(self, state):
    	  copystate = []
  	  copystate.extend(state)

  	  blank = copystate.index(' ')
     	  copystate[blank], copystate[(blank - 1)] = copystate[(blank - 1)], copystate[blank]


  	  return copystate
  def MoveRight(self, state):
    	  copystate = []
  	  copystate.extend(state)

  	  blank = copystate.index(' ')
     	  copystate[blank], copystate[(blank + 1)] = copystate[(blank + 1)], copystate[blank]

  	  return copystate
  def MoveUp(self, state):
    	  copystate = []
  	  copystate.extend(state)

  	  blank = copystate.index(' ')
     	  copystate[blank], copystate[(blank - 3)] = copystate[(blank - 3)], copystate[blank]

  	  return copystate
  def MoveDown(self, state):
    	  copystate = []
  	  copystate.extend(state)

  	  blank = copystate.index(' ')
     	  copystate[blank], copystate[(blank + 3)] = copystate[(blank + 3)], copystate[blank]

  	  return copystate

#Heuristic Functions
#-------------------------------------------------------------------------------------------------------
# Number of incorrectly placed tiles
def H1(state):
  count = 0
  goal = ['1','2','3','8',' ','4','7','6','5']
  for x in range(9):
    if state[x] != goal[x]:
      count += 1
  return count

#sum of (vertical + horizontal) distance from current position of each tile to it's goal state
def H2(state):
  goal = ['1','2','3','8',' ','4','7','6','5']
  answer = 0
  for x in range(9):
    if state[x] != goal[x]:
      vertical = (x / 3) - state.index(goal[x]) /3
      answer += abs(vertical)
      horizontal = state.index(goal[x]) - (x - 3*vertical)
      answer += abs(horizontal)
  return answer

#sum of distance from current position of each tile(H2) + distance from blank tile to tile that is adjacent to each incorrectly
#placed tile.  Logic:  to move a tile it must be adjacent to a blank so the blank tile must be moved next to
#any tile you wish to move.
def H3(state):
  answer = H2(state)
  goal = ['1','2','3','8',' ','4','7','6','5']
  blank = state.index(' ')
  for x in range(9):
    if state[x] != goal[x]:
      if x % 3 != 0:
        left = Adjacent((x-1), blank) # distance from blank to left adjacent tile
        answer += left
      else:
        right = Adjacent((x+1), blank) #distance from blank to right adjacent tile
        answer += right
  return answer

def Adjacent(blank, x):
  answer = 0
  vertical = (blank / 3) - (x/3)
  answer += abs(vertical)
  horizontal = (x) - (blank - 3*vertical)
  answer += abs(horizontal)
  return answer

def EBF(N, d):
  exp = math.log((N+1)) / (d + 1)
  return pow(math.e, exp)

#main
A    = EightPuzzle('1','3',' ','8','2','4','7','6','5')
B    = EightPuzzle('1','3','4','8','6','2',' ','7','5')
C    = EightPuzzle('1','3',' ','4','2','5','8','7','6')
D    = EightPuzzle('7','1','2','8',' ','3','6','5','4')
E    = EightPuzzle('8','1','2','7',' ','4','6','5','3')
G    = EightPuzzle('1','2','3','8',' ','4','7','6','5')


Heuristics = [H1, H2, H3]

startingConfig=[A, B, C, D, E, G]

for Heuristic in Heuristics:
	if Heuristics.index(Heuristic) == 0:
		name = "H1: Misplaced Tiles"
	elif Heuristics.index(Heuristic) == 1:
		name = "H2: Manhattan distance"
	else:
		name = "H3: My Heuristic"

	print "\nHeuristic function %s\n===================\n" % (name)


	for config in startingConfig:
		if startingConfig.index(config) == 0:
			letter = "A"
		elif startingConfig.index(config) == 1:
			letter = "B"
		elif startingConfig.index(config) == 2:
			letter = "C"
		elif startingConfig.index(config) == 3:
			letter = "D"
		elif startingConfig.index(config) == 4:
			letter = "E"
		elif startingConfig.index(config) == 5:
			letter = "G"

		oneIProblem = InstrumentedProblem(config)
		oneSearch = astar_search(oneIProblem, Heuristic)
		bstar = EBF(oneIProblem.states, oneSearch.depth)

		print "problem %s:\ntotal number of nodes generated: %d\ndepth of the goal: %d\n\
effective branching factor: %f\n" % (letter, oneIProblem.states, oneSearch.depth, bstar)

  	  	










