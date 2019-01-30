#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

K::Ray_2 read_ray() {
	long x1, y1, x2, y2;
	cin >> x1 >> y1 >> x2 >> y2;
	return K::Ray_2(K::Point_2(x1, y1), K::Point_2(x2, y2));
}

K::Segment_2 read_segment() {
	long x1, y1, x2, y2;
	cin >> x1 >> y1 >> x2 >> y2;
	return K::Segment_2(K::Point_2(x1, y1), K::Point_2(x2, y2));
}

void testcase(int n) {
	K::Ray_2 ray = read_ray();
	bool hit = false;

	for (int i = 0; i < n; i++) {
		K::Segment_2 segment = read_segment();
		if (!hit) {
			hit = CGAL::do_intersect(ray, segment);
		}
	}

	cout << (hit ? "yes" : "no") << endl;
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
