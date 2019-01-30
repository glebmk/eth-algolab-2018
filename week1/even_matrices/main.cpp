#include <iostream>
#include <vector>

using namespace std;

void cout_line(vector<int>& line) {
	for (int elem : line) {
		cout << elem << " ";
	}
	cout << endl;
}

void cout_matrix(vector<vector<int>>& mat) {
	cout << "matrix: " << endl;
	for (vector<int> line : mat) {
		cout_line(line);
	}
}

vector<vector<int>> read_mat(int n) {
	vector<vector<int>> mat(n, vector<int>(n, -1));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int num = -1;
			cin >> num;
			mat[i][j] = num;
		}
	}

	return mat;
}

vector<vector<vector<int>>> precompute_columns(vector<vector<int>>& mat) {

}

void testcase() {
	int n = 0;
	cin >> n;

	vector<vector<int>> mat = read_mat(n);

	int count_ = 0;


	cout << count_ << endl;
}

int main() {
	ios_base::sync_with_stdio(false);

	int t = 0;
	cin >> t;

	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;
}
