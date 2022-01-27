//-----------------------------------------------------------------
//
// チュートリアル画面 (tutorial.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "tutorial.h"
#include "fade.h"
#include "input_keyboard.h"
#include "sound.h"
#include "manager.h"
#include "renderer.h"
#include "Scene2D.h"
#include "input_joypad.h"
#include "texture.h"

//=============================================================================
// CTutorialのコンストラクタ
//=============================================================================
CTutorial::CTutorial(int nPriority) :CScene(nPriority)
{

}

//=============================================================================
// CTutorialのデストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTutorial *CTutorial::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CTutorial *pTutorial;
	pTutorial = new CTutorial(CScene::PRIORITY_PAUSE);

	// NULLチェック
	if (pTutorial != NULL)
	{
		// 初期化処理
		pTutorial->Init(pos, size);

		// オブジェクトタイプの設定
		pTutorial->SetObjType(OBJTYPE_TUTORIAL);
	}

	return pTutorial;
}

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// メンバ変数の初期化
	m_nCntTutorial = 0;
	m_nCntLogo = 0;

	// チュートリアル情報の読み込み
	LoadData();

	// チュートリアル画面の後ろを薄くする
	m_pBg = new CScene2D(CScene::PRIORITY_NONE);
	m_pBg->Init(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR2(1280.0f, 720.0f));
	m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f));

	CManager::SetPause(true);
	m_nCursor = 0;

	return S_OK;
}

//=============================================================================
// チュートリアルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// メモリの開放
	delete m_pTutorial;
	m_pTutorial = NULL;

	delete m_pLogo;
	m_pLogo = NULL;

	// 矢印の終了処理
	if (m_pLeftArrow != NULL)
	{
		m_pLeftArrow->Uninit();
	}
	if (m_pRightArrow != NULL)
	{
		m_pRightArrow->Uninit();
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// チュートリアルの更新処理
//=============================================================================
void CTutorial::Update(void)
{
	// キーボード情報の取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// ゲームパッド情報の取得
	CInputJoypad *pJoypad = CManager::GetInputJoypad();
	bool *bCrossTrigger = pJoypad->GetCrossKeyTrigger(0);

	// フェード情報の取得
	CFade *pFade = CManager::GetFade();

	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_LEFT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_LEFT])
	{
		// 上にいくとき番号を減らす
		m_nCursor--;

		if (m_nCursor < 0)
		{
			// コンティニューより小さくなったとき
			m_nCursor = 0;
		}
		else
		{
			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_TURN_PAGE);
		}
	}
	else if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_RIGHT) || bCrossTrigger[CInputJoypad::STICK_AND_CROSSKEY_RIGHT])
	{
		// 下にいくとき番号を増やす
		m_nCursor++;

		if (m_nCursor > m_nNumTutorial - 1)
		{
			// クイットより大きくなったとき
			m_nCursor = m_nNumTutorial - 1;
		}
		else
		{
			// 効果音
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_TURN_PAGE);
		}
	}

	// カーソルが最後かつエンターキーを押したときチュートリアル画面を閉じる
	if (pKeyboard->GetTrigger(CInputKeyboard::KEYINFO_OK) || pJoypad->GetButtonTrigger(0, JOYPAD_CANCEL))
	{
		CManager::SetPause(false);

		// 終了処理
		Uninit();

		return;
	}
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	// チュートリアルの描画処理
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}
	if (m_pTutorial[m_nCursor] != NULL)
	{
		m_pTutorial[m_nCursor]->Draw();
	}
	for (int nCnt = 0; nCnt < m_nNumLogo; nCnt++)
	{
		if (m_pLogo[nCnt] != NULL)
		{
			m_pLogo[nCnt]->Draw();
		}
	}

	// 矢印の描画処理
	if (m_nCursor == 0)
	{
		if (m_pRightArrow != NULL)
		{
			m_pRightArrow->Draw();
		}
	}
	else if (m_nCursor == m_nNumTutorial - 1)
	{
		if (m_pLeftArrow != NULL)
		{
			m_pLeftArrow->Draw();
		}
	}
	else
	{
		if (m_pLeftArrow != NULL && m_pRightArrow != NULL)
		{
			m_pRightArrow->Draw();
			m_pLeftArrow->Draw();
		}
	}
}

//=============================================================================
// ポーズデータのロード
//=============================================================================
bool CTutorial::LoadData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/03_tutorial/tutorial.txt", "r");

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
				// LOGOSETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
				}
				// LEFTARROWSETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "LEFTARROWSET") == 0)
				{
					LoadLeftArrow(pFile, nNumTex, pTextureFileName);
				}
				// RIGHTARROWSETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "RIGHTARROWSET") == 0)
				{
					LoadRightArrow(pFile, nNumTex, pTextureFileName);
				}
				// PAUSEMENUSETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "TUTORIALSET") == 0)
				{
					LoadTutorialSheet(pFile, nNumTex, pTextureFileName);
				}
				// END_SCRIPTを見つけたら読み込みを終える
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// メモリの開放
					delete[] pTextureFileName;

					break;
				}
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		return false;
	}
	return true;
}

//=============================================================================
// ロゴのロード
//=============================================================================
bool CTutorial::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "NUM_LOGO") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumLogo);

			// メモリの確保
			m_pLogo = new CScene2D*[m_nNumLogo];
		}
		// LOGOのときスキルゲージ情報を格納する許可を与える
		if (strcmp(&aText[0], "LOGO") == 0)
		{
			Logo(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// ロゴ情報のロード
//=============================================================================
bool CTutorial::Logo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

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
		// END_LOGOSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_LOGO") == 0)
		{
			if (nTex < nNumTex && m_nCntLogo < m_nNumLogo)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));
					m_nCntLogo++;

					return true;
				}
				else
				{
					m_pLogo[m_nCntLogo] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);
					m_nCntLogo++;

					return true;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// 左矢印情報のロード
//=============================================================================
bool CTutorial::LoadLeftArrow(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

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
		// END_LEFTARROWSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_LEFTARROWSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pLeftArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));

					return true;
				}
				else
				{
					m_pLeftArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);

					return true;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// 右矢印情報のロード
//=============================================================================
bool CTutorial::LoadRightArrow(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

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
		// END_RIGHTARROWSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_RIGHTARROWSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pRightArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));

					return true;
				}
				else
				{
					m_pRightArrow = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);

					return true;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// チュートリアルのロード
//=============================================================================
bool CTutorial::LoadTutorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "NUM_TUTORIAL") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumTutorial);

			// メモリの確保
			m_pTutorial = new CScene2D*[m_nNumTutorial];
		}
		// SKILLGAUGESETのときスキルゲージ情報を格納する許可を与える
		if (strcmp(&aText[0], "TUTORIALSHEET") == 0)
		{
			TuotorialSheet(pFile, nNumTex, pTextureFileName);
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_TUTORIALSET") == 0)
		{
			return true;
		}
	}
	return false;
}

//=============================================================================
// ロゴ情報のロード
//=============================================================================
bool CTutorial::TuotorialSheet(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };

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
		// END_LOGOSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_TUTORIALSHEET") == 0)
		{
			if (nTex < nNumTex && m_nCntTutorial < m_nNumTutorial)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					m_pTutorial[m_nCntTutorial] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));
					m_nCntTutorial++;

					return true;
				}
				else
				{
					m_pTutorial[m_nCntTutorial] = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, -1);
					m_nCntTutorial++;

					return true;
				}
			}
		}
	}
	return false;
}