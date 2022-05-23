#pragma once
#include "Vector3.h"

/// <summary>
/// 行列
/// </summary>
class Matrix4 {
  public:
	// 行x列
	float m[4][4];

	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
	  float m00, float m01, float m02, float m03,
	  float m10, float m11, float m12, float m13,
	  float m20, float m21, float m22, float m23,
	  float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);

	static Matrix4 Identity();

	static Matrix4 Translation(float x, float y, float z);
	static Matrix4 Scaling(float x, float y, float z);

	static Matrix4 RotationX(float rad);
	static Matrix4 RotationY(float rad);
	static Matrix4 RotationZ(float rad);

	static Matrix4 RotationZXY(float radX, float radY, float radZ);
	
	static Matrix4 View(Vector3 eye, Vector3 target, Vector3 up);

	static Matrix4 OrthoGraphicProjection(float left, float right, float top, float bottom, float nearZ, float farZ);
	static Matrix4 PerspectiveProjection(float fov, float aspect, float nearZ, float farZ);

};
