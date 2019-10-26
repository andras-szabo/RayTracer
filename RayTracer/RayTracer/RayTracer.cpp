#include "pch.h"
#include "RayTracer.h"

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
			float r = (float)i / (float)width;
			float g = (float)j / (float)height;
			float b = 0.2f;
			PrintRGB(r, g, b, stream);
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
