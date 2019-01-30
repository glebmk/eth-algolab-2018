#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex;
typedef K::Segment_2 Segment;
typedef K::Point_2 Point;
typedef long long ll;
typedef std::map<Point, K::FT> Map;

void maybe_add(Map& map, Point& p, K::FT& dist) {
	// ll dist = std::roundl(CGAL::to_double(dist_ft));
	auto search = map.find(p);
	if (search == map.end()) {
		map.insert(std::pair<Point, K::FT>(p, dist));
	} else {
		search->second = std::min(search->second, dist);
	}
}

double time_to_sqdist(double time) {
	double dist = time * time + 0.5;
	return dist * dist;
}

void print_time(double time) {
	double sqdist = time_to_sqdist(time);
	std::cout << "time " << time << " sqdist " << sqdist << std::endl;
}

ll to_time(K::FT sqdist_ft) {
	double sqdist = CGAL::to_double(sqdist_ft);
	double time = std::round(std::sqrt(std::sqrt(sqdist) - 0.5));

	//print_time(time);
	while (time_to_sqdist(time) < sqdist) {
		time += 1;
	}
	while (time_to_sqdist(time - 1) >= sqdist && time - 1 >= 0) {
		time -= 1;
	}
	return llround(time);
}

void testcase(int n) {
	int left, bottom, right, top;    
	std::cin >> left >> bottom >> right >> top;

    std::vector<Point> pts;
    pts.reserve(n);
    for (int i = 0; i < n; ++i) {
      	Point p;
      	std::cin >> p;
      	pts.push_back(p);
    }
    
	Triangulation t;
    t.insert(pts.begin(), pts.end());
    
	Map map;

    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Point p1 = e->first->vertex((e->second + 1) % 3)->point();	
		Point p2 = e->first->vertex((e->second + 2) % 3)->point();
		K::FT sqdist = CGAL::squared_distance(p1, p2) / 4;
		maybe_add(map, p1, sqdist);
		maybe_add(map, p2, sqdist);
	}

	// Create segments that describe the cell and renew the min
	// distance with the distance to the cell walls.
	Point tl(left, top), tr(right, top), bl(left, bottom), br(right, bottom);
	std::vector<Segment> segments;
	segments.push_back({tl, bl});
	segments.push_back({tr, br});
	segments.push_back({tl, tr});
	segments.push_back({bl, br});

	for (Point p : pts) {
		std::vector<K::FT> distances;
		for (Segment& s : segments) {
			distances.push_back(CGAL::squared_distance(p, s));
		}
		K::FT min_cell_dist = *std::min_element(distances.begin(), distances.end());
		maybe_add(map, p, min_cell_dist);
	}
	
	std::vector<K::FT> distances;
	for (auto& elem : map) {
		distances.push_back(elem.second);
	}
	std::sort(distances.begin(), distances.end());		
	
	K::FT min_dist = distances[0];
	K::FT mid_dist = distances[distances.size() / 2];
	K::FT max_dist = distances[distances.size() - 1];

	// std::cerr << "min dist " << min_dist << " mid dist " << mid_dist << " max dist " << max_dist << std::endl;
	std::cout << to_time(min_dist) << " " << to_time(mid_dist) << " " << to_time(max_dist) << std::endl;
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
