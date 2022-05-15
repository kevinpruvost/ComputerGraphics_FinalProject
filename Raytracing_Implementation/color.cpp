#include"color.h"

#include<sstream>

Color operator + ( const Color& A , const Color& B ) {
    return Color( A.r + B.r , A.g + B.g , A.b + B.b );
}

Color operator - ( const Color& A , const Color& B ) {
    return Color( A.r - B.r , A.g - B.g , A.b - B.b );
}

Color operator * ( const Color& A , const Color& B ) {
    return Color( A.r * B.r , A.g * B.g , A.b * B.b );
}

Color operator * ( const Color& A , const double& k ) {
    return Color( A.r * k , A.g * k , A.b * k );
}

Color operator / ( const Color& A , const double& k ) {
    return Color( A.r / k , A.g / k , A.b / k );
}

Color& operator += ( Color& A , const Color& B ) {
    A.r += B.r;
    A.g += B.g;
    A.b += B.b;
    return A;
}

Color& operator -= ( Color& A , const Color& B ) {
    A.r -= B.r;
    A.g -= B.g;
    A.b -= B.b;
    return A;
}

Color& operator *= ( Color& A , const double& k ) {
    A.r *= k;
    A.g *= k;
    A.b *= k;
    return A;
}

Color& operator /= ( Color& A , const double& k ) {
    A.r /= k;
    A.g /= k;
    A.b /= k;
    return A;
}

void Color::Confine() {
    if ( r > 1 ) r = 1;
    if ( g > 1 ) g = 1;
    if ( b > 1 ) b = 1;
}

void Color::Input( std::stringstream& fin ) {
    fin >> r >> g >> b;
}
