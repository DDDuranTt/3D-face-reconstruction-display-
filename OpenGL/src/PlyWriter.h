#pragma once
#include <Eigen/Dense>
#include <vector>
#include "Model.h"
#include <string>

void WriteFile(std::string fileName, Model& mod);

void WriteHeader(FILE* fp, std::vector<std::string>&  header, size_t vertexNum, size_t faceNum);

void WriteData(FILE* fp, std::vector<vertex>&  vertexList, std::vector<face>&  faceList);