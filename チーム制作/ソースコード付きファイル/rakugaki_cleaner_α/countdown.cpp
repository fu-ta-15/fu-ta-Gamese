//=============================================================================
//
// カウントダウン処理 [countdown.cpp]
// Author : itsuki takaiwa
//
//=============================================================================
#include "countdown.h"
#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "base.h"
#include "game.h"

#include "timer.h"
#include "sound.h"

//=============================================================================
// CCountdownのコンストラクタ
//=============================================================================
CCountdown::CCountdown(int nPriority) : CScene(nPriority)
{
	// 変数の初期化
	m_apNumber = NULL;
	m_nCntInterval = 0;
	m_nTimer = 0;
	m_fSpace = 0.0f;
}

//=============================================================================
// CCountdownのデストラクタ
//=============================================================================
CCountdown::~CCountdown()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CCountdown *CCountdown::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// ポインタ変数の生成
	CCountdown *pCountdown;
	pCountdown = new CCountdown(CScene::PRIORITY_UI);

	// NULLチェック
	if (pCountdown != NULL)
	{ 
		// 初期化処理
		pCountdown->Init(pos, size);

		// オブジェクトタイプの設定
		pCountdown->SetObjType(OBJTYPE_TIMER);
	}

	return pCountdown;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCountdown::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// カウントダウン情報の読み込み
	LoadCountdown();

	// 描画更新
	UpdateDraw();

	m_bFinish = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCountdown::Uninit(void)
{
	// ナンバーの終了処理
	m_apNumber->Uninit();
	m_pStartLogo->Uninit();
	m_pBg->Uninit();

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCountdown::Update(void)
{
	if (m_bFinish != true)
	{
		// ナンバーの更新処理
		m_apNumber->Update();

		// 60になったらタイマーを減らす
		if (m_nCntInterval % 60 == 0)
		{
			m_nTimer--;
			m_nCntInterval = 0;


			if (m_nTimer < 0)
			{
				m_bFinish = true;

				// サウンド
				CSound::Play(CSound::SOUND_LABEL_GAME);
			}
			else if(m_nTimer == 0)
			{
				// 効果音
				CSound::Play(CSound::SOUND_LABEL_SE_GAME_START);
			}
			else
			{
				// 効果音
				CSound::Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
			}
		}

		// カウントの更新
		m_nCntInterval++;

		// 描画更新
		UpdateDraw();
	}

	if (m_bTimerFinish != true)
	{
		// ゲーム情報を取得
		CBase *pBase = CManager::GetBase();

		if (pBase != NULL)
		{
			// ゲーム情報を取得
			CGame *pGame = (CGame*)pBase;

			if(pGame->GetGameMode() == CGame::GAMEMODE_FINISH)
			{
				// 終わったことを伝える
				m_bTimerFinish = true;
			}
		}
	}
	else
	{
		// カウントの更新
		m_nCntInterval++;

		// 60になったら遷移する
		if (m_nCntInterval % 60 == 0)
		{
			// ゲーム情報を取得
			CBase *pBase = CManager::GetBase();

			if (pBase != NULL)
			{
				CGame *pGame = (CGame*)pBase;

				// ゲームモードをリザルト遷移に変更
				pGame->SetGameMode(CGame::GAMEMODE_TRANSITION_RESULT);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CCountdown::Draw(void)
{
	if (m_bFinish != true)
	{
		m_pBg->Draw();

		if (m_nTimer == 0)
		{
			m_pStartLogo->Draw();
		}
		else
		{
			// ナンバーの描画処理
			m_apNumber->Draw();
		}
	}
	if (m_bTimerFinish == true)
	{
		m_pBg->Draw();
		m_pFinishLogo->Draw();
	}
}

//=============================================================================
// 描画更新処理
//=============================================================================
void CCountdown::UpdateDraw(void)
{
	// タイマーを格納
	for (int nCntNumber = 0; nCntNumber < m_nDigit; nCntNumber++)
	{
		if (m_apNumber != NULL)
		{
			// 桁を一つずつ格納
			int nNumber = (m_nTimer / (int)powf(10.0f, (float)(m_nDigit - (nCntNumber + 1))));

			// ナンバーセット
			m_apNumber->SetNumber(nNumber);
		}
	}
}

//-----------------------------------------------------------------
// カウントダウン情報の読み込み
//-----------------------------------------------------------------
bool CCountdown::LoadCountdown(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/04_game/countdown.txt", "r");

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
				// TIMERSETのときタイマー情報を格納する許可を与える
				if (strcmp(&aText[0], "TIMERSET") == 0)
				{
					LoadTimer(pFile, nNumTex, pTextureFileName);
				}
				// STARTLOGOSETのとき開始ロゴ情報を格納する許可を与える
				if (strcmp(&aText[0], "STARTLOGOSET") == 0)
				{
					LoadStartLogo(pFile, nNumTex, pTextureFileName);
				}
				// FINISHLOGOSETのとき終了ロゴ情報を格納する許可を与える
				if (strcmp(&aText[0], "FINISHLOGOSET") == 0)
				{
					LoadFinishLogo(pFile, nNumTex, pTextureFileName);
				}
				// BGSETのとき背景情報を格納する許可を与える
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
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
	}
	else
	{
		return false;
	}
	return true;
}

//-----------------------------------------------------------------
// タイマー情報の読み込み
//-----------------------------------------------------------------
bool CCountdown::LoadTimer(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// DIGITを見つけたら桁数情報を格納
		if (strcmp(&aText[0], "DIGIT") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nDigit);
		}
		// SPACEを見つけたら間隔情報を格納
		if (strcmp(&aText[0], "SPACE") == 0)
		{
			fscanf(pFile, "%s %f", &aText[0], &m_fSpace);
		}
		// TIMEを見つけたら制限時間情報を格納
		if (strcmp(&aText[0], "TIME") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nTimer);
		}
		// END_TIMERSETを見つけたらタイマー情報を格納を終える
		if (strcmp(&aText[0], "END_TIMERSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// テクスチャの生成
					m_apNumber = CNumber::Create(pos, size);
					m_apNumber->SetTexNum(pTex->SetTexture(pTextureFileName[nTex]));

					m_nTimer++;
				}
			}
			return true;
		}
	}
	return false;
}

//=============================================================================
// 開始ロゴ情報のロード
//=============================================================================
bool CCountdown::LoadStartLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

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
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_STARTLOGOSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// ロゴの生成
					m_pStartLogo = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
					m_pStartLogo->SetCol(col);

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// 終了ロゴ情報のロード
//=============================================================================
bool CCountdown::LoadFinishLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

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
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_FINISHLOGOSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// ロゴの生成
					m_pFinishLogo = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(&pTextureFileName[nTex][0]));
					m_pFinishLogo->SetCol(col);

					break;
				}
			}
		}
	}
	return false;
}

//=============================================================================
// 背景情報のロード
//=============================================================================
bool CCountdown::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

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
		// END_BGSETを見つけたら背景の情報を格納を終える
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					if (nTex <= -1)
					{
						// 背景の生成
						m_pBg = CScene2D::Create(pos, D3DXVECTOR3(size.x,size.y,0.0f), CScene::PRIORITY_NONE, -1);
						m_pBg->SetCol(col);
					}
					else
					{
						// 背景の生成
						m_pBg = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_NONE, pTex->SetTexture(pTextureFileName[nTex]));
						m_pBg->SetCol(col);
					}

					break;
				}
			}
		}
	}
	return false;
}