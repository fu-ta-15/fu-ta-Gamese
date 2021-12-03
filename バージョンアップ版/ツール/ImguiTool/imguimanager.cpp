//*****************************************************************************
//
// ImGui�}�l�[�W������ [imguimanager.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include "imguimanager.h"
#include "imgui_ja_gryph_ranges.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "keyinput.h"
#include "TextureScene.h"


//=============================================================================
// ImGui�̏���������
//=============================================================================
HRESULT ImGuiMana::Init(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9 pD3DDevice)
{
	// Version�`�F�b�N
	IMGUI_CHECKVERSION();

	// �e�L�X�g�����ƃA�N�Z�X
	ImGui::CreateContext();

	// ���擾
	ImGuiIO& io = ImGui::GetIO();

	// ����ƃt�H���g�̒ǉ�
	io.Fonts->AddFontFromFileTTF("imgui/Fonts/Gothic.ttc", 13.0f, nullptr, Japanese::glyphRangesJapanese);

	// �F�̃X�^�C��
	ImGui::StyleColorsDark();

	// ImGui�̏�����
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pD3DDevice);

	return S_OK;
}

//=============================================================================
// ImGui�̏I������
//=============================================================================
void ImGuiMana::Uninit(void)
{
	// DX9�pImgui�̃V���b�g�_�E��
	ImGui_ImplDX9_Shutdown();

	// Win32Imgui�̃V���b�g�_�E��
	ImGui_ImplWin32_Shutdown();

	// �e�L�X�g�̊J��
	ImGui::DestroyContext();
}

//=============================================================================
// ImGui�̍X�V����
//=============================================================================
void ImGuiMana::Update(void)
{
	// DX9�̃t���[������
	ImGui_ImplDX9_NewFrame();

	// Win32�̃t���[������
	ImGui_ImplWin32_NewFrame();

	// �t���[���̐���
	ImGui::NewFrame();

	// ���b�V�����̍X�V����
	MeshList::Update();

	// �t���[�������I��
	ImGui::EndFrame();
}

//=============================================================================
// �����_���[��Ԃ̊Ǘ�
//=============================================================================
void ImGuiMana::DeviState(void)
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// ���C���[�t���[��ON�EOFF
	pDevice->SetRenderState(D3DRS_FILLMODE, MeshList::m_fillmode);
	pDevice->SetRenderState(D3DRS_CULLMODE, MeshList::m_d3dcullmode);
}

