#include <array>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>

constexpr size_t MATRIX_SIZE = 3;

using Matrix33 = std::array<std::array<float, MATRIX_SIZE>, MATRIX_SIZE>;
// переменная x для горизонтальной, y для вертикальной 

float CalcDet33(const Matrix33& mat)
{
	return mat[0][0] * mat[1][1] * mat[2][2]
		+ mat[0][1] * mat[1][2] * mat[2][0]
		+ mat[0][2] * mat[1][0] * mat[2][1]
		- mat[0][2] * mat[1][1] * mat[2][0]
		- mat[0][1] * mat[1][0] * mat[2][2]
		- mat[0][0] * mat[1][2] * mat[2][1];
}

float CalcMinor(int x_, int y_, const Matrix33& mat)
{
	std::array<float, 4> flatMatrix22{};

	if ((x_ < 0) || (x_ >= MATRIX_SIZE))
	{
		throw std::runtime_error("CalcMinor: x index out of bound");
	}

	if ((y_ < 0) || (y_ >= MATRIX_SIZE))
	{
		throw std::runtime_error("CalcMinor: y index out of bound");
	}

	size_t counter{ 0 };
	for (size_t x = 0; x < MATRIX_SIZE; ++x)
	{
		for (size_t y = 0; y < MATRIX_SIZE; ++y)
		{
			if ((y_ == y) || (x_ == x))
			{
				continue;
			}
			flatMatrix22[counter] = mat[x][y];
			++counter;
		}
	}
	return flatMatrix22[0] * flatMatrix22[3] - flatMatrix22[1] * flatMatrix22[2];
}

Matrix33 GetInverseMatrix(const Matrix33& mat, bool& wasError)
{
	wasError = false;
	Matrix33 adjencyMatrix{};
	float det = CalcDet33(mat);

	if (abs(det) < 0.0000001)
	{
		wasError = true;
		return adjencyMatrix;
	}

	// изменить название переменной / поместить в другую переменную
	float invertedDet = 1 / det;

	// Calc minor & transpose on-the-fly
	adjencyMatrix[0][0] = CalcMinor(0, 0, mat) * invertedDet;
	adjencyMatrix[1][0] = -CalcMinor(0, 1, mat) * invertedDet;
	adjencyMatrix[2][0] = CalcMinor(0, 2, mat) * invertedDet;
	adjencyMatrix[0][1] = -CalcMinor(1, 0, mat) * invertedDet;
	adjencyMatrix[1][1] = CalcMinor(1, 1, mat) * invertedDet;
	adjencyMatrix[2][1] = -CalcMinor(1, 2, mat) * invertedDet;
	adjencyMatrix[0][2] = CalcMinor(2, 0, mat) * invertedDet;
	adjencyMatrix[1][2] = -CalcMinor(2, 1, mat) * invertedDet;
	adjencyMatrix[2][2] = CalcMinor(2, 2, mat) * invertedDet;

	return adjencyMatrix;
}

Matrix33 ReadMatrix(const std::string& fileName, bool& wasError)
{
	wasError = false;
	Matrix33 matrix{};

	std::ifstream input(fileName, std::ios::in);
	if (!input.is_open())
	{
		wasError = true;
		return matrix;
	}

	for (size_t x = 0; x < MATRIX_SIZE; ++x)
	{
		for (size_t y = 0; y < MATRIX_SIZE; ++y)
		{
			if (!(input >> matrix[x][y]))
			{
				wasError = true;
				return matrix;
			}
		}
	}
	return matrix;
}

void PrintMatrix(const Matrix33& mat, int precision)
{
	for (size_t x = 0; x < MATRIX_SIZE; ++x)
	{
		for (size_t y = 0; y < MATRIX_SIZE; ++y)
		{
			std::cout << std::setw(9) << std::setprecision(precision) << mat[x][y] << ' ';
		}
		std::cout << "\n";
	}
}

struct InputData
{
	std::string inputFilePath;
};

InputData ParseInput(int argc, char** argv)
{
	if (argc != 2)
	{
		throw std::invalid_argument("Invalid arguments");
	}

	return InputData{
		argv[1]
	};
}

int main(int argc, char** argv)
{
	InputData inputData;
	try
	{
		inputData = ParseInput(argc, argv);
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << e.what();
		return -1;
	}

	bool wasError;
	Matrix33 matrix = ReadMatrix(inputData.inputFilePath, wasError);
	if (wasError)
	{
		std::cout << "Invalid matrix format";
		return -2;
	}

	Matrix33 invMatrix = GetInverseMatrix(matrix, wasError);
	if (wasError)
	{
		std::cout << "Inverse matrix not exists (det=0)";
		return -3;
	}

	PrintMatrix(invMatrix, 4);
}