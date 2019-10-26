#include "pch.h"
#include "RayTracer.h"
#include <memory>

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
			Vec3 pixel = screenLowerLeft + Vec3((float)i, (float)j, 0.0f);
			Vec3 normalDirection = (pixel - origin).Normalize();
			Vec3 color = normalDirection / 2.0f + Vec3(0.5f, 0.5f, 0.5f);
			color.PrintRGB(stream);
		}
	}
}

std::unique_ptr<HitableList> MakeWorld()
{
	auto worldPtr = std::make_unique<HitableList>();

	worldPtr->AddSphere(Vec3(0.0f, 0.0f, 2.0f), 0.5f);
	worldPtr->AddSphere(Vec3(0.0f, -101.0f, 2.0f), 100.0f);

	return worldPtr;
}

Camera MakeCamera(int width, int height)
{
	return Camera(Vec3(), width, height, 200.0f);
}

float Rand01()
{
	static std::random_device randomDevice;
	static std::default_random_engine randEngine(randomDevice());
	static std::uniform_real_distribution<float> randomDistribution(0.0f, 1.0f);

	return randomDistribution(randEngine);
}

Vec3 GetRandomUnitVector()
{
	Vec3 p;

	do
	{
		p = Vec3(Rand01() - 1.0f, Rand01() - 1.0f, Rand01() - 1.0f) * 2.0f;
	} while (p.SqrMagnitude() >= 1.0f);

	return p;
}

const int MAX_DEPTH = 100;

Vec3 SampleRecursive(const Ray& ray, HitableList* world, int depth = 0)
{
	static HitInfo hit;
	static Vec3 missVectorVisualA{ 1.0f, 1.0f, 1.0f };
	static Vec3 missVectorVisualB{ 0.5f, 0.7f, 1.0f };

	depth += 1;
	if (depth < MAX_DEPTH)
	{
		if (world->Raycast(ray, OUT hit))
		{
			Vec3 target = hit.point + hit.normal + GetRandomUnitVector();
			return SampleRecursive(Ray(hit.point, target - hit.point), world, depth) * 0.5f;
		}
	}

	Vec3 unitVector = ray.Direction() / ray.Direction().Length();
	float t = 0.5f * (unitVector.y() + 1.0f);
	return Vec3::Lerp(missVectorVisualA, missVectorVisualB, t);
}

Vec3 Sample(const Ray& ray, HitableList* world)
{
	static HitInfo hit;
	static Vec3 missVectorVisualA{ 1.0f, 1.0f, 1.0f };
	static Vec3 missVectorVisualB{ 0.5f, 0.7f, 1.0f };

	if (world->Raycast(ray, OUT hit))
	{
		Vec3 normalClamped = hit.normal / 2.0f + Vec3(0.5f, 0.5f, 0.5f);
		return Vec3(normalClamped.x(), normalClamped.y(), 1.0f - normalClamped.z());
	}
	else
	{
		Vec3 unitVector = ray.Direction() / ray.Direction().Length();
		float t = 0.5f * (unitVector.y() + 1.0f);
		return Vec3::Lerp(missVectorVisualA, missVectorVisualB, t);
	}
}

void PrintSimpleWorldTestTo(int width, int height, std::ostream& stream)
{
	const int sampleCount = 320;

	stream << CreatePPMHeader(320, 200);

	auto world = MakeWorld();
	auto worldPtr = world.get();
	auto camera = MakeCamera(width, height);

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			Vec3 pixelColor;
			for (int k = 0; k < sampleCount; ++k)
			{
				float u = float(i + Rand01()) / (float)width;
				float v = float(j + Rand01()) / (float)height;
				auto ray = camera.GetRay(u, v);
				//pixelColor += Sample(ray, worldPtr);
				pixelColor += SampleRecursive(ray, worldPtr);
			}
			(pixelColor / (float)sampleCount).PrintRGB(stream);
		}
	}
}

void PrintSimpleSphereTestTo(int width, int height, std::ostream & stream)
{
	Vec3 origin;

	float unitWidth = width / 100.0f;
	float unitHeight = height / 100.0f;
	float u = unitWidth / width;
	float v = unitHeight / height;

	Vec3 screenLowerLeft(-(float)unitWidth / 2.0f, -(float)unitHeight / 2.0f, 1.0f);
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
