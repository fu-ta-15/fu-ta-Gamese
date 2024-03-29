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
class CScore;
class CTime;
class CBoss;
class CScene2D;
class CMesh;
class CMesh3D;

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CGame : public CScene
{
public:
	//-------------------------------------------------------------------------
	// メンバ関数
	//-------------------------------------------------------------------------
	CGame();
	~CGame();

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBoss* GetBoss(void)		{ return m_pBoss; }
	static CMesh* GetMesh(void)		{ return m_pField; }
	static CPlayer* GetPlayer(void)	{ return m_pPlayer; }
	static CTime* GetTime(void)		{ return m_pTime; }

private:

	int				m_nWaveCnt;

	static CPlayer		*m_pPlayer;
	static CScore		*m_pScore;
	static CTime		*m_pTime;
	static CBoss		*m_pBoss;
	static CMesh		*m_pField;
	static CMesh3D		*m_pBg3D;
};

#endif // !1
