#include "stdafx.h"
#include "CppUnitTest.h"
#include "../RayTracer/RayTracer.h"
#include "../RayTracer/Vec3.h"
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

		TEST_METHOD(Vec3RGBTest)
		{
			auto a = Vec3(1.0f, 0.0f, 0.0f);
			auto b = Vec3(0.5f, 0.0f, 0.1f);

			Assert::IsTrue(a.RGB() == a * 255.0f);
			Assert::IsTrue(b.RGB() == b * 255.0f);
		}

		TEST_METHOD(Vec3AsRgbToStreamTest)
		{
			std::ostringstream stream;
			auto a = Vec3(1.0f, 0.0f, 0.0f);
			a.PrintRGB(stream);
			Assert::AreEqual(std::string("255 0 0\n"), stream.str());
		}


		TEST_METHOD(Vec3CtorTest)
		{
			auto v = Vec3();
			Assert::AreEqual(v.x(), 0.0f);
			Assert::AreEqual(v.y(), 0.0f);
			Assert::AreEqual(v.z(), 0.0f);
		}

		TEST_METHOD(Vec3CtorTestParams)
		{
			auto v = Vec3(0.0f, 1.0f, 2.0f);
			Assert::AreEqual(v.r(), 0.0f);
			Assert::AreEqual(v.g(), 1.0f);
			Assert::AreEqual(v.b(), 2.0f);
		}

		TEST_METHOD(Vec3Equality)
		{
			auto a = Vec3(0.0f, 0.1f, 0.2f);
			auto b = Vec3(0.0f, 0.1f, 0.2f);
			auto c = Vec3(1.0f, 234.0f, 0.0f);

			Assert::IsTrue(a == b);
			Assert::IsTrue(b == a);
			Assert::IsTrue(c != a);

			Assert::IsFalse(a == c);
			Assert::IsFalse(c == b);
		}

		TEST_METHOD(Vec3Add)
		{
			auto a = Vec3(1.0f, 2.0f, 3.0f);
			auto b = Vec3(1.0f, 1.0f, 1.0f);

			auto c = a + b;
			Assert::IsTrue(c == Vec3(2.0f, 3.0f, 4.0f));
		}

		TEST_METHOD(Vec3Sub)
		{
			auto a = Vec3(1.0f, 2.0f, 3.0f);
			auto b = Vec3(1.0f, 1.0f, 1.0f);

			auto c = a - b;
			Assert::IsTrue(c == Vec3(0.0f, 1.0f, 2.0f));
		}

		TEST_METHOD(Vec3ScalarMul)
		{
			auto a = Vec3(1.0f, 2.0f, 3.0f);
			auto b = a * 2.0f;
			auto c = a / 2.0f;

			Assert::IsTrue(b == Vec3(2.0f, 4.0f, 6.0f));
			Assert::IsTrue(c == Vec3(0.5f, 1.0f, 1.5f));
		}

		TEST_METHOD(Vec3Dot)
		{
			auto a = Vec3(1.0f, 2.0f, 3.0f);
			auto b = Vec3(2.0f, -1.0f, 3.0f);

			auto dot = a.Dot(b);
			Assert::IsTrue(dot == 1.0f * 2.0f + 2.0f * -1.0f + 3.0f * 3.0f);
		}

		TEST_METHOD(Vec3Length)
		{
			auto a = Vec3(1.0f, 0.0f, 0.0f);
			Assert::IsTrue(a.Length() == 1.0f);

			auto b = Vec3(2.0f, 3.0f, 4.0f);
			Assert::IsTrue(b.Length() == (float)sqrt(4.0f + 9.0f + 16.0f));
		}

		TEST_METHOD(Vec3SqrMgn)
		{
			auto a = Vec3(1.0f, 0.0f, 0.0f);
			Assert::IsTrue(a.SqrMagnitude() == 1.0f);

			auto b = Vec3(2.0f, 2.0f, 2.0f);
			Assert::IsTrue(b.SqrMagnitude() == 12.0f);
		}

		TEST_METHOD(Vec3Normalize)
		{
			auto a = Vec3(15.0f, 0.0f, 0.0f);
			Assert::IsTrue(a.Normalize() == Vec3(1.0f, 0.0f, 0.0f));

			auto b = Vec3(2.0f, 14.0f, -3.0f);
			auto n = b / b.Length();
			Assert::IsTrue(b.Normalize() == n);

		}
		
	};
}