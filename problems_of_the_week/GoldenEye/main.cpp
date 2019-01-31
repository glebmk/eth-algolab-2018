#include <iostream>
#include <set>
#include <boost/pending/disjoint_sets.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_data_structure_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Union_find.h>

typedef long long ll;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> vertex_t;
typedef CGAL::Triangulation_face_base_2<K> face_t;
typedef CGAL::Triangulation_data_structure_2<vertex_t,face_t> triangulation_t;
typedef CGAL::Delaunay_triangulation_2<K, triangulation_t>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;
typedef std::vector<Point> Points;
struct Mission {
	Point s, t;
};
typedef std::vector<Mission> Missions;
typedef boost::disjoint_sets_with_storage<> DS;

ll dist(Point& p1, Point& p2) {
	return llround(CGAL::squared_distance(p1, p2));	
}

bool mission_passes(Mission& m, Triangulation& t, DS& ds, ll p) {
	Vertex ns = t.nearest_vertex(m.s);
	if (dist(m.s, ns->point()) > p) {
		return false;
	}
	Vertex nt = t.nearest_vertex(m.t);
	if (dist(m.t, nt->point()) > p) {
		return false;
	}
	return ds.find_set(ns->info()) == ds.find_set(nt->info());
}

void testcase()
{
  	int n, m;
	ll p;
 	std::cin >> n >> m >> p;
  	
	Points jammers;
	for (int i = 0; i < n; i++) {
		Point jammer;
		std::cin >> jammer;
		jammers.push_back(jammer);
	}

	Missions missions;
	for (int i = 0; i < m; i++) {
		Point s, t;
		std::cin >> s >> t;
		missions.push_back({s, t});
	}

  	Triangulation t;
  	t.insert(jammers.begin(), jammers.end());
 
	DS ds(n);
  	
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); e++) {
		Vertex v1 = e->first->vertex((e->second + 1) % 3);
		Vertex v2 = e->first->vertex((e->second + 2) % 3);
		
		if (dist(v1->point(), v2->point()) < p) {
			std::cerr << v1->info() << " " << v2->info() << std::endl;
			//ds.union_set(v1->info(), v2->info());
		}
	}

/*	
	for (int i = 0; i < m; i++) {
		Mission m = missions[i];
		bool passes = mission_passes(m, t, ds, p);
		if (passes) {
			std::cout << "y";
		} else {
			std::cout << "n";
		}
	}
	std::cout << std::endl;
*/	
	/*
	for (int i = 0; i < n; i++) {
		uf.make_set(jammers	
	}
  	
	// output all edges
  	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    	std::cout << t.segment(e) << "\n";
	}
	*/
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}
