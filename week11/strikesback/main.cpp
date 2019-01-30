#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <iostream>
#include <CGAL/Gmpq.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

using namespace std;

typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;
typedef K::FT FT;

struct Part {
	Point p;
	int d;
};

void printv(vector<FT> v) {
	for (int elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

void testcase() {
	int a, s, b, e;
	std::cin >> a >> s >> b >> e;

	std::vector<Part> parts;
	for (int i = 0; i < a; i++) {
		Point p;
		std::cin >> p;
		int d;
		std::cin >> d;
		Part part {p, d};
		parts.push_back(part);
	}

	std::vector<Point> shoots;
	for (int i = 0; i < s; i++) {
		Point p;
		std::cin >> p;
		shoots.push_back(p);
	}

	std::vector<Point> hunters;
	for (int i = 0; i < b; i++) {
		Point p;
		std::cin >> p;
		hunters.push_back(p);
	}

	Triangulation t;
	t.insert(hunters.begin(), hunters.end());

	std::vector<FT> closest;
	if (b > 0) {
		//cerr << "case b > 0" << endl;
		for (int i = 0; i < s; i++) {
			Vertex v = t.nearest_vertex(shoots[i]);
			closest.push_back(CGAL::squared_distance(v->point(), shoots[i])); 
		}
	}
	
	std::vector<std::vector<FT>> dists;
	for (int i = 0; i < a; i++) {
		std::vector<FT> dist;
		for (int j = 0; j < s; j++) {
			FT d = CGAL::squared_distance(parts[i].p, shoots[j]);
			if (b == 0 || d < closest[j]) {
	//			cerr << "d < closest[j]" << endl;
				dist.push_back(d);
			} else {
				dist.push_back(-1);
			}
		}
		dists.push_back(dist);
	}

	Program lp (CGAL::LARGER, true, 0, false, 0);

	int eq = 0;
	for (int i = 0; i < a; i++) {
		//cerr << "particle " << i << endl;
		//printv(dists[i]);
		for (int j = 0; j < s; j++) {
			FT d = dists[i][j];
			if (d == -1) {
				continue;
			} else if (d == 0) {
				lp.set_a(j, eq, 1);
			} else {
				lp.set_a(j, eq, ET(1) / d);
			}
		}
		lp.set_b(eq, parts[i].d);
		eq++;
	}
	
	for (int i = 0; i < s; i++) {
		lp.set_a(i, eq, 1);
	}
	lp.set_b(eq, e);
	lp.set_r(eq, CGAL::SMALLER);

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible()) {
		cout << "n" << endl;
	} else {
		cout << "y" << endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	std::cin >> t;
	while (t) {
		//cerr << "testcase" << endl;
		t--;
		testcase();
	}
}

