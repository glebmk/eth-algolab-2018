#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

typedef vector<int> Vec;

Vec readvec(int size) {
	Vec vec;
	for (int i = 0; i < size; i++) {
		int elem;
		cin >> elem;
		vec.push_back(elem);
	}
	return vec;
}

Vec build_w2c(Vec strs, Vec weights) {
	int n = strs.size();
	int m = weights.size();

	Vec w2c(m + 1);
	
	int is = n - 1;
	for (int w = m - 1; w >= 0; w--) {
		while (is >= 0 && strs[is] >= weights[w]) {
			w2c[w] += 1;
			is--;
		}
		w2c[w] += w2c[w + 1];		
	}
	return w2c;
}

void print(Vec v) {
	for (int elem : v) {
		cout << elem << " ";
	}
	cout << endl;
}

void testcase() {
	int n, m;
	cin >> n >> m;

	Vec strs = readvec(n);
	Vec weights = readvec(m);

	sort(strs.begin(), strs.end());
	sort(weights.begin(), weights.end());

	Vec w2c = build_w2c(strs, weights);
	//print(w2c);
	if (w2c[m - 1] == 0) {
		cout << "impossible" << endl;
		return;
	}

	int available = 0, rounds = 0;

	for (int w = m - 1; w >= 0; w--) {
		available += (w2c[w] - w2c[w + 1]) * (rounds + 1);
		if (available == 0) {
			rounds++;
			available += w2c[w];
		}
		available--;
	}
	//cerr << "rounds " << rounds << endl;
	cout << rounds * 3 + 2 << endl;
}

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		//cerr << "testcase " << i << endl;
		testcase();
	}
	return 0;
}
