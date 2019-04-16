#include <fstream>
#include <string> // for the getline method

#include "main.hpp"
#include "sequence.hpp"


Sequence* FileToSequenceArray(char* filename, int line_count) {
  std::ifstream inputfile;
  inputfile.open(filename);
  std::string line;
  assert (line_count % 3 == 0);
  int size = line_count / 3;
  Sequence* sequence_array = new Sequence[size];
  std::string line;
  for (int sequence_index = 0; sequence_index < size; sequence_index++) {
    std::getline(inputfile, line); // first line: do nothing
    // TODO: compute aa_sequence, cleavage site and length
    std::getline(inputfile, line); // second line: retrieve aa_sequence 
    std::getline(inputfile, line); // third line: compute cleavage site and length
    sequence_array[sequence_index] = Sequence(aa_sequence, cleavage site, length);
  }
}




int main() {
  return 0;
}