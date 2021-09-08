//*****************************************************************************
//
// ボス敵処理 [boss.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _BOSS_H_
#define _BOSS_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "enemy.h"

class CBoss : public CEnemy
{
public:
	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CBoss();
	~CBoss();

	static CBoss *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const int nLife);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdateBoss(void);
	void DamageBoss(void);

	static void SetAlive(bool bAlive) { m_bBoss_Alive = bAlive; }
	static bool GetAlive(void) { return m_bBoss_Alive; }

private:
	static bool m_bBoss_Alive;
	int m_nCnt = 0;
};

#endif // !_BOSS_H_
