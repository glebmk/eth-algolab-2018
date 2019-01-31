#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

using namespace std;

typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct P {
	long x, y;
};

long ceil_to_long(SolT x) {
  double a = std::floor(CGAL::to_double(x));
  while (a - 1 >= x) a -= 1;
  while (a < x) a += 1;
  return lround(a);
}

vector<P> read_points(int size) {
	vector<P> v;
	for (int i = 0; i < size; i++) {
		long x, y;
		cin >> x >> y;
		v.push_back({x, y});
	}
	return v;
}

ET get_max(P& p1, P& p2, long h, long w) {
	ET wcoef = ET(abs(p1.x - p2.x) * 2) / w;
	ET hcoef = ET(abs(p1.y - p2.y) * 2) / h;
	ET m = max(wcoef, hcoef);
	assert(m >= 2);
	return m;
}

void printv(vector<ET> v) {
	for (ET& elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

void testcase() {
	int n, m;
	long h, w;
	cin >> n >> m >> h >> w;

	vector<P> free = read_points(n);
	vector<P> occupied = read_points(m);

	vector<ET> constraints;
	for (int i = 0; i < n; i++) {
		ET c = -1;
		for (P& occ : occupied) {
			ET n = get_max(free[i], occ, h, w) - 1;
			if (c == -1 || n < c) {
				c = n;
			}
		}
		constraints.push_back(c);
	}

	Program lp (CGAL::SMALLER, true, 1, false, 0);

	int eq = 0;
	if (occupied.size() > 0) {
		for (int i = 0; i < n; i++) {
			lp.set_a(i, eq, 1);
			lp.set_b(eq, constraints[i]);
			eq++;
		}
	}
	//cerr << "contraints" << endl;
	//printv(constraints);
	
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			lp.set_a(i, eq, 1);
			lp.set_a(j, eq, 1);
			ET c = get_max(free[i], free[j], h, w);
			//cerr << "i " << i << " j " << j << " c " << c << endl;
			lp.set_b(eq, c);
			eq++;
		}
	}

	for (int i = 0; i < n; i++) {
		lp.set_c(i, - 2 * (h + w));
	}

  	Solution s = CGAL::solve_linear_program(lp, ET());
	//cerr << "solution" << endl;
	//cerr << s << endl;
	cout << ceil_to_long(- s.objective_value()) << endl;
}

int main() {
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
}
