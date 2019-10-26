#include "pch.h"
#include "RayTracer.h"

int main()
{
	PrintSimpleBackgroundTestTo(200, 100, std::cout);
}

float Lerpf(float a, float b, float normalizedValue)
{
	return (1.0f - normalizedValue) * a + normalizedValue * b;
}

void PrintSimpleBackgroundTestTo(int width, int height, std::ostream& stream)
{
	Vec3 origin;
	Vec3 screenLowerLeft(-(float)width / 2.0f, -(float)height / 2.0f, 1.0f);

	stream << CreatePPMHeader(width, height);
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Vec3 pixel = screenLowerLeft + Vec3(i, j, 0.0f);
			Vec3 normalDirection = (pixel - origin).Normalize();
			Vec3 color = normalDirection / 2.0f + Vec3(0.5f, 0.5f, 0.5f);
			color.PrintRGB(stream);
		}
	}

}

void PrintColorTestTo(int width, int height, std::ostream& stream)
{
	stream << CreatePPMHeader(width, height);
	
	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			Vec3 color((float)i / (float)width, (float)j / (float)height, 0.2f);
			color.PrintRGB(stream);
		}
	}
}

std::string CreatePPMHeader(int width, int height)
{
	std::ostringstream headerStream;
	headerStream << "P3\n" << width << " " << height << "\n255\n";
	return headerStream.str();
}

void PrintRGB(float r, float g, float b, std::ostream& stream)
{
	stream << (int)(255.0f * r) << " " << (int)(255.0f * g) << " " << (int)(255.0f * b) << "\n";
}
