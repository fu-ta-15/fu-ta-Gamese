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
#include "effect.h"

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

	D3DXVECTOR3 GetSize(void) { return m_size; }

private:
	void PosControl(void);
	void PlayerAction(void);
	void PlayerMove(void);
	void PlayerMoveControl(void);
	void FieldControl(void);
	void PlayerAnime(void);

	int							m_nAnimeCnt;		// �A�j���[�V�����J�E���^�[
	float						m_fStayTime;		// �Ƃǂ܂��Ă鎞��
	bool						m_bUse;				// �����m�F
	bool						m_bJunp;			// �W�����v
	bool						m_bStay;			// �~�܂��Ă��邩�ǂ���
	bool						m_bBound;			// ���b�V���̔���
	D3DXVECTOR3					m_pos;				// �ʒu
	D3DXVECTOR3					m_move;				// �ړ���
	D3DXVECTOR3					m_size;				// �T�C�Y
	D3DXVECTOR3					m_moveBullet;		// �o���b�g�̈ړ���
	D3DXVECTOR2					m_tex;				// �e�N�X�`���̕�����
	D3DXVECTOR2					m_number;			// �e�N�X�`�����Ԗڂ�
	D3DXCOLOR					m_col;				// �F

	CEffect						*m_pShield;
};


#endif // !_PLAYER_H_
