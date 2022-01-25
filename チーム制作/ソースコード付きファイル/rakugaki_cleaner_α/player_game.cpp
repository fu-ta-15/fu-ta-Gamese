//=============================================================================
//
// �Q�[���V�[���̃v���C���[���� [player_game.cpp]
// Author : 
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "player_game.h"
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_joypad.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "camera.h"
#include "game.h"
#include "score.h"
#include "eraser.h"
#include "graffiti.h"
#include "stage.h"
#include "pause.h"
#include "countdown.h"
#include "eraserdust.h"
#include "player_marker.h"
#include "skill.h"
#include "shadow.h"

#include "keep_object.h"
#include "load_player_data.h"
#include "texture.h"
#include "result.h"

#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MOVE_PLAYER						(1.8f)				// �v���C���[�̈ړ���
#define JUMP_PLAYER						(15.0f)				// �v���C���[�̃W�����v��
#define GRAVITY_PLAYER					(1.0f)				// �v���C���[�̏d��
#define PI_QUARTER						(D3DX_PI / 4.0f)	// �ړ��p
#define INERTIA_NORMAL					(0.4f)				// �ʏ튵��
#define INERTIA_SKY						(0.1f)				// �󒆊���

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// CPlayer_Game�̃R���X�g���N�^
//=============================================================================
CPlayer_Game::CPlayer_Game(int nPriority) : CPlayer(nPriority)
{
	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bCleaner = false;
	m_nCntDeleteGraffiti = 0;
}

//=============================================================================
// CPlayer_Game�̃f�X�g���N�^
//=============================================================================
CPlayer_Game::~CPlayer_Game()
{

}

