#include <iostream>
#include <vector>
#include <cassert>
#include <set>

using namespace std;

typedef vector<int> V;

int rec(int vertex, set<int>& vis, vector<V>& adj) {
	
}

void testcase() {
	int n, m;
	cin >> n >> m;
	assert(m == n - 1);

 	vector<V> adj(n);
	for (int i = 0; i < m; i++) {
		int s, t;
		cin >> s >> t;
		s--; t--;
		adj[s].push_back(t);
		adj[t].push_back(s);
	}

	set<int> vis;	
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
