#pragma once
#include <algorithm>
#include <cmath>

struct ViewMatrix {
  float matrix[16];
};

struct Vector3 {
  float x, y, z;
  // constructor
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

  float distance(const Vector3 &other) const noexcept {
    const Vector3 diff = *this - other;

    return std::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
  }

  constexpr const bool is_zero() const noexcept { return x == 0.f && y == 0.f && z == 0.f; }

  constexpr const bool invalid() const noexcept { return !x || !y || !z; }

};
