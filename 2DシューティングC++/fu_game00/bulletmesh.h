//*****************************************************************************
//
// �o���b�g���b�V���|���S������ [bulletmesh.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BULLETMESH_H_
#define	_BULLETMESH_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "mesh.h"


//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CBulletMesh : public CMesh		// �|���S��2D���p��
{
public:

	CBulletMesh();	   
	~CBulletMesh();	   

	// create�֐�
	static CBulletMesh *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move);	

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// Set�֐�


private:

	D3DXVECTOR3			m_pos;			// �ʒu
	D3DXVECTOR3			m_size;			// �T�C�Y
	D3DXVECTOR3			m_move;			// �ړ���

	int					m_nFrameTime;
	int					m_nVtxID;
	float				m_AddSize;
	float				m_fWaveTime;
	bool				m_bUse;			// �g�p���Ă��邩
	bool				m_Collision;	// �����蔻��
	bool				m_ColiPlayer;
};
#endif // !_BULLET_H_
