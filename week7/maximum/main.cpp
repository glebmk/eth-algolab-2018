#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>
// choose exact Rational type
typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolutionValue;
typedef long long ll;

ll ceil(const SolutionValue& x)
{
	double a = round(CGAL::to_double(x));
	while (a - 1 >= x) {
		a -= 1;
	}
	while (a < x) {
		a += 1;
	}
	return llround(a);
}

void print_solution(Solution& s, bool inverse) {
	if (s.is_infeasible()) {
		std::cout << "no";
	} else if (s.is_unbounded()) {
		std::cout << "unbounded";
	} else {
		ll sol = ceil(s.objective_value());
		if (inverse) {
			sol = -sol;
		}
		std::cout << sol;
	}
	std::cout << std::endl;
}

void maximize(int a, int b) {
	Program qp(CGAL::SMALLER, true, 0, false, 0);
	const int X = 0;
	const int Y = 1;

	// x + y <= 4
	qp.set_a(X, 0, 1);
	qp.set_a(Y, 0, 1);
	qp.set_b(0, 4);

	// 4x + 2y <= ab
	qp.set_a(X, 1, 4);
	qp.set_a(Y, 1, 2);
	qp.set_b(1, a * b);

	// -x + y <= 1
	qp.set_a(X, 2, -1);
	qp.set_a(Y, 2, 1);
	qp.set_b(2, 1);

	// by - ax^2    <- maximize
	// ax^2 - by    <- minimize
	qp.set_d(X, X, 2 * a);
	qp.set_c(Y, -b);

	Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
	assert(s.solves_quadratic_program(qp));

	print_solution(s, true);
}

void minimize(int a, int b) {
	Program qp(CGAL::LARGER, true, 0, false, 0);
	const int X = 0;
	const int Y = 1;
	const int Z = 2;

	// - x - y >= -4
    qp.set_a(X, 0, -1);
	qp.set_a(Y, 0, -1);
	qp.set_b(0, -4);

	// - 4x - 2y + z >= -ab
	qp.set_a(X, 1, -4);
	qp.set_a(Y, 1, -2);
	qp.set_a(Z, 1, 1);
	qp.set_b(1, -a*b);

    // x - y >= -1
	qp.set_a(X, 2, 1);
	qp.set_a(Y, 2, -1);
	qp.set_b(2, -1);

	// ax^2 - by + z^2 -> min
	qp.set_d(X, X, 2 * a);
	qp.set_d(Z, Z, 2);
	qp.set_c(Y, -b);

	Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
	assert(s.solves_quadratic_program(qp));

	print_solution(s, false);
}

int main() {
	while (true) {
		int p;
		std::cin >> p;
		if (p == 0) {
			return 0;
		}
		int a, b;
		std::cin >> a >> b;
		if (p == 1) {
			maximize(a, b);
		} else {
			minimize(a, b);
		}
	}
}
