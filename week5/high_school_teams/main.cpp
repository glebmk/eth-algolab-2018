#include <map>
#include <iostream>
#include <vector>
#include <tuple>

typedef long long ll;

typedef std::map<ll, std::vector<ll>> Map;

void rec(int index, int last_index, ll red, ll blue, int k, std::vector<ll>& players, Map& map, int max_k) {
	if (index == last_index) {
		ll key = red - blue;
		if (map.count(key) == 0) {
			map.insert(std::pair<ll, std::vector<ll>>(key, std::vector<ll>(max_k + 1)));
		}
		std::vector<ll>& k2count = map.find(key)->second;
		k2count[max_k - k] += 1;
		return;
	}

	if (k > 0) {
		rec(index + 1, last_index, red, blue, k - 1, players, map, max_k);
	}
	rec(index + 1, last_index, red + players[index], blue, k, players, map, max_k);
	rec(index + 1, last_index, red, blue + players[index], k, players, map, max_k);
}

void print_vector(std::vector<ll>& vec) {
	for (auto elem : vec) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}

void print_map(Map map) {
	std::cout << "--------------------------" << std::endl;
	std::cout << "printing map" << std::endl;
	for (auto& elem : map) {
		std::cout << "red - blue" << elem.first << std::endl;
		std::cout << "k2count:" << std::endl;
		print_vector(elem.second);
	}
}

ll count_for_vectors(std::vector<ll>& count1, std::vector<ll>& count2, int max_k) {
	ll options = 0;
	for (int i = 0; i <= max_k; i++) {
		for (int j = 0; j <= max_k; j++) {
			if (i + j <= max_k) {
				options += count1[i] * count2[j];
			}
		}
	}
	return options;		
}

long count(ll key, Map& map1, Map& map2, int max_k) {
	if (map2.count(-key) == 0) {
		return 0;
	}
	std::vector<ll>& count1 = map1.find(key)->second;
	std::vector<ll>& count2 = map2.find(-key)->second;
	return count_for_vectors(count1, count2, max_k);
}

void testcase() {
	int n, k;
	std::cin >> n >> k;
	std::vector<ll> players;

	for (int i = 0; i < n; i++) {
		int level;
		std::cin >> level;
		players.push_back(level);
	}

	Map map1; 
	rec(0, n/2, 0, 0, k, players, map1, k);
	Map map2;
	rec(n/2, n, 0, 0, k, players, map2, k);
	
	// std::cout << "n/2 " << n/2 << std::endl;
	// print_map(map1);
	// print_map(map2);
	
	ll options = 0;
	for (auto const& x : map1) {
		options += count(x.first, map1, map2, k);
	}
	std::cout << options << std::endl;
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
