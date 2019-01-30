#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int> > Graph;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
typedef Graph::edge_descriptor Edge;

void testcase()
{
	int n, m;
	std::cin >> n >> m;
	Graph G(n);
	
	WeightMap weights = boost::get(boost::edge_weight, G);

	for (int i = 0; i < m; i++) {
		int src, dst, w;
		Edge e;
		bool added;
		std::cin >> src >> dst >> w;
		boost::tie(e, added) = boost::add_edge(src, dst, G);
		weights[e] = w;
	}

	std::vector<Edge> mst;
	boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
	
	int mst_weight = 0;
	for (Edge edge : mst) {
		mst_weight += weights[edge];	
	}
	
	std::vector<int> distmap(n);
	boost::dijkstra_shortest_paths(
		G, 
		0,
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G)))
	);

	int max_dist = *std::max_element(distmap.begin(), distmap.end());

	std::cout << mst_weight << " " << max_dist << std::endl;
}

int main() {
	int t;
	std::cin >> t;

	for (int i = 0; i < t; i++) {
		testcase();
	}
	
	return 0;
}
