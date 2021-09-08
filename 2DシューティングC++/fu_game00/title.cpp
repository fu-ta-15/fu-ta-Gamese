//*****************************************************************************
//
// タイトル処理 [title.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "title.h"
#include "scene2D.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"
#include "player.h"
#include "bullet.h"
#include "mesh.h"
#include "move.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define TITLE_BG_SIZE		(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF, 0.0f))

#define TITLE_BUTTON_POS	(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF+250.0f, 0.0f))
#define TITLE_BUTTON_SIZE	(D3DXVECTOR3(150.0f, 50.0f, 0.0f))

#define TITLE_LOGO_POS		(D3DXVECTOR3(WIDTH_HALF-300, HEIGHT_HALF-500.0f, 0.0f))
#define TITLE_LOGO_SIZE		(D3DXVECTOR3(600.0f, 400.0f, 0.0f))


//-----------------------------------------------------------------------------
// 静的メンバ変数
//-----------------------------------------------------------------------------
CScene2D *CTitle::m_paTitleUI[UI_MAX] = {};
CMesh *CTitle::m_pTitleLogo = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle() : CScene(OBJ_NONE)
{
	m_ButtonCol = WhiteColor;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// タイトルの生成
//=============================================================================
CTitle * CTitle::Create(void)
{
	// メモリ確保
	CTitle* pTitle = new CTitle;

	// 初期化処理
	pTitle->Init();

	return pTitle;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTitle::Init(void)
{
	m_paTitleUI[UI_BG] = CScene2D::Create(CENTER_POS, TITLE_BG_SIZE);
	m_paTitleUI[UI_BUTTON] = CScene2D::Create(TITLE_BUTTON_POS, TITLE_BUTTON_SIZE);
	m_pTitleLogo = CMesh::Create(90, 0, TITLE_LOGO_POS, TITLE_LOGO_SIZE);

	m_paTitleUI[UI_BG]->CreateTexture("data/TEXTURE/BG.jpg");
	m_paTitleUI[UI_BUTTON]->CreateTexture("data/TEXTURE/Next_Mode.png");
	m_pTitleLogo->CreateTexture("data/TEXTURE/TitleUI.png");
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	CKey *pKey = CManager::GetKey();

	m_nCntTime++;
	char *cC = NULL;

	m_pTitleLogo->WaveMove(50.0f, 10.0f, 100.0f, m_nCntTime);

	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE))
	{
		CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
	}
	ButtonUI();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// Buttonの処理
//=============================================================================
void CTitle::ButtonUI(void)
{
	m_paTitleUI[UI_BUTTON]->SetCol(m_ButtonCol);// α値の更新

	m_ButtonCol.a += AddCol; // α値の減算と加算

	if (m_ButtonCol.a >= 1.0f)
	{// 減算させる値
		AddCol = -0.02f;
	}
	if (m_ButtonCol.a <= 0.0f)
	{// 加算させる値
		AddCol = 0.02f;
	}
}
