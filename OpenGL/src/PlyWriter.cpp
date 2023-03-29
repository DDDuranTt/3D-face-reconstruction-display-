#define _CRT_SECURE_NO_WARNINGS
#include "PlyWriter.h"
#include <iostream>


void WriteFile(std::string fileName, Model& mod) {
	FILE* fp = fopen(fileName.c_str(), "wb+");
	WriteHeader(fp, mod.header, mod.vertexNum, mod.faceNum);
	WriteData(fp, mod.vertices, mod.faces);
}

void WriteHeader(FILE* fp, std::vector<std::string>& header, size_t vertexNum, size_t faceNum) {
	for (std::string str : header) {
		if (str == "element vertex") {
			fputs((str + " " + std::to_string(vertexNum) + "\n").c_str(), fp);
		}
		else if (str == "element face") {
			fputs((str + " " + std::to_string(faceNum) + "\n").c_str(), fp);
		}
		else {
			fputs((str + "\n").c_str(), fp);
		}

	}
}

void WriteData(FILE* fp, std::vector<vertex>& vertexList, std::vector<face>& faceList) {
	std::string outPutStr;

	for (int i = 0; i < vertexList.size(); ++i) {
		outPutStr = "";
		outPutStr = std::to_string(vertexList[i].position(0)) + ' ' + std::to_string(vertexList[i].position(1)) + ' ' + std::to_string(vertexList[i].position(2)) + ' '
			+ std::to_string(vertexList[i].normal(0)) + ' ' + std::to_string(vertexList[i].normal(1)) + ' ' + std::to_string(vertexList[i].normal(2)) + ' '
			+ std::to_string(vertexList[i].uv(0)) + ' ' + std::to_string(vertexList[i].uv(1)) + '\n';
		fputs(outPutStr.c_str(), fp);
	}
	for (const auto& face : faceList) {
		outPutStr = "3 " + std::to_string(face.index(0)) + ' ' + std::to_string(face.index(1)) + ' ' + std::to_string(face.index(2)) + '\n';
		fputs(outPutStr.c_str(), fp);
	}
}