//=============================================================================
//
// 消しゴム処理 [eraser.h]
// Author : 
//
//=============================================================================
#ifndef _ERASER_H_
#define _ERASER_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;		// モデルクラス
class CPlayer;

//-----------------------------------------------------------------------------
// 消しゴムクラス(派生クラス)
//-----------------------------------------------------------------------------
class CEraser : public CScene
{
public:
	// 消しゴムの種類
	typedef enum 
	{
		ERASERTYPE_NORMAL = 0,	// 通常の消しゴム
		ERASERTYPE_SAND,		// 砂消し
		ERASERTYPE_MAX,	
	}EraserType;

	CEraser(int nPriority);
	~CEraser();

	static CEraser *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CModel *GetModel(void) { return m_pModel; }
	EraserType GetType(void) { return m_eraserType; }
	void InitOffset(int nType, int nCntPattern, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetOffset(int nNum);
	void SetOffset(void);
	int SwicthType(void);

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void) { return m_size; }
	void SetColStage(bool bCol) { m_bColStage = bCol; }

	bool CollisionStage(int nRank);				// ステージとの当たり判定

private:
	CModel *m_pModel;						// モデルへのポインタ
	float **m_pOffset;						// 親モデルからのオフセット
	D3DXVECTOR3 **m_pOffsetPos;				// 親モデルからの位置オフセット
	D3DXVECTOR3 **m_pOffsetRot;				// 親モデルからの向きオフセット
	char m_apModelFileName[128];			// モデルファイルネーム
	int m_nNumModel;						// モデル数
	int m_nNumOffset;						// 何番目のオフセットか

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 現在の位置
	D3DXVECTOR3 m_posOld;					// 前回の位置
	D3DXVECTOR3 m_size;						// サイズ
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_move;						// 移動量

	EraserType m_eraserType;				// 消しゴムのタイプ
	bool m_bColStage;						// ステージに当たったかどうか
};
#endif
