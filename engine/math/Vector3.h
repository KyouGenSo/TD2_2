#pragma once
#include "cmath"

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+=(const Vector3& v) {
		Vector3 result;
		x += v.x;
		y += v.y;
		z += v.z;

		return result;
	}

	Vector3 operator-=(const Vector3& v) {
		Vector3 result;
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return result;
	}

	Vector3 operator*=(float s) {
		Vector3 result;
		x *= s;
		y *= s;
		z *= s;

		return result;
	}

	Vector3 operator/=(float s) {
		Vector3 result;
		x /= s;
		y /= s;
		z /= s;

		return result;
	}

	Vector3 operator+(const Vector3& v) const {
		Vector3 result;
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;

		return result;
	}

	Vector3 operator+(float s) const {
		Vector3 result;
		result.x = x + s;
		result.y = y + s;
		result.z = z + s;

		return result;
	}

	Vector3 operator-(const Vector3& v) const {
		Vector3 result;
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;

		return result;
	}

	Vector3 operator-(float s) const {
		Vector3 result;
		result.x = x - s;
		result.y = y - s;
		result.z = z - s;

		return result;
	}

	Vector3 operator*(float s) const {
		Vector3 result;
		result.x = x * s;
		result.y = y * s;
		result.z = z * s;

		return result;
	}

	Vector3 operator/(float s) const {
		Vector3 result;
		result.x = x / s;
		result.y = y / s;
		result.z = z / s;

		return result;
	}

	Vector3 normalize() const {
		float length = std::sqrt(x * x + y * y + z * z);
		if (length == 0.0f) {
			return { 0.0f, 0.0f, 0.0f };
		}
		return { x / length, y / length, z / length };
	}

	// ベクトルの長さを計算するメソッド
	float Length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	// ドット積を計算するメソッド
	float dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	float LengthSquared() const {
		return x * x + y * y + z * z;
	}
};
