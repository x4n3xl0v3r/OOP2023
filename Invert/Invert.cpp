#include <iostream>
#include <array>
#include <fstream>
#include <cassert>
#include <iomanip>

constexpr size_t MATRIX_SIZE = 3;

using Matrix33 = std::array<std::array<float, MATRIX_SIZE>, MATRIX_SIZE>;

float CalcDet33(const Matrix33 &mat)
{
    return mat[0][0] * mat[1][1] * mat[2][2]
        + mat[0][1] * mat[1][2] * mat[2][0]
        + mat[0][2] * mat[1][0] * mat[2][1]
        - mat[0][2] * mat[1][1] * mat[2][0]
        - mat[0][1] * mat[1][0] * mat[2][2]
        - mat[0][0] * mat[1][2] * mat[2][1];
}

float CalcMinor(int x_, int y_, const Matrix33 &mat)
{
    std::array<float, 4> flatMatrix22;

    assert((x_ >= 0) && (x_ < MATRIX_SIZE));
    assert((y_ >= 0) && (y_ < MATRIX_SIZE));

    size_t counter = 0;
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

Matrix33 GetAdjencyMatrix(const Matrix33 &mat, bool &wasError)
{
    wasError = false;
    Matrix33 adjencyMatrix;
    float det = CalcDet33(mat);

    if (abs(det) < 0.0000001) 
    {
        wasError = true;
        return adjencyMatrix;
    }

    det = 1 / det;

    // Calc minor & transpose on-the-fly
    adjencyMatrix[0][0] = CalcMinor(0, 0, mat) * det;
    adjencyMatrix[1][0] = -CalcMinor(0, 1, mat) * det;
    adjencyMatrix[2][0] = CalcMinor(0, 2, mat) * det;
    adjencyMatrix[0][1] = -CalcMinor(1, 0, mat) * det;
    adjencyMatrix[1][1] = CalcMinor(1, 1, mat) * det;
    adjencyMatrix[2][1] = -CalcMinor(1, 2, mat) * det;
    adjencyMatrix[0][2] = CalcMinor(2, 0, mat) * det;
    adjencyMatrix[1][2] = -CalcMinor(2, 1, mat) * det;
    adjencyMatrix[2][2] = CalcMinor(2, 2, mat) * det;

    return adjencyMatrix;
}

Matrix33 ReadMatrix(const std::string &fileName, bool &wasError)
{
    wasError = false;
    std::ifstream input(fileName, std::ios::in);
    assert(input.is_open());

    Matrix33 matrix;
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

void PrintMatrix(const Matrix33 &mat, int precision)
{
    for (size_t x = 0; x < MATRIX_SIZE; ++x)
    {
        for (size_t y = 0; y < MATRIX_SIZE; ++y)
        {
            std::cout << std::setw(9) << mat[x][y] << ' ' << std::setprecision(precision);
        }
        std::cout << "\n";
    }
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    bool wasError = false;
    Matrix33 m = ReadMatrix(argv[1], wasError);

    if (wasError)
    {
        std::cout << "Invalid matrix format";
        return -1;
    }


    Matrix33 adjMatrix = GetAdjencyMatrix(m, wasError);
    if (wasError)
    {
        std::cout << "Inverse matrix not exists (det=0)";
        return -1;
    }

    PrintMatrix(adjMatrix, 4);
}