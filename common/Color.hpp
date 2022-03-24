#pragma once

#include <stdint.h>
#include <string>

namespace common {
class Color {
  public:
    static Color random();

    Color();
    /// Construct from RGB values
    Color(double r, double g, double b);
    /// Construct from optional RGB values
    Color(int r, int g, int b);
    /// Construct from encoded 32bit integer with the following hexadecimal
    /// form: 0xRRGGBB__
    Color(uint32_t);

    /// Convert to string for printing and debuggin
    std::string toString() const;
    /// @returns Brightness of the color (average of RGB values)
    uint8_t brightness() const;
    /// Add conversion to 32bit integer
    operator uint32_t() const;

    uint8_t getR() const;
    uint8_t getG() const;
    uint8_t getB() const;
    void setR(int);
    void setG(int);
    void setB(int);

    /// @returns Difference between two colors by RGB values
    double difference(const Color &) const;
    /// @returns Converted color to grayscale
    Color grayscale() const;
    /// @returns Inverted color
    Color invert() const;
    /// @returns Color rounded to the closest color. Defaults to black and white
    /// (acts as a black/white filter)
    Color round(const Color &c1 = {0, 0, 0},
                const Color &c2 = {255, 255, 255}) const;

#pragma region Operator overloading
    /// @param scalar If > 1, increases brightness. If < 1, decreases
    /// brightness.
    Color operator*(double scalar) const;
    /// @param scalar If > 1, increases brightness. If < 1, decreases
    /// brightness.
    Color operator*(int scalar) const;
    /// @param scalar If < 1, increases brightness. If > 1, decreases
    /// brightness.
    Color operator/(double scalar) const;
    /// @param scalar If < 1, increases brightness. If > 1, decreases
    /// brightness.
    Color operator/(int scalar) const;
    /// @returns Color with summed RGB values
    Color operator+(const Color &) const;
    /// @returns Color with subtracted RGB values
    Color operator-(const Color &) const;
    /// Multiples the color by the scalar
    void operator*=(double scalar);
    /// Multiples the color by the scalar
    void operator*=(int scalar);
    /// Divides the color by the scalar
    void operator/=(double scalar);
    /// Divides the color by the scalar
    void operator/=(int scalar);
    /// Adds the provided color to this color
    void operator+=(const Color &);
    /// Subtracts the provided color from this color
    void operator-=(const Color &);
#pragma endregion

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