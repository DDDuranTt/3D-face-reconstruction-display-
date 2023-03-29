#include <iostream>
#include <string.h>
#include "vector.h"
#include "matrix.h"



vector vector::operator*(const matrix& other) const
{

	if (this->count == other.row) {
		std::cout << "这是右乘" << std::endl;
	}
	else if (this->count == other.col) {
		std::cout << "这是左乘" << std::endl;
	}
	else {
		std::cout<<"[Error]: 无法匹配矩阵乘法"<<std::endl;
	}
}

vector vector::operator*(const int other) const
{
	for (int i = 0; i < count; ++i)
	{
		data[i] *= other;
	}
}

vector vector::operator*(const float other) const 
{
	for (int i = 0; i < count; ++i)
	{
		data[i] *= other;
	}
}

vector vector::dot(const vector& other) const
{

}


vector vector::operator-(const vector& other) const
{

}


vector vector::operator+(const vector& other) const
{

}


vector::~vector()
{
	delete data;
}



vector::vector(int _count)
	:count(_count)
{
	data = new float[count];
	memset(data, 0, sizeof(data));
}