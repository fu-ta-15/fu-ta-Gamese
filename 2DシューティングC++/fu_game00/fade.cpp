//*****************************************************************************
//
// フェード処理 [fade.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "fade.h"
#include "scene2D.h"
#include "renderer.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define FADE_SIZE	(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))
#define FADE_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f))
#define FADE_SPEED	(0.025f)

CFade::FADE CFade::m_FadeMode;

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_FadeMode = FADE_OUT;
	m_modeNext = CManager::MODE_TITLE;
	m_pVtxBuff = NULL;
	m_pVtx = NULL;
	m_pos = FADE_POS;
	m_size = FADE_SIZE;
	m_col = WhiteColor;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// フェードの生成
//=============================================================================
CFade * CFade::Create(void)
{
	// フェードのポインタ
	CFade* pFade = new CFade;

	// NULLチェック
	if (pFade != NULL)
	{
		// 初期化処理
		pFade->Init();
	}

	return pFade;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CFade::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	m_col = WhiteColor;
	m_pos = FADE_POS;
	m_size = FADE_SIZE;
	m_FadeMode = FADE_OUT;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 各頂点の座標
	m_pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y + m_size.y, 0.0f);
	m_pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_size.x, m_pos.y - m_size.y, 0.0f);
	m_pVtx[2].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y, 0.0f);
	m_pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y, 0.0f);

	//rhwの設定		=	1.0f固定
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;

	//頂点カラーの設定・赤・緑・青
	m_pVtx[0].col = m_col;
	m_pVtx[1].col = m_col;
	m_pVtx[2].col = m_col;
	m_pVtx[3].col = m_col;

	//テクスチャ座標
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	//頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	if (m_FadeMode != FADE_NONE)
	{// フェードの状態がNONEじゃない場合
		if (m_FadeMode == FADE_IN)
		{// フェードイン

			//α値を減らしていく
			m_col.a -= FADE_SPEED;

			// 色を反映させる
			SetCol(m_col);

			if (m_col.a <= 0.0f)
			{// A値が０以下の場合
				m_FadeMode = FADE_NONE;		//何もしていない状態にする
			}
		}
		else if (m_FadeMode == FADE_OUT)
		{// フェードアウト

			//α値を増やしていく
			m_col.a += FADE_SPEED;

			// 色を反映させる
			SetCol(m_col);

			if (m_col.a >= 1.0f)
			{// A値が１以上の場合
				CManager::SetMode(m_modeNext);	//次のモードへ
				m_FadeMode = FADE_IN;		//フェードインに切り替え
			}
		}
	}
	else if (m_FadeMode == FADE_NONE)
	{
		//透明にする
		m_col.a = 0.0f;

		// 色を反映させる
		SetCol(m_col);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);		// 0//描画を開始する頂点インデックス
}

//=============================================================================
// フェードモードの設定
//=============================================================================
void CFade::SetFade(CManager::MODE mode)
{
	m_FadeMode = FADE_OUT;	// フェード状態
	m_modeNext = mode;		// 次のモードへ
}

//=============================================================================
// 色の設定
//=============================================================================
void CFade::SetCol(D3DXCOLOR col)
{
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//頂点カラーの設定・赤・緑・青
	m_pVtx[0].col = col;
	m_pVtx[1].col = col;
	m_pVtx[2].col = col;
	m_pVtx[3].col = col;

	//バッファのアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// フェードの情報取得
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_FadeMode;
}
