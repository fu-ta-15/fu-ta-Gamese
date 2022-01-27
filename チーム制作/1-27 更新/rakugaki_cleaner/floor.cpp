//=============================================================================
//
// 床処理 [floor.cpp]
// Author : 
//
//=============================================================================
#include "floor.h"
#include "manager.h"
#include "renderer.h"

#include "texture.h"
#include "debugproc.h"

//=============================================================================
// CFloorのコンストラクタ
//=============================================================================
CFloor::CFloor(int nPriority) : CSceneMeshfield(nPriority)
{
	// 変数の初期化
}

//=============================================================================
// CFloorのデストラクタ
//=============================================================================
CFloor::~CFloor()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CFloor *CFloor::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 block, int nTex)
{
	// ポインタ変数の生成
	CFloor *pFloor;
	pFloor = new CFloor(CScene::PRIORITY_FLOOR);

	// NULLチェック
	if (pFloor != NULL)
	{
		// 初期化処理
		pFloor->Init(pos, size,block,nTex);

		// オブジェクトタイプの設定
		pFloor->SetObjType(OBJTYPE_FLOOR);
	}

	return pFloor;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFloor::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR2 block, int nTex)
{
	// ブロック数の設定
	SetBlock(D3DXVECTOR2(block.x, block.y));

	// サイズの確定
	size.x /= block.x;
	size.y /= block.y;

	// CScene3Dの初期化処理
	CSceneMeshfield::Init(pos, size);

	// テクスチャの割り当て
	BindTextureNum(nTex);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFloor::Uninit(void)
{
	// CScene3Dの終了処理
	CSceneMeshfield::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFloor::Update(void)
{
	// CScene3Dの更新処理
	CSceneMeshfield::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CFloor::Draw(void)
{
	// CScene3Dの描画処理
	CSceneMeshfield::Draw();
}

//=============================================================================
// 床の当たり判定
//=============================================================================
bool CFloor::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	// 床の位置情報を取得
	D3DXVECTOR3 posFloor = GetPosition();

	// 床のサイズ情報を取得
	D3DXVECTOR2 sizeFloor = GetSize();

	// ブロック数の取得
	D3DXVECTOR2 block = GetBlock();

	// サイズの補正
	sizeFloor.x *= block.x / 2.0f;
	sizeFloor.y *= block.y / 2.0f;

	// オブジェクトとの当たり判定
	if (pPos->y < posFloor.y && posOld.y >= posFloor.y&&
		(pPos->x >= posFloor.x - sizeFloor.x && pPos->x <= posFloor.x + sizeFloor.x) &&
		(pPos->z >= posFloor.z - sizeFloor.y && pPos->z <= posFloor.z + sizeFloor.y))
	{
		pPos->y = posFloor.y;

		return true;
	}

	return false;
}