//-----------------------------------------------------------------
//
//  モード選択画面配置 (set_mode_select.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_mode_select.h"
#include "sceneX.h"
#include "debugproc.h"

#include "manager.h"
#include "texture.h"

#include "press_logo.h"
#include "title_logo.h"
#include "bg.h"
#include "mode_logo.h"
#include "Scene2D.h"

//-----------------------------------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// CSetModeSelectのコンストラクタ
//-----------------------------------------------------------------
CSetModeSelect::CSetModeSelect()
{

}

//-----------------------------------------------------------------
// CSetModeSelectのデストラクタ
//-----------------------------------------------------------------
CSetModeSelect::~CSetModeSelect()
{

}

//-----------------------------------------------------------------
// モデル配置情報の初期化
//-----------------------------------------------------------------
void CSetModeSelect::Init(void)
{
	CModeLogo *pModeLogo = CModeLogo::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f });

	LoadData();
}

//-----------------------------------------------------------------
// モデル配置情報の更新処理
//-----------------------------------------------------------------
void CSetModeSelect::Update(void)
{

}

//---------------------------------------------------------------- -
// UI情報の読み込み
//-----------------------------------------------------------------
void CSetModeSelect::LoadData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/01_mode_select/mode_select.txt", "r");

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
				if (strcmp(&aText[0], "LOGOSET") == 0)
				{
					LoadLogo(pFile, nNumTex, pTextureFileName);
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
bool CSetModeSelect::LoadBg(FILE *pFile, int nNumTex, char **pTextureFileName)
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
// モード選択画面ロゴ情報の読み込み
//-----------------------------------------------------------------
bool CSetModeSelect::LoadLogo(FILE *pFile, int nNumTex, char **pTextureFileName)
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
		if (strcmp(&aText[0], "END_LOGOSET") == 0)
		{
			if (nNum < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// プレスロゴの生成
					CScene2D::Create(pos, D3DXVECTOR3(size.x,size.y,0.0f),CScene::PRIORITY_UI, pTex->SetTexture(pTextureFileName[nNum]));
					return true;
				}
			}
		}
	}
	return false;
}