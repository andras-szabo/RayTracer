#pragma once
#include "Hitable.h"
#include "Utilities.h"

struct AMaterial
{
public:
	AMaterial(const std::string& name) : name{ name } {}

	virtual bool DoesScatter(const Ray& ray, const HitInfo& hit,
							 OUT Vec3& attenuation,
							 OUT Ray& scatteredRay) const = 0;

	const std::string name;
						
};

struct DiffuseMaterial : public AMaterial
{
public:
	DiffuseMaterial(const std::string& name, const Vec3& a, float diffuseFactor) : AMaterial{ name }, albedo { a }, diffuseFactor{ diffuseFactor } {}
	bool DoesScatter(const Ray& ray, const HitInfo& hit,
					 OUT Vec3& attenuation,
					 OUT Ray& scatteredRay) const override
	{
		Vec3 target = hit.point + hit.normal + Utilities::GetRandomUnitVector();
		scatteredRay = Ray(hit.point, target - hit.point);
		attenuation = albedo * diffuseFactor;
		return true;
	}

	Vec3 albedo;
	float diffuseFactor;
};

struct MetallicMaterial : public AMaterial
{
public:
	MetallicMaterial(const std::string& name, const Vec3& albedo) : AMaterial{ name }, albedo { albedo } {}
	bool DoesScatter(const Ray& ray, const HitInfo& hit,
					 OUT Vec3& attenuation,
					 OUT Ray& scatteredRay) const override
	{
		Vec3 rayNormal = ray.Direction();
		Vec3 reflected = Utilities::Reflect(rayNormal.Normalize(), hit.normal);
		scatteredRay = Ray(hit.point, reflected);
		attenuation = albedo;
		return (reflected.Dot(hit.normal) > 0.0f);
	}

	Vec3 albedo;
};

class MaterialStorage
{
public:
	MaterialStorage()
	{
		diffuse = std::make_unique<DiffuseMaterial>(DiffuseMaterial("def_diffuse", Vec3(1.0f, 0.6f, 0.6f), 0.8f));
		metallic = std::make_unique<MetallicMaterial>(MetallicMaterial("def_metallic", Vec3(0.5f, 0.5f, 0.8f)));
	}

	AMaterial* GetDiffuse() { return diffuse.get(); }
	AMaterial* GetMetallic() { return metallic.get(); }

	void CreateMaterial(const std::string& name, bool diffuse, Vec3 albedo, float diffuseParam)
	{
		if (diffuse) { CreateDiffuseMaterial(name, albedo, diffuseParam); }
		if (!diffuse) { CreateMetallicMaterial(name, albedo); }
	}

	void CreateDiffuseMaterial(const std::string& name, Vec3 albedo, float diffuseParam)
	{
		auto mat = std::make_unique<DiffuseMaterial>(DiffuseMaterial(name, albedo, diffuseParam));
		storage.push_back(std::move(mat));
	}

	void CreateMetallicMaterial(const std::string& name, Vec3 albedo)
	{
		auto mat = std::make_unique<MetallicMaterial>(MetallicMaterial(name, albedo));
		storage.push_back(std::move(mat));
	}

	AMaterial* Get(const std::string& name)
	{
		for (auto& material : storage)
		{
			if (material->name == name)
			{
				return material.get();
			}
		}

		return nullptr;
	}


private:
	std::unique_ptr<AMaterial> diffuse;
	std::unique_ptr<AMaterial> metallic;

	std::vector<std::unique_ptr<AMaterial>> storage;
};
