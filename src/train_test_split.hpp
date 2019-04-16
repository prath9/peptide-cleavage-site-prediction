#pragma once

std::string train_filename = "../data/train.red";
std::string test_filename = "../data/test.red";

int GetLineCount(std::string filename);
int GetSize(std::string filename);
bool* CreateTrainTestMask(int size);
void PrintArray(bool* array, int size);
void CreateTrainTestSets(std::string filename);