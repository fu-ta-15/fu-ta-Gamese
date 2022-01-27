//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "player.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "camera.h"
#include "floor.h"
#include "model.h"
#include "motion.h"
#include "camera.h"
#include "game.h"
#include "score.h"
#include "eraser.h"
#include "graffiti.h"
#include "stage.h"
#include "shadowS.h"
#include "pause.h"
#include "countdown.h"
#include "eraserdust.h"
#include "player_marker.h"
#include "rank.h"

#include "keep_object.h"
#include "load_player_data.h"
#include "texture.h"
#include "result.h"

#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_PLAYER		(1.8f)		// �v���C���[�̈ړ���
#define JUMP_PLAYER		(15.0f)		// �v���C���[�̃W�����v��
#define GRAVITY_PLAYER	(1.0f)		// �v���C���[�̏d��
#define PI_QUARTER		(D3DX_PI / 4.0f)

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// CPlayer�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority) : CScene(nPriority)
{
	// �ϐ��̏�����
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumber = 0;
	m_nNumModel = 0;
	m_pMotion = NULL;
	m_motionType = MOTIONTYPE_MOVE;
}

//=============================================================================
// CPlayer�̃f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ��������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int nNumber)
{
	// �|�C���^�ϐ��̐���
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	// NULL�`�F�b�N
	if (pPlayer != NULL)
	{
		// ���Ԗڂ��ۑ�
		pPlayer->SetNumber(nNumber);

		// ����������
		pPlayer->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pPlayer->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = D3DXVECTOR3(size.x, size.y, size.x);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_motionType = MOTIONTYPE_NEUTORAL;
	m_bFinishMotion = true;

	// ���[�V�������̐���
	m_pMotion = CMotion::Create();

	// �v���C���[�f�[�^���̎擾
	CLoadPlayerData::Player_Data player_Data = CManager::GetKeepObject()->GetLoadPlayerData()->GetPlayerData(m_nNumber);
	m_nNumModel = player_Data.nNumModel;

	// ���f���̓ǂݍ���
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// �f�[�^�̕ۑ�
		m_apModel[nCntModel] = CModel::Create(D3DXVECTOR3(player_Data.aOffset[nCntModel][0], player_Data.aOffset[nCntModel][1], player_Data.aOffset[nCntModel][2]),
			D3DXVECTOR3(player_Data.aOffset[nCntModel][3], player_Data.aOffset[nCntModel][4], player_Data.aOffset[nCntModel][5]),
			&player_Data.apModelFileName[nCntModel][0]);

		// �e���f���̐ݒ�
		if (player_Data.nIndexParent[nCntModel] >= 0)
		{
			m_apModel[nCntModel]->SetParent(m_apModel[player_Data.nIndexParent[nCntModel]]);
		}
		else
		{
			m_apModel[nCntModel]->SetParent(NULL);
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �������̊J��
	if (m_pMotion != NULL)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// ����Ă��邩�ǂ���
	bool bLandModel = false;

	// ���W�̕ۑ�
	m_posOld = m_pos;

	// ���݂̉�]�p�𐧌�
	if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		m_rotDest.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		m_rotDest.y -= D3DX_PI * 2.0f;
	}
	// �����̍X�V
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.2f;

	// ���݂̉�]�p�𐧌�
	if (m_rot.y < -D3DX_PI)
	{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// ���[�V�����Ǘ�
	Motion();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	CRenderer *pRender = CManager::GetRenderer();
	pDevice = pRender->GetDevice();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		// �p�[�c�̕`��
		m_apModel[nCnt]->Draw();
	}
}

//=============================================================================
// ���[�V�����̊Ǘ�
//=============================================================================
void CPlayer::Motion(void)
{
	// �ϐ��錾
	CMotion::MOTION_INFO *pMotionInfo = CMotion::GetMotionInfo(0);		// ���[�V�������̎擾
	CMotion::MOTION motion = m_pMotion->GetMotion();					// ���݂̃��[�V�������̎擾
	CMotion::KEY *pKey;													// �L�[���
	CMotion::KEY *pKeyNext;												// ���̃L�[���
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (motion.nKey == pMotionInfo[m_motionType].nNumKey - 1 && pMotionInfo[m_motionType].bLoop == false)
		{
			// �I��������Ƃ�`����
			if (m_bFinishMotion == false)
			{
				m_bFinishMotion = true;
			}
			break;
		}

		// �L�[�̊m��
		pKey = &pMotionInfo[m_motionType].aKeyInfo[motion.nKey].aKey[nCntModel];
		pKeyNext = &pMotionInfo[m_motionType].aKeyInfo[(motion.nKey + 1) % pMotionInfo[m_motionType].nNumKey].aKey[nCntModel];

		// �����̊m��
		float fDiffMoitonPosX = pKeyNext->fPosX - pKey->fPosX;
		float fDiffMoitonPosY = pKeyNext->fPosY - pKey->fPosY;
		float fDiffMoitonPosZ = pKeyNext->fPosZ - pKey->fPosZ;
		float fDiffMoitonRotX = pKeyNext->fRotX - pKey->fRotX;
		float fDiffMoitonRotY = pKeyNext->fRotY - pKey->fRotY;
		float fDiffMoitonRotZ = pKeyNext->fRotZ - pKey->fRotZ;

		// ���Βl�̊m��
		float fRateMotion = (float)motion.nCounterMotion / (float)pMotionInfo[m_motionType].aKeyInfo[motion.nKey].nFrame;

		// ���[�V�����̈ʒu�ƌ����̊m��
		fPosX = pKey->fPosX + (fDiffMoitonPosX * fRateMotion);
		fPosY = pKey->fPosY + (fDiffMoitonPosY * fRateMotion);
		fPosZ = pKey->fPosZ + (fDiffMoitonPosZ * fRateMotion);
		fRotX = pKey->fRotX + (fDiffMoitonRotX * fRateMotion);
		fRotY = pKey->fRotY + (fDiffMoitonRotY * fRateMotion);
		fRotZ = pKey->fRotZ + (fDiffMoitonRotZ * fRateMotion);

		// ���݂̉�]�p�𐧌�
		if (fRotX > D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			fRotX -= D3DX_PI * 2.0f;
		}
		else if (fRotX < -D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			fRotX += D3DX_PI * 2.0f;
		}
		// ���݂̉�]�p�𐧌�
		if (fRotY > D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			fRotY -= D3DX_PI * 2.0f;
		}
		else if (fRotY < -D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			fRotY += D3DX_PI * 2.0f;
		}
		// ���݂̉�]�p�𐧌�
		if (fRotZ > D3DX_PI)
		{// 3.14f���傫���Ȃ����Ƃ��l��-3.14f�ɂ���
			fRotZ -= D3DX_PI * 2.0f;
		}
		else if (fRotZ < -D3DX_PI)
		{// -3.14f��菬�����Ȃ����Ƃ��l��3.14f�ɂ���
			fRotZ += D3DX_PI * 2.0f;
		}

		// �f�[�^�ۑ��p�N���X�̎擾
		CKeepObject *pKeepObject = CManager::GetKeepObject();

		if (pKeepObject != NULL)
		{
			// �v���C���[�f�[�^���̎擾
			CLoadPlayerData::Player_Data player_Data = pKeepObject->GetLoadPlayerData()->GetPlayerData(m_nNumber);

			// �ʒu�ƌ����̐ݒ�
			m_apModel[nCntModel]->SetPosition(D3DXVECTOR3(fPosX + player_Data.aOffset[nCntModel][0], fPosY + player_Data.aOffset[nCntModel][1], fPosZ + player_Data.aOffset[nCntModel][2]));
			m_apModel[nCntModel]->SetRot(D3DXVECTOR3(fRotX, fRotY, fRotZ));
		}
	}

	// ���[�V�����J�E���^���e�L�[�̍ŏI�t���[���ɓ��B�����Ƃ�
	if (motion.nCounterMotion >= pMotionInfo[m_motionType].aKeyInfo[motion.nKey].nFrame)
	{
		if (pMotionInfo[m_motionType].bLoop == false && motion.nKey == pMotionInfo[m_motionType].nNumKey - 1)
		{
			// ���[�V�����J�E���^�̍X�V
			motion.nCounterMotion = pMotionInfo[m_motionType].aKeyInfo[motion.nKey].nFrame;
		}
		else
		{
			// ���[�V�����J�E���^�̏�����
			motion.nCounterMotion = 0;

			// �L�[�̍X�V
			motion.nKey++;

			// �L�[�̊m��
			motion.nKey = motion.nKey % pMotionInfo[m_motionType].nNumKey;
		}
	}
	else
	{
		// ���[�V�����J�E���^�̍X�V
		motion.nCounterMotion++;
	}

	// ���[�V�������̐ݒ�
	m_pMotion->SetMotion(motion);
}