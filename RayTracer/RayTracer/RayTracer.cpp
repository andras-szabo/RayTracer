#include "pch.h"
#include "RayTracer.h"
#include <memory>

const int DEFAULT_MAX_DEPTH = 32;
const int DEFAULT_SAMPLE_COUNT = 10;
const int DEFAULT_SAME_SAMPLE_LIMIT = 12;

int sampleCount = DEFAULT_SAMPLE_COUNT;
int maxDepth = DEFAULT_MAX_DEPTH;

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		sampleCount = std::stoi(argv[1]);
	}

	if (argc > 2)
	{
		maxDepth = std::stoi(argv[2]);
	}

	std::cout << "Taking " << sampleCount << " samples per pixel; max depth: " << maxDepth << std::endl;

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

void AddIndices(Mesh* mesh, int a, int b, int c)
{
	mesh->indices.push_back(a);
	mesh->indices.push_back(b);
	mesh->indices.push_back(c);
}

void CreateX(MeshStorage* storage)
{
	auto mesh = storage->Create("X");
	mesh->vertices.push_back(Vec3(0.0f, 0.0f, 0.0f));	// 0
	mesh->vertices.push_back(Vec3(0.4f, 0.8f, 0.0f));	// 1
	mesh->vertices.push_back(Vec3(0.5f, 0.7f, 0.0f));	// 2	
	mesh->vertices.push_back(Vec3(0.1f, 0.0f, 0.0f));	// 3
	mesh->vertices.push_back(Vec3(0.0f, 1.6f, 0.0f));	// 4
	mesh->vertices.push_back(Vec3(0.1f, 1.6f, 0.0f));	// 5
	mesh->vertices.push_back(Vec3(0.5f, 0.9f, 0.0f));	// 6
	mesh->vertices.push_back(Vec3(0.9f, 1.6f, 0.0f));	// 7
	mesh->vertices.push_back(Vec3(1.0f, 1.6f, 0.0f));	// 8
	mesh->vertices.push_back(Vec3(0.6f, 0.8f, 0.0f));	// 9
	mesh->vertices.push_back(Vec3(1.0f, 0.0f, 0.0f));	// 10
	mesh->vertices.push_back(Vec3(0.9f, 0.0f, 0.0f));	// 11

	AddIndices(mesh, 0, 1, 2);	
	AddIndices(mesh, 0, 2, 3);	// left leg
	AddIndices(mesh, 2, 1, 6);
	AddIndices(mesh, 2, 6, 9);	// centre bit
	AddIndices(mesh, 1, 4, 5);
	AddIndices(mesh, 1, 5, 6);  // left arm
	AddIndices(mesh, 6, 7, 8);
	AddIndices(mesh, 6, 8, 9);  // right arm
	AddIndices(mesh, 2, 9, 11);
	AddIndices(mesh, 11, 9, 10); // right leg
}

std::unique_ptr<HitableList> MakeWorld(MaterialStorage* matStorage, MeshStorage* meshStorage)
{
	matStorage->CreateDiffuseMaterial("blue", Vec3(0.3f, 0.3f, 1.0f), 0.9f);
	matStorage->CreateDiffuseMaterial("green", Vec3(0.3f, 1.0f, 0.3f), 0.7f);
	matStorage->CreateDiffuseMaterial("red", Vec3(1.0f, 0.3f, 0.3f), 0.8f);

	matStorage->CreateMetallicMaterial("mirror", Vec3(1.0f, 1.0f, 1.0f));
	matStorage->CreateMetallicMaterial("brass", Vec3(0.8f, 0.6f, 0.5f));

	CreateX(meshStorage);

	auto worldPtr = std::make_unique<HitableList>();

	//worldPtr->AddSphere(Vec3(0.0f, 0.0f, 2.0f), 0.5f, matStorage->Get("mirror"));
	worldPtr->AddSphere(Vec3(0.0f, -100.0f, 2.0f), 99.5f, matStorage->Get("green"));
	worldPtr->AddSphere(Vec3(1.0f, -0.25f, 2.0f), 0.25f, matStorage->Get("red"));
	worldPtr->AddSphere(Vec3(-1.0f, 0.0f, 2.0f), 0.45f, matStorage->Get("brass"));

	worldPtr->AddMesh(meshStorage->Get("X"), matStorage->Get("red"), Vec3(-1.0f, 0.0f, 3.0f));
	//worldPtr->AddTriangle(Vec3(1.0f, 0.0f, 3.0f), Vec3(1.2f, 0.5f, 2.25f), Vec3(1.5f, 0.25f, 2.0f), matStorage->Get("blue"));

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
		p = Vec3(Rand01() * 2.0f - 1.0f,
				 Rand01() * 2.0f - 1.0f,
				 Rand01() * 2.0f - 1.0f);
	} while (p.SqrMagnitude() > 1.0f);

	return p;
}


