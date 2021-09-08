//*****************************************************************************
//
// シーン処理 [scene.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ
//-----------------------------------------------------------------------------
#define MAX_OBJECT		(128)

//-----------------------------------------------------------------------------
// クラス
//-----------------------------------------------------------------------------
class CScene
{
public:
	// オブジェクトタイプ
	typedef enum ObjType	
	{
		OBJ_NONE = 0,	// シンプルオブジェクト
		OBJ_BULLET,		// バレットオブジェクト
		OBJ_PLAYER,		// プレイヤーオブジェクト
		OBJ_ENEMY,		// 敵オブジェクト
		OBJ_MAX
	}ObjectType;	
	
	enum PauseType
	{
		PAUSE_BG = 0,
		PAUSE_BUTTON1,
		PAUSE_BUTTON2,
		PAUSE_MAX
	};

	CScene();								// コンストラクタ
	CScene(ObjType type);					// オーバロードされたコンストラクタ
	CScene(PauseType type);					// オーバロードされたコンストラクタ
	CScene(bool bpause);					// オーバロードされたコンストラクタ

	// 仮想純粋関数
	virtual ~CScene();						// デストラクタ
	virtual HRESULT Init(void)	= 0;		// 初期化処理
	virtual void Uninit(void)	= 0;		// 終了処理
	virtual void Update(void)	= 0;		// 更新処理
	virtual void Draw(void)		= 0;		// 描画処理

	static void ReleaseAll(void);			// すべてのオブジェクト削除
	static void UpdateAll(void);			// すべてのオブジェクトの更新
	static void DrawAll(void);				// すべてのオブジェクトの更新

	/* Set関数 */
	void SetPos(D3DXVECTOR3 pos);			// 位置の設定
	void SetSize(D3DXVECTOR3 size);			// サイズの設定
	void SetCol(D3DXCOLOR col);				// 色の設定

	/* Get関数 */
	static int GetObjeNum(ObjType type);						// 特定のオブジェクト数
	static D3DXVECTOR3 GetPos(ObjType type, int nID);			// 特定のオブジェクトの位置取得
	static D3DXVECTOR3 GetSize(ObjType type, int nID);			// 特定のオブジェクトのサイズ取得
	static D3DXCOLOR GetCol(ObjType type, int nID);

	void Release(void);									// 個々のオブジェクトの削除
	static void ObjRelease(ObjType type, int nID);				// 特定のオブジェクトの削除
	static void PauseRelease(void);

protected:

private:
	static CScene		*m_apScene[OBJ_MAX][MAX_OBJECT];   // シーンの静的変数
	static CScene		*m_pPauseScene;					   // シーンを止める静的変数
	static CScene		*m_pPauseObj[PAUSE_MAX];
	static int			 m_nNumAll;						   // すべてのオブジェクト数
	static bool			 m_bPause;
	ObjType				 m_type;						   // オブジェクタイプ
	D3DXVECTOR3			 m_pos;							   // 位置
	D3DXVECTOR3			 m_size;						   // サイズ
	D3DXCOLOR			 m_col;							   // 色
	bool				 m_bBool;
	int					 m_nID;							   // 特定のナンバー
};


#endif