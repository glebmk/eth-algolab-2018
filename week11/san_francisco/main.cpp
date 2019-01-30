#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef long long ll;
typedef map<int, ll> Map;

Map make_step(int step, Map& v2s, vector<vector<int>>& G, vector<vector<ll>>& W, vector<ll>& step2score, vector<int>& v2discovery, vector<ll>& v2score, int target_k) {
	Map new_v2s;

	for (auto& entry : v2s) {
		int vertex = entry.first;
		ll cur_score = entry.second;

		if (v2discovery[vertex] != -1) {
			int discovery = v2discovery[vertex];
			int cycle_length = step - discovery;
			ll cycle_weight = cur_score - v2score[vertex];
			
			int i = 1;
			while (discovery + cycle_length * i <=  target_k) {
				step2score[discovery + cycle_length * i] = \
				max(step2score[discovery + cycle_length * i], v2score[vertex] + cycle_weight * i);
				i++;
			}
			continue;
		}
	
		v2discovery[vertex] = step;
		v2score[vertex] = cur_score;
		step2score[step] = max(cur_score, step2score[step]);

		vector<int>& outs = G[vertex];
		assert(outs.size() > 0);
		vector<ll>& weights = W[vertex];

// TODO[gleb] the problem is in how vertexes without outgoing are processed.		
		
		for (int i = 0; i < outs.size(); i++) {
			int out = outs[i];
			ll weight = weights[i];

			if (G[out].size() == 0) {
				out = 0;
			}

			auto search = new_v2s.find(out);
			if (search == new_v2s.end()) {
				new_v2s.insert(pair<int, ll>(out, cur_score + weight)); 
			} else {
				search->second = max(cur_score + weight, search->second);
			}
		}
	}

	return new_v2s;
}

void testcase() {
	int n, m, k;
	ll x;
	cin >> n >> m >> x >> k;

	vector<vector<int>> G(n);
	vector<vector<ll>> W(n);

	for (int i = 0; i < m; i++) {
		int u, v;
		ll p;
		cin >> u >> v >> p;
		G[u].push_back(v);
		W[u].push_back(p);
	}

	Map v2s;
	v2s.insert(pair<int, ll>(0, 0));
	vector<ll> step2score(k + 1);
	vector<int> v2discovery(n, -1);
	vector<ll> v2score(n);

	for (int i = 0; i <= k; i++) {
		v2s = make_step(i, v2s, G, W, step2score, v2discovery, v2score, k);
	}

	for (int i = 0; i < n; i++) {
		cerr << "i=" << i << " discovery=" << v2discovery[i] << " score=" << v2score[i] << endl;
	}

	cerr << "step2score: ";	
	for (int i = 0; i <= k; i++) {
		cerr << step2score[i] << " ";
	}
	cerr << endl;

	for (int i = 1; i <= k; i++) {
		if (step2score[i] >= x) {
			cout << i << endl;
			return;
		}
	}

	cout << "Impossible" << endl;
}

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
