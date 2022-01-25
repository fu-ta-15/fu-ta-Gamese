//=============================================================================
//
// ���N�K�L�p�Q�[�W���� [graffiti_gauge.h]
// Author : itsuki takaiwa
//
//=============================================================================
#ifndef _GRAFFITI_GAUGE_H_
#define _GRAFFITI_GAUGE_H_

//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "gauge.h"

//-----------------------------------------------------------------------------
// �O���錾
//-----------------------------------------------------------------------------
class CScore;			// �X�R�A�N���X
class CNumber;			// �i���o�[�N���X
class CScene2D;			// Cscene2D�N���X

//-----------------------------------------------------------------------------
// �N���X
//-----------------------------------------------------------------------------
class CGraffitiGauge : public CGauge
{
public:
	CGraffitiGauge();
	~CGraffitiGauge();

	static CGraffitiGauge * Create(const D3DXVECTOR3& pos, const D3DXVECTOR2& size, int nTex);

	HRESULT Init(int nTex);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRatio(float fRatio) { m_fRatio = fRatio; }
	void SetPosXRightSide(float fPos) { m_fPosXRightSide = fPos; }

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_size;
	float m_fRatio;				// ����
	float m_fPosXRightSide;		// �E�[��X���W

	CScore *m_pScoreInteger;	// �X�R�A�N���X���̃|�C���^(��������)
	CNumber *m_pNumberDecimal;	// �i���o�[�N���X���̃|�C���^(��������)
	CScene2D *m_pLogoComma;		// �R���}���S���̃|�C���^
	CScene2D *m_pLogoPercent;	// �p�[�Z���g���S���̃|�C���^
};
#endif