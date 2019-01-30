#include <iostream>

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

typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

const int MAX = 100000;

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

void testcase() {
	int n, m, l;

	std::cin >> n >> m >> l;

    Graph G(n);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder adder(G, capacitymap, weightmap, revedgemap);
   
	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);

	adder.addEdge(source, 0, l, 0);
	adder.addEdge(n - 1, target, l, 0);

	for (int i = 0; i < n - 1; i++) {
		adder.addEdge(i, i + 1, l, MAX);
	}

	for (int i = 0; i < m; i++) {
		int x, y, q;
		std::cin >> x >> y >> q;
		adder.addEdge(x, y, 1, (y - x) * MAX - q);
	}
   	
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
    long cost = boost::find_flow_cost(G);
	
	long flow = 0;
    OutEdgeIt e, eend;
	for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; e++) {
		if (capacitymap[*e] > 0) {
			flow += capacitymap[*e] - rescapacitymap[*e];
		}
	}

	std::cout << flow * MAX * (n - 1) - cost << std::endl;
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
}
