#ifndef VECTOR_H
#define VECTOR_H


#define _USE_MATH_DEFINES

#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <algorithm>
#include <limits>
#include <string>
#include <sstream>


template <size_t dim, typename T>
class Vector
{
    T data_[dim];

public:

    Vector() { for (size_t i = dim; i--; data_[i] = T()); }
    Vector(const Vector<dim, T> & vec) { for (size_t i = dim; i--; data_[i] = vec[i]); }
    Vector(Vector<dim, T> && vec) { for (size_t i = dim; i--; data_[i] = vec[i]); }
    Vector<dim, T> & operator=(const Vector<dim, T> & vec);
    Vector<dim, T> & operator=(Vector<dim, T> && vec);
    virtual ~Vector() = default;

    T& operator[](const size_t i);
    const T& operator[](const size_t i) const;
    void swap(Vector<dim, T> & vec);

    Vector<dim, T> & operator+=(const Vector<dim, T> &vec);
    Vector<dim, T> & operator-=(const Vector<dim, T> &vec);
    bool operator==(const Vector<dim, T> & vec) const;

    template<typename U>
    Vector<dim, T> & operator*=(const U &elem);

    friend std::ostream & operator<< <dim, T>(std::ostream & ostr, const Vector<dim, T> & vec);
    friend std::istream & operator>> <dim, T>(std::istream & istr, Vector<dim, T> & vec);
};


//-----------------------------------------------------VECTOR---------------------------------------------


template<size_t dim, typename T>
Vector<dim, T> & Vector<dim, T>::operator=(const Vector<dim, T> & vec)
{
    Vector<dim, T> v_tmp(vec);

    swap(v_tmp);

    return *this;
}


template<size_t dim, typename T>
Vector<dim, T> & Vector<dim, T>::operator=(Vector<dim, T> && vec)
{
    Vector<dim, T> v_tmp = vec;

    swap(v_tmp);

    return *this;
}


template<size_t dim, typename T>
void Vector<dim, T>::swap(Vector<dim, T> & vec)
{
    std::swap(data_, vec.data_);
}


//-------------------------------------------PARTIAL_SPECIALIZATION_dim=2---------------------------------------------


typedef Vector<2, float> Vec2f;


template <typename T>
class Vector<2, T>
{
    T x_, y_;

public:

    Vector() : x_(T()), y_(T()) {};
    Vector(T x, T y) : x_(x), y_(y) {};
    
    T& operator[](const size_t i);
    const T& operator[](const size_t i) const;
};


