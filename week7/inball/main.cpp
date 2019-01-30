#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef long ll;

typedef CGAL::Gmpz ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

ll myfloor(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return llround(a);
}

ll norm(vector<ll> v) {
	ll sum = 0;
	for (ll elem : v) {
		sum += elem * elem;
	}
	return llround(sqrt(sum));
}

void testcase(int n) {
	int d;
	cin >> d;

	Program lp (CGAL::SMALLER, false, 0, false, 0);
	int eq = 0;
	for (int i = 0; i < n; i++) {
		vector<ll> v; 
		for (int j = 0; j < d; j++) {
			ll a;
			cin >> a;
			v.push_back(a);
			lp.set_a(j, eq, a);
			lp.set_a(j, eq + 1, a);
		}
		ll b;
		cin >> b;
		ll n = norm(v);
		lp.set_a(d, eq, n);
		lp.set_b(eq, b); 
		lp.set_a(d, eq + 1, -n);
		lp.set_b(eq + 1, b);
		eq += 2;
	}
	lp.set_c(d, -1);

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible()) {
		cout << "none" << endl;
	} else if (s.is_unbounded()) {
		cout << "inf" << endl;
	} else {
		ll ans = myfloor(-CGAL::to_double(s.objective_value()));
		cout << ans << endl;
	}
}

int main() {
  	ios_base::sync_with_stdio(false);
	while (true) {
		int n;
		cin >> n;
		if (n == 0) {
			return 0;
		}
		testcase(n);
	}
}
