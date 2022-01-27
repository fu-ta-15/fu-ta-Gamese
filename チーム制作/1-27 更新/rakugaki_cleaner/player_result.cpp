//=============================================================================
//
// ���U���g�V�[���̃v���C���[���� [player_result.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "player_result.h"
#include "manager.h"
#include "renderer.h"
#include "motion.h"
#include "camera.h"
#include "stage.h"
#include "player_marker.h"
#include "rank.h"
#include "shadow.h"

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
// CPlayer_Result�̃R���X�g���N�^
//=============================================================================
CPlayer_Result::CPlayer_Result(int nPriority) : CPlayer(nPriority)
{

}

//=============================================================================
// CPlayer_Result�̃f�X�g���N�^
//=============================================================================
CPlayer_Result::~CPlayer_Result()
{

}

//=============================================================================
// ��������
//=============================================================================
CPlayer_Result *CPlayer_Result::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber)
{
	// �|�C���^�ϐ��̐���
	CPlayer_Result *pPlayer_Result;
	pPlayer_Result = new CPlayer_Result(CScene::PRIORITY_PLAYER);

	// NULL�`�F�b�N
	if (pPlayer_Result != NULL)
	{
		// ���Ԗڂ��ۑ�
		pPlayer_Result->SetNumber(nNumber);

		// ����������
		pPlayer_Result->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pPlayer_Result->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer_Result;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer_Result::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �v���C���[�̏�����
	CPlayer::Init(pos,size);

	// �����o�ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pShadow = CShadow::Create(pos, D3DXVECTOR2(25.0f, 25.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer_Result::Uninit(void)
{
	// �v���C���[�̏I������
	CPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer_Result::Update(void)
{
	// �v���C���[�̍X�V����
	CPlayer::Update();

	// �d��
	m_move.y -= GRAVITY_PLAYER;

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	// �ʒu�̍X�V
	pos += m_move;

	// �ʒu�̐ݒ�
	SetPos(pos);

	// �X�e�[�W�Ƃ̓����蔻��
	CollisionStage();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer_Result::Draw(void)
{
	// �v���C���[�̕`�揈��
	CPlayer::Draw();
}

//=============================================================================
// �X�e�[�W�Ƃ̓����蔻��
//=============================================================================
bool CPlayer_Result::CollisionStage(void)
{
	// �����̎擾
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_MODEL);

	// NULL�`�F�b�N
	if (pScene != NULL)
	{
		while (pScene)
		{
			// ���̏���ۑ�
			CScene *pSceneNext = pScene->GetNext();

			// �I�u�W�F�N�g�̎�ނ����������珰�ɃL���X�g
			if (pScene->GetObjType() == CScene::OBJTYPE_STAGE)
			{
				CStage *pStage = (CStage*)pScene;

				// �ʒu�̎擾
				D3DXVECTOR3 pos = GetPos();

				// �����蔻��
				if (pStage->Collision(&pos, GetSize()))
				{
					m_move.y = 0.0f;

					// �ʒu�̐ݒ�
					SetPos(pos);

					return true;
				}
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
	return false;
}

//=============================================================================
// ���[�V�����̐ݒ�
//=============================================================================
bool CPlayer_Result::SetMotion(MotionType motionType)
{
	// ���[�V�����̎�ނɂ���ď�����ς���
	switch (motionType)
	{
		// �j���[�g����
	case MOTIONTYPE_NEUTORAL:
		break;

		// �ړ�
	case MOTIONTYPE_MOVE:
		break;

		// �A�N�V����
	case MOTIONTYPE_DELETE:
		break;
	}

	if (GetMotionType() != motionType)
	{
		// ���[�V�����̎�ސݒ�
		SetMotionType(motionType);

		// ���݂̃��[�V�������̎擾
		CMotion::MOTION motion = m_pMotion->GetMotion();

		// ���[�V�����J�E���^��������
		motion.nCounterMotion = 0;

		// ���݂̃L�[��������
		motion.nKey = 0;

		// ���݂̃��[�V��������ݒ�
		m_pMotion->SetMotion(motion);

		return true;
	}
	return false;
}

//=============================================================================
// �v���C���[�}�[�J�[���̐ݒ�
//=============================================================================
void CPlayer_Result::SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera)
{
	m_pPlayerMarker = pPlayerMarker;

	m_pPlayerMarker->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPlayerMarker->SetTex(D3DXVECTOR2((float)GetNumber(), 1));

	D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
	m_pPlayerMarker->SetPosition(D3DXVECTOR3(pos.x, pos.y + 10.0f, pos.z));
}

//=============================================================================
// �����N���̐ݒ�
//=============================================================================
void CPlayer_Result::SetRank(CRank *pRank, int nRank, CCamera *pCamera)
{
	m_pRank = pRank;

	m_pRank->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pRank->SetTex(D3DXVECTOR2((float)nRank, 1));

	// �}�[�J�[�̈ʒu��ύX
	D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
	m_pRank->SetPosition(D3DXVECTOR3(pos.x, pos.y - 400.0f, pos.z));
}