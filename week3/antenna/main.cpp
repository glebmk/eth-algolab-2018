#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/enum.h>
#include <math.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef long long ll;

ll cell_to_long(const K::FT& x)
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

void testcase(int n) {
	K::Point_2 P[n];

	for (int i = 0; i < n; i++) {
		long x, y;
		cin >> x >> y;
		P[i] = K::Point_2(x, y);
	}

	if (n == 1) {
		cout << 1 << endl;
		return;
	}

	Min_circle mc(P, P + n, true);
	Traits::Circle c = mc.circle();

	cout << cell_to_long(CGAL::sqrt(c.squared_radius())) << endl;

}

int main() {
	ios_base::sync_with_stdio(false);
	while (true) {
		int n = -1;
		cin >> n;
		if (n == 0) {
			return 0;
		}
		testcase(n);
	}
	return 0;
}
