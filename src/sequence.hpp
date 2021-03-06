#pragma once

#include <string>

class Sequence {
    int length;
    int cleavage_site;
    std::string aa_sequence;

    public:
        Sequence();
        Sequence(std::string _aa_sequence, int _cleavage_site, int _length);
        int get_length();
        int get_cleavage_site();
        std::string get_aa_sequence();
};