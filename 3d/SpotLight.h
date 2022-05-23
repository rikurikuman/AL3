#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "MathUtility.h"

/// <summary>
/// スポットライト
/// </summary>
class SpotLight
{
public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector3 lightv;
		float pad1;
		Vector3 lightpos;
		float pad2;
		Vector3 lightcolor;
		float pad3;
		Vector3 lightatten;
		float pad4;
		Vector2 lightfactoranglecos;
		unsigned int active;
		float pad5;
	};

public: // メンバ関数
	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="lightdir">ライト方向</param>
	inline void SetLightDir(const Vector3& lightdir) { 
		Vector3 result = lightdir;
		this->lightdir = MathUtility::Vector3Normalize(result);
	}

	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const Vector3& GetLightDir() { return lightdir; }

	/// <summary>
	/// ライト座標をセット
	/// </summary>
	/// <param name="lightpos">ライト座標</param>
	inline void SetLightPos(const Vector3& lightpos) { this->lightpos = lightpos; }

	/// <summary>
	/// ライト座標を取得
	/// </summary>
	/// <returns>ライト座標</returns>
	inline const Vector3& GetLightPos() { return lightpos; }

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightcolor">ライト色</param>
	inline void SetLightColor(const Vector3& lightcolor) { this->lightcolor = lightcolor; }

	/// <summary>
	/// ライト色を取得
	/// </summary>
	/// <returns>ライト色</returns>
	inline const Vector3& GetLightColor() { return lightcolor; }

	/// <summary>
	/// ライト距離減衰係数をセット
	/// </summary>
	/// <param name="lightatten">ライト距離減衰係数</param>
	inline void SetLightAtten(const Vector3& lightAtten) { this->lightAtten = lightAtten; }

	/// <summary>
	/// ライト距離減衰係数を取得
	/// </summary>
	/// <returns>ライト距離減衰係数</returns>
	inline const Vector3& GetLightAtten() { return lightAtten; }

	/// <summary>
	/// ライト減衰角度をセット
	/// </summary>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度[radian]</param>
	inline void SetLightFactorAngle(const Vector2& lightFactorAngle) { 
		this->lightFactorAngleCos.x = cosf(lightFactorAngle.x);
		this->lightFactorAngleCos.y = cosf(lightFactorAngle.y);
	}

	/// <summary>
	/// ライト減衰角度を取得
	/// </summary>
	/// <returns>ライト距離減衰係数</returns>
	inline const Vector2& GetLightFactorAngleCos() { return lightFactorAngleCos; }

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(bool active) { this->active = active; }

	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return active; }

private: // メンバ変数
	// ライト方向（単位ベクトル）
	Vector3 lightdir = { 1,0,0 };
	// ライト座標（ワールド座標系）
	Vector3 lightpos = { 0,0,0 };
	// ライト色
	Vector3 lightcolor = { 1,1,1 };
	// ライト距離減衰係数
	Vector3 lightAtten = { 1.0f, 1.0f, 1.0f };
	// ライト減衰角度
	Vector2 lightFactorAngleCos = { 0.2f, 0.5f };
	// 有効フラグ
	bool active = false;
};

