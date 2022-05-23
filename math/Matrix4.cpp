#include "Matrix4.h"
#include <math.h>

Matrix4 Matrix4::Identity()
{
	Matrix4 mat;
	mat.m[0][0] = 1;
	mat.m[1][1] = 1;
	mat.m[2][2] = 1;
	mat.m[3][3] = 1;
	return mat;
}

Matrix4 Matrix4::Translation(float x, float y, float z)
{
	Matrix4 mat = Matrix4::Identity();
	mat.m[3][0] = x;
	mat.m[3][1] = y;
	mat.m[3][2] = z;
	return mat;
}

Matrix4 Matrix4::Scaling(float x, float y, float z)
{
	Matrix4 mat = Matrix4::Identity();
	mat.m[0][0] = x;
	mat.m[1][1] = y;
	mat.m[2][2] = z;
	return mat;
}

Matrix4 Matrix4::RotationX(float rad)
{
	Matrix4 mat = Matrix4::Identity();
	mat.m[1][1] = cosf(rad);
	mat.m[1][2] = sinf(rad);
	mat.m[2][1] = -sinf(rad);
	mat.m[2][2] = cosf(rad);
	return mat;
}

Matrix4 Matrix4::RotationY(float rad)
{
	Matrix4 mat = Matrix4::Identity();
	mat.m[0][0] = cosf(rad);
	mat.m[0][2] = -sinf(rad);
	mat.m[2][0] = sinf(rad);
	mat.m[2][2] = cosf(rad);
	return mat;
}

Matrix4 Matrix4::RotationZ(float rad)
{
	Matrix4 mat = Matrix4::Identity();
	mat.m[0][0] = cosf(rad);
	mat.m[0][1] = sinf(rad);
	mat.m[1][0] = -sinf(rad);
	mat.m[1][1] = cosf(rad);
	return mat;
}

Matrix4 Matrix4::RotationZXY(float radX, float radY, float radZ)
{
	Matrix4 mat = Matrix4::Identity();
	mat *= RotationZ(radZ);
	mat *= RotationX(radX);
	mat *= RotationY(radY);
	return mat;
}