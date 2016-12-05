'''
generate-graph.ph
Generates a random graph, given a number of vertices and edges
Usage: n m output.edges
'''

import sys
import random

if len(sys.argv) != 4:
    raise ValueError('Not enough arguments')

n = int(sys.argv[1])

m = int(sys.argv[2])

if m % 2 != 0:
	raise ValueError('m must be even')

# only want to pick edges such that (u < v) and then will double them up
m = m / 2

out_filename = sys.argv[3]

# generate all edges
complete_graph = []

for u in range(1, n + 1):
	for v in range (u + 1, n + 1):
		complete_graph.append((u, v))

with open(out_filename, 'w') as fout:
	random_subgraph = random.sample(complete_graph, m)
	for edge in random_subgraph:
		fout.write(str(edge[0]) + ' ' + str(edge[1]) + '\n')
		fout.write(str(edge[1]) + ' ' + str(edge[0]) + '\n')