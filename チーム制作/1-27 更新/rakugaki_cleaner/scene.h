//=============================================================================
//
// シーン処理 [scene.h]
// Author : 
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)	// 頂点フォーマット
#define MAX_VERTEX (4)												// 頂点の数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef  struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex1;
	D3DXVECTOR2 tex2;
} VERTEX_3D;

//-----------------------------------------------------------------------------
// オブジェクトクラス(基本クラス)
//-----------------------------------------------------------------------------
class CScene
{
public:
	// オブジェクトの種類
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_SHADOW,
		OBJTYPE_BG,				// 背景
		OBJTYPE_LOGO,			// ロゴ
		OBJTYPE_STAGE,			// ステージ
		OBJTYPE_ERASER,			// 消しゴム
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_FLOOR,			// 床(地面)
		OBJTYPE_MODEL,			// 床(地面)
		OBJTYPE_ERASERDUST,		// 消しカス
		OBJTYPE_GRAFFITI,		// ラクガキ
		OBJTYPE_SKILL,			// スキル
		OBJTYPE_TIMER,			// タイマー
		OBJTYPE_SCORE,			// スコア
		OBJTYPE_PLAYER_MARKER,	// プレイヤーマーカー
		OBJTYPE_UI,				// UI
		OBJTYPE_TUTORIAL,		// チュートリアル
		OBJTYPE_PAUSE,			// ポーズ
		OBJTYPE_MAX,	
	}OBJTYPE;			

	// 描画の優先順位
	typedef enum
	{
		PRIORITY_NONE = 0,		// なし
		PRIORITY_BG,			// 背景
		PRIORITY_MODEL,			// モデル
		PRIORITY_FLOOR,			// 床(地面)
		PRIORITY_PLAYER,		// プレイヤー
		PRIORITY_ERASER,		// 消しゴム
		PRIORITY_GRAFFITI,		// ラクガキ
		PRIORITY_SHADOW,		// 影
		PRIORITY_UI,			// UI
		PRIORITY_PAUSE,			// ポーズ
		PRIORITY_MAX,			
	}PRIORITY;

	CScene();
	CScene(int nPriolity);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	OBJTYPE GetObjType(void) { return m_objType; }
	void SetObjType(OBJTYPE objType) { m_objType = objType; }
	CScene *GetScene(int nPriority) { return m_pTop[nPriority]; }
	CScene *GetScene(int nPriority,int nCnt);
	CScene *GetNext(void) { return m_pNext; }
	bool GetDeath(void) { return m_bDeath; }

protected:
	void Release(void);

private:
	static void UpdateTitle(void);			// タイトルでの更新処理
	static void UpdateGame(void);			// ゲームでの更新処理
	static void UpdateResult(void);			// リザルトでの更新処理
	static void ReleaseScene(void);			// 破棄処理
	OBJTYPE m_objType;						// オブジェクトの種類
	static CScene *m_pTop[PRIORITY_MAX];	// 先頭のオブジェクトへのポインタ
	static CScene *m_pCur[PRIORITY_MAX];	// 現在(最後尾)のオブジェクトへのポインタ
	CScene *m_pPrev;						// 前のオブジェクトへのポインタ
	CScene *m_pNext;						// 次のオブジェクトへのポインタ
	bool m_bDeath;							// 死亡フラグ
};
#endif