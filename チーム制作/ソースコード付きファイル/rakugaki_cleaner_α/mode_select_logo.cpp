//=============================================================================
//
// ���[�h�I�����S���� [mode_select_logo.cpp]
// Author : 
//
//=============================================================================
#include "mode_select_logo.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CModeSelectLogo�̃R���X�g���N�^
//=============================================================================
CModeSelectLogo::CModeSelectLogo(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CModeSelectLogo�̃f�X�g���N�^
//=============================================================================
CModeSelectLogo::~CModeSelectLogo()
{

}

//=============================================================================
// ��������
//=============================================================================
CModeSelectLogo *CModeSelectLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// �|�C���^�ϐ��̐���
	CModeSelectLogo *pModeSelectLogo;
	pModeSelectLogo = new CModeSelectLogo(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pModeSelectLogo != NULL)
	{
		// ����������
		pModeSelectLogo->Init(pos, size, nTex);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pModeSelectLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pModeSelectLogo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModeSelectLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �e�N�X�`���Ǘ����̎擾
	CTexture *pTex = CManager::GetTexture();

	// ���NULL����Ȃ��Ƃ��e�N�X�`����K�p����
	if (pTex != NULL)
	{
		BindTexture(pTex->GetAddress(nTex));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModeSelectLogo::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CModeSelectLogo::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModeSelectLogo::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}