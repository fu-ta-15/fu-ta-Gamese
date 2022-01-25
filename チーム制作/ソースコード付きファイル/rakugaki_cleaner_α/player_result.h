//=============================================================================
//
// リザルトシーンのプレイヤー処理 [player_result.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYERREULT_H_
#define _PLAYERREULT_H_

#include "player.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayerMarker;	// プレイヤーマーカークラス
class CRank;			// ランククラス
class CCamera;

//-----------------------------------------------------------------------------
// プレイヤークラス(派生クラス)
//-----------------------------------------------------------------------------
class CPlayer_Result : public CPlayer
{
public:
	CPlayer_Result(int nPriority);
	~CPlayer_Result();

	static CPlayer_Result *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPlayerMarker *GetPlayerMarker(void) { return m_pPlayerMarker; }
	void SetPlayerMarker(CPlayerMarker *pPlayerMarker, CCamera *pCamera);
	void SetRank(CRank *pRank, int nRank, CCamera *pCamera);

	bool SetMotion(MotionType motionType);	// モーションの設定

private:
	bool CollisionStage(void);				// ステージの当たり判定

	D3DXVECTOR3 m_move;
	CPlayerMarker *m_pPlayerMarker;			// プレイヤーマーカー情報のポインタ
	CRank *m_pRank;
};
#endif