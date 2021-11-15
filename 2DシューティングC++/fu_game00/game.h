//*****************************************************************************
//
// ゲーム処理 [game.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _GAME_H_
#define	_GAME_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene.h"

//-----------------------------------------------------------------------------
//前方宣言
//-----------------------------------------------------------------------------
class CPlayer;
class CEnemy;
class CTime;
class CBoss;
class CScene2D;
class CMesh;

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CGame : public CScene
{
public:
	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// Get関数
	static CBoss* GetBoss(void)		{ return m_pBoss; }
	static CMesh* GetMesh(void)		{ return m_pField; }
	static CPlayer* GetPlayer(void)	{ return m_pPlayer; }
	static CTime* GetTime(void)		{ return m_pTime; }

private:
	float				m_fWaveHeight;		// 波の高さ
	float				m_nWaveCnt;			// 波を起こすためのカウント
	CMesh				*m_pLifeMesh;		// ボスのダメージ表現のメッシュポリゴン
	static CPlayer		*m_pPlayer;			// プレイヤーのポインタ
	static CTime		*m_pTime;			// タイムのポインタ
	static CBoss		*m_pBoss;			// ボスのポインタ
	static CScene2D		*m_pBg;				// 背景のポインタ
	static CMesh		*m_pField;			// メッシュのポインタ（地面）
};

#endif // !1
