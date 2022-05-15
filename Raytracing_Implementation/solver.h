#ifndef SOLVER_H
#define SOLVER_H
/**
* Reference: https://blog.csdn.net/maple_2014/article/details/104578335
* Original author: Marc Pony(marc_pony@163.com)
*/

#include <cmath>
#include <cfloat>

typedef unsigned int UINT32;
constexpr UINT32 ERR_NAN = 0x00000001;
constexpr UINT32 ERR_INF = 0x00000002;

#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#define MIN(a, b) ((a) < (b)) ? (a) : (b)

/*************************************************
Function: is_number
Description: 判断浮点数是否为nan
Input: 浮点数x
Output: 无
Return: 若浮点数x为nan返回0，否则返回1
Author: Marc Pony(marc_pony@163.com)
*************************************************/
int is_number(double x)
{
    return (x == x);
}

/*************************************************
Function: is_finite_number
Description: 判断浮点数是否为inf
Input: 浮点数x
Output: 无
Return: 若浮点数x为inf返回0，否则返回1
Author: Marc Pony(marc_pony@163.com)
*************************************************/
int is_finite_number(double x)
{
    return (x >= -FLT_MAX && x <= FLT_MAX);
}

/*************************************************
Function: solve_quadratic_equation
Description: 求一元二次方程(a*x^2 + b*x + c = 0)的所有实数根
Input: 方程的系数 p = {c, b, a}
Output: 方程的所有实数根x, 实数根的个数rootCount
Return: 错误号
Author: Marc Pony(marc_pony@163.com)
*************************************************/
UINT32 solve_quadratic_equation(double p[], double x[], int* rootCount)
{
    int i;
    double a, b, c, delta, sqrtDelta;
    const double ZERO = FLT_MIN;  // min normalized positive value（1.175494351e-38F）
    const double EPS = FLT_MIN;
    UINT32 errNo = 0;

    *rootCount = 0;
    for (i = 0; i < 3; i++) {
        if (!is_number(p[i])) {
            errNo = ERR_NAN;
            return errNo;
        }
        if (!is_finite_number(p[i])) {
            errNo = ERR_INF;
            return errNo;
        }
    }

    a = p[2];
    b = p[1];
    c = p[0];

    if (abs(a - 0.0) < EPS) {
        if (abs(b - 0.0) > EPS) {
            x[0] = -c / b;
            *rootCount = 1;
        }
    } else {
        b /= a;
        c /= a;
        a = 1.0;

        delta = b * b - 4.0 * a * c;
        if (delta > ZERO) {
            if (abs(c - 0.0) < EPS)	//若c = 0,由于计算误差,sqrt(b*b - 4*a*c）不等于|b|
            {
                x[0] = 0.0;
                x[1] = -b / a;
            } else {
                sqrtDelta = sqrt(delta);
                if (b > 0.0) {
                    x[0] = (-2.0 * c) / (b + sqrtDelta);	//避免两个很接近的数相减,导致精度丢失
                    x[1] = (-b - sqrtDelta) / (2.0 * a);
                } else {
                    x[0] = (-b + sqrtDelta) / (2.0 * a);
                    x[1] = (-2.0 * c) / (b - sqrtDelta);	//避免两个很接近的数相减,导致精度丢失
                }
            }
            *rootCount = 2;
        } else if (abs(delta - 0.0) < EPS) {
            x[0] = x[1] = -b / (2.0 * a);
            *rootCount = 2;
        } else {
            *rootCount = 0;
        }
    }
    return errNo;
}


