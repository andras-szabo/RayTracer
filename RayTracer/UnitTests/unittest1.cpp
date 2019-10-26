#include "stdafx.h"
#include "CppUnitTest.h"
#include "../RayTracer/RayTracer.h"
#include <string>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(CreatePPMHeaderTest)
		{
			auto header = CreatePPMHeader(200, 100);
			Assert::AreEqual(header, std::string("P3\n200 100\n255\n"));
		}

		TEST_METHOD(PrintRGBTest)
		{
			std::ostringstream stream;
			PrintRGB(0.0f, 1.0f, 1.0f, stream);
			Assert::AreEqual(stream.str(), std::string("0 255 255\n"));

			PrintRGB(0.5f, 0.0f, 0.1f, stream);
			Assert::AreEqual(stream.str(), std::string("0 255 255\n127 0 25\n"));
		}
	};
}