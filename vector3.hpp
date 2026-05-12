#pragma once
#include <cmath>
#include <iostream>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0.f), y(0.f), z(0.f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vector3 operator+(const Vector3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vector3 operator-(const Vector3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Vector3 operator*(float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    Vector3 operator/(float scalar) const { return {x / scalar, y / scalar, z / scalar}; }

    Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

    float Length() const { return std::sqrt(x * x + y * y + z * z); }
    float Distance(const Vector3& other) const { return (*this - other).Length(); }

    Vector3 Normalize() const {
        float len = Length();
        return (len > 0) ? *this / len : Vector3(0, 0, 0);
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vector3& obj) {
    out << "[" << obj.x << ", " << obj.y << ", " << obj.z << "]";
    return out;
}