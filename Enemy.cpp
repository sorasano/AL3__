#include "Enemy.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, const Vector3& position) {

	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.jpeg");

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//ワールド変換初期化
	worldtransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldtransform_.translation_ = { position.x,position.y,position.z };

	//アフィン行列
	affine_ = new Affine();

	InitializeApproach();

}

void Enemy::Update() {

	Vector3 approachVelocity(0, 0, -0.01);
	Vector3 leaveVelocity(0.1, 0.1, -0.1);

	switch (phase_) {
	case Phase::Approach:
	default:
		//移動
		worldtransform_.translation_ += approachVelocity;
		//既定の位置に到達したら離脱
		//if (worldtransform_.translation_.z < -10.0f) {
		//	phase_ = Phase::Leave;
		//}

		//発射タイマーカウントダウン
		fireTimer--;
		if (fireTimer <= 0) {
			//弾を発射
			Fire();
			//発射タイマーを初期化
			fireTimer = kFireInterval;
		}
		break;
	case Phase::Leave:
		//移動
		worldtransform_.translation_ += leaveVelocity;
		break;
	}


	//行列更新
	worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
	worldtransform_.TransferMatrix();


	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {

	//3Dモデルを描画
	model_->Draw(worldtransform_, viewProjection, textureHandle_);


	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Enemy::Fire() {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		////速度ベクトルを自機の向きに合わせて回転させる
		//velocity = transform(velocity, worldtransform_.matWorld_);

		//弾を生成し初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

}

void Enemy::InitializeApproach() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}