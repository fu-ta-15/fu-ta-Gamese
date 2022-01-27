//=============================================================================
//
// �������� ����[eraserdust.cpp]
// Author : SUZUKI FUUTA
//
//=============================================================================
#include "shadowS.h"
#include "Scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CShadowS::CShadowS(int nPriority) : CSceneX(nPriority)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CShadowS::~CShadowS()
{
}

//=============================================================================
// ��������
//=============================================================================
CShadowS * CShadowS::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot, char *pModelFileName)
{
	// �ϐ��錾
	CShadowS *pShadows = NULL;
	pShadows = new CShadowS(PRIORITY_SHADOW);

	if (pShadows != NULL)
	{
		// ���f���t�@�C���l�[���̐ݒ�
		pShadows->BindXFileName(pModelFileName);

		// ��]�̐ݒ�
		pShadows->SetRotate(rot);

		// ������
		pShadows->Init(pos, size);

		// �I�u�W�F�N�g�̎�ނ�ݒ�
		pShadows->SetObjType(OBJTYPE_SHADOW);
	}

	return pShadows;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CShadowS::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CSceneX�̏���������
	CSceneX::Init(pos, size);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * MAX_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + SCREEN_HEIGHT / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f - SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - SCREEN_HEIGHT / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + SCREEN_HEIGHT / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f + SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - SCREEN_HEIGHT / 2.0f, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CShadowS::Uninit(void)
{
	// CSceneX�̏I������
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CShadowS::Update(void)
{
	// CSceneX�̍X�V����
	CSceneX::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CShadowS::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �X�e���V���e�X�g��L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// Z�o�b�t�@�ւ̏������݂𖳌�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �J���[�o�b�t�@�ւ̏������݂𖳌�
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// �X�e���V���o�b�t�@�̔�r�p�����[�^�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_INCRSAT);

	// �\�ʂ��J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// CSceneX�̕`�揈��
	CSceneX::Draw();

	// �X�e���V���o�b�t�@�̎Q�ƒl��ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �X�e���V���o�b�t�@�̔�r�p�����[�^�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCRSAT);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);

	// ���ʂ��J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// CSceneX�̕`�揈��
	CSceneX::Draw();

	// �J���[�o�b�t�@�ւ̏������݂�L��
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);

	// �X�e���V���o�b�t�@�̎Q�ƒl��ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// �X�e���V���o�b�t�@�̔�r�p�����[�^�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �ݒ��߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}
