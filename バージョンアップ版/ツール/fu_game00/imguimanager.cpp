//*****************************************************************************
//
// ImGuiマネージャ処理 [imguimanager.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "imguimanager.h"
#include "imgui_ja_gryph_ranges.h"
#include "game.h"
#include "manager.h"
#include "camera.h"
#include "light.h"

//=============================================================================
// ImGuiの初期化処理
//=============================================================================
HRESULT ImGuiMana::Init(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9 pD3DDevice)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("imgui/Fonts/Gothic.ttc", 14.0f, nullptr, Japanese::glyphRangesJapanese);
	ImGui::StyleColorsLight();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pD3DDevice);

	return S_OK;
}

//=============================================================================
// ImGuiの終了処理
//=============================================================================
void ImGuiMana::Uninit(void)
{
	// DX9用Imguiのシャットダウン
	ImGui_ImplDX9_Shutdown();

	// Win32Imguiのシャットダウン
	ImGui_ImplWin32_Shutdown();

	// テキストの開放
	ImGui::DestroyContext();
}

//=============================================================================
// ImGuiの更新処理
//=============================================================================
void ImGuiMana::Update(void)
{
	// 現在のモードの取得
	CManager::MODE mode = CManager::GetMode();

	// DX9のフレーム生成
	ImGui_ImplDX9_NewFrame();

	// Win32のフレーム生成
	ImGui_ImplWin32_NewFrame();

	// フレームの生成
	ImGui::NewFrame();

	// モード取得
	if (mode == CManager::MODE_GAME)
	{
		// Imguiウィンドウ生成
		ImGui::Begin(u8"メッシュポリゴン");

		// メッシュ処理
		MeshList::MeshInfo();

		// Imguiウィンドウ生成完了
		ImGui::End();
	}

	// Imguiウィンドウ生成
	ImGui::Begin(u8"カメラやライトetc");

	Option::OperationExplanation();

	// Imguiウィンドウ生成完了
	ImGui::End();

	// フレーム生成終了
	ImGui::EndFrame();
}


