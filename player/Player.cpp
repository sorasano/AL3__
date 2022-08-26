#include "Player.h"
#include "Affine.h"
#include "PlayerBullet.h"
#include "Matrix4.h"

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle) {

	////NULLポインタチェック
	//assert(model);

	//因数としてうけとったデータを記録
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換初期化
	worldtransform_.Initialize();

	//アフィン行列
	affine_ = new Affine();

};

//更新
void Player::Update() {

	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>&bullet) {
		return bullet->IsDead();
	});

	//キャラクターの移動ベクトル
	Vector3 move = { 0.0f,0.0f,0.0f };

	//キャラクターの移動速さ
	const float speed = 0.2f;

	//移動ベクトルの変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -speed,0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { speed,0,0 };
	}

	if (input_->PushKey(DIK_UP)) {
		move = { 0,speed,0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0,-speed,0 };
	}

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	//ベクトルの加算
	worldtransform_.translation_.x += move.x;
	worldtransform_.translation_.y += move.y;
	worldtransform_.translation_.z += move.z;

	worldtransform_.translation_.x = max(worldtransform_.translation_.x, -kMoveLimitX);
	worldtransform_.translation_.x = min(worldtransform_.translation_.x, kMoveLimitX);
	worldtransform_.translation_.y = max(worldtransform_.translation_.y, -kMoveLimitY);
	worldtransform_.translation_.y = min(worldtransform_.translation_.y, kMoveLimitY);

	//行列更新
	worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
	//worldtransform_.TransferMatrix();

	//debugText_->SetPos(0, 0);
	//debugText_->Printf("PlayerPos(%f,%f,%f)", worldtransform_.translation_.x, worldtransform_.translation_.y, worldtransform_.translation_.z);

	Rotate();
	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
};

//描画
void Player::Draw(ViewProjection& viewProjection_) {

	//3Dモデルを描画
	model_->Draw(worldtransform_, viewProjection_, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

};

void Player::Rotate() {


	//キャラクターの移動ベクトル
	Vector3 rot = { 0.0f,0.0f,0.0f };

	//キャラクターの移動速さ
	const float rotY = 0.01f;

	//移動ベクトルの変更
	if (input_->PushKey(DIK_R)) {
		rot = { 0.0f,rotY,0.0f };
	}
	else if (input_->PushKey(DIK_T)) {
		rot = { 0.0f,-rotY,0.0f };
	}

	//ベクトルの加算

	worldtransform_.rotation_.x += rot.x;
	worldtransform_.rotation_.y += rot.y;
	worldtransform_.rotation_.z += rot.z;

	//行列更新
	worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(worldtransform_.rotation_.x), affine_->RotY(worldtransform_.rotation_.y), affine_->RotZ(worldtransform_.rotation_.z)), affine_->Trans(worldtransform_.translation_));
	worldtransform_.TransferMatrix();

	//debugText_->SetPos(0, 20);
	//debugText_->Printf("PlayerRot(%f,%f,%f)", worldtransform_.rotation_.x, worldtransform_.rotation_.y, worldtransform_.rotation_.z);


};

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = transform(velocity, affine_->Rot(affine_->RotX(worldtransform_.rotation_.x), affine_->RotY(worldtransform_.rotation_.y), affine_->RotZ(worldtransform_.rotation_.z)));

		//弾を生成し初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldtransform_.translation_,velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}

}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得
	worldPos.x = worldtransform_.matWorld_.m[3][0];
	worldPos.y = worldtransform_.matWorld_.m[3][1];
	worldPos.z = worldtransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {

}