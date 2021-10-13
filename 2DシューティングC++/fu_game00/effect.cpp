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

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect() : CScene2D(OBJ_EFFECT)
{
	this->m_bUse = true;
	this->m_col = WhiteColor;
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
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{
		pEffect = new CEffect;
		pEffect->m_pos = pos;
		pEffect->m_size = size;
		pEffect->m_bMove = false;
		pEffect->m_bGravity = false;
		pEffect->Init();
	}

	return pEffect;
}

CEffect * CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{
		pEffect = new CEffect;
		pEffect->m_pos = pos;
		pEffect->m_size = size;
		pEffect->m_move = move;
		pEffect->m_bMove = true;
		pEffect->m_bGravity = true;
		pEffect->Init();
	}

	return pEffect;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CEffect::Init(void)
{
	CScene2D::Init(m_pos, m_size);
	CScene2D::SetPos(m_pos);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	if (m_bMove)
	{
		if (m_bGravity == true)
		{
			m_move.y += 0.45f;
		}

		m_pos += m_move;

		m_move.x += (0 - m_move.x)*0.025f;

		m_col.a -= 0.035f;

		if (m_col.a < 0)
		{
			Release();
		}
		CollisionField();
	}

	CScene2D::SetPos(m_pos);
	CScene2D::SetSize(m_size);
	CScene2D::SetCol(m_col);
	CScene2D::SetUse(m_bUse);
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 合成方法
	D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソース（描画元）の合成方法の設定
	D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// デスティネーション（描画先）の合成方法の設定/画像の透明度が反映
}

void CEffect::CollisionField(void)
{
	CMesh* pMesh = NULL;
	pMesh = MESH_GAME;

	// 頂点情報の取得
	VERTEX_2D *pVtx = pMesh->GetVERTEX();

	// 底辺の中心座標設定
	D3DXVECTOR3 posA = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y, 0.0f);

	// 外積当たり判定
	bool bOutPro = false;

	for (int nCnt = 0; nCnt < pMesh->GetVtxNum() / 2; nCnt++)
	{// メッシュポリゴン上辺のみ算出
		if (m_pos.x > pVtx[nCnt].pos.x && m_pos.x < pVtx[nCnt + 1].pos.x)
		{// 二つの頂点と点の外積判定
			bOutPro = CCollision::OutProduct(pVtx[nCnt].pos, pVtx[nCnt + 1].pos, posA);
		}
		if (bOutPro == true)
		{// 点が二点より下にいたら

			m_pos.y = pVtx[nCnt].pos.y - m_size.y;

			break;
		}
	}

}

