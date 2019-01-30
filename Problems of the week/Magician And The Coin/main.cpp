#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

double recursive(int day, int wealth, vector<double>& probs, vector<vector<double>>& memo, int total_days, int target_wealth) {
	if (wealth >= target_wealth) {
		return 1;
	}
	if (day >= total_days) {
		return 0;
	}
	if (memo[day][wealth] != -1) {
		return memo[day][wealth];
	}
	for (int bet = 0; bet <= wealth; bet++) {
		double lose = recursive(day + 1, wealth - bet, probs, memo, total_days, target_wealth);
		double win = recursive(day + 1, wealth + bet, probs, memo, total_days, target_wealth);
		double prob = win * probs[day] + lose * (1 - probs[day]);
		memo[day][wealth] = max(prob, memo[day][wealth]);
	}
	return memo[day][wealth];
}

void cout_vector(vector<double>& v) {
	for (double elem : v) {
		cout << elem << " ";
	}
	cout << endl;
}

void testcase() {
	int total_days, start_wealth, target_wealth;
	cin >> total_days;
	cin >> start_wealth;
	cin >> target_wealth;

	vector<double> probs(total_days);
	for (int day = 0; day < total_days; day++) {
		double prob;
		cin >> prob;
		probs[day] = prob;
	}

	vector<vector<double>> memo(total_days, vector<double>(target_wealth, -1));
	double ans = recursive(0, start_wealth, probs, memo, total_days, target_wealth);

	cout << std::fixed << std::setprecision(5);
	cout << ans << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
