#include <iostream>
#include "PSSM.hpp"

PSSM::PSSM() {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        for (int j = 0; j < p + q; j++) {
            pssm[i][j] = 0;
        }
    }
}

void PSSM::train(Sequence* training_set, int training_set_size, double pseudocount) {
    double freq_array[ALPHABET_SIZE][p + q] = {{0}};
    double general_freq_array[ALPHABET_SIZE] = {0};
    // Compute the array of the N(a,i)
    int total_num_aa = 0;

    // First count the occurrences
    for (int sequence_index = 0; sequence_index < training_set_size; sequence_index++) {
        Sequence seq = training_set[sequence_index];
        int cleavage_site = seq.get_cleavage_site();

        for (int i = 1; i < seq.get_length(); i++) {  // ignore the first aa (usually methionine)
            general_freq_array[seq.get_aa_sequence()[i] - ASCII_CONSTANT]++;
            total_num_aa += seq.get_length();
            if (cleavage_site - p <= i && i < cleavage_site + q) {
                freq_array[seq.get_aa_sequence()[i] - ASCII_CONSTANT][i + p - cleavage_site]++;
            }
        }
    }

    // Then get the corresponding frequencies, applying the pseudocount
    for (int aa = 0; aa < ALPHABET_SIZE; aa++) {
        general_freq_array[aa]  = (general_freq_array[aa] + pseudocount)/(total_num_aa + pseudocount * NUM_AA); // for letters that are not aa, there is a residual frequency
        for (int i = 0; i < p + q; i++) {
            freq_array[aa][i] = (freq_array[aa][i] + pseudocount)/(training_set_size + pseudocount * NUM_AA);
        }
    }
    
    // Compute the PSSM
    for (int aa = 0; aa < ALPHABET_SIZE; aa++) {
        for (int i = 0; i < p + q; i++) {
            pssm[aa][i] = log(freq_array[aa][i]) - log(general_freq_array[aa]);
        }
    }
}

double PSSM::WindowScore(Sequence s, int window_position) {
    // window_position must be greater than or equal to p
    double word_score = 0;
    for (int i = 0; i < p + q; i++) {
        word_score += pssm[s.get_aa_sequence()[window_position + i - p] - ASCII_CONSTANT][i];
    }
    return word_score;
}

bool PSSM::WindowIsCleavage(Sequence s, int window_position, double threshold) {
    double word_score = this->WindowScore(s, window_position);
    return (word_score > threshold) ? true : false;
}