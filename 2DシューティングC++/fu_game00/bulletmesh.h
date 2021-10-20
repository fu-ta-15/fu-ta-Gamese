//*****************************************************************************
//
// バレットメッシュポリゴン処理 [bulletmesh.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BULLETMESH_H_
#define	_BULLETMESH_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "mesh.h"


//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CBulletMesh : public CMesh		// ポリゴン2Dを継承
{
public:

	CBulletMesh();	   
	~CBulletMesh();	   

	// create関数
	static CBulletMesh *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 move);	

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool CollisionBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	// Set関数


private:

	D3DXVECTOR3			m_pos;			// 位置
	D3DXVECTOR3			m_size;			// サイズ
	D3DXVECTOR3			m_move;			// 移動量

	int					m_nFrameTime;
	int					m_nVtxID;
	float				m_AddSize;
	float				m_fWaveTime;
	bool				m_bUse;			// 使用しているか
	bool				m_Collision;	// 当たり判定
	bool				m_ColiPlayer;
};
#endif // !_BULLET_H_
