#ifndef SPHERE_H
#define SPHERE_H


#include "Vector.hpp"

class Material
{
    Vec4f albedo_;            // albedo[0] - diffuse reflection, albedo[1] - mirror reflection, albedo[2] - reflection coefficient, albedo[3] - refraction
    Vec3f diff_color_;        // diffuse color
    float specular_exponent_; // gloss factor
    float refractive_index_;  

public:

    Material(const Vec4f &albedo, const Vec3f &color, const float specular_exponent, float refractive_index)
    : albedo_(albedo), diff_color_(color), specular_exponent_(specular_exponent), refractive_index_(refractive_index) {};
    Material() : albedo_(1, 0, 0, 0), diff_color_(), specular_exponent_(), refractive_index_(1) {};

    Vec3f diff_color() const { return diff_color_; }
    Vec4f albedo() const { return albedo_; }
    float specular_exponent() const { return specular_exponent_; }
    float refractive_index() const { return refractive_index_; }

};


//-----------------------------------------------------------------------------------------------


class Light
{
    Vec3f position_;
    float intensive_;

public:

    Light(const Vec3f &position, const float intensive) : position_(position), intensive_(intensive) {};

    Vec3f position() const { return position_; }
    float intensive() const { return intensive_; }

};


//-----------------------------------------------------------------------------------------------


class Sphere
{
    Vec3f center_;
    float radius_;
    Material material_;

public:

    Sphere(const Vec3f &center, const float &radius, const Material &material) :
    center_(center), radius_(radius), material_(material) {};

    Vec3f center() const { return center_; }
    Material material() const { return material_; }
    float radius() const { return radius_; }

    bool ray_intersect(const Vec3f &dot, const Vec3f &ray, float &t0) const;

};


//-----------------------------------------SPHERE-------------------------------------------------


bool Sphere::ray_intersect(const Vec3f &dot, const Vec3f &ray, float &t0) const //ray - normalized vector from a point dot , t0 = parameter
{                                                                               //nearest intersect point is x = dot + t0 * ray
    Vec3f dist1 = center_ - dot;
    float proj = dist1 * ray;

    float dist2 = dist1 * dist1 - proj * proj;

    if (dist2 > radius_ * radius_)
    {
        return false;
    }

    float sqr = std::sqrt(radius_ * radius_ - dist2);

    t0 = proj - sqr;
    float t1 = proj + sqr;

    if (t0 < 0)
    {
        t0 = t1;
    }

    if (t0 < 0)
    {
        return false;
    }

    return true;
}




#endif
