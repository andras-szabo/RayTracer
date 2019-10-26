#pragma once
#include <fstream>
#include <string>
#include <chrono>

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
