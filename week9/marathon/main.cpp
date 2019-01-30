#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; 

typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; 
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; 

void printv(vector<long> v) {
	for (long elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

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

struct My {
	long a, b, c, d;
};

void testcase() {
	int n, m, s, f;
	cin >> n >> m >> s >> f;

	Graph G(n);
	EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
	
	vector<My> edges;	
	for (int i = 0; i < m; i++) {
		long a, b, c, d;
		cin >> a >> b >> c >> d;
		My my {a, b, c, d};
		edges.push_back(my);
		Edge e; bool success;
		boost::tie(e, success) = add_edge(a, b, G);
		weightmap[e] = d;
		boost::tie(e, success) = add_edge(b, a, G);
		weightmap[e] = d;
	}

	vector<long> froms(n);
	boost::dijkstra_shortest_paths(G, s,
		boost::distance_map(boost::make_iterator_property_map(froms.begin(), boost::get(boost::vertex_index, G))));

	vector<long> fromf(n);
	boost::dijkstra_shortest_paths(G, f,
		boost::distance_map(boost::make_iterator_property_map(fromf.begin(), boost::get(boost::vertex_index, G))));

	Graph G2(n);	
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G2);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G2);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G2);
	EdgeAdder adder(G2, capacitymap, revedgemap);

	/*
	cerr << "froms" << endl;
	printv(froms);
	cerr << "fromf" << endl;
	printv(fromf);
	*/

	long shortest = froms[f];
	for (auto& e : edges) {
		if (froms[e.a] >= LONG_MAX) {
			continue;
		}
		//cerr << "a " << e.a << " b " << e.b << " d " << e.d << endl;
		if (froms[e.a] + e.d + fromf[e.b] == shortest) {
			//cerr << "first" << endl;
			adder.addEdge(e.a, e.b, e.c);
		} 
		if (froms[e.b] + e.d + fromf[e.a] == shortest) {
			//cerr << "second" << endl;
			adder.addEdge(e.b, e.a, e.c);
		}
	}

	long flow = boost::push_relabel_max_flow(G2, s, f);
	cout << flow << endl;
}

int main() {
	int t;
	cin >> t;
	while (t) {
		//cerr << "testcase" << endl;
		t--;
		testcase();
	}
}
