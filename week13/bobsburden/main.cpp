#include <iostream>
#include <vector>
#include <cassert>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef vector<int> Row;
typedef vector<Row> Table;

typedef adjacency_list<vecS, vecS, directedS,
		no_property,
		property<edge_weight_t, int>
		>					Graph;
typedef graph_traits<Graph>::vertex_descriptor		Vertex;
typedef graph_traits<Graph>::edge_descriptor		Edge;
typedef graph_traits<Graph>::edge_iterator		EdgeIt;	
typedef property_map<Graph, edge_weight_t>::type	WeightMap;

void print_table(Table& table, int k) {
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			cerr << table[i][j] << " ";
		}
		cerr << endl;
	}
}

int tov(int i, int j) {
	return (i + 1) * i / 2 + j; 
}

void testcase() {
	int k;
	cin >> k;

	Table weights(k, vector<int>(k));

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			int weight;
			cin >> weight;
			weights[i][j] = weight;
		}
	}

	int n = k * (k + 1) / 2;
	Graph G(n);
	WeightMap weightmap = get(edge_weight, G);
		
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			int v = tov(i, j);
			int w = weights[i][j];
			Edge e; bool success;
			if (i < k - 1) {
				int other = tov(i + 1, j);
				tie(e, success) = add_edge(v, other, G);
				weightmap[e] = weights[i + 1][j];
				tie(e, success) = add_edge(other, v, G);
				weightmap[e] = w;
			}
			if (i < k - 1 && j <= i) {
				int other = tov(i + 1, j + 1);
				tie(e, success) = add_edge(v, other, G);
				weightmap[e] = weights[i + 1][j + 1];
				tie(e, success) = add_edge(other, v, G);
				weightmap[e] = w;
			}
			if (j < i) {
				int other = tov(i, j + 1);
				tie(e, success) = add_edge(v, other, G);
				weightmap[e] = weights[i][j + 1];
				tie(e, success) = add_edge(other, v, G);
				weightmap[e] = w;
			}
		}
	}

	vector<int> predmap(n);

	vector<int> top(n);
	boost::dijkstra_shortest_paths(G, tov(0, 0),
		boost::distance_map(boost::make_iterator_property_map(top.begin(), boost::get(boost::vertex_index, G))));
	
	vector<int> left(n);
	boost::dijkstra_shortest_paths(G, tov(k - 1, 0), 
		boost::distance_map(boost::make_iterator_property_map(left.begin(), boost::get(boost::vertex_index, G))));

	vector<int> right(n);
	boost::dijkstra_shortest_paths(G, tov(k - 1, k - 1), 
		boost::distance_map(boost::make_iterator_property_map(right.begin(), boost::get(boost::vertex_index, G))));

	
	int mymin = -1;
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			int v = tov(i, j);
			int cursum = top[v] + left[v] + right[v] - 2 * weights[i][j];
			if (mymin == -1 || cursum < mymin) {
				mymin = cursum;
			}
		}
	}
	cout << mymin << endl; 
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
}
