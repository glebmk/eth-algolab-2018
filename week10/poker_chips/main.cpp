#include <math.h>
#include <vector>
#include <iostream>
#include <set>
#include <map>

using namespace std;

typedef vector<int> V;
typedef vector<vector<int>> Table;
typedef map<V, int> Map;

int k2score(int k) {
	if (k == 1) {
		return 0;
	}
	return round(pow(2, k - 2));
}

void printv(V& v) {
	for (int elem : v) {
		cerr << elem << " ";
	}
	cerr << endl;
}

void prints(set<int>& s) {
	for (int elem : s) {
		cerr << elem << " ";
	}
	cerr << endl;
}

int rec(V&, Map&, Table&);

int rec_helper(int i, int k, int chip, V& state, Map& memo, Table& chips) {
	int n = state.size();
	
	if (i == n) {
		if (k == 0) {
			return 0;
		}
		return k2score(k) + rec(state, memo, chips);
	}

	int max_score = rec_helper(i + 1, k, chip, state, memo, chips);

	if (state[i] != -1 && chips[i][state[i]] == chip) {
		state[i]--;
		max_score = max(max_score, rec_helper(i + 1, k + 1, chip, state, memo, chips));
		state[i]++;
	}
	return max_score;
}

int rec(V& state, Map& memo, Table& chips) {
	int n = state.size(); 

	auto search = memo.find(state);
	if (search != memo.end()) {
		return search->second;
	}

	set<int> distinct;
	for (int i = 0; i < n; i++) {
		if (state[i] != -1) {
			distinct.insert(chips[i][state[i]]);
		}
	}

	/*
	cerr << "state" << endl;
	printv(state);
	cerr << "distinct" << endl;
	prints(distinct);
	*/

	if (distinct.size() == 0) {
		return 0;
	}

	int max_score = 0; 
	for (int chip : distinct) {
		max_score = max(max_score, rec_helper(0, 0, chip, state, memo, chips));
	}

	memo.insert(pair<V, int>(state, max_score));
	return max_score;
}

void testcase() {
	int n;
	cin >> n;

	V sizes;
	for (int i = 0; i < n; i++) {
		int m;
		cin >> m;
		sizes.push_back(m);
	}

	Table chips;
	for (int i = 0; i < n; i++) {
		V row;
		for (int j = 0; j < sizes[i]; j++) {
			int c;
			cin >> c;
			row.push_back(c);
		}
		chips.push_back(row);
	}

	Map memo;
	for (int i = 0; i < n; i++) {
		sizes[i]--;
	}
	//cerr << "state" << endl;
	//printv(sizes);
	int max_score = rec(sizes, memo, chips);
	cout << max_score << endl;
}

int main() {
	int t;
	cin >> t;
	while (t) {
		t--;
		//cerr << "testcase" << endl;
		testcase();
	}
}