//=============================================================================
// ��������
//=============================================================================
CPlayer_Game *CPlayer_Game::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber)
{
	// �|�C���^�ϐ��̐���
	CPlayer_Game *pPlayer_Game;
	pPlayer_Game = new CPlayer_Game(CScene::PRIORITY_PLAYER);

	// NULL�`�F�b�N
	if (pPlayer_Game != NULL)
	{
		// ���Ԗڂ��ۑ�
		pPlayer_Game->SetNumber(nNumber);

		// ����������
		pPlayer_Game->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pPlayer_Game->SetObjType(OBJTYPE_PLAYER);
	}

	return pPlayer_Game;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer_Game::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �v���C���[�̏���������
	CPlayer::Init(pos, size);

	// �ϐ��̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bCleaner = false;
	m_nCntDeleteGraffiti = 0;
	m_pPlayerMarker = NULL;
	m_pEraser = NULL;
	m_bSkill = false;
	m_bColEraser = false;
	m_bColSkill = false;
	m_nCntSkillGauge = 0;
	m_bMaxSkillGauge = false;
	m_nRank = 0;

	m_pShadow = CShadow::Create(pos,D3DXVECTOR2(25.0f, 25.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer_Game::Uninit(void)
{
	// ���������N�K�L���̕ۑ�
	CManager::GetKeepObject()->SetDeleteGraffiti(m_nCntDeleteGraffiti, GetNumber());

	// �폜
	CPlayer::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer_Game::Update(void)
{
	// �v���C���[�̍X�V����
	CPlayer::Update();

	// �J�E���g�_�E�������擾
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		CGame *pGame = (CGame*)pBase;
		CCountdown *pCountdown = pGame->GetCountdwn();

		if (pCountdown != NULL && pCountdown->GetFinish() && pGame->GetGameMode() == CGame::GAMEMODE_NORMAL)
		{
			if (m_bSkill == false)
			{
				// �ړ�����
				int nNumber = GetNumber();

				if (m_bColSkill == false)
				{
					/*if (nNumber == CGame::GetnCntPlayer())
					{
						MoveKeyboard();
					}
					else*/
					{
						Move();
					}

					if (CManager::GetKeepObject()->GetGameMode() == CKeepObject::GAMEMODE_BATTLE)
					{
						// �X�L���Ƃ̓����蔻��
						CollisionSkill();
					}
				}
			}
			else
			{
				// �����S���ƃX�e�[�W�̓����蔻��
				m_bColEraser = m_pEraser->CollisionStage(m_nRank - 1);

				if (GetFinishMotion() == true)
				{
					m_bSkill = false;
					m_bColEraser = false;
					m_pEraser->SetColStage(false);
				}
			}

			// �|�[�Y
			Pause();
		}
	}

	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// ����
	if (pos.y <= 0.0f)
	{
		// �n��
		m_move.x += (0.0f - m_move.x) * INERTIA_NORMAL;
		m_move.z += (0.0f - m_move.z) * INERTIA_NORMAL;
	}
	else
	{
		// ��
		m_move.x += (0.0f - m_move.x) * INERTIA_SKY;
		m_move.z += (0.0f - m_move.z) * INERTIA_SKY;
	}

	// �d��
	m_move.y -= GRAVITY_PLAYER;

	// �ړ��ʂ��ʒu�ɉ��Z
	pos += m_move;

	// �ʒu�̐ݒ�
	SetPos(pos);

	// �X�e�[�W�Ƃ̓����蔻��
	CollisionStage();

	// �����J�X�Ƃ̓����蔻��
	CollisionEraserDust();

	// �v���C���[���m�̓����蔻��
	CollisionPlayer();

	if (pBase != NULL && m_pPlayerMarker != NULL)
	{
		CGame *pGame = (CGame*)pBase;
		CCamera *pCamera = pGame->GetCamera();

		// �}�[�J�[�̈ʒu��ύX
		D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
		m_pPlayerMarker->SetPosition(D3DXVECTOR3(pos.x, pos.y - 70.0f, pos.z));
	}

	// �e�̈ʒu�X�V
	pos = GetPos();
	m_pShadow->SetPosition(D3DXVECTOR3(pos.x, 1.0f, pos.z));
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer_Game::Draw(void)
{
	// �v���C���[�̕`�揈��
	CPlayer::Draw();
}

//=============================================================================
// �|�[�Y�ݒ菈��
//=============================================================================
void CPlayer_Game::Pause(void)
{
	// �|�[�Y���̎擾
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_PAUSE);

	// NULL�`�F�b�N
	if (pScene != NULL)
	{
		while (pScene)
		{
			// ���̏���ۑ�
			CScene *pSceneNext = pScene->GetNext();

			// �I�u�W�F�N�g�̎�ނ����������珰�ɃL���X�g
			if (pScene->GetObjType() == CScene::OBJTYPE_PAUSE)
			{
				CPause *pPause = (CPause*)pScene;

				// �L�[�{�[�h���̎擾
				CInputKeyboard *pKey = GET_KEYBOARD;

				// CInputKeyboard�C���X�^���X�̎擾
				CInputJoypad *pJoypad = GET_JOYPAD;

				// ���Ԗڂ��擾
				int nNumber = GetNumber();

				if ((pKey->GetTrigger(KEY_PAUSE) && nNumber == CGame::GetnCntPlayer()) || pJoypad->GetButtonTrigger(nNumber, JOYPAD_PAUSE))
				{
					pPause->SetPause(nNumber);
				}
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer_Game::Move(void)
{
	// �J�����̊p�x���̎擾
	CGame *pGame = NULL;
	CCamera *pCamera = NULL;
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		pGame = (CGame*)pBase;
		pCamera = pGame->GetCamera();
	}
	D3DXVECTOR3 rotCamera = pCamera->GetRotate();

	// ���Ԗڂ��擾
	int nNumber = GetNumber();

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	// CInputKeyboard�C���X�^���X�̎擾
	CInputJoypad *pJoypad = GET_JOYPAD;

	// �Q�[���p�b�h���̎擾
	DIJOYSTATE2 Controller = pJoypad->GetControllerPless(nNumber);

	if (Controller.lX != 0 || Controller.lY != 0)
	{
		// �ϐ��錾
		D3DXVECTOR3 rotDest = {};

		// �X�e�B�b�N�̓��͊p�x
		float fAngle = atan2f((float)Controller.lX, (float)Controller.lY);

		rotDest.y = rotCamera.y - fAngle;

		m_move.x -= MOVE_PLAYER * sinf(rotDest.y);			// X���ړ��ʂ̌���
		m_move.z -= MOVE_PLAYER * cosf(rotDest.y);			// Z���ړ��ʂ̌���

		// ���[�V�������ړ����[�V�����ɂ���
		SetMotion(CPlayer::MOTIONTYPE_MOVE);

		// �p�x�̐ݒ�
		SetRotDest(rotDest);
	}
	else
	{
		// �j���[�g�������[�V�����ɂ���
		SetMotion(CPlayer::MOTIONTYPE_NEUTORAL);
	}

	// ���N�K�L��������
	if (pJoypad->GetButtonPress(nNumber, JOYPAD_GRAFFITI_DELETE))
	{
		// ���N�K�L�Ƃ̓����蔻��
		CollisionGraffiti();

		// �ړ��ʂ𔼕��ɂ���
		m_move *= 0.5f;

		// �����Ă��Ԃɂ���
		m_bCleaner = true;

		// ���[�V�������A�N�V�������[�V�����ɂ���
		SetMotion(CPlayer::MOTIONTYPE_DELETE);

		// �X�L���Q�[�W�p�J�E���g
		m_nCntSkillGauge++;

		if (m_nCntSkillGauge >= DELETE_GARAFFITI_NUMBER_MAX)
		{
			m_nCntSkillGauge = DELETE_GARAFFITI_NUMBER_MAX;
		}
	}
	else
	{
		m_bCleaner = false;
	}

	// �����S���؂�ւ�����
	if (pJoypad->GetButtonTrigger(nNumber, JOYPAD_ERASERTYPE_SWITCH))
	{
		m_pEraser->SwicthType();
	}

	// �X�L������
	if (m_bMaxSkillGauge == true)
	{
		if (pJoypad->GetButtonTrigger(nNumber, JOYPAD_SKILL))
		{
			// �X�L�����g�p�������Ƃ�`����
			m_bSkill = true;

			// ���[�V�������X�L���ɂ���
			SetMotion(CPlayer::MOTIONTYPE_SKILL);

			// �l��������
			m_nCntSkillGauge = 0;
			m_bMaxSkillGauge = false;
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer_Game::MoveKeyboard(void)
{
	// �J�����̊p�x���̎擾
	CGame *pGame = NULL;
	CCamera *pCamera = NULL;
	CBase *pBase = CManager::GetBase();

	if (pBase != NULL)
	{
		pGame = (CGame*)pBase;
		pCamera = pGame->GetCamera();
	}

	// �ϐ��錾
	D3DXVECTOR3 rotDest = GetRotDest();
	D3DXVECTOR3 rotCamera = pCamera->GetRotate();

	// �L�[�{�[�h���̎擾
	CInputKeyboard *pKey = GET_KEYBOARD;

	// CInputKeyboard�C���X�^���X�̎擾
	CInputJoypad *pJoypad = GET_JOYPAD;

	// �Q�[���p�b�h���̎擾
	DIJOYSTATE2 Controller = pJoypad->GetControllerPless(0);

	if (pKey->GetPress(KEY_MOVE_UP) == true)
	{
		if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
		{// ���f���΂߉E��ړ�
			m_move.z -= MOVE_PLAYER * sinf(rotCamera.y - PI_QUARTER);
			m_move.x += MOVE_PLAYER * cosf(rotCamera.y - PI_QUARTER);
			rotDest.y = rotCamera.y - PI_QUARTER * 3.0f;
		}
		else if (pKey->GetPress(KEY_MOVE_LEFT) == true)
		{// ���f���΂߉E��ړ�
			m_move.z += MOVE_PLAYER * sinf(rotCamera.y + PI_QUARTER);
			m_move.x -= MOVE_PLAYER * cosf(rotCamera.y + PI_QUARTER);
			rotDest.y = rotCamera.y + PI_QUARTER * 3.0f;
		}
		else
		{// ���f����ړ�
			m_move.z += MOVE_PLAYER * cosf(rotCamera.y);
			m_move.x += MOVE_PLAYER * sinf(rotCamera.y);
			rotDest.y = rotCamera.y - D3DX_PI;
		}
		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else if (pKey->GetPress(KEY_MOVE_DOWN) == true)
	{
		if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
		{// ���f���΂߉E��ړ�
			m_move.z -= MOVE_PLAYER * sinf(rotCamera.y + PI_QUARTER);
			m_move.x += MOVE_PLAYER * cosf(rotCamera.y + PI_QUARTER);
			rotDest.y = rotCamera.y - PI_QUARTER;
		}
		else if (pKey->GetPress(KEY_MOVE_LEFT) == true)
		{// ���f���΂߉E��ړ�
			m_move.z += MOVE_PLAYER * sinf(rotCamera.y - PI_QUARTER);
			m_move.x -= MOVE_PLAYER * cosf(rotCamera.y - PI_QUARTER);
			rotDest.y = rotCamera.y + PI_QUARTER;
		}
		else
		{// ���f�����ړ�
			m_move.z -= MOVE_PLAYER * cosf(rotCamera.y);
			m_move.x -= MOVE_PLAYER * sinf(rotCamera.y);
			rotDest.y = rotCamera.y;
		}
		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else if (pKey->GetPress(KEY_MOVE_RIGHT) == true)
	{// ���f���E�ړ�
		m_move.z -= MOVE_PLAYER * sinf(rotCamera.y);
		m_move.x += MOVE_PLAYER * cosf(rotCamera.y);
		rotDest.y = rotCamera.y - D3DX_PI / 2.0f;

		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else if (pKey->GetPress(KEY_MOVE_LEFT) == true)
	{// ���f�����ړ�
		m_move.z += MOVE_PLAYER * sinf(rotCamera.y);
		m_move.x -= MOVE_PLAYER * cosf(rotCamera.y);
		// �ړ����Ă���Ƃ��ړ����[�V�����ɐݒ�
		rotDest.y = rotCamera.y + D3DX_PI / 2.0f;

		SetMotion(CPlayer::MOTIONTYPE_MOVE);
	}
	else
	{
		SetMotion(CPlayer::MOTIONTYPE_NEUTORAL);
	}

	// ���N�K�L��������
	if (pKey->GetPress(KEY_DELETE))
	{
		// ���N�K�L�Ƃ̓����蔻��
		CollisionGraffiti();

		// �ړ��ʂ𐧌�����
		m_move *= 0.4f;

		// �����Ă��Ԃɂ���
		m_bCleaner = true;

		// ���[�V�������A�N�V�������[�V�����ɂ���
		SetMotion(CPlayer::MOTIONTYPE_DELETE);

		// �X�L���Q�[�W�p�J�E���g
		m_nCntSkillGauge += 2;

		if (m_nCntSkillGauge >= DELETE_GARAFFITI_NUMBER_MAX)
		{
			m_nCntSkillGauge = DELETE_GARAFFITI_NUMBER_MAX;
		}
	}
	else
	{
		m_bCleaner = false;
	}

	// �����S���؂�ւ�����
	if (pKey->GetTrigger(KEY_SWITCH_ERASER))
	{
		m_pEraser->SwicthType();
	}

	// �X�L������
	if (m_bMaxSkillGauge == true)
	{
		if (pKey->GetTrigger(KEY_SKILL))
		{
			// �X�L�����g�p�������Ƃ�`����
			m_bSkill = true;

			// ���[�V�������X�L���ɂ���
			SetMotion(CPlayer::MOTIONTYPE_SKILL);

			// �l��������
			m_nCntSkillGauge = 0;
			m_bMaxSkillGauge = false;
		}
	}

	// �ړI�̌����̐ݒ�
	SetRotDest(rotDest);
}

//=============================================================================
// ���[�V�����̐ݒ�
//=============================================================================
bool CPlayer_Game::SetMotion(MotionType motionType)
{
	// ���[�V�����̎�ނɂ���ď�����ς���
	switch (motionType)
	{
		// �j���[�g����
	case MOTIONTYPE_NEUTORAL:
		if (GetMotionType() == MOTIONTYPE_DELETE && m_bCleaner == true)
		{
			// ���݂̃��[�V�������W�����v�Ȃ�Ԃ�
			return false;
		}
		break;

		// �ړ�
	case MOTIONTYPE_MOVE:
		if (GetMotionType() == MOTIONTYPE_DELETE && m_bCleaner == true)
		{
			// ���݂̃��[�V�������W�����v�Ȃ�Ԃ�
			return false;
		}
		break;

		// �A�N�V����
	case MOTIONTYPE_DELETE:
		break;

		// �X�L��
	case MOTIONTYPE_SKILL:
		// ���[�V�������J�n�������Ƃ�`����
		SetFinishMotion(false);
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

		// �����S���̃I�t�Z�b�g�ݒ�
		m_pEraser->SetOffset((int)motionType);

		// ���[�V����
		Motion();

		return true;
	}
	return false;
}

//=============================================================================
// �X�e�[�W�Ƃ̓����蔻��
//=============================================================================
bool CPlayer_Game::CollisionStage(void)
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
					// �ړ��ʂ̏�����
					if (m_move.y < 0.0f)
					{
						m_move.y = 0.0f;

						if (m_bColSkill == true)
						{
							m_bColSkill = false;
						}
					}
					// �ʒu�̐ݒ�
					SetPos(pos);

					return true;
				}

				// �ʒu�̐ݒ�
				SetPos(pos);
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
	return false;
}

//=============================================================================
// ���N�K�L�Ƃ̓����蔻��
//=============================================================================
void CPlayer_Game::CollisionGraffiti(void)
{
	// �����̎擾
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_GRAFFITI);

	// NULL�`�F�b�N
	if (pScene != NULL)
	{
		while (pScene)
		{
			// ���̏���ۑ�
			CScene *pSceneNext = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_GRAFFITI)
			{
				// �I�u�W�F�N�g�̎�ނ����N�K�L�������烉�N�K�L�ɃL���X�g
				CGraffiti *pGraffiti = (CGraffiti*)pScene;

				// �����S���^�C�v�̎擾
				CEraser::EraserType eraserType = m_pEraser->GetType();

				// �����S���̈ʒu�擾
				D3DXVECTOR3 posEraser = m_pEraser->GetPosition();
				D3DXVECTOR3 sizeEraser = m_pEraser->GetSize();

				// �����蔻��
				if (pGraffiti->Collision(posEraser, sizeEraser, eraserType))
				{
					// �����������J�E���g
					m_nCntDeleteGraffiti++;

					// �X�L���Q�[�W�p�J�E���g
					m_nCntSkillGauge += 5;

					if (m_nCntSkillGauge >= DELETE_GARAFFITI_NUMBER_MAX)
					{
						m_nCntSkillGauge = DELETE_GARAFFITI_NUMBER_MAX;
					}
				}
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �����J�X�Ƃ̓����蔻��
//=============================================================================
void CPlayer_Game::CollisionEraserDust(void)
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

			if (pScene->GetObjType() == CScene::OBJTYPE_ERASERDUST)
			{
				// �I�u�W�F�N�g�̎�ނ�	�����J�X��������L���X�g
				CEraserDust *pEraserDust = (CEraserDust*)pScene;

				// �����蔻��
 				pEraserDust->Collision(GetPos(), GetRot());
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �X�L���Ƃ̓����蔻��
//=============================================================================
void CPlayer_Game::CollisionSkill(void)
{
	// �����̎擾
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pScene != NULL)
	{
		while (pScene)
		{
			// ���̏���ۑ�
			CScene *pSceneNext = pScene->GetNext();

			// �I�u�W�F�N�g�̎�ނ����������珰�ɃL���X�g
			if (pScene->GetObjType() == CScene::OBJTYPE_SKILL)
			{
				CSkill *pSkill = (CSkill*)pScene;

				// �ʒu�ƃT�C�Y�̎擾
				D3DXVECTOR3 pos = GetPos();
				D3DXVECTOR3 size = GetSize();

				// �����蔻��
				if (pSkill->Collision(pos, size, &m_move, D3DXVECTOR3(15.0f, 10.0f, 15.0f)))
				{
					m_bColSkill = true;

					float fRata = (float)m_nCntDeleteGraffiti * 0.5f;

					m_nCntDeleteGraffiti -= (int)fRata;

					if (m_nCntDeleteGraffiti < 0)
					{
						m_nCntDeleteGraffiti = 0;
					}
				}
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �X�L���Ƃ̓����蔻��
//=============================================================================
void CPlayer_Game::CollisionPlayer(void)
{
	// �����̎擾
	CScene *pScene = CScene::GetScene(CScene::PRIORITY_PLAYER);

	// NULL�`�F�b�N
	if (pScene != NULL)
	{
		while (pScene)
		{
			// ���̏���ۑ�
			CScene *pSceneNext = pScene->GetNext();

			// �I�u�W�F�N�g�̎�ނ����������珰�ɃL���X�g
			if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
			{
				CPlayer_Game *pPlayer = (CPlayer_Game*)pScene;

				// �v���C���[�̔ԍ��������ȊO�̎�����
				if (pPlayer->GetNumber() != GetNumber())
				{
					// �ʒu�ƃT�C�Y�̎擾
					D3DXVECTOR3 pos = GetPos();
					D3DXVECTOR3 size = GetSize();

					// �Ώۂ̈ʒu�ƃT�C�Y�̎擾
					D3DXVECTOR3 posPlayer = pPlayer->GetPos();
					D3DXVECTOR3 sizePlayer = pPlayer->GetSize();

					// ���������߂�
					D3DXVECTOR3 vec = posPlayer - pos;
					float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

					// ���鋗����Z���Ƃ������蔻������
					if (fLength < 25.0f)
					{
						// �ړ��ʂ����߂�
						float fRate = 1.0f - (fLength / 25.0f);
						pos.x -= vec.x * fRate;
						pos.z -= vec.z * fRate;
						posPlayer.x += vec.x * fRate;
						posPlayer.z += vec.z * fRate;
						SetPos(pos);
						pPlayer->SetPos(posPlayer);
					}
				}
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �����S���̃I�t�Z�b�g���̓ǂݍ���
//=============================================================================
void CPlayer_Game::LoadEraser(void)
{
	// FILE�^�̃|�C���^�֐�
	FILE *pFile;

	// �t�@�C�����J��
	pFile = fopen("data/TEXT/eraser.txt", "r");

	// NULL�`�F�b�N
	if (pFile != NULL)
	{
		// �ϐ��錾
		char aLine[128];
		char aText[128];
		bool bScript = false;
		bool bPartsSet = false;
		bool bTypeSet = false;
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntType = 0;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;

		// SCRIPT��T��
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// ���s�󔒂�������܂œǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPT���������炻��ȍ~��ǂݍ��ދ���^����
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				bScript = true;
			}
			if (bScript == true)
			{
				// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
				if (strcmp(&aText[0], "TYPESET") == 0)
				{
					bTypeSet = true;
				}
				// �e�p�[�c�̏����i�[���鋖������Ƃ�����
				if (bTypeSet == true)
				{
					// PARTSSET��&aText[0]�e�p�[�c�̏����i�[���鋖��^����
					if (strcmp(&aText[0], "OFFSET") == 0)
					{
						bPartsSet = true;
					}
					// �e�p�[�c�̏����i�[���鋖������Ƃ�����
					if (bPartsSet == true)
					{
						// POS������������W�����i�[
						if (strcmp(&aText[0], "POS") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&pos.x,
								&pos.y,
								&pos.z);
						}
						// ROT����������p�x�����i�[
						if (strcmp(&aText[0], "ROT") == 0)
						{
							fscanf(pFile, "%s %f %f %f", &aText[0],
								&rot.x,
								&rot.y,
								&rot.z);
						}
						// END_PARTSSET����������e�p�[�c�̏����i�[���I����
						if (strcmp(&aText[0], "END_OFFSET") == 0)
						{
							bPartsSet = false;
							m_pEraser->InitOffset(nCntParts, nCntType, pos, rot);

							// �p�[�c�̑��������i�[����̂ŃJ�E���g�𑝂₷
							nCntParts++;
						}
					}
					// END_PARTSSET����������e�p�[�c�̏����i�[���I����
					if (strcmp(&aText[0], "END_TYPESET") == 0)
					{
						bTypeSet = false;

						// �p�[�c�̑��������i�[����̂ŃJ�E���g�𑝂₷
						nCntType++;
						nCntParts = 0;
					}
				}
				// END_SCRIPT����������ǂݍ��݂��I����
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					break;
				}
			}
		}
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
	}
}

//=============================================================================
// �v���C���[�}�[�J�[���̐ݒ�
//=============================================================================
void CPlayer_Game::SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera)
{
	m_pPlayerMarker = pPlayerMarker;

	m_pPlayerMarker->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pPlayerMarker->SetTex(D3DXVECTOR2((float)GetNumber(), 1));

	D3DXVECTOR3 pos = pCamera->ChangePos_WorldtoScreen(GetPos());
	m_pPlayerMarker->SetPosition(D3DXVECTOR3(pos.x, pos.y, pos.z));
}

//=============================================================================
// �����S�����̐ݒ�
//=============================================================================
void CPlayer_Game::SetEraser(CEraser *pEraser)
{
	m_pEraser = pEraser;

	m_pEraser->GetModel()->SetParent(GetModel(3));
	LoadEraser();
	m_pEraser->SetOffset((int)GetMotionType());
}