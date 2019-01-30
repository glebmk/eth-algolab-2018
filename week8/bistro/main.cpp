#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;

void testcase(int n)
{
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    K::Point_2 p;
    std::cin >> p;
    pts.push_back(p);
  }
  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  bool assigned = false;
  K::FT min_distance;

  int m;
  std::cin >> m;

  for (int i = 0; i < m; ++i) {
	K::Point_2 p;
	std::cin >> p;
  	Vertex n = t.nearest_vertex(p);
	long ans = std::round(CGAL::to_double(CGAL::squared_distance(n->point(), p)));
	std::cout << ans << std::endl;
  }
}

int main() {
	while (true) {
		int n;
		std::cin >> n;
		if (n == 0) {
			return 0;
		}
		testcase(n);
	}
}
