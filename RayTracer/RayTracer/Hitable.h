#pragma once
#include "Ray.h"

#define OUT

class HitInfo
{
public:
	Vec3 point;
	Vec3 normal;
	bool ignoreBackFaces = true;
};

class AHitable
{
public:
	virtual bool Raycast(const Ray& ray, OUT HitInfo& hitInfo) const = 0;
};

class Triangle : public AHitable
{
public:
	Triangle(const Vec3& a, const Vec3& b, const Vec3& c) : a{ a }, b{ b }, c{ c } 
	{
		edgeAB = b - a;
		edgeAC = c - a;
		normal = edgeAB.Cross(edgeAC).Normalize();
	}

	const Vec3& A() const noexcept { return a; }
	const Vec3& B() const noexcept { return b; }
	const Vec3& C() const noexcept { return c; }

	bool Raycast(const Ray& ray, OUT HitInfo& hitInfo) const override;

private:
	Vec3 a;
	Vec3 b;
	Vec3 c;

	Vec3 edgeAB;
	Vec3 edgeAC;

	Vec3 normal;
};

class Sphere : public AHitable
{
public:
	Sphere(const Vec3& origin, float radius) : origin{ origin }, radius{ radius } { squaredRadius = radius * radius; }
	const Vec3& Origin() const { return origin; }
	float Radius() const { return radius; }

	bool Raycast(const Ray& ray, OUT HitInfo& hitInfo) const override;
	bool Contains(const Vec3& point) const
	{
		return (point - origin).SqrMagnitude() <= squaredRadius;
	}
	
private:
	Vec3 origin;
	float radius;
	float squaredRadius;
};
