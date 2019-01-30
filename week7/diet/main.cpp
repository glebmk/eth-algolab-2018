#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>
// choose exact integral type
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef long long ll;
typedef CGAL::Quotient<ET> SolutionValue;

struct Nutrient {
	int min;
	int max;
	Nutrient(int min, int max) : min(min), max(max) {};
};

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

void print_solution(Solution& s) {
	if (s.is_infeasible()) {
		std::cout << "No such diet.";
	} else if (s.is_unbounded()) {
		std::cout << 0;
	} else {
		ll sol = -ceil(-s.objective_value());
        std::cout << sol;
    }
	std::cout << std::endl;
}

void solve(int n, int m, std::vector<Nutrient>& nutrients, std::vector<int>& prices, std::vector<std::vector<int>>& products_nutrients) {
	Program lp(CGAL::SMALLER, true, 0, false, 0);
	
	for (int j = 0; j < m; j++) {
		lp.set_c(j, prices[j]);
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			lp.set_a(j, 2 * i, products_nutrients[j][i]);
			lp.set_a(j, 2 * i + 1, - products_nutrients[j][i]);
		}
		lp.set_b(2 * i, nutrients[i].max);
		lp.set_b(2 * i + 1, -nutrients[i].min);
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	assert(s.solves_linear_program(lp));
	print_solution(s);
}

void testcase(int n, int m) {
	std::vector<Nutrient> nutrients;
	for (int i = 0; i < n; i++) {
		int min, max;
		std::cin >> min >> max;
		nutrients.push_back(Nutrient(min, max));
	}

	std::vector<int> prices;
	std::vector<std::vector<int>> products_nutrients;
	
	for (int j = 0; j < m; j++) {
		int price;
		std::cin >> price;
		prices.push_back(price);

		std::vector<int> product;
		for (int i = 0; i < n; i++) {
			int c;
			std::cin >> c;
			product.push_back(c);
		}
		products_nutrients.push_back(product);
	}

	solve(n, m, nutrients, prices, products_nutrients);
}

int main() {
	while (true) {
		int n, m;
		std::cin >> n >> m;
		if (n == 0) {
			return 0;
		}
		testcase(n, m);
	}
}

