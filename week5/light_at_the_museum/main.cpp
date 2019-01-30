#include <iostream>
#include <vector>
#include <map>

struct OnOff {
	int on;
	int off;
};
typedef std::vector<OnOff> Switch;
typedef std::vector<Switch> Switches;
typedef std::vector<int> State;
typedef std::map<State, int> Map;

void rec(int index, int num_switches, State& state, Switches& switches, Map& map) {
	int n = switches.size();
	int m = state.size();

	if (index == n) {
		auto search = map.find(state);
		if (search != map.end()) {
			search->second = std::min(search->second, num_switches);
		} else {
			map.insert(std::pair<State, int>(state, num_switches));
		}
		return;
	}
	
	rec(index + 1, num_switches, state, switches, map);
	
	Switch& sw = switches[index];
	for (int i = 0; i < m; i++) {
		state[i] += sw[i].off - sw[i].on;
	}
	rec(index + 1, num_switches + 1, state, switches, map);
	for (int i = 0; i < m; i++) {
		state[i] += sw[i].on - sw[i].off;
	}
}

State create_initial_state(Switches& switches, int m) {
	State initial_state(m);
	for (Switch& sw : switches) {
		for (int i = 0; i < m; i++) {
			initial_state[i] += sw[i].on; 
		}
	}
	return initial_state;
}

Map create_map(Switches& switches, int m) {
	State initial_state = create_initial_state(switches, m);

	Map map;
	rec(0, 0, initial_state, switches, map);
	return map;
}

std::vector<Switch> read_switches(int n, int m) {
	std::vector<Switch> switches;
	for (int i = 0; i < n; i++) {
		Switch sw;
		for (int j = 0; j < m; j++) {
			int on, off;
			std::cin >> on >> off;
			OnOff on_off { on, off };
			sw.push_back(on_off);
		}
		switches.push_back(sw);
	}
	return switches;
}

State find_state_diff(State& target, State& init) {
	int m = target.size();
	std::vector<int> diff(m);
	
	for (int i = 0; i < m; i++) {
		diff[i] = target[i] - init[i];
	}
	return diff;
}

int count_rec(int index, int num_switches, State& state, Switches& switches, Map& map, State& target, int n) {
	int m = state.size();

	if (index == switches.size()) {
		State diff = find_state_diff(target, state);
		auto search = map.find(diff);
		if (search != map.end()) {
			return search->second + num_switches;
		} else {
			return n + 1;
		}
	}

	int ans = count_rec(index + 1, num_switches, state, switches, map, target, n);
	Switch& sw = switches[index];
	for (int i = 0; i < m; i++) {
		state[i] += sw[i].off - sw[i].on;
	}
	ans = std::min(ans, count_rec(index + 1, num_switches + 1, state, switches, map, target, n));
	for (int i = 0; i < m; i++) {
		state[i] += sw[i].on - sw[i].off;
	}
	return ans;
}

int number_of_switches(std::vector<Switch>& switches, Map& map, State& target, int m, int n) {
	State initial_state = create_initial_state(switches, m);
	return count_rec(0, 0, initial_state, switches, map, target, n);	
}

State read_target(int m) {
	State target;
	for (int i = 0; i < m; i++) {
		int t;
		std::cin >> t;
		target.push_back(t);
	}
	return target;
}

void testcase() {
	int n, m;
	std::cin >> n >> m;

	State target = read_target(m);

	std::vector<Switch> first = read_switches(n/2, m);
	std::vector<Switch> second = read_switches(n - n/2, m);
	Map map = create_map(second, m);
	int switches = number_of_switches(first, map, target, m, n);
	if (switches <= n) {
		std::cout << switches << std::endl;
	} else {
		std::cout << "impossible" << std::endl;
	}
}

int main() {
	int t;
	std::cin >> t;
	for (int i = 0; i < t; i++) {
		testcase();
	}
	return 0;
}