//=============================================================================
// ImGuiの描画処理
//=============================================================================
void ImGuiMana::Draw(void)
{
	// DX9でのレンダラー開始
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=============================================================================
// ImGuiの描画終了処理
//=============================================================================
void ImGuiMana::DrawEnd(HRESULT result, LPDIRECT3DDEVICE9 pD3DDevice, D3DPRESENT_PARAMETERS d3dpp)
{
	// 以下による条件がそろっていたら
	if (result == D3DERR_DEVICELOST && pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		// DX９で使用していたオブジェクトやデバイスの破棄
		ImGui_ImplDX9_InvalidateDeviceObjects();
		HRESULT hr = pD3DDevice->Reset(&d3dpp);
		if (hr == D3DERR_INVALIDCALL)
			IM_ASSERT(0);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

//=============================================================================
// ImGuiのメッシュポリゴンの情報
//
// ・生成設定/波の表現の設定/その他設定…
//=============================================================================
void MeshList::MeshInfo(void)
{
	// デモウィンドウの表示切替Button
	if (ImGui::Button(u8"[デモウィンドウの生成]"))
	{
		// デモウィンドウの表示切り替え
		m_bDemo = m_bDemo ? false : true;
	}

	// trueの場合のみデモウィンドウ表示
	if (m_bDemo)
	{
		// デモウィンドウの生成
		ImGui::ShowDemoWindow();
	}

	// 空白の行を生成
	ImGui::Spacing();

	// メッシュの情報ノード
	if (ImGui::TreeNode(u8"---情報---"))
	{

		// 生成する際の情報
		if (ImGui::TreeNode(u8"---生成---"))
		{
			// メッシュの生成
			CreateMeshMenu();

			// ノード生成終了
			ImGui::TreePop();
		}

		// 空白の行を生成
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// 波の表現の情報
		if (ImGui::TreeNode(u8"---波の表現---"))
		{
			// 波の表現
			MeshWave();

			// ノード生成終了
			ImGui::TreePop();
		}

		// 空白の行を生成
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		// その他の詳細
		if (ImGui::TreeNode(u8"---メッシュポリゴン詳細---"))
		{
			

			// ノード生成終了
			ImGui::TreePop();
		}

		// ノード生成終了
		ImGui::TreePop();
	}
}

//=============================================================================
// メッシュポリゴンによる波の表現設定
//=============================================================================
void MeshList::MeshWave(void)
{
	// カウント開始・停止の切り替えボタン
	if (ImGui::Button(u8"[再生]・[停止]"))
	{
		// 押されたら再生と停止のどちらかにする
		m_bTimeStart = m_bTimeStart ? false : true;
	}

	// フレームカウント開始されていたら
	if (m_bTimeStart)
	{
		// カウントアップ
		m_nTime++;
	}

	// 生成されていたら波の表現開始
	if (m_pMesh != NULL)
	{
		m_pMesh->MeshWave(m_WaveCenter, m_nTime, m_fHeight, m_nCycle);
	}

	// 波の詳細設定ノード
	if (ImGui::TreeNode(u8"---波の詳細設定---"))
	{
		// 波θからの高さ
		ImGui::InputFloat(u8"高さ", &m_fHeight, 0.5f);

		// 波の周期の長さ
		ImGui::InputInt(u8"周期", &m_nCycle, 1);

		// 波を起こす原点の設定ノード
		if (ImGui::TreeNode(u8"震源"))
		{
			// 波を起こす原点の設定
			ImGui::InputFloat(u8"X座標", &m_WaveCenter.x, 0.5f);
			ImGui::InputFloat(u8"Z座標", &m_WaveCenter.z, 0.5f);

			// ノードの終了
			ImGui::TreePop();
		}

		// フレームカウント表示ノード
		if (ImGui::TreeNode("FRAME"))
		{
			// フレームの表示
			ImGui::Text(u8" カウント数 : %d", m_nTime);

			// ノードの終了
			ImGui::TreePop();
		}

		// ノードの終了
		ImGui::TreePop();
	}
}

//=============================================================================
// メッシュ生成するための詳細設定
//=============================================================================
void MeshList::CreateMeshMenu(void)
{
	// 横に並べるための空白の準備
	float SpacingButton = ImGui::GetStyle().ItemInnerSpacing.x;

	// Buttonを改行せず生成
	ImGui::PushButtonRepeat(true);

	// メッシュの生成Button
	if (ImGui::Button(u8"[生成ボタン]"))
	{
		// NULLであれば生成する
		if (m_pMesh == NULL)
		{// NULLチェック

			// （｛四角の中に入る｝縦の本数・横の本数・位置・サイズ）
			m_pMesh = CMesh3D::Create(m_nVertical, m_nSide, m_pos, m_size);
		}
	}

	// 空白の生成
	ImGui::SameLine(0.0f, SpacingButton);

	// メッシュの削除ボタン
	if (ImGui::Button(u8"[削除ボタン]"))
	{
		// NULLでなければ削除する
		if (m_pMesh != NULL)
		{// NULLチェック

			// メッシュの開放
			m_pMesh->Uninit();

			// NULL代入
			m_pMesh = NULL;
		}
	}

	// 空白の生成
	ImGui::SameLine(0.0f, SpacingButton);

	// 設定していた数値をリセット
	if (ImGui::Button(u8"[数値リセット]"))
	{
		m_nVertical = 0;	  // 縦線の本数
		m_nSide = 0;		  // 横線の本数
		m_pos = ZeroVector3;  // 位置
		m_size = ZeroVector3; // サイズ
	}

	// 繰り返し表示の終了
	ImGui::PopButtonRepeat();

	// 現在の数値表示・数値入力可能・ボタンで１±可能
	ImGui::InputInt(u8"縦線", &m_nVertical, 1);

	// 限界値を越えていたら戻す処理
	if (m_nVertical < 0) { m_nVertical = 0; }
	else if (m_nVertical > 150) { m_nVertical = 150; }

	// 現在の数値表示・数値入力可能・ボタンで１±可能
	ImGui::InputInt(u8"横線", &m_nSide, 1);

	// 限界値を越えていたら戻す処理
	if (m_nSide < 0) { m_nSide = 0; }
	else if (m_nSide > 150) { m_nSide = 150; }

	// メッシュの位置設定ノード
	if (ImGui::TreeNode(u8"---位置---"))
	{
		// メッシュ生成開始の位置設定
		ImGui::InputFloat(u8"X座標", &m_pos.x, 0.5f);
		ImGui::InputFloat(u8"Y座標", &m_pos.y, 0.5f);
		ImGui::InputFloat(u8"Z座標", &m_pos.z, 0.5f);

		// ノードの終了
		ImGui::TreePop();
	}

	// メッシュのサイズ設定ノード
	if (ImGui::TreeNode(u8"---サイズ---"))
	{
		// メッシュの縦と横の設定
		ImGui::InputFloat(u8"横サイズ", &m_size.x, 0.5f);
		ImGui::InputFloat(u8"縦サイズ", &m_size.z, 0.5f);

		// ノードの終了
		ImGui::TreePop();
	}

	TextureMake::TextureLoad();
}

void TextureMake::TextureLoad(void)
{

	if (ImGui::TreeNode(u8"テクスチャ生成"))
	{




		// ノードの終了
		ImGui::TreePop();
	}
}

void TextureMake::TextureBind(void)
{
}


void LightOption::LightMove(void)
{
	// ライトの情報取得
	CLight *pLight = CManager::GetLight();

}

void Option::OperationExplanation(void)
{
	if (ImGui::TreeNode(u8"カメラ操作"))
	{
		ImGui::Text(u8"移動方法");
		ImGui::Text(u8"W/前 : S/後 : A/右 : D/左");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text(u8"旋回");
		ImGui::Text(u8"Q/視点の左旋回 : E/視点の右旋回");
		ImGui::Text(u8"Z/注視点の左旋回 : C/注視点の右旋回");

		// ノードの終了
		ImGui::TreePop();
	}
}