Vec3 SampleRecursiveWithMaterial(const Ray& ray, HitableList* world, int depth)
{
	static HitInfo hit;
	static Vec3 missVectorVisualA{ 1.0f, 1.0f, 1.0f };
	static Vec3 missVectorVisualB{ 0.5f, 0.7f, 1.0f };

	depth += 1;
	if (depth < maxDepth)
	{
		if (world->Raycast(ray, OUT hit))
		{
			Ray scattered;
			Vec3 attenuation;
			if (hit.materialPtr->DoesScatter(ray, hit, attenuation, scattered))
			{
				return attenuation * SampleRecursiveWithMaterial(scattered, world, depth);
			}
			else
			{
				return Vec3();
			}
		}
	}

	Vec3 unitVector = ray.Direction() / ray.Direction().Length();
	float t = 0.5f * (unitVector.y() + 1.0f);
	return Vec3::Lerp(missVectorVisualA, missVectorVisualB, t);
}

Vec3 SampleRecursive(const Ray& ray, HitableList* world, int depth = 0)
{
	static HitInfo hit;
	static Vec3 missVectorVisualA{ 1.0f, 1.0f, 1.0f };
	static Vec3 missVectorVisualB{ 0.5f, 0.7f, 1.0f };

	depth += 1;
	if (depth < DEFAULT_MAX_DEPTH)
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
	stream << CreatePPMHeader(320, 200);

	auto materials = std::make_unique<MaterialStorage>();
	auto meshes = std::make_unique<MeshStorage>();

	auto world = MakeWorld(materials.get(), meshes.get());
	auto worldPtr = world.get();
	auto camera = MakeCamera(width, height);

	for (int j = height - 1; j >= 0; --j)
	{
		for (int i = 0; i < width; ++i)
		{
			Vec3 pixelColor;
			Vec3 previousSampleColour;
			int sameSampleResultCount = 1;

			int samplesTaken;
			bool shouldContinue = true;

			// If we get back the same colour for "n" samples, then stop
			// shooting and taking samples.

			for (samplesTaken = 0; shouldContinue && samplesTaken < sampleCount; ++samplesTaken)
			{
				float u = float(i + Rand01()) / (float)width;
				float v = float(j + Rand01()) / (float)height;
				auto ray = camera.GetRay(u, v);

				Vec3 sampledColor = SampleRecursiveWithMaterial(ray, worldPtr, 0);

				if (sampledColor == previousSampleColour)
				{
					sameSampleResultCount++;
					if (sameSampleResultCount >= DEFAULT_SAME_SAMPLE_LIMIT)
					{
						shouldContinue = false;
					}
				}
				else
				{
					previousSampleColour = sampledColor;
					sameSampleResultCount = 1;
				}

				pixelColor += sampledColor;
			}

			(pixelColor / (float)samplesTaken).PrintRGB(stream);
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

	DiffuseMaterial material("foo", Vec3(1.0f, 0.5f, 0.5f), 0.8f);

	Vec3 screenLowerLeft(-(float)unitWidth / 2.0f, -(float)unitHeight / 2.0f, 1.0f);
	Sphere sphere(Vec3(0.0f, 0.5f, 1.0f), 0.5f, &material);

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

	DiffuseMaterial mat("foo", Vec3(1.0f, 0.4f, 0.4f), 0.9f);

	Vec3 screenLowerLeft(-(float)unitWidth / 2.0f, -(float)unitHeight / 2.0f, 1.0f);
	Triangle tri(Vec3(0.2f, 0.0f, 1.0f), Vec3(0.0f, 1.0f, 1.0f), Vec3(1.0f, 0.5f, 1.0f), &mat);

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
