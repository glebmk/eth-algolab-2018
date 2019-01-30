#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
	boost::property<boost::edge_residual_capacity_t, long,
	boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
	boost::property<boost::edge_weight_t, long>
	>>>> Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;
typedef boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeightMap;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;

class EdgeAdder {
	Graph& G;
	EdgeCapacityMap& capacitymap;
	ReverseEdgeMap& revmap;
	EdgeWeightMap& weightmap;

public:
	EdgeAdder(Graph& G, EdgeCapacityMap& capacitymap, ReverseEdgeMap& revmap, EdgeWeightMap& weightmap) :
		G(G), capacitymap(capacitymap), revmap(revmap), weightmap(weightmap) {}

	void add_edge(int from, int to, long capacity, long weight) {
		bool success;
		Edge e, rev_e;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);

		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;

		revmap[e] = rev_e;
		revmap[rev_e] = e;

		weightmap[e] = weight;
		weightmap[rev_e] = -weight;
	}
};

const int MAX = 200;

void testcase() {
	int n, m, s;
	std::cin >> n >> m >> s;
	Graph G(n + m + s);
	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);
	
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revmap = boost::get(boost::edge_reverse, G);
	EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);

	EdgeAdder adder(G, capacitymap, revmap, weightmap);
	
	for (int i = 0; i < n; i++) {
		adder.add_edge(source, i, 1, 0);
	}

	for (int i = 0; i < s; i++) {
		int quota;
		std::cin >> quota;

		adder.add_edge(n + m + i, target, quota, 0);
	}

	for (int i = 0; i < m; i++) {
		int state;
		std::cin >> state;
		adder.add_edge(n + i, n + m + state - 1, 1, 0);
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int cost;
			std::cin >> cost;
			adder.add_edge(i, n + j, 1, MAX-cost);
		}
	}

	long flow = boost::push_relabel_max_flow(G, source, target);
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
	long cost = boost::find_flow_cost(G);

	std::cout << flow << " " << -cost + MAX * flow << std::endl;
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
