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

//-----------------------------------------------------------------------------
// 静的変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CEffect::m_pTexture;

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect() : CScene2D(OBJ_EFFECT)
{
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
		pEffect->m_bUse = true;
		pEffect->Init();
	}

	return pEffect;
}


//=============================================================================
// アンロード
//=============================================================================
void CEffect::Unload()
{
	if (m_pTexture != NULL)
	{// 頂点バッファの開放
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ロード
//=============================================================================
HRESULT CEffect::Load(const LPCSTR pSrcFile)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &m_pTexture);

	return S_OK;
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
	Unload();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	//m_pos = CScene2D::GetPos();

	//if (m_nLife <= 0)
	//{
	//	Uninit();
	//}

	CScene2D::SetPos(m_pos);
	CScene2D::SetSize(m_size);
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	if (m_bUse == true)
	{
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		CScene2D::Draw();

		D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 合成方法
		D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ソース（描画元）の合成方法の設定
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// デスティネーション（描画先）の合成方法の設定/画像の透明度が反映
	}

	//// 減産合成の設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//=============================================================================
// 色の設定
//=============================================================================
void CEffect::SetColor(D3DXCOLOR col)
{
	m_col = col;
	CScene2D::SetCol(m_col);
}

//=============================================================================
// 移動量の設定
//=============================================================================
void CEffect::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// ライフの設定
//=============================================================================
void CEffect::SetLife(int nLife)
{
	m_nLife = nLife;
}

void CEffect::SetUse(bool bUse)
{
	m_bUse = bUse;
}

void CEffect::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}
