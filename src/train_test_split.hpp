#pragma once

int GetLineCount(std::string filename);
int GetSize(std::string filename);
bool* CreateTrainTestMask(int size);
void PrintArray(bool* array, int size);
void CreateTrainTestSets(std::string filename, std::string train_filename, std::string test_filename);