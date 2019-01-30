#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
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
typedef	boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

struct MyEdge {
	int from, to;
	long cost;	
};

typedef std::vector<MyEdge> MyEdges;

class EdgeAdder {
	Graph& G;
	EdgeCapacityMap& capacitymap;
	ReverseEdgeMap& revedgemap;
	MyEdges& edges;
	
public:
	EdgeAdder(
		Graph& G,
		EdgeCapacityMap& capacitymap,
		ReverseEdgeMap& revedgemap,
		MyEdges& edges 
		):
	G(G),
	capacitymap(capacitymap),
	revedgemap(revedgemap),
	edges(edges) {}

	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
		edges.push_back({from, to, capacity});
		//std::cerr << "from " << from << " to " << to << " capacity " << capacity << std::endl;
	}
};

std::vector<bool> bfs_on_res_graph(Graph& G, ResidualCapacityMap& rescapacitymap, int n, int src) {
    // BFS to find vertex set S
	std::vector<bool> vis(n, false); // visited flags
	std::queue<int> Q; // BFS queue (from std:: not boost::)
	vis[src] = true; // Mark the source as visited
	Q.push(src);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}
	return vis;
}

struct Ret {
	long cost;
	std::set<int> vertexes;
};

Ret cut_cost(Vertex source, Vertex target, Graph& G, ResidualCapacityMap& rescapacitymap, MyEdges myedges, int n) {
	int flow = boost::push_relabel_max_flow(G, source, target);
	std::vector<bool> visited = bfs_on_res_graph(G, rescapacitymap, n, source);

	std::set<int> left, right;
	for (int i = 0; i < n; i++) {
		if (visited[i]) {
			left.insert(i);
		} else {
			right.insert(i);
		}
	}

	long left_cost = 0;
	long right_cost = 0;

	for (MyEdge edge : myedges) {
		bool from_left = left.count(edge.from) > 0;
		bool to_left = left.count(edge.to) > 0;
		if (from_left && !to_left) {
			left_cost += edge.cost;
		} else if (!from_left && to_left) {
			right_cost += edge.cost;
		}
	}
	if (left_cost <= right_cost) {
		// std::cerr << "left cost" << std::endl;
		return {left_cost, left};
	} else {
		// std::cerr << "right cost" << std::endl;
		return {right_cost, right};
	}
}

void testcase() {
	int n, m;
	std::cin >> n >> m;
	
	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	MyEdges myedges;
	EdgeAdder adder(G, capacitymap, revedgemap, myedges);
	for (int i = 0; i < m; i++) { 
		// Edge from a to b with cost c
		int a, b, c;
		std::cin >> a >> b >> c;
		adder.addEdge(a, b, c);
	}	
	
	long min_cost = -1;
	std::set<int> vertexes;
	for (int i = 1; i < n; i++) {
		Ret r = cut_cost(0, i, G, rescapacitymap, myedges, n);
		if (min_cost == -1 || r.cost < min_cost) {
			min_cost = r.cost;
			vertexes = r.vertexes;
		}
		r = cut_cost(i, 0, G, rescapacitymap, myedges, n);
		if (min_cost == -1 || r.cost < min_cost) {
			min_cost = r.cost;
			vertexes = r.vertexes;
		}
	}
	
	std::cout << min_cost << std::endl;
	std::cout << vertexes.size() << " ";
	for (int elem : vertexes) {
		std::cout << elem << " ";
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
