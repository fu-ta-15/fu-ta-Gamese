//*****************************************************************************
//
// �o���b�g���b�V���|���S������ [bulletmesh.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "bulletmesh.h"
#include "manager.h"
#include "game.h"
#include "move.h"
#include "player.h"

#define PLAYER_GET_POS			(CGame::GetPlayer()->GetPos())
#define ANGLE_POINT_X			(m_pos.x - PLAYER_GET_POS.x)
#define ANGLE_POINT_Y			(m_pos.y - PLAYER_GET_POS.y)
#define ANGLE_SIN				(sinf(D3DX_PI + fAngle))
#define ANGLE_COS				(cosf(D3DX_PI - fAngle))

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBulletMesh::CBulletMesh() : CMesh(OBJ_BULLET)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBulletMesh::~CBulletMesh()
{
}

//=============================================================================
// Mesh�̃o���b�g����
//=============================================================================
CBulletMesh * CBulletMesh::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move)
{
	CBulletMesh *pMBullet = NULL;

	if (pMBullet == NULL)
	{
		pMBullet = new CBulletMesh;
		pMBullet->m_pos = pos;
		pMBullet->m_size = size;
		pMBullet->m_move = move;
		pMBullet->Init();
	}

	return pMBullet;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CBulletMesh::Init(void)
{
	CMesh::Init(10, 0, m_pos, m_size);
	// ���_���̎擾
	VERTEX_2D *pVtx = this->GetVERTEX();
	m_AddSize = 50.0f;
	m_nFrameTime = 0;
	m_nVtxID = 0;
	m_fWaveTime++;

	for (int nCnt = 0; nCnt < this->GetVtxNum() / 2; nCnt++)
	{
		D3DXVECTOR3 pos = m_pos;
		pVtx[nCnt].pos.y = Move::SinWave(pos.y, 5.0f, 10.0f, m_fWaveTime + (nCnt * 2));
		pos.y += m_size.y;
		pVtx[nCnt + this->GetVtxNum() / 2].pos.y = Move::SinWave(pos.y, 5.0f, 10.0f, m_fWaveTime + (nCnt * 2));
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBulletMesh::Uninit(void)
{
	CMesh::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBulletMesh::Update(void)
{
	// ���_���̎擾
	VERTEX_2D *pVtx = this->GetVERTEX();

	m_fWaveTime += 0.8f;
	m_nFrameTime++;

	if ((m_nFrameTime % 1) == 0)
	{
		if (m_nVtxID < this->GetVtxNum() / 2)
		{
			m_nVtxID++;
		}	
	}

	for (int nCnt = this->GetVtxNum() / 2 - 1; nCnt > this->GetVtxNum() / 2 - 1 - m_nVtxID; nCnt--)
	{
		D3DXVECTOR3 pos = m_pos;
		pVtx[nCnt].pos.x += m_move.x;
		pVtx[nCnt].pos.y = Move::SinWave(pos.y, 20.0f, m_move.x, m_fWaveTime+(nCnt*2));
		pos.y += m_size.y;
		pVtx[nCnt + this->GetVtxNum() / 2].pos.x += m_move.x;
		pVtx[nCnt+ this->GetVtxNum() / 2].pos.y = Move::SinWave(pos.y, 20.0f, m_move.x, m_fWaveTime+(nCnt*2));
	}

	if (pVtx[0].pos.x > SCREEN_WIDTH)
	{
		Release();
	}

	CMesh::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBulletMesh::Draw(void)
{
	CMesh::Draw();
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CBulletMesh::CollisionBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	return false;
}
