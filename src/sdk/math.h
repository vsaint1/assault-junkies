#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

constexpr const float PI = 3.14159265358979323846f;

struct ViewMatrix {
    float matrix[16];

    std::ostringstream Debug() {
        std::ostringstream oss;
        oss << "ViewMatrix: [";
        for (int i = 0; i < 16; ++i) {
            oss << std::fixed << std::setprecision(2) << matrix[i];
            if (i < 15)
                oss << ", ";
        }
        oss << "]";
        return oss;
    }
};

struct Vector4 {
    float x, y, z, w;
};

struct Vector2 {
    float x, y;

    constexpr const bool IsZero() const noexcept { return x == 0.f && y == 0.f; }

    constexpr const bool Invalid() const noexcept { return !x || !y; }
};

struct Vector3 {
    float x, y, z;

    constexpr Vector3(const float x = 0.f, const float y = 0.f, const float z = 0.f) noexcept : x(x), y(y), z(z) {}

    // operator overloads
    constexpr const Vector3 &operator-(const Vector3 &other) const noexcept { return Vector3{x - other.x, y - other.y, z - other.z}; }

    constexpr const Vector3 &operator+(const Vector3 &other) const noexcept { return Vector3{x + other.x, y + other.y, z + other.z}; }

    constexpr const Vector3 &operator/(const float factor) const noexcept { return Vector3{x / factor, y / factor, z / factor}; }

    constexpr const Vector3 &operator*(const float factor) const noexcept { return Vector3{x * factor, y * factor, z * factor}; }

    constexpr const bool operator>(const Vector3 &other) const noexcept { return x > other.x && y > other.y && z > other.z; }

    constexpr const bool operator>=(const Vector3 &other) const noexcept { return x >= other.x && y >= other.y && z >= other.z; }

    constexpr const bool operator<(const Vector3 &other) const noexcept { return x < other.x && y < other.y && z < other.z; }

    constexpr const bool operator<=(const Vector3 &other) const noexcept { return x <= other.x && y <= other.y && z <= other.z; }

    constexpr const bool operator==(const Vector3 &other) const noexcept { return x == other.x && y == other.y && z == other.z; }

    float Distance(const Vector3 &other) const noexcept {
        const Vector3 diff = *this - other;

        return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    }

    constexpr const bool IsZero() const noexcept { return x == 0.f && y == 0.f && z == 0.f; }

    constexpr const bool Invalid() const noexcept { return !x || !y || !z; }

    Vector2 WorldToScreen(ViewMatrix view, int wW, int wH) {

        Vector4 clip;

        clip.x = this->x * view.matrix[0] + this->y * view.matrix[4] + this->z * view.matrix[8] + view.matrix[12];
        clip.y = this->x * view.matrix[1] + this->y * view.matrix[5] + this->z * view.matrix[9] + view.matrix[13];
        clip.z = this->x * view.matrix[2] + this->y * view.matrix[6] + this->z * view.matrix[10] + view.matrix[14];
        clip.w = this->x * view.matrix[3] + this->y * view.matrix[7] + this->z * view.matrix[11] + view.matrix[15];

        if (clip.w < 0.1f) {
            return {-1, -1};
        }

        Vector3 NDC;
        NDC.x = clip.x / clip.w;
        NDC.y = clip.y / clip.w;
        NDC.z = clip.z / clip.w;

        Vector2 screen;
        screen.x = (wW / 2 * NDC.x) + (NDC.x + wW / 2);
        screen.y = -(wH / 2 * NDC.y) + (NDC.y + wH / 2);

        return screen;
    }
};

#endif // MATH_H
