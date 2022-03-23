//
// Created by 滕逸青 on 2021/10/27.
//

// 这里的向量都是列向量
// 矩阵都是方阵

#ifndef MYTINYRENDER_MYMATH_HPP
#define MYTINYRENDER_MYMATH_HPP

#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

template <typename T, int DIM>
class vec
{
private:
    T _data[DIM];

public:
    vec(T *data)
    {
        //        _data = new ValueType[DIM];
        for (int i = 0; i < DIM; i++)
            _data[i] = data[i];
    }

    vec()
    {
        //        _data = new ValueType[DIM];
        for (int i = 0; i < DIM; i++)
        {
            _data[i] = T(0);
        }
    }

    T &operator[](int index)
    {
        assert(index >= 0);
        assert(index < DIM);

        return _data[index];
    }

    const T &operator[](int index) const
    {
        assert(index >= 0);
        assert(index < DIM);

        return _data[index];
    }
};

template <typename ValueType>
class vec<ValueType, 3>
{
private:
    ValueType _x;
    ValueType _y;
    ValueType _z;

public:
    vec()
    {
        _x = ValueType(0);
        _y = ValueType(0);
        _z = ValueType(0);
    }

    template <typename U>
    vec(initializer_list<U> il)
    {
        auto it = begin(il);
        _x = ValueType(*(it++));
        _y = ValueType(*(it++));
        _z = ValueType(*(it++));
    }

    vec operator+(double i)
    {
        return {_x + i, _y + i, _z + i};
    }

    vec(ValueType *data)
    {
        _x = data[0];
        _y = data[1];
        _z = data[2];
    }

    template <typename U>
    vec(vec<U, 3> &v) : _x(ValueType(v[0])), _y(ValueType(v[1])), _z(ValueType(v[2]))
    {
    }

    template <typename U>
    vec<ValueType, 3> &operator=(vec<U, 3> v)
    {
        _x = ValueType(v[0]);
        _y = ValueType(v[1]);
        _z = ValueType(v[2]);
        return *this;
    }

    ValueType &operator[](int index)
    {
        assert(index >= 0);
        assert(index < 3);
        if (index == 0)
            return _x;
        if (index == 1)
            return _y;
        if (index == 2)
            return _z;
    };

    const ValueType &operator[](int index) const
    {
        assert(index >= 0);
        assert(index < 3);
        if (index == 0)
            return _x;
        if (index == 1)
            return _y;
        if (index == 2)
            return _z;
    };

    ValueType &x()
    {
        return _x;
    }

    ValueType &y()
    {
        return _y;
    }

    ValueType &z()
    {
        return _z;
    }

    vec<ValueType, 3> operator+(const vec<ValueType, 3> &b) const
    {
        return {
            _x + ValueType(b[0]), _y + ValueType(b[1]), _z + ValueType(b[2])};
    }

    vec<ValueType, 3> operator-(const vec<ValueType, 3> &b) const
    {
        return {
            _x - ValueType(b[0]), _y - ValueType(b[1]), _z - ValueType(b[2])};
    }

    void normalize()
    {
        double length = sqrt(_x * _x + _z * _z + _y * _y);
        _x /= length;
        _y /= length;
        _z /= length;
    }

    ValueType operator*(vec<ValueType, 3> b) const
    {
        return _x * ValueType(b[0]) + _y * ValueType(b[1]) + _z * ValueType(b[2]);
    }

    vec<ValueType, 3> operator*(ValueType b) const
    {
        return {ValueType(_x * b), ValueType(_y * b), ValueType(_z * b)};
    }

    vec<ValueType, 3> cross(const vec<ValueType, 3> &b) const
    {
        ValueType x = b[0], y = b[1], z = b[2];
        return {
            -(_y * z - _z * y), -(_z * x - _x * z), -(_x * y - x * _y)};
    }

    friend ostream &operator<<(ostream &out, const vec<ValueType, 3> &v)
    {
        return out << v._x << " " << v._y << " " << v._z << endl;
    }
};

template <typename ValueType>
class vec<ValueType, 2>
{
private:
    ValueType _x;
    ValueType _y;

public:
    vec()
    {
        _x = ValueType(0);
        _y = ValueType(0);
    }