//=============================================================================
// ImGui�̕`�揈��
//=============================================================================
void ImGuiMana::Draw(void)
{
	// �f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// ���C���[�t���[���̉e���󂯂Ȃ����߂����Őݒ�
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// DX9�ł̃����_���[�J�n
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=============================================================================
// ImGui�̕`��I������
//=============================================================================
void ImGuiMana::DrawEnd(HRESULT result, LPDIRECT3DDEVICE9 pD3DDevice, D3DPRESENT_PARAMETERS d3dpp)
{
	// �ȉ��ɂ�������������Ă�����
	if (result == D3DERR_DEVICELOST && pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		// DX�X�Ŏg�p���Ă����I�u�W�F�N�g��f�o�C�X�̔j��
		ImGui_ImplDX9_InvalidateDeviceObjects();
		HRESULT hr = pD3DDevice->Reset(&d3dpp);
		if (hr == D3DERR_INVALIDCALL)
			IM_ASSERT(0);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

//=============================================================================
// ���b�V���Ŏg�p����X�V����
//=============================================================================
void MeshList::Update(void)
{
	// ���݂̃��[�h�̎擾
	CManager::MODE mode = CManager::GetMode();

	// ���[�h�擾
	if (mode == CManager::MODE_GAME)
	{
		// Imgui�E�B���h�E����
		ImGui::Begin(u8"�c�[���E�B���h�E");

		// ���b�V������
		MeshList::MeshInfo();

		// Imgui�E�B���h�E��������
		ImGui::End();
	}
}

//=============================================================================
// ImGui�̃��b�V���|���S���̏��
//
// �E�����ݒ�/�g�̕\���̐ݒ�/���̑��ݒ�c
//=============================================================================
void MeshList::MeshInfo(void)
{
	// �f���E�B���h�E�̕\���ؑ�Button
	if (ImGui::Button(u8"[�f���E�B���h�E�̐���]"))
	{
		// �f���E�B���h�E�̕\���ؑ�
		m_bDemo = m_bDemo ? false : true;
	}

	// true�̏ꍇ�̂݃f���E�B���h�E�\��
	if (m_bDemo)
	{
		// �f���E�B���h�E�̐���
		ImGui::ShowDemoWindow();
	}

	// �������C���ɊԊu��������
	ImGui::SameLine(0.0f, 20.0f);

	// �������̕\���ؑ�Button
	if (ImGui::Button(u8"������"))
	{
		// �������̕\���ؑ�
		m_bExplanation = m_bExplanation ? false : true;
	}

	// true�̏ꍇ�̂ݐ������\��
	if (m_bExplanation)
	{
		// Imgui�E�B���h�E����
		ImGui::Begin(u8"�J�����⃉�C�getc");

		// �ݒ�
		Option::OperationExplanation();

		// Imgui�E�B���h�E��������
		ImGui::End();
	}

	// �󔒂̍s�𐶐�
	ImGui::Spacing();

	// ���j���[���̐���
	if (ImGui::CollapsingHeader(u8"���b�V���|���S�����X�g"))
		{
			// ���b�V���̐���
			CreateMeshMenu();

			// ���j���[���̐���
			if (ImGui::CollapsingHeader(u8"�\�����@"))
			{
				// �g�̕\��
				MeshList::MeshWave();

				// �󔒂̍s�𐶐�
				ImGui::Spacing(), ImGui::Spacing(), ImGui::Spacing();

				// ��]�\��
				MeshList::MeshCycle();
			}

			// ���j���[���̐���
			if (ImGui::CollapsingHeader(u8"���̑��ڍ�"))
			{
				// ���b�V���̐�����
				MeshOptionMenu::MeshOption();
			}
		}
}

//=============================================================================
// ���b�V���|���S���ɂ��g�̕\���ݒ�
//=============================================================================
void MeshList::MeshWave(void)
{
	// �g�̕\���̏��
	if (ImGui::TreeNode(u8"---�g�̕\��---"))
	{
		// �J�E���g�J�n�E��~�̐؂�ւ��{�^��
		if (ImGui::Button(u8"�g:[�Đ�]�E[��~]"))
		{
			// �����ꂽ��Đ��ƒ�~�̂ǂ��炩�ɂ���
			m_bWaveStart = m_bWaveStart ? false : true;
		}

		// �t���[���J�E���g�J�n����Ă�����
		if (m_bWaveStart)
		{
			// �J�E���g�A�b�v
			m_nTime++;

			// ��������Ă�����g�̕\���J�n
			if (m_pMesh != NULL)
			{
				// �g�̕\���̊J�n
				m_pMesh->MeshWave(m_WaveCenter, m_nTime, m_fHeight, m_nCycle);
			}
		}

		// �g�̏ڍאݒ�m�[�h
		if (ImGui::TreeNode(u8"---�g�̏ڍאݒ�---"))
		{
			// �g�Ƃ���̍���
			ImGui::InputFloat(u8"����", &m_fHeight, 0.5f);

			// �g�̎����̒���
			ImGui::InputInt(u8"����", &m_nCycle, 1);

			// �g���N�������_�̐ݒ�m�[�h
			if (ImGui::TreeNode(u8"�k��"))
			{
				// �g���N�������_�̐ݒ�
				ImGui::InputFloat(u8"X���W", &m_WaveCenter.x, 0.5f);
				ImGui::InputFloat(u8"Z���W", &m_WaveCenter.z, 0.5f);

				// �m�[�h�̏I��
				ImGui::TreePop();
			}

			// �t���[���J�E���g�\���m�[�h
			if (ImGui::TreeNode("FRAME"))
			{
				// �t���[���̕\��
				ImGui::Text(u8" �J�E���g�� : %d", m_nTime);

				// �m�[�h�̏I��
				ImGui::TreePop();
			}

			// �m�[�h�̏I��
			ImGui::TreePop();
		}

		// �m�[�h�����I��
		ImGui::TreePop();
	}
}

//=============================================================================
// ���b�V���|���S���ɂ���]����
//=============================================================================
void MeshList::MeshCycle(void)
{
	// ��]�̕\��
	if (ImGui::TreeNode(u8"---��]�̕\��---"))
	{
		// ��]�J�n�E��~�̐؂�ւ��{�^��
		if (ImGui::Button(u8"��]:[�Đ�]�E[��~]"))
		{
			// true false �̐؂�ւ�
			m_bCycleStart = m_bCycleStart ? false : true;
		}

		// ��]�J�n�̍��}���o�Ă�����
		if (m_bCycleStart)
		{
			// ��������Ă�����g�̕\���J�n
			if (m_pMesh != NULL)
			{
				// ��]�^��
				m_pMesh->MeshCycleMove();
			}
		}

		// �m�[�h�̐����I��
		ImGui::TreePop();
	}
}

//=============================================================================
// ���b�V���������邽�߂̏ڍאݒ�
//=============================================================================
void MeshList::CreateMeshMenu(void)
{
	// ���ɕ��ׂ邽�߂̋󔒂̏���
	float SpacingButton = ImGui::GetStyle().ItemInnerSpacing.x + 10.0f;

	// Button�����s��������
	ImGui::PushButtonRepeat(true);

	// ���b�V���̐���Button
	if (ImGui::Button(u8"[����]"))
	{
		// NULL�ł���ΐ�������
		if (m_pMesh == NULL)
		{
			// �i�o�l�p�̒��ɓ���p�c�̖{���E���̖{���E�ʒu�E�T�C�Y�j
			m_pMesh = CMesh3D::Create(m_nVertical, m_nSide, m_pos, m_size);
		}

		// NULL�ł���ΐ�������
		if (m_pTextureList == NULL)
		{
			m_pTextureList = new CTextureScene;
		}
	}

	// �󔒂̐���
	ImGui::SameLine(0.0f, SpacingButton);

	// ���b�V���̍폜�{�^��
	if (ImGui::Button(u8"[�폜]"))
	{
		// NULL�łȂ���΍폜����
		if (m_pMesh != NULL)
		{
			// ���b�V���̊J��
			m_pMesh->Uninit();

			// NULL���
			m_pMesh = NULL;
		}

		m_nVertical = 0;							// �c���̖{��
		m_nSide = 0;								// �����̖{��
		m_pos = ZeroVector3;						// �ʒu
		m_size = D3DXVECTOR3(100.0f, 0.0f, 100.0f); // �T�C�Y
	}

	// �󔒂̐���
	ImGui::SameLine(0.0f, SpacingButton);

	// ���b�V���̕ύX�{�^��
	if (ImGui::Button(u8"[�ύX]"))
	{
		// NULL�łȂ���΍폜����
		if (m_pMesh != NULL)
		{
			// ���b�V���̊J��
			m_pMesh->Uninit();

			// NULL���
			m_pMesh = NULL;

			// �i�o�l�p�̒��ɓ���p�c�̖{���E���̖{���E�ʒu�E�T�C�Y�j
			m_pMesh = CMesh3D::Create(m_nVertical, m_nSide, m_pos, m_size);
		}
	}

	// �󔒂̐���
	ImGui::SameLine(0.0f, SpacingButton);

	// �ݒ肵�Ă������l�����Z�b�g
	if (ImGui::Button(u8"[���l���Z�b�g]"))
	{
		m_nVertical = 0;	  // �c���̖{��
		m_nSide = 0;		  // �����̖{��
		m_pos = ZeroVector3;  // �ʒu
		m_size = ZeroVector3; // �T�C�Y
	}

	// �J��Ԃ��\���̏I��
	ImGui::PopButtonRepeat();

	// ���̒ǉ��m�[�h
	if (ImGui::TreeNode(u8"���̒ǉ�"))
	{
		// ���݂̐��l�\���E���l���͉\�E�{�^���łP�}�\
		ImGui::InputInt(u8"�c��", &m_nVertical, 1);

		// ���E�l���z���Ă�����߂�����
		if (m_nVertical < 0) { m_nVertical = 0; }
		else if (m_nVertical > 150) { m_nVertical = 150; }

		// ���݂̐��l�\���E���l���͉\�E�{�^���łP�}�\
		ImGui::InputInt(u8"����", &m_nSide, 1);

		// ���E�l���z���Ă�����߂�����
		if (m_nSide < 0) { m_nSide = 0; }
		else if (m_nSide > 150) { m_nSide = 150; }

		// �m�[�h�̐����I��
		ImGui::TreePop();
	}

	// ���b�V���̈ʒu�ݒ�m�[�h
	if (ImGui::TreeNode(u8"---�ʒu---"))
	{
		// ���b�V�������J�n�̈ʒu�ݒ�
		ImGui::InputFloat(u8"X���W", &m_pos.x, 0.5f);
		ImGui::InputFloat(u8"Y���W", &m_pos.y, 0.5f);
		ImGui::InputFloat(u8"Z���W", &m_pos.z, 0.5f);

		// �m�[�h�̏I��
		ImGui::TreePop();
	}

	// ���b�V���̃T�C�Y�ݒ�m�[�h
	if (ImGui::TreeNode(u8"---�T�C�Y---"))
	{
		// ���b�V���̏c�Ɖ��̐ݒ�
		ImGui::InputFloat(u8"���T�C�Y", &m_size.x, 0.5f);
		ImGui::InputFloat(u8"�c�T�C�Y", &m_size.z, 0.5f);

		// �m�[�h�̏I��
		ImGui::TreePop();
	}
}

//=============================================================================
// �e�N�X�`���̃��[�h
//=============================================================================
void TextureMake::TextureLoad(void)
{
	// �L�[���̓N���X�̃|�C���^
	CKey *pKey = CManager::GetKey();

	if (ImGui::TreeNode(u8"�e�N�X�`������"))
	{
		if (ImGui::InputText(u8"�e�N�X�`����", cName, IM_ARRAYSIZE(cName)))
		{

		}
		else if (pKey->GetState(CKey::STATE_TRIGGER,DIK_RETURN))
		{
			MeshList::m_pTextureList->CreateTexture(&cName[0]);
		}

		ImGui::Text(u8"�����N��F%s", cLink);

		// �m�[�h�̏I��
		ImGui::TreePop();
	}
}

//=============================================================================
// �e�N�X�`���̓K�p
//=============================================================================
void TextureMake::TextureBind(void)
{
}

//=============================================================================
// ���C�g�̈ړ�
//=============================================================================
void LightOption::LightMove(void)
{
	// ���C�g�̏��擾
	CLight *pLight = CManager::GetLight();
}

//=============================================================================
// �J�����̑������
//=============================================================================
void Option::OperationExplanation(void)
{
	// �J������������p�m�[�h����
	if (ImGui::TreeNode(u8"�J��������"))
	{
		// ��������e�L�X�g�̕\��
		ImGui::Text(u8"�ړ����@");
		ImGui::Text(u8"W/�O : S/�� : A/�E : D/��");
		ImGui::Text(u8"T/�����_�̏�ړ��FB/�����_�̉��ړ��FY/���_�̏�ړ��FN/���_�̉��ړ�");

		// �󔒂̍s�𐶐�
		ImGui::Spacing(), ImGui::Spacing(), ImGui::Spacing();

		// ��������e�L�X�g�̕\��
		ImGui::Text(u8"����");
		ImGui::Text(u8"Q/���_�̍����� : E/���_�̉E����");
		ImGui::Text(u8"Z/�����_�̍����� : C/�����_�̉E����");

		// �m�[�h�̏I��
		ImGui::TreePop();
	}
}

//=============================================================================
// ���b�V���|���S���̏ڍאݒ�
//=============================================================================
void MeshOptionMenu::MeshOption(void)
{
	// �t���[�����[�h�̕ύX
	if (ImGui::Button(u8"�t���[�����[�h"))
	{
		// �t���[���̎��
		switch (MeshList::m_fillmode)
		{
		case D3DFILL_SOLID:

			// ���C���[�t���[���ł̕\��
			MeshList::m_fillmode = D3DFILL_WIREFRAME;
			break;
		case D3DFILL_WIREFRAME:
		
			// �|���S���Ƃ��ĕ`��
			MeshList::m_fillmode = D3DFILL_SOLID;
			break;

		default:
			break;
		}
	}
	
	// ������ɊԊu���󂯂�
	ImGui::SameLine(0.0f, 10.0f);

	// �e�L�X�g�\��
	if (MeshList::m_fillmode == D3DFILL_WIREFRAME) { ImGui::Text(u8"���C���[�t���[��"); }
	else if (MeshList::m_fillmode == D3DFILL_SOLID) { ImGui::Text(u8"�f�t�H���g�t���[��"); }

	// �J�����O���[�h�̕ύX
	if (ImGui::Button(u8"�J�����O���[�h"))
	{
		// �J�����O���@�̎��
		switch (MeshList::m_d3dcullmode)
		{
		case D3DCULL_CCW:

			// �J�����OOFF�i���ʂ��`��j
			MeshList::m_d3dcullmode = D3DCULL_NONE;
			break;

		case D3DCULL_NONE:

			// �J�����OON�i���ʕ`��Ȃ�)
			MeshList::m_d3dcullmode = D3DCULL_CCW;
			break;

		default:
			break;
		}
	}

	// ������ɊԊu���󂯂�
	ImGui::SameLine(0.0f, 10.0f);

	// �e�L�X�g�\��
	if (MeshList::m_d3dcullmode == D3DCULL_NONE) { ImGui::Text(u8"�J�����OOFF"); }
	else if (MeshList::m_d3dcullmode == D3DCULL_CCW) { ImGui::Text(u8"�J�����OON"); }

	// �J�E���g�J�n�E��~�̐؂�ւ��{�^��
	if (ImGui::Button(u8"�@��:[�L��]�E[����]"))
	{
		// �����ꂽ��Đ��ƒ�~�̂ǂ��炩�ɂ���
		MeshList::m_bNorSeting = MeshList::m_bNorSeting ? false : true;

		// ��������Ă�����
		if (MeshList::m_pMesh != NULL)
		{
			// �@���̌v�Z�����邩���Ȃ����ݒ肷��
			MeshList::m_pMesh->SetingbNor(MeshList::m_bNorSeting);
		}
	}

	// ������ɊԊu���󂯂�
	ImGui::SameLine(0.0f, 10.0f);

	// �e�L�X�g�\��
	if (MeshList::m_bNorSeting) { ImGui::Text(u8"�@�����R"); }
	else if (!MeshList::m_bNorSeting) { ImGui::Text(u8"�@���Œ�"); }

	// �e�N�X�`���̐ݒ�
	TextureMake::TextureLoad();
}
