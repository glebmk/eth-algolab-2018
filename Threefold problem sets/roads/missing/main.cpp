#include <iostream>
#include <cstdlib>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    Edge addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    	return e;
	}
};

void testcase() {
	int n, m, k;

	cin >> n >> m >> k;
	
	Graph G(n + m);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder adder(G, capacitymap, weightmap, revedgemap);
   
	Vertex source = add_vertex(G);
	Vertex start = add_vertex(G);
	Vertex finish = add_vertex(G);

	Edge source2start = adder.addEdge(source, start, k, 0);
	
	for (int i = 0; i < n; i++) {
		adder.addEdge(start, i, 1, 0);
	}

   	for (int i = 0; i < m; i++) {
		int s, t, c;
		cin >> s >> t >> c;
		s--;
		t--;
		adder.addEdge(s, n + i, 1, 0);
		adder.addEdge(t, n + i, 1, 0);
		adder.addEdge(n + i, finish, 1, c); 
	}

	boost::successive_shortest_path_nonnegative_weights(G, source, finish);
    int flow = capacitymap[source2start] - rescapacitymap[source2start];
	int cost = boost::find_flow_cost(G);
	if (flow < k) {
		cout << "no" << endl;
	} else {
		cout << cost << endl;
	}
}

int main() {
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
	return 0;
}
