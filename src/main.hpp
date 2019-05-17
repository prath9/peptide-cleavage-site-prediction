#pragma once

#define p Predictor::p
#define q Predictor::q
#define ASCII_CONSTANT 65

Sequence* FileToSequenceArray(std::string filename, int size);
void RemoveInvalidSequences(Sequence*& sequence_array, int& size);
void SequenceSVMTranslate(Sequence*& sequence_array, int size);
template <typename T, int rows, int cols> void printMatrix(T (&mat)[rows][cols]);
void TestPSSM(Sequence*& train_sequences, int train_size, Sequence*& test_sequences, int test_size);