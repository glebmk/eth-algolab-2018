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
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void add_helper(int p1, int p2, EdgeAdder& adder, int source, int target, int w) {
	if (p1 > p2) {
		std::swap(p1, p2);
	}

	int left, right;
	
	if (p1 == 0) {
		left = source;
	} else {
		left = (p1 - 1) * 2 + 1;
	}

	if (p2 == w) {
		right = target;
	} else {
		right = (p2 - 1) * 2;
	}
    //	std::cout << "left " << left << " right " << right << std::endl;
	adder.addEdge(left, right, 1);
}

void testcase() {
	int w, n;
	std::cin >> w >> n;
	
	// 0 and w - are two edge points.
	Graph G(2 * (w - 1));
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	EdgeAdder adder(G, capacitymap, revedgemap);

	for (int i = 0; i < w - 1; i++) {
		adder.addEdge(2 * i, 2 * i + 1, 1);
	}

	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);

	for (int i = 0; i < n; i++) {
		int p1, p2;
		std::cin >> p1 >> p2;
		add_helper(p1, p2, adder, source, target, w);	
	}

	long flow = boost::push_relabel_max_flow(G, source, target);
	std::cout << flow << std::endl;
}

int main() {
	int t; 
	std::cin >> t;

	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}	
