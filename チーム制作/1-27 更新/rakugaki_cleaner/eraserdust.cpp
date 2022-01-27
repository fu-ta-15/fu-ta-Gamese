//#############################################################################
//
// �������� ����[eraserdust.cpp]
// Author : SUZUKI FUUTA
//
//#############################################################################

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "eraserdust.h"
#include "player.h"
#include "game.h"
#include "stage.h"
#include "skill.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEraserDust::CEraserDust(int nPriority) : CSceneX(nPriority)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEraserDust::~CEraserDust()
{
}

//=============================================================================
// ��������
//=============================================================================
CEraserDust * CEraserDust::Create(const D3DXVECTOR3 & pos, const D3DXVECTOR2 & size, D3DXVECTOR3 rot, char *pModelFileName)
{
	// �ϐ��錾
	CEraserDust *pEraserDust = NULL; 
	pEraserDust = new CEraserDust(PRIORITY_MODEL);

	if (pEraserDust != NULL)
	{
		// ���f���t�@�C���l�[���̐ݒ�
		pEraserDust->BindXFileName(pModelFileName);

		// ��]�̐ݒ�
		pEraserDust->SetRotate(rot);

		// ������
		pEraserDust->Init(pos, size);

		// �I�u�W�F�N�g�̎�ނ�ݒ�
		pEraserDust->SetObjType(OBJTYPE_ERASERDUST);
	}

	return pEraserDust;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEraserDust::Init(const D3DXVECTOR3 & pos, const D3DXVECTOR2 & size)
{
	// CSceneX�̏���������
	CSceneX::Init(pos, size);

	// �ʒu�̕ۑ�
	m_pos = pos;

	// ��]�̏�����
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLength = sqrtf(GetVtxMax().x * GetVtxMax().x + GetVtxMax().z *GetVtxMax().z);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEraserDust::Uninit(void)
{
	// CSceneX�̏I������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEraserDust::Update(void)
{
	// CSceneX�̍X�V����
	CSceneX::Update();

	// �d��
	m_move.y -= 0.3f;

	// ����
	m_move.x += (0.0f - m_move.x) * 0.08f;
	m_move.z += (0.0f - m_move.z) * 0.08f;

	// �ʒu���擾
	D3DXVECTOR3 pos = GetPosition();

	// �ړ��ʂ��ʒu�ɉ��Z
	pos += m_move;

	// �ʒu�̐ݒ�
	SetPosition(pos);

	// �X�e�[�W�Ƃ̓����蔻��
	if (CollisionStage() == false)
	{
		return;
	}

	// �X�L���Ƃ̓����蔻��
	CollisionSkill();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEraserDust::Draw(void)
{
	// CSceneX�̕`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �����J�X�̓����蔻��
//=============================================================================
bool CEraserDust::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ʒu�擾
	D3DXVECTOR3 posEraserDust = GetPosition();

	// �����J�X�͈̔͂ɓ������Ă�����
	if (posEraserDust.x - CSceneX::GetVtxMin().x < pos.x &&
		posEraserDust.x + CSceneX::GetVtxMax().x > pos.x &&
		posEraserDust.z - CSceneX::GetVtxMin().z - 5.0f < pos.z &&
		posEraserDust.z + CSceneX::GetVtxMax().z + 5.0f > pos.z)
	{
		// ���������߂�
		D3DXVECTOR3 vec = posEraserDust - pos;
		float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

		// ���鋗����Z���Ƃ������蔻������
		// �ړ��ʂ����߂�
		float fRate = 1.0f - (fLength / m_fLength);
		/*posEraserDust.x += vec.x * fRate;
		posEraserDust.z += vec.z * fRate;
		m_rot.y += cosf(fLength / m_fLength);*/

		// �ʒu�����炷
		posEraserDust.x += 1.0f * sinf(rot.y);
		posEraserDust.z += 1.0f * cosf(rot.y);

		// �v���C���[�̌����Ŏ��g�̉�]�ɉe������
		m_rot.y = cosf(rot.y*1.5f);

		// ���N���X�ɏ���^����
		CSceneX::SetRotate(m_rot);
		CSceneX::SetPosition(posEraserDust);

		return true;
	}
	return false;
}

//=============================================================================
// �X�L���Ƃ̓����蔻��
//=============================================================================
void CEraserDust::CollisionSkill(void)
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

				// �ʒu�̎擾
				D3DXVECTOR3 pos = GetPosition();

				// �����蔻��
				pSkill->Collision(pos, GetVtxMax(), &m_move,D3DXVECTOR3(8.0f,10.0f, 8.0f));
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �X�e�[�W�Ƃ̓����蔻��
//=============================================================================
bool CEraserDust::CollisionStage(void)
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
				D3DXVECTOR3 pos = GetPosition();

				// �����蔻��
				if (pStage->CollisionUp(&pos, GetVtxMax()))
				{
					// �ړ��ʂ̏�����
					m_move.y = 0.0f;

					// �ʒu�̐ݒ�
					SetPosition(pos);

					return true;
				}
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
	return false;
}
