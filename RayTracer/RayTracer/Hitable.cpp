#include "pch.h"
#include "Hitable.h"

const float EPSILON = 0.000001;

bool Triangle::Raycast(const Ray& ray, OUT HitInfo& hitInfo) const
{
	// Moeller - Trumbore algorithm, as found on Wikipedia (https://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm),
	// with added backface check

	if (hitInfo.ignoreBackFaces && ray.Direction().Dot(normal) > 0.0f)
	{
		return false;
	}

	Vec3 rayXAC = ray.Direction().Cross(edgeAC);
	float a = edgeAB.Dot(rayXAC);
	if (a > -EPSILON && a < EPSILON)
	{
		return false;	// Ray is parallel to the triangle
	}

	float f = 1.0f / a;
	Vec3 s = ray.Origin() - this->a;
	float u = f * s.Dot(rayXAC);

	Vec3 q = s.Cross(edgeAB);
	float v = f * ray.Direction().Dot(q);

	// See Matt Godbolt's talk about how this might
	// matter for branch prediction (CppCon 2019)

	if (u < 0.0f || u > 1.0f || v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	float t = f * edgeAC.Dot(q);

	if (t > EPSILON && t < 1.0f / EPSILON)
	{
		hitInfo.point = ray.At(t);
		hitInfo.normal = normal;

		return true;
	}

	return false;
}

bool Sphere::Raycast(const Ray& ray, OUT HitInfo& hitInfo) const
{
	// if backfaces are to be ignored, and the ray starts closer
	// to the origin than the radius => we're inside the sphere,
	// no need to go on.

	if (hitInfo.ignoreBackFaces && Contains(ray.Origin()))
	{
		return false;
	}

	// 	Sphere equation: x * x + y * y + z * z == R * R
	//	If the sphere is at (cx, cy, cz):
	//	(x-cx)^2 + (y-cy)^2 + (z-cz)^2 == R^2
	//	Conveniently, with vectors:
	//	a vector pointing from the origin (C) to a point (p) on
	//	the sphere's surface is p-C
	//	but dot(p-C, p-C) is exactly (px - cx)^2 + (py - cy)^2 + (pz-cz)^2
	//	
	//  So if (dot(p-C, p-C) == R^2), then p is on the surface.
	//
	//	We have a ray, where p becomes
	//	a + t * b, where a is the ray's origin, and b its direction
	//
	// So then:
	// dot((a + t * b - C), (a + t * b - C)) == R^2
	//
	// expanding this, and taking advantage of the dot product
	// we get the equasion:
	//
	// dot(b,b) * t^2 + 2 * dot(b, a-c) * t + dot(a-c, a-c) - R^2 = 0
	//
	// So this is a simple quadratic equation.

	Vec3 fromOriginToRayStart = ray.Origin() - origin;

	float a = ray.Direction().SqrMagnitude();
	float b = 2.0f * ray.Direction().Dot(fromOriginToRayStart);
	float c = fromOriginToRayStart.SqrMagnitude() - squaredRadius;
	float d = b * b - 4.0f * a * c;

	// When casting a ray from inside, e.g. ray origin == origin:
	// a = 1f;
	// b = 0f;
	// c = -4f;	   (because radius is, say, 2f)
	// d = 20f;		

	float t = -1.0f;

	if (d > 0.0f)
	{
		float sqrt = sqrtf(d);
		float t0 = (-b + sqrt) / (a * 2.0f);
		float t1 = (-b - sqrt) / (a * 2.0f);

		if (t0 > 0.0f && t1 > 0.0f) { t = t0 < t1 ? t0 : t1; }
		else { t = t0 < t1 ? t1 : t0; }

		hitInfo.point = ray.At(t);
		hitInfo.normal = (hitInfo.point - origin).Normalize();
	}

	return t > 0.001f;
}

bool HitableList::Raycast(const Ray& ray, OUT HitInfo& hitInfo) const
{
	float closestHitDistanceSquared = -1.0f;
	HitInfo closestHit;

	for (const auto& hitable : hitables)
	{
		if (hitable->Raycast(ray, OUT hitInfo))
		{
			float dstSquared = (ray.Origin() - hitInfo.point).SqrMagnitude();
			if (closestHitDistanceSquared < 0.0f || closestHitDistanceSquared > dstSquared)
			{
				closestHit = hitInfo;
				closestHitDistanceSquared = dstSquared;
			}
		}
	}

	if (closestHitDistanceSquared > 0.0f)
	{
		hitInfo = closestHit;
		return true;
	}

	return false;
}

HitableList::~HitableList()
{
	for (auto& ptr : hitables)
	{
		delete ptr;
	} 
}

int HitableList::AddSphere(const Vec3& origin, float radius)
{
	hitables.push_back(new Sphere(origin, radius));
	return hitables.size();
}

int HitableList::AddTriangle(const Vec3& a, const Vec3& b, const Vec3& c)
{
	hitables.push_back(new Triangle(a, b, c));
	return hitables.size();
}
