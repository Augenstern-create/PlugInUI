#pragma once

#include "SrcUe4Math.h"

typedef struct _D3DMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DMATRIX;

class Vector3 {
   public:
    float x, y, z;
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    // Vector(const FVector& v) :x(v.X), y(v.Y), z(v.Z) {}

    float InnerProduct(const Vector3& v) const { return (x * v.x) + (y * v.y) + (z * v.z); }

    Vector3 OuterProduct(const Vector3& v) const {
        Vector3 output;
        output.x = (y * v.z) - (z * v.y);
        output.y = (z * v.x) - (x * v.z);
        output.z = (x * v.y) - (y * v.x);
        return output;
    }

    Vector3 Min(const Vector3& v) const {
        Vector3 output;
        output.x = x < v.x ? x : v.x;
        output.y = y < v.y ? y : v.y;
        output.z = z < v.z ? z : v.z;
        return output;
    }

    Vector3 Max(const Vector3& v) const {
        Vector3 output;
        output.x = x > v.x ? x : v.x;
        output.y = y > v.y ? y : v.y;
        output.z = z > v.z ? z : v.z;
        return output;
    }

    bool operator==(const Vector3& v) const { return x == v.x && y == v.y && z == v.z; }

    bool operator!=(const Vector3& v) const { return !(*this == v); }

    Vector3 operator-() const { return Vector3(-x, -y, -z); }

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }

    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }

    Vector3 operator*(const float fValue) const { return Vector3(x * fValue, y * fValue, z * fValue); }

    Vector3 operator/(const float fValue) const { return Vector3(x / fValue, y / fValue, z / fValue); }

    void Normalize() {
        float factor = 1.0f / sqrtf(x * x + y * y + z * z);
        (*this) = (*this) * factor;
    }

    float Length() const { return sqrtf(x * x + y * y + z * z); }

    float Distance(const Vector3& v) const { return (v - *this).Length(); }

    Vector3 operator^(const Vector3& v) const { return OuterProduct(v); }

    float operator*(const Vector3& v) const { return InnerProduct(v); }
};

struct FText {
    char buffer[256];
};

class VectorHelper {
   public:
    // static Vector3 WorldToScreen(Vector3 WorldLocation);

    // static void WorldToScreen(Vector3 WorldLocation, Vector3* pOut, float* DeltaInMeters);

    // static Vector3 WorldToScreenAim(Vector3 WorldLocation);

    static Vector3 GetBoneWithRotation(FTransform BoneArray, FTransform ComponentToWorld);
};