#pragma once
#include <cmath>
#include <numbers>

struct Vec4 {
	// constructor
	constexpr Vec4(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f,
		const float w = 0.f) noexcept :
		x(x), y(y), z(z), w(w) { }

	// struct data
	float x, y, z, w;
};

struct Vec3 {

	float x, y, z;

	// constructor
	constexpr Vec3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	// operator overloads
	constexpr const Vec3& operator-(const Vec3& other) const noexcept {
		return Vec3{ x - other.x, y - other.y, z - other.z };
	}

	constexpr const Vec3& operator+(const Vec3& other) const noexcept {
		return Vec3{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vec3& operator/(const float factor) const noexcept {
		return Vec3{ x / factor, y / factor, z / factor };
	}

	constexpr const Vec3& operator*(const float factor) const noexcept {
		return Vec3{ x * factor, y * factor, z * factor };
	}

	// utils
	constexpr const Vec3& ToAngle() const noexcept {
		return Vec3{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f 
		};
	}

	constexpr const Vec3& ToRadians() noexcept {
		return Vec3{
			(x * std::numbers::pi_v<float>) / 180.f,
			(y * std::numbers::pi_v<float>) / 180.f,
			(z * std::numbers::pi_v<float>) / 180.f
		};
	}

	constexpr const Vec3& ToDegrees() noexcept {
		return Vec3{
			x * (180.f / std::numbers::pi_v<float>),
			y * (180.f / std::numbers::pi_v<float>),
			z * (180.f / std::numbers::pi_v<float>)
		};
	}

	constexpr const bool IsZero() const noexcept {
		return x == 0.f && y == 0.f && z == 0.f;
	}

	constexpr const float Dot(const float other) {
		int product = 0;
		product += x * other;
		product += y * other;
		product += z * other;
		return product;
	}

	float SquareRoot() {
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 Normalize() {
		float len = SquareRoot();
		if (len != 0) {
			return Vec3{
				x / len,
				y / len,
				z / len,
			};
		}
		return Vec3{ 0.f, 0.f, 0.f };
	}

	constexpr const Vec3 Copy() {
		return Vec3{
			x, y, z
		};
	}
};