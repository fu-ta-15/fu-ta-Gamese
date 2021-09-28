//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"



class CSound
{
public:
	//*****************************************************************************
	// サウンドファイル
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// タイトル
		SOUND_LABEL_BGM001,			// チュートリアル
		SOUND_LABEL_BGM002,			// ゲーム
		SOUND_LABEL_BGM003,			// result

		SOUND_LABEL_SE_PAUSE0,		// 弾発射音
		SOUND_LABEL_SE_PAUSE1,		// ヒット音
		SOUND_LABEL_SE_PAUSE2,		// 爆発音
		SOUND_LABEL_SE_ATTACK,		// 弾発射音
		SOUND_LABEL_SE_OK,
		SOUND_LABEL_SE_NO,

		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDPARAM;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:

	static IXAudio2					*m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice		*m_pMasteringVoice;						// マスターボイス
	static IXAudio2SourceVoice			*m_apSourceVoice[SOUND_LABEL_MAX];			// ソースボイス
	static BYTE						*m_apDataAudio[SOUND_LABEL_MAX];			// オーディオデータ
	static DWORD						 m_aSizeAudio[SOUND_LABEL_MAX];			// オーディオデータサイズ
	static SOUNDPARAM					 m_aParam[SOUND_LABEL_MAX];				// 各音素材のパラメータ
		


};


#endif
