#pragma once
#include "Model.h"
#include "PlyReader.h"

struct vec4 {
	float x, y, z, w;
};


struct mat4 {
	vec4 value[4];
	vec4& operator[](int i) {
		return this->value[i];
	}
};

void ComputeUV(vertex& vtx, mat4& mat) {
	Eigen::Vector4f clipPosition = Eigen::Vector4f::Zero();
	Eigen::Vector4f ndcPosition = Eigen::Vector4f::Zero();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (i == 0) {
				clipPosition(i) += vtx.position(j) * mat[j].x;
			}
			else if (i == 1) {
				clipPosition(i) += vtx.position(j) * mat[j].y;
			}
			else if (i == 2) {
				clipPosition(i) += vtx.position(j) * mat[j].z;
			}
			else {
				clipPosition(i) += vtx.position(j) * mat[j].w;
			}

		}
	}
	ndcPosition(0) = clipPosition(0) / clipPosition(3);
	ndcPosition(1) = clipPosition(1) / clipPosition(3);
	ndcPosition(2) = clipPosition(2) / clipPosition(3);
	ndcPosition(3) = clipPosition(3) / clipPosition(3);

	vtx.uv(0) = (ndcPosition(0) + 1.0f) / 2.0f;
	vtx.uv(1) = (ndcPosition(1) - 1.0f) / -2.0f;

}

bool ReadMat(std::string fileName, mat4& targetMat) {
	FILE* fp = fopen(fileName);
	if (fp == NULL) {
		return false;
	}
	char* orig_line;
	char* pEnd;
	for (int i = 0; i < 4; ++i) {
		get_words(fp, &orig_line);
		targetMat[i].x = strtof(orig_line, &pEnd);
		targetMat[i].y = strtof(pEnd, &pEnd);
		targetMat[i].z = strtof(pEnd, &pEnd);
		targetMat[i].w = strtof(pEnd, NULL);
	}
	return true;
}