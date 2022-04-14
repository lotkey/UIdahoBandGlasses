#pragma once

#include <tuple>
#include <vector>

namespace common {
/// Class for templated, fixed-size matrices.
/// Any "modifiers" return a modified copy.
/// Base class for Images and Masks.
/// Since this is a templated class, there is no corresponding source file.
template <typename T> class Matrix {
  public:
    Matrix();
    /// Construct matrix from size
    Matrix(int numrows, int numcols);
    /// Construct matrix from 2D array
    Matrix(const std::vector<std::vector<T>> &data);
    Matrix(const Matrix &);
    Matrix(Matrix &&);
    ~Matrix();
    void operator=(const Matrix &);
    void operator=(Matrix &&);

    /// @returns A reference to the object at the position specified
    T &at(int row, int col);
    /// @returns A const reference to the object at the position specified
    const T &at(int row, int col) const;
    /// @returns A pair with {numrows, numcols}
    std::pair<int, int> shape() const;
    /// @returns Number of rows in the matrix
    int numRows() const;
    /// @returns Number of columns in the matrix
    int numCols() const;
    /// @returns The resized image
    Matrix<T> resize(int numrows, int numcols) const;

  protected:
    int m_numrows;
    int m_numcols;
    T **m_data = nullptr;
    // std::vector<std::vector<T>> m_data;

    /// @returns A weighted sum of references from a floating point position.
    /// at(1.4, 0.3) would return the weighted sum of these pixels:
    /// - 1, 0 (60% * 70%)
    /// - 2, 0 (40% * 70%)
    /// - 1, 1 (60% * 30%)
    /// - 2, 1 (40% * 30%)
    T at(float row, float col) const;
    void copyTo(Matrix &) const;
    void moveTo(Matrix &);
    void allocateMemory();
    void freeMemory();
};

template <typename T> Matrix<T>::Matrix() {}

template <typename T> Matrix<T>::Matrix(const Matrix<T> &m) { m.copyTo(*this); }

template <typename T> Matrix<T>::Matrix(Matrix<T> &&m) { m.moveTo(*this); }

template <typename T>
Matrix<T>::Matrix(int numrows, int numcols)
    : m_numrows(numrows), m_numcols(numcols) {
    allocateMemory();
}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &data)
    : m_numrows(data.size()), m_numcols(data.front().size()) {
    allocateMemory();
    for (int i = 0; i < m_numrows; i++) {
        for (int j = 0; j < m_numcols; j++) {
            m_data[i][j] = m_data[i][j];
        }
    }
}

template <typename T> void Matrix<T>::operator=(const Matrix<T> &m) {
    m.copyTo(*this);
}

template <typename T> void Matrix<T>::operator=(Matrix<T> &&m) {
    m.moveTo(*this);
}

template <typename T> void Matrix<T>::allocateMemory() {
    m_data = new T *[m_numrows];
    for (int i = 0; i < m_numrows; i++) {
        m_data[i] = new T[m_numcols];
        for (int j = 0; j < m_numcols; j++) {
            m_data[i][j] = (T)0;
        }
    }
}

template <typename T> Matrix<T>::~Matrix() { freeMemory(); }

template <typename T> void Matrix<T>::freeMemory() {
    if (m_data == nullptr) {
        return;
    }
    for (int i = 0; i < m_numrows; i++) {
        delete[] m_data[i];
    }
    delete[] m_data;
    m_data = nullptr;
}

template <typename T> T &Matrix<T>::at(int row, int col) {
    return m_data[row][col];
}

template <typename T> const T &Matrix<T>::at(int row, int col) const {
    return m_data[row][col];
}

template <typename T> std::pair<int, int> Matrix<T>::shape() const {
    return {m_numrows, m_numcols};
}

template <typename T> int Matrix<T>::numRows() const { return m_numrows; }

template <typename T> int Matrix<T>::numCols() const { return m_numcols; }

template <typename T>
Matrix<T> Matrix<T>::resize(int numrows, int numcols) const {
    Matrix<T> resized = Matrix<T>(numrows, numcols);
    std::vector<T> v;

    for (int i = 0; i < numrows; i++) {
        float percentHeight = (float)i / (float)numrows;
        float oldRow = percentHeight * numRows();

        for (int j = 0; j < numcols; j++) {
            float percentWidth = (float)j / (float)numcols;
            float oldCol = percentWidth * numCols();

            resized.at(i, j) = at(oldRow, oldCol);
        }
    }
    return resized;
}

template <typename T> T Matrix<T>::at(float row, float col) const {
    T weightedSum = (T)0;
    float percentRow1 = row - (int)row;
    float percentRow0 = 1.0 - percentRow1;
    float percentCol1 = col - (int)col;
    float percentCol0 = 1.0 - percentCol0;

    float pixelPercents[2][2] = {
        {percentRow0 * percentCol0, percentRow1 * percentCol0},
        {percentRow1 * percentCol0, percentRow1 * percentCol1}};

    weightedSum += at((int)row, (int)col) * pixelPercents[0][0];
    if ((int)row < numRows() - 1) {
        weightedSum += at((int)row + 1, (int)col) * pixelPercents[1][0];
    }
    if ((int)col < numCols() - 1) {
        weightedSum += at((int)row, (int)col + 1) * pixelPercents[0][1];
    }
    if ((int)row < numRows() - 1 && (int)col < numCols() - 1) {
        weightedSum += at((int)row + 1, (int)col + 1) * pixelPercents[1][1];
    }

    return weightedSum;
}

template <typename T> void Matrix<T>::copyTo(Matrix<T> &m) const {
    m.freeMemory();
    m.m_numcols = m_numcols;
    m.m_numrows = m_numrows;
    m.allocateMemory();

    for (int i = 0; i < m_numrows; i++) {
        for (int j = 0; j < m_numcols; j++) {
            m.m_data[i][j] = m_data[i][j];
        }
    }
}

template <typename T> void Matrix<T>::moveTo(Matrix<T> &m) {
    m.m_data = m_data;
    m.m_numcols = m_numcols;
    m.m_numrows = m_numrows;

    m_data = nullptr;
    m_numcols = 0;
    m_numrows = 0;
}
} // namespace common