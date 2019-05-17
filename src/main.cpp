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

// Remove sequences such that cleavage_site <= p since we ignore the first aa
void RemoveInvalidSequences(Sequence*& sequence_array, int& size) {
  // count number of valid sequences: will be the size of the new sequence_array
  int number_valid_sequences = 0;
  for (int sequence_index = 0; sequence_index < size; sequence_index++) {
    if (sequence_array[sequence_index].get_cleavage_site() > p) { // sequence is valid: don't ignore
      number_valid_sequences++;
    }
  }
  // create new sequence array
  Sequence* new_sequence_array = new Sequence[number_valid_sequences];
  int new_sequence_index = 0;
  for (int sequence_index = 0; sequence_index < size; sequence_index++) {
    if (sequence_array[sequence_index].get_cleavage_site() > p) {
      new_sequence_array[new_sequence_index] = sequence_array[sequence_index];
      new_sequence_index++;
    }
  }
  size = number_valid_sequences;
  sequence_array = new_sequence_array;
}

void SequenceSVMTranslate(Sequence*& sequence_array, int size) {
  // Convert sequence_array into a series of vectors of size 26 * window_size,
  // for each window in each sequence, and write the output into ../data/train_svm.txt,
  // in a format readable by libsvm.

  std::ofstream output_file;
  output_file.open("../data/train_svm.txt");
  for (int sequence_index = 0; sequence_index < size; sequence_index++)
  {
    Sequence seq = sequence_array[sequence_index];
    std::string aa_sequence = seq.get_aa_sequence();
    int length = seq.get_length();
    for (int potential_cs = p + 1; potential_cs < length - q; potential_cs++) // p + 1 because we ignore the first aa
    {
      int feature_index = 1;
      std::string label = (potential_cs == seq.get_cleavage_site()) ? "+1" : "-1";
      output_file << label;
      for (int aa_index = 0; aa_index < p + q; aa_index++)
      {
        char aa = aa_sequence[potential_cs - p + aa_index];
        int aa_letter_index = aa - ASCII_CONSTANT;
        for (int letter_index = 0; letter_index < 26; letter_index++)
        {
          output_file << " " << feature_index << ":" << (aa_letter_index == letter_index);
          feature_index++; 
        }
      }
      output_file << std::endl;
    }
  }
  output_file.close();
}


template <typename T, int rows, int cols> void printMatrix(T (&mat)[rows][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols - 1; j++) {
      printf("%g ", mat[i][j]);
    }
    printf("%g\n", mat[i][cols - 1]);
  }
}

void TestPSSM(Sequence*& train_sequences, int train_size, Sequence*& test_sequences, int test_size) {
  PSSM pssm;
  pssm.train(train_sequences, train_size, 1.);
  // printMatrix(pssm.pssm);

  std::vector<int> candidates_0 = pssm.FindCleavagesThreshold(train_sequences[1], 1.);
  std::cout << "Candidate cleavage sites for a sequence in the training set:" << std::endl;
  for (size_t i = 0; i < candidates_0.size(); i++)
  {
    std::cout << candidates_0[i] << std::endl;
  }

  Sequence seq = train_sequences[1];
  std::cout << "Predicted CS for a sequence in the training set: " << pssm.FindCleavageMax(seq) << std::endl;
  std::cout << "Real CS for a sequence in the training set: " << seq.get_cleavage_site() << std::endl;

  std::cout << "Classification accuracy on training data: " << pssm.SequenceAccuracy(train_sequences, train_size) << std::endl;
  std::cout << "Classification accuracy on test data: " << pssm.SequenceAccuracy(test_sequences, test_size) << std::endl;
}



int main() {
  std::string FILENAME = "../data/EUKSIG_13.red";
  int size = GetSize(FILENAME);
  int test_size = size / 5;
  int train_size = size - test_size;
  CreateTrainTestSets(FILENAME);
  Sequence* train_sequences = FileToSequenceArray(train_filename, train_size);
  std::cout << "Initial number of training sequences: " << train_size << std::endl;
  RemoveInvalidSequences(train_sequences, train_size);
  std::cout << "Number of training sequences after removing invalid data: " << train_size << std::endl;
  Sequence* test_sequences = FileToSequenceArray(test_filename, test_size);
  std::cout << "Initial number of test sequences: " << test_size << std::endl;
  RemoveInvalidSequences(test_sequences, test_size);
  std::cout << "Number of test sequences after removing invalid data: " << test_size << std::endl;

  //TestPSSM(train_sequences, train_size, test_sequences, test_size);

  SequenceSVMTranslate(train_sequences, train_size);

  delete[] train_sequences;
  delete[] test_sequences;
}