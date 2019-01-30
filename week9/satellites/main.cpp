#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/breadth_first_search.hpp>

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
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef	boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

class EdgeAdder {
	Graph& G;
	EdgeCapacityMap& capacitymap;
	ReverseEdgeMap& revmap;
public:
	EdgeAdder(Graph& G, EdgeCapacityMap& capacitymap, ReverseEdgeMap& revmap) :
		G(G), capacitymap(capacitymap), revmap(revmap) {}

	void add_edge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
	
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		
		revmap[e] = rev_e;
		revmap[rev_e] = e;
	}

};

void init_graph(Graph& G, int source, int target, int g, int s, int l) {
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revmap = boost::get(boost::edge_reverse, G);

	EdgeAdder adder(G, capacitymap, revmap);

	for (int i = 0; i < l; i++) {
		int ground, sat;
		std::cin >> ground >> sat;
		adder.add_edge(ground, g + sat, 1);	
	}
	
	for (int i = 0; i < g; i++) {
		adder.add_edge(source, i, 1);
	}

	for (int i = 0; i < s; i++) {
		adder.add_edge(g + i, target, 1);
	}
}

std::set<int> get_visited(Graph& G, ResidualCapacityMap& rescapacitymap, int n, int src) { 
	// BFS to find vertex set S
	std::vector<bool> vis(n, false); // visited flags
	std::set<int> visited;
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[src] = true; // Mark the source as visited
	visited.insert(src);
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			visited.insert(v);
			Q.push(v);
		}
	}
	return visited;
}

void testcase() {
	int g, s, l;
	std::cin >> g >> s >> l;
	
	Graph G(g + s);
	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);
	init_graph(G, source, target, g, s, l);

	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	long flow = boost::push_relabel_max_flow(G, source, target);
	std::set<int> visited = get_visited(G, rescapacitymap, g + s + 2, source);

	std::vector<int> outg, outs;
	for (int i = 0; i < g; i++) {
		if (!visited.count(i)) {
			outg.push_back(i);
		}
	}

	for (int i = 0; i < s; i++) {
		if (visited.count(g + i)) {
			outs.push_back(i);
		}
	}
	
	std::cout << outg.size() << " " << outs.size() << std::endl;
	for (int v : outg) {
		std::cout << v << " ";
	}
	for (int v : outs) {
		std::cout << v << " ";
	}
	std::cout << std::endl;
}

int main() {
	int t;
	std::cin >> t;

	for (int i = 0; i < t; i++) {
		testcase();
	}
	
	return 0;
}

