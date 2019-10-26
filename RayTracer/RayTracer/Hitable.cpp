#include "pch.h"
#include "Hitable.h"

bool Sphere::Raycast(const Ray& ray, OUT HitInfo& hitInfo) const
{
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
	float c = fromOriginToRayStart.SqrMagnitude() - (radius * radius);
	float d = b * b - 4.0f * a * c;

	float t = -1.0f;

	if (d > 0.0f)
	{
		float sqrt = sqrtf(d);
		float t0 = (-b + sqrt) / (a * 2.0f);
		float t1 = (-b - sqrt) / (a * 2.0f);

		t = t0 < t1 ? t0 : t1;

		hitInfo.point = ray.At(t);
		hitInfo.normal = (hitInfo.point - origin).Normalize();
	}

	return t > 0.0f;
}