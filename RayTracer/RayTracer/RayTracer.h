#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include "Utilities.h"
#include "Ray.h"
#include "Hitable.h"
#include "Camera.h"

std::string CreatePPMHeader(int width, int height);
void PrintRGB(float r, float g, float b, std::ostream& stream);
void PrintColorTestTo(int width, int height, std::ostream& stream);
void PrintSimpleBackgroundTestTo(int width, int height, std::ostream& stream);
void PrintSimpleSphereTestTo(int width, int height, std::ostream& stream);
void PrintSimpleTriangleTestTo(int width, int height, std::ostream& stream);
void PrintSimpleWorldTestTo(int width, int height, std::ostream& stream);
float Lerpf(float a, float b, float normalizedValue);
