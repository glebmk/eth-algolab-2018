#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>
#include <boost/graph/push_relabel_max_flow.hpp>

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
	EdgeAdder(
		Graph& G,
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

class Game {
public:
	int f;
	int s;	
	Game(int f, int s): f(f), s(s) {}
};

void print_vector(std::vector<int>& v) {
	for (int elem : v) {
		std::cerr << elem << " ";
	}
	std::cerr << std::endl;
}

void print_games(std::vector<Game>& games) {
	for (Game& game : games) {
		std::cerr << game.f << " " << game.s << std::endl;
	}
}

long compute_flow(std::vector<Game>& games, std::vector<int>& res_scores) {
	int games_num = games.size();
	int players_num = res_scores.size();

	Graph G(games_num + players_num + 2);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	EdgeAdder edge_adder(G, capacitymap, revedgemap);

	Vertex source = 0;
	Vertex target = 1;

	for (int i = 0; i < games_num; i++) {
		edge_adder.addEdge(source, 2 + i, 1);
		Game& game = games[i];
		edge_adder.addEdge(2 + i, 2 + games_num + game.f, 1);
		edge_adder.addEdge(2 + i, 2 + games_num + game.s, 1);
	}

	for (int i = 0; i < players_num; i++) {
		edge_adder.addEdge(2 + games_num + i, target, res_scores[i]);
	}

	long flow = boost::push_relabel_max_flow(G, source, target);
	return flow;
}

void testcase() {
	int n, m;
	std::cin >> n >> m;
	
	std::vector<Game> games;
	std::vector<int> cur_scores(n);
	std::vector<int> target_scores(n);

	for (int i = 0; i < m; i++) {
		int f, s, c;
		std::cin >> f >> s >> c;
		
		if (c == 1) {
			cur_scores[f] += 1;
		} else if (c == 2) {
			cur_scores[s] += 1;
		} else {
			games.push_back(Game(f, s));
		}
	}

	for (int i = 0; i < n; i++) {
		int target;
		std::cin >> target;
		target_scores[i] = target; 
	}

	std::cerr << "cur scores" << std::endl;
	print_vector(cur_scores);

	std::cerr << "games" << std::endl;
	print_games(games);
	
	std::cerr << "target" << std::endl;
	print_vector(target_scores);

	std::vector<int> residual_scores(n);
	int total_residual = 0;
	for (int i = 0; i < n; i++) {
		int residual = target_scores[i] - cur_scores[i];
		residual_scores[i] = residual;
		total_residual += residual;
		if (residual < 0) {
			std::cout << "no" << std::endl;
			return;
		}
	}
	
	std::cerr << "residual scores" << std::endl;
	print_vector(residual_scores);

	if (total_residual != games.size()) {
		std::cout << "no" << std::endl;
		return;
	}
	
	long flow = compute_flow(games, residual_scores);
	std::cerr << "flow is " << flow << std::endl;
	if (flow != total_residual) {
		std::cout << "no" << std::endl;
	} else {
		std::cout << "yes" << std::endl;
	}
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
