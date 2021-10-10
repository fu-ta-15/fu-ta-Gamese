//*****************************************************************************
//
// �o���b�g���� [bullet.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BULLET_H_
#define	_BULLET_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "scene2D.h"

//-----------------------------------------------------------------------------
// �}�N���ϐ�
//-----------------------------------------------------------------------------
#define MAX_BULLET		(256)	// �o���b�g�̍ő吔


//-----------------------------------------------------------------------------
// �o���b�g�N���X
//-----------------------------------------------------------------------------
class CBullet : public CScene2D		// �|���S��2D���p��
{
public:

	CBullet();	   // �R���X�g���N�^
	~CBullet();	   // �f�X�g���N�^

	// create�֐�
	static CBullet *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move);	// �o���b�g�̃C���X�^���X�����֐�

	// �����o�֐�
	HRESULT Init(void);		
	void Uninit(void);		
	void Update(void);		
	void Draw(void);		

	// BOOL�֐�
	bool CollisionBullet(D3DXVECTOR3 pos ,D3DXVECTOR3 size);	

	// Set�֐�


private:

	D3DXVECTOR3			m_pos;			// �ʒu
	D3DXVECTOR3			m_size;			// �T�C�Y
	D3DXVECTOR3			m_move;			// �ړ���

	int					m_BulletStock;
	bool				m_bUse;			// �g�p���Ă��邩
	bool				m_Collision;	// �����蔻��
	bool				m_ColiPlayer;
};
#endif // !_BULLET_H_
