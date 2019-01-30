#include <iostream>
#include <cassert>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef vector<long> V; 

double floor_to_double(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int tov(int i, int j) {
	return (i + 1) * i / 2 + j;
}

void add_exterior_condition(int v1, int v2, int eq, Program& lp) {
	lp.set_a(v1, eq, 1);
	lp.set_a(v2, eq, 1);
	lp.set_b(eq, 2);
	lp.set_r(eq, CGAL::EQUAL);	
}

void add_interior_condition(int v1, int v2, int eq, Program& lp) {
	lp.set_a(v1, eq, 1);
	lp.set_a(v2, eq, 1);
	lp.set_b(eq, 2);
}

void testcase() {
	int k;
	cin >> k;
	
	vector<V> balls(k, V(k));
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			long v;
			cin >> v;
			balls[i][j] = v;
		}
	}

	Program lp (CGAL::SMALLER, true, 0, true, 2);
	int eq = 0;	

	// Conditions on the exterior.
	for (int i = 0; i < k - 1; i++) {
		add_exterior_condition(tov(i, 0), tov(i + 1, 0), eq, lp);
		eq++;
	}

	for (int j = 0; j < k - 1; j++) {
		add_exterior_condition(tov(k - 1, j), tov(k - 1, j + 1), eq, lp);
		eq++;
	}

	for (int i = 0; i < k - 1; i++) {
		add_exterior_condition(tov(i, i), tov(i + 1, i + 1), eq, lp);
		eq++;
	}

	for (int i = 2; i < k; i++) {
		for (int j = 1; j < i; j++) {
			int v = tov(i, j);
			add_interior_condition(v, tov(i - 1, j - 1), eq, lp);
			add_interior_condition(v, tov(i - 1, j), eq + 1, lp);
			add_interior_condition(v, tov(i, j - 1), eq + 2, lp);
			eq += 3;
		}
	}

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			lp.set_c(tov(i, j), -balls[i][j]);
		}
	}
	
	Solution s = CGAL::solve_linear_program(lp, ET());
  	assert(!s.is_infeasible());
	cout << floor_to_double(-s.objective_value()) << endl;
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
