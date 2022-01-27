//=============================================================================
//
// ポーズ (pause.cpp)
// Author:Itsuki Takaiwa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "mode_logo.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
#include "input_joypad.h"
#include "texture.h"
#include "base.h"
#include "mode_select.h"
#include "tutorial.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// CModeLogoのコンストラクタ
//=============================================================================
CModeLogo::CModeLogo(int nPriority) :CScene(nPriority)
{

}

//=============================================================================
// CModeLogoのデストラクタ
//=============================================================================
CModeLogo::~CModeLogo()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CModeLogo *CModeLogo::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CModeLogo *pModeLogo;
	pModeLogo = new CModeLogo(CScene::PRIORITY_UI);

	// NULLチェック
	if (pModeLogo != NULL)
	{
		// 初期化処理
		pModeLogo->Init(pos, size);

		// オブジェクトタイプの設定
		pModeLogo->SetObjType(OBJTYPE_UI);
	}

	return pModeLogo;
}

//=============================================================================
// ポーズの初期化処理
//=============================================================================
HRESULT CModeLogo::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_nCntLogo = 0;
	m_nNumLogo = 0;
	m_nNumMode = 0;
	m_nNumSelect = 0;

	// ロゴ情報の読み込み
	LoadModeLogo();

	return S_OK;
}

//=============================================================================
// ポーズの終了処理
//=============================================================================
void CModeLogo::Uninit(void)
{
	// メモリの開放
	delete[] m_pLogo;
	m_pLogo = NULL;

	delete[] m_nNumMode;
	m_nNumMode = NULL;

	// オブジェクト破棄
	Release();
}

//=============================================================================
// ポーズの更新処理
//=============================================================================
void CModeLogo::Update(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *bCrossTrigger = pJoypad->GetCrossKeyTrigger(0);

	// フェード情報の取得
	CFade *pFade = CManager::GetFade();

	// 各UIの色を設定
	for (int nCnt = 0; nCnt < m_nNumLogo; nCnt++)
	{
		if (m_nNumSelect == nCnt)
		{
			// 現在選択されているものはα値1.0fに設定
			m_pLogo[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			// それ以外はα値0.3fに設定
			m_pLogo[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
		}
	}

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_LEFT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_LEFT])
		{
			// 上にいくとき番号を減らす
			m_nNumSelect--;

			if (m_nNumSelect < 0)
			{
				// コンティニューより小さくなったとき
				m_nNumSelect = m_nNumLogo - 1;
			}
		}
		else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_RIGHT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_RIGHT])
		{
			// 下にいくとき番号を増やす
			m_nNumSelect++;

			if (m_nNumSelect > m_nNumLogo - 1)
			{
				// クイットより大きくなったとき
				m_nNumSelect = 0;
			}
		}
		if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_DECISION))
		{
			if (m_nNumMode[m_nNumSelect] == (int)CModeSelect::MODESELECT_TRANSITION_GAME_TUTORIAL)
			{
				CTutorial *pTutorial = CTutorial::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });
			}
			else
			{
				// ベースの取得
				CBase *pBase = CManager::GetBase();

				if (pBase != NULL)
				{
					// モード選択にキャスト
					CModeSelect *pModeSelect = (CModeSelect*)pBase;

					pModeSelect->SetMode((CModeSelect::MODESELECT)m_nNumMode[m_nNumSelect]);
				}
			}
		}
	}
}

//=============================================================================
// ポーズの描画処理
//=============================================================================
void CModeLogo::Draw(void)
{
	// 背景の描画
	for (int nCnt = 0; nCnt < m_nCntLogo; nCnt++)
	{
		m_pLogo[nCnt]->Draw();
	}
}

//=============================================================================
// ロゴ情報のロード
//=============================================================================
bool CModeLogo::LoadModeLogo(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/01_mode_select/logo.txt", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// 変数宣言
		char aLine[128];
		char aText[128];
		bool bScript = false;
		int nCntTex = 0;
		int nNumTex = 0;
		char **pTextureFileName;

		// SCRIPTを探す
		while (fgets(aLine, 128, pFile) != NULL)
		{
			// 毎行空白を見つけるまで読み込む
			fscanf(pFile, "%s", &aText[0]);

			// SCRIPTを見つけたらそれ以降を読み込む許可を与える
			if (strcmp(&aText[0], "SCRIPT") == 0)
			{
				bScript = true;
			}
			if (bScript == true)
			{
				if (strcmp(&aText[0], "NUM_TEXTURE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumTex);

					// モデルファイルネーム格納用変数の領域を動的に確保する
					pTextureFileName = new char*[nNumTex];

					for (int nCnt = 0; nCnt < nNumTex; nCnt++)
					{
						pTextureFileName[nCnt] = new char[128];
					}
				}
				// TEXTURE_FILENAMEを見つけたらテクスチャファイル名を格納
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntTex < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &pTextureFileName[nCntTex][0]);

					nCntTex++;
				}

				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "NUM_LOGO") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumLogo);

					// メモリの確保
					m_pLogo = new CScene2D*[m_nNumLogo];
					m_nNumMode = new int[m_nNumLogo];
				}
				// END_PARTSSETを見つけたら各パーツの情報を格納を終える
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					ModeLogo(pFile, nNumTex, pTextureFileName);
				}
				// END_SCRIPTを見つけたら読み込みを終える
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// メモリの開放
					delete[] pTextureFileName;
					pTextureFileName = NULL;

					break;
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);

		return true;
	}
	return false;
}

//=============================================================================
// ロゴ情報のロード
//=============================================================================
bool CModeLogo::ModeLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };
	int nNumTra = -1;	// 遷移先

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTex);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "POS") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&pos.x,
				&pos.y,
				&pos.z);
		}
		// SIZEを見つけたらサイズ情報を格納
		if (strcmp(&aText[0], "SIZE") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&size.x,
				&size.y);
		}
		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "COLOR") == 0)
		{
			fscanf(pFile, "%s %f %f %f %f", &aText[0],
				&col.r,
				&col.g,
				&col.b,
				&col.a);
		}
		// MODEを見つけたら座標情報を格納
		if (strcmp(&aText[0], "MODE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumMode[m_nCntLogo]);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL && m_nCntLogo < m_nNumLogo)
				{
					// スキルゲージの生成
					m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
					m_pLogo[m_nCntLogo]->SetCol(col);
					m_nCntLogo++;
					return true;
				}
			}
		}
	}
	return false;
}