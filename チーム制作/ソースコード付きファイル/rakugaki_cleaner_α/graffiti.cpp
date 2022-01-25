//=============================================================================
//
// ラクガキ処理 [graffiti.cpp]
// Author : 
//
//=============================================================================
#include "graffiti.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "eraserdust.h"
#include "eraser.h"
#include "base.h"
#include "game.h"
#include "graffiti_manager.h"

#include "debugproc.h"

//=============================================================================
// CGraffitiのコンストラクタ
//=============================================================================
CGraffiti::CGraffiti(int nPriority) : CScene3D(nPriority)
{
	// 変数の初期化
}

//=============================================================================
// CGraffitiのデストラクタ
//=============================================================================
CGraffiti::~CGraffiti()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CGraffiti *CGraffiti::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col)
{
	// ポインタ変数の生成
	CGraffiti *pGraffiti;
	pGraffiti = new CGraffiti(CScene::PRIORITY_GRAFFITI);

	// NULLチェック
	if (pGraffiti != NULL)
	{
		// 初期化処理
		pGraffiti->Init(pos, size, col);

		// オブジェクトタイプの設定
		pGraffiti->SetObjType(OBJTYPE_GRAFFITI);
	}

	return pGraffiti;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGraffiti::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col)
{
	// CScene2Dの初期化処理
	CScene3D::Init(pos, size);

	// 色の指定
	SetCol(col);

	// テクスチャ管理情報の取得
	CTexture *pTexture = CManager::GetTexture();

	if (pTexture != NULL)
	{
		// テクスチャの割り当て
		BindTexture(NULL);
	}

	if (col == D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
	{
		m_graffitiCol = GRAFFITICOLOR_BLACK;
	}
	else
	{
		m_graffitiCol = GRAFFITICOLOR_COLORFUL;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGraffiti::Uninit(void)
{
	// CScene2Dの終了処理
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGraffiti::Update(void)
{
	// CScene2Dの更新処理
	CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CGraffiti::Draw(void)
{
	// CScene2Dの描画処理
	CScene3D::Draw();
}

//=============================================================================
// ラクガキの当たり判定処理
//=============================================================================
bool CGraffiti::Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nType)
{
	// 位置とサイズの取得
	D3DXVECTOR3 posGraffiti = GetPosition();
	D3DXVECTOR2 sizeGraffiti = GetSize();

	if ((m_graffitiCol == GRAFFITICOLOR_BLACK && nType == CEraser::ERASERTYPE_NORMAL) ||
		(m_graffitiCol == GRAFFITICOLOR_COLORFUL && nType == CEraser::ERASERTYPE_SAND))
	{
		// 当たっていたら色を薄くしていく
		if (pos.x - size.x < posGraffiti.x + sizeGraffiti.x && pos.x + size.x > posGraffiti.x - sizeGraffiti.x &&
			pos.z - size.z < posGraffiti.z + sizeGraffiti.y && pos.z + size.z > posGraffiti.z - sizeGraffiti.y)
		{
			// 色の取得
			D3DXCOLOR col = GetCol();

			// α値が0.0fより大きいなら処理(同時消し回避)
			if (col.a > 0.0f)
			{
				// 半透明にする
				col.a -= 0.1f;

				// 色の設定
				SetCol(col);

				// 0.0f以下になったら削除
				if (col.a <= 0.0f)
				{
					Uninit();

					// 消しカスを生成
					if (rand() % 2 == 0)
					{
						CEraserDust::Create(posGraffiti, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data/MODEL/ErasingRemaining_00.x");
					}
				}

				// 現在のラクガキ数を減らす
				CBase *pBase = CManager::GetBase();

				if (pBase != NULL)
				{
					CGame *pGame = (CGame*)pBase;
					CGraffitiManager *pGraffitiManager = pGame->GetGraffitiManager();

					pGraffitiManager->SetCurGraffiti(-1);
				}

				return true;
			}
		}
	}
	return false;
}