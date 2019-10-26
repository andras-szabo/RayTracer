#include "pch.h"
#include <iostream>
#include <sstream>
#include "RayTracer.h"

int main()
{
    std::cout << "Hello World!\n"; 
}

std::string CreatePPMHeader(int width, int height)
{
	std::ostringstream headerStream;
	headerStream << "P3\n" << width << " " << height << "\n255\n";
	return headerStream.str();
}
