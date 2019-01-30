#include <iostream>
#include <vector>

using namespace std;

typedef vector<long> V;

const long NOT_FILLED = -1;
const long NOT_VALID = -2;

void printv(V& v) {
	for (long elem : v) {
		cout << elem << " ";
	}
	cout << endl;
}

long rec(int pos, int att, vector<V>& memo, V& ends, int n) {
	if (att == 0) {
		return 0;
	}
	if (pos == n) {
		return NOT_VALID;
	}
	if (memo[pos][att] != NOT_FILLED) {
		return memo[pos][att];
	}

	long val = rec(pos + 1, att, memo, ends, n);
	if (ends[pos] != -1) {
		long new_val = rec(ends[pos] + 1, att - 1, memo, ends, n);
		if (new_val != NOT_VALID) {
			val = max(new_val + ends[pos] + 1 - pos, val);
		}
	}

	memo[pos][att] = val;

	return val;
}

void testcase() {
	long n, m, k;
	cin >> n >> m >> k;

	V defs;
	for (int i = 0; i < n; i++) {
		long def;
		cin >> def;
		defs.push_back(def);
	}
	
	V ends(n, -1);
	int l = 0, r = 0;
	int sum = 0;
	while (r < n) {
		sum += defs[r];
		//cerr << "l " << l << " r " << r << " sum " << sum << endl;
		while (sum > k) {
			sum -= defs[l];
			l++;
		}
		if (sum < k) {
			r++;
			continue;
		}
		if (sum == k) {
			ends[l] = r;
		}
		sum -= defs[l];
		l++;
		r++;
	}

	/*
	cerr << "defs" << endl;
	printv(defs);
	cerr << "ends" << endl;
	printv(ends);
	*/

	vector<V> memo(n + 1, V(m + 1, NOT_FILLED));
	long out = rec(0, m, memo, ends, n);
	if (out == NOT_VALID) {
		cout << "fail" << endl;
	} else {
		cout << out << endl;
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
