// Chris McVickar
#pragma once

#include <stdint.h>
#include <string>

namespace common {
class Color {
  public:
    /// @returns A random color
    static Color random();
    /// @param h Hue [0.0-1.0]
    /// @param s Saturation [0.0-1.0]
    /// @param v Value [0.0-1.0]
    static Color fromHSV(double h, double s, double v);
    /// @returns A color from an encoded 8-bit color
    static Color decode(uint8_t);

    Color();
    /// Construct from RGB values
    Color(double r, double g, double b);
    /// Construct from optional RGB values
    Color(int r, int g, int b);
    /// Constructs from brightness [0.0 - 1.0]
    Color(double d);

    /// Convert to string for printing and debugging
    std::string toString() const;
    /// @returns Brightness of the color on a scale of [0.0-1.0]
    double intensity() const;
    /// @returns An 8-bit integer to represent a color
    uint8_t encode() const;

    /// @returns Hue of the color on a scale of [0.0-1.0]
    double hue() const;
    /// @returns Saturation of the color on a scale of [0.0-1.0]
    double saturation() const;
    /// @returns Value of the color on a scale of [0.0-1.0]
    double value() const;
    /// @returns Color with the provided saturation
    /// @param s Saturation in range [0.0-1.0]
    Color withSaturation(double s) const;
    /// @returns Color with the provided hue
    /// @param h Hue in range [0.0-1.0]
    Color withHue(double h) const;

    /// Getters and setters
    uint8_t getR() const;
    uint8_t getG() const;
    uint8_t getB() const;
    void setR(int);
    void setG(int);
    void setB(int);
    uint8_t &operator[](int);

    /// @returns Difference between two colors by RGB values
    double difference(const Color &) const;
    /// @returns Converted color to grayscale
    Color grayscale() const;
    /// @returns Inverted color
    Color invert() const;

#pragma region Operator overloading
    /// @param scalar If > 1, increases intensity. If < 1, decreases
    /// intensity.
    Color operator*(double scalar) const;
    /// @param scalar If > 1, increases intensity. If < 1, decreases
    /// intensity.
    Color operator*(int scalar) const;
    /// @param scalar If < 1, increases intensity. If > 1, decreases
    /// intensity.
    Color operator/(double scalar) const;
    /// @param scalar If < 1, increases intensity. If > 1, decreases
    /// intensity.
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

    /// @returns Max of RGB values
    uint8_t max() const;
    /// @returns Min of RGB values
    uint8_t min() const;
    /// @returns Difference between max and min of RGB values
    uint8_t delta() const;
};
} // namespace common