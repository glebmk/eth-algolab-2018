#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

using namespace std;

typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef K::Point_2 Point;
typedef K::Segment_2 Segm;

struct Stamp {
	Point p;
	long M;
};

long dist(const Point& p1, const Point& p2) {
	long dx = p1.x() - p2.x();
	long dy = p1.y() - p2.y();
	return dx * dx + dy * dy;
}

bool inter(const Segm& segm, const vector<Segm>& segms) {
	for (const auto& s : segms) {
		if (CGAL::intersection(segm, s)) {
			return true;
		}
	}
	return false;
}

void testcase() {
	int l, s, w;
	cin >> l >> s >> w;
	
	vector<Point> lamps;
	for (int i = 0; i < l; i++) {
		long x, y;
		cin >> x >> y;
		lamps.push_back({x, y});
	}

	vector<Stamp> stamps;
	for (int i = 0; i < s; i++) {
		long x, y, M;
		cin >> x >> y >> M;
		Point p {x, y};
		stamps.push_back({p, M});
	}

	vector<Segm> walls;
	for (int i = 0; i < w; i++) {
		long a, b, c, d;
		cin >> a >> b >> c >> d;
		Segm s { Point(a, b), Point(c, d) };
		walls.push_back(s);
	}
	
  	Program lp (CGAL::SMALLER, true, 1, true, 4096);
	int eq = 0;
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < l; j++) {
			Segm segm {stamps[i].p, lamps[j]};
			if (inter(segm, walls)) {
				continue;
			}
			long d = dist(stamps[i].p, lamps[j]);
			assert(d != 0);
			lp.set_a(eq, j, ET(1) / d); 
			lp.set_a(eq + 1, j, ET(1) / d);
		}
		lp.set_b(eq, stamps[i].M);
		lp.set_b(eq + 1, 1);
		lp.set_r(eq + 1, CGAL::LARGER);
		eq += 2;
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible()) {
		cout << "no" << endl;
	} else {
		cout << "yes" << endl;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
	return 0;
}

