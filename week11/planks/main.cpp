#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<bool>> Splits;
typedef long long ll;

void print_bvec(vector<bool>& vec) {
	for (auto elem : vec) {
		cerr << elem << " ";
	}
	cerr << endl;
}

ll split_count(int index, int cursum, int target, vector<int>& numbers) {
	if (index == numbers.size()) {
		return cursum == target;
	}

	ll ret = split_count(index + 1, cursum, target, numbers);
	if (cursum + numbers[index] <= target) {
		ret += split_count(index + 1, cursum + numbers[index], target, numbers);
	}
	return ret;
}

int process_split(vector<bool>& splitv, vector<int>& numbers, int sum) {
	std::vector<int> first, second;
	for (int i = 0; i < numbers.size(); i++) {
		if (splitv[i]) {
			first.push_back(numbers[i]);
		} else {
			second.push_back(numbers[i]);
		}
	}
	
	ll first_count = split_count(0, 0, sum / 4, first);
	if (first_count == 0) {
		return 0;
	}
	ll second_count = split_count(0, 0, sum / 4, second);
	return first_count * second_count;
}

ll rec(int index, int cursum, vector<bool>& cur, int target, vector<int>& numbers) {
	if (index == numbers.size()) {
		if (cursum == target && cur[0] == false) {
			return process_split(cur, numbers, target * 2);
		} else {
			return 0;
		}
	}

	ll ret = rec(index + 1, cursum, cur, target, numbers);
	if (cursum + numbers[index] <= target) {
		cur[index] = true;
		ret += rec(index + 1, cursum + numbers[index], cur, target, numbers);
		cur[index] = false;
	}
	return ret;
}

void testcase() {
	int n;
	std::cin >> n;
	vector<int> numbers;
	int sum = 0;
	for (int i = 0; i < n; i++) {
		int l;
		std::cin >> l;
		numbers.push_back(l);
		sum += l;
	}
	
	if (sum % 4 != 0) {
		//std::cerr << "sum not divisible by 4" << std::endl; 
		cout << 0 << std::endl;
		return;
	}
	
	vector<bool> cur(n);
	ll ans = rec(0, 0, cur, sum / 2, numbers);
	cout << ans / 12 << std::endl;
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		//cerr << "testcase " << i << std::endl;
		testcase();
	}
	return 0;
}
