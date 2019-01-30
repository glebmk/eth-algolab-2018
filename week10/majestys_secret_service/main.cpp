#include <iostream>
#include <cstdlib>
#include <climits>
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

void printv(V& v) {
	for (int elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

V readv(int size) {
	V v;
	for (int i = 0; i < size; i++) {
		int elem;
		cin >> elem;
		v.push_back(elem);
	}
	return v;
}

long find_flow(vector<V>& dists, int agents, int shelters, int upper, int d, int c) {
	assert(dists.size() == agents);
	Graph G(agents + 2 * shelters);

	int source = boost::add_vertex(G);
	int target = boost::add_vertex(G);
	
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder adder(G, capacitymap, weightmap, revedgemap);

	for (int a = 0; a < agents; a++) {
		V& dist = dists[a];
		assert(dist.size() == shelters);
		for (int s = 0; s < shelters; s++) {
			if (dist[s] != -1 && dist[s] <= upper) {
				adder.addEdge(a, agents + s, 1, 0);
			}
			if (dist[s] != -1 && dist[s] + d <= upper && c == 2) {
				adder.addEdge(a, agents + shelters + s, 1, 0);	
			}
		}
		adder.addEdge(source, a, 1, 0);
	}
	
	for (int s = 0; s < shelters; s++) {
		adder.addEdge(agents + s, target, 1, 0);
		adder.addEdge(agents + shelters + s, target, 1, 0);
	}
	long flow = boost::push_relabel_max_flow(G, source, target);
	return flow;
}

void testcase() {
	int n, m, a, s, c, d;
	cin >> n >> m >> a >> s >> c >> d;

	Graph G(n);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
	for (int i = 0; i < m; i++) {
		char w;
		int x, y, z;
		cin >> w >> x >> y >> z;
		Edge e; bool success;
		boost::tie(e, success) = boost::add_edge(x, y, G);
		weightmap[e] = z;
		if (w == 'L') {
			boost::tie(e, success) = boost::add_edge(y, x, G);
			weightmap[e] = z;
		}
	}
	
	V agents = readv(a);
	V shelters = readv(s);

	int upper = 0;
	vector<V> dists;
	for (int agent : agents) {
		V distmap(n);
		boost::dijkstra_shortest_paths(G, agent, \
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
		V dist;
		for (int shelter : shelters) {
			int d = distmap[shelter];
			if (distmap[shelter] < INT_MAX) {
				dist.push_back(d);
				upper = max(d, upper);
			} else {
				dist.push_back(-1);
			}
		}
		dists.push_back(dist);
	}
	upper += d;

	int lower = 0;
	while (lower + 1 < upper) {
		int mid = (lower + upper) / 2;
		if (find_flow(dists, a, s, mid, d, c) == a) {
			upper = mid;
		} else {
			lower = mid;
		}
	}
	if (find_flow(dists, a, s, lower, d, c) == a) {
		cout << lower + d << endl;
	} else {
		cout << upper + d << endl;
	}
}

int main() {
	int t;
	cin >> t;
	while (t) {
		//cerr << "testcase" << endl;
		t--;
		testcase();
	}
	return 0;
}
