#include "pch.h"
#include "RayTracer.h"
#include "Vec3.h"

int main()
{
	PrintTestTo(200, 100, std::cout);
}

void PrintTestTo(int width, int height, std::ostream& stream)
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