    template <typename U>
    vec(initializer_list<U> il)
    {
        auto it = begin(il);
        _x = ValueType(*(it++));
        _y = ValueType(*(it++));
        ValueType(*(it++));
    }

    vec(ValueType *data)
    {
        _x = data[0];
        _y = data[1];
    }

    template <typename U>
    vec(vec<U, 3> v) : _x(ValueType(v[0])), _y(ValueType(v[1]))
    {
    }

    template <typename U>
    vec<ValueType, 3> &operator=(vec<U, 3> v)
    {
        _x = ValueType(v[0]);
        _y = ValueType(v[1]);
        return *this;
    }

    ValueType &operator[](int index)
    {
        assert(index >= 0);
        assert(index < 2);
        if (index == 0)
            return _x;
        if (index == 1)
            return _y;
    };

    const ValueType &operator[](int index) const
    {
        assert(index >= 0);
        assert(index < 2);
        if (index == 0)
            return _x;
        if (index == 1)
            return _y;
    };

    ValueType &x()
    {
        return _x;
    }

    ValueType &y()
    {
        return _y;
    }

    vec operator+(double i)
    {
        return {_x + i, _y + i};
    }

    vec<ValueType, 2> operator+(const vec<ValueType, 2> &b) const
    {
        return {
            _x + ValueType(b[0]), _y + ValueType(b[1])};
    }

    vec<ValueType, 2> operator-(const vec<ValueType, 2> &b) const
    {
        return {
            _x - ValueType(b.x()), _y - ValueType(b.y())};
    }

    ValueType operator*(const vec<ValueType, 2> &b) const
    {
        return _x * ValueType(b[0]) + _y * ValueType(b[1]);
    }

    vec<ValueType, 2> operator*(ValueType b) const
    {
        return {ValueType(_x * b), ValueType(_y * b)};
    }

    friend ostream &operator<<(ostream &out, vec<ValueType, 2> v)
    {
        return out << v._x << " " << v._y << endl;
    }
};

template <typename ValueType>
class vec<ValueType, 4>
{
private:
    ValueType _x;
    ValueType _y;
    ValueType _z;
    ValueType _w;

public:
    vec()
    {
        _x = ValueType(0);
        _y = ValueType(0);
        _z = ValueType(0);
        _w = ValueType(0);
    }

    vec(initializer_list<ValueType> il)
    {
        auto it = begin(il);
        _x = *(it++);
        _y = *(it++);
        _z = *(it++);
        _w = *(it);
    }

    vec(ValueType *data)
    {
        _x = data[0];
        _y = data[1];
        _z = data[2];
        _w = data[3];
    }

    template <typename U>
    vec(vec<U, 4> v) : _x(ValueType(v[0])), _y(ValueType(v[1])), _z(ValueType(v[2])), _w(ValueType(v[3]))
    {
    }

    template <typename U>
    vec<ValueType, 4> &operator=(vec<U, 4> v)
    {
        _x = ValueType(v[0]);
        _y = ValueType(v[1]);
        _z = ValueType(v[2]);
        _w = ValueType(v[3]);
        return *this;
    }

    ValueType &operator[](int index)
    {
        assert(index >= 0);
        assert(index < 4);
        if (index == 0)
            return _x;
        if (index == 1)
            return _y;
        if (index == 2)
            return _z;
        if (index == 3)
            return _w;
    };

    const ValueType &operator[](int index) const
    {
        assert(index >= 0);
        assert(index < 4);
        if (index == 0)
            return _x;
        if (index == 1)
            return _y;
        if (index == 2)
            return _z;
        if (index == 3)
            return _w;
    };

    ValueType &x()
    {
        return _x;
    }

    ValueType &y()
    {
        return _y;
    }

    ValueType &z()
    {
        return _z;
    }

    ValueType &w()
    {
        return _w;
    }

    vec operator+(double i)
    {
        return {_x + i, _y + i, _z + i, _w + i};
    }

    template <typename U>
    vec<ValueType, 4> operator+(vec<U, 4> b) const
    {
        return {
            _x + ValueType(b.x()), _y + ValueType(b.y()), _z + ValueType(b.z()), _w + ValueType(b.w())};
    }

