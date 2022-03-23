#include "Tgaimage.h"
#include "MyMath.hpp"
#include "Render.hpp"
#include "Model.hpp"
#include "Light.hpp"
#include "Constant.cpp"


int main(int argc, char **argv)
{
    auto viewportMat = getViewportMat(SCENE_WIDTH, SCENE_HEIGHT);
    auto modelMat = getModelMat();
    Model model(MODEL_NAME);
    Light light(LIGHT_POS, LIGHT_TARGET);
    //渲染深度图 为了阴影
    auto lightViewMat = getViewMat(light._pos, LIGHT_TARGET, EYE_UP);
    auto lightPrejectionMat = getOrthoMat(0.1, 3.5, -1.5, 1.5, -1.5, 1.5);
    vector<vector<float>> light_zBuffer(SCENE_WIDTH, vector<float>(SCENE_HEIGHT, std::numeric_limits<double>::max()));
    vector<vector<float>> zBuffer(SCENE_WIDTH, vector<float>(SCENE_HEIGHT, std::numeric_limits<double>::max()));
    TGAImage depthImage(SCENE_WIDTH, SCENE_HEIGHT, TGAImage::RGB);
    auto orthoMat = getOrthoMat(NEAR, FAR, LEFT, RIGHT, BOTTOM, TOP);
    auto perspectiveMat = getPerspectiveMat(NEAR, FAR, EYE_FOV, ASPECT);
    auto viewMat = getViewMat(EYE_POS, EYE_TARGET, EYE_UP);
    TGAImage targetImage(SCENE_WIDTH, SCENE_HEIGHT, TGAImage::RGB);
    for (int i = 0; i < model._numsTriangle; i++)
    {
        vector<Vec3d> tmps = model.getPosCoords(i);
        vector<Vec4d> lightPoints;
        vector<Vec4d> points;
        for (auto tmp: tmps)
        {
            Vec4d a = {tmp[0], tmp[1], tmp[2], 1.};
            points.push_back(a);
        }
        for (auto &p: points)
        {
            p = modelMat * p;
            lightPoints.push_back(p);
        }
        int light_xMin = INT_MAX;
        int light_yMin = INT_MAX;
        int light_xMax = INT_MIN;
        int light_yMax = INT_MIN;
        for (auto &p: lightPoints)
        {
            p = lightViewMat * p;
            p = lightPrejectionMat * p;
            double w = p[3];
            p = p * (1 / p[3]);
            p = viewportMat * p;
            p[3] = w;
            light_xMin = min(light_xMin, int(floor(p[0])));
            light_xMax = max(light_xMax, int(ceil(p[0])));
            light_yMin = min(light_yMin, int(floor(p[1])));
            light_yMax = max(light_yMax, int(ceil(p[1])));
        }
        light_xMin = max(light_xMin, 0);
        light_yMin = max(light_yMin, 0);
        light_xMax = min(light_xMax, depthImage.get_width() - 1);
        light_yMax = min(light_yMax, depthImage.get_height() - 1);
        for (int x = light_xMin; x <= light_xMax; ++x)
        {
            for (int y = light_yMin; y <= light_yMax; ++y)
            {
                Vec4d v[3] = {lightPoints[0], lightPoints[1], lightPoints[2]};
                Vec3d proportion = computeBarycentric2D(x, y, v);
                if (proportion[0] >= 0 && proportion[1] >= 0 && proportion[2] >= 0)
                {
                    float z = 0;
                    for (int i = 0; i < 3; i++)
                    {
                        z += proportion[i] * lightPoints[i][2];
                    }
                    if (z < light_zBuffer[x][y])
                    {
                        light_zBuffer[x][y] = z;
                        TGAColor color = TGAColor(255, 255, 255) * ((z + 1.) / 2);
                        depthImage.set(x, y, color);
                    }
                }
            }
        }
    }
    depthImage.flip_vertically(); // I want to have the origin at the left bottom corner of the image
    depthImage.write_tga_file("depth.tga");
    for (int i = 0; i < model._numsTriangle; i++)
    {
        vector<Vec3d> tmps = model.getPosCoords(i);
        vector<Vec2d> uvs = model.getTexCoords(i);
        vector<Vec3d> normals = model.getNormals(i);
        vector<Vec3d> pointsWorld;
        vector<Vec4d> points;
        for (auto tmp: tmps)
        {
            Vec4d a = {tmp[0], tmp[1], tmp[2], 1.};
            points.push_back(a);
        }
        int xMin = INT_MAX;
        int yMin = INT_MAX;
        int xMax = INT_MIN;
        int yMax = INT_MIN;
        for (auto &p: points)
        {
            p = modelMat * p;
            pointsWorld.push_back({p[0], p[1], p[2]});
            p = viewMat * p;
            p = perspectiveMat * p;
            double w = p[3];
            p = p * (1 / p[3]);
            p = viewportMat * p;
            p[3] = w;
            xMin = min(xMin, int(floor(p[0])));
            xMax = max(xMax, int(ceil(p[0])));
            yMin = min(yMin, int(floor(p[1])));
            yMax = max(yMax, int(ceil(p[1])));
        }
        for (auto &n: normals)
        {
            Vec4d tmp = {n[0], n[1], n[2], 0.};
            tmp = modelMat.getInvert().getTranspose() * tmp;
            n = {tmp[0], tmp[1], tmp[2]};
        }
        xMin = max(xMin, 0);
        yMin = max(yMin, 0);
        xMax = min(targetImage.get_width() - 1, xMax);
        yMax = min(targetImage.get_height() - 1, yMax);
        for (int x = xMin; x <= xMax; ++x)
        {
            for (int y = yMin; y <= yMax; ++y)
            {
                Vec4d v[3] = {points[0], points[1], points[2]};
                Vec3d proportion = computeBarycentric2D(x, y, v);
                if (proportion[0] >= 0 && proportion[1] >= 0 && proportion[2] >= 0)
                {
                    float z = 0;
                    for (int i = 0; i < 3; i++)
                    {
                        z += proportion[i] * points[i][2];
                    }
                    double p_w =
                            (points[0][3] * points[1][3] * points[2][3]) /
                            (proportion[0] * points[1][3] * points[2][3] +
                             proportion[1] * points[0][3] * points[2][3] +
                             proportion[2] * points[1][3] * points[0][3]);
                    for (int i = 0; i < 3; i++)
                        proportion[i] = proportion[i] * p_w / points[i][3];
                    if (z < zBuffer[x][y])
                    {
                        Vec3d pointWorld = interpolation(pointsWorld, proportion);
//                        float visibility=1;
                        float visibility = getVisibility(pointWorld, lightViewMat, lightPrejectionMat, light_zBuffer,
                                                         SCENE_WIDTH, SCENE_HEIGHT, 7);
                        Vec2d uv = interpolation(uvs, proportion);
                        //TBN
                        Vec3d N = interpolation(normals, proportion);
                        N.normalize();
                        Mat3d tmpMat3d;
                        tmpMat3d[0] = pointsWorld[1] - pointsWorld[0],
                        tmpMat3d[1] = pointsWorld[2] - pointsWorld[0],
                        tmpMat3d[2] = N;
                        tmpMat3d = tmpMat3d.getInvert();
                        Vec3d tmpVec3d1 = {uvs[1][0] - uvs[0][0], uvs[2][0] - uvs[0][0], 0.};
                        Vec3d tmpVec3d2 = {uvs[1][1] - uvs[0][1], uvs[2][1] - uvs[0][1], 0.};
                        Vec3d T = tmpMat3d * tmpVec3d1;
                        T.normalize();
                        ///TODO B=T cross N
                        Vec3d B = tmpMat3d * tmpVec3d2;
                        B.normalize();
                        Mat3d TBN
                                {
                                        {T[0], B[0], N[0]},
                                        {T[1], B[1], N[1]},
                                        {T[2], B[2], N[2]}
                                };
                        auto n = model.getNormal(uv[0], uv[1], TBN);
                        TGAColor color = light.getColor(model.getColor(uv[0], uv[1]), pointWorld, n, EYE_POS, AMBIENT,
                                                        model.getSpecular(uv[0], uv[1]), visibility);
                        targetImage.set(x, y, color);
                        zBuffer[x][y] = z;
                    }
                }
            }
        }
    }
    targetImage.flip_vertically(); // I want to have the origin at the left bottom corner of the image
    targetImage.write_tga_file(OUTPUT_FILENAME);
    return 0;
}
