#include <algorithm>  // for random_shuffle
#include <cassert>
#include <cstdlib>  // for rand, srand
#include <fstream>  // for writing to files
#include <iostream>
#include <string> // for the getline method
const char *FILENAME = "../data/EUKSIG_13.red";


// Return the number of lines minus one since the provided file's last line is empty
int GetLineCount(const char *filename) {
  int line_count = 0;
  FILE *fp = fopen(filename, "r");

  for (char c = getc(fp); c != EOF; c = getc(fp))
    if (c == '\n')
      line_count++;

  fclose(fp);
  return line_count;
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

void CreateTrainTestSets(const char *filename) {
  std::ifstream inputfile;
  inputfile.open(filename);
  // TODO: create name of train and test files
  std::string train_filename = "../data/train.red";
  std::string test_filename = "../data/test.red";
  // Create train and test files
  std::ofstream trainfile;
  trainfile.open(train_filename);
  std::ofstream testfile;
  testfile.open(test_filename);
  // Compute number of observations 
  int line_count = GetLineCount(filename);
  assert (line_count % 3 == 0);
  int size = line_count / 3;
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
  trainfile.close();
  testfile.close();
  inputfile.close();
}

int main() {
  CreateTrainTestSets("../data/EUKSIG_13.red");
}