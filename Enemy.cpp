#include "Enemy.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, const Vector3& position) {

	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("enemy.jpeg");

	//���[���h�ϊ�������
	worldtransform_.Initialize();
	//�����Ŏ󂯎�����������W���Z�b�g
	worldtransform_.translation_ = { position.x,position.y,position.z };

	//�A�t�B���s��
	affine_ = new Affine();

}

void Enemy::Update() {

	Vector3 velocity(0, 0, 0.1);
	//���W���ړ�������
	worldtransform_.translation_ -= velocity;

	//�s��X�V
	worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
	worldtransform_.TransferMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	//3D���f����`��
	model_->Draw(worldtransform_, viewProjection, textureHandle_);

}