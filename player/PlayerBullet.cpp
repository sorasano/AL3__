#include "PlayerBullet.h"
#include "GameScene.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");

	//ワールド変換初期化
	worldtransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldtransform_.translation_ = {position.x,position.y,position.z};

	//アフィン行列
	affine_ = new Affine();

}

void PlayerBullet::Update() {
	//行列更新
	worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
	worldtransform_.TransferMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldtransform_, viewProjection, textureHandle_);

}