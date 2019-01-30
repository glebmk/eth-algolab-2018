#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <utility>
#include <map>
#include <set>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::no_property> Graph;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Vertex;
typedef std::set<int> Student;

int count_matches(Student& first, Student& second) {
	int matches = 0;
	for (const auto& interest : first) {
		if (second.count(interest) > 0) {
			matches++;
		}
	}
	return matches;
}

void testcase() {
	int n, c, f;
	std::cin >> n >> c >> f;
	int next_encoding = 0;
	std::unordered_map<std::string, int> mapping; 
	std::vector<Student> students;
	for (int i = 0; i < n; i++) {
		Student student;
		for (int j = 0; j < c; j++) {
			std::string interest;
			std::cin >> interest;
			int encoding = -1;
			auto search = mapping.find(interest);
			if (search != mapping.end()) {
				encoding = search -> second;
			} else {
				encoding = next_encoding;
				mapping.insert({interest, encoding});
				next_encoding += 1;
			}
			student.insert(encoding);
		}
		students.push_back(student);
	}

	Graph G(n);
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			int matches = count_matches(students[i], students[j]);
			if (matches > f) {
				Edge e;
				bool added;
				boost::tie(e, added) = boost::add_edge(i, j, G);
			}
		}
	}
	std::vector<Vertex> matemap(n);
	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, G)));
    int matchingsize = boost::matching_size(G, boost::make_iterator_property_map(matemap.begin(), boost::get(boost::vertex_index, G)));
	if (matchingsize * 2 == n) {
		std::cout << "not optimal" << std::endl;	
	} else {
		std::cout << "optimal" << std::endl;
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
