//#############################################################################
//
// ���������w�b�_�t�@�C�� [eraserdust.h]
// Author : SUZUKI FUUTA
//
//#############################################################################
#ifndef _ERASERDUST_H_
#define _ERASERDUST_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "sceneX.h"

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CEraserDust : public CSceneX
{
public:

	CEraserDust(int nPriority);
	~CEraserDust();

	static CEraserDust *Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, D3DXVECTOR3 rot, char *pModelFileName);
	HRESULT Init(const D3DXVECTOR3& pos, const D3DXVECTOR2& size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	bool CollisionStage(void);				// �X�e�[�W�Ƃ̓����蔻��
	void CollisionSkill(void);				// �X�L���Ƃ̓����蔻��

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_move;
};

#endif
