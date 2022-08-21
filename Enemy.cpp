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

	Vector3 approachVelocity(0, 0, -0.1);
	Vector3 leaveVelocity(0.1, 0.1, -0.1);

	switch (phase_) {
	case Phase::Approach:
	default:
		//�ړ�
		worldtransform_.translation_ += approachVelocity;
		//����̈ʒu�ɓ��B�����痣�E
		if (worldtransform_.translation_.z < -10.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//�ړ�
		worldtransform_.translation_ += leaveVelocity;
		break;
	}


	//�s��X�V
	worldtransform_.matWorld_ = affine_->World(affine_->Scale(affine_->Scale_), affine_->Rot(affine_->RotX(affine_->Rot_.x), affine_->RotY(affine_->Rot_.y), affine_->RotZ(affine_->Rot_.z)), affine_->Trans(worldtransform_.translation_));
	worldtransform_.TransferMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	//3D���f����`��
	model_->Draw(worldtransform_, viewProjection, textureHandle_);

}