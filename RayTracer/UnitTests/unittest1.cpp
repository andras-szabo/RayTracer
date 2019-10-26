#include "stdafx.h"
#include "CppUnitTest.h"
#include "../RayTracer/RayTracer.h"
#include <string>

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
	};
}