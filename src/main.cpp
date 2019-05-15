#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include "predictor.hpp"
#include "PSSM.hpp"
#include "sequence.hpp"
#include "train_test_split.hpp"
#include "main.hpp"


Sequence* FileToSequenceArray(std::string filename, int size) {
  std::ifstream inputfile;
  inputfile.open(filename);
  std::string line;
  Sequence* sequence_array = new Sequence[size];
  std::string aa_sequence;
  int length;  // length of each sequence
  int cleavage_site;  // position of the cleavage site of each sequence
  for (int sequence_index = 0; sequence_index < size; sequence_index++) {
    std::getline(inputfile, line); // first line: do nothing
    std::getline(inputfile, aa_sequence); // second line: retrieve aa_sequence
    length = aa_sequence.length();  // number of bytes (and characters, here) in aa_sequence
    std::getline(inputfile, line); // third line: compute cleavage site
    int index = 0;
    while (line[index] != 'C') {
      index++;
    }
    cleavage_site = index;
    Sequence a(aa_sequence, cleavage_site, length);
    sequence_array[sequence_index] = a;
  }
  return sequence_array;
}

template <typename T, int rows, int cols> void printMatrix(T (&mat)[rows][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols - 1; j++) {
      printf("%g ", mat[i][j]);
    }
    printf("%g\n", mat[i][cols - 1]);
  }
}



int main() {
  std::string FILENAME = "../data/EUKSIG_13.red";
  int size = GetSize(FILENAME);
  int test_size = size / 5;
  int train_size = size - test_size;
  CreateTrainTestSets(FILENAME);
  Sequence* train_sequences = FileToSequenceArray(train_filename, train_size);
  //Sequence* test_sequences = FileToSequenceArray(test_filename, test_size);

  // Test PSSM
  PSSM pssm;
  pssm.train(train_sequences, train_size, 1.);
  printMatrix(pssm.pssm);

  Sequence seq = train_sequences[1];
  int length = seq.get_length();
  std::cout << seq.get_aa_sequence() << std::endl;
  for (int potential_cleavage_site = Predictor::p; potential_cleavage_site <= length - Predictor::q; potential_cleavage_site++) {
    double score = pssm.WindowScore(seq, potential_cleavage_site);
    std::cout << "PCS: " << potential_cleavage_site << " Score: " << score << std::endl;
  }
  std::cout << "Real CS: " << seq.get_cleavage_site() << std::endl;
  
  delete[] train_sequences;
  //delete[] test_sequences;
}