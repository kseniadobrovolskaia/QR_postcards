#ifndef __MODEL_H__
#define __MODEL_H__

#include "Sphere.hpp"
#include <cassert>

class Model {
private:
    std::vector<Vec3f> verts;
    std::vector<Vec3i> faces;
    Material material_;
public:
    Model(const char *filename, Material material);

    int nverts() const;                          // number of vertices
    int nfaces() const;                          // number of triangles

    bool ray_triangle_intersect(const int &fi, const Vec3f &orig, const Vec3f &dir, float &tnear) const;
    bool model_intersect(const Vec3f &orig, const Vec3f &dir, Vec3f &hit, Vec3f &norm, Material &material) const;

    const Vec3f &point(int i) const;                   // coordinates of the vertex i
    Vec3f &point(int i);                   // coordinates of the vertex i
    int vert(int fi, int li) const;              // index of the vertex for the triangle fi and local index li
    void get_bbox(Vec3f &min, Vec3f &max); // bounding box for all the vertices, including isolated ones
};

std::ostream& operator<<(std::ostream& out, Model &m);


// fills verts and faces arrays, supposes .obj file to have "f " entries without slashes
Model::Model(const char *filename, Material material) : verts(), faces(), material_(material) {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            Vec3i f;
            int idx, cnt=0;
            iss >> trash;
            while (iss >> idx) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f[cnt++] = idx;
            }
            if (3==cnt) faces.push_back(f);
        }
    }
    std::cerr << "# v# " << verts.size() << " f# "  << faces.size() << std::endl;

    Vec3f min, max;
    get_bbox(min, max);
}

// Moller and Trumbore
bool Model::ray_triangle_intersect(const int &fi, const Vec3f &orig, const Vec3f &dir, float &tnear) const {
    Vec3f edge1 = point(vert(fi,1)) - point(vert(fi,0));
    Vec3f edge2 = point(vert(fi,2)) - point(vert(fi,0));
    Vec3f pvec = cross(dir, edge2);
    float det = edge1*pvec;
    if (det<1e-5) return false;

    Vec3f tvec = orig - point(vert(fi,0));
    float u = tvec*pvec;
    if (u < 0 || u > det) return false;

    Vec3f qvec = cross(tvec, edge1);
    float v = dir*qvec;
    if (v < 0 || u + v > det) return false;

    tnear = edge2*qvec * (1./det);
    return tnear>1e-5;
}


bool Model::model_intersect(const Vec3f &orig, const Vec3f &dir, Vec3f &hit, Vec3f &norm, Material &material) const
{
    float tnear, tnear_min = std::numeric_limits<float>::max();

    for (size_t i=0; i < faces.size(); i++)
    {
        if (ray_triangle_intersect(i, orig, dir, tnear) && (tnear < tnear_min))
        {
            tnear_min = tnear;
            hit = orig + dir * tnear;

            Vec3f edge1 = point(vert(i,1)) - point(vert(i,0));
            Vec3f edge2 = point(vert(i,2)) - point(vert(i,0));
            norm = cross(edge1, edge2).normalize();
            material = material_;
            return true;
        }
    }

    return tnear_min != std::numeric_limits<float>::max();
}


int Model::nverts() const {
    return (int)verts.size();
}

int Model::nfaces() const {
    return (int)faces.size();
}

void Model::get_bbox(Vec3f &min, Vec3f &max) {
    min = max = verts[0];
    for (int i=1; i<(int)verts.size(); ++i) {
        for (int j=0; j<3; j++) {
            min[j] = std::min(min[j], verts[i][j]);
            max[j] = std::max(max[j], verts[i][j]);
        }
    }
    std::cerr << "bbox: [" << min << " : " << max << "]" << std::endl;
}

const Vec3f &Model::point(int i) const {
    assert(i>=0 && i<nverts());
    return verts[i];
}

Vec3f &Model::point(int i) {
    assert(i>=0 && i<nverts());
    return verts[i];
}

int Model::vert(int fi, int li) const {
    assert(fi>=0 && fi<nfaces() && li>=0 && li<3);
    return faces[fi][li];
}

std::ostream& operator<<(std::ostream& out, Model &m) {
    for (int i=0; i<m.nverts(); i++) {
        out << "v " << m.point(i) << std::endl;
    }
    for (int i=0; i<m.nfaces(); i++) {
        out << "f ";
        for (int k=0; k<3; k++) {
            out << (m.vert(i,k)+1) << " ";
        }
        out << std::endl;
    }
    return out;
}

#endif //__MODEL_H__

