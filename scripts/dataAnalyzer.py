#!/usr/bin/python3
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import csv
import math
import sys

def csvToColumns(filename):
  points = csv.reader(open(filename))
  columns = []

  for point in points:
    for i in range(len(point)):
        if i >= len(columns):
          columns.append([])

        try:
            columns[i].append(float(point[i]))
        except:
            pass

  return columns

def lineSeg2PointDist(P1x, P1y, P2x, P2y, Rx, Ry):
  projScalar = ((Rx - P1x)*(P2x - P1x) + (Ry - P1y)*(P2y - P1y)) / ((P2x - P1x)*(P2x - P1x) + (P2y - P1y)*(P2y - P1y))
  #print("\n"+str(projScalar))

  distsqr = 0
  if projScalar >= 1:
    distsqr = (P2x - Rx) ** 2 + (P2y - Ry) ** 2
  elif projScalar <= 0:
    distsqr = (P1x - Rx) ** 2 + (P1y - Ry) ** 2
  else:
    distsqr = (P1x - Rx) ** 2 + (P1y - Ry) ** 2 - ((projScalar ** 2) * ((P2x - P1x) ** 2 + (P2y - P1y) ** 2))

  #print(distsqr)

  cross = (P2x - P1x) * (Ry - P1y) - (P2y - P1y) * (Rx - P1x)

  # in the event the error is actually 0, rounding errors may cause distsqr to be negative,
  # causing errors when the sqrt is taken
  if abs(distsqr) < 10 ** -12:
    distsqr = 0

  return math.sqrt(distsqr) if cross >= 0 else -1 * math.sqrt(distsqr)


fig, ax = plt.subplots()

#ax.set_xlim(-61, 305)
#ax.set_ylim(-61, 305)

path = csvToColumns('paths/path1.4.csv')
ax.plot(path[0], path[1], 'b')

robot = csvToColumns(str(sys.argv[1]))
ax.plot(robot[1], robot[2], 'r')

ax.set(xlabel='X (cm)', ylabel='Y (cm)', title='Path')
ax.grid()

fig.savefig("analysis/" + str(sys.argv[1])[5:-4] + ".png")

lastJ = 0
error = []
for i in range(len(robot[0])):
  lastDist = float('inf')
  for j in range(lastJ, len(path[0]) - 1):
    dist = lineSeg2PointDist(path[0][j], path[1][j], path[0][j+1], path[1][j+1], robot[1][i], robot[2][i])
    #print("%s (%s, %s)\t %s (%s,%s)\t dist = %s" % (i, robot[1][i], robot[2][i], j, path[0][j], path[1][j], dist))
    if(abs(dist) > abs(lastDist)):
      #print("selecting %s:%s (%s)" % (i, lastJ, lastDist))
      error.append(lastDist)
      break
    else:
      lastDist = dist
      lastJ = j
  if lastJ == len(path[0]) - 2:
    error.append(lastDist)

# ax.plot(range(len(error)), error, 'b')

plt.show()
