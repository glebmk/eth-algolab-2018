#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

typedef std::vector<std::string> Piece;
typedef std::pair<char, char> Pair;
typedef std::map<char, int> CharMap;
typedef std::map<Pair, int> PairMap;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
	boost::property<boost::edge_residual_capacity_t, long,
	boost::property<boost::edge_reverse_t, Traits::edge_descriptor>
	>>> Graph;

typedef boost::property_map<Graph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

class EdgeAdder {
	Graph& G;
	EdgeCapacityMap& capacitymap;
	ReverseEdgeMap& revedgemap;

  public:
    EdgeAdder(Graph& G,
			  EdgeCapacityMap& capacitymap,
			  ReverseEdgeMap& revedgemap):
		G(G),
		capacitymap(capacitymap),
		revedgemap(revedgemap) {}

	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

Piece read_piece(int h, bool reverse) {
	Piece piece;

	for (int i = 0; i < h; i++) {
		std::string line;
		std::cin >> line;
		if (reverse) {
			std::reverse(line.begin(), line.end());
		}
		piece.push_back(line);
	}

	return piece;
}

CharMap read_target() {
	std::string message;
	std::cin >> message;
	CharMap map;

	for (char c : message) {
		auto search = map.find(c);

		if (search != map.end()) {
			search->second += 1;
		} else {
			map.insert(std::pair<char, int>(c, 1));
		}
	}

	return map;
}

PairMap create_pair_map(Piece& first, Piece& second, int h, int w) {
	PairMap map;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			char c1 = first[i][j];
			char c2 = second[i][j];
			if (c1 > c2) {
				std::swap(c1, c2);
			}
			Pair pair {c1, c2};
			auto search = map.find(pair);
			
			if (search != map.end()) {
				search->second += 1;
			} else {
				map.insert(std::pair<Pair, int>(pair, 1));	
			}
		}
	}
	return map;
}

const int NUM = 'Z' - 'A' + 1;

int pair2v(Pair pair) {
	return (pair.first - 'A') * NUM + (pair.second - 'A');
}

int char2v(char c) {
	return NUM * NUM + c - 'A';
}

long get_sum(CharMap& map) {
	long sum = 0;
	for (auto& elem : map) {
		sum += elem.second;
	}
	return sum;
}

void solve(CharMap& char_map, PairMap& pair_map) {
	Graph G(NUM * NUM + NUM);	
	Vertex source = add_vertex(G);
	Vertex target = add_vertex(G);
	
	EdgeCapacityMap capacitymap = get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = get(boost::edge_reverse, G);
	EdgeAdder adder(G, capacitymap, revedgemap);
	
	for (auto& elem : pair_map) {
		Pair pair = elem.first;
		int count = elem.second;
		int v = pair2v(pair);
		adder.addEdge(source, v, count);
		adder.addEdge(v, char2v(pair.first), count);
		adder.addEdge(v, char2v(pair.second), count);
	}
	
	for (auto& elem : char_map) {
		adder.addEdge(char2v(elem.first), target, elem.second);
	}

	long target_flow = get_sum(char_map);
	long flow = boost::push_relabel_max_flow(G, source, target);
	
	if (flow == target_flow) {
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;	
	}	
}

void testcase() {
	int h, w;
	std::cin >> h >> w;
	CharMap target = read_target();
	Piece front = read_piece(h, false);
	Piece back = read_piece(h, true);
	PairMap pair_map = create_pair_map(front, back, h, w);
	solve(target, pair_map);
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
