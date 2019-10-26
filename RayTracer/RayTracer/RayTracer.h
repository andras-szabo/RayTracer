#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "Ray.h"

std::string CreatePPMHeader(int width, int height);
void PrintRGB(float r, float g, float b, std::ostream& stream);
void PrintColorTestTo(int width, int height, std::ostream& stream);
void PrintSimpleBackgroundTestTo(int width, int height, std::ostream& stream);
float Lerpf(float a, float b, float normalizedValue);
