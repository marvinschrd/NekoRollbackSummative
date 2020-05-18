//
// Created by unite on 03/05/2020.
//

#include "mathematics/vector.h"
#include "mathematics/matrix.h"
#include "mathematics/transform.h"
#include "mathematics/const.h"
#include "mathematics/angle.h"
#include "mathematics/quaternion.h"

namespace neko::Transform3d
{


Mat4f const TranslationMatrixFrom(const Vec3f translation)
{
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1.0f, 0, 0, 0),
                            Vec4f(0, 1.0f, 0, 0),
                            Vec4f(0, 0, 1.0f, 0),
                            Vec4f(translation[0], translation[1], translation[2], 1)});
}


Mat4f const ScalingMatrixFrom(const Vec3f& scale)
{
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(scale[0], 0, 0, 0),
                            Vec4f(0, scale[1], 0, 0),
                            Vec4f(0, 0, scale[2], 0),
                            Vec4f(0, 0, 0, 1)});
}

Mat4f const RotationMatrixFrom(const degree_t angle, const Vec3f axis)
{
    const Vec3f normalizedAxis = axis.Normalized();

    const float x = normalizedAxis[0];
    const float y = normalizedAxis[1];
    const float z = normalizedAxis[2];
    const float c = Cos(angle);
    const float s = Sin(angle);
    const float t = 1.0f - c;
    const float txx = t * x * x;
    const float tyy = t * y * y;
    const float tzz = t * z * z;
    const float txy = t * x * y;
    const float txz = t * x * z;
    const float tyz = t * y * z;
    const float sx = s * x;
    const float sy = s * y;
    const float sz = s * z;

    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(txx + c, txy - sz, txz + sy, 0),
                            Vec4f(txy + sz, tyy + c, tyz - sx, 0),
                            Vec4f(txz - sy, tyz + sx, tzz + c, 0),
                            Vec4f(0, 0, 0, 1)});
}


Mat4f const RotationMatrixFrom(const radian_t angle, const Vec3f axis)
{
    const Vec3f normalizedAxis = axis.Normalized();

    const float x = normalizedAxis[0];
    const float y = normalizedAxis[1];
    const float z = normalizedAxis[2];
    const float c = Cos(angle);
    const float s = Sin(angle);
    const float t = 1.0f - c;
    const float txx = t * x * x;
    const float tyy = t * y * y;
    const float tzz = t * z * z;
    const float txy = t * x * y;
    const float txz = t * x * z;
    const float tyz = t * y * z;
    const float sx = s * x;
    const float sy = s * y;
    const float sz = s * z;

    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(txx + c, txy - sz, txz + sy, 0),
                            Vec4f(txy + sz, tyy + c, tyz - sx, 0),
                            Vec4f(txz - sy, tyz + sx, tzz + c, 0),
                            Vec4f(0, 0, 0, 1)});
}


Mat4<float> const RotationMatrixFrom(const EulerAngles cardinalRotation)
{
    const radian_t rotX = cardinalRotation[0];
    const radian_t rotY = cardinalRotation[1];
    const radian_t rotZ = cardinalRotation[2];
    const float cosX = Cos(rotX);
    const float sinX = Sin(rotX);
    const float cosY = Cos(rotY);
    const float sinY = Sin(rotY);
    const float cosZ = Cos(rotZ);
    const float sinZ = Sin(rotZ);

    const Mat4f matX = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1, 0, 0, 0),
                            Vec4f(0, cosX, sinX, 0),
                            Vec4f(0, -sinX, cosX, 0),
                            Vec4f(0, 0, 0, 1)});
    const Mat4f matY = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(cosY, 0, 0, -sinY),
                            Vec4f(0, 1, 0, 0),
                            Vec4f(sinY, 0, 1, cosY),
                            Vec4f(0, 0, 0, 1)});
    const Mat4f matZ = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(cosZ, sinZ, 0, 0),
                            Vec4f(-sinZ, cosZ, 0, 0),
                            Vec4f(0, 0, 1, 0),
                            Vec4f(0, 0, 0, 1)});

    return matZ * matY * matX;
}



Mat4f const RotationMatrixFrom(const Quaternion& quaternion)
{
    const float x = quaternion.x;
    const float y = quaternion.y;
    const float z = quaternion.z;
    const float w = quaternion.w;
    const float xx = 2.0f * x * x;
    const float yy = 2.0f * y * y;
    const float zz = 2.0f * z * z;
    const float xy = 2.0f * z * y;
    const float xz = 2.0f * x * z;
    const float yz = 2.0f * y * z;
    const float zw = 2.0f * z * w;
    const float yw = 2.0f * y * w;
    const float xw = 2.0f * x * w;

    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1.0f - yy - zz, xy + zw, xz - yw, 0),
                            Vec4f(xy - zw, 1.0f - xx - zz, yz + xw, 0),
                            Vec4f(xz + yw, yz - xw, 1.0f - xx - yy, 0),
                            Vec4f(0, 0, 0, 1)
                    });
}




Mat4f Translate(const Mat4f& transform, const Vec3f translation)
{
    return TranslationMatrixFrom(translation) * transform;
}


Mat4f Scale(const Mat4f& transform, const Vec3f scale)
{
    return ScalingMatrixFrom(scale) * transform;
}


Mat4f Rotate(const Mat4f& transform, const degree_t angle, const Vec3f axis)
{
    return RotationMatrixFrom(angle, axis) * transform;
}


Mat4f Rotate(const Mat4f& transform, const radian_t angle, const Vec3f axis)
{
    return RotationMatrixFrom(angle, axis) * transform;
}


Mat4f Rotate(const Mat4f& transform, const Quaternion& quaternion)
{
    return RotationMatrixFrom(quaternion) * transform;
}


Mat4f Rotate(const Mat4f& transform, const EulerAngles eulerAngles)
{
    const Quaternion quaternion = Quaternion::FromEuler(eulerAngles);
    return RotationMatrixFrom(quaternion) * transform;
}



Mat4f Perspective(radian_t fovy, float aspect, float near, float far)
{
    neko_assert(fabsf(aspect - std::numeric_limits<float>::epsilon()) > 0.0f, "Aspect should not be zero");

    const float tanHalfFovy = Tan(fovy / 2.0f);
    Mat4f perspective{Mat4f::Zero};

    perspective[0][0] = 1.0f/ (aspect * tanHalfFovy);
    perspective[1][1] = 1.0f / (tanHalfFovy);
    perspective[2][2] = - (far + near) / (far - near);
    perspective[2][3] = - 1.0f;
    perspective[3][2] = - (2.0f * far * near) / (far - near);
    return perspective;
}
}