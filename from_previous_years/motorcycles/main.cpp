#include <iostream>
#include <vector>
#include <CGAL/Gmpq.h>
#include <cassert>

typedef CGAL::Gmpq ET;

using namespace std;

const ET INIT = ET(-1);

void testcase() {
	int n;
	cin >> n;

	vector<ET> ks;

	for (int i = 0; i < n; i++) {
		long y0, x1, y1;
		cin >> y0 >> x1 >> y1;
		ET k = (y1 - y0) / x1;
		ks.push_back(k);
	}

	ET min_k_abs = INIT;
	bool min_k_abs_pos;
	for (ET k : ks) {
		bool pos = true;
		if (k < 0) {
			pos = false;
			k = -k;
		}
		if (k < min_k_abs || min_k_abs == INIT) {
			min_k_abs_pos = pos;
			min_k_abs = k;
		} else if (k == min_k_abs && pos) {
			min_k_abs_pos = true;	
		}
	}

	assert(min_k_abs != INIT);	
	ET k_survive = min_k_abs;
	if (!min_k_abs_pos) {
		k_survive = -k_survive;
	}

	for (int i = 0; i < n; i++) {
		if (ks[i] == k_survive) {
			cout << i << " ";
		}
	}
	cout << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
}
