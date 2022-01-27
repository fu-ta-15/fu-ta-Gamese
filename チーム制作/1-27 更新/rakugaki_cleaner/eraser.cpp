//=============================================================================
//
// �����S������ [eraser.cpp]
// Author : 
//
//=============================================================================
#include "eraser.h"
#include "scene.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input_keyboard.h"
#include "player.h"
#include "stage.h"
#include "skill.h"
#include "keep_object.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
// CWood�̃R���X�g���N�^
//=============================================================================
CEraser::CEraser(int nPriority) : CScene(nPriority)
{

}

//=============================================================================
// CEraser�̃f�X�g���N�^
//=============================================================================
CEraser::~CEraser()
{

}

//=============================================================================
// ��������
//=============================================================================
CEraser *CEraser::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CEraser *pEraser;
	pEraser = new CEraser(CScene::PRIORITY_ERASER);

	// NULL�`�F�b�N
	if (pEraser != NULL)
	{
		// ����������
		pEraser->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pEraser->SetObjType(OBJTYPE_ERASER);
	}

	return pEraser;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEraser::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ϐ��̏�����
	m_pos = pos;
	m_size = D3DXVECTOR3(size.x, size.y, size.x);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_eraserType = ERASERTYPE_NORMAL;
	m_nNumOffset = 0;
	m_bColStage = false;

	// �������̊m��
	m_pOffsetPos = new D3DXVECTOR3 *[CPlayer::MOTIONTYPE_MAX];
	m_pOffsetRot = new D3DXVECTOR3 *[CPlayer::MOTIONTYPE_MAX];

	for (int nCnt = 0; nCnt < CPlayer::MOTIONTYPE_MAX; nCnt++)
	{
		m_pOffsetPos[nCnt] = new D3DXVECTOR3[ERASERTYPE_MAX];
		m_pOffsetRot[nCnt] = new D3DXVECTOR3[ERASERTYPE_MAX];
	}

	for (int nCntPattern = 0; nCntPattern < CPlayer::MOTIONTYPE_MAX; nCntPattern++)
	{
		for (int nCntType = 0; nCntType < ERASERTYPE_MAX; nCntType++)
		{
			m_pOffsetPos[nCntPattern][nCntType] = { 0.0f,0.0f,0.0f };
			m_pOffsetRot[nCntPattern][nCntType] = { 0.0f,0.0f,0.0f };
		}
	}

	// ���f���̓ǂݍ���
	for (int nCntModel = 0; nCntModel < 1; nCntModel++)
	{
		m_pModel = CModel::Create(D3DXVECTOR3(m_pOffsetPos[m_nNumOffset][m_eraserType].x, m_pOffsetPos[m_nNumOffset][m_eraserType].y, m_pOffsetPos[m_nNumOffset][m_eraserType].z),
			D3DXVECTOR3(m_pOffsetRot[m_nNumOffset][m_eraserType].x, m_pOffsetRot[m_nNumOffset][m_eraserType].y, m_pOffsetRot[m_nNumOffset][m_eraserType].z),
			"data/MODEL/eraser.x");
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEraser::Uninit(void)
{
	// �������̊J��
	delete[] m_pOffsetPos;
	m_pOffsetPos = NULL;

	delete[] m_pOffsetRot;
	m_pOffsetRot = NULL;

	// �폜
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEraser::Update(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEraser::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �ϐ��̎󂯓n��
	pDevice = CManager::GetRenderer()->GetDevice();

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

	for (int nCnt = 0; nCnt < 1; nCnt++)
	{
		// �p�[�c�̕`��
		m_pModel->Draw();
	}
}

//=============================================================================
// �����S���^�C�v�̐؂�ւ�����
//=============================================================================
int CEraser::SwicthType(void)
{
	switch (m_eraserType)
	{
	case ERASERTYPE_NORMAL:
		m_eraserType = ERASERTYPE_SAND;
		break;

	case ERASERTYPE_SAND:
		m_eraserType = ERASERTYPE_NORMAL;
		break;
	}

	// �I�t�Z�b�g�ݒ�
	SetOffset();

	return (int)m_eraserType;
}

//=============================================================================
// �I�t�Z�b�g�̕ύX
//=============================================================================
void CEraser::SetOffset(int nNum)
{
	// ���Ԗڂ̃I�t�Z�b�g���g�����ۑ�
	m_nNumOffset = nNum;

	// �I�t�Z�b�g�ݒ�
	SetOffset();
}

//=============================================================================
// �I�t�Z�b�g�̐ݒ�
//=============================================================================
void CEraser::SetOffset(void)
{
	// �I�t�Z�b�g�̈ʒu
	m_pModel->SetPosition(D3DXVECTOR3(m_pOffsetPos[m_nNumOffset][m_eraserType].x,
		m_pOffsetPos[m_nNumOffset][m_eraserType].y,
		m_pOffsetPos[m_nNumOffset][m_eraserType].z));

	// �I�t�Z�b�g�̌���
	m_pModel->SetRot(D3DXVECTOR3(m_pOffsetRot[m_nNumOffset][m_eraserType].x,
		m_pOffsetRot[m_nNumOffset][m_eraserType].y,
		m_pOffsetRot[m_nNumOffset][m_eraserType].z));
}

//=============================================================================
// �I�t�Z�b�g�̐ݒ�
//=============================================================================
void CEraser::InitOffset(int nType,int nCntPattern, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �I�t�Z�b�g�̐ݒ�
	m_pOffsetPos[nCntPattern][nType] = pos;
	m_pOffsetRot[nCntPattern][nType] = rot;
}

//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CEraser::GetPosition(void)
{
	// �}�g���b�N�X���̎擾
	D3DXMATRIX mtx = m_pModel->GetMtxWorld();

	// �ʒu����Ԃ�
	return (D3DXVECTOR3(mtx._41,mtx._42,mtx._43));
}

//=============================================================================
// �X�e�[�W�Ƃ̓����蔻��
//=============================================================================
bool CEraser::CollisionStage(int nRank)
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
				pos.y -= 24.0f;

				// �����蔻��
				if (pStage->Collision(&pos, GetSize()))
				{
					// �ړ��ʂ̏�����
					m_move.y = 0.0f;

					if (m_bColStage == false)
					{
						// �X�L���͈�
						CSkill::Create(D3DXVECTOR3(pos.x,pos.y + 1.0f,pos.z), D3DXVECTOR2(180.0f, 180.0f));

						m_bColStage = true;
					}

					return true;
				}
			}

			// ���̏��ɂ���
			pScene = pSceneNext;
		}
	}
	return false;
}