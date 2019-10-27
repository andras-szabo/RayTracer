#pragma once
#include "Vec3.h"

class Ray
{
public: 
	Ray() = default;
	Ray(const Vec3& origin, const Vec3& direction)
		: origin{ origin }, direction{ direction } {}

	inline Vec3 At(float distance) const { return origin + direction * distance; }
	inline const Vec3& Origin() const { return origin; }
	inline const Vec3& Direction() const { return direction; }

private:
	Vec3 origin;
	Vec3 direction;

};
