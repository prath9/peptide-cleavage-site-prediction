#pragma once
#include <vector>
#include "sequence.hpp"

class Predictor {
public:
    static const int p = 13;
    static const int q = 2;
    void train(Sequence* training_set, int training_set_size);
    virtual double WindowScore(Sequence& s, int window_position) = 0;
    int FindCleavageMax(Sequence& s);
    std::vector<int> FindCleavagesThreshold(Sequence& s, double threshold);
    double SequenceAccuracy(Sequence* test_set, int test_set_size);
};