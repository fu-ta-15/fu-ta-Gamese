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
// 前方宣言
//-----------------------------------------------------------------------------
class CBullet;

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

	//-----------------------------------------------------------------------------
	// メンバ関数
	//-----------------------------------------------------------------------------
	CPlayer();
	~CPlayer();

	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetCollEnemy(bool bcoll) { m_bCollEnemy = bcoll; }
	void SetState(StateType state) { m_state = state; }
	D3DXVECTOR3 GetSize(void)	{ return m_size; }
	bool GetStay(void)			{ return m_bStay; }

private:
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
	bool				m_bUse;				// 生存確認
	bool				m_bJunp;			// ジャンプ
	bool				m_bStay;			// 止まっているかどうか
	bool				m_bBound;			// メッシュの反発
	bool				m_bCollEnemy;		// 敵との当たり判定
	D3DXVECTOR3			m_pos;				// 位置
	D3DXVECTOR3			m_KnockUpPos;		// ノックアップの位置
	D3DXVECTOR3			m_move;				// 移動量
	D3DXVECTOR3			m_size;				// サイズ
	D3DXVECTOR3			m_moveBullet;		// バレットの移動量
	D3DXVECTOR2			m_tex;				// テクスチャの分割数
	D3DXVECTOR2			m_number;			// テクスチャ何番目か
	D3DXCOLOR			m_col;				// 色

	StateType			m_state;
	CEffect				*m_pShield;
};


#endif // !_PLAYER_H_
