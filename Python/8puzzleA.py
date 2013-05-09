from search import *


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

#main
A    = EightPuzzle('1','3',' ','8','2','4','7','6','5')
B    = EightPuzzle('1','3','4','8','6','2',' ','7','5')
C    = EightPuzzle('1','3',' ','4','2','5','8','7','6')
D    = EightPuzzle('7','1','2','8',' ','3','6','5','4')
E    = EightPuzzle('8','1','2','7',' ','4','6','5','3')
G    = EightPuzzle('1','2','3','8',' ','4','7','6','5')


startingConfig=[A, B, C, D, E, G]

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
	#breadth first tree search
	print "Apply breadth first tree search"
	print "\nProblem: %s" % (letter)
	print config
	breadth = breadth_first_tree_search(config)
	cost = config.path_cost(breadth.path_cost, config.initial, breadth_first_tree_search(config), config.goal)
	print "\nThe Solution to this problem has a cost of %d" % (cost)
	print "\nSolution:\n"
	for x in reversed(range(cost)):
  		print "%s %s %s\n%s %s %s\n%s %s %s" % (breadth.path()[x].state[0], breadth.path()[x].state[1], breadth.path()[x].state[2], 
  		breadth.path()[x].state[3], breadth.path()[x].state[4], breadth.path()[x].state[5], breadth.path()[x].state[6], 
		breadth.path()[x].state[7], breadth.path()[x].state[8])
  		print '\n'
		
	#debugging:
	#depth first tree search
	#print "Apply depth first search"
	#print "\nProblem: %s" % (letter)
	#print config
	#depth = depth_first_graph_search(config)
	#print "\nThe Solution to this problem has a cost of %d" % (config.path_cost(depth.path_cost, config.initial,
	#depth_first_tree_search(config), config.goal))
	#print "\nSolution:\n"
	#for x in reversed(range(config.path_cost(depth.path_cost, config.initial, depth_first_tree_search(config), config.goal))):
  		#print "%s %s %s\n%s %s %s\n%s %s %s" % (depth.path()[x].state[0], depth.path()[x].state[1], depth.path()[x].state[2], 
  		#depth.path()[x].state[3], depth.path()[x].state[4], depth.path()[x].state[5], depth.path()[x].state[6], 
		#depth.path()[x].state[7], depth.path()[x].state[8])
  		#print '\n'

	#breadth first graph search
	print "Apply breadth first graph search"
	print "\nProblem: %s" % (letter)
	print config
	breadth = breadth_first_graph_search(config)
	cost = config.path_cost(breadth.path_cost, config.initial, breadth_first_tree_search(config), config.goal)
	print "\nThe Solution to this problem has a cost of %d" % (cost)
	print "\nSolution:\n"
	for x in reversed(range(cost)):
  		print "%s %s %s\n%s %s %s\n%s %s %s" % (breadth.path()[x].state[0], breadth.path()[x].state[1], breadth.path()[x].state[2], 
  		breadth.path()[x].state[3], breadth.path()[x].state[4], breadth.path()[x].state[5], breadth.path()[x].state[6], 
		breadth.path()[x].state[7], breadth.path()[x].state[8])
  		print '\n'

	#depth limited search
	print "Apply depth limited search"
	print "\nProblem: %s" % (letter)
	print config
	search = depth_limited_search(config)
	cost = config.path_cost(breadth.path_cost, config.initial, breadth_first_tree_search(config), config.goal)
	print "\nThe Solution to this problem has a cost of %d" % (cost)
	print "\nSolution:\n"
	for x in reversed(range(cost)):
  		print "%s %s %s\n%s %s %s\n%s %s %s" % (search.path()[x].state[0], search.path()[x].state[1], search.path()[x].state[2], 
  		search.path()[x].state[3], search.path()[x].state[4], search.path()[x].state[5], search.path()[x].state[6], 
		search.path()[x].state[7], search.path()[x].state[8])
  		print '\n'


	#iterative deepening search
	print "Apply iterative deepening search"
	print "\nProblem: %s" % (letter)
	print config
	search = iterative_deepening_search(config)
	cost = config.path_cost(breadth.path_cost, config.initial, breadth_first_tree_search(config), config.goal)
	print "\nThe Solution to this problem has a cost of %d" % (cost)
	print "\nSolution:\n"
	for x in reversed(range(cost)):
  		print "%s %s %s\n%s %s %s\n%s %s %s" % (search.path()[x].state[0], search.path()[x].state[1], search.path()[x].state[2], 
  		search.path()[x].state[3], search.path()[x].state[4], search.path()[x].state[5], search.path()[x].state[6], 
		search.path()[x].state[7], search.path()[x].state[8])
  		print '\n'

