#include "Vector.h"
// #include <D3dx9math.h>
#include "Data.h"
#include "Offset.h"
#include "SrcVmm.h"
#include "srcUtils.h"

D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2) {
    D3DMATRIX pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

    return pOut;
}

D3DMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0)) {
    float radPitch = (rot.x * float(M_PI) / 180.f);
    float radYaw = (rot.y * float(M_PI) / 180.f);
    float radRoll = (rot.z * float(M_PI) / 180.f);

    float SP = sinf(radPitch);
    float CP = cosf(radPitch);
    float SY = sinf(radYaw);
    float CY = cosf(radYaw);
    float SR = sinf(radRoll);
    float CR = cosf(radRoll);

    D3DMATRIX matrix;
    matrix.m[0][0] = CP * CY;
    matrix.m[0][1] = CP * SY;
    matrix.m[0][2] = SP;
    matrix.m[0][3] = 0.f;

    matrix.m[1][0] = SR * SP * CY - CR * SY;
    matrix.m[1][1] = SR * SP * SY + CR * CY;
    matrix.m[1][2] = -SR * CP;
    matrix.m[1][3] = 0.f;

    matrix.m[2][0] = -(CR * SP * CY + SR * SY);
    matrix.m[2][1] = CY * SR - CR * SP * SY;
    matrix.m[2][2] = CR * CP;
    matrix.m[2][3] = 0.f;

    matrix.m[3][0] = origin.x;
    matrix.m[3][1] = origin.y;
    matrix.m[3][2] = origin.z;
    matrix.m[3][3] = 1.f;

    return matrix;
}

Vector3 VectorHelper::WorldToScreen(Vector3 WorldLocation) {
    Vector3 Screenlocation = Vector3(0, 0, 0);

    Vector3 Rotation = gameData.Rotation;

    D3DMATRIX tempMatrix = Matrix(Rotation);

    Vector3 vAxisX, vAxisY, vAxisZ;

    vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

    Vector3 vDelta = WorldLocation - gameData.Location;
    Vector3 vTransformed = Vector3(vDelta * vAxisY, vDelta * vAxisZ, vDelta * vAxisX);

    if (vTransformed.z < 1.f) vTransformed.z = 1.f;

    float FovAngle = gameData.FOV;
    float ScreenCenterX = gameData.ScreenWidth / 2.0f;
    float ScreenCenterY = gameData.ScreenHeight / 2.0f;

    Screenlocation.x =
        ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
    Screenlocation.y =
        ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

    return Screenlocation;
}

void VectorHelper::WorldToScreen(Vector3 WorldLocation, Vector3* pOut, float* DeltaInMeters) {
    Vector3 Screenlocation = Vector3(0, 0, 0);

    Vector3 Rotation = gameData.Rotation;

    D3DMATRIX tempMatrix = Matrix(Rotation);

    Vector3 vAxisX, vAxisY, vAxisZ;

    vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

    Vector3 vDelta = WorldLocation - gameData.Location;
    Vector3 vTransformed = Vector3(vDelta * vAxisY, vDelta * vAxisZ, vDelta * vAxisX);

    if (vTransformed.z < 1.f) vTransformed.z = 1.f;
    if (DeltaInMeters != NULL) {
        *DeltaInMeters = vTransformed.z / 100;
    }

    float FovAngle = gameData.FOV;
    float ScreenCenterX = gameData.ScreenWidth / 2.0f;
    float ScreenCenterY = gameData.ScreenHeight / 2.0f;

    Screenlocation.x =
        ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
    Screenlocation.y =
        ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

    pOut->x = Screenlocation.x;
    pOut->y = Screenlocation.y;
    pOut->z = 0;
}

