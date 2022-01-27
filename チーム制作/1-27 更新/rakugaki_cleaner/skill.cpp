//=============================================================================
//
// �X�L������ [skill.cpp]
// Author : 
//
//=============================================================================
#include "skill.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "eraserdust.h"
#include "eraser.h"
#include "base.h"
#include "game.h"
#include "graffiti_manager.h"

#include "debugproc.h"

//=============================================================================
// CSkill�̃R���X�g���N�^
//=============================================================================
CSkill::CSkill(int nPriority) : CScene3D(nPriority)
{
	// �ϐ��̏�����
}

//=============================================================================
// CSkill�̃f�X�g���N�^
//=============================================================================
CSkill::~CSkill()
{

}

//=============================================================================
// ��������
//=============================================================================
CSkill *CSkill::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CSkill *pSkill;
	pSkill = new CSkill(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pSkill != NULL)
	{
		// ����������
		pSkill->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pSkill->SetObjType(OBJTYPE_SKILL);
	}

	return pSkill;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSkill::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �����o�ϐ��̏�����
	m_nCnt = 0;

	// CScene2D�̏���������
	CScene3D::Init(pos, size);

	// �F�̎w��
	SetCol(D3DXCOLOR(1.0f,0.0f,0.0f,0.5f));

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTexture = CManager::GetTexture();

	if (pTexture != NULL)
	{
		// �e�N�X�`���̊��蓖��
		BindTexture(pTexture->GetAddress(pTexture->SetTexture("data/TEXTURE/effect_explosion.png")));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSkill::Uninit(void)
{
	// CScene2D�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSkill::Update(void)
{
	// CScene2D�̍X�V����
	CScene3D::Update();

	// �J�E���g�X�V
	m_nCnt++;

	// 10�t���[����폜
	if (m_nCnt == 10)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSkill::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene3D::Draw();
}

//=============================================================================
// �X�L���̓����蔻�菈��
//=============================================================================
bool CSkill::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size,D3DXVECTOR3 *pMove, D3DXVECTOR3 moveBase)
{
	// �ʒu�ƃT�C�Y�̎擾
	D3DXVECTOR3 posSkill = GetPosition();
	D3DXVECTOR2 sizeSkill = GetSize();

	// �I�u�W�F�N�g�ƃX�L���͈͂̒��S�Ƃ̋������Z�o
	D3DXVECTOR3 vec = pos - posSkill;
	float fLength = sqrtf(vec.x * vec.x + vec.z * vec.z);

	// �͈͓��Ȃ��΂�
	if (fLength < sizeSkill.x / 2.0f)
	{
		// �p�x�����߂�
		float fAngle = atan2f(vec.x, vec.z);

		// ���������߂�
		D3DXVECTOR2 ratio;
		float fRatio = (sizeSkill.x - fLength) / sizeSkill.x;
		ratio.x = fLength / (sizeSkill.x / 2.0f);
		ratio.y = (sizeSkill.x - fLength) / (sizeSkill.x);

		// �ړ��ʂ����߂�
		pMove->x = moveBase.x * sinf(fAngle) * fRatio;
		pMove->y = moveBase.y * fRatio;
		pMove->z = moveBase.z * cosf(fAngle) * fRatio;

		return true;
	}

	return false;
}