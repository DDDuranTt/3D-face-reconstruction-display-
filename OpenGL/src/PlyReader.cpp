#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>

#include "PlyReader.h"


std::vector< std::string > get_words(FILE* fp, char** orig_line)
{
#define BIG_STRING 4096
	static char str[BIG_STRING];
	static char str_copy[BIG_STRING];
	std::vector< std::string > words;
	int max_words = 10;
	int num_words = 0;
	char* ptr, * ptr2;
	char* result;

	/* read in a line */
	result = fgets(str, BIG_STRING, fp);
	if (result == NULL)
	{
		*orig_line = NULL;
		return words;
	}
	/* convert line-feed and tabs into spaces */
	/* (this guarentees that there will be a space before the */
	/*  null character at the end of the string) */

	str[BIG_STRING - 2] = ' ';
	str[BIG_STRING - 1] = '\0';

	for (ptr = str, ptr2 = str_copy; *ptr != '\0'; ptr++, ptr2++)
	{
		*ptr2 = *ptr;
		// Added line here to manage carriage returns
		if (*ptr == '\t' || *ptr == '\r')
		{
			*ptr = ' ';
			*ptr2 = ' ';
		}
		else if (*ptr == '\n')
		{
			*ptr = ' ';
			*ptr2 = '\0';
			break;
		}
	}

	/* find the words in the line */

	ptr = str;
	while (*ptr != '\0')
	{
		/* jump over leading spaces */
		while (*ptr == ' ') ptr++;

		/* break if we reach the end */
		if (*ptr == '\0') break;

		char* _ptr = ptr;

		/* jump over non-spaces */
		while (*ptr != ' ') ptr++;

		/* place a null character here to mark the end of the word */
		*ptr++ = '\0';

		/* save pointer to beginning of word */
		words.push_back(_ptr);
	}

	/* return the list of words */
	*orig_line = str_copy;
	return words;
}

void ReadData(FILE* fp, Model& mod) {
	char* orig_line;
	char* pEnd;
	vertex tempVertex;
	for (int i = 0; i < mod.vertexNum; ++i)
	{
		get_words(fp, &orig_line);
		tempVertex.position(0) = strtof(orig_line, &pEnd);
		tempVertex.position(1) = strtof(pEnd, &pEnd);
		tempVertex.position(2) = strtof(pEnd, &pEnd);
		tempVertex.normal(0) = strtof(pEnd, &pEnd);
		tempVertex.normal(1) = strtof(pEnd, &pEnd);
		tempVertex.normal(2) = strtof(pEnd, NULL);
		mod.vertices.push_back(tempVertex);
	}
	for (int i = 0; i < mod.faceNum; ++i) {
		get_words(fp, &orig_line);
		strtof(orig_line, &pEnd);
		mod.faces[i].index(0) = (int)strtof(pEnd, &pEnd);
		mod.faces[i].index(1) = (int)strtof(pEnd, &pEnd);
		mod.faces[i].index(2) = (int)strtof(pEnd, NULL);
	}
}

void ReadHeader(FILE* fp, std::vector<std::string>& header, size_t& vn, size_t& fn) {

	size_t vertexNum = 0, faceNum = 0;

	std::vector<std::string> words;
	char* orig_line;
	words = get_words(fp, &orig_line);
	if (!words.size() || words[0] != "ply")
	{
		std::cout << "[Reader Error] this is not ply file." << std::endl;
		return;
	}

	while (words.size())
	{
		/*剔除value与curvature的header*/
		if (words[0] != "comment") {
			if (strcmp(orig_line, "property float nz") == 0) {//剔除curvature，增加u、v
				header.push_back(orig_line);
				header.push_back("property float u");
				header.push_back("property float v");
			}
			else if (words[0] == "element") {
				header.push_back(words[0] + " " + words[1]);
				if (words[1] == "vertex") {
					for (size_t i = words[2].length(), j = 0; i > 0; --i, ++j) {
						vertexNum += (size_t)((words[2][j] - '0') * pow(10.0, i - 1));
					}
				}
				else if (words[1] == "face") {
					for (size_t i = words[2].length(), j = 0; i > 0; --i, ++j) {
						faceNum += (size_t)((words[2][j] - '0') * pow(10.0, i - 1));
					}
				}
			}
			else header.push_back(orig_line);

		}
		if (words[0] == "end_header") break;
		words = get_words(fp, &orig_line);
	}

	vn = vertexNum;
	fn = faceNum;

}


Model* ReadFile(std::string fileName) {
	FILE* fp = fopen(fileName.c_str(), "rt");
	if (fp == NULL) {
		std::cout << "[Reader Error] fail to open file" << std::endl;
		return NULL;
	}

	Model* res = new Model();

	ReadHeader(fp, res->header, res->vertexNum, res->faceNum);
	res->SetVertexNum();
	res->SetFaceNum();
	ReadData(fp, *res);

	return res;
}

FILE* fopen(std::string fileName) {
	return fopen(fileName.c_str(), "rt");
}