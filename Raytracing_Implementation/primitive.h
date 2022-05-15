#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include<iostream>
#include<sstream>
#include<string>
#include<vector>

#include"color.h"
#include"vector3.h"
#include"bmp.h"

extern const double EPS;
extern const double PI;
const double BIG_DIST = 1e100;

class Blur {
public:
    virtual std::pair<double, double> GetXY() = 0;
};

class ExpBlur : public Blur {
public:
    /* return x & y coord of a random point inside a unit sphere,
       with radius following an exponential sampling.
    */
    std::pair<double, double> GetXY();
};

class Material {
public:
    Color color , absor;
    double refl , refr;
    double diff , spec;
    double rindex;
    double drefl;
    Bmp* texture;
    Blur* blur;

    Material();
    ~Material() {}

    void Input( std::string , std::stringstream& );
};

struct CollidePrimitive;

class Primitive {
protected:
    int sample;
    Material* material;
    Primitive* next;

public:

    Primitive();
    Primitive( const Primitive& );
    ~Primitive();

    int GetSample() { return sample; }
    Material* GetMaterial() { return material; }
    Primitive* GetNext() { return next; }
    void SetNext( Primitive* primitive ) { next = primitive; }

    virtual void Input( std::string , std::stringstream& );
    virtual CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V ) = 0;
    virtual Color GetTexture(Vector3 crash_C) = 0;
    virtual bool IsLightPrimitive() const { return false; }
};

struct CollidePrimitive {
    bool isCollide;
    Primitive* collide_primitive;
    Vector3 N , C;
    double dist;
    bool front;
    CollidePrimitive(){isCollide = false; collide_primitive = NULL; dist = BIG_DIST;}
    Color GetTexture(){return collide_primitive->GetTexture(C);}
};

class Sphere : public Primitive {
protected:
    Vector3 O , De , Dc;
    double R;

public:
    Sphere();
    ~Sphere() {}

    void Input( std::string , std::stringstream& );
    CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
    Color GetTexture(Vector3 crash_C);
};

class SphereLightPrimitive : public Sphere{
public:
    SphereLightPrimitive(Vector3 pO, double pR, Color color) : Sphere()
    {O = pO; R = pR; material->color = color; }
    virtual bool IsLightPrimitive() const { return true; }
};

class Plane : public Primitive {
protected:
    Vector3 N , Dx , Dy;
    double R;

public:
    Plane() : Primitive() {}
    ~Plane() {}

    void Input( std::string , std::stringstream& );
    CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
    Color GetTexture(Vector3 crash_C);
};

class Square : public Primitive {
protected:
    Vector3 O , Dx , Dy;

public:
    Square() : Primitive() {}
    ~Square() {}

    void Input( std::string , std::stringstream& );
    CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
    Color GetTexture(Vector3 crash_C);
};

class PlaneAreaLightPrimitive : public Square {
public:
    explicit PlaneAreaLightPrimitive(Vector3 pO, Vector3 pDx, Vector3 pDy, Color color): Square()
    {O = pO; Dx = pDx; Dy = pDy; material->color = color; }
    virtual bool IsLightPrimitive() const { return true; }
};

/*
// TODO: NEED TO IMPLEMENT. Follow the code given below or totally re-write it
class Cylinder : public Primitive {
    Vector3 O1, O2;
    double R;

public:
    Cylinder() : Primitive() {}
    explicit Cylinder(Vector3 pO1, Vector3 pO2, double pR) : Primitive() {O1 = pO1; O2 = pO2; R = pR; }
    ~Cylinder() {}

    void Input( std::string , std::stringstream& );
    CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
    Color GetTexture(Vector3 crash_C);
};

class Bezier : public Primitive {
    Vector3 O1, O2;
    Vector3 N, Nx, Ny;
    std::vector<double> R;
    std::vector<double> Z;
    int degree;
    Cylinder* boundingCylinder;

public:
    Bezier() : Primitive() {boundingCylinder = NULL; degree = -1;}
    ~Bezier() {}

    void Input( std::string , std::stringstream& );
    CollidePrimitive Collide( Vector3 ray_O , Vector3 ray_V );
    Color GetTexture(Vector3 crash_C);

private:
    std::pair<double, double> valueAt(double u);
    std::pair<double, double> valueAt(double u, const std::vector<double>& xs, const std::vector<double>& ys);
};
 
*/

#endif
