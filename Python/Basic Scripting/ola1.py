import sys

f = open("p1in.sample", 'r')
while(f):
   a = f.readline()
   if 'x=8' in a:
      b = a.split(' ')
      c = int(b[4].split('=')[1])
      if c > 8000:
         print a
         b[2] = b[2].split('.')[0]
         print "%s %s %s" % (int(b[0][0:-1]), int(b[2]), b[4])
         break
f.close()
f = open("p1in.sample", 'r')
while(f):
   a = f.readline()
   if 'y=9' in a:
      b = a.split(' ')
      c = int(b[4].split('=')[1])
      if c > 9000:
         print a
         b[2] = b[2].split('.')[0]
         print "%s %s %s" % (int(b[0][0:-1]), int(b[2]), b[4])
         break

         

   