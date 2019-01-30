#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
	boost::property<boost::edge_residual_capacity_t, long,
	boost::property<boost::edge_reverse_t, Traits::edge_descriptor>
>>> Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

class EdgeAdder {
	Graph& G;
	EdgeCapacityMap& capacitymap;
	ReverseEdgeMap& revedgemap;

public:
	EdgeAdder(
		Graph& G,
		EdgeCapacityMap& capacitymap,
		ReverseEdgeMap& revedgemap):
	G(G),
	capacitymap(capacitymap),
	revedgemap(revedgemap) {}

	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = capacity;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase() {
	int n, m, s;
	std::cin >> n >> m >> s;
	
	Graph G(n + 1);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	EdgeAdder edge_adder(G, capacitymap, revedgemap);

	int source = 0;
	int target = n;


	for (int i = 0; i < m; i++) {
		int f, s;
		std::cin >> f >> s;
		edge_adder.addEdge(f, s, 1);
	}
	
	for (int i = 0; i < s; i++) {
		int store;
		std::cin >> store;
		edge_adder.addEdge(store, target, 1);
	}

	long flow = boost::push_relabel_max_flow(G, source, target);
	
	if (flow == s) {
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	std::cin >> t;
	
	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}
