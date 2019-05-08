#pragma once

extern std::string train_filename;
extern std::string test_filename;

int GetLineCount(std::string filename);
int GetSize(std::string filename);
bool* CreateTrainTestMask(int size);
void PrintArray(bool* array, int size);
void CreateTrainTestSets(std::string filename);