/*************************************************
Function: solve_cubic_equation
Description: 盛金公式求一元三次方程(a*x^3 + b*x^2 + c*x + d = 0)的所有实数根
             A = b * b - 3.0 * a * c;
             B = b * c - 9.0 * a * d;
             C = c * c - 3.0 * b * d;
             (1)当A = B = 0时，方程有一个三重实根
             (2)当Δ = B^2－4 * A * C > 0时，方程有一个实根和一对共轭虚根
             (3)当Δ = B^2－4 * A * C = 0时，方程有三个实根，其中有一个两重根
             (4)当Δ = B^2－4 * A * C < 0时，方程有三个不相等的实根
Input: 方程的系数 p = {d, c, b, a}
Output: 方程的所有实数根x，实数根的个数rootCount
Return: 错误号
Author: Marc Pony(marc_pony@163.com)
*************************************************/
UINT32 solve_cubic_equation(double p[], double x[], int* rootCount)
{
    int i;
    double a, b, c, d, A, B, C, delta;
    double Y1, Y2, Z1, Z2, K, parm[3], roots[2], theta, T;
    const double ZERO = FLT_MIN;  // min normalized positive value（1.175494351e-38F）
    const double EPS = FLT_MIN;
    const double CALCULATE_ERROR = 1.0e-7;
    UINT32 errNo = 0;

    *rootCount = 0;
    for (i = 0; i < 4; i++) {
        if (!is_number(p[i])) {
            errNo = ERR_NAN;
            return errNo;
        }
        if (!is_finite_number(p[i])) {
            errNo = ERR_INF;
            return errNo;
        }
    }

    a = p[3];
    b = p[2];
    c = p[1];
    d = p[0];

    if (abs(a - 0.0) < EPS) {
        parm[2] = b;
        parm[1] = c;
        parm[0] = d;

        errNo = solve_quadratic_equation(parm, x, rootCount);
    } else {
        b /= a;
        c /= a;
        d /= a;
        a = 1.0;

        A = b * b - 3.0 * a * c;
        B = b * c - 9.0 * a * d;
        C = c * c - 3.0 * b * d;

        delta = B * B - 4.0 * A * C;

        if (abs(A - 0.0) < EPS && abs(B - 0.0) < EPS) {
            x[0] = x[1] = x[2] = -b / (3.0 * a);
            *rootCount = 3;
            return errNo;
        }

        if (delta > ZERO) {
            parm[2] = 1.0;
            parm[1] = B;
            parm[0] = A * C;

            errNo = solve_quadratic_equation(parm, roots, rootCount);
            if (errNo != 0) {
                return errNo;
            }
            Z1 = roots[0];
            Z2 = roots[1];

            Y1 = A * b + 3.0 * a * Z1;
            Y2 = A * b + 3.0 * a * Z2;

            if (Y1 < 0.0 && Y2 < 0.0)	//pow函数的底数必须为非负数,必须分类讨论
            {
                x[0] = (-b + pow(-Y1, 1.0 / 3.0) + pow(-Y2, 1.0 / 3.0)) / (3.0 * a);
            } 			else if (Y1 < 0.0 && Y2 > 0.0) 			{
                x[0] = (-b + pow(-Y1, 1.0 / 3.0) - pow(Y2, 1.0 / 3.0)) / (3.0 * a);
            } 			else if (Y1 > 0.0 && Y2 < 0.0) 			{
                x[0] = (-b - pow(Y1, 1.0 / 3.0) + pow(-Y2, 1.0 / 3.0)) / (3.0 * a);
            } 			else 			{
                x[0] = (-b - pow(Y1, 1.0 / 3.0) - pow(Y2, 1.0 / 3.0)) / (3.0 * a);
            }
            *rootCount = 1;
        } else if (abs(delta - 0.0) < EPS) {
            if (abs(A - 0.0) > EPS) {
                K = B / A;
                x[0] = -b / a + K;
                x[1] = x[2] = -0.5 * K;
                *rootCount = 3;
            }
        } else {
            if (A > 0.0) {
                T = (2.0 * A * b - 3.0 * a * B) / (2.0 * pow(A, 3.0 / 2.0));
                //由于计算误差,T的值可能略大于1(如1.0000001)
                if (T > 1.0) {
                    if (T < 1.0 + CALCULATE_ERROR) {
                        T = 1.0;
                    } else {
                        return errNo;
                    }
                }
                if (T < -1.0) {
                    if (T > -1.0 - CALCULATE_ERROR) {
                        T = -1.0;
                    } else {
                        return errNo;
                    }
                }
                theta = acos(T);
                x[0] = (-b - 2.0 * sqrt(A) * cos(theta / 3.0)) / (3.0 * a);
                x[1] = (-b + sqrt(A) * (cos(theta / 3.0) + sqrt(3.0) * sin(theta / 3.0))) / (3.0 * a);
                x[2] = (-b + sqrt(A) * (cos(theta / 3.0) - sqrt(3.0) * sin(theta / 3.0))) / (3.0 * a);
                *rootCount = 3;
            }
        }
    }
    return errNo;
}


