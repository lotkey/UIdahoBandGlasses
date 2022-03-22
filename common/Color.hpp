#pragma once

#include <stdint.h>
#include <string>

namespace common {
class Color {
  public:
    /// Construct from optional RGB values
    Color(double r = 0, double g = 0, double b = 0);
    /// Construct from encoded 32bit integer with the following hexadecimal
    /// form: 0xRRGGBB__
    Color(uint32_t);

    /// Convert to string for printing and debuggin
    std::string toString() const;
    /// @returns Brightness of the color (average of RGB values)
    uint8_t brightness() const;
    /// Add conversion to 32bit integer
    operator uint32_t() const;
    /// @returns R value (non-reference)
    uint8_t r() const;
    /// @returns G value (non-reference)
    uint8_t g() const;
    /// @returns B value (non-reference)
    uint8_t b() const;

    /// @returns Difference between two colors by RGB values
    double difference(const Color &) const;
    /// @returns Converted color to grayscale
    Color grayscale() const;
    /// @returns Color rounded to the closest color. Defaults to black and white
    /// (acts as a black/white filter)
    Color round(const Color &c1 = {0, 0, 0},
                const Color &c2 = {255, 255, 255}) const;
    /// @param scalar If > 1, increases brightness. If < 1, decreases
    /// brightness.
    /// @returns Color with RGB values multiplied by the scalar
    Color operator*(double scalar) const;
    /// @param scalar If < 1, increases brightness. If > 1, decreases
    /// brightness.
    /// @returns Color with RGB values divided by the scalar
    Color operator/(double scalar) const;
    /// @returns Color with summed RGB values
    Color operator+(const Color &) const;
    /// @returns Color with subtracted RGB values
    Color operator-(const Color &) const;
    /// Multiples the color by the scalar
    void operator*=(double scalar);
    /// Divides the color by the scalar
    void operator/=(double scalar);
    /// Adds the provided color to this color
    void operator+=(const Color &);
    /// Subtracts the provided color from this color
    void operator-=(const Color &);

  private:
    uint8_t m_r = 0;
    uint8_t m_g = 0;
    uint8_t m_b = 0;

    /// @returns 8bit integer, given some number. If the number is greater than
    /// 255, it clips and returns 255. If it is less than 0, it clips and
    /// returns 0.
    static uint8_t clip(double);
};
} // namespace common