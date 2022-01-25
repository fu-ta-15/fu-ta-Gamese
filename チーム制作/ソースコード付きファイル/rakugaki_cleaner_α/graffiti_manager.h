//=============================================================================
//
// ���N�K�L�Ǘ����� [graffiti_manager.h]
// Author : 
//
//=============================================================================
#ifndef _GRAFFITIMANAGER_H_
#define _GRAFFITIMANAGER_H_

#include "scene.h"

//*****************************************************************************
// ���N�K�L�Ǘ��N���X(��{�N���X)
//*****************************************************************************
class CGraffitiManager : public CScene
{
public:
	CGraffitiManager();
	~CGraffitiManager();

	HRESULT Init(D3DXVECTOR3 pos ,D3DXVECTOR2 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetTotalGraffiti(void) { return m_nNumGraffiti; }
	void SetTotalGraffiti(int nAdd) { m_nNumGraffiti += nAdd; }
	int GetCurGraffiti(void) { return m_nNumCurGraffiti; }
	void SetCurGraffiti(int nAdd) { m_nNumCurGraffiti += nAdd; }

private:
	int m_nNumGraffiti;				// ���N�K�L�̑���
	int m_nNumCurGraffiti;			// ���݂̃��N�K�L��
};
#endif