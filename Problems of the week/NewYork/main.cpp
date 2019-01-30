#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>

using namespace std;

typedef vector<int> V;
typedef unordered_map<int, int> Map;

void printv(V& v) {
	for (int elem : v) {
		cout << elem << " ";
	}
	cout << endl;
}

bool comp(int left, int right) {
	return left < right;
}

bool comprev(int left, int right) {
	return left > right;
}

class Heap {
	bool minheap;
	vector<int> h;
public:
	Heap(bool minheap) : minheap(minheap) {};

	void push(int v) {
		h.push_back(v);
		if (minheap) {
			push_heap(h.begin(), h.end(), comprev);
		} else {
			push_heap(h.begin(), h.end(), comp);
		}
	}

	int pop() {
		assert(h.size() > 0);
		if (minheap) {
			pop_heap(h.begin(), h.end(), comprev); 
		} else {
			pop_heap(h.begin(), h.end(), comp);
		}
		int v = h.back();
		h.pop_back();
		return v;	
	}

	int peek() {
		assert(h.size() > 0);
		return h[0];
	}
	
	int size() {
		return h.size();
	}
};

void cleanup(Heap& heap, Map& count) {
	while (true) {
		auto search = count.find(heap.peek());
		assert(search != count.end());
		if (search->second == 0) {
			heap.pop();
		} else {
			return;
		}
	}
}

void checked_change(int key, int delta, Map& count) {
	auto search = count.find(key);
	assert(search != count.end());
	search->second += delta;
}

void first(int v, V& path, Map& count, Heap& minheap, Heap& maxheap, V& temp, vector<V>& routes, int m, int k, set<int>& out) {
	path.push_back(v);
	int curtemp = temp[v];
	minheap.push(curtemp);
	maxheap.push(curtemp);
	checked_change(curtemp, 1, count);

	if (path.size() > m) {
		int todel = temp[path[path.size() - m - 1]];
		checked_change(todel, -1, count);
	}

	cleanup(minheap, count);
	cleanup(maxheap, count);

	if (path.size() >= m) {
		int maxt = maxheap.peek();
		int mint = minheap.peek();
		//cerr << "maxt " << maxt << " mint " << mint << endl;
		assert(maxt >= mint);
		int diff = maxt - mint;
		if (diff <= k) {
			out.insert(path[path.size() - m]);
		}
	}
}

void second(int v, V& path, Map& count, Heap& minheap, Heap& maxheap, V& temp, vector<V>& routes, int m, int k, set<int>& out) {
	int curtemp = temp[v];

	if (path.size() > m) {
		int toadd = temp[path[path.size() - m - 1]];
		checked_change(toadd, 1, count);
		minheap.push(toadd);
		maxheap.push(toadd);
	}

	checked_change(curtemp, -1, count);
	path.pop_back();
}

void testcase() {
	int n, m, k;

	cin >> n >> m >> k;

	V temp;
	Map count;
	for (int i = 0; i < n; i++) {
		int t;
		cin >> t;
		auto search = count.find(t);
		if (search == count.end()) {
			count.insert(pair<int, int>(t, 0));
		}
		temp.push_back(t);
	}

	vector<V> routes(n);
	for (int i = 0; i < n - 1; i++) {
		int u, v;
		cin >> u >> v;
		routes[u].push_back(v);
	}

	Heap minheap(true), maxheap(false);
	V path;
	set<int> out;

	set<int> processed;
	vector<int> dfs;
	dfs.push_back(0);

	while (dfs.size() > 0) {
		int v = dfs.back();
		dfs.pop_back();

		if (processed.count(v)) {
			second(v, path, count, minheap, maxheap, temp, routes, m, k, out);
			continue;
		}
		
		processed.insert(v);
		dfs.push_back(v);
		for (int next : routes[v]) {
			dfs.push_back(next);
		}

		first(v, path, count, minheap, maxheap, temp, routes, m, k, out);
	}
	
	V outsorted(out.begin(), out.end());
	sort(outsorted.begin(), outsorted.end());
	if (outsorted.size() == 0) {
		cout << "Abort mission" << endl;
	} else {
		printv(outsorted);
	}
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
