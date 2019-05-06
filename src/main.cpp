#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include "sequence.hpp"
#include "train_test_split.hpp"
#include "main.hpp"


Sequence** FileToSequenceArray(std::string filename, int size) {
  std::ifstream inputfile;
  inputfile.open(filename);
  std::string line;
  Sequence** sequence_array = new Sequence*[size];
  std::string aa_sequence;
  int length;  // length of each sequence
  int cleavage_site;  // position of the cleavage site of each sequence
  for (int sequence_index = 0; sequence_index < size; sequence_index++) {
    std::getline(inputfile, line); // first line: do nothing
    std::getline(inputfile, line); // second line: retrieve aa_sequence
    aa_sequence = line;
    std::getline(inputfile, line); // third line: compute cleavage site and length
    length = aa_sequence.length();  // number of bytes (and characters, here) in aa_sequence
    int index = 0;
    while (line[index] != 'C') {
      index++;
    }
    cleavage_site = index;
    sequence_array[sequence_index] = new Sequence(aa_sequence, cleavage_site, length);
    // std::cout << sequence_array[sequence_index]->get_aa_sequence() << std::endl;
  }
  return sequence_array;
}




int main() {
  std::string FILENAME = "../data/EUKSIG_13.red";
  int size = GetSize(FILENAME);
  int test_size = size / 5;
  int train_size = size - test_size;
  CreateTrainTestSets(FILENAME);
  Sequence** train_sequences = FileToSequenceArray(train_filename, size);
  Sequence** test_sequences = FileToSequenceArray(test_filename, size);

  for (int i = 0; i < test_size; i++) {
    std::cout << test_sequences[i]->get_aa_sequence() << std::endl;
  }
}