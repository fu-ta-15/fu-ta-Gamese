//*****************************************************************************
//
// Imgui���� [imguimanager.h]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
#ifndef _IMGUIMANAGER_H_
#define _IMGUIMANAGER_H_

//-----------------------------------------------------------------------------
// Imgui�t�@�C���̃C���N���[�h
//-----------------------------------------------------------------------------
#include "main.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "mesh3D.h"

//-----------------------------------------------------------------------------
// ���O��ԁEImGui�}�l�[�W��
//-----------------------------------------------------------------------------
namespace ImGuiMana
{
	HRESULT Init(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9 pD3DDevice);

	void Uninit(void);

	void Update(void);

	void DeviState(void);

	void Draw(void);

	void DrawEnd(HRESULT result,LPDIRECT3DDEVICE9 pD3DDevice, D3DPRESENT_PARAMETERS d3dpp);
}


//-----------------------------------------------------------------------------
// ���O��ԁE���b�V�����X�g
//-----------------------------------------------------------------------------
namespace MeshList
{
	void MeshInfo(void);
	void MeshWave(void);
	void MeshCycle(void);
	void CreateMeshMenu(void);

	static CMesh3D *m_pMesh = NULL;					// ���b�V���̏����󂯎��|�C���^
	static bool m_bCreate = false;					// ����
	static int m_nVertical = 0;						// �c���̐�
	static int m_nSide = 0;							// �����̐�
	static D3DXVECTOR3 m_pos;						// �ʒu
	static D3DXVECTOR3 m_size;						// �傫��
	static bool m_bTimeStart = false;				// �J�E���g�J�n�ƒ�~
	static int m_nTime = 0;							// ���ԁi�t���[���J�E���g)
	static float m_fHeight = 0.0f;					// �g�̍���
	static D3DXVECTOR3 m_WaveCenter = {};			// �k��
	static int m_nCycle = 100;						// ����
	static bool m_bDemo = false;					// �f���E�B���h�E
	static D3DFILLMODE m_fillmode = D3DFILL_SOLID;	// ���C���[�t���[��
	static DWORD m_d3dcullmode = D3DCULL_CCW;		// �J�����O���[�h
	static bool m_bNorSeting = false;
	static bool m_bCycleStart = false;
};

//-----------------------------------------------------------------------------
// ���O��ԁE���b�V���̐ݒ�
//-----------------------------------------------------------------------------
namespace MeshOptionMenu
{
	void MeshOption(void);
}

//-----------------------------------------------------------------------------
// ���O��ԁE�e�N�X�`������
//-----------------------------------------------------------------------------
namespace TextureMake
{
	void TextureLoad(void);
	void TextureBind(void);
}

//-----------------------------------------------------------------------------
// ���O��ԁE�ڍאݒ�/����
//-----------------------------------------------------------------------------
namespace Option
{
	void OperationExplanation(void);
}

//-----------------------------------------------------------------------------
// ���O��ԁE���C�g�ݒ�
//-----------------------------------------------------------------------------
namespace LightOption
{
	void LightMove(void);
}

#endif // !_IMGUI_H_
