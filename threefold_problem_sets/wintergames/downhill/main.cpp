#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef vector<long> V;

struct P {
	long x, y;
};

bool overlaps(const P& p1, const P& p2, const long r) {
	long dx = p1.x - p2.x;
	long dy = p1.y - p2.y;
	return dx * dx + dy * dy < r * r;
}

const int UNSET = -2;
const int OUT = -1;
const int RED = 0;
const int BLUE = 1; 

void testcase() {
	int n;
	long r;
	cin >> n >> r;
	vector<P> points;

	for (int i = 0; i < n; i++) {
		long x, y;
		cin >> x >> y;
		points.push_back({x, y});
	}

	vector<V> adj(n);
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (overlaps(points[i], points[j], r)) {
				adj[i].push_back(j);
				adj[j].push_back(i);
			}
		}
	}

	for (int i = 0; i < n; i++) {
		assert(adj[i].size() <= 2);
	}


}

int main() {
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
}

