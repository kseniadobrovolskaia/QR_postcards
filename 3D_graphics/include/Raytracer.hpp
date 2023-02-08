#ifndef RAYTRACER_H
#define RAYTRACER_H


#include "Sphere.hpp"


Vec3f cast_ray(const Vec3f &dot, const Vec3f &ray, const std::vector<Sphere> &spheres, const std::vector<Light> &lights, size_t depth = 0);
bool scene_intersect(const Vec3f &dot, const Vec3f &ray, const std::vector<Sphere> &spheres,
Vec3f &hit, Vec3f &norm, Material &material); // Vec3f norm - normal to the point of intersection with the sphere "hit"
Vec3f reflect(const Vec3f &light_ray, const Vec3f &norm);
Vec3f refract(const Vec3f &light_ray, const Vec3f &norm, const float &refractive_index);


Vec3f cast_ray(const Vec3f &dot, const Vec3f &ray, const std::vector<Sphere> &spheres, const std::vector<Light> &lights, size_t depth)
{
	Vec3f hit, norm;
	Material material;

	if (depth > 3 || !scene_intersect(dot, ray, spheres, hit, norm, material))
	{
		return Vec3f(0.2, 0.7, 0.8);
	}

	Vec3f reflect_ray = reflect(ray, norm).normalize();
	Vec3f reflect_dot = reflect_ray * norm < 0 ? hit - norm * 1e-3 : hit + norm * 1e-3;
	Vec3f reflect_color = cast_ray(reflect_dot, reflect_ray, spheres, lights, depth + 1);

	Vec3f refract_ray = refract(ray, norm, material.refractive_index()).normalize();
	Vec3f refract_dot = refract_ray * norm < 0 ? hit - norm * 1e-3 : hit + norm * 1e-3;
	Vec3f refract_color = cast_ray(refract_dot, refract_ray, spheres, lights, depth + 1);

	float diff_light_intensive = 0, specular_light_intensive = 0, light_dist = 0;
	int count_light = lights.size();
	Vec3f light_ray, shadow_dot, shadow_hit, shadow_norm;
	Material tmp_material;

	for (int light = 0; light < count_light; light++)
	{
		light_ray = (lights[light].position() - hit).normalize();

		light_dist = (lights[light].position() - hit).norm();

		shadow_dot = light_ray * norm < 0 ? hit - norm * 1e-3 : hit + norm * 1e-3;

		if (scene_intersect(shadow_dot, light_ray, spheres, shadow_hit, shadow_norm, tmp_material)
		   && (shadow_hit - lights[light].position()).norm() < light_dist)
		{
			continue;
		}

		diff_light_intensive += lights[light].intensive() * std::max(0.f, light_ray * norm);

		specular_light_intensive += lights[light].intensive() * powf(std::max(0.f, -reflect(-light_ray, norm) * ray), material.specular_exponent());
	}

	return material.diff_color() * diff_light_intensive * material.albedo()[0] + Vec3f(1., 1., 1.) * specular_light_intensive * material.albedo()[1] // reflection white(Vec3f(1., 1., 1.)) color
	+ reflect_color * material.albedo()[2] + refract_color * material.albedo()[3];
}


bool scene_intersect(const Vec3f &dot, const Vec3f &ray, const std::vector<Sphere> &spheres, Vec3f &hit, Vec3f &norm, Material &material)
{
    float dist_i, dist_to_sphere = std::numeric_limits<float>::max();

    for (size_t i=0; i < spheres.size(); i++)
    {
        if (spheres[i].ray_intersect(dot, ray, dist_i) && dist_i < dist_to_sphere)
        {
            dist_to_sphere = dist_i;
            hit = dot + ray * dist_i;
            norm = (hit - spheres[i].center()).normalize();
            material = spheres[i].material();
        }
    }

    return dist_to_sphere != std::numeric_limits<float>::max();
}


Vec3f reflect(const Vec3f &light_ray, const Vec3f &norm)
{
    return light_ray - norm * 2. * (light_ray * norm);
}

Vec3f refract(const Vec3f &light_ray, const Vec3f &norm, const float &refractive_index)
{
	float cos = -std::max(-1.f, std::min(1.f, light_ray * norm));
	float n1 = 1, n2 = refractive_index;
	Vec3f n = norm;

	if (cos < 0)
	{
		cos = -cos;
		std::swap(n1, n2);
		n = -n;
	}

	float div = n1 / n2;
	float k = 1 - div * div * (1 - cos * cos);

	return k < 0 ? Vec3f(0, 0, 0) : light_ray * div + n * (div * cos - sqrtf(k));
}


#endif
