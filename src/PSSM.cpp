#include "PSSM.hpp"
#define num_aa 20
void Predictor::train(Sequence* training_set, int training_set_size) {
    // Compute the array of the N(a,i)
    double** freq_array = new double[num_aa][p+q] ();
    double* general_freq_array = new double[num_aa];
    int total_num_aa = 0;
    for (int sequence = 0; sequence < training_set_size; sequence++) {
        for (int i = 0; i < training_set[sequence].get_length(); i++) {
            general_freq_array[] += 1;
            total_num_aa += training_set[sequence].get_length();
        }
    }
    for (int i = 0; i < num_aa; i++) {
        general_freq_array[i]/= total_num_aa;
    }
}