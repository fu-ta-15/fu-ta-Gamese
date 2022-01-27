//=============================================================================
//
// ������ [floor.h]
// Author : 
//
//=============================================================================
#ifndef _FLOOR_H_
#define _FLOOR_H_

#include "sceneMeshfield.h"

//-----------------------------------------------------------------------------
// ���N���X(��{�N���X)
//-----------------------------------------------------------------------------
class CFloor : public CSceneMeshfield
{
public:
	CFloor(int nPriority);
	~CFloor();

	static CFloor *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,D3DXVECTOR2 block,int nTex);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 block, int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);

private:
	int m_nTex;		// �e�N�X�`���i���o�[
};
#endif