#include <algorithm>  // for random_shuffle
#include <cstdlib>  // for rand, srand
#include <iostream>
#include <fstream>  // for writing to files

const char *FILENAME = "../data/EUKSIG_13.red";

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
  
}

int main() {
  int size = GetLineCount(FILENAME);
  bool* mask = CreateTrainTestMask(size);
  PrintArray(mask, size);
}