#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

struct Jedi {
	long a, b;
	
	bool operator<(const Jedi& o) const {
		return b < o.b;
	}
};

typedef vector<Jedi> JV;

void printj(const Jedi& j) {
	cerr << "a " << j.a << " b " << j.b << endl;
}

void printvj(const JV& jv) {
	cerr << jv.size() << " jedis:" << endl;
	for (const auto& j : jv) {
		printj(j);
	}
}

bool intersect(const Jedi& j, const Jedi& o) {
	if (j.a <= j.b) {
		bool a_in = j.a <= o.a && o.a <= j.b;
		bool b_in = j.a <= o.b && o.b <= j.b;
		return a_in || b_in;
	} else {
		bool a_in = j.a <= o.a || o.a <= j.b;
		bool b_in = j.a <= o.b || o.b <= j.b;
		return a_in || b_in;
	}
}

int find_score(const Jedi& ref, const int m, const JV& alljedis) {
	JV jedis;

	for (Jedi j : alljedis) {
		if (intersect(ref, j)) {
			continue;
		}
		if (j.a < ref.b) {
			j.a += m;
		}
		if (j.b < ref.b) {
			j.b += m;
		}
		if (j.a > j.b) {
			j.b += m;
		}
		assert(j.a != ref.b);
		assert(j.b != ref.b);
		j.a -= ref.b;
		j.b -= ref.b;
		jedis.push_back(j);
	}

	sort(jedis.begin(), jedis.end());

	int count = 0;
	long last_used = 0;
	for (Jedi& j : jedis) {
		if (last_used < j.a) {
			last_used = j.b;
			count++;
		}
	}
	
	return count;
}

struct Event {
	long x;
	bool add;
	bool operator<(const Event& o) const {
		return x < o.x;
	}
};

void testcase() {
	int n;
	long m;
	cin >> n >> m;
	
	JV jedis;
	vector<Event> events;
	for (int i = 0; i < n; i++) {
		long a, b;
		cin >> a >> b;
		jedis.push_back({a, b});
		events.push_back({a, true});
		//TODO[gleb] this causes a bug. Think about how i can fix it.
		events.push_back({b, false});
	}
	
	sort(events.begin(), events.end());
	int count = 0;
	int i = 0;
	int min_x = -1;
	int min_count = -1;
	while (i < events.size()) {
		long x = events[i].x;
		int plus = 0;
		int minus = 0;
		while (i < events.size() && events[i].x == x) {
			if (events[i].add) {
				plus++;
			} else {
				minus++;
			}
			i++;
		}
		count += plus;
		if (min_x == -1 || count < min_count) {
			min_x = x;
			min_count = count; 
		}
		count -= minus;
	}
	assert(min_x > 0);
	//cerr << "min x " << min_x << " min count " << min_count << endl; 

	Jedi ref {min_x, min_x};
	JV intersecting;
	for (Jedi& j : jedis) {
		if (intersect(ref, j)) {
			intersecting.push_back(j);
		}
	}
	assert(intersecting.size() <= 10);
	//cerr << "intersecting" << endl;
	//printvj(intersecting);

	int max_count = find_score(ref, m, jedis);
	for (Jedi& j : intersecting) {
		int score = find_score(j, m, jedis);
		//cerr << "starting with jedi: " << endl;
		//printj(j);
		//cerr << "score: " << score << endl;
		max_count = max(max_count, 1 + score);
	}
	cout << max_count << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	int t;
	cin >> t;
	while (t) {
		t--;
		testcase();
	}
	return 0;
}
