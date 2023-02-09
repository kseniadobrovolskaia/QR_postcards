#ifndef RENDER_H
#define RENDER_H


#include "Raytracer.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int envmap_width, envmap_height;
std::vector<Vec3f> envmap;


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
    const int width_ = 600;
    const int height_ = 400;
    const int fov_ = M_PI / 2.;      //vertical viewing angle

    //Vec3f max_corner, min_corner;
    //duck.get_bbox(min_corner, max_corner);

    std::vector<Vec3f> framebuffer_(width_ * height_);
    float ray_x, ray_y, ray_z;

    Vec3f norm, hit;
    Material material;

    for (size_t i = 0; i < height_; i++)
    {
        for (size_t j = 0; j < width_; j++)
        {
            ray_x =  (j + 0.5) -  width_ / 2.;
            ray_y = -(i + 0.5) + height_ / 2.;
            ray_z = -height_ / (2. * tan(fov_ / 2.));
            Vec3f ray = Vec3f(ray_x, ray_y, ray_z).normalize();

            //std::cout << "текущая точка: (" << ray_x << ", " << ray_y << ", " << ray_z << ")\n";
            framebuffer_[j + i * width_] = cast_ray(Vec3f(0, 0, 0), ray, spheres, lights);
            
            if (framebuffer_[j + i * width_] == Vec3f(0.2, 0.7, 0.8))
            {
                framebuffer_[j + i * width_] = envmap[j + i * width_];
            }
        }
    }

    std::vector<unsigned char> pixmap(width_*height_*3);
    for (size_t i = 0; i < height_*width_; ++i) {
        Vec3f &c = framebuffer_[i];
        float max = std::max(c[0], std::max(c[1], c[2]));
        if (max>1) c = c*(1./max);
        for (size_t j = 0; j<3; j++) {
            pixmap[i*3+j] = (unsigned char)(255 * std::max(0.f, std::min(1.f, framebuffer_[i][j])));
        }
    }
    stbi_write_jpg("out.ppm", width_, height_, 3, pixmap.data(), 100);
    #if 0

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
    #endif
}


#endif
