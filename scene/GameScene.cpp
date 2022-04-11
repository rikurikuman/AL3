#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 10, 10, 10 };
	worldTransform_.rotation_ = { 0.785898, 0.785898, 0 };
	worldTransform_.scale_ = { 5, 5, 5 };
	worldTransform_.UpdateMatrix();
	viewProjection_.Initialize();
	textureHandle_ = TextureManager::Load("mario.jpg");
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
}

void GameScene::Update() {
	string st = "translation:(" + to_string(worldTransform_.translation_.x) + ", " + to_string(worldTransform_.translation_.y) + ", " + to_string(worldTransform_.translation_.z) + ")";
	string st2 = "rotation:(" + to_string(worldTransform_.rotation_.x) + ", " + to_string(worldTransform_.rotation_.y) + ", " + to_string(worldTransform_.rotation_.z) + ")";
	string st3 = "scale:(" + to_string(worldTransform_.scale_.x) + ", " + to_string(worldTransform_.scale_.y) + ", " + to_string(worldTransform_.scale_.z) + ")";
	debugText_->Print(st, 50, 70, 1.0f);
	debugText_->Print(st2, 50, 90, 1.0f);
	debugText_->Print(st3, 50, 110, 1.0f);
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
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

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
