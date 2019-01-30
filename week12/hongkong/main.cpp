#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <queue>
#include <vector>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef K::FT FT;
typedef K::Point_2 Point;
typedef CGAL::Triangulation_face_base_with_info_2<FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Face_handle Face;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::All_faces_iterator All_faces_iterator;

const FT INF = -2;
const FT UNSET = -1;

struct F {
	FT d;
	Face face;

	bool operator<(const F& o) const {
		return d <= o.d;
	}
};

FT dist(const Point& p1, const Point& p2) {
	return CGAL::squared_distance(p1, p2);
}

void testcase() {
	int n, m;
	FT r;
	cin >> n >> m >> r;

	vector<Point> pts;
	for (int i = 0; i < n; i++) {
		long x, y;
		cin >> x >> y;
		pts.push_back({x, y});
	}

	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	priority_queue<F> q;
	for (All_faces_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
		if (t.is_infinite(f)) {
			f->info() = INF;
			if (!t.is_infinite(f->neighbor(0))) {
				FT d = dist(f->vertex(1)->point(), f->vertex(2)->point());
				q.push({d, f->neighbor(0)});	
			}
			if (!t.is_infinite(f->neighbor(1))) {
				FT d = dist(f->vertex(0)->point(), f->vertex(2)->point());
				q.push({d, f->neighbor(1)});
			}
			if (!t.is_infinite(f->neighbor(2))) {
				FT d = dist(f->vertex(0)->point(), f->vertex(1)->point());
				q.push({d, f->neighbor(2)});
			}
		} else {
			f->info() = UNSET;
			FT d = CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
			q.push({d, f});
		}
	}

	while (q.size() > 0) {
		FT d = q.top().d;
		Face f = q.top().face;
		q.pop();	
		if (f->info() != UNSET) {
			continue;
		}
		f->info() = d;
		q.push({ min(d, dist(f->vertex(1)->point(), f->vertex(2)->point())), f->neighbor(0) });
		q.push({ min(d, dist(f->vertex(0)->point(), f->vertex(2)->point())), f->neighbor(1) });
		q.push({ min(d, dist(f->vertex(0)->point(), f->vertex(1)->point())), f->neighbor(2) });
	}

	for (int i = 0; i < m; i++) {
		long x, y;
		cin >> x >> y;
		Point p {x, y};
		FT s;
		cin >> s;

		FT rs = CGAL::square(r + s);
		Vertex v = t.nearest_vertex(p);
		if (dist(p, v->point()) < rs) {
			cout << "n";
			continue;
		}
		
		Face f = t.locate(p);
		assert(f->info() != UNSET);
		if (f->info() == INF || f->info() >= 4 * rs) {
			cout << "y";
		} else {
			cout << "n";
		}
	}
	cout << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		//cerr << "testcase" << endl;
		t--;
		testcase();
	}
	return 0;
}