/*************************************************
Function: solve_quartic_equation
Description: 费拉里法求一元四次方程(a*x^4 + b*x^3 + c*x^2 + d*x + e = 0)的所有实数根
Input: 方程的系数 p = {e, d, c, b, a}
Output: 方程的所有实数根x,实数根的个数rootCount
Return: 错误号
Author: Marc Pony(marc_pony@163.com)
*************************************************/
UINT32 solve_quartic_equation(double p[], double x[], int* rootCount)
{
    double a, b, c, d, e;
    double parm[4], roots[3];
    double y, M, N;
    double x1[2], x2[2];
    int rootCount1, rootCount2, i;
    double MSquareTemp, MSquare, yTemp;
    const double EPS = FLT_MIN;  //min normalized positive value（1.175494351e-38F）
    UINT32 errNo = 0;

    *rootCount = 0;
    for (i = 0; i < 5; i++) {
        if (!is_number(p[i])) {
            errNo = ERR_NAN;
            return errNo;
        }
        if (!is_finite_number(p[i])) {
            errNo = ERR_INF;
            return errNo;
        }
    }

    a = p[4];
    b = p[3];
    c = p[2];
    d = p[1];
    e = p[0];

    if (abs(a - 0.0) < EPS) {
        if (abs(b - 0.0) < EPS) {
            parm[2] = c;
            parm[1] = d;
            parm[0] = e;
            errNo = solve_quadratic_equation(parm, x, rootCount);
        } else {
            parm[3] = b;
            parm[2] = c;
            parm[1] = d;
            parm[0] = e;
            errNo = solve_cubic_equation(parm, x, rootCount);
        }
    } else {
        b /= a;
        c /= a;
        d /= a;
        e /= a;

        parm[3] = 8.0;
        parm[2] = -4.0 * c;
        parm[1] = 2.0 * (b * d - 4.0 * e);
        parm[0] = -e * (b * b - 4.0 * c) - d * d;

        errNo = solve_cubic_equation(parm, roots, rootCount);
        if (*rootCount != 0) {
            y = roots[0];
            MSquare = 8.0 * y + b * b - 4.0 * c;
            for (i = 1; i < *rootCount; i++) {
                yTemp = roots[i];
                MSquareTemp = 8.0 * yTemp + b * b - 4.0 * c;
                if (MSquareTemp > MSquare) {
                    MSquare = MSquareTemp;
                    y = yTemp;
                }
            }

            if (MSquare > 0.0) {
                M = sqrt(MSquare);
                N = b * y - d;
                parm[2] = 2.0;
                parm[1] = b + M;
                parm[0] = 2.0 * (y + N / M);
                errNo = solve_quadratic_equation(parm, x1, &rootCount1);

                parm[2] = 2.0;
                parm[1] = b - M;
                parm[0] = 2.0 * (y - N / M);
                errNo = solve_quadratic_equation(parm, x2, &rootCount2);

                if (rootCount1 == 2) {
                    x[0] = x1[0];
                    x[1] = x1[1];
                    x[2] = x2[0];
                    x[3] = x2[1];
                } else {
                    x[0] = x2[0];
                    x[1] = x2[1];
                    x[2] = x1[0];
                    x[3] = x1[1];
                }
                *rootCount = rootCount1 + rootCount2;
            }
        }
    }
    return errNo;
}

/*
void main(void)
{
    double x[4], p[5];
    int rootCount;
    double a, b, c, d, e;
    UINT32 errNo = ERR_NO_ERROR;

    //一元四次方程测试
    //（1）(x - 1) * (x - 2) * (x^2 + 1) = 0 (x^4 - 3*x^3 + 3*x^2 - 3*x + 2 = 0)
    a = 1;
    b = -3;
    c = 3;
    d = -3;
    e = 2;

    //(2) (x - 1)^2 * (x^2 + 1) = 0 (x^4 - 2*x^3 + 2*x^2 - 2*x + 1 = 0)
    //a = 1;
    //b = -2;
    //c = 2;
    //d = -2;
    //e = 1;

    //(3) (x - 1) * (x - 2) * (x - 3) * (x - 4) = 0 (x^4 - 10*x^3 + 35*x^2 - 50*x + 24 = 0)
    //a = 1;
    //b = -10;
    //c = 35;
    //d = -50;
    //e = 24;

    //(4) (x - 1)^2 * (x - 2)^2 = 0 (x^4 - 6*x^3 + 13*x^2 - 12*x + 4 = 0)
    //a = 1;
    //b = -6;
    //c = 13;
    //d = -12;
    //e = 4;

    //(5) 0*x^4 + x^3 - 3*x^2 + 3*x - 1 = 0
    //a = 0;
    //b = 1;
    //c = -3;
    //d = 3;
    //e = -1;

    //(6) 0*x^4 + 0*x^3 + x^2 - 2*x + 1 = 0
    //a = 0;
    //b = 0;
    //c = 1;
    //d = -2;
    //e = 1;

    //(7) 0*x^4 + 0*x^3 + 0*x^2 - 2*x + 1 = 0
    //a = 0;
    //b = 0;
    //c = 0;
    //d = -2;
    //e = 1;

    p[0] = e;
    p[1] = d;
    p[2] = c;
    p[3] = b;
    p[4] = a;
    errNo = solve_quartic_equation(p, x, &rootCount);
}
*/

#endif
