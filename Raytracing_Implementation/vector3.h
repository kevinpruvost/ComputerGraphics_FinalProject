#ifndef VECTOR3_H
#define VECTOR3_H

#include<sstream>

extern const double EPS;
extern const double PI;

class Vector3 {
public:
    double x , y , z;

    Vector3() : x(0), y(0), z(0) {}
    explicit Vector3( double X , double Y , double Z ) : x( X ) , y( Y ) , z( Z ) {}
    ~Vector3() {}

    friend Vector3 operator + ( const Vector3& , const Vector3& );
    friend Vector3 operator - ( const Vector3& , const Vector3& );
    friend Vector3 operator * ( const Vector3& , const double& );
    friend Vector3 operator * ( const double& , const Vector3& );
    friend Vector3 operator / ( const Vector3& , const double& );
    friend Vector3 operator * ( const Vector3& , const Vector3& ); //cross product
    friend Vector3& operator += ( Vector3& , const Vector3& );
    friend Vector3& operator -= ( Vector3& , const Vector3& );
    friend Vector3& operator *= ( Vector3& , const double& );
    friend Vector3& operator /= ( Vector3& , const double& );
    friend Vector3& operator *= ( Vector3& , const Vector3& ); //cross product
    friend Vector3 operator - ( const Vector3& );

    double Dot( const Vector3& ) const;
    double Module2() const;
    double Module() const;
    double Distance2( Vector3& ) const;
    double Distance( Vector3& ) const;
    Vector3 Ortho( Vector3 ) const;
    Vector3 GetUnitVector() const;
    /* Generate a unit vector perpendicular to this vector. */
    Vector3 GetAnVerticalVector() const;
    bool IsZeroVector() const;
    Vector3 Reflect( Vector3 N ) const;
    Vector3 Refract( Vector3 N , double n ) const;
    /* Generate a random vector with the same length above the tangent plaine. */
    Vector3 Diffuse() const;
    Vector3 Rotate( Vector3 axis , double theta ) const;

    double& GetCoord(int axis);

    void Input(std::stringstream& fin);
    void AssRandomVector();
};

#endif
