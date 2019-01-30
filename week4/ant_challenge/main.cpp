#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <utility>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;

typedef std::map<std::pair<int, int>, std::vector<int>> EdgeToWeights;

std::vector<Vertex> run_prim(int species, int start_vertex, EdgeToWeights edge_to_weights, int n) {
	Graph G(n);
	WeightMap weight_map = boost::get(boost::edge_weight, G);	
	std::vector<Vertex> parents(n);
	
	for (auto& key_value : edge_to_weights) {
		const std::pair<int, int>& edge = key_value.first;
		int w = key_value.second[species];
		Edge e; 
		bool added;
		boost::tie(e, added) = boost::add_edge(edge.first, edge.second, G);
		weight_map[e] = w;
	}

	boost::prim_minimum_spanning_tree(G, &parents[0], boost::root_vertex(start_vertex));  
	return parents;
}

void order(int& src, int& dst) {
	assert(src != dst);
	if (src > dst) {
		std::swap(src, dst);
	}
}

void print_vector(std::vector<Vertex> elems) {
	for (Vertex elem : elems) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}

void testcase()
{
	int n, e, s, a, b;
	std::cin >> n >> e >> s >> a >> b;
	
	EdgeToWeights edge2weights;
	for (int i = 0; i < e; i++) {
		int src, dst;
		std::cin >> src >> dst;
		order(src, dst);
		std::pair<int, int> edge = std::make_pair(src, dst);
		assert(!edge2weights.count(edge) && !edge2weights.count(std::make_pair(dst, src)));
		edge2weights[edge] = std::vector<int>(s);
		std::vector<int>& weights = edge2weights[edge]; 
		for (int j = 0; j < s; j++) {
			int w;
			std::cin >> w;
			assert(w >= 0);
			weights[j] = w;
		}
	}
	
	std::vector<int> start_vertexes(s);
	for (int i = 0; i < s; i++) {
		int start_vertex;
		std::cin >> start_vertex;
		start_vertexes[i] = start_vertex;
	}
	
	Graph G(n);
	WeightMap weight_map = boost::get(boost::edge_weight, G);	
	for (int i = 0; i < s; i++) {
		std::vector<Vertex> parents = run_prim(i, start_vertexes[i], edge2weights, n);
		assert(parents.size() == n);
		//print_vector(parents);
		//std::cout << " i = " << i << std::endl;
		for (int j = 0; j < parents.size(); j++) {
			int src = j;
			int dst = parents[j];
			if (src == dst) {
				continue;
			}
			order(src, dst);
			int w = edge2weights[std::make_pair(src, dst)][i];
			assert(w >= 0); 
			Edge e;
			bool added;
			boost::tie(e, added) = boost::add_edge(src, dst, G);
			weight_map[e] = w;
		}
	}

	std::vector<int> distmap(n);
	boost::dijkstra_shortest_paths(
		G,
		a,
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G)))
	);
	std::cout << distmap[b] << std::endl;
	
}


int main() {
	int t;
	std::cin >> t;

	for (int i = 0; i < t; i++) {
		testcase();
	}
	
	return 0;
}
