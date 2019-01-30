#include <iostream>
#include <vector>
#include <algorithm>

class Boat {
  public:
	int l, p;
  	Boat(int length, int position) : l(length), p(position) {}
};

bool compare_boats(const Boat& left, const Boat& right) {
	return (left.p < right.p);
}

int compute_ending(int last_ending, int i, std::vector<Boat>& boats) {
	return std::max(last_ending, boats[i].p - boats[i].l) + boats[i].l;
}

void testcase() {
	int n = 0;
	std::cin >> n;
	std::vector<Boat> boats;
	
	for (int i = 0; i < n; i++) {
		int l, p;
		std::cin >> l >> p;
		boats.push_back(Boat(l, p));
	}

	std::sort(boats.begin(), boats.end(), compare_boats);
    int count = 0;	
	int last_ending = - 2 * (boats[0].p + boats[0].l);
	
	for (int i = 0; i < boats.size(); i++) {
		if (boats[i].p < last_ending) {
			continue;
		}

		int ending = compute_ending(last_ending, i, boats);
		bool use_boat = true;
		for (int j = i + 1; j < boats.size(); j++) {
			if (boats[j].p >= ending) { 
				break;
			}
			int new_ending = compute_ending(last_ending, j, boats);
			if (new_ending < ending) {
				use_boat = false;
				break;
			}
		}
		if (use_boat) {
			count++;
			last_ending = ending;
		}
	}


	std::cout << count << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t = 0;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
