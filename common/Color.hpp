/// @author @lotkey Chris McVickar
#pragma once

#include <cstdint>

namespace common {
class Color {
public:
  Color() = default;
  Color(Color const&) = default;
  Color(Color&&) = default;
  Color& operator=(Color const&) = default;
  Color& operator=(Color&&) = default;
  virtual ~Color() = default;

  /// @returns A random color
  static Color random();
  /// @param h Hue [0.0-1.0]
  /// @param s Saturation [0.0-1.0]
  /// @param v Value [0.0-1.0]
  static Color fromHSV(double h, double s, double v);
  /// @returns A color from an encoded 8-bit color
  static Color decode(std::uint8_t);
  /// @param[in] encoded_color Color encoded to 8bit integer (RRRGGGBB)
  /// @param[out] r Reference to 8bit integer to write the R value into
  /// @param[out] g Reference to 8bit integer to write the G value into
  /// @param[out] b Reference to 8bit integer to write the B value into
  static void decode(std::uint8_t encoded_color,
                     std::uint8_t& r,
                     std::uint8_t& g,
                     std::uint8_t& b);

  /// Construct from RGB values
  Color(double r, double g, double b);
  /// Construct from optional RGB values
  Color(int r, int g, int b);
  /// Constructs from brightness [0.0 - 1.0]
  Color(double d);

  /// @returns Brightness of the color on a scale of [0.0-1.0]
  double intensity() const;
  /// @returns An 8-bit integer to represent a color
  std::uint8_t encode() const;

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
  std::uint8_t getR() const;
  std::uint8_t getG() const;
  std::uint8_t getB() const;
  void setR(int);
  void setG(int);
  void setB(int);
  std::uint8_t& operator[](int);

  /// @returns Difference between two colors by RGB values
  double difference(Color const&) const;
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
  Color operator+(Color const&) const;
  /// @returns Color with subtracted RGB values
  Color operator-(Color const&) const;
  /// Multiples the color by the scalar
  void operator*=(double scalar);
  /// Multiples the color by the scalar
  void operator*=(int scalar);
  /// Divides the color by the scalar
  void operator/=(double scalar);
  /// Divides the color by the scalar
  void operator/=(int scalar);
  /// Adds the provided color to this color
  void operator+=(Color const&);
  /// Subtracts the provided color from this color
  void operator-=(Color const&);
#pragma endregion

private:
  std::uint8_t m_r = 0;
  std::uint8_t m_g = 0;
  std::uint8_t m_b = 0;

  /// @returns 8bit integer, given some number. If the number is greater than
  /// 255, it clips and returns 255. If it is less than 0, it clips and
  /// returns 0.
  static std::uint8_t clip(double);

  /// @returns Max of RGB values
  std::uint8_t max() const;
  /// @returns Min of RGB values
  std::uint8_t min() const;
  /// @returns Difference between max and min of RGB values
  std::uint8_t delta() const;
};
} // namespace common