#pragma once

#include "Vector3.h"
#include "MathUtility.h"

/// <summary>
/// 丸影
/// </summary>
class CircleShadow
{
public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector3 dir;
		float pad1;
		Vector3 casterPos;
		float distanceCasterLight;
		Vector3 atten;
		float pad2;
		Vector2 factorAngleCos;
		unsigned int active;
		float pad3;
	};

public: // メンバ関数
	/// <summary>
	/// 方向をセット
	/// </summary>
	/// <param name="lightdir">方向</param>
	inline void SetDir(const Vector3& dir) { 
		Vector3 result = dir;
		this->dir = MathUtility::Vector3Normalize(result);
	}

	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const Vector3& GetDir() { return dir; }

	/// <summary>
	/// キャスター座標をセット
	/// </summary>
	/// <param name="lightpos">キャスター座標</param>
	inline void SetCasterPos(const Vector3& casterPos) { this->casterPos = casterPos; }

	/// <summary>
	/// キャスター座標を取得
	/// </summary>
	/// <returns>キャスター座標</returns>
	inline const Vector3& GetCasterPos() { return casterPos; }

	/// <summary>
	/// キャスターとライトの距離をセット
	/// </summary>
	/// <param name="lightpos">キャスターとライトの距離</param>
	inline void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }

	/// <summary>
	/// キャスターとライトの距離を取得
	/// </summary>
	/// <returns>キャスターとライトの距離</returns>
	inline float GetDistanceCasterLight() { return distanceCasterLight; }

	/// <summary>
	/// 距離減衰係数をセット
	/// </summary>
	/// <param name="lightatten">ライト距離減衰係数</param>
	inline void SetAtten(const Vector3& atten) { this->atten = atten; }

	/// <summary>
	/// 距離減衰係数を取得
	/// </summary>
	/// <returns>ライト距離減衰係数</returns>
	inline const Vector3& GetAtten() { return atten; }

	/// <summary>
	/// 減衰角度をセット
	/// </summary>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度[radian]</param>
	inline void SetFactorAngle(const Vector2& factorAngle) {
		this->factorAngleCos.x = cosf(factorAngle.x);
		this->factorAngleCos.y = cosf(factorAngle.y);
	}

	/// <summary>
	/// 減衰角度を取得
	/// </summary>
	/// <returns>減衰角度</returns>
	inline const Vector2& GetFactorAngleCos() { return factorAngleCos; }

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
	// 方向（単位ベクトル）
	Vector3 dir = { 1,0,0 };
	// キャスターとライトの距離
	float distanceCasterLight = 100.0f;
	// キャスター座標（ワールド座標系）
	Vector3 casterPos = { 0,0,0 };
	// 距離減衰係数
	Vector3 atten = { 0.5f, 0.6f, 0.0f };
	// 減衰角度
	Vector2 factorAngleCos = { 0.2f, 0.5f };
	// 有効フラグ
	bool active = false;
};

