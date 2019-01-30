#include <iostream>
#include <vector>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef K::Point_2 Point;
typedef boost::disjoint_sets_with_storage<> DisjointSets;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Face_circulator Face_circulator;


void testcase(int n) {
	vector<Point> points;
	for (int i = 0; i < n; i++) {
		Point p; 
		cin >> p;
		points.push_back(p);
	}
	
	Triangulation t;
  	t.insert(points.begin(), points.end());

	int facecount = 0;
	for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
		f->info() = facecount;
		facecount++;
	}
	int infface = facecount;
	assert(infface >= 1);

	Face_circulator f = t.incident_faces(t.infinite_vertex());	
	do {
		f->info() = facecount;
		facecount++;
	} while (++f != t.incident_faces(t.infinite_vertex()));
	assert(facecount - infface >= 1);
		
	DisjointSets ds(facecount);
	for (int i = infface; i < facecount - 1; i++) {
		ds.union_set(i, i + 1);	
	}
	
	int infrep = ds.find_set(infface);
	
	int m;
	cin >> m;
	for (int i = 0; i < m; i++) {
		Point p;
		cin >> p;
		long d;
		cin >> d;
	}

}

int main() {
	while (true) {
		int n;
		cin >> n;
		if (n == 0) {
			return 0;
		}
		testcase(n);
	}
}