Vector3 VectorHelper::WorldToScreenAim(Vector3 WorldLocation) {
    ULONG64 AnimScriptInstance = VmmCore::ReadValue<ULONG64>(gameData.LocalPlayerMesh + Offset::AnimScriptInstance);
    Vector3 Rotation = VmmCore::ReadValue<Vector3>(AnimScriptInstance + Offset::ControlRotation_CP) +
                       VmmCore::ReadValue<Vector3>(AnimScriptInstance + Offset::RecoilADSRotation_CP);
    Vector3 Screenlocation = Vector3(0, 0, 0);
    Utils::Log(2, " %f | %f | %f", Rotation.x, Rotation.y, Rotation.z);
    D3DMATRIX tempMatrix = Matrix(Rotation);

    Vector3 vAxisX, vAxisY, vAxisZ;

    vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
    vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
    vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

    Vector3 vDelta = WorldLocation - gameData.Location;
    Vector3 vTransformed = Vector3(vDelta * vAxisY, vDelta * vAxisZ, vDelta * vAxisX);

    if (vTransformed.z < 1.f) vTransformed.z = 1.f;

    float FovAngle = gameData.FOV;
    float ScreenCenterX = gameData.ScreenWidth / 2.0f;
    float ScreenCenterY = gameData.ScreenHeight / 2.0f;

    Screenlocation.x =
        ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
    Screenlocation.y =
        ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

    return Screenlocation;
}

D3DMATRIX ToMatrixWithScale(FQuat Rotation, FVector Translation, FVector Scale3D) {
    D3DMATRIX m;

    m._41 = Translation.X;
    m._42 = Translation.Y;
    m._43 = Translation.Z;

    float x2 = Rotation.X + Rotation.X;
    float y2 = Rotation.Y + Rotation.Y;
    float z2 = Rotation.Z + Rotation.Z;

    float xx2 = Rotation.X * x2;
    float yy2 = Rotation.Y * y2;
    float zz2 = Rotation.Z * z2;
    m._11 = (1.0f - (yy2 + zz2)) * Scale3D.X;
    m._22 = (1.0f - (xx2 + zz2)) * Scale3D.Y;
    m._33 = (1.0f - (xx2 + yy2)) * Scale3D.Z;

    float yz2 = Rotation.Y * z2;
    float wx2 = Rotation.W * x2;
    m._32 = (yz2 - wx2) * Scale3D.Z;
    m._23 = (yz2 + wx2) * Scale3D.Y;

    float xy2 = Rotation.X * y2;
    float wz2 = Rotation.W * z2;
    m._21 = (xy2 - wz2) * Scale3D.Y;
    m._12 = (xy2 + wz2) * Scale3D.X;

    float xz2 = Rotation.X * z2;
    float wy2 = Rotation.W * y2;
    m._31 = (xz2 + wy2) * Scale3D.Z;
    m._13 = (xz2 - wy2) * Scale3D.X;

    m._14 = 0.0f;
    m._24 = 0.0f;
    m._34 = 0.0f;
    m._44 = 1.0f;
    return m;
}
D3DMATRIX D3DMATRIXMultiply(D3DMATRIX pM1, D3DMATRIX pM2) {
    D3DMATRIX pOut;
    pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
    pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
    pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
    pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
    pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
    pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
    pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
    pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
    pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
    pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
    pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
    pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
    pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
    pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
    pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
    pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
    return pOut;
}

Vector3 VectorHelper::GetBoneWithRotation(FTransform BoneArray, FTransform ComponentToWorld) {
    // FMatrix Matrix;
    // Matrix = MatrixMultiplication(BoneArray.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
    D3DMATRIX BoneMatrix = ToMatrixWithScale(BoneArray.Rotation, BoneArray.Translation, BoneArray.Scale3D);
    D3DMATRIX ComponentToWorldMatrix =
        ToMatrixWithScale(ComponentToWorld.Rotation, ComponentToWorld.Translation, ComponentToWorld.Scale3D);
    D3DMATRIX NewMatrix = D3DMATRIXMultiply(BoneMatrix, ComponentToWorldMatrix);
    // return Vector3(Matrix._41, Matrix._42, Matrix._43);
    return Vector3(NewMatrix._41, NewMatrix._42, NewMatrix._43);
}