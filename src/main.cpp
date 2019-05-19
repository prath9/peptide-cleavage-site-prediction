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



std::vector<std::string> SequenceToWindows(Sequence*& sequence_array, int size) {
  // Write all the windows corresponding to sequence_array in the result vector
  std::vector<std::string> window_vector;
  for (int sequence_index = 0; sequence_index < size; sequence_index++)
  {
    Sequence seq = sequence_array[sequence_index];
    std::string aa_sequence = seq.get_aa_sequence();
    int length = seq.get_length();
    for (int potential_cs = p + 1; potential_cs < length - q; potential_cs++) // p + 1 because we ignore the first aa
    { 
      std::string window_string = aa_sequence.substr(potential_cs - p, p + q);     
      window_vector.push_back(window_string);
    }
  }
  return window_vector;
}

std::vector<std::string> SequenceToLabels(Sequence*& sequence_array, int size) {
  // Write all the labels corresponding to sequence_array in the result vector
  std::vector<std::string> label_vector;
  for (int sequence_index = 0; sequence_index < size; sequence_index++)
  {
    Sequence seq = sequence_array[sequence_index];
    int length = seq.get_length();
    int cleavage_site = seq.get_cleavage_site();
    for (int potential_cs = p + 1; potential_cs < length - q; potential_cs++) // p + 1 because we ignore the first aa
    { 
      std::string label = (potential_cs == cleavage_site) ? "+1" : "-1" ;     
      label_vector.push_back(label);
    }
  }
  return label_vector;
}

void SequenceSVMTranslate(std::vector<std::string> window_vector, std::vector<std::string> label_vector, std::string filename) {
  // Convert sequence_array into a series of vectors of size 26 * window_size,
  // for each window in each sequence, and write the output into filename,
  // in a format readable by libsvm.

  std::ofstream output_file;
  output_file.open(filename);
  int num_windows = window_vector.size();
  for (int window_index = 0; window_index < num_windows; window_index++) {
    int feature_index = 1;
    output_file << label_vector[window_index];
    std::string window_string = window_vector[window_index];
    for (int aa_index = 0; aa_index < p + q; aa_index++)
      {
        char aa = window_string[aa_index];
        int aa_letter_index = aa - ASCII_CONSTANT;
        for (int letter_index = 0; letter_index < 26; letter_index++)
        {
          output_file << " " << feature_index << ":" << (aa_letter_index == letter_index);
          feature_index++; 
        }
      }
      output_file << std::endl;
  }
  output_file.close();
}


void SequenceSVMTranslateSimilarityMatrix(std::vector<std::string> window_vector, std::vector<std::string> label_vector, std::vector<std::string> train_window_vector, std::vector<std::vector<int> > similarity_matrix, std::string filename) {
// write the value of the similarity (defined in a specified AA-wise similarity matrix) between 
// the windows in window_vector and those in train_window_vector (training instances) into filename
// such that the format of filename is readable by libsvm
  std::ofstream output_file;
  output_file.open(filename);
  int num_windows = window_vector.size();
  int train_num_windows = train_window_vector.size();
  for (int window_index = 0; window_index < num_windows; window_index++)
  {
    output_file << label_vector[window_index] << " 0:" << window_index + 1;
    std::string window_string = window_vector[window_index];
    for (int train_window_index = 0; train_window_index < train_num_windows; train_window_index++)
    {
      std::string train_window_string = train_window_vector[train_window_index];
      // use similarity_matrix to compute the similarity between windows
      int similarity = 0;
      for (int aa_index = 0; aa_index < p + q; aa_index++) {
        char aa = window_string[aa_index];
        char train_aa = train_window_string[aa_index];
        int aa_letter_index = aa - ASCII_CONSTANT;
        int train_aa_letter_index = train_aa - ASCII_CONSTANT;
        similarity += similarity_matrix[aa_letter_index][train_aa_letter_index];
      }
      output_file << " " << train_window_index + 1 << ":" << similarity;
    }
    output_file << std::endl;
  }
  output_file.close();
}


