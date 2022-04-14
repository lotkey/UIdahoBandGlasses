#include "Color.hpp"

#include <algorithm>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>

namespace common {
Color Color::random() { return {rand() % 256, rand() % 256, rand() % 256}; }

Color Color::fromHSV(double h, double s, double v) {
    h *= 360;
    double c = v * s;
    double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double r = 0;
    double g = 0;
    double b = 0;

    if (h < 60) {
        r = c;
        g = x;
    } else if (h < 120) {
        r = x;
        g = c;
    } else if (h < 180) {
        g = c;
        b = x;
    } else if (h < 240) {
        g = x;
        b = c;
    } else if (h < 300) {
        r = x;
        b = c;
    } else {
        r = c;
        b = x;
    }

    return {(r + m) * 255, (g + m) * 255, (b + m) * 255};
}

Color Color::decode(uint8_t encoded) {
    uint8_t r = (encoded >> 5) & 0b00000111;
    uint8_t g = (encoded >> 2) & 0b00000111;
    uint8_t b = encoded & 0b00000011;
    r = (r / 8.f) * 255;
    g = (g / 8.f) * 255;
    b = (b / 4.f) * 255;
    return Color(r, g, b);
}

Color::Color() : m_r(0), m_g(0), m_b(0) {}

Color::Color(double r, double g, double b) {
    m_r = clip(r);
    m_g = clip(g);
    m_b = clip(b);
}

Color::Color(int r, int g, int b) {
    m_r = clip(r);
    m_g = clip(g);
    m_b = clip(b);
}

Color::Color(double d) {
    m_r = clip(d * 255);
    m_g = clip(d * 255);
    m_b = clip(d * 255);
}

std::string Color::toString() const {
    std::string s = "(" + std::to_string((int)m_r) + ", " +
                    std::to_string((int)m_g) + ", " + std::to_string((int)m_b) +
                    ")";
    return s;
}

double Color::intensity() const {
    double average = m_r + m_g + m_b;
    average /= 3.0;
    return clip(average) / 255.0;
}

uint8_t Color::encode() const {
    uint8_t r = (m_r / 255.f) * 8;
    uint8_t g = (m_g / 255.f) * 8;
    uint8_t b = (m_b / 255.f) * 4;
    uint8_t encoded = r & 0b00000111;
    encoded = (encoded << 3) | (g & 0b00000111);
    encoded = (encoded << 2) | (b & 0b00000011);
    return encoded;
}

double Color::hue() const {
    if (delta() == 0) {
        return 0;
    }

    double hue;
    if (max() == m_r) {
        hue = fmod((double)(m_g - m_b) / delta(), 6);
    } else if (max() == m_g) {
        hue = (double)(m_b - m_r) / delta() + 2;
    } else {
        hue = (double)(m_r - m_g) / delta() + 4;
    }
    hue *= 60;
    return fmod(hue / 360, 360);
}

double Color::saturation() const {
    if (max() == 0) {
        return 0;
    }

    double saturation = delta() / (double)(max() + min());
    return clip(saturation) / 255.0;
}

double Color::value() const { return max() / 255.0; }

Color Color::withSaturation(double s) const {
    if (s > 1) {
        s = 1;
    } else if (s < 0) {
        s = 0;
    }

    return fromHSV(hue(), s, value());
}

Color Color::withHue(double h) const {
    if (h > 1) {
        h = 1;
    } else if (h < 0) {
        h = 0;
    }

    return fromHSV(h, saturation(), value());
}

uint8_t Color::getR() const { return m_r; }

uint8_t Color::getG() const { return m_g; }

uint8_t Color::getB() const { return m_b; }

double Color::difference(const Color &color) const {
    double diffr = abs(m_r - color.getR());
    double diffg = abs(m_g - color.getG());
    double diffb = abs(m_b - color.getB());

    return (diffr + diffg + diffb) / 3.0;
}

Color Color::grayscale() const {
    double value = intensity();
    return {value * 255, value * 255, value * 255};
}

Color Color::invert() const { return {255 - m_r, 255 - m_g, 255 - m_b}; }

#pragma region Operator overloading
Color Color::operator*(double scalar) const {
    return {m_r * scalar, m_g * scalar, m_b * scalar};
}

Color Color::operator*(int scalar) const {
    return {m_r * scalar, m_g * scalar, m_b * scalar};
}

Color Color::operator/(double scalar) const {
    return {m_r / scalar, m_g / scalar, m_b / scalar};
}

Color Color::operator/(int scalar) const {
    return {m_r / scalar, m_g / scalar, m_b / scalar};
}

Color Color::operator+(const Color &color) const {
    return {m_r + color.getR(), m_g + color.getG(), m_b + color.getB()};
}

Color Color::operator-(const Color &color) const {
    return {m_r - color.getR(), m_g - color.getG(), m_b - color.getB()};
}

void Color::operator*=(double scalar) { (*this) = *this * scalar; }

void Color::operator*=(int scalar) { (*this) = *this * scalar; }

void Color::operator/=(double scalar) { (*this) = *this / scalar; }

void Color::operator/=(int scalar) { (*this) = *this / scalar; }

void Color::operator+=(const Color &color) { (*this) = *this + color; }

void Color::operator-=(const Color &color) { (*this) = *this - color; }
#pragma endregion

uint8_t Color::clip(double f) {
    if (f > 255) {
        return 255;
    }
    if (f < 0) {
        return 0;
    }
    return (uint8_t)f;
}

uint8_t Color::max() const { return std::max(m_r, std::max(m_g, m_b)); }

uint8_t Color::min() const { return std::min(m_r, std::min(m_g, m_b)); }

uint8_t Color::delta() const { return max() - min(); }
} // namespace common