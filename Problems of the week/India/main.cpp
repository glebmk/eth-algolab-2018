#include <iostream>
#include <cstdlib>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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

const long MAX = 1000000000;

void testcase() {
	int c, g, start, finish;
	long b;
	cin >> c >> g >> b >> start >> finish;	 	

	Graph G(c);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder adder(G, capacitymap, weightmap, revedgemap);

	for (int i = 0; i < g; i++) {
		int x, y, w, c;	
		cin >> x >> y >> w >> c;
		assert(w >= 1);
		adder.addEdge(x, y, c, w);
	}

	int source = add_vertex(G);
	Edge e = adder.addEdge(source, start, 0, 0);

	long left = 0;
	long right = b;
//	long right = b / distmap[finish] + 1;
	while (left + 1 < right) {
		long mid = (left + right) / 2;
		capacitymap[e] = mid;
		boost::successive_shortest_path_nonnegative_weights(G, source, finish);
		long cost = boost::find_flow_cost(G);
		if (cost <= b) {
			left = mid;
		} else {
			right = mid;
		}
	}

	capacitymap[e] = right;
	boost::successive_shortest_path_nonnegative_weights(G, source, finish);
	long cost = boost::find_flow_cost(G);
	if (cost <= b) {
		cout << capacitymap[e] - rescapacitymap[e] << endl;
		return;
	}
	capacitymap[e] = left;
	boost::successive_shortest_path_nonnegative_weights(G, source, finish);
	cost = boost::find_flow_cost(G);
	assert(cost <= b);	
	cout << capacitymap[e] - rescapacitymap[e] << endl;
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
