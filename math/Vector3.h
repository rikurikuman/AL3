#pragma once

class Matrix4;

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 {
  public:
	float x; // x成分
	float y; // y成分
	float z; // z成分

  public:

	// コンストラクタ
	Vector3();                          // 零ベクトルとする
	Vector3(float x, float y, float z); // x成分, y成分, z成分 を指定しての生成
	  
	// 単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	// 代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);


	//ベクトルの足し算
	Vector3 operator+(const Vector3& a) const;
	//ベクトルの引き算
	Vector3 operator-(const Vector3& a) const;
	//定数倍
	Vector3 operator*(const float a) const;
	//定数分の1倍
	Vector3 operator/(const float a) const;

	//内積
	float Dot(const Vector3& a) const;
	//外積
	Vector3 Cross(const Vector3& a) const;

	//ベクトルの長さ（√を使うので重め）
	float Length() const;
	//ベクトルのべき乗長さ（√を使わないので軽め）
	float LengthSq() const;

	//正規化（このインスタンスを書き換える）
	Vector3& Normalize();
	//正規化ベクトルを取得
	//（このインスタンスは書き換えずに別のインスタンスとして取得する）
	Vector3 GetNormalize() const;

	Vector3& operator*=(const Matrix4& m);
};
