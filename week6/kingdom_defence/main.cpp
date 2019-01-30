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

void testcase() {
  int L, P;
  int expected_flow = 0;
  std::cin >> L >> P;

  Graph G(L);
  EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
  ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
  ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
  EdgeAdder adder(G, capacitymap, revedgemap);

  std::vector<int> balance(L);

  int g, d;
  for(int i = 0; i < L; i++){
    std::cin >> g >> d;
    balance[i] = g - d;
  }

  int src, dest, lower, upper;
  for(int i = 0; i < P; i++){
    std::cin >> src >> dest >> lower >> upper;
    balance[src] -= lower;
    balance[dest] += lower;
    adder.addEdge(src, dest, upper - lower);
  }

  Vertex source = boost::add_vertex(G);
  Vertex target = boost::add_vertex(G);

  for(int i = 0; i < L; i++){
    if(balance[i] > 0){
      adder.addEdge(source, i, balance[i]);
    } else {
      expected_flow -= balance[i];
      adder.addEdge(i, target, -1*balance[i]);
    }
  }

  long flow = boost::push_relabel_max_flow(G, source, target);
  if(flow >= expected_flow)
    std::cout << "yes" << std::endl;
  else
    std::cout << "no" << std::endl;
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
