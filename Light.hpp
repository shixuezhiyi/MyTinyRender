#include "MyMath.hpp"
#include "Tgaimage.h"


#define P 2

class Light
{
public:
    Vec3d _pos;
    double _power;
    Vec3d _lightDir;

    Light(const Vec3d &pos, const Vec3d &target, double power = 1) : _pos(pos), _power(power)
    {
        _lightDir = target - pos;
        _lightDir.normalize();
    }

    /// Blinn-Phong 着色模型:
    /// \param color
    /// \param pointPos
    /// \param pointNormal
    /// \param eyePos
    /// \param ambient
    /// \param spec
    /// \return
    TGAColor getColor(const TGAColor &color, const Vec3d &pointPos, const Vec3d &pointNormal,
                      const Vec3d &eyePos, const double ambient, const double spec, float visibility)
    {
        Vec3d eyeDir = eyePos - pointPos;
        eyeDir.normalize();
        Vec3d h = (_lightDir + eyeDir) * 0.5;
        h.normalize();
        double L = 0.;
        L += ambient;
        L += 0.75 * _power * max(0., -(pointNormal * _lightDir)) * visibility;
        L += 0.35 * _power * (pow(max(0., -(h * pointNormal)), 8) * spec) * visibility;
        Vec3d res;
        for (int i = 0; i < 3; i++)
        {
            res[i] = double(color[i]) * L;
        }
        TGAColor resColor;
        resColor[3] = color[3];
        for (int i = 0; i < 3; i++)
        {
            resColor[i] = min(255, int(res[i]));
        }
        return resColor;
    }
};