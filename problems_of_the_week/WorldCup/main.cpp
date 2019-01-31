#include <set>
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

// choose exact integral type
typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Warehouse {
	int x, y, s, a;
};

struct Stadium {
	int x, y, d, u;
};

struct Cont {
	int x, y, r;

	bool in_cont(int x2, int y2) {
		long x_diff = x2 - x;
		long y_diff = y2 - y;
		return x_diff * x_diff + y_diff * y_diff <= r*r;
	}
};

int tov(int warehouse, int stadium, int total_number_of_warehouses) {
	return total_number_of_warehouses * warehouse + stadium;
}

int floor_to_double(const SolT& x)
{
  	double a = std::ceil(CGAL::to_double(x));
  	while (a + 1 <= x) a += 1;
  	while (a > x) a -= 1;
  	return a;
}

int diff(set<int> first, set<int> second) {
	int mutual = 0;
	for (int elem : first) {
		if (second.count(elem)) {
			mutual++;
		}
	}
	return first.size() + second.size() - 2 * mutual;
}

void testcase() {
  	int n, m, c;
	cin >> n >> m >> c;

	vector<Warehouse> warehouses;
	for (int i = 0; i < n; i++) {
		int x, y, s, a;
		cin >> x >> y >> s >> a;
		Warehouse house {x, y, s, a};
		warehouses.push_back(house);
	}

	vector<Stadium> stadiums;
	for (int i = 0; i < m; i++) {
		int x, y, d, u;
		cin >> x >> y >> d >> u;
		Stadium stadium {x, y, d, u};
		stadiums.push_back(stadium);
	}
	
	vector<vector<int>> revenues;
	for (int i = 0; i < n; i++) {
		vector<int> row;
		for (int j = 0; j < m; j++) {
			int r;
			cin >> r;
			row.push_back(r);
		}
		revenues.push_back(row);
	}

	vector<Cont> conts;
	for (int i = 0; i < c; i++) {
		int x, y, r;
		cin >> x >> y >> r;
		Cont cont {x, y, r};
		conts.push_back(cont);
	}

	vector<set<int>> w2c;
	for (int i = 0; i < n; i++) {
		set<int> for_w;
		Warehouse w = warehouses[i];
		for (int j = 0; j < c; j++) {
			if (conts[j].in_cont(w.x, w.y)) {
				for_w.insert(j);	
			}
		}
		w2c.push_back(for_w);
	}

	vector<set<int>> s2c;
	for (int i = 0; i < m; i++) {
		set<int> for_s;
		Stadium s = stadiums[i];
		for (int j = 0; j < c; j++) {
			if (conts[j].in_cont(s.x, s.y)) {
				for_s.insert(j);
			}
		}
		s2c.push_back(for_s);
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			revenues[i][j] = 100 * revenues[i][j] - diff(w2c[i], s2c[j]);
		}
	}

	// by default, we have a nonnegative LP with Ax <= b
  	Program lp (CGAL::SMALLER, true, 0, false, 0);

	int eq = 0;

	// Constraint on not exceeding the supply of the warehouse.
	for (int w = 0; w < n; w++) {
		for (int s = 0; s < m; s++) {
			lp.set_a(tov(w, s, n), eq, 1); 
		}
		lp.set_b(eq, warehouses[w].s);
		eq++;
	}

	// Constraint on meeting the demand and not exceeding upper limit on alcohol
	for (int s = 0; s < m; s++) {
		for (int w = 0; w < n; w++) {
			int v = tov(w, s, n);
			lp.set_a(v, eq, 1);
			lp.set_a(v, eq + 1, warehouses[w].a);  
		}
		Stadium& st = stadiums[s];
		lp.set_b(eq, st.d);
		lp.set_r(eq, CGAL::EQUAL);
		lp.set_b(eq + 1, st.u * 100);
		eq += 2;
	}

	//cerr << "n " << n << " m " << m << " eq " << eq << endl;

	// Function that we want to maximize.	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			lp.set_c(tov(i, j, n), -revenues[i][j]);
		}
	}
	
 	// solve the program, using ET as the exact type
 	Solution s = CGAL::solve_linear_program(lp, ET());
  	assert (s.solves_linear_program(lp));

	if (s.status() == CGAL::QP_INFEASIBLE) {
		cout << "RIOT!" << endl;
	} else {
		cout << floor_to_double(-s.objective_value() / 100) << endl; 
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		testcase();
		t--;
	}
	return 0;
}
