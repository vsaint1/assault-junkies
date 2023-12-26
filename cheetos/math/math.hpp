#pragma once
#include <cmath>


struct Vector3 {
  float x, y, z;

  constexpr Vector3() : x(0), y(0), z(0) {}
  constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

  constexpr const Vector3 &operator-(const Vector3 &other) const noexcept { return Vector3{x - other.x, y - other.y, z - other.z}; }

  constexpr const Vector3 &operator+(const Vector3 &other) const noexcept { return Vector3{x + other.x, y + other.y, z + other.z}; }

  constexpr const Vector3 &operator/(const float factor) const noexcept { return Vector3{x / factor, y / factor, z / factor}; }

  constexpr const Vector3 &operator*(const float factor) const noexcept { return Vector3{x * factor, y * factor, z * factor}; }

  constexpr const bool operator>(const Vector3 &other) const noexcept { return x > other.x && y > other.y && z > other.z; }

  constexpr const bool operator>=(const Vector3 &other) const noexcept { return x >= other.x && y >= other.y && z >= other.z; }

  constexpr const bool operator<(const Vector3 &other) const noexcept { return x < other.x && y < other.y && z < other.z; }

  constexpr const bool operator<=(const Vector3 &other) const noexcept { return x <= other.x && y <= other.y && z <= other.z; }

  constexpr const bool operator==(const Vector3 &other) const noexcept { return x == other.x && y == other.y && z == other.z; }

  constexpr const bool operator!=(const Vector3 &other) const noexcept { return x != other.x || y != other.y || z != other.z; }

  float distance(const Vector3 &other) const noexcept { return std::sqrtf(std::powf(x - other.x, 2) + std::powf(y - other.y, 2) + std::powf(z - other.z, 2)); }

  bool isZero() const noexcept { return x == 0 && y == 0 && z == 0; }

  Vector3 normalize() const noexcept {
	const auto length = std::sqrtf(x * x + y * y + z * z);
	return Vector3{x / length, y / length, z / length};
  }

};

struct ViewMatrix {
	float matrix[16];
};