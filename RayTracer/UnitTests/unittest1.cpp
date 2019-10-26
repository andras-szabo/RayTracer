#include "stdafx.h"
#include "CppUnitTest.h"
#include "../RayTracer/RayTracer.h"
#include "../RayTracer/Vec3.h"
#include "../RayTracer/Ray.h"
#include "../RayTracer/Hitable.h"
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

		TEST_METHOD(Vec3LerpTest)
		{
			auto a = Vec3();
			auto b = Vec3(1.0f, 1.0f, 1.0f);

			Assert::IsTrue(Vec3::Lerp(a, b, 0.0f) == a);
			Assert::IsTrue(Vec3::Lerp(a, b, 0.5f) == Vec3(0.5f, 0.5f, 0.5f));
			Assert::IsTrue(Vec3::Lerp(a, b, 1.0f) == b);
		}

		TEST_METHOD(Vec3UnaryMinus)
		{
			Vec3 a(1.0f, -1.0f, 1.0f);
			Vec3 b = -a;

			Assert::IsTrue(b == Vec3(-1.0f, 1.0f, -1.0f));
		}

		TEST_METHOD(Vec3AddShorthand)
		{
			Vec3 a(1.0f, 1.0f, 1.0f);
			a += Vec3(1.0f, 0.0f, -1.0f);

			Assert::IsTrue(a == Vec3(2.0f, 1.0f, 0.0f));
		}

		TEST_METHOD(Vec3Cross)
		{
			Vec3 a(0.0f, 0.0f, 1.0f);
			Vec3 b(0.0f, 1.0f, 0.0f);

			auto aXb = a.Cross(b);
			auto bXa = b.Cross(a);

			Assert::IsFalse(aXb == bXa);
			Assert::IsTrue(aXb.Length() == 1.0f);
			Assert::IsTrue(bXa.Length() == 1.0f);

			Assert::IsTrue(aXb == Vec3(-1.0f, 0.0f, 0.0f));
			Assert::IsTrue(bXa == Vec3(1.0f, 0.0f, 0.0f));
		}

		TEST_METHOD(RayBasicTests)
		{
			auto ray = Ray(Vec3(), Vec3::Up());

			Assert::IsTrue(ray.At(1.0f) == Vec3(0.0f, 1.0f, 0.0f));
			Assert::IsTrue(ray.At(-1.0f) == Vec3(0.0f, -1.0f, 0.0f));
			Assert::IsTrue(ray.At(2.0f) == Vec3(0.0f, 2.0f, 0.0f));
		}

		TEST_METHOD(TriangleBasicTests)
		{
			auto tri = Triangle(Vec3(), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.5f, 0.0f));
			auto ray = Ray(Vec3(0.0f, 0.0f, -5.0f), Vec3::Forward());
			auto hitInfo = HitInfo();
			bool isHit = tri.Raycast(ray, OUT hitInfo);

			Assert::IsTrue(isHit);
			Assert::IsTrue(hitInfo.point == Vec3());
			Assert::IsTrue(hitInfo.normal == -Vec3::Forward());
		}

		TEST_METHOD(TriangleBackfaceTest)
		{
			auto tri = Triangle(Vec3(), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.5f, 0.0f));
			auto ray = Ray(Vec3(0.0f, 0.0f, 5.0f), -Vec3::Forward());
			auto hitInfo = HitInfo();
			bool isHit = tri.Raycast(ray, OUT hitInfo);

			// The ray should hit, but it should hit a backface!

			Assert::IsFalse(isHit);
		}
		
		TEST_METHOD(SphereBasicTests)
		{
			auto s = Sphere(Vec3(), 2.0f);
			Assert::IsTrue(s.Radius() == 2.0f);
			Assert::IsTrue(s.Origin() == Vec3());

			auto r = Ray(Vec3(0.0f, 0.0f, -5.0f), Vec3::Forward());
			auto hitInfo = HitInfo();
			bool isHit = s.Raycast(r, OUT hitInfo);
			Assert::IsTrue(isHit);
			Assert::IsTrue(hitInfo.point == Vec3(0.0f, 0.0f, -2.0f), hitInfo.point.ToWString().c_str());
			Assert::IsTrue(hitInfo.normal == -Vec3::Forward());

			r = Ray(Vec3(0.0f, -5.0f, 0.0f), Vec3::Up());
			isHit = s.Raycast(r, OUT hitInfo);
			Assert::IsTrue(isHit);
			Assert::IsTrue(hitInfo.point == Vec3(0.0f, -2.0f, 0.0f));
			Assert::IsTrue(hitInfo.normal == -Vec3::Up());

			r = Ray(Vec3(0.0f, -5.0f, 0.0f), -Vec3::Up());
			isHit = s.Raycast(r, OUT hitInfo);
			Assert::IsFalse(isHit, hitInfo.point.ToWString().c_str());
		}

		TEST_METHOD(SphereBackfaceTest)
		{
			auto s = Sphere(Vec3(), 2.0f);
			auto r = Ray(Vec3(), Vec3::Forward());
			auto hitInfo = HitInfo();
			bool isHit = s.Raycast(r, OUT hitInfo);

			Assert::IsFalse(isHit);

			hitInfo.ignoreBackFaces = false;

			isHit = s.Raycast(r, OUT hitInfo);

			Assert::IsTrue(isHit);
		}

		TEST_METHOD(HitableLists)
		{
			auto hl = HitableList();

			Sphere s1(Vec3(), 2.0f);
			Sphere s2(Vec3(100.0f, 100.0f, 100.0f), 42.0f);

			hl.Add(&s1);
			hl.Add(&s2);

			Assert::IsTrue(hl.Count() == 2);

			Triangle tri(Vec3(10.0f, 10.0f, 10.0f), Vec3(10.0f, 12.0f, 10.0f), Vec3(14.0f, 8.0f, 14.0f));
			hl.Add(&tri);

			Assert::IsTrue(hl.Count() == 3);

			Ray ray(Vec3(0.0f, 0.0f, -10.0f), Vec3::Forward());
			HitInfo hit;
			bool isHit = hl.Raycast(ray, OUT hit);

			Assert::IsTrue(isHit);

			ray = Ray(Vec3(0.0f, 0.0f, -10.0f), -Vec3::Forward());
			Assert::IsFalse(hl.Raycast(ray, OUT hit));
		}
	};
}