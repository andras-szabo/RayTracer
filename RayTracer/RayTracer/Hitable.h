#pragma once
#include "Ray.h"

#define OUT

class HitInfo
{
public:
	Vec3 point;
};

class AHitable
{
public:
	virtual bool Raycast(const Ray& ray, OUT HitInfo& hitInfo) const = 0;
};

class Sphere : public AHitable
{
public:
	Sphere(const Vec3& origin, float radius) : origin{ origin }, radius{ radius } {}
	const Vec3& Origin() const { return origin; }
	float Radius() const { return radius; }

	bool Raycast(const Ray& ray, OUT HitInfo& hitInfo) const override;
	
private:
	Vec3 origin;
	float radius;
};
