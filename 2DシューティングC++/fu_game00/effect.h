//*****************************************************************************
//
// �G�t�F�N�g���� [effect.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "manager.h"
#include "scene2D.h"

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CEffect : public CScene2D
{
public:

	CEffect();
	~CEffect();

	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	static CEffect *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXVECTOR3 move, LPDIRECT3DTEXTURE9 ptex);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CollisionField(void);

	// Set�֐�
	void SetColor(D3DXCOLOR col)		{ m_col = col; }
	void SetMove(D3DXVECTOR3 move)		{ m_move = move; }
	void SetUse(bool bUse)				{ m_bUse = bUse; }
	void SetGravity(bool bGravity)		{ m_bGravity = bGravity; }
	void SetPos(D3DXVECTOR3 pos)		{ m_pos = pos; }

	// Get�֐�
	D3DXCOLOR GetColor(void)	{ return m_col; }
	D3DXVECTOR3 GetPos(void)	{ return m_pos; }
	D3DXVECTOR3 GetMove(void)	{ return m_move; }
	bool GetUse(void)			{ return m_bUse; }

private:
	D3DXVECTOR3					m_pos;		   // �ʒu
	D3DXVECTOR3					m_size;		   // �T�C�Y
	D3DXVECTOR3					m_move;		   // �ړ���
	D3DXCOLOR					m_col;		   // �F
	bool						m_bUse;		   // �g�p���Ă��邩�ǂ���
	bool						m_bMove;	   // �Œ�E���R
	bool						m_bGravity;	   // �d�̗͂L��

};
#endif // !_EFFECT_H_