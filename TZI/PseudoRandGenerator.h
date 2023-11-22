#ifndef PSEUDO_RANDOM_GENERATOR_H
#define PSEUDO_RANDOM_GENERATOR_H

#include <vector>
#include <set>
#include<string>

using namespace std;
class PseudoRandGenerator {
public:
    PseudoRandGenerator(int a, int c, int m, int x0);
    pair<vector<int>, int> GenerateRandSequence(const string& n_str);

private:
    int a_;
    int c_;
    int m_;
    int x0_;
};

#endif 
