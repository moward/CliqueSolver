'''
graph-stats.ph
Given an edge file, produces statistics about the graph

Graph is assumed to be normalized, i.e. vertices numbered continuously
as 1 ... n
'''

import sys
import igraph
import re
import time

# helpers
def ensure_vertex(g, v):
	if g.vcount() <= v:
		g.add_vertices(v - g.vcount() + 1)

stats_list = []

def output_stat(name, value):
	global stats_list
	print(name.ljust(20) + ' = ' + str(value))
	stats_list.append(str(value))

if len(sys.argv) != 2:
	print('Usage: python %s <filename>.edges' % sys.argv[0])
	exit(0)

g = igraph.Graph()

in_filename = sys.argv[1]

edges = []

with open(in_filename) as fin:
	for line in fin:
		line_parsed = line.split(' ')
		if not len(line_parsed) == 2:
			raise ValueError('Invalid line: ' + line)

		# note: vertices are 0 indexed, not 1 indexed
		u = int(line_parsed[0]) - 1
		v = int(line_parsed[1]) - 1

		ensure_vertex(g, u)
		ensure_vertex(g, v)
		edges.append((u, v))

	g.add_edges(edges)

	output_stat('Analyzing', in_filename)

	# number of edges
	output_stat('n', g.vcount())
	output_stat('m', g.ecount())
	output_stat('density', g.density())

	output_stat('diameter', g.diameter())

	output_stat('radius', g.radius())

	clique_start = time.clock()

	output_stat('omega', g.omega())

	clique_end = time.clock()

	output_stat('omega time', clique_end - clique_start)

	clusters = g.clusters()

	output_stat('# CC', len(clusters))

	print(','.join(stats_list))