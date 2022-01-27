//-----------------------------------------------------------------
//
// モデル配置 (set_model.cpp)
// Author:Itsuki Takaiwa
//
//-----------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "set_model.h"
#include "sceneX.h"
#include "stage.h"
#include "floor.h"
#include "texture.h"
#include "manager.h"
#include "debugproc.h"

//-----------------------------------------------------------------
// 静的メンバ変数宣言
//-----------------------------------------------------------------
char *CSetModel::m_apTextName = {};				// 読み込むテキストの情報
char *CSetModel::m_apTextNameSetModel[64] = {};	// モデル配置情報を読み込むテキストの情報
int CSetModel::m_nNumType = 0;					// モデルの種類数
int CSetModel::m_nNumModel = 0;					// モデルの総数

//-----------------------------------------------------------------
// CSetModelのコンストラクタ
//-----------------------------------------------------------------
CSetModel::CSetModel()
{

}

//-----------------------------------------------------------------
// CSetModelのデストラクタ
//-----------------------------------------------------------------
CSetModel::~CSetModel()
{

}

//-----------------------------------------------------------------
// モデル配置情報の初期化
//-----------------------------------------------------------------
void CSetModel::Init(void)
{

}

//-----------------------------------------------------------------
// モデル配置情報の更新処理
//-----------------------------------------------------------------
void CSetModel::Update(void)
{
	// 表示
	CDebugProc::Print("\n--- モデル情報 ---\n");
	CDebugProc::Print("モデルの種類 (%d)\n", m_nNumType);
	CDebugProc::Print("モデルの総数 (%d)\n", m_nNumModel);
}

//-----------------------------------------------------------------
// モデル配置情報の読み込み
//-----------------------------------------------------------------
void CSetModel::LoadData(void)
{
	// FILE型のポインタ関数
	FILE *pFile;

	// ファイルを開く
	pFile = fopen("data/TEXT/04_game/model.txt", "r");

	// NULLチェック
	if (pFile != NULL)
	{
		// 変数宣言
		char aLine[128];
		char aText[128];
		bool bScript = false;
		bool bPartsSet = false;
		int nCntTex = 0;
		int nCntModel = 0;
		int nNumModelType = -1;
		D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
		int nNumModel = 0;
		char **apModelFileName;
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
				if (strcmp(&aText[0], "TEXTURE_FILENAME") == 0 && nCntTex < nNumTex)
				{
					fscanf(pFile, "%s %s", &aText[0], &apTextureFileName[nCntTex][0]);

					nCntTex++;
				}
				if (strcmp(&aText[0], "NUM_MODEL") == 0)
				{
					fscanf(pFile, "%s %d", &aText[0], &nNumModel);

					m_nNumType = nNumModel;

					// モデルファイルネーム格納用変数の領域を動的に確保する
					apModelFileName = new char*[nNumModel];

					for (int nCnt = 0; nCnt < nNumModel; nCnt++)
					{
						apModelFileName[nCnt] = new char[128];
					}
				}
				// MODEL_FILENAMEを見つけたらXファイル名を格納
				if (strcmp(&aText[0], "MODEL_FILENAME") == 0 && nCntModel < nNumModel)
				{
					fscanf(pFile, "%s %s", &aText[0], &apModelFileName[nCntModel][0]);

					nCntModel++;
				}
				// FLOORSETを見つけたら床情報を格納する許可を与える
				if (strcmp(&aText[0], "FLOORSET") == 0)
				{
					LoadFloor(pFile, nNumTex, apTextureFileName);
				}
				// STAGESETを見つけたらステージ情報を格納する許可を与える
				if (strcmp(&aText[0], "STAGESET") == 0)
				{
					LoadStage(pFile, nNumModel, apModelFileName);
				}
				// MODELSETを見つけたらモデル情報を格納する許可を与える
				if (strcmp(&aText[0], "MODELSET") == 0)
				{
					LoadModel(pFile, nNumModel, apModelFileName);
				}
				// END_SCRIPTを見つけたら読み込みを終える
				if (strcmp(&aText[0], "END_SCRIPT") == 0)
				{
					// メモリの開放
					delete[] apModelFileName;

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
// 床情報の読み込み
//-----------------------------------------------------------------
bool CSetModel::LoadFloor(FILE *pFile, int nNumTex, char **pTextureFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR2 size = { 0.0f,0.0f };
	D3DXVECTOR2 block = { 0.0f,0.0f };

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
			if (nTex < nNumTex)
			{
				// テクスチャ情報の取得
				CTexture *pTex = CManager::GetTexture();

				if (pTex != NULL)
				{
					// モデルの生成
					CFloor::Create(pos, size, block, pTex->SetTexture(&pTextureFileName[nTex][0]));
				}
			}
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// ステージ情報の読み込み
//-----------------------------------------------------------------
bool CSetModel::LoadStage(FILE *pFile, int nNumModel, char **pModelFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f ,0.0f};

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
		// ROTを見つけたら角度情報を格納
		if (strcmp(&aText[0], "ROT") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&rot.x,
				&rot.y,
				&rot.z);

			rot.x = (rot.x / 180.0f) * D3DX_PI;
			rot.y = (rot.y / 180.0f) * D3DX_PI;
			rot.z = (rot.z / 180.0f) * D3DX_PI;
		}
		// END_STAGESETを見つけたらステージの情報を格納を終える
		if (strcmp(&aText[0], "END_STAGESET") == 0)
		{
			if (nTex < m_nNumType)
			{
				// モデルの生成
				CStage::Create(pos, D3DXVECTOR2(0.0f, 0.0f), rot, &pModelFileName[nTex][0]);
			}

			// モデル総数のカウント
			m_nNumModel++;

			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------
// モデル情報の読み込み
//-----------------------------------------------------------------
bool CSetModel::LoadModel(FILE *pFile, int nNumModel, char **pModelFileName)
{
	// 変数宣言
	char aLine[128];
	char aText[128];
	int nTex;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f ,0.0f };

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
		// ROTを見つけたら角度情報を格納
		if (strcmp(&aText[0], "ROT") == 0)
		{
			fscanf(pFile, "%s %f %f %f", &aText[0],
				&rot.x,
				&rot.y,
				&rot.z);

			rot.x = (rot.x / 180.0f) * D3DX_PI;
			rot.y = (rot.y / 180.0f) * D3DX_PI;
			rot.z = (rot.z / 180.0f) * D3DX_PI;
		}
		// END_PARTSSETを見つけたら各パーツの情報を格納を終える
		if (strcmp(&aText[0], "END_MODELSET") == 0)
		{
			if (nTex < m_nNumType)
			{
				// モデルの生成
				CSceneX::Create(pos, D3DXVECTOR2(0.0f, 0.0f), rot, &pModelFileName[nTex][0]);
			}

			// モデル総数のカウント
			m_nNumModel++;

			return true;
		}
	}
	return false;
}