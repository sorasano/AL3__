#include "Enemy.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.jpeg");

	//ワールド変換初期化
	worldtransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldtransform_.translation_ = { position.x,position.y,position.z };

	//アフィン行列
	affine_ = new Affine();

}

void Enemy::Update() {

	Vector3 velocity(0, 0, 0.1);
	//座標を移動させる
	worldtransform_.translation_ -= velocity;

	//行列更新
	worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
	worldtransform_.TransferMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldtransform_, viewProjection, textureHandle_);

}