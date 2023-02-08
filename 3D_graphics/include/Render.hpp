#ifndef RENDER_H
#define RENDER_H


#include "Raytracer.hpp"


class Render
{
    const size_t width_;
    const size_t height_;
    const float fov_;      //vertical viewing angle
    std::vector<Vec3f> framebuffer_;

public:

    Render(const size_t width = 1024, const size_t height = 768, const float fov = 1.05);

    void fill_file(const std::string name_file);
    void render(const std::vector<Sphere> &spheres, const std::vector<Light> &lights);

    int width() const { return width_; }
    int height() const { return height_; }
    float fov() const { return fov_; }

};

Render::Render(const size_t width, const size_t height, const float fov) : width_(width), height_(height), fov_(fov), framebuffer_(width * height)
{
  
}


void render(const std::vector<Sphere> &spheres, const std::vector<Light> &lights)
{
    const int width_ = 1024;
    const int height_ = 768;
    const int fov_ = M_PI / 2.;      //vertical viewing angle

    std::vector<Vec3f> framebuffer_(width_ * height_);
    float ray_x, ray_y, ray_z;

    for (size_t pix = 0; pix < height_ * width_; pix++)
    {
        ray_x =  (pix % width_ + 0.5) -  width_ / 2.;
        ray_y = -(pix / width_ + 0.5) + height_ / 2.;
        ray_z = -height_ / (2. * tan(fov_ / 2.));
        Vec3f ray = Vec3f(ray_x, ray_y, ray_z).normalize();
        framebuffer_[pix] = cast_ray(Vec3f(0, 0, 0), ray, spheres, lights);
    }

    std::ofstream ofs;
    ofs.open("out.ppm");

    ofs << "P6\n" << width_ << " " << height_ << "\n255\n";

    for (size_t pix = 0; pix < height_ * width_; ++pix)
    {
        Vec3f &pixel = framebuffer_[pix];
        float max = std::max(pixel[0], std::max(pixel[1], pixel[2]));
        
        if (max > 1)
        {
            pixel = pixel * (1. / max);
        }

        for (size_t coord = 0; coord < 3; ++coord)
        {
            ofs << static_cast<char>(255 * framebuffer_[pix][coord]);
        }
    }

    ofs.close();
}


#endif
