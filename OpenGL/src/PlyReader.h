#pragma once
#include <vector>
#include "Model.h"
std::vector< std::string > get_words(FILE* fp, char** orig_line);

Model* ReadFile(std::string fileName);

void ReadHeader(FILE* fp, std::vector<std::string>& header, size_t& vn, size_t& fn);

void ReadData(FILE* fp, Model& mod);

FILE* fopen(std::string fileName);