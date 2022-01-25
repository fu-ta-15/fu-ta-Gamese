//=============================================================================
//
// ������ [floor.cpp]
// Author : 
//
//=============================================================================
#include "floor.h"
#include "manager.h"
#include "renderer.h"

#include "texture.h"
#include "debugproc.h"

//=============================================================================
// CFloor�̃R���X�g���N�^
//=============================================================================
CFloor::CFloor(int nPriority) : CSceneMeshfield(nPriority)
{
	// �ϐ��̏�����
}

//=============================================================================
// CFloor�̃f�X�g���N�^
//=============================================================================
CFloor::~CFloor()
{

}

//=============================================================================
// ��������
//=============================================================================
CFloor *CFloor::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 block, int nTex)
{
	// �|�C���^�ϐ��̐���
	CFloor *pFloor;
	pFloor = new CFloor(CScene::PRIORITY_FLOOR);

	// NULL�`�F�b�N
	if (pFloor != NULL)
	{
		// ����������
		pFloor->Init(pos, size,block,nTex);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pFloor->SetObjType(OBJTYPE_FLOOR);
	}

	return pFloor;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFloor::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 block, int nTex)
{
	// �u���b�N���̐ݒ�
	SetBlock(D3DXVECTOR2(block.x, block.y));

	// �T�C�Y�̊m��
	size.x /= block.x;
	size.y /= block.y;

	// CScene3D�̏���������
	CSceneMeshfield::Init(pos, size);

	// �e�N�X�`���̊��蓖��
	BindTextureNum(nTex);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFloor::Uninit(void)
{
	// CScene3D�̏I������
	CSceneMeshfield::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFloor::Update(void)
{
	// CScene3D�̍X�V����
	CSceneMeshfield::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFloor::Draw(void)
{
	// CScene3D�̕`�揈��
	CSceneMeshfield::Draw();
}

//=============================================================================
// ���̓����蔻��
//=============================================================================
bool CFloor::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	// ���̈ʒu�����擾
	D3DXVECTOR3 posFloor = GetPosition();

	// ���̃T�C�Y�����擾
	D3DXVECTOR2 sizeFloor = GetSize();

	// �u���b�N���̎擾
	D3DXVECTOR2 block = GetBlock();

	// �T�C�Y�̕␳
	sizeFloor.x *= block.x / 2.0f;
	sizeFloor.y *= block.y / 2.0f;

	// �I�u�W�F�N�g�Ƃ̓����蔻��
	if (pPos->y < posFloor.y && posOld.y >= posFloor.y&&
		(pPos->x >= posFloor.x - sizeFloor.x && pPos->x <= posFloor.x + sizeFloor.x) &&
		(pPos->z >= posFloor.z - sizeFloor.y && pPos->z <= posFloor.z + sizeFloor.y))
	{
		pPos->y = posFloor.y;

		return true;
	}

	return false;
}