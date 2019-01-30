#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Point_2 Point;
typedef pair<Point, int> Pair;
typedef CGAL::Gmpq ET;
typedef CGAL::Quotient<ET> SolT;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Gang {
	int u, v, w;
};

void testcase() {
	long Z, U, V, W;
	cin >> Z >> U >> V >> W;
	int a, g;
	cin >> a >> g;

	vector<Pair> gangpos;
	vector<Gang> gangs;

	for (int i = 0; i < g; i++) {
		int x, y, u, v, w;
		cin >> x >> y >> u >> v >> w;
		gangpos.push_back({{x, y}, i});
		gangs.push_back({u, v, w});
	}

	Triangulation t;
	t.insert(gangpos.begin(), gangpos.end());

	vector<int> gang2price(g, -1);
	for (int i = 0; i < a; i++) {
		int x, y, z;
		cin >> x >> y >> z;
		Vertex gang = t.nearest_vertex({x, y});
		int id = gang->info();
		assert(0 <= id && id < g);
		if (gang2price[id] == -1 || z < gang2price[id]) {
			gang2price[id] = z;
		}
	}

	Program lp (CGAL::LARGER, true, 0, true, 24);

	int var = 0;
  	for (int i = 0; i < g; i++) {
		if (gang2price[i] == -1) {
			continue;
		}
		
		Gang& gang = gangs[i];
		lp.set_a(var, 0, gang.u);
		lp.set_a(var, 1, gang.v);
		lp.set_a(var, 2, gang.w);
		lp.set_c(var, gang2price[i]);

		var++;
	}
	lp.set_b(0, U);
	lp.set_b(1, V);
	lp.set_b(2, W);

	Solution s = CGAL::solve_linear_program(lp, ET());
	
	if (s.is_infeasible()) {
		cout << "H" << endl;
		return;
	}
	
	if (s.objective_value() > ET(Z)) {
		cout << "H" << endl;
	} else {
		cout << "L" << endl;
	}
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
