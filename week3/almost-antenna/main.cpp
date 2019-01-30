#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/enum.h>
#include <math.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef long long ll;

ll ceil_to_long(const K::FT& x)
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

ll compute_result(std::vector<K::Point_2>& P, int index_to_remove) {
	K::Point_2 removed = P[index_to_remove];
	P.erase(P.begin() + index_to_remove);
	Min_circle mc(P.begin(), P.end(), true);
	P.insert(P.begin() + index_to_remove, removed);
	Traits::Circle c = mc.circle();
	return ceil_to_long(CGAL::sqrt(c.squared_radius()));
}

void testcase(int n) {
	std::vector<K::Point_2> P;

	for (int i = 0; i < n; i++) {
		long x, y;
		std::cin >> x >> y;
		P.push_back(K::Point_2(x, y));
	}
	
	Min_circle mc(P.begin(), P.end(), true);
	std::vector<int> boundary_points_indexes;
	for (int i = 0; i < P.size(); i++) {
		if (mc.has_on_boundary(P[i])) {
	  		boundary_points_indexes.push_back(i);
		}
	}
	
    ll min_result = -1;
	for (int index_to_remove : boundary_points_indexes) {
		ll result = compute_result(P, index_to_remove);
		if (min_result == -1 || result < min_result) {
			min_result = result;
		}
	}

	std::cout << min_result << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	while (true) {
		int n = -1;
		std::cin >> n;
		if (n == 0) {
			return 0;
		}
		testcase(n);
	}
	return 0;
}
