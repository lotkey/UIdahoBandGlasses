#pragma once

#include <stdint.h>
#include <string>

namespace common {
class Color {
  public:
    Color(float r = 0, float g = 0, float b = 0);
    Color(uint32_t);
    ~Color();

    std::string toString() const;
    uint8_t brightness() const;
    operator uint32_t() const;
    uint8_t r() const;
    uint8_t g() const;
    uint8_t b() const;

    float difference(const Color &) const;
    Color grayscale() const;
    Color round(const Color &c1 = {0, 0, 0}, const Color &c2 = {255, 255, 255});
    Color operator*(float scalar) const;
    Color operator/(float scalar) const;
    Color operator+(const Color &) const;
    Color operator-(const Color &) const;
    void operator*=(float scalar);
    void operator/=(float scalar);
    void operator+=(const Color &);
    void operator-=(const Color &);

  private:
    uint8_t m_r = 0;
    uint8_t m_g = 0;
    uint8_t m_b = 0;

    static uint8_t clip(float);
};
} // namespace common