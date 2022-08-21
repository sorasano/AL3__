#pragma once

#include "WorldTransform.h"
#include "WinApp.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include "Affine.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include "Matrix4.h"

class Player {

public:
	//������
	void Initialize(Model* model, uint32_t textureHandle);

	//�X�V
	void Update();

	//�`��
	void Draw(ViewProjection& viewProjection_);

	//����
	void Rotate();

	//�U��
	void Attack();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldtransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�A�t�B���s��
	Affine* affine_;

private:
	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
};