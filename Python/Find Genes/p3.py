import sys, re
c = 0
REGEX = r'(atg.{14,94}tag)'
for line in open(sys.argv[1]):
   s = line.split()
   for m in re.finditer(REGEX, s[1]):
      if (m.end() - m.start())%3 == 0 and (m.end() - m.start()) > 20 and (m.end() - m.start()) < 100:
         print '%s %03d %03d' % (s[0][3:], m.start(), (m.end() - m.start()))
         c += 1
print c
 