#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long UNSET = -1;
typedef vector<long> V;

void testcase() {
	int k;
	cin >> k;
	vector<V> balls(k, V(k, UNSET));
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			long v;
			cin >> v;
			balls[i][j] = v;
		}
	}

	vector<V> cum(k + 1, V(k + 1, UNSET));
	for (int i = 0; i < k + 1; i++) {
		cum[k][i] = 0;
	}

	for (int i = 0; i < k; i++) {
		cum[k - 1][i] = balls[k - 1][i];	
	}

	for (int i = k - 2; i >= 0; i--) {
		for (int j = 0; j < i + 1; j++) {
			cum[i][j] = balls[i][j] + cum[i + 1][j] + cum[i + 1][j + 1] - cum[i + 2][j + 1];
		}
	}

	long m = cum[0][0];
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < i + 1; j++) {
			m = max(m, cum[i][j]);
		}
	}
	cout << m << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
	return 0;
}
