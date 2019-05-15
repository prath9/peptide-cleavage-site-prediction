#pragma once

Sequence* FileToSequenceArray(std::string filename, int size);
void RemoveInvalidSequences(Sequence*& sequence_array, int& size);