//=============================================================================
//
// ���N�K�L���� [graffiti.h]
// Author : 
//
//=============================================================================
#ifndef _SKILL_H_
#define _SKILL_H_

#include "scene3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//-----------------------------------------------------------------------------
// �X�L���N���X(�h���N���X)
//-----------------------------------------------------------------------------
class CSkill : public CScene3D
{
public:
	CSkill(int nPriority);
	~CSkill();

	static CSkill *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 *pMove,D3DXVECTOR3 moveBase);			// �����蔻��

private:
	int m_nCnt;
};
#endif