//=============================================================================
//
// �X�e�[�W���� [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"
#include "manager.h"
#include "renderer.h"
#include "eraserdust.h"

#include "debugproc.h"

//=============================================================================
// CStage�̃R���X�g���N�^
//=============================================================================
CStage::CStage(int nPriority) : CSceneX(nPriority)
{
	// �ϐ��̏�����
}

//=============================================================================
// CStage�̃f�X�g���N�^
//=============================================================================
CStage::~CStage()
{

}

//=============================================================================
// ��������
//=============================================================================
CStage *CStage::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot,char *pFileName)
{
	// �|�C���^�ϐ��̐���
	CStage *pStage;
	pStage = new CStage(CScene::PRIORITY_MODEL);

	// NULL�`�F�b�N
	if (pStage != NULL)
	{
		// �p�x�̎w��
		pStage->SetRotate(rot);

		// �t�@�C���l�[���̎w��
		pStage->BindXFileName(pFileName);

		// ����������
		pStage->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pStage->SetObjType(OBJTYPE_STAGE);
	}

	return pStage;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CStage::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CScene2D�̏���������
	CSceneX::Init(pos, size);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CStage::Uninit(void)
{
	// CSceneX�̏I������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CStage::Update(void)
{
	// CSceneX�̍X�V����
	CSceneX::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CStage::Draw(void)
{
	// CSceneX�̕`�揈��
	CSceneX::Draw();
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
bool CStage::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 size)
{
	// �ϐ��錾
	bool bLand = false;

	// �ʒu�ƃT�C�Y�̎擾
	D3DXVECTOR3 posStage = GetPosition();
	D3DXVECTOR3 vtxMax = GetVtxMax();
	D3DXVECTOR3 vtxMin = GetVtxMin();

	// �o�Ȃ��悤�ɂ���
	if (pPos->x - size.x < posStage.x - vtxMin.x)
	{
		// ��
		pPos->x = posStage.x - vtxMin.x + size.x;
	}
	else if (pPos->x + size.x > posStage.x + vtxMax.x)
	{
		// �E
		pPos->x = posStage.x + vtxMax.x - size.x;
	}
	if (pPos->z - size.z < posStage.z - vtxMin.z)
	{
		// ��O
		pPos->z = posStage.z - vtxMin.z + size.z;
	}
	else if (pPos->z + size.z > posStage.z + vtxMax.z)
	{
		// ��
		pPos->z = posStage.z + vtxMax.z - size.z;
	}

	// ��
	if (pPos->y < posStage.y + vtxMax.y)
	{
		pPos->y = posStage.y + vtxMax.y;

		bLand = true;
	}

	return bLand;
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
bool CStage::CollisionUp(D3DXVECTOR3 *pPos, D3DXVECTOR3 size)
{
	// �ϐ��錾
	bool bLand = false;

	// �ʒu�ƃT�C�Y�̎擾
	D3DXVECTOR3 posStage = GetPosition();
	D3DXVECTOR3 vtxMax = GetVtxMax();
	D3DXVECTOR3 vtxMin = GetVtxMin();

	// ��
	if (pPos->x - size.x >= posStage.x - vtxMin.x && pPos->x + size.x <= posStage.x + vtxMax.x &&
		pPos->z - size.z >= posStage.z - vtxMin.z && pPos->z + size.z <= posStage.z + vtxMax.z)
	{
		if (pPos->y < posStage.y + vtxMax.y)
		{
			pPos->y = posStage.y + vtxMax.y;

			bLand = true;
		}
	}

	return bLand;
}