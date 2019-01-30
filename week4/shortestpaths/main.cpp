#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;


// Directed graph with integer weights on edges.
typedef adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, int>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;	// Vertex type		
typedef graph_traits<Graph>::edge_descriptor		Edge;	// Edge type
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	// Edge iterator
typedef property_map<Graph, edge_weight_t>::type	WeightMap;


int distance(Graph& G, int n, int s, int t) {
	std::vector<int> distmap(n);		// We must use at least one of these
	dijkstra_shortest_paths(G, s,
		distance_map(make_iterator_property_map(	// concatenated by .
		distmap.begin(), get(vertex_index, G))));
	
	return distmap[t];
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int n, m, q;
	std::cin >> n >> m >> q;
	
	Graph G(n);
	WeightMap weightmap = get(edge_weight, G);
	for (int i = 0; i < n; i++) {
		int discard;
		std::cin >> discard >> discard;
	}

	for (int i = 0; i < m; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;
		Edge e;	bool success;	
		tie(e, success) = add_edge(a, b, G);
		weightmap[e] = c;
	}
	
	for (int i = 0; i < q; i++) {
		int s, t;
		std::cin >> s >> t;
		//std::cout << "s " << s << " t " << t << std::endl;
		int d = distance(G, n, s, t);
		if (d == 2147483647) {
			std::cout << "unreachable" << std::endl;
		} else {
			std::cout << d << std::endl;
		}
	}
}