template <typename T>
T & Vector<2, T>::operator[](const size_t i)
{
    if (i >= 2 || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = 2 there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return i == 0 ? x_ : y_;
}


template <typename T>
const T & Vector<2, T>::operator[](const size_t i) const
{
    if (i >= 2 || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = 2 there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return i == 0 ? x_ : y_;
}


//-------------------------------------------PARTIAL_SPECIALIZATION_dim=3---------------------------------------------


typedef Vector<3, float> Vec3f;
typedef Vector<3, int> Vec3i;


template <typename T>
class Vector<3, T>
{
    T x_, y_, z_;

public:

    Vector() : x_(T()), y_(T()), z_(T()) {};
    Vector(T x, T y, T z) : x_(x), y_(y), z_(z) {};
    
    T& operator[](const size_t i);
    const T& operator[](const size_t i) const;
    T& x() { return x_; }
    const T& x() const { return x_; }
    T& y() { return y_; }
    const T& y() const { return y_; }
    T& z() { return z_; }
    const T& z() const { return z_; }

    bool operator==(const Vector<3, T> &vec) const;

    template<typename U>
    Vector<3, T> & operator*=(const U &elem);

    float norm() { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
    Vector<3, T> & normalize(T type = 1) { *this *= type / norm(); return *this; }
};


template <typename T>
T & Vector<3, T>::operator[](const size_t i)
{
    if (i >= 3 || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = 3 there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return i == 0 ? x_ : (i == 1 ? y_ : z_);
}


template <typename T>
const T & Vector<3, T>::operator[](const size_t i) const
{
    if (i >= 3 || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = 3 there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return i == 0 ? x_ : (i == 1 ? y_ : z_);
}


template <typename T>
bool Vector<3, T>::operator==(const Vector<3, T> &vec) const 
{
    return (x_ == vec.x_) && (y_ == vec.y_) && (z_ == vec.z_);
}


template <typename T> 
Vector<3, T> cross(const Vector<3,T> &v1, const Vector<3,T> &v2) //vector mult
{
    return Vector<3,T>(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x());
}


template<typename T>
template<typename U>
Vector<3, T> & Vector<3, T>::operator*=(const U &elem)
{
    x_ *= elem;
    y_ *= elem;
    z_ *= elem;

    return *this;
}


//-------------------------------------------PARTIAL_SPECIALIZATION_dim=4---------------------------------------------


typedef Vector<4, float> Vec4f;


template <typename T>
class Vector<4, T>
{
    T x_, y_, z_, w_;

public:

    Vector() : x_(T()), y_(T()), z_(T()), w_(T()) {};
    Vector(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) {};
    
    T& operator[](const size_t i);
    const T& operator[](const size_t i) const;
};


template <typename T>
T & Vector<4, T>::operator[](const size_t i)
{
    if (i >= 4 || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = 4 there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return i == 0 ? x_ : (i == 1 ? y_ : (i == 2 ? z_ : w_)); 
}


template <typename T>
const T & Vector<4, T>::operator[](const size_t i) const
{
    if (i >= 4 || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = 4 there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return i == 0 ? x_ : (i == 1 ? y_ : (i == 2 ? z_ : w_)); 
}


//---------------------------------------------Operators in the class----------------------------------------------


template<size_t dim, typename T>
bool Vector<dim, T>::operator==(const Vector<dim, T> & vec) const
{ 
    if (!std::equal(data_[0], data_[0] + dim, vec[0], vec[0] + dim))
    {
        return 0;
    }

    return 1;
}


template <size_t dim, typename T>
T & Vector<dim, T>::operator[](const size_t i)
{
    if (i >= dim || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = " + std::to_string(dim) + " there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return data_[i];
}


template <size_t dim, typename T>
const T & Vector<dim, T>::operator[](const size_t i) const
{ 
    if (i >= dim || i < 0)
    {
        std::string mess;
        mess += "In vector with dim = " + std::to_string(dim) + " there is not " + std::to_string(i) + " component";
        throw std::logic_error(mess);
    }

    return data_[i];
}


template <size_t dim, typename T>
Vector<dim, T> & Vector<dim, T>::operator+=(const Vector<dim,T> &rhs)
{
    for (size_t i = dim; i--; data_[i] += rhs[i]);

    return *this;
}


template <size_t dim, typename T>
Vector<dim, T> & Vector<dim, T>::operator-=(const Vector<dim,T> &rhs)
{
    for (size_t i = dim; i--; data_[i] -= rhs[i]);

    return *this;
}


template<size_t dim, typename T>
template<typename U>
Vector<dim, T> & Vector<dim, T>::operator*=(const U &elem)
{
    for (int i = dim; i--; data_[i] *= elem);

    return *this;
}


//----------------------------------------Operators outside the class-----------------------------------------


template<size_t dim, typename T, typename U>
Vector<dim, T> operator*(const Vector<dim, T> &lhs, const U &rhs)
{
    Vector<dim, T> res;

    for (size_t i = dim; i--; res[i] = lhs[i] * rhs);

    return res;
}


template<size_t dim, typename T> 
Vector<dim, T> operator-(const Vector<dim, T> &lhs)
{
    return lhs*T(-1);
}


template<size_t dim, typename T>
Vector<dim, T> operator-(const Vector<dim, T> &lhs, const Vector<dim, T> &rhs)
{
    Vector<dim, T> res;

    for (size_t i = dim; i--; res[i] = lhs[i] - rhs[i]);

    return res;
}


template<size_t dim, typename T>
Vector<dim, T> operator+(const Vector<dim, T> &lhs, const Vector<dim, T> &rhs)
{
    Vector<dim, T> res;

    for (size_t i = dim; i--; res[i] = lhs[i] + rhs[i]);

    return res;
}


template<size_t dim, typename T>
T operator*(const Vector<dim, T> &lhs, const Vector<dim, T> &rhs) // scalar mult
{
    T res = T();

    for (size_t i = dim; i--; res += lhs[i] * rhs[i]);

    return res;
}


//-------------------------------------------Friends----------------------------------------------------


template<size_t dim, typename T>
std::istream & operator>>(std::istream & istr, Vector<dim, T> & vec)
{
    char space;

    for (int i = 0; i < dim; i++)
    {
        istr >> std::ws;

        istr >> vec[i];
        istr >> space;

        if (!std::isspace(space) || istr.fail())
        {
            throw std::logic_error("Invalid input");
        }
    }

    while (space != '\n')
    {
        if (!isspace(space))
        {
            throw std::logic_error("Invalid input");
        }
        istr >> space;
    }

    return istr;
}


template<size_t dim, typename T>
std::ostream & operator<<(std::ostream & ostr, const Vector<dim, T> & vec)
{
    for (int i = 0; i < static_cast<int>(dim); i++)
    {
        ostr << vec[i] << " ";
        ostr << std::endl;
    }

    return ostr;      
}


#endif
