#pragma once
#include <iostream>
#include <string>
#include <sstream>

std::string CreatePPMHeader(int width, int height);
void PrintRGB(float r, float g, float b, std::ostream& stream);
