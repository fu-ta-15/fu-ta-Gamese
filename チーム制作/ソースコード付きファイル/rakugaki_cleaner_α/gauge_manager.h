//=============================================================================
//
// �Q�[�W�Ǘ����� [gauge_manager.h]
// Author : 
//
//=============================================================================
#ifndef _GAUGEMANAGER_H_
#define _GAUGEMANAGER_H_

#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CGraffitiGauge;		// ���N�K�L�Q�[�W�N���X
class CCrown;				// �����N���X
class CPlayerMarker;		// �v���C���[�}�[�J�[�N���X

//*****************************************************************************
// �Q�[�W�Ǘ��N���X(��{�N���X)
//*****************************************************************************
class CGaugeManager : public CScene
{
public:
	CGaugeManager();
	~CGaugeManager();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetGraffitiGauge(CGraffitiGauge *pGauge) { m_pGraffitiGauge = pGauge; }

private:
	void ScoreGauge(void);
	void SkillGauge(void);
	void GraffitiGauge(void);

	int m_nNumPlayer;			// �v���C���[��
	float *m_pAddSizeOld;		// �O��̑�����
	int m_nCntRank_One;			// ��ʂ̐l��

	CGraffitiGauge *m_pGraffitiGauge;	// ���N�K�L�Q�[�W���̃|�C���^
	CCrown **m_pCrown;					// �������̃|�C���^
	CPlayerMarker **m_pPlayerMarker;	// �v���C���[�}�[�J�[���̃|�C���^
};
#endif