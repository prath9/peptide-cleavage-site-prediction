#pragma once

class Sequence {
    int length;
    int cleavage_site;
    std::string aa_sequence;

    public:
        Sequence(std::string _aa_sequence, int _cleavage_site, int _length);
        int get_length();
        std::string get_aa_sequence();
};