//=============================================================================
//
// ステージ処理 [stage.cpp]
// Author : 
//
//=============================================================================
#include "stage.h"
#include "manager.h"
#include "renderer.h"
#include "eraserdust.h"

#include "debugproc.h"

//=============================================================================
// CStageのコンストラクタ
//=============================================================================
CStage::CStage(int nPriority) : CSceneX(nPriority)
{
	// 変数の初期化
}

//=============================================================================
// CStageのデストラクタ
//=============================================================================
CStage::~CStage()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CStage *CStage::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 rot,char *pFileName)
{
	// ポインタ変数の生成
	CStage *pStage;
	pStage = new CStage(CScene::PRIORITY_MODEL);

	// NULLチェック
	if (pStage != NULL)
	{
		// 角度の指定
		pStage->SetRotate(rot);

		// ファイルネームの指定
		pStage->BindXFileName(pFileName);

		// 初期化処理
		pStage->Init(pos, size);

		// オブジェクトタイプの設定
		pStage->SetObjType(OBJTYPE_STAGE);
	}

	return pStage;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CStage::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// CScene2Dの初期化処理
	CSceneX::Init(pos, size);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CStage::Uninit(void)
{
	// CSceneXの終了処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CStage::Update(void)
{
	// CSceneXの更新処理
	CSceneX::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CStage::Draw(void)
{
	// CSceneXの描画処理
	CSceneX::Draw();
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CStage::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 size)
{
	// 変数宣言
	bool bLand = false;

	// 位置とサイズの取得
	D3DXVECTOR3 posStage = GetPosition();
	D3DXVECTOR3 vtxMax = GetVtxMax();
	D3DXVECTOR3 vtxMin = GetVtxMin();

	// 出ないようにする
	if (pPos->x - size.x < posStage.x - vtxMin.x)
	{
		// 左
		pPos->x = posStage.x - vtxMin.x + size.x;
	}
	else if (pPos->x + size.x > posStage.x + vtxMax.x)
	{
		// 右
		pPos->x = posStage.x + vtxMax.x - size.x;
	}
	if (pPos->z - size.z < posStage.z - vtxMin.z)
	{
		// 手前
		pPos->z = posStage.z - vtxMin.z + size.z;
	}
	else if (pPos->z + size.z > posStage.z + vtxMax.z)
	{
		// 奥
		pPos->z = posStage.z + vtxMax.z - size.z;
	}

	// 上
	if (pPos->y < posStage.y + vtxMax.y)
	{
		pPos->y = posStage.y + vtxMax.y;

		bLand = true;
	}

	return bLand;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
bool CStage::CollisionUp(D3DXVECTOR3 *pPos, D3DXVECTOR3 size)
{
	// 変数宣言
	bool bLand = false;

	// 位置とサイズの取得
	D3DXVECTOR3 posStage = GetPosition();
	D3DXVECTOR3 vtxMax = GetVtxMax();
	D3DXVECTOR3 vtxMin = GetVtxMin();

	// 上
	if (pPos->x - size.x >= posStage.x - vtxMin.x && pPos->x + size.x <= posStage.x + vtxMax.x &&
		pPos->z - size.z >= posStage.z - vtxMin.z && pPos->z + size.z <= posStage.z + vtxMax.z)
	{
		if (pPos->y < posStage.y + vtxMax.y)
		{
			pPos->y = posStage.y + vtxMax.y;

			bLand = true;
		}
	}

	return bLand;
}