#pragma once

#include "Vector3.h"
#include "Matrix4.h"

namespace MathUtility {

const float PI = 3.141592654f;

// 零ベクトルを返す
const Vector3 Vector3Zero();
// 2ベクトルが一致しているか調べる
bool Vector3Equal(const Vector3& v1, const Vector3& v2);
// ノルム(長さ)を求める
float Vector3Length(const Vector3& v);
// 正規化する
Vector3& Vector3Normalize(Vector3& v);
// 内積を求める
float Vector3Dot(const Vector3& v1, const Vector3& v2);
// 外積を求める
Vector3 Vector3Cross(const Vector3& v1, const Vector3& v2);

// 2項演算子オーバーロード
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

// 単位行列を求める
Matrix4 Matrix4Identity();
// 転置行列を求める
Matrix4 Matrix4Transpose(const Matrix4& m);

// 拡大縮小行列の作成
Matrix4 Matrix4Scaling(float sx, float sy, float sz);

// 回転行列の作成
Matrix4 Matrix4RotationX(float angle);
Matrix4 Matrix4RotationY(float angle);
Matrix4 Matrix4RotationZ(float angle);

// 平行移動行列の作成
Matrix4 Matrix4Translation(float tx, float ty, float tz);

// ビュー行列の作成
Matrix4 Matrix4LookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
// 並行投影行列の作成
Matrix4 Matrix4Orthographic(
  float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
// 透視投影行列の作成
Matrix4 Matrix4Perspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);

// 座標変換（w除算なし）
Vector3 Vector3Transform(const Vector3& v, const Matrix4& m);
// 座標変換（w除算あり）
Vector3 Vector3TransformCoord(const Vector3& v, const Matrix4& m);
// ベクトル変換
Vector3 Vector3TransformNormal(const Vector3& v, const Matrix4& m);

// 2項演算子オーバーロード
Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Vector3 operator*(const Vector3& v, const Matrix4& m);

} // namespace MathUtility