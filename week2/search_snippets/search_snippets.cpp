#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

struct Occurrence {
    int position;
    int word;

    Occurrence(int position, int word):position(position), word(word) {}
};

vector<vector<int>> read_input() {
    int number_of_words;
    cin >> number_of_words;
    vector<unsigned int> number_of_occurrences_per_word;
    number_of_occurrences_per_word.reserve(number_of_words);
    for (int i = 0; i < number_of_words; i++) {
        unsigned int m;
        cin >> m;
        number_of_occurrences_per_word.push_back(m);
    }

    vector<vector<int>> input;
    input.reserve(number_of_words);
    for (int number_of_occurrences : number_of_occurrences_per_word) {
        vector<int> occurrences(number_of_occurrences);
        for (int i = 0; i < number_of_occurrences; i++) {
            int p;
            cin >> p;
            occurrences[i] = p;
        }
        input.push_back(occurrences);
    }
    return input;
}

/** Compare function for min heap. */
bool compare(const Occurrence& left, const Occurrence& right)
{
    return left.position > right.position;
}


void push(const Occurrence& elem, vector<Occurrence>& heap) {
    heap.push_back(elem);
    push_heap(heap.begin(), heap.end(), compare);
}

Occurrence pop(vector<Occurrence>& heap) {
    pop_heap(heap.begin(), heap.end(), compare);
    Occurrence& popped = heap.back();
    heap.pop_back();
    return popped;
}

int compute_answer(vector<vector<int>>& input) {
    auto n = input.size();
    vector<int> word2index(n, 0);
    vector<Occurrence> min_heap;
    vector<Occurrence> max_heap;

    int current_max = input[0][0];
    for (int word = 0; word < n; word++) {
        Occurrence occurrence(input[word][0], word);
        push(occurrence, min_heap);
        current_max = max(current_max, occurrence.position);
    }

    int ans = current_max - min_heap.front().position + 1;

    while (true) {
        Occurrence occurrence = pop(min_heap);
        int word = occurrence.word;
        word2index[word]++;
        if (word2index[word] == input[word].size()) {
            return ans;
        }
        Occurrence new_occurrence(input[word][word2index[word]], word);
        push(new_occurrence, min_heap);
        current_max = max(current_max, new_occurrence.position);
        ans = min(current_max - min_heap.front().position + 1, ans);
    }
}

void testcase() {
    vector<vector<int>> input = read_input();
    int ans = compute_answer(input);
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