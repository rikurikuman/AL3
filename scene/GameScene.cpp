#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	random_device device;
	uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	uniform_real_distribution<float> posDist(-20, 20);

	for (int i = 0; i < 100; i++) {
		transforms[i].translation_ = { posDist(device), posDist(device), posDist(device) };
		transforms[i].rotation_ = { rotDist(device), rotDist(device), rotDist(device) };
		transforms[i].Initialize();
	}
	viewProjection_.Initialize();
	textureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
}

void GameScene::Update() {
	random_device device;
	uniform_real_distribution<float> rotDist(0.0f, XMConvertToRadians(5));

	for (int i = 0; i < 100; i++) {
		transforms[i].rotation_.x += rotDist(device);
		transforms[i].rotation_.y += rotDist(device);
		transforms[i].rotation_.z += rotDist(device);
		transforms[i].UpdateMatrix();
	}

	XMFLOAT3 moveVecA = {0, 0, 0};
	XMFLOAT3 moveVecB = { 0, 0, 0 };
	const float moveSpeed = 0.2f;
	const float viewRotSpeed = 0.05f;

	if (input_->PushKey(DIK_W)) {
		moveVecA = { 0, 0, moveSpeed };
	}
	else if (input_->PushKey(DIK_S)) {
		moveVecA = { 0, 0, -moveSpeed };
	}

	if (input_->PushKey(DIK_LEFTARROW)) {
		moveVecB = { -moveSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHTARROW)) {
		moveVecB = { moveSpeed, 0, 0 };
	}

	if (input_->PushKey(DIK_SPACE)) {
		viewAngle += viewRotSpeed;
	}

	viewProjection_.eye.x += moveVecA.x;
	viewProjection_.eye.y += moveVecA.y;
	viewProjection_.eye.z += moveVecA.z;

	viewProjection_.target.x += moveVecB.x;
	viewProjection_.target.y += moveVecB.y;
	viewProjection_.target.z += moveVecB.z;

	viewProjection_.up = { cosf(viewAngle), sinf(viewAngle), 0 };

	viewProjection_.UpdateMatrix();

	debugText_->Print("eye:(" + to_string(viewProjection_.eye.x) + ", " + to_string(viewProjection_.eye.y) + ", " + to_string(viewProjection_.eye.z) + ")", 50, 70, 1.0);
	debugText_->Print("target:(" + to_string(viewProjection_.target.x) + ", " + to_string(viewProjection_.target.y) + ", " + to_string(viewProjection_.target.z) + ")", 50, 90, 1.0);
	debugText_->Print("eye:(" + to_string(viewProjection_.up.x) + ", " + to_string(viewProjection_.up.y) + ", " + to_string(viewProjection_.up.z) + ")", 50, 110, 1.0);

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < 100; i++) {
		model_->Draw(transforms[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
