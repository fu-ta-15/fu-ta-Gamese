//-----------------------------------------------------------------
//
// データ読み込み (set_game.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#include "set_game.h"

#include "set_model.h"
#include "floor.h"
#include "skillgauge.h"
#include "scoregauge.h"
#include "graffiti_gauge.h"
#include "gauge_manager.h"
#include "Scene2D.h"
#include "timer.h"

#include "manager.h"
#include "base.h"
#include "game.h"
#include "player_game.h"
#include "texture.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// CSetGameのコンストラクタ
//-----------------------------------------------------------------
CSetGame::CSetGame()
{

}

//-----------------------------------------------------------------
// CSetGameのデストラクタ
//-----------------------------------------------------------------
CSetGame::~CSetGame()
{

}

//-----------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------
void CSetGame::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
	m_nNumTex = -1;
	m_nCntSkillGauge = 0;
	m_nCntScoreGauge = 0;

	CSetModel *pSetModel = new CSetModel;

	// モデル配置情報の読み込み
	pSetModel->LoadData();

	// 床情報の読み込み
	LoadFloor();

	// ゲージ情報の読み込み
	LoadUIInfo();
}

//-----------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------
void CSetGame::Update(void)
{

}

//-----------------------------------------------------------------
// 床情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadFloor(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/04_game/floor.txt", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// 変数宣言
		char aLine[128];
		char aText[128];
		bool bScript = false;
		bool bSetFloor = false;
		int nCntModel = 0;
		int nNumTexType = -1;
		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
		D3DXVECTOR2 size = { 0.0f,0.0f };
		D3DXVECTOR2 block = { 0.0f,0.0f };
		int nNumTex = 0;
		char **apTextureFileName;

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
					apTextureFileName = new char*[nNumTex];

					for (int nCnt = 0; nCnt < nNumTex; nCnt++)
					{
						apTextureFileName[nCnt] = new char[128];
					}
				}
				// MODEL_FILENAMEを見つけたらXファイル名を格納
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntModel < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &apTextureFileName[nCntModel][0]);

					nCntModel++;
				}
				// PARTSSETを&aText[0]各パーツの情報を格納する許可を与える
				if (strcmp(&aText[0], "FLOORSET") == 0)
				{
					bSetFloor = true;
				}
				// 各パーツの情報を格納する許可があるとき処理
				if (bSetFloor == true)
				{
					// POSを見つけたら座標情報を格納
					if (strcmp(&aText[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s %d", &aText[0], &nNumTexType);
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
					// BLOCKを見つけたらメッシュのブロック数情報を格納
					if (strcmp(&aText[0], "BLOCK") == 0)
					{
						fscanf(pFile, "%s %f %f", &aText[0],
							&block.x,
							&block.y);
					}
					// END_PARTSSETを見つけたら各パーツの情報を格納を終える
					if (strcmp(&aText[0], "END_FLOORSET") == 0)
					{
						bSetFloor = false;

						if (nNumTexType < nNumTex)
						{
							// テクスチャ情報の取得
							CTexture *pTex = CManager::GetTexture();

							if (pTex != NULL)
							{
								// モデルの生成
								CFloor::Create(pos, size, block, pTex->SetTexture(&apTextureFileName[nNumTexType][0]));
							}
						}
					}
					// END_SCRIPTを見つけたら読み込みを終える
					if (strcmp(&aText[0], "END_SCRIPT") == 0)
					{
						// メモリの開放
						delete[] apTextureFileName;
						apTextureFileName = NULL;

						break;
					}
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
// UI情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadUIInfo(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/04_game/ui.txt", "r");

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
				// SKILLGAUGESETのときスキルゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "SKILLGAUGESET") == 0)
				{
					LoadSkillGauge(pFile, nNumTex, pTextureFileName);
				}
				// SCOREGAUGESETのときスコアゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "SCOREGAUGESET") == 0)
				{
					LoadScoreGauge(pFile, nNumTex, pTextureFileName);
				}
				// GRAFFITIGAUGESETのときラクガキゲージ情報を格納する許可を与える
				if (strcmp(&aText[0], "GRAFFITIGAUGESET") == 0)
				{
					LoadGraffitiGauge(pFile, nNumTex, pTextureFileName);
				}
				// PLAYERUISETのときプレイヤーUI情報を格納する許可を与える
				if (strcmp(&aText[0], "PLAYERUISET") == 0)
				{
					LoadPlayerUI(pFile, nNumTex, pTextureFileName);
				}
				// ERASERSTATEUISETのとき消しゴム状態UI情報を格納する許可を与える
				if (strcmp(&aText[0], "ERASERSTATEUISET") == 0)
				{
					LoadEraserStateUI(pFile, nNumTex, pTextureFileName);
				}
				// TIMERSETのときタイマー情報を格納する許可を与える
				if (strcmp(&aText[0], "TIMERSET") == 0)
				{
					LoadTimer(pFile, nNumTex, pTextureFileName);
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

//-----------------------------------------------------------------
// スキルゲージ情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadSkillGauge(FILE *pFile,int nNumTex,char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "GAUGE") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// 毎行空白を見つけるまで読み込む
				fscanf(pFile, "%s", &aText[0]);

				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
				// BGSETを見つけたら背景情報を格納
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// END_PARTSSETを見つけたら各パーツの情報を格納を終える
				if (strcmp(&aText[0], "END_GAUGE") == 0)
				{
					if (m_nNumTex < nNumTex)
					{
						// テクスチャ情報の取得
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL)
						{
							// プレイヤー数を取得
							CBase *pBase = CManager::GetBase();

							if (pBase != NULL)
							{
								CGame *pGame = (CGame*)pBase;
								int nNumPlayer = pGame->GetNumPlayer();

								if (nNumPlayer > m_nCntSkillGauge)
								{
									CPlayer_Game **pPlayer = pGame->GetPlayer();
									
									// スキルゲージの生成
									CSkillGauge *pGauge = CSkillGauge::Create(pos, size, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
									pGauge->SetCol(col);
									pPlayer[m_nCntSkillGauge]->SetSkillGauge(pGauge);

									m_nCntSkillGauge++;
								}
							}

							break;
						}
					}
				}
			}
		}
		// FRAMEを見つけたら枠情報を格納
		if (strcmp(&aText[0], "FRAME") == 0)
		{
			LoadGaugeFrame(pFile, nNumTex, pTextureFileName);

			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// スコアゲージ情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadScoreGauge(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "GAUGE") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// 毎行空白を見つけるまで読み込む
				fscanf(pFile, "%s", &aText[0]);

				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
				// BGSETを見つけたら背景情報を格納
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// END_PARTSSETを見つけたら各パーツの情報を格納を終える
				if (strcmp(&aText[0], "END_GAUGE") == 0)
				{
					if (m_nNumTex < nNumTex)
					{
						// テクスチャ情報の取得
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL)
						{
							// プレイヤー数を取得
							CBase *pBase = CManager::GetBase();

							if (pBase != NULL)
							{
								CGame *pGame = (CGame*)pBase;
								int nNumPlayer = pGame->GetNumPlayer();

								if (nNumPlayer > m_nCntScoreGauge)
								{
									CPlayer_Game **pPlayer = pGame->GetPlayer();

									// スキルゲージの生成
									CScoreGauge *pGauge = CScoreGauge::Create(pos, size, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
									pGauge->SetCol(col);
									pPlayer[m_nCntScoreGauge]->SetScoreGauge(pGauge);

									m_nCntScoreGauge++;
								}
							}
							break;
						}
					}
				}
			}
		}
		// FRAMEを見つけたら枠情報を格納
		if (strcmp(&aText[0], "FRAME") == 0)
		{
			LoadGaugeFrame(pFile, nNumTex, pTextureFileName);
			
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// ラクガキゲージ情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadGraffitiGauge(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "GAUGE") == 0)
		{
			while (fgets(aLine, 128, pFile) != NULL)
			{
				// 毎行空白を見つけるまで読み込む
				fscanf(pFile, "%s", &aText[0]);

				// POSを見つけたら座標情報を格納
				if (strcmp(&aText[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
				// BGSETを見つけたら背景情報を格納
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// END_PARTSSETを見つけたら各パーツの情報を格納を終える
				if (strcmp(&aText[0], "END_GAUGE") == 0)
				{
					if (m_nNumTex < nNumTex)
					{
						// テクスチャ情報の取得
						CTexture *pTex = CManager::GetTexture();

						if (pTex != NULL)
						{
							// スキルゲージの生成
							CGraffitiGauge *pGauge = CGraffitiGauge::Create(pos, size, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
							pGauge->SetCol(col);

							CBase *pBase = CManager::GetBase();

							if (pBase != NULL)
							{
								CGame *pGame = (CGame*)pBase;
								pGame->GetGaugeManager()->SetGraffitiGauge(pGauge);
							}

							break;
						}
					}
				}
			}
		}
		// FRAMEを見つけたら枠情報を格納
		if (strcmp(&aText[0], "FRAME") == 0)
		{
			LoadGaugeFrame(pFile, nNumTex, pTextureFileName);

			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// ゲージの枠情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadGaugeFrame(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &m_nNumTex);
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
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_FRAME") == 0)
		{
			if (m_nNumTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// テクスチャの生成
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[m_nNumTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// プレイヤーUI情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadPlayerUI(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_PLAYERUISETを見つけたらプレイヤーUIの情報を格納を終える
		if (strcmp(&aText[0], "END_PLAYERUISET") == 0)
		{
			if (m_nNumTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// テクスチャの生成
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// 消しゴム状態UI情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadEraserStateUI(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_ERASERSTATEUISETを見つけたら消しゴム状態UIの情報を格納を終える
		if (strcmp(&aText[0], "END_ERASERSTATEUISET") == 0)
		{
			if (m_nNumTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// テクスチャの生成
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// タイマー情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadTimer(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	int nDigit;
	float fSpace;
	int nTime;

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
			fscanf(pFile, "%s %d", &aText[0],&nDigit);
		}
		// SPACEを見つけたら間隔情報を格納
		if (strcmp(&aText[0], "SPACE") == 0)
		{
			fscanf(pFile, "%s %f", &aText[0], &fSpace);
		}
		// TIMEを見つけたら制限時間情報を格納
		if (strcmp(&aText[0], "TIME") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nTime);
		}
		// BGSETを見つけたら背景情報を格納
		if (strcmp(&aText[0], "BGSET") == 0)
		{
			LoadBg(pFile, nNumTex, pTextureFileName);
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
					// カウントダウン情報を取得
					CBase *pBase = CManager::GetBase();

					if (pBase != NULL)
					{
						CGame *pGame = (CGame*)pBase;

						// タイマーの生成
						pGame->SetTimer(CTimer::Create(pos, size, nDigit, fSpace, nTime, pTex->SetTexture(&pTextureFileName[nTex][0])));
					}
					else
					{
						// タイマーの生成
						CTimer::Create(pos, size, nDigit, fSpace, nTime, pTex->SetTexture(&pTextureFileName[nTex][0]));
					}
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// 各種背景情報の読み込み
//-----------------------------------------------------------------
bool CSetGame::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		// END_BGSETを見つけたら背景情報を格納を終える
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// テクスチャの生成
					CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}