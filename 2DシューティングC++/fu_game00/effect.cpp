//*****************************************************************************
//
// �G�t�F�N�g���� [effect.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "effect.h"
#include "renderer.h"
#include "game.h"
#include "mesh.h"
#include "collision.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect() : CScene2D(OBJ_EFFECT)
{
	this->m_bUse = true;
	this->m_col = WhiteColor;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// �G�t�F�N�g�|���S���̂̐���
//=============================================================================
CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{
		pEffect = new CEffect;
		pEffect->m_pos = pos;
		pEffect->m_size = size;
		pEffect->m_bMove = false;
		pEffect->m_bGravity = false;
		pEffect->Init();
	}

	return pEffect;
}

CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{
		pEffect = new CEffect;
		pEffect->m_pos = pos;
		pEffect->m_size = size;
		pEffect->m_move = move;
		pEffect->m_bMove = true;
		pEffect->m_bGravity = true;
		pEffect->Init();
	}

	return pEffect;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CEffect::Init(void)
{
	CScene2D::Init(m_pos, m_size);
	CScene2D::SetPos(m_pos);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	if (m_bMove)
	{
		if (m_bGravity == true)
		{
			m_move.y += 0.45f;
		}

		m_pos += m_move;

		m_move.x += (0 - m_move.x)*0.025f;

		m_col.a -= 0.035f;

		if (m_col.a < 0)
		{
			Release();
		}
		CollisionField();
	}

	CScene2D::SetPos(m_pos);
	CScene2D::SetSize(m_size);
	CScene2D::SetCol(m_col);
	CScene2D::SetUse(m_bUse);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// �������@
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �\�[�X�i�`�挳�j�̍������@�̐ݒ�
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �f�X�e�B�l�[�V�����i�`���j�̍������@�̐ݒ�/�摜�̓����x�����f
}

void CEffect::CollisionField(void)
{
	CMesh* pMesh = NULL;
	pMesh = MESH_GAME;

	// ���_���̎擾
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// ��ӂ̒��S���W�ݒ�
	D3DXVECTOR3 posA = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);

	// �O�ϓ����蔻��
	bool bOutPro = false;

	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{// ���b�V���|���S����ӂ̂ݎZ�o
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{// ��̒��_�Ɠ_�̊O�ϔ���
			bOutPro = CCollision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);
		}
		if (bOutPro == true)
		{// �_����_��艺�ɂ�����

			m_pos.y = pVtx[nCnt].pos.y - m_size.y;

			break;
		}
	}

}

