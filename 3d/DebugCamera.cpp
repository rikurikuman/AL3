#include "DebugCamera.h"
#include "DirectXCommon.h"
#include <cassert>

using namespace DirectX;

DebugCamera::DebugCamera(int window_width, int window_height) {

	input_ = Input::GetInstance();
	// 画面サイズに対する相対的なスケールに調整
	scaleX_ = 1.0f / (float)window_width;
	scaleY_ = 1.0f / (float)window_height;

	MultiplyMatrix(XMMatrixIdentity());

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void DebugCamera::Update() {
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;

	// マウスの入力を取得
	Input::MouseMove mouseMove = input_->GetMouseMove();

	// マウスの左ボタンが押されていたらカメラを回転させる
	if (input_->IsPressMouse(0)) {
		float dy = mouseMove.lX * scaleY_;
		float dx = mouseMove.lY * scaleX_;

		angleX = -dx * XM_PI;
		angleY = -dy * XM_PI;
		dirty = true;
	}

	// マウスの中ボタンが押されていたらカメラを並行移動させる
	if (input_->IsPressMouse(2)) {
		float dx = mouseMove.lX / 100.0f;
		float dy = mouseMove.lY / 100.0f;

		XMVECTOR move = {-dx, +dy, 0, 0};
		move = XMVector3Transform(move, matRot);

		viewProjection_.eye.x += move.m128_f32[0];
		viewProjection_.eye.y += move.m128_f32[1];
		viewProjection_.eye.z += move.m128_f32[2];

		viewProjection_.target.x += move.m128_f32[0];
		viewProjection_.target.y += move.m128_f32[1];
		viewProjection_.target.z += move.m128_f32[2];

		dirty = true;
	}

	// ホイール入力で距離を変更
	if (mouseMove.lZ != 0) {
		distance_ -= mouseMove.lZ / 100.0f;
		distance_ = max(distance_, 1.0f);
		dirty = true;
	}

	if (dirty) {
		// 追加回転分の回転行列を生成
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationX(-angleX);
		matRotNew *= XMMatrixRotationY(-angleY);

		MultiplyMatrix(matRotNew);
	}

	viewProjection_.UpdateMatrix();
}

void DebugCamera::MultiplyMatrix(const DirectX::XMMATRIX& matrix) {
	// 累積の回転行列を合成
	// ※回転行列を累積していくと、浮動小数点数の誤差でスケーリングがかかる危険がある為
	// クォータニオンを使用する方が望ましい
	matRot = matrix * matRot;

	// 注視点から視点へのベクトルと、上方向ベクトル
	XMVECTOR vTargetEye = {0.0f, 0.0f, -distance_, 1.0f};
	XMVECTOR vUp = {0.0f, 1.0f, 0.0f, 0.0f};

	// ベクトルを回転
	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	vUp = XMVector3Transform(vUp, matRot);

	// 注視点からずらした位置に視点座標を決定
	XMFLOAT3& eye = viewProjection_.eye;
	XMFLOAT3& up = viewProjection_.up;
	const XMFLOAT3& target = viewProjection_.target;
	eye.x = target.x + vTargetEye.m128_f32[0];
	eye.y = target.y + vTargetEye.m128_f32[1];
	eye.z = target.z + vTargetEye.m128_f32[2];

	up.x = vUp.m128_f32[0];
	up.y = vUp.m128_f32[1];
	up.z = vUp.m128_f32[2];
}
