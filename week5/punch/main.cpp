#include <iostream>
#include <vector>

using namespace std;

struct Bev {
	int c, v;
};

struct Stat {
	int cost, count;
	Stat() : cost(-1), count(-1) {};
	Stat(int cost, int count) : cost(cost), count(count) {};

	bool operator<(const Stat& other) const {
		if (cost < other.cost) {
			return true;
		}
		return (cost == other.cost & count >= other.count);
	}
};

void testcase() {	
	int n, k;
	
	cin >> n >> k;

	vector<Bev> bevs;
	int max_v = 0;
	for (int i = 0; i < n; i++) {
		int c, v;
		cin >> c >> v;
		max_v = max(v, max_v);
		Bev bev {c, v};
		bevs.push_back(bev);
	}

	vector<vector<Stat>> DP(n, vector<Stat>(k + max_v));
	
	for (int i = 0; i < n; i++) {
		DP[i][0] = Stat(0, 0);
	}

	// Fill the first row of the DP table;
	for (int i = 1; i < k + max_v; i++) {
		int v = bevs[0].v;
		int to_buy = i / v;
		if (i % v) {
			to_buy++;
		}
		DP[0][i] = Stat(to_buy * bevs[0].c, 1);
	}

	for (int i = 1; i < n; i++) {
		Bev& bev = bevs[i];
		for (int j = 1; j < k + max_v; j++) {
			Stat o1 = DP[i - 1][j];
			if (j - bev.v < 0) {
				DP[i][j] = o1;
				continue;
			}
			
			Stat o2 = DP[i][j - bev.v];
			o2.cost += bev.c;
			Stat o3 = DP[i - 1][j - bev.v];
			o3.cost += bev.c;
			o3.count++;
		
			DP[i][j] = min(o1, o2);
			DP[i][j] = min(DP[i][j], o3);
		}
	}
	
	Stat ans = DP[n - 1][k];
	for (int j = k + 1; j < k + max_v; j++) {
		ans = min(ans, DP[n - 1][j]);
	}

	cout << ans.cost << " " << ans.count << endl;
}

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
