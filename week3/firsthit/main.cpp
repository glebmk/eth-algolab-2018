#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <math.h>
#include <vector>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

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

long floor_to_long(const K::FT& x)
{
 	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
 	while (a+1 <= x) a += 1;
 	return lround(a);
}

void maybe_update(const K::Ray_2& ray, const K::Point_2& intersection, K::Point_2& first_intersection, K::FT& min_distance, bool& found) {
	K::FT distance = CGAL::squared_distance(ray.source(), intersection);
	if (!found || min_distance > distance) {
		found=true;
		first_intersection = intersection;
		min_distance = distance;
	}
}

void testcase(int n) {
	K::Ray_2 ray = read_ray();
	K::FT min_distance;
	K::Point_2 first_intersection;
	bool found = false;

	for (int i = 0; i < n; i++) {
		K::Segment_2 segment = read_segment();
		if (CGAL::do_intersect(ray, segment)) {
			auto o = CGAL::intersection(ray, segment);
			
			if (K::Point_2* intersection = boost::get<K::Point_2>(&*o)) {
				maybe_update(ray, *intersection, first_intersection, min_distance, found);
			} else if (K::Segment_2* intersection = boost::get<K::Segment_2>(&*o)) {
				maybe_update(ray, intersection->source(), first_intersection, min_distance, found);
				maybe_update(ray, intersection->target(), first_intersection, min_distance, found);
			} else {
				throw runtime_error("unexpected intersection");
			}
		}
	}

	if (found) {
		cout << floor_to_long(first_intersection.x()) << " " << floor_to_long(first_intersection.y()) << endl;
	} else {
		cout << "no" << endl;
	}
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

