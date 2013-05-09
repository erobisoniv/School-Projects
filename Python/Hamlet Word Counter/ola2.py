import sys

d = {}
for x in sys.argv[2:]: d[x] = 0
for k in d:
   for line in open(sys.argv[1]):
      if line.startswith("  " + k + "."):
	 b = line.split()
         d[k] += len(b) - 1
      if line.startswith("    ") and line[4] != ' ':
	    b = line.split()
            d[k] += len(b)
for k in d:
   print "%s %d" % (k, d[k])
