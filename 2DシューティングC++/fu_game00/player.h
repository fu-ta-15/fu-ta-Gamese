//*****************************************************************************
//
// �v���C���[���� [player.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene2D.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CBullet;

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	//-----------------------------------------------------------------------------
	// �����o�֐�
	//-----------------------------------------------------------------------------
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void PosControl(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);
	void PlayerAction(const D3DXVECTOR3 pos, const D3DXVECTOR3 move);

	float						m_fG;			// �d��
	bool						m_bJunp;		// �W�����v

	D3DXVECTOR3					m_pos;			// �ʒu
	D3DXVECTOR3					m_move;			// �ʒu
	D3DXVECTOR3					m_size;			// �T�C�Y
	D3DXVECTOR3					m_moveBullet;	// �o���b�g�̈ړ���
	D3DXCOLOR					m_col;			// �F
	bool						m_bUse;			// �����m�F
};


#endif // !_PLAYER_H_
