#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Affine.h"
#include "EnemyBullet.h"

#include "WinApp.h"
#include "DebugText.h"
#include "Input.h"
#include <memory>
#include <list>
#include "Matrix4.h"

class Player;

class Enemy
{
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void Fire();

	void TargetFire();

	//�ڋ߃t�F�[�Y������
	void InitializeApproach();

	void SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂�����Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�A�t�B���s��
	Affine* affine_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���L����
	Player* player_ = nullptr;

	//�s���t�F�[�Y
	enum class Phase {
		Approach,//�ڋ߂���
		Leave,   //���E����
		usually, //�ʏ탂�[�h
		wait, //�ҋ@
	};

	Phase phase_ = Phase::wait;

	int usually = 0;

	int life = 10;

private:
	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

public:
	//���ˊԊu
	static const int kFireInterval = 60;

private:
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};

