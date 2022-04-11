#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	textureHandle_ = TextureManager::Load("task_01_02/mario.jpg");
	soundDataHandle_ = audio_->LoadWave("task_01_02/se_sad03.wav");
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	model_ = Model::Create();
	audio_->PlayWave(soundDataHandle_);
}

void GameScene::Update() {
	XMFLOAT2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);

	worldTransform_.rotation_.x += 0.05f;
	worldTransform_.rotation_.z += 0.05f;
	worldTransform_.UpdateMatrix();

	if (input_->TriggerKey(DIK_SPACE)) {
		if (audio_->IsPlaying(voiceHandle_)) {
			audio_->StopWave(voiceHandle_);
		}
		else {
			audio_->PlayWave(voiceHandle_, true);
		}
	}

	string st = "x:" + to_string(sprite_->GetPosition().x) + " y:" + to_string(sprite_->GetPosition().y);
	debugText_->Print(st, 50, 70, 1.0f);
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
	sprite_->Draw();

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
