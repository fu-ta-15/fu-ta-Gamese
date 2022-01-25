//-----------------------------------------------------------------
//
//  プレイヤー待機画面配置 (set_preparation_player.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_preparation_player.h"
#include "sceneX.h"
#include "debugproc.h"

#include "manager.h"
#include "texture.h"

#include "preparation_player.h"
#include "preparation_player_manager.h"
#include "press_logo.h"
#include "bg.h"
#include "Scene2D.h"

//-----------------------------------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// CSetPreparationPlayerのコンストラクタ
//-----------------------------------------------------------------
CSetPreparationPlayer::CSetPreparationPlayer()
{

}

//-----------------------------------------------------------------
// CSetPreparationPlayerのデストラクタ
//-----------------------------------------------------------------
CSetPreparationPlayer::~CSetPreparationPlayer()
{

}

//-----------------------------------------------------------------
// モデル配置情報の初期化
//-----------------------------------------------------------------
void CSetPreparationPlayer::Init(void)
{
	// メンバ変数の初期化
	for (int nCnt =0;nCnt < 4;nCnt++)
	{
		m_pFrame[nCnt] = NULL;
	}
	m_nCntFrame = 0;

	// 情報の読み込み
	LoadData();
}

//-----------------------------------------------------------------
// モデル配置情報の更新処理
//-----------------------------------------------------------------
void CSetPreparationPlayer::Update(void)
{

}

//---------------------------------------------------------------- -
// UI情報の読み込み
//-----------------------------------------------------------------
void CSetPreparationPlayer::LoadData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/02-1_preparation_player/preparation_player.txt", "r");

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
				// BGSETのとき背景情報を格納する
				if (strcmp(&aText[0], "BGSET") == 0)
				{
					LoadBg(pFile, nNumTex, pTextureFileName);
				}
				// PRESSLOGOSETのときプレスロゴ情報を格納する
				if (strcmp(&aText[0], "PRESSLOGOSET") == 0)
				{
					LoadPressLogo(pFile, nNumTex, pTextureFileName);
				}
				// LOGOSETのときロゴ情報を格納する
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
				}
				// FRAMESETのときフレーム情報を格納する
				if (strcmp(&aText[0], "FRAMESET") == 0)
				{
					LoadFrame(pFile, nNumTex, pTextureFileName);
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
	}
}

//-----------------------------------------------------------------
// 背景情報の読み込み
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nNum = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };
	D3DXVECTOR2 moveTex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// MOVETEXを見つけたらテクスチャ座標移動量情報を格納
		if (strcmp(&aText[0], "MOVETEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&moveTex.x,
				&moveTex.y);
		}
		// END_BGSETを見つけたら背景の情報を格納を終える
		if (strcmp(&aText[0], "END_BGSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// プレスロゴの生成
					CBg::Create(pos, size, col, moveTex, pTex->SetTexture(pTextureFileName[nNum]));
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------
// プレスロゴ情報の読み込み
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadPressLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nNum = 0;
	int nNumSwap = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// SWAPを見つけたら折り返し情報を格納
		if (strcmp(&aText[0], "SWAP") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNumSwap);
		}
		// END_PRESSLOGOを見つけたらプレスロゴの情報を格納を終える
		if (strcmp(&aText[0], "END_PRESSLOGOSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// プレスロゴの生成
					CPressLogo *pLogo =CPressLogo::Create(pos, size, nNumSwap, pTex->SetTexture(pTextureFileName[nNum]));
					pLogo->SetStartCnt(false);
					CPreparationPlayer::GetPPM()->SetPressLogo(pLogo);
					
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------
// ロゴ情報の読み込み
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nNum = 0;
	int nNumSwap = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXCOLOR col = { 0.0f,0.0f,0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// END_LOGOSETを見つけたらロゴの情報を格納を終える
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// ロゴの生成
					CScene2D::Create(pos, D3DXVECTOR3(size.x,size.y,0.0f), CScene::PRIORITY_UI, pTex->SetTexture(pTextureFileName[nNum]));
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------
// フレーム情報の読み込み
//-----------------------------------------------------------------
bool CSetPreparationPlayer::LoadFrame(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nNum = 0;
	int nNumSwap = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXVECTOR2 texsheet = { 0.0f,0.0f };
	D3DXVECTOR2 tex = { 0.0f,0.0f };

	while (fgets(aLine, 128, pFile) != NULL)
	{
		// 毎行空白を見つけるまで読み込む
		fscanf(pFile, "%s", &aText[0]);

		// POSを見つけたら座標情報を格納
		if (strcmp(&aText[0], "TYPE") == 0)
		{
			fscanf(pFile, "%s %d", &aText[0], &nNum);
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
		// TEXSHEETを見つけたらUV値情報を格納
		if (strcmp(&aText[0], "TEXSHEET") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&texsheet.x,
				&texsheet.y);
		}
		// TEXを見つけたらテクスチャ座標情報を格納
		if (strcmp(&aText[0], "TEX") == 0)
		{
			fscanf(pFile, "%s %f %f", &aText[0],
				&tex.x,
				&tex.y);
		}
		// END_FRAMESETを見つけたらフレームの情報を格納を終える
		if (strcmp(&aText[0], "END_FRAMESET") == 0)
		{
			if (nNum < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// フレームの生成
					CScene2D *pFrame = CScene2D::Create(pos, D3DXVECTOR3(size.x, size.y, 0.0f), CScene::PRIORITY_UI, pTex->SetTexture(pTextureFileName[nNum]));
					pFrame->SetTexSheet(texsheet);
					pFrame->SetTex(tex);
					CPreparationPlayer::GetPPM()->SetFrame(pFrame,m_nCntFrame);

					// カウント
					m_nCntFrame++;

					return true;
				}
			}
		}
	}
	return false;
}