#include"vector3.h"

#include<cmath>
#include<sstream>
#include<cstdlib>
#include<iostream>

/* random double in [0, 1) */
#define ran() ( double( rand() % RAND_MAX ) / RAND_MAX )

const double EPS = 1e-6;
const double PI = 3.1415926535897932384626;

Vector3 operator + ( const Vector3& A , const Vector3& B ) {
    return Vector3( A.x + B.x , A.y + B.y , A.z + B.z );
}

Vector3 operator - ( const Vector3& A , const Vector3& B ) {
    return Vector3( A.x - B.x , A.y - B.y , A.z - B.z );
}

Vector3 operator * ( const Vector3& A , const double& k ) {
    return Vector3( A.x * k , A.y * k , A.z * k );
}

Vector3 operator * ( const double& k, const Vector3& A ) {
    return Vector3( A.x * k , A.y * k , A.z * k );
}

Vector3 operator / ( const Vector3& A , const double& k ) {
    return Vector3( A.x / k , A.y / k , A.z / k );
}

Vector3 operator * ( const Vector3& A , const Vector3& B ) {
    return Vector3( A.y * B.z - A.z * B.y , A.z * B.x - A.x * B.z , A.x * B.y - A.y * B.x );
}

Vector3& operator += ( Vector3& A , const Vector3& B ) {
    A.x += B.x;
    A.y += B.y;
    A.z += B.z;
    return A;
}

Vector3& operator -= ( Vector3& A , const Vector3& B ) {
    A.x -= B.x;
    A.y -= B.y;
    A.z -= B.z;
    return A;
}

Vector3& operator *= ( Vector3& A , const double& k ) {
    A.x *= k;
    A.y *= k;
    A.z *= k;
    return A;
}

Vector3& operator /= ( Vector3& A, const double& k ) {
    A.x /= k;
    A.y /= k;
    A.z /= k;
    return A;
}

Vector3& operator *= ( Vector3& A , const Vector3& B ) {
    auto x = A.y * B.z - A.z * B.y;
    auto y = A.z * B.x - A.x * B.z;
    auto z = A.x * B.y - A.y * B.x;
    A.x = x;
    A.y = y;
    A.z = z;
    return A;
}

Vector3 operator - ( const Vector3& A ) {
    return Vector3( -A.x , -A.y , -A.z );
}

double Vector3::Dot( const Vector3& term ) const
{
    return x * term.x + y * term.y + z * term.z;
}

double Vector3::Module2() const
{
    return x * x + y * y + z * z;
}

double Vector3::Module() const
{
    return sqrt( x * x + y * y + z * z );
}

double Vector3::Distance2( Vector3& term ) const
{
    return ( term - *this ).Module2();
}

double Vector3::Distance( Vector3& term ) const
{
    return ( term - *this ).Module();
}

Vector3 Vector3::Ortho( Vector3 term ) const
{
    return *this - term * this->Dot(term);
}

double& Vector3::GetCoord( int axis ) {
    if (axis == 0) {
        return x;
    } else if (axis == 1) {
        return y;
    } else {
        return z;
    }
}

Vector3 Vector3::GetUnitVector() const
{
    return *this / Module();
}

void Vector3::AssRandomVector() {
    do {
        x = 2 * ran() - 1;
        y = 2 * ran() - 1;
        z = 2 * ran() - 1;
    } while ( x * x + y * y + z * z > 1 || x * x + y * y + z * z < EPS );
    *this = GetUnitVector();
}

Vector3 Vector3::GetAnVerticalVector() const
{
    Vector3 ret = *this * Vector3( 0 , 0 , 1 );
    if (ret.IsZeroVector()) {
        ret = Vector3(1, 0, 0);
    } else {
        ret = ret.GetUnitVector();
    }
    return ret;
}

bool Vector3::IsZeroVector() const
{
    return fabs( x ) < EPS && fabs( y ) < EPS && fabs( z ) < EPS;
}

void Vector3::Input( std::stringstream& fin ) {
    fin >> x >> y >> z;
}

Vector3 Vector3::Reflect( Vector3 N ) const
{
    return *this - N * ( 2 * Dot( N ) );
}

Vector3 Vector3::Refract( Vector3 N , double n ) const
{
    Vector3 V = GetUnitVector();
    double cosI = -N.Dot( V ) , cosT2 = 1 - ( n * n ) * ( 1 - cosI * cosI );
    if ( cosT2 > EPS ) return V * n + N * ( n * cosI - sqrt( cosT2 ) );
    return V.Reflect( N );
}

Vector3 Vector3::Diffuse() const
{
    Vector3 Vert = GetAnVerticalVector();
    // sqrt to avoid too small value
    // theta in [0, pi/2), more likely to be a larger value
    double theta = acos( sqrt( ran() ) );
    double phi = ran() * 2 * PI;
    // rotate this vector around a perpendicular vector by theta,
    // then rotate the result vector around the original vector by phi.
    return Rotate( Vert , theta ).Rotate( *this , phi );
}

Vector3 Vector3::Rotate( Vector3 axis , double theta ) const
{
    Vector3 ret;
    double cost = cos( theta );
    double sint = sin( theta );
    axis = axis.GetUnitVector();

    ret.x += x * ( axis.x * axis.x + ( 1 - axis.x * axis.x ) * cost );
    ret.x += y * ( axis.x * axis.y * ( 1 - cost ) - axis.z * sint );
    ret.x += z * ( axis.x * axis.z * ( 1 - cost ) + axis.y * sint );

    ret.y += x * ( axis.y * axis.x * ( 1 - cost ) + axis.z * sint );
    ret.y += y * ( axis.y * axis.y + ( 1 - axis.y * axis.y ) * cost );
    ret.y += z * ( axis.y * axis.z * ( 1 - cost ) - axis.x * sint );

    ret.z += x * ( axis.z * axis.x * ( 1 - cost ) - axis.y * sint );
    ret.z += y * ( axis.z * axis.y * ( 1 - cost ) + axis.x * sint );
    ret.z += z * ( axis.z * axis.z + ( 1 - axis.z * axis.z ) * cost );
    return ret;
}
