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

typedef std::vector<int> AssetCovariances;

struct Asset {
	int c, r;
	Asset(int c, int r) : c(c), r(r) {};	
};

struct Investor {
	int max_cost, min_return, max_variance;
	Investor(int c, int r, int v) : max_cost(c), min_return(r), max_variance(v) {};
};

void process_investor(Investor investor, std::vector<Asset>& assets, std::vector<AssetCovariances>& covariances) {
	int n = assets.size();
	Program qp(CGAL::SMALLER, true, 0, false, 0);
	
	for (int i = 0; i < n; i++) {
		qp.set_a(i, 0, assets[i].c);
		qp.set_a(i, 1, -assets[i].r);
	}
	qp.set_b(0, investor.max_cost);
	qp.set_b(1, -investor.min_return);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j > i) {
				continue;
			}
			qp.set_d(i, j, 2 * covariances[i][j]);
		}
	}

	Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
 	assert(s.solves_quadratic_program(qp));
	
	if (s.is_infeasible()) {
		std::cout << "No." << std::endl;
	} else if (s.objective_value() > investor.max_variance) {
		std::cout << "No." << std::endl;
	} else {
		std::cout << "Yes." << std::endl;
	}
}

void testcase(int n, int m) {
	std::vector<Asset> assets;

	for (int i = 0; i < n; i++) {
		int c, r;
		std::cin >> c >> r;
		assets.push_back(Asset(c, r));
	}

	std::vector<AssetCovariances> covariances;
	for (int i = 0; i < n; i++) {
		AssetCovariances asset;
		for (int i = 0; i < n; i++ ) {
			int v;
			std::cin >> v;
			asset.push_back(v);
		}
		covariances.push_back(asset);
	}

	for (int i = 0; i < m; i++) {
		int c, r, v;
		std::cin >> c >> r >> v;
		process_investor(Investor(c, r, v), assets, covariances);
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (true) {
		int n, m;
		std::cin >> n >> m;
		if (n == 0) {
			return 0;
		}
		testcase(n, m);
	}
}
