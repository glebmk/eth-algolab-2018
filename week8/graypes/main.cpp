#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;

double ceil_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a + 1 <= x) a += 1;
  while (a - 1 >= x) a += 1;
  return a;
}

void testcase(int n)
{
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    K::Point_2 p;
    std::cin >> p;
    pts.push_back(p);
  }
  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  bool assigned = false;
  K::FT min_distance;
  // output all edges
  for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
	Vertex v1 = e->first->vertex((e->second + 1) % 3);
    Vertex v2 = e->first->vertex((e->second + 2) % 3);
	K::FT squared_distance = CGAL::squared_distance(v1->point(), v2->point());
	if (!assigned || squared_distance < min_distance) {
		min_distance = squared_distance;
		assigned = true;
	}
  }
  double ans = std::ceil(std::sqrt(CGAL::to_double(min_distance)) * 50);
  std::cout << ans << std::endl;
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
