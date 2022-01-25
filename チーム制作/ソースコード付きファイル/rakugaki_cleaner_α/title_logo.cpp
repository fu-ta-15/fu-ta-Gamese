//=============================================================================
//
// �^�C�g�����S���� [title_logo.cpp]
// Author : 
//
//=============================================================================
#include "title_logo.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input_keyboard.h"
#include "input_joypad.h"

#include "title.h"

//=============================================================================
// CTitleLogo�̃R���X�g���N�^
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CTitleLogo�̃f�X�g���N�^
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
// ��������
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// �|�C���^�ϐ��̐���
	CTitleLogo *pTitleLogo;
	pTitleLogo = new CTitleLogo(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pTitleLogo != NULL)
	{
		// ����������
		pTitleLogo->Init(pos, size, nTex);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pTitleLogo->SetObjType(OBJTYPE_LOGO);
	}

	return pTitleLogo;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitleLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
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
void CTitleLogo::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitleLogo::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitleLogo::Draw(void)
{
	// CScene2D�̕`�揈��
	CScene2D::Draw();
}