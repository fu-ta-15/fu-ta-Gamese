//-------------------------------------------------------------------
//
// モデル配置ヘッダー (set_model.h)
// Author:Itsuki Takaiwa
//
//-------------------------------------------------------------------
#ifndef _SETMODEL_H_
#define _SETMODEL_H_

#include "main.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAX_LIGHT		(3)				// ライトの最大数

//-------------------------------------------------------------------
// モデル配置クラス(基本クラス)
//-------------------------------------------------------------------
class CSetModel
{
public:
	CSetModel();
	~CSetModel();

	void Init(void);
	static void Update(void);
	void LoadData(void);

private:
	bool LoadFloor(FILE *pFile, int nNumTex, char **pTextureFileName);		// 床
	bool LoadStage(FILE *pFile, int nNumModel, char **pModelFileName);
	bool LoadModel(FILE *pFile, int nNumModel, char **pModelFileName);

	static char *m_apTextName;				// 読み込むテキストの情報
	static char *m_apTextNameSetModel[64];	// モデル配置情報を読み込むテキストの情報
	static int m_nNumType;					// モデルの種類数
	static int m_nNumModel;					// モデルの総数
};

#endif