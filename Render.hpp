//
// Created by 滕逸青 on 2021/10/29.
//

#include "Tgaimage.h"
#include "MyMath.hpp"
#include <math.h>

using namespace std;

Mat4d getModelMat()
{
    return Mat4d::identity();
}


Mat4d getOrthoMat(double near, double far, double left, double right, double bottom, double top)
{
    Mat4d t = {
            {1, 0, 0, -((left + right)) / 2},
            {0, 1, 0, -((bottom + top)) / 2},
            {0, 0, 1, -((near + far)) / 2},
            {0, 0, 0, 1}
    };
    double a = right - left, b = top - bottom, c = far - near;
    Mat4d s{
            {2 / a, 0,     0,     0},
            {0,     2 / b, 0,     0},
            {0,     0,     2 / c, 0},
            {0,     0,     0,     1}
    };
    return s * t;
}

///
/// \param near 近平面
/// \param far 远平面
/// \param eyeFov 角度制
/// \param aspectRatio 宽高比
/// \return
Mat4d getPerspectiveMat(double near, double far, double eyeFov, double aspectRatio)
{
    Mat4d p{
            {near, 0,    0,          0},
            {0,    near, 0,          0},
            {0,    0,    near + far, -near * far},
            {0,    0,    1,          0}
    };
    double halfFov = eyeFov / 180 * M_PI / 2;
    double top = near * tan(halfFov);
    double right = top * aspectRatio;
    Mat4d ortho = getOrthoMat(near, far, -right, right, -top, top);
    return ortho * p;
}

Mat4d getViewMat(Vec3d pos, Vec3d target, Vec3d up)
{
    Vec3d z = target - pos;
    z.normalize();
    up.normalize();
    Vec3d y = up;
    Vec3d x = z.cross(y);
    x.normalize();
    y = x.cross(z);
    y.normalize();
    Mat4d T{
            {1., 0., 0., -pos[0]},
            {0., 1., 0., -pos[1]},
            {0., 0., 1., -pos[2]},
            {0., 0., 0., 1.}
    };
    Mat4d R{
            {x[0], x[1], x[2], 0},
            {y[0], y[1], y[2], 0},
            {z[0], z[1], z[2], 0},
            {0.,   0.,   0.,   1.}
    };
    return R * T;
}

Mat4d getViewportMat(int w, int h)
{
    return {
            {double(w) / 2, 0,             0, double(w) / 2},
            {0,             double(h) / 2, 0, double(h) / 2},
            {0,             0,             1, 0},
            {0,             0,             0, 1},
    };
}


//Inside triangle
Vec3d computeBarycentric2D(double x, double y, Vec4d *v)
{
    double c1 = (x * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * y + v[1].x() * v[2].y() - v[2].x() * v[1].y()) /
                (v[0].x() * (v[1].y() - v[2].y()) + (v[2].x() - v[1].x()) * v[0].y() + v[1].x() * v[2].y() -
                 v[2].x() * v[1].y());
    double c2 = (x * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * y + v[2].x() * v[0].y() - v[0].x() * v[2].y()) /
                (v[1].x() * (v[2].y() - v[0].y()) + (v[0].x() - v[2].x()) * v[1].y() + v[2].x() * v[0].y() -
                 v[0].x() * v[2].y());
    double c3 = (x * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * y + v[0].x() * v[1].y() - v[1].x() * v[0].y()) /
                (v[2].x() * (v[0].y() - v[1].y()) + (v[1].x() - v[0].x()) * v[2].y() + v[0].x() * v[1].y() -
                 v[1].x() * v[0].y());
    return {c1, c2, c3};
}


//Bresenham’s Line
void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        } else
        {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}


template<typename T>
T interpolation(vector<T> a, Vec3d proportion)
{
    return a[0] * proportion[0] + a[1] * proportion[1] + a[2] * proportion[2];
}

//PCF
float getVisibility(const Vec3d &pointWorld, const Mat4d &lightViewMat,
                    const Mat4d &lightPMat, const vector<vector<float>> &depthBuffer, int width, int height,
                    int filterSize = 1)
{
    float bios = 0.015;
    Vec4d point = {pointWorld[0], pointWorld[1], pointWorld[2], 1.};
    point = lightPMat * lightViewMat * point;
    point = point * (1 / point[3]);
    int X = ceil((point[0] + 1) / 2 * width);
    int Y = ceil((point[1] + 1) / 2 * height);
    float totalCount = 0;
    float count = 0;
    int k = filterSize / 2;
    for (int x = X - k; x <= X + k; ++x)
    {
        for (int y = Y - k; y <= Y + k; ++y)
        {
            if (x >= 0 && y >= 0 && x < width && y < width)
            {
                totalCount++;
                float z = depthBuffer[x][y];
                if (point[2] - bios < z)
                    count++;
            }
        }
    }
    return count / totalCount;
}

