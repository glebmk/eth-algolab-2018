#include <set>
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, long> 
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

struct E {
	int a, b;
	long w;
};

void printv(vector<long>& v) {
	for (long elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

void dfs(int v, long max_weight, vector<long>& mweight, vector<vector<int>>& adj, vector<vector<long>>& adjw);

void testcase() {
	int n, tat;
	cin >> n >> tat;
	
	Graph G(n);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	vector<vector<long>> edges(n, vector<long>(n, -1));
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			long w;
			cin >> w;
			Edge e; bool success;
			boost::tie(e, success) = add_edge(i, j, G);
			weightmap[e] = w;
			edges[i][j] = w;
			edges[j][i] = w;
		}
	}
	
	vector<Edge> mst;
 	kruskal_minimum_spanning_tree(G, back_inserter(mst));
	assert(mst.size() == n - 1);

	vector<vector<int>> adj(n);
	vector<vector<long>> adjw(n);
	vector<vector<bool>> inmst(n, vector<bool>(n, false));
	long total = 0;	
	for (Edge& e : mst) {
		int a = boost::source(e, G);
		int b = boost::target(e, G);
		long w = weightmap[e];
		total += w;
		adj[a].push_back(b);
		adj[b].push_back(a);
		adjw[a].push_back(w);
		adjw[b].push_back(w);
		inmst[a][b] = true;
		inmst[b][a] = true;
	}
	//cerr << "mst total weight " << total << endl;

	vector<vector<long>> mweights;
	for (int v = 0; v < n; v++) {
		vector<long> mweight(n, -1);
		dfs(v, 0, mweight, adj, adjw);
		mweights.push_back(mweight);
		//cerr << "mweigth from vertex " << v << endl;
		//printv(mweight);
	}

	long mindiff = 1000000000;
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (inmst[i][j]) {
				continue;
			}
			mindiff = min(mindiff, edges[i][j] - mweights[i][j]);
		}
	}
	cout << total + mindiff << endl;
}

void dfs(int v, long max_weight, vector<long>& mweight, vector<vector<int>>& adj, vector<vector<long>>& adjw) {
	if (mweight[v] != -1) {
		return;
	}

	mweight[v] = max_weight;
	for (int i = 0; i < adj[v].size(); i++) {
		dfs(adj[v][i], max(max_weight, adjw[v][i]), mweight, adj, adjw);
	}
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