std::vector<std::vector<int> > FileToMatrix(std::string filename) {
  std::vector<std::vector<int> > mat;
  std::ifstream inputfile;
  inputfile.open(filename);
  std::string line;
  while (std::getline(inputfile, line)) {
    int length = line.length();
    std::vector<int> mat_line; 
    bool minus = false;
    for (int i = 0; i < length; i++)
    {
      if (line[i] != ' ') {
        if (line[i] == '-') {
          minus = true;
        }
        else {
          if (minus == true) {
            mat_line.push_back(-(line[i] - NUMBER_ASCII_CONSTANT));
            minus = false;
          }
          else {
            mat_line.push_back(line[i] - NUMBER_ASCII_CONSTANT); 
          }
        }
      }
    }
    mat.push_back(mat_line);
  }
  return mat;
}


template <typename T, int rows, int cols> void printMatrix(T (&mat)[rows][cols]) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols - 1; j++) {
      printf("%g ", mat[i][j]);
    }
    printf("%g\n", mat[i][cols - 1]);
  }
}


void printMatrixSTL(std::vector<std::vector<int> > mat) {
  int num_lines = mat.size();
  int num_columns = mat[0].size();
  for (int i = 0; i < num_lines; i++) {
    for (int j = 0; j < num_columns - 1; j++)
    {
      if (mat[i][j] >= 0) {
        std::cout << " " << mat[i][j] << " ";
      }
      else {
        std::cout << mat[i][j] << " ";
      }
    }
    if (mat[i][num_columns - 1] >= 0) {
      std::cout << " " << mat[i][num_columns - 1] << std::endl;
    }
    else {
      std::cout << mat[i][num_columns - 1] << std::endl;
    }
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
  std::string FILENAME = "../data/EUKSIG_13_small.red";
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
  std::cout << std::endl;

  //TestPSSM(train_sequences, train_size, test_sequences, test_size);
  std::vector<std::string> train_window_vector = SequenceToWindows(train_sequences, train_size);
  std::vector<std::string> test_window_vector = SequenceToWindows(test_sequences, test_size);
  std::vector<std::string> train_label_vector = SequenceToLabels(train_sequences, train_size);
  std::vector<std::string> test_label_vector = SequenceToLabels(test_sequences, test_size);

  // Build libsvm-compatible datasets for default kernels
  //std::cout << "Building ../data/train_svm.txt ... ";
  //SequenceSVMTranslate(train_window_vector, train_label_vector, "../data/train_svm.txt");
  //std::cout << "OK" << std::endl;
  //std::cout << "Building ../data/test_svm.txt ... ";
  //SequenceSVMTranslate(test_window_vector, test_label_vector, "../data/test_svm.txt");
  //std::cout << "OK" << std::endl;

  // test matrix builder
  std::cout << "Identity matrix:" << std::endl;
  std::vector<std::vector<int> > identity = FileToMatrix("../data/matrices/identity.txt"); 
  printMatrixSTL(identity);
  std::cout << "BLOSUM62 matrix:" << std::endl;
  std::vector<std::vector<int> > blosum62 = FileToMatrix("../data/matrices/BLOSUM62.txt"); 
  printMatrixSTL(blosum62);

  // Build libsvm-compatible datasets for precomputed kernels
  std::cout << "Building ../data/train_svm_identity.txt ... " << std::endl;
  SequenceSVMTranslateSimilarityMatrix(train_window_vector, train_label_vector, train_window_vector, identity, "../data/train_svm_identity.txt");
  std::cout << "OK" << std::endl;
  std::cout << "Building ../data/train_svm_blosum62.txt ... " << std::endl;
  SequenceSVMTranslateSimilarityMatrix(train_window_vector, train_label_vector, train_window_vector, identity, "../data/train_svm_blosum62.txt");
  std::cout << "OK" << std::endl;

  // delete sequence arrays
  delete[] train_sequences;
  delete[] test_sequences;
}