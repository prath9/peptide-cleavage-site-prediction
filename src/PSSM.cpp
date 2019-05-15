#include <cassert>
#include <iostream>
#include "PSSM.hpp"

PSSM::PSSM() {
    for (int aa = 0; aa < ALPHABET_SIZE; aa++) {
        for (int window_index = 0; window_index < p + q; window_index++) {
            pssm[aa][window_index] = 0;
        }
    }
}

void PSSM::train(Sequence* training_set, int training_set_size, double pseudocount) {
    double freq_array[ALPHABET_SIZE][p + q] = {{0}};
    double general_freq_array[ALPHABET_SIZE] = {0};
    // Compute the array of the N(a,window_index)
    int cleavage_site;
    int absolute_index;  // index in the sequences corresponding to window_index in the window

    // First count the occurrences
    for (int sequence_index = 0; sequence_index < training_set_size; sequence_index++) {
        Sequence seq = training_set[sequence_index];
        cleavage_site = seq.get_cleavage_site();

        for (int window_index = 0; window_index < p + q; window_index++) {  // relative position in the window
            absolute_index = window_index + cleavage_site - p;
            // p must be small enough for the first amino acid (usually methionine) not to be in the window:
            assert(absolute_index >= 1);
            // q must also be small enough for the absolute index to be in the given sequence:
            assert(absolute_index < seq.get_length());
            general_freq_array[seq.get_aa_sequence()[absolute_index] - ASCII_CONSTANT]++;
            freq_array[seq.get_aa_sequence()[absolute_index] - ASCII_CONSTANT][window_index]++;
        }
    }

    // Then get the corresponding frequencies, applying the pseudocount
    int total_num_aa = (p + q) * training_set_size;
    for (int aa = 0; aa < ALPHABET_SIZE; aa++) {
        general_freq_array[aa]  = (general_freq_array[aa] + pseudocount)/(total_num_aa + pseudocount * NUM_AA); // for letters that are not aa, there is a residual frequency
        for (int window_index = 0; window_index < p + q; window_index++) {
            freq_array[aa][window_index] = (freq_array[aa][window_index] + pseudocount)/(training_set_size + pseudocount * NUM_AA);
        }
    }
    
    // Compute the PSSM
    for (int aa = 0; aa < ALPHABET_SIZE; aa++) {
        for (int window_index = 0; window_index < p + q; window_index++) {
            pssm[aa][window_index] = log(freq_array[aa][window_index]) - log(general_freq_array[aa]);
        }
    }
}

double PSSM::WindowScore(Sequence s, int potential_cleavage_site) {
    // potential_cleavage_site must be greater than or equal to p
    double word_score = 0;
    for (int window_index = 0; window_index < p + q; window_index++) {
        word_score += pssm[s.get_aa_sequence()[potential_cleavage_site + window_index - p] - ASCII_CONSTANT][window_index];
    }
    return word_score;
}