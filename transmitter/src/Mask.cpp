// Chris McVickar
#include "Mask.hpp"
#include "colors.hpp"
#include "common/common.hpp"

#include <tuple>
#include <vector>

namespace transmitter {
Mask::Mask(const common::Image &img)
    : common::Matrix<double>(img.numRows(), img.numCols()) {
    for (int i = 0; i < img.numRows(); i++) {
        for (int j = 0; j < img.numCols(); j++) {
            at(i, j) = img.at(i, j).intensity() / 255.0;
        }
    }
}

Mask::Mask(int numrows, int numcols)
    : common::Matrix<double>(numrows, numcols) {}

Mask::Mask(const Matrix<double> &matrix) : Matrix<double>(matrix) {}

Mask Mask::invert() const {
    Mask inverted = *this;
    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            inverted.at(i, j) = 1.0 - at(i, j);
        }
    }
    return inverted;
}

common::Image Mask::maskImage(common::Image img) const {
    if (img.shape() != shape()) {
        throw std::runtime_error("Mask and image must be the same size");
    }

    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            img.at(i, j) *= at(i, j);
        }
    }

    return img;
}

common::Image Mask::toImage(const common::Color &fg_color,
                            const common::Color &bg_color) {
    common::Image img(numRows(), numCols());
    for (int i = 0; i < numRows(); i++) {
        for (int j = 0; j < numCols(); j++) {
            img.at(i, j) = fg_color * at(i, j) + bg_color * (1.0 - at(i, j));
        }
    }
    return img;
}
} // namespace transmitter