//=============================================================================
//
// �������� [crown.cpp]
// Author : 
//
//=============================================================================
#include "crown.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CCrown�̃R���X�g���N�^
//=============================================================================
CCrown::CCrown(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CCrown�̃f�X�g���N�^
//=============================================================================
CCrown::~CCrown()
{

}

//=============================================================================
// ��������
//=============================================================================
CCrown *CCrown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// �|�C���^�ϐ��̐���
	CCrown *pCrown;
	pCrown = new CCrown(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pCrown != NULL)
	{
		// ����������
		pCrown->Init(pos, size, nTex);

		// �I�u�W�F�N�g�̎�ނ�ݒ�
		pCrown->SetObjType(CScene::OBJTYPE_PLAYER_MARKER);
	}

	return pCrown;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCrown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nTex)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, size);

	// �e�N�X�`���̓\��t��
	CTexture *pTex = CManager::GetTexture();

	if (pTex != NULL)
	{
		CScene2D::BindTexture(pTex->GetAddress(nTex));
	}

	m_bDisplay = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCrown::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCrown::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCrown::Draw(void)
{
	if (m_bDisplay == true)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// �A���t�@�e�X�g
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 120);

		// CScene2D�̕`�揈��
		CScene2D::Draw();

		// �A���t�@�e�X�g�����̐ݒ�ɖ߂�
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
	}
}