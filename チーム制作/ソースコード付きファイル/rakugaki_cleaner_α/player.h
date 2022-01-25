//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;			// モデルクラス
class CMotion;			// モーションクラス

//-----------------------------------------------------------------------------
// プレイヤークラス(派生クラス)
//-----------------------------------------------------------------------------
class CPlayer : public CScene
{
public:
	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTORAL = 0,	// ニュートラル
		MOTIONTYPE_MOVE,			// 移動
		MOTIONTYPE_DELETE,			// 消し消し
		MOTIONTYPE_SKILL,			// スキル
		MOTIONTYPE_VICTORY,			// 勝利
		MOTIONTYPE_LOSE,			// 敗北
		MOTIONTYPE_MAX,
	}MotionType;

	CPlayer(int nPriority);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nNumber);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CModel *GetModel(int nCnt) { return m_apModel[nCnt]; }
	MotionType GetMotionType(void) { return m_motionType; }
	int GetNumber(void) { return m_nNumber; }
	D3DXVECTOR3 GetOffsetPos(int nCnt) { return D3DXVECTOR3(m_aOffset[nCnt][0], m_aOffset[nCnt][1], m_aOffset[nCnt][2]); }
	D3DXVECTOR3 GetOffsetRot(int nCnt) { return D3DXVECTOR3(m_aOffset[nCnt][3], m_aOffset[nCnt][4], m_aOffset[nCnt][5]); }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	bool GetFinishMotion(void) { return m_bFinishMotion; }

	void SetMotionType(MotionType motiontype) { m_motionType = motiontype; }
	void SetNumber(int nNumber) { m_nNumber = nNumber; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }
	void SetFinishMotion(bool bFinish) { m_bFinishMotion = bFinish; }

protected:
	void Motion(void);						// モーション
	CMotion *m_pMotion;						// モーション情報のポインタ

private:
	CModel *m_apModel[10];					// モデルへのポインタ
	int m_nIndexParent[10];					// 親モデルのINDEX
	float m_aOffset[10][6];					// 親モデルからのオフセット
	char m_apModelFileName[10][128];		// モデルファイルネーム
	int m_nNumModel;						// モデル数

	MotionType m_motionType;				// モーションの種類

	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 現在の位置
	D3DXVECTOR3 m_posOld;					// 前回の位置
	D3DXVECTOR3 m_size;						// サイズ
	D3DXVECTOR3 m_rot;						// 向き
	D3DXVECTOR3 m_rotDest;					// 目的の向き

	int m_nNumber;							// 何番目のプレイヤーか
	bool m_bFinishMotion;					// ループしないモーションが終わったかどうか
};
#endif