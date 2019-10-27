#pragma once
#include <vector>
#include "Vec3.h"
#include "Hitable.h";

class Mesh
{
public:
	Mesh(const std::string& name) : name{ name } {}
	std::vector<Triangle> GetTriangles(AMaterial* material) const
	{
		std::vector<Triangle> tris;

		int index = 0;

		while (index < indices.size() - 2)
		{
			Vec3 a = vertices[indices[index]];
			Vec3 b = vertices[indices[index + 1]];
			Vec3 c = vertices[indices[index + 2]];

			tris.push_back(Triangle(a, b, c, material));

			index += 3;
		}

		return tris;
	}

	const std::string name;

	std::vector<Vec3> vertices;
	std::vector<int> indices;
};

class MeshStorage
{
public:
	Mesh* Create(const std::string& name)
	{
		storage.push_back(std::make_unique<Mesh>(name));
		return Get(name);
	}

	Mesh* Get(const std::string& name)
	{
		for (auto& mesh : storage)
		{
			if (mesh->name == name)
			{
				return mesh.get();
			}
		}

		return nullptr;
	}

private:
	std::vector<std::unique_ptr<Mesh>> storage;
};
