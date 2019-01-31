#include <iostream>
#include <vector>

using namespace std;

int gcd(int a, int b) {
	if (a > b) {
		return gcd(b, a);
	}
	
	if (a == 0) {
		return b;
	}

	return gcd(b % a, a);
}

const int UNSET = -1;

void testcase() {
	// n is bigger.
	int l, m, n;
	cin >> l >> m >> n;

	int common = (m * n) / gcd(m, n);
	int num_common = l / common;

	int num_n = (num_common * common) / n;
	int residual = l - num_common * common;
	
	// TODO[gleb] in this table I want to maximize the n count.
	vector<int> DP(residual + 1, UNSET);
	DP[0] = 0;
	for (int i = 0; i < residual; i++) {
		if (DP[i] == UNSET) {
			continue;	
		}
		
		if (i + n <= residual) {
			if (DP[i + n] == UNSET || DP[i + n] < DP[i] + 1) {
				DP[i + n] = DP[i] + 1;
			}
		}
		
		if (i + m <= residual) {
			if (DP[i + m] == UNSET || DP[i + m] < DP[i]) {
				DP[i + m] = DP[i];	
			}
		}

	}

	for (int i = residual; i >= 0; i--) {
		if (DP[i] == UNSET) {
			continue;
		}
		num_n += DP[i];
		int num_m = (i - n * DP[i]) / m;
		assert((i - n * DP[i]) % m == 0);
		int uncovered = l - num_n * n - num_m * m;
		cout << num_m << " " << num_n << " " << uncovered << endl;
		return;
	}
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
