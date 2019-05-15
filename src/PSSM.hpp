#pragma once

#include <cmath>

#include "predictor.hpp"

#define ALPHABET_SIZE 26
#define ASCII_CONSTANT 65
#define NUM_AA 20

class PSSM : public Predictor {
public:
    double pssm[ALPHABET_SIZE][p + q];

    PSSM();
    void train(Sequence* training_set, int training_set_size, double pseudocount);
    double WindowScore(Sequence s, int window_position);
    int FindCleavage(Sequence s);
};