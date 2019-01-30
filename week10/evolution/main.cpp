#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cassert>

using namespace std;

void rec(int v, vector<int>& parent, vector<vector<int>>& exps);

int find(int v, long age, vector<long>& ages, vector<vector<int>>& exps);

void testcase() {
	int n, q;
	cin >> n >> q;

	map<string, int> m;
	vector<long> ages;
	vector<string> names;
	for (int i = 0; i < n; i++) {
		string name;
		long age;
		cin >> name >> age;
		assert(name != "");
		m.insert(pair<string, int>(name, i));
		ages.push_back(age);
		names.push_back(name);
	}

	vector<int> parent(n, -1);
	for (int i = 0; i < n - 1; i++) {
		string s, p;
		cin >> s >> p;
		int scode = m.find(s)->second;
		int pcode = m.find(p)->second;
		assert(0 <= scode && scode < n);
		assert(0 <= pcode && pcode < n);
		parent[scode] = pcode;
	}

	vector<vector<int>> exps(n);
	for (int i = 0; i < n; i++) {
		rec(i, parent, exps);
	}

	for (int i = 0; i < q; i++) {
		string name;
		long age;
		cin >> name;
		cin >> age;
		assert(name != "");	
		int id = m.find(name)->second;
		assert(0 <= id && id < n);
		int out = find(id, age, ages, exps);	
		assert(0 <= out && out < n);
		cout << names[out] << " ";
	}
	cout << endl;	
}

int find(int v, long age, vector<long>& ages, vector<vector<int>>& exps) {
	assert(ages[v] <= age);
	int n = exps.size();
	assert(0 <= v && v < n);
	vector<int>& jump = exps[v];
	for (int i = jump.size() - 1; i >= 0; i--) {
		assert(0 <= jump[i] && jump[i] < n);
		assert(jump[i] != v);
		if (ages[jump[i]] <= age) {
			return find(jump[i], age, ages, exps);
		}
	}
	return v;
}

void rec(int v, vector<int>& parent, vector<vector<int>>& exps) {
	if (parent[v] == -1 || exps[v].size() > 0) {
		return;
	}
	int n = exps.size();
	assert(0 <= v < n);
	rec(parent[v], parent, exps);

	vector<int>& exp = exps[v];
	exp.push_back(parent[v]);
	
	int power = 0;
	int next = parent[v];
	while (exps[next].size() > power) {
		next = exps[next][power];
		assert(0 <= next && next < n);
		exp.push_back(next);
		power++;
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
}
