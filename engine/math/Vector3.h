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

	// 正規化　※長さが0の場合は(0, 0, 0)を返す
	Vector3 normalize() {
		float length = sqrt(x * x + y * y + z * z);
		if (length == 0.0f) {
			return { 0.0f, 0.0f, 0.0f };
		}

		x /= length;
		y /= length;
		z /= length;

		return { x, y, z };
	}
};
