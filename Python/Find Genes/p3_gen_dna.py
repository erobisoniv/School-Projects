#!/usr/bin/env python

import sys, random

nchroms = 1000

nucs = ( 'a', 'c', 'g', 't' )

minChromLen = 500
maxChromLen = 2000

for i in range(0,nchroms):
    lenChrom = random.randint(minChromLen,maxChromLen)
    chrom = [ random.choice(nucs) for j in range(lenChrom) ]
    chrom = ''.join(chrom)
    print "SEQ%04d %s"  %  (i,chrom)
