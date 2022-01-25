//-----------------------------------------------------------------
//
//  タイトル配置 (set_title.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_title.h"
#include "sceneX.h"
#include "debugproc.h"

#include "manager.h"
#include "texture.h"

#include "title.h"
#include "title_manager.h"
#include "press_logo.h"
#include "title_logo.h"
#include "bg.h"

//-----------------------------------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// CSetTitleのコンストラクタ
//-----------------------------------------------------------------
CSetTitle::CSetTitle()
{

}

//-----------------------------------------------------------------
// CSetTitleのデストラクタ
//-----------------------------------------------------------------
CSetTitle::~CSetTitle()
{

}

//-----------------------------------------------------------------
// モデル配置情報の初期化
//-----------------------------------------------------------------
void CSetTitle::Init(void)
{
	LoadData();
}

//-----------------------------------------------------------------
// モデル配置情報の更新処理
//-----------------------------------------------------------------
void CSetTitle::Update(void)
{

}

//---------------------------------------------------------------- -
// UI情報の読み込み
//-----------------------------------------------------------------
void CSetTitle::LoadData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/00_title/title.txt", "r");

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
				// TITLELOGOSETのときタイトルロゴ情報を格納する
				if (strcmp(&aText[0], "TITLELOGOSET") == 0)
				{
					LoadTitleLogo(pFile, nNumTex, pTextureFileName);
				}
				// PRESSLOGOSETのときプレスロゴ情報を格納する
				if (strcmp(&aText[0], "PRESSLOGOSET") == 0)
				{
					LoadPressLogo(pFile, nNumTex, pTextureFileName);
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
	}
}

//-----------------------------------------------------------------
// 背景情報の読み込み
//-----------------------------------------------------------------
bool CSetTitle::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
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
					CBg::Create(pos, size, col, moveTex,pTex->SetTexture(pTextureFileName[nNum]));
					return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------
// タイトルロゴ情報の読み込み
//-----------------------------------------------------------------
bool CSetTitle::LoadTitleLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nNum = 0;
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
		// END_TITLELOGOSETを見つけたらタイトルロゴの情報を格納を終える
		if (strcmp(&aText[0], "END_TITLELOGOSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// プレスロゴの生成
					CTitleLogo::Create(pos, size, pTex->SetTexture(pTextureFileName[nNum]));
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
bool CSetTitle::LoadPressLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
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
			fscanf(pFile, "%s %d", &aText[0],&nNumSwap);
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
					CTitle::GetTitleManager()->SetPressLogo(CPressLogo::Create(pos, size, nNumSwap, pTex->SetTexture(pTextureFileName[nNum])));
					return true;
				}
			}
		}
	}
	return false;
}