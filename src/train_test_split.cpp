#include <algorithm>  // for random_shuffle
#include <cassert>
#include <cstdlib>  // for rand, srand
#include <fstream>  // for writing to files
#include <iostream>
#include <string> // for the getline method

#include "train_test_split.hpp"

// Return the number of lines minus one since the provided file's last line is empty
int GetLineCount(std::string filename) {
  int line_count = 0;
  std::ifstream inputfile;
  inputfile.open(filename);
  std::string line;
  while (std::getline(inputfile, line)) {
    line_count++;
  }
  return line_count;
}

// Return the number of sequences in the given file
int GetSize(std::string filename) {
  int line_count = GetLineCount(filename);
  assert (line_count % 3 == 0);
  return line_count / 3;
}

// Create an array of bools with 20% of true (test data) and 80% of false (training data).
// The number of true is rounded to the lesser integer.
// Indices are chosen by building a random shuffle.
bool* CreateTrainTestMask(int size) {
  bool* mask = new bool[size]();
  for (int i = 0; i < size / 5; i++)
    mask[i] = true;

  std::srand(0);
  std::random_shuffle(mask, mask + size);
  return mask;
}

// Test function
void PrintArray(bool* array, int size) {
  for (int i = 0; i < size - 1; i++) {
    std::cout << array[i] << " ";
  }
  std::cout << array[size - 1] << std::endl;
}

void CreateTrainTestSets(std::string filename, std::string train_filename, std::string test_filename) {
  std::ifstream inputfile;
  inputfile.open(filename);
  // Create train and test files
  std::ofstream trainfile;
  trainfile.open(train_filename);
  std::ofstream testfile;
  testfile.open(test_filename);
  // Compute number of observations 
  int size = GetSize(filename);
  // For every observation write in corresponding file
  bool* mask = CreateTrainTestMask(size);
  std::string line;
  for (int i = 0; i < size; i++) {
    for (int l = 0; l < 3; l++) {
      std::getline(inputfile, line);
      if (mask[i]) 
        testfile << line << std::endl;
      else 
        trainfile << line << std::endl;
    }
  }
  delete[] mask;
  trainfile.close();
  testfile.close();
  inputfile.close();
}