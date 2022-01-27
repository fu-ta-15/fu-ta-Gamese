//=============================================================================
//
// �v���C���[�}�[�J�[���� [player_marker.cpp]
// Author : 
//
//=============================================================================
#include "player_marker.h"

#include "manager.h"
#include "renderer.h"
#include "texture.h"

//=============================================================================
// CPlayerMarker�̃R���X�g���N�^
//=============================================================================
CPlayerMarker::CPlayerMarker(int nPriority) : CScene2D(nPriority)
{

}

//=============================================================================
// CPlayerMarker�̃f�X�g���N�^
//=============================================================================
CPlayerMarker::~CPlayerMarker()
{

}

//=============================================================================
// ��������
//=============================================================================
CPlayerMarker *CPlayerMarker::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	// �|�C���^�ϐ��̐���
	CPlayerMarker *pPlayerMarker;
	pPlayerMarker = new CPlayerMarker(CScene::PRIORITY_UI);

	// NULL�`�F�b�N
	if (pPlayerMarker != NULL)
	{
		// ����������
		pPlayerMarker->Init(pos, size, texSheet, nTex);

		// �I�u�W�F�N�g�̎�ނ�ݒ�
		pPlayerMarker->SetObjType(CScene::OBJTYPE_PLAYER_MARKER);
	}

	return pPlayerMarker;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayerMarker::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 texSheet, int nTex)
{
	// CScene2D�̏���������
	CScene2D::Init(pos,size);

	// �e�N�X�`���̓\��t��
	CTexture *pTex = CManager::GetTexture();

	if (pTex != NULL)
	{
		CScene2D::BindTexture(pTex->GetAddress(nTex));
	}

	// �e�N�X�`���̕����ʂ̐ݒ�
	CScene2D::SetTexSheet(texSheet);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayerMarker::Uninit(void)
{
	// CScene2D�̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayerMarker::Update(void)
{
	// CScene2D�̍X�V����
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayerMarker::Draw(void)
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