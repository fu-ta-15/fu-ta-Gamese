//=============================================================================
//
// �v���C���[������ʏ��� [preparation_player.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _PREPARATIONPLAYER_H_
#define _PREPARATIONPLAYER_H_

#include "base.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPreparationPlayerManager;		// �v���C���[������ʊǗ��N���X

//*****************************************************************************
// �v���C���[�ҋ@��ʃN���X(�h���N���X)
//*****************************************************************************
class CPreparationPlayer : public CBase
{
public:
	// �^�C�g���̃��[�h
	typedef enum
	{
		PREPARATIONPLAYER_NORMAL = 0,				// �ʏ�
		PREPARATIONPLAYER_TRANSITION_GAME_BATTLE,	// �V���O�����[�h�J��
		PREPARATIONPLAYER_TRANSITION_GAME_COOPERATION,	// �}���`���[�h�J��
		PREPARATIONPLAYER_TRANSITION_MODESELECT,	// ���[�h�I��
		PREPARATIONPLAYER_MAX,
	}PREPARATIONPLAYER;

	CPreparationPlayer();
	~CPreparationPlayer();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	PREPARATIONPLAYER GetMode(void) { return m_preparationPlayer; }
	void SetMode(PREPARATIONPLAYER preparationPlayer) { m_preparationPlayer = preparationPlayer; }

	static CPreparationPlayerManager *GetPPM(void) { return m_pPPM; }
	static int GetnCntPlayer(void) { return m_nCntPlayerOP; }

private:
	static int m_nCntPlayerOP;
	int m_nNumPlayer;
	PREPARATIONPLAYER m_preparationPlayer;		// ���[�h�I�����[�h
	static CPreparationPlayerManager *m_pPPM;	// �v���C���[������ʊǗ����̃|�C��
};
#endif