#pragma once

class Sequence {
    int length;
    int cleavage_site;
    char* aa_sequence;

    public:
        Sequence(char* _aa_sequence, int _cleavage_site, int _length);
        int get_length();
        char* get_aa_sequence();
};