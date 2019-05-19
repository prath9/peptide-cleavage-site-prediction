#pragma once

#define p Predictor::p
#define q Predictor::q
#define ASCII_CONSTANT 65

Sequence* FileToSequenceArray(std::string filename, int size);
void RemoveInvalidSequences(Sequence*& sequence_array, int& size);
std::vector<std::string> SequenceToLabels(Sequence*& sequence_array, int size);
std::vector<std::string> SequenceToWindows(Sequence*& sequence_array, int size);
void SequenceSVMTranslate(std::vector<std::string> window_vector, std::vector<std::string> label_vector, std::string filename);
void SequenceSVMTranslateSimilarityMatrix(std::vector<std::string> window_vector, std::vector<std::string> label_vector, std::vector<std::string> train_window_vector, int similarity_matrix[26][26], std::string filename);
template <typename T, int rows, int cols> void printMatrix(T (&mat)[rows][cols]);
void TestPSSM(Sequence*& train_sequences, int train_size, Sequence*& test_sequences, int test_size);