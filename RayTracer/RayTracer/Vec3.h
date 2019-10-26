#pragma once
#include <math.h>
#include <iostream>

class Vec3
{
public:
	// Using a left-handed coordinate system, like Unity
	static Vec3 Up() { return Vec3(0.0f, 1.0f, 0.0f); }
	static Vec3 Forward() { return Vec3(0.0f, 0.0f, 1.0f); }
	static Vec3 Right() { return Vec3(1.0f, 0.0f, 0.0f); }
	
	static Vec3 Lerp(const Vec3& a, const Vec3& b, float normalizedValue)
	{
		float oneMinusNormalizedValue = 1.0f - normalizedValue;
		return Vec3(oneMinusNormalizedValue * a.val[0] + normalizedValue * b.val[0],
					oneMinusNormalizedValue * a.val[1] + normalizedValue * b.val[1],
					oneMinusNormalizedValue * a.val[2] + normalizedValue * b.val[2]);
	}

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

	inline Vec3 RGB() const
	{
		return Vec3(val[0] * 255.0f, val[1] * 255.0f, val[2] * 255.0f);
	}

	inline void PrintRGB(std::ostream& stream) const
	{
		stream << (int)(val[0] * 255.0f) << " " << (int)(val[1] * 255.0f) << " " << (int)(val[2] * 255.0f) << "\n";
	}

private:
	float val[3];
};

inline std::ostream& operator<<(std::ostream& stream, const Vec3& v)
{
	stream << v.x() << " " << v.y() << " " << v.z();
	return stream;
}
