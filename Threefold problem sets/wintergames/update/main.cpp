#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

using namespace std;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::FT FT;
typedef K::Point_2 Point;

typedef CGAL::Quotient<FT> SolT;
typedef CGAL::Quadratic_program<FT> Program;
typedef CGAL::Quadratic_program_solution<FT> Solution;

long floor_to_long(const FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return lround(a);
}

long ceil_to_long(const SolT& s) {
	double a = std::ceil(CGAL::to_double(s));
	while (a - 1 >= s) a -= 1;
	while (a < s) a += 1;
	return lround(a);
}

FT dist(const Point& p1, const Point& p2) {
	return CGAL::squared_distance(p1, p2);
}

typedef pair<Point, int> Pair;

void testcase() {
	int n;
	cin >> n;

	vector<Pair> pts;
	for (int i = 0; i < n; i++) {
		long x, y;
		cin >> x >> y;
		pts.push_back(pair<Point, int>({x, y}, i));
	}

	Triangulation t;
	t.insert(pts.begin(), pts.end());



	FT minsq = -1;
  	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Vertex v1 = e->first->vertex((e->second + 1) % 3);
		Vertex v2 = e->first->vertex((e->second + 2) % 3);
		FT d = dist(v1->point(), v2->point());
		if (minsq == -1 || d < minsq) {
			minsq = d;
		}
	}

	Program lp (CGAL::SMALLER, true, 0, false, 0);
	FT mind = -1;
	int eq = 0;
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Vertex v1 = e->first->vertex((e->second + 1) % 3);
		Vertex v2 = e->first->vertex((e->second + 2) % 3);
		int v1i = v1->info();
		int v2i = v2->info();
		lp.set_a(v1i, eq, 1);
		lp.set_a(v2i, eq, 1);
		
		FT d = CGAL::sqrt(dist(v1->point(), v2->point()));
		lp.set_b(eq, d);
		if (d < mind || mind == -1) {
			mind = d;
		}
		eq++;
	}

	long lower_constraint = floor_to_long(mind / 2);
	assert(lower_constraint >= 0);
	for (int i = 0; i < n; i++) {
		lp.set_a(i, eq, 1);
		lp.set_b(eq, lower_constraint);
		lp.set_r(eq, CGAL::LARGER);
		eq++;
		lp.set_c(i, -1);
	}
	lp.set_c0(n * lower_constraint);

  	Solution s = CGAL::solve_linear_program(lp, FT());
  	assert (s.solves_linear_program(lp));
	assert(!s.is_infeasible());

	long ans = ceil_to_long(-s.objective_value());
	cout << ans << endl;
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
