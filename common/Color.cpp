#include "Color.hpp"

#include <math.h>
#include <stdint.h>
#include <string>

namespace common {
Color::Color(double r, double g, double b) {
    m_r = clip(r);
    m_g = clip(g);
    m_b = clip(b);
}

Color::Color(uint32_t color)
    : m_r((color & 0xFF000000) >> 24), m_g((color & 0xFF0000) >> 16),
      m_b((color & 0xFF00) >> 8) {}

std::string Color::toString() const {
    std::string s = "(" + std::to_string((int)m_r) + ", " +
                    std::to_string((int)m_g) + ", " + std::to_string((int)m_b) +
                    ")";
    return s;
}

uint8_t Color::brightness() const {
    double average = m_r + m_g + m_b;
    average /= 3.0;
    return clip(average);
}

Color::operator uint32_t() const {
    return (uint32_t)m_r << 24 | (uint32_t)m_g << 16 | (uint32_t)m_b << 8 | 0;
}

uint8_t Color::r() const { return m_r; }

uint8_t Color::g() const { return m_g; }

uint8_t Color::b() const { return m_b; }

double Color::difference(const Color &color) const {
    double diffr = abs(m_r - color.r());
    double diffg = abs(m_g - color.g());
    double diffb = abs(m_b - color.b());

    return (diffr + diffg + diffb) / 3.0;
}

Color Color::grayscale() const {
    uint8_t value = brightness();
    return {value, value, value};
}

Color Color::round(const Color &c1, const Color &c2) const {
    double diff1 = difference(c1);
    double diff2 = difference(c2);

    if (diff1 >= diff2) {
        return c1;
    } else {
        return c2;
    }
}

Color Color::operator*(double scalar) const {
    return {m_r * scalar, m_g * scalar, m_b * scalar};
}

Color Color::operator/(double scalar) const {
    return {m_r / scalar, m_g / scalar, m_b / scalar};
}

Color Color::operator+(const Color &color) const {
    return {m_r + color.r(), m_g + color.g(), m_b + color.b()};
}

Color Color::operator-(const Color &color) const {
    return {m_r - color.r(), m_g - color.g(), m_b - color.b()};
}

void Color::operator*=(double scalar) { (*this) = *this * scalar; }

void Color::operator/=(double scalar) { (*this) = *this / scalar; }

void Color::operator+=(const Color &color) { (*this) = *this + color; }

void Color::operator-=(const Color &color) { (*this) = *this - color; }

uint8_t Color::clip(double f) {
    if (f > 255) {
        return 255;
    }
    if (f < 0) {
        return 0;
    }
    return (uint8_t)f;
}
} // namespace common