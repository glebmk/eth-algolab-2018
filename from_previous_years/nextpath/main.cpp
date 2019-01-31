#include <iostream>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, int>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::edge_iterator		EdgeIt;
typedef property_map<Graph, edge_weight_t>::type	WeightMap;
typedef vector<int> V;

const int NOTFOUND = -1; 

void check(int v, int n) {
	assert(0 <= v && v < n);
}

int rec(int v, const int s, const V& distmap, const V& predmap, const vector<V>& inc) {
	int n = inc.size();
	check(v, n);
	if (v == s) {
		return NOTFOUND;
	}

	int secondbest = NOTFOUND;
	for (int e : inc[v]) {
		check(e, n);
		if (distmap[e] >= INT_MAX || e == predmap[v]) {
			continue;
		}
		if (secondbest == NOTFOUND || 1 + distmap[e] < secondbest) {
			secondbest = 1 + distmap[e];
		}
	}

	int next = rec(predmap[v], s, distmap, predmap, inc);
	if (secondbest == NOTFOUND) {
		return next;
	} else {
		assert(distmap[v] - secondbest >= 0);
		return min(next, distmap[v] - secondbest);
	}
}

void testcase() {
	int n, m, s, t;
	cin >> n >> m >> s >> t;
	s--;
	t--;
	check(s, n);
	check(t, n);
	
	Graph G(n);
	WeightMap weightmap = get(edge_weight, G);
	
	vector<V> inc(n);

	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		u--;
		v--;
		check(u, n);
		check(v, n);
		Edge e;	bool success;
		tie(e, success) = add_edge(u, v, G);
		weightmap[e] = 1;
		inc[v].push_back(u);
	}

	V distmap(n);
	V predmap(n);
	dijkstra_shortest_paths(G, t,
			predecessor_map(make_iterator_property_map(
					predmap.begin(), get(vertex_index, G))).
			distance_map(make_iterator_property_map(
					distmap.begin(), get(vertex_index, G))));
	
	int ans = rec(t, s, distmap, predmap, inc);
	if (ans == NOTFOUND) {
		cout << "no" << endl;
	} else {
		cout << ans << endl;		
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

