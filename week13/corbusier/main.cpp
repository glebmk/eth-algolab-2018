#include <iostream>
#include <vector>

using namespace std;

void printv(vector<bool>& vector) {
	for (bool elem : vector) {
		cout << elem << " ";
	}
	cout << endl;
}

void testcase() {
	int n, target, k;
	cin >> n >> target >> k;
	//cerr << "n " << n << " target " << target << " k " << k << endl;	
	vector<bool> status(k, false);
	
	for (int i = 0; i < n; i++) {
		int h;
		cin >> h;
		h %= k;
		//cerr << "h " << h << endl;
		vector<bool> old_status = status;
		for (int j = 0; j < k; j++) {
			if (old_status[j]) {
				//cerr << "in if j = " << j << endl;
				//cerr << "(j + h) % k = " << (j + h) % k << endl;
				status[j] = true;
				status[(j + h) % k] = true;
			}
		}
		//printv(status);
		status[h] = true;
		//printv(status);
	}

	//printv(status);
	if (status[target]) {
		cout << "yes" << endl;
	} else {
		cout << "no" << endl;
	}
}

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
