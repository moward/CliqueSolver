'''
Graph normalizer
'''

import sys
import re

if len(sys.argv) < 3:
    print('Not enough arguments')

in_filename = sys.argv[1]

out_filename = sys.argv[2]

line_format = re.compile("^(\d+) (\d+)$")

edgeMap = {}
vertexCount = 0

def getNormalVertex(u):
	global edgeMap, vertexCount
	if not u in edgeMap:
		vertexCount += 1
		edgeMap[u] = vertexCount
	return edgeMap[u]

with open(in_filename) as fin:
	with open(out_filename, 'w') as fout:
		for line in fin:
			line_parsed = line_format.match(line)
			if not line_parsed:
				raise ValueError('Invalid line: ' + line)
			u = int(line_parsed.group(1))
			v = int(line_parsed.group(2))

			fout.write(str(getNormalVertex(u)) + ' ' + str(getNormalVertex(v)) + '\n')
