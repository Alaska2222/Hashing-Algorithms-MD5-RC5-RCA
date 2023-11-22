#include "PseudoRandGenerator.h"

PseudoRandGenerator::PseudoRandGenerator(int a, int c, int m, int x0)
    : a_(a), c_(c), m_(m), x0_(x0) {}

pair<vector<int>, int> PseudoRandGenerator::GenerateRandSequence(const string& n_str) {
    int n = stoi(n_str);
    int x_n = (a_ * x0_ + c_) % m_;
    vector<int> sequence = { x_n };

    for (int i = 0; i < n; ++i) {
        int x_n1 = (a_ * x_n + c_) % m_;
        sequence.push_back(x_n1);
        x_n = x_n1;
    }

    int power = set<int>(sequence.begin(), sequence.end()).size();
    return make_pair(sequence, power);

}


