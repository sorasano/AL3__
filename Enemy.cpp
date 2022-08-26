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
	debugText_ = DebugText::GetInstance();

	//ワールド変換初期化
	worldtransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldtransform_.translation_ = { position.x,position.y,position.z };

	//アフィン行列
	affine_ = new Affine();

	InitializeApproach();

}

void Enemy::Update() {

	if (life <= 0) {
		worldtransform_.translation_ = Vector3(100,100,100);
		//行列更新
		worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
		worldtransform_.TransferMatrix();

		debugText_->SetPos(400,100);
		debugText_->Printf("clear");
	}
	else {

		//デスフラグの立った球を削除
		bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
			return bullet->IsDead();
			});

		Vector3 approachVelocity(0, 0, -0.1);
		Vector3 leaveVelocity(0.1, 0.1, -0.1);
		Vector3 usuallyVelocity(0.1, 0, 0);

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
				TargetFire();
				//発射タイマーを初期化
				fireTimer = kFireInterval;
			}
			break;
		case Phase::Leave:
			//移動
			worldtransform_.translation_ += leaveVelocity;
			break;
		case Phase::usually:

			if (worldtransform_.translation_.x <= 20 && usually == 0) {
				worldtransform_.translation_ += usuallyVelocity;
			}
			else {
				usually = 1;
			}

			if (worldtransform_.translation_.x >= -20 && usually == 1) {
				worldtransform_.translation_ -= usuallyVelocity;
			}
			else {
				usually = 0;
			}

			//発射タイマーカウントダウン
			fireTimer--;
			if (fireTimer <= 0) {
				//弾を発射
				Fire();
				//発射タイマーを初期化
				fireTimer = kFireInterval;
			}
			break;
		case Phase::wait:
			worldtransform_.translation_ = Vector3(0, 0, 20);
			break;
		}

		if (input_->TriggerKey(DIK_1)) {
			phase_ = Phase::Approach;
		}
		else if (input_->TriggerKey(DIK_2)) {
			phase_ = Phase::usually;
		}
		else if (input_->TriggerKey(DIK_3)) {
			phase_ = Phase::wait;
		}

		//行列更新
		worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
		worldtransform_.TransferMatrix();


		//弾更新
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Update();
		}
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

	assert(player_);

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		if (phase_ == Phase::Approach) {

			//自キャラのワールド座標を取得
			Vector3 playerWorldPos = player_->GetWorldPosition();
			//敵キャラのワールド座標を取得
			Vector3 enemyWorldPos = GetWorldPosition();
			//敵キャラ→自キャラの差分ベクトルを求める
			Vector3 velocity = velocity.sub(enemyWorldPos, playerWorldPos);
			//ベクトルの正規化
			velocity.normalize();
			////ベクトルの長さを速さに合わせる
			//velocity *= kBulletSpeed;
		}

		//弾を生成し初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

}

void Enemy::TargetFire() {

	assert(player_);

	//自キャラのワールド座標を取得
	Vector3 playerWorldPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	Vector3 enemyWorldPos = GetWorldPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	Vector3 velocity = velocity.sub(enemyWorldPos, playerWorldPos);
	//ベクトルの正規化
	velocity.normalize();

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

Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {
	life--;
}