    template <typename U>
    vec<ValueType, 4> operator-(const vec<U, 4> &b) const
    {
        return {
            _x - ValueType(b.x()), _y - ValueType(b.y()), _z - ValueType(b.z()), _w - ValueType(b.w())};
    }

    ValueType operator*(const vec<ValueType, 4> &b) const
    {
        return _x * (b[0]) + _y * (b[1]) + _z * (b[2]) + _w * (b[3]);
    }

    template <typename U>
    vec<ValueType, 4> operator*(U b) const
    {
        return {ValueType(_x * b), ValueType(_y * b), ValueType(_z * b), ValueType(_w * b)};
    }

    friend ostream &operator<<(ostream &out, vec v)
    {
        return out << v._x << " " << v._y << " " << v._z << " " << v._w << endl;
    }
};

// TODO:由于 mat<ValueType,DIM-1>a 声明不合法,考虑将 DIM 参数作为成员变量
template <typename ValueType, int DIM>
class mat
{
private:
    vec<ValueType, DIM> _matrix[DIM];

public:
    static mat identity()
    {
        mat tmp;
        for (int i = 0; i < DIM; ++i)
            tmp[i][i] = ValueType(1);
        return tmp;
    }

    vec<ValueType, DIM> &operator[](int index)
    {
        assert(index >= 0);
        assert(index < DIM);
        return _matrix[index];
    }

    const vec<ValueType, DIM> &operator[](int index) const
    {
        assert(index >= 0);
        assert(index < DIM);
        return _matrix[index];
    }

    mat(initializer_list<initializer_list<ValueType>> il)
    {
        auto it = begin(il);
        for (int i = 0; i < DIM; i++)
            _matrix[i] = *(it + i);
    }

    mat(const mat<ValueType, DIM> &m)
    {
        for (int i = 0; i < DIM; i++)
            _matrix[i] = m[i];
    }

    mat operator=(mat<ValueType, DIM> m)
    {
        for (int i = 0; i < DIM; i++)
            _matrix[i] = m[i];
    }

    mat()
    {
        for (int i = 0; i < DIM; ++i)
        {
            _matrix[i] = vec<ValueType, DIM>();
        }
    }

    mat operator+(const mat &m) const
    {
        mat res;
        for (int i = 0; i < DIM; i++)
        {
            res[i] = _matrix[i] + m[i];
        }
        return res;
    }

    mat operator-(const mat &m) const
    {
        mat res;
        for (int i = 0; i < DIM; i++)
        {

            res[i] = _matrix[i] - m[i];
        }
        return res;
    }

    vec<ValueType, DIM> operator*(const vec<ValueType, DIM> &v) const
    {
        vec<ValueType, DIM> res;
        for (int i = 0; i < DIM; i++)
        {
            res[i] = _matrix[i] * v;
        }
        return res;
    }

    mat operator*(const mat<ValueType, DIM> &m) const
    {
        mat res;
        for (int i = 0; i < DIM; i++)
        {
            for (int j = 0; j < DIM; j++)
            {
                res[i][j] = _matrix[i] * m.getCol(j);
            }
        }
        return res;
    }

    mat operator*(ValueType u) const
    {
        mat res;
        for (int i = 0; i < DIM; i++)
        {
            res[i] = _matrix[i] * u;
        }
        return res;
    }

    vec<ValueType, DIM> getCol(int index) const
    {
        assert(index >= 0);
        assert(index < DIM);
        vec<ValueType, DIM> col;
        for (int i = 0; i < DIM; i++)
            col[i] = _matrix[i][index];
        return col;
    }

