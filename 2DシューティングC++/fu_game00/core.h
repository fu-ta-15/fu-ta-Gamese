//*****************************************************************************
//
// �R�A���� [core.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _CORE_H_
#define	_CORE_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene2D.h"

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CCore : public CScene2D
{
public:

	CCore(Priority nPriority);
	~CCore();

	// create�֐�
	static CCore *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, Priority nPriority);

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// BOOL�֐�
	bool CollisionBullet(void);

	// Set�֐�


private:

	D3DXVECTOR3			m_pos;			// �ʒu
	D3DXVECTOR3			m_size;			// �T�C�Y
	D3DXVECTOR3			m_move;			// �ړ���

	bool				m_bUse;			// �g�p���Ă��邩
	bool				m_Collision;	// �����蔻��
};
#endif // !_BULLET_H_
