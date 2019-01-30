#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

const int MAX = 200000;

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
        weightmap[e] = w; 
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; 
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

typedef vector<int> V;

enum T { Dep, Arr };

struct B {
	int time;
	T type;
	int target;
	int p;
};

bool comp(const B& left, const B& right) {
	if (left.time < right.time) {
		return true;
	}
	if (left.time > right.time) {
		return false;
	}
	return left.type == T::Arr;
}

void testcase() {
	int n, s;
	cin >> n >> s;
	
	Graph G(s);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder adder(G, capacitymap, weightmap, revedgemap);
   
	vector<vector<B>> bookings(s);
	for (int i = 0; i < n; i++) {
		int s, t, d, a, p;
		cin >> s >> t >> d >> a >> p;
		B source { d, T::Dep, t, p };
		bookings[s].push_back(source);
		B target { a, T::Arr, t, p };
		bookings[s].push_back(target);
	}

	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);
	
	V count(s);
	V next(s);
	V arrived(s);
	V vertex(s);
	for (int i = 0; i < s; i++) {
		int l;
		cin >> l;
		count[i] = l;
		vertex[i] = i;
		adder.addEdge(source, i, l, 0);
	}

	for (int i = 0; i < s; i++) {
		vector<B>& bs = bookings[i];
		std::sort(bs.begin(), bs.end(), comp);
		
		for (B& b : bs) {
			if (b.type == T::Arr) {
				next[i]++;
				continue;
			}
			if (count[i] == 0 && next[i] > 0) {
				Vertex newv = add_vertex(G);
				adder.addEdge(vertex[i], newv, MAX, 0);
				adder.addEdge(source, newv, next[i], 0);
				vertex[i] = newv;
				count[i] = next[i];
				next[i] = 0;
			}
			if (count[i] > 0) {
				count[i]--;
			}
			adder.addEdge(vertex[i], vertex[b.target], 1, b.p);		
		}
	}
	// Add the edges
	// adder.addEdge(v_source, v_farm1, 3, 0);	

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
