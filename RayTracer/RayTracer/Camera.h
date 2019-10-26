#pragma once
#include "Ray.h"

// OK so by definition the virtual screen is at z == 1.0f,
// and the camera is at origin (0, 0, 0)

class Camera
{
public:
	Camera(const Vec3& origin, int screenPixelWidth, int screenPixelHeight, float pixelsPerUnit) :
		origin{ origin },
		widthInUnits{ (float)screenPixelWidth / pixelsPerUnit },
		heightInUnits{ (float)screenPixelHeight / pixelsPerUnit }
	{
		screenBottomLeft = Vec3(-widthInUnits / 2.0f, -heightInUnits / 2.0f, 1.0f);
	}

	Ray GetRay(float unitPositionX, float unitPositionY) const
	{
		return Ray(origin, screenBottomLeft + Vec3(unitPositionX * widthInUnits, unitPositionY * heightInUnits, 0.0f) - origin);
	}

private:
	Vec3 origin;
	Vec3 screenBottomLeft;
	float widthInUnits;
	float heightInUnits;
};
