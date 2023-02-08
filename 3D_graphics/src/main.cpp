#include "Render.hpp"


int main()
{
    try
    {
    	Material red(Vec4f(0.6, 0.3, 0.1, 0), Vec3f(1, 0, 0), 50., 1.);
        Material blue(Vec4f(0.9, 0.1, 0.0, 0), Vec3f(0, 1, 1), 10., 1.);
        Material mirror(Vec4f(0.0, 10.0, 0.8, 0), Vec3f(1.0, 1.0, 1.0), 1425., 1.);
        Material glass(Vec4f(0.0, 10.0, 0.8, 0.8), Vec3f(1.0, 1.0, 1.0), 125., 2.);

    	std::vector<Sphere> spheres;
    	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, blue));
        spheres.push_back(Sphere(Vec3f(-7, 6, -16), 2, red));
    	spheres.push_back(Sphere(Vec3f(-1, -1.5, -8), 2, mirror));
        spheres.push_back(Sphere(Vec3f(1, 0, -18), 3, red));
        spheres.push_back(Sphere(Vec3f(0, 5, -22), 4, mirror));
            
    	std::vector<Light> lights;
        lights.push_back(Light(Vec3f(-20, 20,  20), 1.5));
        lights.push_back(Light(Vec3f( 30, 50, -25), 1.8));
        lights.push_back(Light(Vec3f( 30, 20,  30), 1.7));
    
        render(spheres, lights);
    }
    catch(std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}