    ///计算矩阵对应的行列式的值(只需要实现 1-4 维)
    /// \return
    ValueType getDet() const
    {
        assert(DIM < 5);
        if (DIM == 1)
        {
            return _matrix[0][0];
        }
        if (DIM == 2)
        {
            return _matrix[0][0] * _matrix[1][1] - _matrix[0][1] * _matrix[1][0];
        }
        if (DIM == 3)
        {
            return (_matrix[0][0] * _matrix[1][1] * _matrix[2][2] + _matrix[0][1] * _matrix[1][2] * _matrix[2][0] +
                    _matrix[0][2] * _matrix[1][0] * _matrix[2][1]) -
                   (_matrix[0][2] * _matrix[1][1] * _matrix[2][0] + _matrix[1][2] * _matrix[0][0] * _matrix[2][1] +
                    _matrix[0][1] * _matrix[2][2] * _matrix[1][0]);
        }
        if (DIM == 4)
        {
            ValueType res = 0;
            for (int i = 0; i < 4; i++)
            {
                int a = _matrix[0][i];
                mat<ValueType, 3> tmp;
                for (int j = 1; j < 4; ++j)
                {
                    int col = 0;
                    for (int k = 0; k < 4; k++)
                    {
                        if (k == i)
                            continue;
                        tmp[j - 1][col++] = _matrix[j][k];
                    }
                }
                if (i % 2 == 0)
                {
                    res += a * tmp.getDet();
                }
                else
                {
                    res -= a * tmp.getDet();
                }
            }
            return res;
        }
    }

    mat<double, DIM> getAdjoint() const
    {
        mat<double, DIM> adj;
        if (DIM == 1)
        {
            return {{1}};
        }
        if (DIM == 2)
        {
            adj = {
                {double(_matrix[1][1]), double(-_matrix[0][1])},
                {double(-_matrix[1][0]), double(_matrix[0][0])}};
        }
        if (DIM == 3)
        {
            for (int i = 0; i < DIM; i++)
            {
                for (int j = 0; j < DIM; j++)
                {
                    mat<ValueType, 2> tmp;
                    int a = 0, b = 0;
                    for (int row = 0; row < DIM; row++)
                    {
                        if (row == i)
                            continue;
                        for (int col = 0; col < DIM; col++)
                        {
                            if (col == j)
                                continue;
                            tmp[a][b++] = _matrix[row][col];
                        }
                        b = 0;
                        ++a;
                    }
                    auto tmpDel = tmp.getDet();
                    if ((i + j) % 2 == 1)
                        tmpDel = -tmpDel;
                    adj[j][i] = tmpDel;
                }
            }
        }
        if (DIM == 4)
        {
            for (int i = 0; i < DIM; i++)
            {
                for (int j = 0; j < DIM; j++)
                {
                    mat<ValueType, 3> tmp;
                    int a = 0, b = 0;
                    for (int row = 0; row < DIM; row++)
                    {
                        if (row == i)
                            continue;
                        for (int col = 0; col < DIM; col++)
                        {
                            if (col == j)
                                continue;
                            tmp[a][b++] = _matrix[row][col];
                        }
                        b = 0;
                        ++a;
                    }
                    auto tmpDel = tmp.getDet();
                    if ((i + j) % 2 == 1)
                        tmpDel = -tmpDel;
                    adj[j][i] = tmpDel;
                }
            }
        }
        return adj;
    }

    /// 矩阵求逆(伴随矩阵)
    /// TODO: 使用LU分解法实现该算法
    /// \return
    mat<double, DIM> getInvert() const
    {
        assert(DIM < 5);
        double del = getDet();
        assert(del != 0);
        if (DIM == 1)
            return {{1 / del}};
        //求矩阵的伴随矩阵
        mat<double, DIM> adj = getAdjoint();
        return adj * (1 / del);
    }

    /// 返回矩阵的转置
    /// \return
    mat getTranspose() const
    {
        mat<ValueType, DIM> res;
        for (int i = 0; i < DIM; ++i)
        {
            for (int j = 0; j < DIM; ++j)
            {
                res[i][j] = _matrix[j][i];
            }
        }
        return res;
    }

    friend ostream &operator<<(ostream &out, const mat &m)
    {
        for (int i = 0; i < DIM; i++)
        {
            out << m[i];
        }
        return out;
    }
};

typedef vec<int, 3> Vec3i;
typedef vec<double, 3> Vec3d;
typedef vec<int, 4> Vec4i;
typedef vec<double, 4> Vec4d;
typedef vec<int, 2> Vec2i;
typedef vec<double, 2> Vec2d;
typedef mat<int, 4> Mat4i;
typedef mat<double, 4> Mat4d;
typedef mat<int, 3> Mat3i;
typedef mat<double, 3> Mat3d;

#endif // MYTINYRENDER_MYMATH_HPP
