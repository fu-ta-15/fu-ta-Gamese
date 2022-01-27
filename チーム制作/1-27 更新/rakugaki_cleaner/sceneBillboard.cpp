//=============================================================================
//
// �V�[���r���{�[�h���� [sceneBillboard.cpp]
// Author : 
//
//=============================================================================
#include "sceneBillboard.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_X		(1)							// �A�j���[�V�����p�^�[��No.�̉��̍ő吔
#define TEXTURE_Y		(1)							// �A�j���[�V�����p�^�[��No.�̏c�̍ő吔

//=============================================================================
// CSceneBillboard�̃R���X�g���N�^
//=============================================================================
CSceneBillboard::CSceneBillboard(int nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//=============================================================================
// CSceneBillboard�̃f�X�g���N�^
//=============================================================================
CSceneBillboard::~CSceneBillboard()
{

}

//=============================================================================
// ��������
//=============================================================================
CSceneBillboard *CSceneBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot)
{
	// �|�C���^�ϐ��̐���
	CSceneBillboard *pBillboard;
	pBillboard = new CSceneBillboard(CScene::PRIORITY_FLOOR);

	// NULL�`�F�b�N
	if (pBillboard != NULL)
	{
		// ����������
		pBillboard->Init(pos, size);

		// �I�u�W�F�N�g�^�C�v�̐ݒ�
		pBillboard->SetObjType(OBJTYPE_FLOOR);
	}

	return pBillboard;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��̏�����
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = size.x / 2.0f;
	m_fHeight = size.y / 2.0f;

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MAX_VERTEX,	// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - m_fWidth, pos.y + m_fHeight, pos.z);
	pVtx[1].pos = D3DXVECTOR3(pos.x + m_fWidth, pos.y + m_fHeight, pos.z);
	pVtx[2].pos = D3DXVECTOR3(pos.x - m_fWidth, pos.y - m_fHeight, pos.z);
	pVtx[3].pos = D3DXVECTOR3(pos.x + m_fWidth, pos.y - m_fHeight, pos.z);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W
	pVtx[0].tex1 = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex1 = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex1 = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex1 = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneBillboard::Uninit(void)
{
	if (m_bDeath == true)
	{
		// ���_�o�b�t�@�̊J��
		if (m_pVtxBuff != NULL)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = NULL;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneBillboard::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneBillboard::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRender = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				// �r���[�}�g���b�N�X�擾�p

	// ���C�g���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@��K�p
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
#if 1
	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;
#else
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;
#endif

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, m_pTexture);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �W���ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�g���I���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �e�N�X�`�����̊��蓖��
//=============================================================================
void CSceneBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	// �e�N�X�`���������蓖�Ă�
	m_pTexture = pTexture;
}

//=============================================================================
// ���W�ݒ菈��
//=============================================================================
void CSceneBillboard::SetPosition(D3DXVECTOR3 pos)
{
	// ���W�ݒ�
	m_pos = pos;

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// �T�C�Y�ݒ菈��
//=============================================================================
void CSceneBillboard::SetSize(D3DXVECTOR2 size)
{
	// ���W�ݒ�
	m_fWidth = size.x;
	m_fHeight = size.y;

	// ���_���W�̐ݒ�
	SetVertex();
}

//=============================================================================
// ���_���W�̐ݒ菈��
//=============================================================================
void CSceneBillboard::SetVertex(void)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̍X�V
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x - sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle + m_rot.z) * m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.y = m_pos.y - cosf(m_fAngle - m_rot.z) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���_�J���[�̐ݒ菈��
//=============================================================================
void CSceneBillboard::SetCol(D3DXCOLOR col)
{
	// �ϐ��̍X�V
	m_col = col;

	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̍X�V
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ菈��
//=============================================================================
void CSceneBillboard::SetTex(D3DXVECTOR2 texUV, D3DXVECTOR2 texAnimetionSheets)
{
	// �\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̍X�V
	pVtx[0].tex1 = D3DXVECTOR2(texUV.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[1].tex1 = D3DXVECTOR2(texUV.x, texUV.y);
	pVtx[2].tex1 = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y + 1.0f / texAnimetionSheets.y);
	pVtx[3].tex1 = D3DXVECTOR2(texUV.x + 1.0f / texAnimetionSheets.x, texUV.y);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}