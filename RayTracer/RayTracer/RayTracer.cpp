#include "pch.h"
#include "RayTracer.h"

int main()
{
	Stopwatch s("Main", true);
	OutputFile imageFile("test.ppm");
	PrintSimpleWorldTestTo(320, 200, imageFile.GetStream());
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

void PrintSimpleWorldTestTo(int width, int height, std::ostream& stream)
{
}

void PrintSimpleSphereTestTo(int width, int height, std::ostream & stream)
{
	Vec3 origin;

	float unitWidth = width / 100.0f;
	float unitHeight = height / 100.0f;
	float u = unitWidth / width;
	float v = unitHeight / height;

	Vec3 screenLowerLeft(-(float) unitWidth / 2.0f, -(float) unitHeight / 2.0f, 1.0f);
	Sphere sphere(Vec3(0.0f, 0.5f, 1.0f), 0.5f);

	stream << CreatePPMHeader(width, height);

	HitInfo hit;
	Vec3 hitColor(1.0f, 0.2f, 0.2f);

	const int AA_SAMPLE_COUNT = 32;
	std::random_device randomDevice;
	std::default_random_engine randEngine(randomDevice());
	std::uniform_real_distribution<float> randomDistribution(0.0f, 1.0f);

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			Vec3 pixelColor;

			for (int k = 0; k < AA_SAMPLE_COUNT; ++k)
			{
				float randomDx = randomDistribution(randEngine);
				float randomDy = randomDistribution(randEngine);

				Vec3 pixel = screenLowerLeft + Vec3((i + randomDx) * u, (j + randomDy) * v, 0.0f);
				Vec3 direction = pixel - origin;
				Ray ray(origin, direction);
				bool isHit = sphere.Raycast(ray, OUT hit);
				if (isHit)
				{
					Vec3 normalClamped = hit.normal / 2.0f + Vec3(0.5f, 0.5f, 0.5f);
					pixelColor += Vec3(normalClamped.x(), normalClamped.y(), 1.0f - normalClamped.z());
				}
				else
				{
					pixelColor += Vec3((float)i / (float)width, (float)j / (float)height, 0.2f);
				}
			}

			(pixelColor / (float)AA_SAMPLE_COUNT).PrintRGB(stream);
		}
	}
}

void PrintSimpleTriangleTestTo(int width, int height, std::ostream & stream)
{
	Vec3 origin;

	float unitWidth = width / 100.0f;
	float unitHeight = height / 100.0f;
	float u = unitWidth / width;
	float v = unitHeight / height;

	Vec3 screenLowerLeft(-(float)unitWidth / 2.0f, -(float)unitHeight / 2.0f, 1.0f);
	Triangle tri(Vec3(0.2f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(1.0f, 0.5f, 1.0f));

	stream << CreatePPMHeader(width, height);
	HitInfo hit;
	Vec3 hitColor(1.0f, 0.2f, 0.2f);

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			Vec3 pixelColor;

			Vec3 pixel = screenLowerLeft + Vec3(i * u, j * v, 0.0f);
			Vec3 direction = pixel - origin;
			Ray ray(origin, direction);
			bool isHit = tri.Raycast(ray, OUT hit);
			if (isHit)
			{
				pixelColor = hitColor;
			}
			else
			{
				pixelColor = Vec3((float)i / (float)width, (float)j / (float)height, 0.2f);
			}

			pixelColor.PrintRGB(stream);
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
