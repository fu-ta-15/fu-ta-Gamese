//=============================================================================
//
// ゲームシーンのプレイヤー処理 [player_game.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYERGAME_H_
#define _PLAYERGAME_H_

#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DELETE_GARAFFITI_NUMBER_MAX		(3000)				// スキル用

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEraser;			// 消しゴムクラス
class CScoreGauge;		// スコアゲージクラス
class CSkillGauge;		// スキルゲージクラス
class CPlayerMarker;	// プレイヤーマーカークラス
class CCamera;
class CShadow;

//-----------------------------------------------------------------------------
// プレイヤークラス(派生クラス)
//-----------------------------------------------------------------------------
class CPlayer_Game : public CPlayer
{
public:
	// プレイヤーの状態
	typedef enum
	{
		PLAYER_STATE_NORMAL = 0,		// 通常
		PLAYER_STATE_FLYING,			// 吹っ飛び
		PLAYER_STATE_MAX,
	}Player_State;

	CPlayer_Game(int nPriority);
	~CPlayer_Game();

	static CPlayer_Game *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetDeleteGraffiti(void) { return m_nCntDeleteGraffiti; }
	int GetCntSkillGauge(void) { return m_nCntSkillGauge; }
	CScoreGauge *GetScoreGauge(void) { return m_pScoreGauge; }
	void SetScoreGauge(CScoreGauge *pScoreGauge) { m_pScoreGauge = pScoreGauge; }
	CSkillGauge *GetSkillGauge(void) { return m_pSkillGauge; }
	void SetSkillGauge(CSkillGauge *pSkillGauge) { m_pSkillGauge = pSkillGauge; }
	CPlayerMarker *GetPlayerMarker(void) { return m_pPlayerMarker; }
	void SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera);
	CEraser *GetEraser(void) { return m_pEraser; }
	void SetEraser(CEraser *pEraser);

	bool GetMaxSkillGauge(void) { return m_bMaxSkillGauge; }
	void SetMaxSkillGauge(bool bMax) { m_bMaxSkillGauge = bMax; }

	int GetDeleteGraffitiNum(void) { return m_nCntDeleteGraffiti; }

	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetRank(int nRank) { m_nRank = nRank; }

private:
	void Pause(void);
	void Move(void);						// 移動処理
	void MoveKeyboard(void);				// プレイヤーのキーボード操作
	bool SetMotion(MotionType motionType);	// モーションの設定
	bool CollisionStage(void);				// ステージとの当たり判定
	void CollisionGraffiti(void);			// ラクガキとの当たり判定
	void CollisionEraserDust(void);			// 消しカスとの当たり判定
	void CollisionSkill(void);				// スキルとの当たり判定
	void CollisionPlayer(void);				// プレイヤー同士の当たり判定
	void LoadEraser(void);

	CEraser *m_pEraser;						// 消しゴム情報のポインタ

	D3DXVECTOR3 m_move;						// 移動量
	bool m_bCleaner;						// 消してる途中かどうか
	bool m_bSkill;							// スキル発動中かどうか
	bool m_bColEraser;						// 消しゴムがステージと当たったかどうか
	bool m_bColSkill;						// スキルが当たったかどうか

	int m_nRank;							// 順位
	int m_nCntDeleteGraffiti;				// 消したラクガキの数をカウント
	int m_nCntSkillGauge;					// スキルゲージ用のカウント
	CScoreGauge *m_pScoreGauge;				// スコアゲージ
	CSkillGauge *m_pSkillGauge;				// スキルゲージ
	bool m_bMaxSkillGauge;					// スキルゲージが溜まったかどうか

	CPlayerMarker *m_pPlayerMarker;			// プレイヤーマーカー情報のポインタ
	CShadow *m_pShadow;
};
#endif