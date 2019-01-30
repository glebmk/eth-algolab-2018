#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <vector>
#include <CGAL/Line_2.h>
#include <assert.h>
#include <unordered_map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef long long ll;

int COST_NOT_INIT = -10;
int LEG_NOT_COVERED = -17;

class TriangleSide {
	const Point q0, q1;
  public:
  	TriangleSide(const Point q0, const Point q1) : q0(q0), q1(q1) {}

	bool left_turn_or_collinear(const Point& p) const {
		return !CGAL::right_turn(q0, q1, p);
	}
};

class Triangle {
	const TriangleSide side0, side1, side2;
  public:
  	Triangle(const TriangleSide side0, const TriangleSide side1, const TriangleSide side2) : side0(side0), side1(side1), side2(side2) {}

	bool point_in(const Point& p) const {
		return side0.left_turn_or_collinear(p) && 
				side1.left_turn_or_collinear(p) &&
				side2.left_turn_or_collinear(p);
	}
};

class Leg {
	const Point pos1, pos2;
  public:
  	Leg(const Point pos1, const Point pos2) : pos1(pos1), pos2(pos2) {}

	bool in_triangle(const Triangle& triangle) {
		bool val = triangle.point_in(pos1) && triangle.point_in(pos2);
		return val;
	}
};

Point read_point() {
	ll x, y;
	std::cin >> x >> y;
	Point point = Point(x, y);
	return point;
}

void maybe_swap(Point& q0, Point& q1, const Point& other) {
	if (!CGAL::left_turn(q0, q1, other)) {
		std::swap(q0, q1);
	}
}

Triangle read_triangle() {
	Point q0 = read_point();
	Point q1 = read_point();
	Point q2 = read_point();
	Point q3 = read_point();
	Point q4 = read_point();
	Point q5 = read_point();

	maybe_swap(q0, q1, q2);
	maybe_swap(q2, q3, q0);
	maybe_swap(q4, q5, q0);

	TriangleSide side0(q0, q1), side1(q2, q3), side2(q4, q5);
	return Triangle(side0, side1, side2);
}

void maybe_renew_min(int current_triangle_num, int& min_cost, std::vector<int>& leg2triangle_num) {
	int min_triangle = *std::min_element(std::begin(leg2triangle_num), std::end(leg2triangle_num));
	if (min_triangle == LEG_NOT_COVERED) {
		return;
	}
	int new_cost = current_triangle_num - min_triangle + 1;
	if (min_cost == COST_NOT_INIT || new_cost < min_cost) {
		min_cost = new_cost;
	}
}

void testcase() {
	int m, n;
	std::cin >> m >> n;
	std::vector<Leg> legs;

	Point prev_point = read_point();
	for (int i = 0; i < m - 1; i++) {
		Point point = read_point();
		legs.push_back(Leg(prev_point, point));
		prev_point = point;
	}
	assert(legs.size() == m - 1);
	
	std::vector<int> leg2triangle_num;
	for (int i = 0; i < m - 1; i++) {
		leg2triangle_num.push_back(LEG_NOT_COVERED);
	}
    
	int min_cost = COST_NOT_INIT;
	for (int triangle_num = 0; triangle_num < n; triangle_num++) {
		Triangle triangle = read_triangle();
		for (int leg_i = 0; leg_i < legs.size(); leg_i++) {
			if (legs[leg_i].in_triangle(triangle)) {
				leg2triangle_num[leg_i] = triangle_num;
			}
		}
		maybe_renew_min(triangle_num, min_cost, leg2triangle_num);
	}
	assert(min_cost != COST_NOT_INIT);
	std::cout << min_cost << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int c;
	std::cin >> c;
	for (int i = 0; i < c; i++) {
		testcase();
	}
	return 0;
}
