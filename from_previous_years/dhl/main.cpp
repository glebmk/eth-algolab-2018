#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef vector<int> V;
const int UNSET = -1;

int rec(const int ia, const int ib, const V& sa, const V& sb, vector<V>& memo) {
	assert(ia >= 0 && ib >= 0);

	if (ia == 0) {
		return (sa[0] - 1) * (sb[ib] - ib - 1); 		
	} else if (ib == 0) {
		return (sb[0] - 1) * (sa[ia] - ia - 1);
	}
	
	if (memo[ia][ib] != UNSET) {
		return memo[ia][ib];
	}

	int ans = UNSET;

	for (int nib = 0; nib < ib; nib++) {
		int cur = (sa[ia] - sa[ia - 1]  - 1) * (sb[ib] - sb[nib] - (ib - nib));
		int newans = cur + rec(ia - 1, nib, sa, sb, memo);
		if (ans == UNSET || newans < ans) {
			ans = newans;
		}
	}

	for (int nia = 0; nia < ia; nia++) {
		int cur = (sb[ib] - sb[ib - 1] - 1) * (sa[ia] - sa[nia] - (ia - nia));
		int newans = cur + rec(nia, ib - 1, sa, sb, memo);
		if (ans == UNSET || newans < ans) {
			ans = newans;
		}
	}
	
	assert(ans != UNSET);

	memo[ia][ib] = ans;

	return ans;
}

V readv(int n) {
	V v;
	for (int i = 0; i < n; i++) {
		int e;
		cin >> e;
		v.push_back(e);
	}
	return v;
}

V sumv(const V& v) {
	V s;
	assert(v.size() > 0);
	int cum = 0;
	for (int i = 0; i < v.size(); i++) {
		cum += v[i];
		s.push_back(cum);
	}
	return s;
}

void testcase() {
	//cerr << "testcase" << endl;
	int n;
	cin >> n;
	V va = readv(n);
	V vb = readv(n);
	V sa = sumv(va);
	V sb = sumv(vb);

	vector<V> memo(n, V(n, UNSET));
	int minsum = rec(n - 1, n - 1, sa, sb, memo);
	cout << minsum << endl;
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
