#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef vector<bool> V;
typedef vector<V> Table;

// True stands for free, False stands for taken.

void update_map(int index, int count, V& first, V& second, map<V, int>& state2count) {
	int w = first.size();
	if (index >= w - 1) {
		auto search = state2count.find(second);
		if (search == state2count.end()) {
			state2count.insert(pair<V, int>(second, count));
		} else {
			search->second = max(count, search->second);
		}
		return;
	}

	// Don't position tile on the current index.
	update_map(index + 1, count, first, second, state2count);

	// If it is possible to put the tile on this spot - just return.
	if (!first[index] || !first[index + 1] || !second[index] || !second[index + 1]) {
		return;
	}

	second[index] = false;
	second[index + 1] = false;
	update_map(index + 2, count + 1, first, second, state2count);
	second[index] = true;
	second[index + 1] = true;
}

map<V, int> make_step(int row, map<V, int>& first_state2count, Table& floor) {
	V second = floor[row + 1];
	map<V, int> second_state2count;
	for (auto& entry : first_state2count) {
		V first = entry.first;
		int count = entry.second;
		update_map(0, count, first, second, second_state2count); 
	}
	return second_state2count;
}

void testcase() {
	int h, w;
	cin >> h >> w;
	
	Table floor;
	for (int i = 0; i < h; i++) {
		V row;
		for (int j = 0; j < w; j++) {
			char c;
			cin >> c;
			if (c == '1') {
				row.push_back(true);
			} else {
				row.push_back(false);
			}
		}
		floor.push_back(row);
	}

	map<V, int> state2count;
	V start = floor[0];
	state2count.insert(pair<V, int>(start, 0));

	for (int i = 0; i < h - 1; i++) {
		state2count = make_step(i, state2count, floor); 
	}

	int max_tiles = 0;
	for (auto& entry : state2count) {
		max_tiles = max(entry.second, max_tiles);
	}
	cout << max_tiles << endl;
}

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}



