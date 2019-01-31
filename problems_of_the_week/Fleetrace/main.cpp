#include <iostream>
#include <boost/graph/adjacency_list.hpp>
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
                boost::property <boost::edge_weight_t, long> > > > > Graph; 
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt;
 
long find(int cap, EdgeCapacityMap& capacitymap, ResidualCapacityMap& rescapacitymap, Graph& G, Edge& e, int source, int finish);
	
void printv(vector<long>& v) {
	for (long elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

/* This function was used to empirically confirm that cost function of flow is unimodal */
bool isunimodal(vector<long>& res) {
	bool increase = true;
	for (int i = 0; i < res.size() - 1; i++) {
		if (res[i + 1] > res[i] && !increase) {
			return false;	
		} else if (res[i + 1] < res[i]) {
			increase = false;
		}
	}
	return true;
}

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
        weightmap[e] = w;
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w;
        revedgemap[e] = rev_e;
        revedgemap[rev_e] = e;
		return e;
    }
};

const int MAX = 50;

class ResFinder {
	EdgeCapacityMap& capacitymap;
	ResidualCapacityMap& rescapacitymap;
	Graph& G;
	Edge& e;
	int source;
	int finish;
public:
	ResFinder(EdgeCapacityMap& capacitymap, ResidualCapacityMap& rescapacitymap, Graph& G, Edge& e, int source, int finish) : \
	capacitymap(capacitymap), rescapacitymap(rescapacitymap), G(G), e(e), source(source), finish(finish) {}

	long get(int cap) {
		capacitymap[e] = cap;
		boost::successive_shortest_path_nonnegative_weights(G, source, finish);
		long cost = boost::find_flow_cost(G);
		return MAX * (capacitymap[e] - rescapacitymap[e]) - cost;		
	}
};

//TODO[gleb] Try some approach with dynamic programming.
void testcase() {
	int boats, sailors, p;
	cin >> boats >> sailors >> p;
	Graph G(boats + sailors);
    
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder adder(G, capacitymap, weightmap, revedgemap);
	
	int start = add_vertex(G);
	int finish = add_vertex(G);
	int source = add_vertex(G);

	for (int i = 0; i < p; i++) {
		int b, s, c;
		cin >> b >> s >> c;
		adder.addEdge(b, boats + s, 1, MAX - c);	
	}
	
	for (int b = 0; b < boats; b++) {
		adder.addEdge(start, b, 1, 0);
	}

	for (int s = 0; s < sailors; s++) {
		adder.addEdge(boats + s, finish, 1, 0);
	}

	Edge e = adder.addEdge(source, start, 0, 0);
	int lower = 1;
	int upper = min(boats, sailors);

	ResFinder finder(capacitymap, rescapacitymap, G, e, source, finish);
	while (lower + 1 < upper) {
		int mid = (lower + upper) / 2;
	
		long cur = finder.get(mid);
		long next = finder.get(mid + 1);

		if (next > cur) {
			lower = mid;
		} else {
			upper = mid;
		}
	}

	long forlower = finder.get(lower);
	long forupper = finder.get(upper);
	
	cout << max(forlower, forupper) << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
	return 0;
}
