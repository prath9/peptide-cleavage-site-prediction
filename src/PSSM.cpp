#include "PSSM.hpp"

void PSSM::train(Sequence* training_set, int training_set_size) {
    // Compute the array of the N(a,i)
    int total_num_aa = 0;

    // First count the occurrences
    for (int sequence_index = 0; sequence_index < training_set_size; sequence_index++) {
        Sequence seq = training_set[sequence_index];
        int cleavage_site = seq.get_cleavage_site();

        for (int i = 1; i < seq.get_length(); i++) {  // ignore the first aa (usually methionine)
            general_freq_array[seq.get_aa_sequence()[i] - ASCII_CONSTANT]++;
            total_num_aa += seq.get_length();
            if (cleavage_site - p <= i < cleavage_site + q) {
                freq_array[seq.get_aa_sequence()[i] - ASCII_CONSTANT][i + p - cleavage_site]++;
            }
        }
    }

    // Then get the corresponding frequencies
    for (int aa = 0; aa < ALPHABET_SIZE; aa++) {
        general_freq_array[aa] /= total_num_aa;
        for (int i = 0; i < p + q; i++) {
            freq_array[aa][i] /= training_set_size;
        }
    }
}

double PSSM::WindowScore(Sequence s, int window_position) {
    
}