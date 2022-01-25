//=============================================================================
//
// �X�L������ [skill.cpp]
// Author : 
//
//=============================================================================
#include "shadow.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "debugproc.h"

//=============================================================================
// CShadow�̃R���X�g���N�^
//=============================================================================
CShadow::CShadow(int nPriority) : CScene3D(nPriority)
{
	// �ϐ��̏�����
}

//=============================================================================
// CShadow�̃f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// ��������
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �|�C���^�ϐ��̐���
	CShadow *pShadow;
	pShadow = new CShadow(CScene::PRIORITY_SHADOW);

	// NULL�`�F�b�N
	if (pShadow != NULL)
	{
		// ����������
		pShadow->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pShadow->SetObjType(OBJTYPE_SHADOW);
	}

	return pShadow;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CScene2D�̏���������
	CScene3D::Init(pos, size);

	// �e�N�X�`���̐ݒ�
	CTexture *pTex = CManager::GetTexture();

	if (pTex != NULL)
	{
		BindTexture(pTex->GetAddress(pTex->SetTexture("data/TEXTURE/shadow000.jpg")));
	}

	SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,0.5f));
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadow::Uninit(void)
{
	// CScene2D�̏I������
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadow::Update(void)
{
	// CScene2D�̍X�V����
	CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// CScene2D�̕`�揈��
	CScene3D::Draw();

	// �ʏ�̐ݒ�ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}