#include "Image.hpp"

#include <stdexcept>
#include <tuple>
#include <vector>

namespace common {
Image::Image(int numrows, int numcols, const Color &fillColor)
    : m_numrows(numrows), m_numcols(numcols) {
    if (numrows <= 0) {
        throw std::runtime_error("Image cannot have 0 or fewer rows.");
    }
    if (numcols <= 0) {
        throw std::runtime_error("Image cannot have 0 or fewer columns.");
    }

    for (int i = 0; i < numrows; i++) {
        m_pixels.push_back(std::vector<Color>());
        for (int j = 0; j < numcols; j++) {
            m_pixels[i].push_back(fillColor);
        }
    }
}

Image::Image(const std::vector<std::vector<Color>> &pixels)
    : m_numrows(pixels.size()), m_numcols(pixels.front().size()) {
    for (const auto &row : pixels) {
        if (row.size() != m_numcols) {
            throw std::runtime_error(
                "All rows in an image must have the same number of pixels.");
        }
    }

    m_pixels = pixels;
}

std::pair<int, int> Image::shape() const { return {m_numrows, m_numcols}; }

int Image::numRows() const { return m_numrows; }

int Image::numCols() const { return m_numcols; }

Image Image::resize(int numrows, int numcols) const {
    Image resized = Image(numrows, numcols);

    for (int i = 0; i < numrows; i++) {
        float percentHeight = (float)i / (float)numrows;
        float oldRow = percentHeight * numRows();

        for (int j = 0; j < numcols; j++) {
            float percentWidth = (float)j / (float)numcols;
            float oldCol = percentWidth * numCols();

            // Weighted sum of four pixels
            Color weightedSum = Color();

            float percentOriginalRow = 1.0 - (oldRow - (int)oldRow);
            float percentOriginalCol = 1.0 - (oldCol - (int)oldCol);
            bool edgeRow = oldRow >= numRows() - 1;
            bool edgeCol = oldCol >= numCols() - 1;

            Color origin = at((int)oldRow, (int)oldCol);
            weightedSum +=
                origin * (double)(percentOriginalRow * percentOriginalCol);

            if (!edgeRow) {
                Color neighbor = at((int)oldRow, (int)oldCol);
                weightedSum += neighbor * ((1.0 - percentOriginalRow) *
                                           percentOriginalCol);
            }

            if (!edgeCol) {
                Color neighbor = at((int)oldRow, (int)oldCol + 1);
                weightedSum += neighbor * (percentOriginalRow *
                                           (1.0 - percentOriginalCol));
            }

            if (!(edgeRow || edgeCol)) {
                Color neighbor = at((int)oldRow + 1, (int)oldCol + 1);
                weightedSum += neighbor * ((1.0 - percentOriginalRow) *
                                           (1.0 - percentOriginalCol));
            }

            resized.at(i, j) = weightedSum;
        }
    }
    return resized;
}

Color &Image::at(int row, int column) { return m_pixels.at(row).at(column); }

const Color &Image::at(int row, int column) const {
    return m_pixels.at(row).at(column);
}
} // namespace common