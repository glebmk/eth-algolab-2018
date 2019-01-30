#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,	
		boost::no_property,	
		boost::property<boost::edge_weight_t, int>
		> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;		
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;
//typedef boost::property_map<Graph, boost::edge_component_t>::type ComponentMap;

typedef pair<int, int> E;
void testcase() {
	int n, m;
	cin >> n >> m;
	Graph G(n);
	WeightMap componentmap = get(boost::edge_weight, G);

	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		Edge e; bool success;
		boost::tie(e, success) = add_edge(a, b, G);
	}
  	int numcomp = boost::biconnected_components(G, componentmap);	
	
	vector<vector<E>> comp2edges(numcomp);
	EdgeIt ebeg, eend;
	for (boost::tie(ebeg, eend) = edges(G); ebeg != eend; ++ebeg) {
		int a = boost::source(*ebeg, G);
		int b = boost::target(*ebeg, G);
		E e {min(a, b), max(a, b)};
		comp2edges[componentmap[*ebeg]].push_back(e);	
        // this edge (u,v) in GT implies that component sccmap[u] is not minimal in G
	}

	vector<E> out;
	for (vector<E>& edges : comp2edges) {
		if (edges.size() == 1) {
			out.push_back(edges[0]);
		}
	}
	sort(out.begin(), out.end());
	cout << out.size() << endl;
	for (E e : out) {
		cout << e.first << " " << e.second << endl;
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

