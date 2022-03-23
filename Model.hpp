//
// Created by 滕逸青 on 2021/10/29.
//

#ifndef MYTINYRENDER_OBJECTION_H
#define MYTINYRENDER_OBJECTION_H

#include <string>
#include <sstream>
#include "MyMath.hpp"
#include "Tgaimage.h"
#include <vector>

using namespace std;

class Model
{
private:
    vector<Vec3d> _pCoords; /// 点在空间中的坐标[-1,1]
    vector<Vec3d> _normals;
    vector<Vec2d> _texCoords; /// 点的纹理坐标[0,1]
    vector<Vec3i> _vIdx;      /// 三角形三个点对应在 pCoord 的下标
    vector<Vec3i> _vtIdx;     /// 三角形三个点对应在 texCoord 的下标
    vector<Vec3i> _vnIdx;     /// 三角形三个点对应在 _normals 的下标

    TGAImage _diffuseMap;
    TGAImage _normalMap;
    TGAImage _specularMap;

public:
    int _numsTriangle; ///三角形个数

    Model(string names)
    {
        string objPath = "../obj/" + names + "/" + names + ".obj";
        string texPath = "../obj/" + names + "/" + names + "_diffuse.tga";
        string normalPath = "../obj/" + names + "/" + names + "_nm_tangent.tga";
        string specPath = "../obj/" + names + "/" + names + "_spec.tga";
        _diffuseMap.read_tga_file(texPath.c_str());
        _normalMap.read_tga_file(normalPath.c_str());
        _specularMap.read_tga_file(specPath.c_str());
        ifstream in;
        in.open(objPath);
        if (in.fail())
        {
            cout << "Fail to load objPath:  " << objPath << endl;
            return;
        }

        while (!in.eof())
        {
            string line;
            getline(in, line);
            istringstream iss(line);
            if (line.compare(0, 2, "f ") == 0) //处理点的 Index
            {
                string trash;
                iss >> trash;
                Vec3i vIdx, vtIdx, vnIdx;
                for (int i = 0; i < 3; ++i)
                {
                    char tmp;
                    iss >> vIdx[i];
                    --vIdx[i];
                    iss >> tmp;
                    iss >> vtIdx[i];
                    --vtIdx[i];
                    iss >> tmp;
                    iss >> vnIdx[i];
                    --vnIdx[i];
                }
                _vIdx.push_back(vIdx);
                _vtIdx.push_back(vtIdx);
                _vnIdx.push_back(vnIdx);
            }
            else if (line.compare(0, 3, "vn ") == 0)
            {
                string trash;
                iss >> trash;
                Vec3d normal;
                iss >> normal[0] >> normal[1] >> normal[2];
                normal.normalize();
                _normals.push_back(normal);
            }
            else if (line.compare(0, 2, "v ") == 0)
            {
                string trash;
                iss >> trash;
                Vec3d position;
                iss >> position[0] >> position[1] >> position[2];
                _pCoords.push_back(position);
            }
            else if (line.compare(0, 3, "vt ") == 0)
            {
                string trash;
                iss >> trash;
                Vec2d vt;
                iss >> vt[0] >> vt[1];
                _texCoords.push_back(vt);
            }
        }
        _numsTriangle = _vIdx.size();
        cout << "Success to load objPath:  " << objPath << endl;
        cout << "Triangle num: " << _vIdx.size() << endl;
        cout << "Position nums: " << _pCoords.size() << endl;
        cout << "normal nums: " << _normals.size() << endl;
        cout << "texCoord num: " << _texCoords.size() << endl;
    }

    vector<Vec3d> getNormals(int index)
    {
        Vec3i idx = _vnIdx[index];
        return {_normals[idx[0]], _normals[idx[1]], _normals[idx[2]]};
    }

    vector<Vec2d> getTexCoords(int index)
    {
        Vec3i idx = _vtIdx[index];
        return {_texCoords[idx[0]], _texCoords[idx[1]], _texCoords[idx[2]]};
    }

    vector<Vec3d> getPosCoords(int index)
    {
        Vec3i idx = _vIdx[index];
        return {_pCoords[idx[0]], _pCoords[idx[1]], _pCoords[idx[2]]};
    }

    TGAColor getColor(double u, double v)
    {
        v = 1 - v;
        u = u * _diffuseMap.get_width();
        v = v * _diffuseMap.get_height();
        return _diffuseMap.get(floor(u), floor(v));
    }

    double getSpecular(double u, double v)
    {
        v = 1 - v;
        u = u * _specularMap.get_width();
        v = v * _specularMap.get_height();
        return _specularMap.get(floor(u), floor(v))[0] / 1.;
    }

    Vec3d getNormal(double u, double v, const Mat3d &TBN)
    {
        v = 1 - v;
        u = u * _normalMap.get_width();
        v = v * _normalMap.get_height();
        auto color = _normalMap.get(floor(u), floor(v));
        Vec3d tmp = {double(color[2]), double(color[1]), double(color[0])};
        tmp.normalize();
        tmp = tmp * 2 + (-1);
        Vec3d normal = TBN * tmp;
        normal.normalize();
        return normal;
    }
};

#endif // MYTINYRENDER_OBJECTION_H
