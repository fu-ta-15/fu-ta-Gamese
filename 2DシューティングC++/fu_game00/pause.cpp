//*****************************************************************************
//
// ポーズ処理 [pause.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "pause.h"
#include "scene2D.h"
#include "renderer.h"
#include "keyinput.h"
#include "manager.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// マクロ変数
//-----------------------------------------------------------------------------
#define PAUSE_SIZE		(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF, 0.0f))
#define PAUSE_POS		(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF, 0.0f))
#define PAUSE_COL		(D3DXCOLOR(0.7f,0.7f,0.7f,0.6f))
#define BUTTON_COL		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define BUTTON_POS		(D3DXVECTOR3(WIDTH_HALF, HEIGHT_HALF-200, 0.0f))
#define BUTTON_SIZE		(D3DXVECTOR3(200.0f, 50.0f, 0.0f))
#define PAUSE_OBJ_NUM	(CPause::MENU_MAX)
#define MENU_SELECTCOL	(D3DXCOLOR(0.4f,0.4f,0.4f,1.0f))

//-----------------------------------------------------------------------------
// 静的変数
//-----------------------------------------------------------------------------
CScene2D *CPause::m_pScene2D = NULL;
CScene2D *CPause::m_pPauseButton[MENU_MAX] = {};
int CPause::m_nMenuID = 0;

//=============================================================================
// コンストラクタ（ポーズ用のオーバーロード）
//=============================================================================
CPause::CPause() : CScene(true)
{
	m_nMenuID = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{
}

//=============================================================================
// ポーズの生成
//=============================================================================
CPause * CPause::Create(void)
{
	CPause *pPause = NULL;	 // ポーズの変数（ポインタ）
	
	if (pPause == NULL)
	{// NULLチェック
		pPause = new CPause; // インスタンス生成
		pPause->Init();		 // 初期化
	}

	return pPause;			 // 情報を返す
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CPause::Init(void)
{
	m_col = PAUSE_COL;	 // 色
	m_pos = PAUSE_POS;	 // 位置
	m_ButtonPos = BUTTON_POS;
	m_size = PAUSE_SIZE; // サイズ

	// ポーズBGの作成
	m_pScene2D = new CScene2D(PAUSE_BG); // ポリゴンの作成
	m_pScene2D->Init(m_pos, m_size);	 // 初期化処理
	m_pScene2D->SetCol(m_col);			 // 色の設定
	m_pScene2D->SetUse(true);
	
	m_pPauseButton[0] = new CScene2D(PAUSE_BUTTON1);
	m_pPauseButton[0]->Init(m_ButtonPos, BUTTON_SIZE);
	m_pPauseButton[0]->SetCol(MENU_SELECTCOL);
	m_pPauseButton[0]->CreateTexture("data/TEXTURE/retry.png");
	m_pPauseButton[0]->SetUse(true);
	m_ButtonPos.y += 150;

	m_pPauseButton[1] = new CScene2D(PAUSE_BUTTON2);
	m_pPauseButton[1]->Init(m_ButtonPos, BUTTON_SIZE);
	m_pPauseButton[1]->SetCol(WhiteColor);
	m_pPauseButton[1]->CreateTexture("data/TEXTURE/quit.png");
	m_pPauseButton[1]->SetUse(true);
	m_ButtonPos.y += 150;

	m_pPauseButton[2] = new CScene2D(PAUSE_BUTTON3);
	m_pPauseButton[2]->Init(m_ButtonPos, BUTTON_SIZE);
	m_pPauseButton[2]->SetCol(WhiteColor);
	m_pPauseButton[2]->CreateTexture("data/TEXTURE/continue.png");
	m_pPauseButton[2]->SetUse(true);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	m_pScene2D->Uninit(true);	// 終了処理（ポーズシーンのみ消す用）

	for (int nCnt = 0; nCnt < PAUSE_OBJ_NUM; nCnt++)
	{
		m_pPauseButton[nCnt]->Uninit(true);
	}
	//オブジェクトの破棄
	PauseRelease();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	// キーボード情報取得
	CKey *pKey = CManager::GetKey();

	/* ポーズの切り替え */
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_UP) == true)			// プレス・Dが押されたとき
	{
		m_nMenuID -= 1;
		if (m_nMenuID < 0)
		{
			m_nMenuID = 2;
		}

	}
	if (pKey->GetState(CKey::STATE_TRIGGER, DIK_DOWN) == true)			// プレス・Dが押されたとき
	{
		m_nMenuID = (m_nMenuID + 1) % 3;

	}
	SelectMenu(m_nMenuID);

	if (pKey->GetState(CKey::STATE_RELEASE, DIK_SPACE))
	{
		MenuOk(m_nMenuID);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{
	m_pScene2D->Draw();	 // ポリゴンの描画
	for (int nCnt = 0; nCnt < PAUSE_OBJ_NUM; nCnt++)
	{
		m_pPauseButton[nCnt]->Draw();
	}
}

//=============================================================================
// 選択しているメニューの色を更新
//=============================================================================
void CPause::SelectMenu(const int nMenuID)
{
	for (int nCnt = 0; nCnt < PAUSE_OBJ_NUM; nCnt++)
	{
		if (nCnt == nMenuID)
		{
			m_pPauseButton[nCnt]->SetCol(MENU_SELECTCOL);
		}
		else
		{
			m_pPauseButton[nCnt]->SetCol(WhiteColor);
		}
	}
}

void CPause::MenuOk(const int nMenuID)
{
	switch (nMenuID)
	{
	case MENU_RETRY:
		CManager::GetFade()->SetFade(CManager::MODE_GAME);
		Uninit();
		break;

	case MENU_TITLE:
		CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		Uninit();
		break;

	case MENU_QUIT:
		CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		Uninit();
		break;

	default:
		break;
	}
}
