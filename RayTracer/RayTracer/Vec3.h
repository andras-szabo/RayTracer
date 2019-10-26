#pragma once
#include <math.h>

class Vec3
{
public:
	Vec3() : val{ 0.0f, 0.0f, 0.0f } {}
	Vec3(float x, float y, float z) : val{ x, y, z } {}
	
	inline float x() const { return val[0]; }
	inline float y() const { return val[1]; }
	inline float z() const { return val[2]; }

	inline float r() const { return val[0]; }
	inline float g() const { return val[1]; }
	inline float b() const { return val[2]; }

	inline bool operator==(const Vec3& other) const
	{
		return	other.val[0] == val[0] &&
				other.val[1] == val[1] &&
				other.val[2] == val[2];
	}

	inline bool operator!=(const Vec3& other) const
	{
		return	other.val[0] != val[0] ||
				other.val[1] != val[1] ||
				other.val[2] != val[2];
	}

	inline Vec3 operator+(const Vec3& other) const
	{
		return Vec3(val[0] + other.val[0], val[1] + other.val[1], val[2] + other.val[2]);
	}

	inline Vec3 operator-(const Vec3& other) const
	{
		return Vec3(val[0] - other.val[0], val[1] - other.val[1], val[2] - other.val[2]);
	}

	inline Vec3 operator*(float scalar) const
	{
		return Vec3(val[0] * scalar, val[1] * scalar, val[2] * scalar);
	}

	inline Vec3 operator/(float scalar) const
	{
		return Vec3(val[0] / scalar, val[1] / scalar, val[2] / scalar);
	}

	inline float Dot(const Vec3& other) const
	{
		return val[0] * other.val[0] + val[1] * other.val[1] + val[2] * other.val[2];
	}

	inline float Length() const
	{
		return sqrtf(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]);
	}

	inline float SqrMagnitude() const
	{
		return val[0] * val[0] + val[1] * val[1] + val[2] * val[2];
	}

	inline Vec3& Normalize()
	{
		float len = Length();

		val[0] /= len;
		val[1] /= len;
		val[2] /= len;

		return *this;
	}

private:
	float val[3];
};
