'''
generate-pref-graph.py
Generates a graph using the igraph Preference model
'''

import igraph
import sys

if len(sys.argv) != 4:
	print('Usage: python %s n m <output_file>.edges' % sys.argv[0])
	exit(0)

n = int(sys.argv[1])

m = int(sys.argv[2])

out_filename = sys.argv[3]

# constants
n = 512 # number of vertices
target_m = 512 * 64 # target number of edges
num_groups = 10 # number of groups / circles
in_group_pref = 0.9

# distribute nodes evenly among groups
type_dist = [float(1) / num_groups] * num_groups

# generate pref matrix
group_size = float(n) / num_groups;

num_in_edges = num_groups * group_size * (group_size - 1) / 2;

out_group_pref = float (target_m - num_in_edges * in_group_pref) / (n * (n - 1) / 2 - num_in_edges)

pref_matrix = []

for i in range(num_groups):
	row = []
	for j in range(num_groups):
		if i == j:
			row.append(in_group_pref)
		else:
			row.append(out_group_pref)
	pref_matrix.append(row)

g = igraph.Graph.Preference(n, type_dist, pref_matrix)

print('n %d' % g.vcount())
print('m %d' % g.ecount())
print('in_group_pref %f' % in_group_pref)
print('out_group_pref %f' % out_group_pref)
# print('density %f' % g.density())
# print('diameter %d' % g.diameter())
# print('radius %d' % g.radius())
# print('omega %d' % g.omega())

#output
with open(out_filename, 'w') as fout:
	for edge in g.es:
		fout.write(str(edge.source + 1) + ' ' + str(edge.target + 1) + '\n')
		fout.write(str(edge.target + 1) + ' ' + str(edge.source + 1) + '\n')