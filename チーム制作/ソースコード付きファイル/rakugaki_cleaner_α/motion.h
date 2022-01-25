//=============================================================================
//
// デバックプロックヘッダー (debugproc.h)
// Author:Itsuki Takaiwa
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"

//*****************************************************************************
// モーションクラス(基本クラス)
//*****************************************************************************
class CMotion
{
public:
	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_PLAYER = 0,
		MOTIONTYPE_MAX,
	}MotionType;

	// キ-
	typedef struct
	{
		float fPosX;			// 位置X
		float fPosY;			// 位置Y
		float fPosZ;			// 位置Z
		float fRotX;			// 向きX
		float fRotY;			// 向きY
		float fRotZ;			// 向きZ
	}KEY;						

	// キー情報
	typedef struct
	{
		int nFrame;				// フレーム数
		KEY aKey[10];			// 各モデルのキー要素
	}KEY_INFO;					

	// モーション情報
	typedef struct
	{
		bool bLoop;				// ループするかどうか
		int nNumKey;			// キーの総数
		KEY_INFO aKeyInfo[5];	// キー情報
	}MOTION_INFO;				

	typedef struct
	{
		int nKey;				// キーNo.
		int nCounterMotion;		// モーションカウンタ
	}MOTION;

	CMotion();
	~CMotion();

	static CMotion *Create(void);
	void Init(void);
	static void LoadMotionData(void);

	static MOTION_INFO *GetMotionInfo(int nNum) { return &m_aMotionInfo[nNum][0]; }
	MOTION GetMotion(void) { return m_motion; }
	void SetMotion(MOTION motion) { m_motion = motion; }

private:
	static char m_aTextMotion[MOTIONTYPE_MAX][128];
	static MOTION_INFO m_aMotionInfo[MOTIONTYPE_MAX][10];		// モーション情報のポインタ
	MOTION m_motion;											// モーションの状態
};

#endif