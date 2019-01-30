#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Vertex_handle Vertex;
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef K::Point_2 Point;
typedef pair<Point, int> Radio;

bool dfs(Vertex& v, bool color, int comp, Triangulation& t, vector<bool>& colors, vector<int>& components, long r) {
	int vi = v->info();
	if (components[vi] != -1) {
		return true;
	}

	colors[vi] = color;
	components[vi] = comp;

	Vertex_circulator ov = incident_vertices(vertex);
	do {
		if (CGAL::squared_distance(v->point(), ov->point()) > r * r) {
			continue;
		}
			
		int ovi = ov->info();
		if (comp[ovi] != -1 && color[ovi] == color) {
			return false;
		} else if (comp[ovi] == -1) {
			dfs(ov, !color, comp, t, colors, components, r);	
		}
	} while (++ov != t.incident_vertices(vertex));
}

void testcase() {
	int n, m;
	long r;
	cin >> n >> m >> r;

	vector<Radio> radios;
	for (int i = 0; i < n; i++) {
		Point p;
		cin >> p;
		radios.push_back(pair<Point, int>(p, i));
	}

	Triangulation t;
	t.insert(radios.begin(), radios.end());
	
	vector<int> components(n, -1);
	vector<int> colors(n, -1);
	int comp = 0;
	// Now circulate through vertexes and run dfs from them.
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
