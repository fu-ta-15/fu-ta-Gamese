//*****************************************************************************
//
// プレイヤー処理 [player.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene2D.h"
#include "effect.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define MAX_STOCK	(5)

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CPlayer : public CScene2D
{
public:
	typedef enum StateType
	{
		STATE_NONE = 0,
		STATE_DAMAGE,
		STATE_KNOCKUP,
		STATE_MAX
	}StateType;

	//-------------------------------------------------------------------------
	// メンバ関数
	//-------------------------------------------------------------------------
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Set関数
	void SetCollEnemy(bool bcoll)	{ m_bCollEnemy = bcoll; }
	void SetState(StateType state)	{ m_state = state; }

	// Get関数
	D3DXVECTOR3 GetSize(void)		{ return m_size; }
	bool GetStay(void)				{ return m_bStay; }
	bool GetDamage(void)			{ return m_bDamage; }
	bool GetAlive(void)				{ return m_bAlive; }

private:
	void StockUpdate(void);
	void PosControl(void);
	void PlayerAction(void);
	void PlayerMove(void);
	void PlayerMoveControl(void);
	void FieldControl(void);
	void PlayerAnime(void);
	void PlayerState(void);
	void DamagePlayer(void);

	int					m_nAnimeCnt;		// アニメーションカウンター
	int					m_nDamageCnt;		// ダメージカウント

	float				m_fStayTime;		// とどまってる時間
	float				m_fStockTime;		// ストックが存在している時間

	bool				m_bUse;				// 生存確認
	bool				m_bJunp;			// ジャンプ
	bool				m_bStay;			// 止まっているかどうか
	bool				m_bCollEnemy;		// 敵との当たり判定
	bool				m_bFall;			// 落下判定
	bool				m_bDamage;			// ダメージの判定
	bool				m_bAlive;			// 生きているかどうか

	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_KnockUpPos;		// ノックアップの位置
	D3DXVECTOR3			m_move;				// 移動量
	D3DXVECTOR3			m_StockMove;		// ストックの移動量
	D3DXVECTOR3			m_size;				// サイズ
	D3DXVECTOR3			m_moveBullet;		// バレットの移動量
	D3DXVECTOR2			m_tex;				// テクスチャの分割数
	D3DXVECTOR2			m_number;			// テクスチャ何番目か
	D3DXCOLOR			m_col;				// 色

	StateType			m_state;

	CScene2D			*m_pGaugeStock[MAX_STOCK];
	bool				m_bStock[MAX_STOCK];

	CEffect				*m_pShield;
};


#endif // !_PLAYER_H_
