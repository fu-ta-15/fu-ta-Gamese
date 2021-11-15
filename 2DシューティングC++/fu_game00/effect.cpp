//*****************************************************************************
//
// エフェクト処理 [effect.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "effect.h"
#include "renderer.h"
#include "game.h"
#include "mesh.h"
#include "collision.h"
#include "tutorial.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect() : CScene2D(OBJ_EFFECT1)
{
	this->m_bUse = true;		// 描画する合図
	this->m_col = WhiteColor;	// 色の初期化
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// エフェクトポリゴンのの生成
//=============================================================================
CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// クラスのポインタ変数
	CEffect *pEffect = new CEffect;

	// NULLチェック
	if (pEffect != NULL)
	{
		pEffect->m_pos = pos;		// 位置
		pEffect->m_size = size;		// サイズ
		pEffect->m_bMove = false;	// 移動するかしないか
		pEffect->Init();			// 初期化
	}

	return pEffect;
}

//=============================================================================
// パーティクル用のエフェクト生成
//=============================================================================
CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXVECTOR3 move, LPDIRECT3DTEXTURE9 ptex)
{
	// クラスのポインタ変数
	CEffect *pEffect = new CEffect;

	// NULLチェック
	if (pEffect != NULL)
	{
		pEffect->m_pos = pos;		// 位置
		pEffect->m_size = size;		// サイズ
		pEffect->m_move = move;		// 移動量
		pEffect->SetTexture(ptex);	// テクスチャ
		pEffect->m_bMove = true;	// 移動の合図
		pEffect->Init();			// 初期化
	}

	return pEffect;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CEffect::Init(void)
{
	// ポリゴンの初期化処理
	CScene2D::Init(m_pos, m_size);

	// 位置の設定
	CScene2D::SetPos(m_pos);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	// 終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	// 位置の更新
	CScene2D::SetPos(m_pos);

	// 色の更新
	CScene2D::SetCol(m_col);

	// 描画するかの有無の更新
	CScene2D::SetUse(m_bUse);

	// もし移動するなら
	if (m_bMove)
	{
		// 移動量の加算
		m_pos += m_move;

		// 移動量の減衰
		m_move.x += (0 - m_move.x) * 0.025f;
		m_move.y += (0 - m_move.y) * 0.025f;

		// 透明度の加算
		m_col.a -= 0.055f;

		// 地面との当たり判定
		CollisionField();

		// 透明度がゼロを超えたら
		if (m_col.a < 0)
		{
			// ポリゴンの開放
			CScene2D::Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// 加算合成
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CScene2D::Draw();

	// 通常合成に戻す
	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 地面との当たり判定
//=============================================================================
void CEffect::CollisionField(void)
{
	// Meshのポインタ
	CMesh* pMesh = NULL;

	// メッシュポリゴンの情報取得
	switch (CManager::GetMode())
	{
		// チュートリアル
	case CManager::MODE_TUTORIAL:
		pMesh = CTutorial::GetMesh();
		break;

		// ゲーム
	case CManager::MODE_GAME:
		pMesh = CGame::GetMesh();
		break;

	default:
		break;
	}

	// 頂点情報の取得
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// 底辺の中心座標設定
	D3DXVECTOR3 posA = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);

	// 外積当たり判定
	bool bOutPro = false;

	// メッシュポリゴン上辺のみ算出
	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{
		// 二つの頂点と点の外積判定
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{
			// 判定結果を代入
			bOutPro = Collision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);
		}

		// 点が二点より下にいたら
		if (bOutPro == true)
		{
			// 位置を戻す
			m_pos.y = pVtx[nCnt].pos.y - m_size.y;

			break;
		}
	}
}

