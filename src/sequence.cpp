#include <string>

#include "sequence.hpp"

Sequence::Sequence(std::string _aa_sequence, int _cleavage_site, int _length) {
    length = _length;
    aa_sequence = _aa_sequence;
    cleavage_site = _cleavage_site;
}

int Sequence::get_length() {
    return length;
}

int Sequence::get_cleavage_site() {
    return cleavage_site;
}

std::string Sequence::get_aa_sequence() {
    return aa_sequence;
}
