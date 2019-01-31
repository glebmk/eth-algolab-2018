#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>
#include <set>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, long>
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

typedef vector<long> V;

void printv(V& v) {
	for (long elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

void testcase() {
	long n, m, k, T;
	cin >> n >> m >> k >> T;
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);

	vector<bool> network(n, false);
	for (long i = 0; i < T; i++) {
		long v;
		cin >> v;
		network[v] = true;
	}

	for (long i = 0; i < m; i++) {
		long u, v, c;
		cin >> u >> v >> c;
		Edge e; bool success;
		boost::tie(e, success) = add_edge(v, u, G);
		weightmap[e] = c;
	}

	V distmap(n);
	boost::dijkstra_shortest_paths(G, n - 1, 
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
	//cerr << "first distmap" << endl;
	//printv(distmap);

	V components(n, -1);
	long compnum = boost::strong_components(G, boost::make_iterator_property_map(components.begin(), get(boost::vertex_index, G)));

	vector<V> comps(compnum);
	for (long i = 0; i < n; i++) {
		if (!network[i]) {
			continue;
		}
		comps[components[i]].push_back(i);
	}

	for (V& comp : comps) {
		long closest = -1;
		long dist = LONG_MAX;
		for (long elem : comp) {
			if (distmap[elem] < dist) {
				dist = distmap[elem];
				closest = elem;
			}
		}

		if (closest == -1) {
			continue;
		}

		for (long elem : comp) {
			Edge e; bool success;
			boost::tie(e, success) = add_edge(closest, elem, G);
			assert(success == true);
			weightmap[e] = comp.size() - 1;
		}
	}

	boost::dijkstra_shortest_paths(G, n - 1, 
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
	//cerr << "second distmap" << endl;
	//prlongv(distmap);
	
	long time = LONG_MAX;
	for (long i = 0; i < k; i++) {
		time = min(distmap[i], time);
	}
	if (time <= 1000000) {
		cout << time << endl;
	} else {
		cout << "no" << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	long t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
	return 0;
}
