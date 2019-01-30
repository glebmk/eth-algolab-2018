#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> V;
typedef vector<V> Table;

V get_pattern(int x, int k) {
	V pattern;

	while (k) {
		pattern.push_back(x % 2);
		x /= 2;
		k--;
	}

	reverse(pattern.begin(), pattern.end());
	return pattern;
}

int get_misses(V& first, V& second) {
	assert(first.size() == second.size());

	int count = 0;
	for (int i = 0; i < first.size(); i++) {
		if (first[i] != second[i]) {
			count++;
		}
	}
	return count;
}

void printv(V& v) {
	for (int elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

int rec(int i, int flipped, V& misses, int k, Table& memo) {
	if (i == -1) {
		return 0;
	}

	if (memo[i][flipped] != -1) {
		return memo[i][flipped];
	}

	int m = misses[i];
	
	int next_flipped = rec(i - 1, 1, misses, k, memo);
	int next_non_flipped = rec(i - 1, 0, misses, k, memo);
	
	int res = -1;
	if (!flipped) {
		res = m + min(next_flipped + 1, next_non_flipped);
	} else {
		res = k - m + min(next_flipped, next_non_flipped + 1);
	}

	memo[i][flipped] = res;
	
	return res;
}

void testcase() {
	int n, k, x;
	cin >> n >> k >> x;
	int m = n / k;

	V pattern = get_pattern(x, k);

	V misses;
	for (int i = 0; i < m; i++) {
		V cur;
		for (int j = 0; j < k; j++) {
			int bit;
			cin >> bit;
			cur.push_back(bit);
		}
		misses.push_back(get_misses(pattern, cur));
	}

	Table memo(m, V(2, -1));
	int res = min(rec(m - 1, 0, misses, k, memo), rec(m - 1, 1, misses, k, memo) + 1);
	cout << res << endl;
}

int main() {
	int t;
	cin >> t;
	while (t) {
		testcase();
		t--;
	}
	return 0;
}
