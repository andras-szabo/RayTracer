#pragma once
#include "Ray.h"
#include <vector>

#define OUT

enum class HitableType
{
	None,
	Sphere,
	Triangle
};

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
	virtual ~AHitable() = default;

	virtual bool Raycast(const Ray& ray, OUT HitInfo& hitInfo) const = 0;
	HitableType GetType() const { return type; }

protected:
	HitableType type;
};

class Triangle : public AHitable
{
public:
	Triangle(const Vec3& a, const Vec3& b, const Vec3& c) : a{ a }, b{ b }, c{ c }
	{
		type = HitableType::Triangle;

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
	Sphere(const Vec3& origin, float radius) : origin{ origin }, radius{ radius } 
	{ 
		type = HitableType::Sphere;
		squaredRadius = radius * radius; 
	}

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

class HitableList
{
public:
	~HitableList();

	int AddSphere(const Vec3& origin, float radius);
	int AddTriangle(const Vec3& a, const Vec3& b, const Vec3& c);

	int Count() const { return hitables.size(); }
	bool Raycast(const Ray& ray, OUT HitInfo& hit) const;

private:
	std::vector<AHitable*> hitables;
};
