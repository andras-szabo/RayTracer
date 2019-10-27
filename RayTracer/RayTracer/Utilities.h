#pragma once
#include <fstream>
#include <string>
#include <chrono>
#include "Ray.h"

class Utilities
{
public:
	static Vec3 GetRandomUnitVector()
	{
		static std::random_device randomDevice;
		static std::default_random_engine randEngine(randomDevice());
		static std::uniform_real_distribution<float> randomDistribution(0.0f, 1.0f);

		float x, y, z, squaredMagnitude;
		do
		{
			x = randomDistribution(randEngine) * 2.0f - 1.0f;
			y = randomDistribution(randEngine) * 2.0f - 1.0f;
			z = randomDistribution(randEngine) * 2.0f - 1.0f;

			squaredMagnitude = x * x + y * y + z * z;
		} while (squaredMagnitude > 1.0f);

		return Vec3(x, y, z);
	}

	static Vec3 Reflect(const Vec3& v, const Vec3& n)
	{
		return v - (n * (v.Dot(n) * 2.0f));
	}
};

class Stopwatch
{
public:
	Stopwatch(const std::string& name, bool logOnDestroy) : name{ name }, logOnDestroy{ logOnDestroy }
	{
		start = std::chrono::system_clock::now();
	}

	~Stopwatch()
	{
		if (logOnDestroy)
		{
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end - start;
			std::cout << name << " - elapsed: " << elapsed_seconds.count() << " s";
		}

	}

private:
	std::string name;
	std::chrono::time_point<std::chrono::system_clock> start;
	bool logOnDestroy;
};

class OutputFile
{
public:
	OutputFile(const std::string& fileName) : name{ fileName }, outputStream{}
	{
		outputStream.open(name);
	}

	~OutputFile()
	{
		outputStream.close();
	}

	const std::string& Name() const { return name; }
	std::ofstream& GetStream() { return outputStream; }

private:
	std::ofstream outputStream;
	std::string name;
};
