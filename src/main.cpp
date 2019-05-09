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
    std::getline(inputfile, line); // second line: retrieve aa_sequence
    aa_sequence = line;
    std::getline(inputfile, line); // third line: compute cleavage site and length
    length = aa_sequence.length();  // number of bytes (and characters, here) in aa_sequence
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

  std::cout << train_sequences[0].get_aa_sequence() << std::endl;

  double score = pssm.WindowScore(train_sequences[0], Predictor::p);
  std::cout << score << std::endl;
  
  delete[] train_sequences;
  //delete[] test_sequences;
}