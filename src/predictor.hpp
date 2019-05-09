#pragma once
#include "sequence.hpp"

class Predictor {
public:
    static const int p = 3;
    static const int q = 2;
    void train(Sequence* training_set, int training_set_size);
    double WindowScore(Sequence s, int window_position);
    bool WindowIsCleavage(Sequence s, int window_position, double threshold);
    int FindCleavage(Sequence s);
};