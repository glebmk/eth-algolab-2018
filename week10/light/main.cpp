#include <iostream>
#include <cassert>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <set>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle  Vertex;
typedef K::Point_2 Point;

using namespace std;

void printv(vector<int>& out) {
	for (int elem : out) {
		cerr << elem << " ";
	}
	cerr << endl;
}

struct Person {
	Point p;
	long r;
};

bool inrad(Person& person, Point& p, long h) {
	long dist = h + person.r;
	return CGAL::squared_distance(person.p, p) < dist * dist;
}

vector<bool> run(vector<Point>& lights, int leave_first, vector<Person>& people, long h) {
	Triangulation t;
	int offset = lights.size() - leave_first;
	t.insert(lights.begin(), lights.end() - offset);

	int m = people.size();
	vector<bool> out(m, true);
	for (int i = 0; i < m; i++) {
		Vertex v = t.nearest_vertex(people[i].p);
		if (inrad(people[i], v->point(), h)) {
			out[i] = false;
		}
	}

	return out;
}

bool at_least_one_alive(vector<bool>& v) {
	for (bool elem : v) {
		if (elem) {
			return true;
		}
	}
	return false;
}

void testcase() {
	int m, n;
	cin >> m >> n;
	vector<Person> people;
	for (int i = 0; i < m; i++) {
		Point p;
		cin >> p;
		long r;
		cin >> r;
		people.push_back({p, r});
	}
	
	long h;
	cin >> h;
	vector<Point> lights;
	for (int i = 0; i < n; i++) {
		Point p;
		cin >> p;
		lights.push_back(p);
	}

	int lower = 0;
	int upper = n;

	while (lower + 1 < upper) {
		int mid = (lower + upper) / 2;
		//cerr << "mid " << mid << endl;
		vector<bool> out = run(lights, mid, people, h);
		if (at_least_one_alive(out)) {
		 	lower = mid;
		} else {
			upper = mid;
		}
	}

	vector<bool> out = run(lights, upper, people, h);	
	if (!at_least_one_alive(out)) {
		out = run(lights, lower, people, h);
	}
	assert(at_least_one_alive(out));

	for (int i = 0; i < m; i++) {
		if (out[i]) {
			cout << i << " ";
		}
	}
	cout << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
	return 0;
}
