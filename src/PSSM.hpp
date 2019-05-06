#pragma once

#include "predictor.hpp"

#define ALPHABET_SIZE 26
#define ASCII_CONSTANT 65

class PSSM : public Predictor {
public:
    double freq_array[ALPHABET_SIZE][p + q] = {0};
    double general_freq_array[ALPHABET_SIZE] = {0};
    void train(Sequence* training_set, int training_set_size);
    double WindowScore(Sequence s, int window_position);
    bool WindowIsCleavage(Sequence s, int window_position, double threshold);
    int FindCleavage(Sequence s);
};