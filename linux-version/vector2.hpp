#pragma once
#include <cmath>
#include <iostream>

class Vector2 {
    public:
        float x, y;

        Vector2() : x(0.f), y(0.f) {}
        Vector2(float _x, float _y) : x(_x), y(_y) {}

        Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
        Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
        Vector2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
        Vector2 operator/(float scalar) const { return {x / scalar, y / scalar}; }

        Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y;return *this; }
        Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y;return *this; }

        float Length() const { return std::sqrt(x * x + y * y); }
        float Distance(const Vector2& other) const { return (*this - other).Length(); }

        Vector2 Normalize() const {
            float len = Length();
            return (len > 0) ? *this / len : Vector2(0, 0);
        }
};

inline std::ostream& operator<<(std::ostream& out, const Vector2& obj) {
    out << "[" << obj.x << ", " << obj.y << "]";
    return out;
}