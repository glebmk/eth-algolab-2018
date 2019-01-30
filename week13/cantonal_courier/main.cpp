#include <iostream>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;

typedef std::vector<int> Vec;

const int MAX = 10000000;

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	//TODO[gleb] maybe should return the edge.
	// to use the Function (add an edge)
	Edge addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
		return e;
	}
};

bool has_bad_zone(int job, std::set<int>& bad_zones, std::vector<Vec>& job2zone) {
	Vec& zones = job2zone[job];
	for (int z : zones) {
		if (bad_zones.count(z)) {
			return true;
		}
	}
	return false;
}

void calc(std::set<int>& bad_zones, std::vector<Vec>& job2zone, Vec& zoneprice, Vec& jobprofit) {
	int Z = zoneprice.size();
	int J = jobprofit.size();

	Graph G(J + Z);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder adder(G, capacitymap, revedgemap);

	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	for (int j = 0; j < J; j++) {
		if (has_bad_zone(j, bad_zones, job2zone)) {
			continue;
		}
		adder.addEdge(source, j, jobprofit[j]);
		for (int z : job2zone[j]) {
			adder.addEdge(j, J + z, MAX);
		}
	}

	std::vector<Edge> edges;
	for (int z = 0; z < Z; z++) {
		if (bad_zones.count(z)) {
			continue;	
		}
		Edge edge = adder.addEdge(J + z, target, zoneprice[z]);
		edges.push_back(edge);
	}

	long flow = boost::push_relabel_max_flow(G, source, target);

	for (Edge& edge : edges) {
		if (rescapacitymap[edge] > 0) {
			bad_zones.insert(boost::source(edge, G) - J);
		}
	}
}

Vec readvec(int size) {
	Vec vec;
	for (int i = 0; i < size; i++) {
		int elem;
		std::cin >> elem;
		vec.push_back(elem);
	}
	return vec;
}

void printset(std::set<int>& s) {
	for (int elem : s) {
		std::cerr << elem << " ";
	}
	std::cerr << std::endl;
}

void testcase() {
	int Z, J;
	std::cin >> Z >> J;

	Vec zoneprice = readvec(Z);
	Vec jobprofit = readvec(J);

	std::vector<Vec> job2zone;
	for (int i = 0; i < J; i++) {
		int n;
		std::cin >> n;
		Vec zones = readvec(n);
		job2zone.push_back(zones);
	}

	std::set<int> bad_zones;
	int bad_zones_size = 0;

	calc(bad_zones, job2zone, zoneprice, jobprofit);
	while (bad_zones_size != bad_zones.size()) {
		bad_zones_size = bad_zones.size();
		calc(bad_zones, job2zone, zoneprice, jobprofit);
	}

	//std::cerr << "bad zones" << std::endl;
	//printset(bad_zones);

	int profit = 0;	
	for (int j = 0; j < J; j++) {
		if (!has_bad_zone(j, bad_zones, job2zone)) {
			profit += jobprofit[j]; 
		}
	}

	for (int z = 0; z < Z; z++) {
		if (!bad_zones.count(z)) {
			profit -= zoneprice[z];
		}
	}
	std::cout << profit << std::endl;
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}


