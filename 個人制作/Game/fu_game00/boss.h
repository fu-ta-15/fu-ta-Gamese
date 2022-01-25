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
#include "effect.h"
#include "mesh.h"
#include "core.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define BOSS_LIFE_STOCK		(BOSS_LIFE / 10)				// ライフのストック


//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CBoss : public CEnemy
{
public:
	// ボスの状態
	typedef enum BOSS_STATE
	{
		STATE_NONE = 0,
		STATE_DAMAGE,
		STATE_NOT_DAMAGE,
		STATE_MAX
	}BOSS_STATE;

	// ボスのライフの状態
	typedef enum LIFE_STATE
	{
		LIFE_RATE_0 = 0,
		LIFE_RATE_2,
		LIFE_RATE_5,
		LIFE_RATE_8,
		LIFE_RATE_MAX
	}LIFE_STATE;

	CBoss();	  // コンストラクタ
	~CBoss();	  // デストラクタ

	static CBoss *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size,const int nLife);  // クリエイト

	HRESULT Init(void);	   // 初期化
	void Uninit(void);	   // 終了
	void Update(void);	   // 更新
	void Draw(void);	   // 描画

	// Set関数
	void SetState(BOSS_STATE state)		{ m_State = state; }
	void SetAlive(bool bAlive)			{ m_bBoss_Alive = bAlive; }

	// Get関数
	CCore **GetCore(void)				{ return &m_pCore[0]; }
	BOSS_STATE GetState(void)			{ return m_State; }
	LIFE_STATE GetLifeState(void)		{ return m_LifeState; }
	float GetLife(void)					{ return m_fLife; }
	bool GetAlive(void)					{ return m_bBoss_Alive; }

private:
	void DamageBoss(void);
	LIFE_STATE LifeState(int nLife);
	void MoveBoss(void);
	void SummonsEnemy(void);
	void StateUpdate(void);

	CEffect				*m_pDamage;					 // ダメージ時のエフェクト
	CScene2D			*m_pLife[BOSS_LIFE_STOCK];	 // ライフのストック
	CScene2D			*m_pShiled;					 // シールド
	CCore				*m_pCore[3];				 // コア
	BOSS_STATE			m_State;					 // ボスの状態
	LIFE_STATE			m_LifeState;				 // HPの状態
	D3DXVECTOR3			m_OldPos;					 // 過去の位置
	D3DXCOLOR			m_StateCol;					 // 状態の色
	static bool			m_bBoss_Alive;				 // ボスの生存確認
	bool				m_bDamage;					 // ダメージフラグ
	int					m_nDamageCnt;				 // ダメージカウント
	float				m_fLife;					 // ライフ
	float				m_fA_Damage;				 // 透明度の加算用
	float				m_fMoveTime;				 // 移動用のカウント
};

#endif // !_BOSS_H_
