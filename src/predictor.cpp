#include "predictor.hpp"


int Predictor::FindCleavageMax(Sequence& s) {
    // Return the potential cleavage site with maximum score
    double score = WindowScore(s, p);
    double max_score = score;  // to be computed in the following loop
    int max_score_pos = p;  // idem
    int length = s.get_length();
    for (int potential_cs = p + 1; potential_cs <= length - q; potential_cs++) {
        score = WindowScore(s, potential_cs);
        if (score > max_score) {
            max_score = score;
            max_score_pos = potential_cs;
        }
    }
    return max_score_pos;
}


std::vector<int> Predictor::FindCleavagesThreshold(Sequence& s, double threshold) {
    std::vector<int> v;
    int length = s.get_length();
    for (int potential_cs = p; potential_cs <= length - q; potential_cs++) {
        if (WindowScore(s, potential_cs) >= threshold)
            v.push_back(potential_cs);
    }
    return v;
}

double Predictor::SequenceAccuracy(Sequence* test_set, int test_set_size) {
    int successes = 0.;
    for (int sequence_index = 0; sequence_index < test_set_size; sequence_index++)
    {
        Sequence current_sequence = test_set[sequence_index];
        if (FindCleavageMax(current_sequence) == current_sequence.get_cleavage_site()) {
            successes++;
        }
    }
    return (double) successes / test_set_size;
}