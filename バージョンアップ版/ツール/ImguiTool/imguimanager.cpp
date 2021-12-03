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
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "camera.h"
#include "light.h"
#include "keyinput.h"
#include "TextureScene.h"


//=============================================================================
// ImGuiの初期化処理
//=============================================================================
HRESULT ImGuiMana::Init(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp, LPDIRECT3DDEVICE9 pD3DDevice)
{
	// Versionチェック
	IMGUI_CHECKVERSION();

	// テキスト生成とアクセス
	ImGui::CreateContext();

	// 情報取得
	ImGuiIO& io = ImGui::GetIO();

	// 言語とフォントの追加
	io.Fonts->AddFontFromFileTTF("imgui/Fonts/Gothic.ttc", 13.0f, nullptr, Japanese::glyphRangesJapanese);

	// 色のスタイル
	ImGui::StyleColorsDark();

	// ImGuiの初期化
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
	// DX9のフレーム生成
	ImGui_ImplDX9_NewFrame();

	// Win32のフレーム生成
	ImGui_ImplWin32_NewFrame();

	// フレームの生成
	ImGui::NewFrame();

	// メッシュ情報の更新処理
	MeshList::Update();

	// フレーム生成終了
	ImGui::EndFrame();
}

//=============================================================================
// レンダラー状態の管理
//=============================================================================
void ImGuiMana::DeviState(void)
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// ワイヤーフレームON・OFF
	pDevice->SetRenderState(D3DRS_FILLMODE, MeshList::m_fillmode);
	pDevice->SetRenderState(D3DRS_CULLMODE, MeshList::m_d3dcullmode);
}

//=============================================================================
// ImGuiの描画処理
//=============================================================================
void ImGuiMana::Draw(void)
{
	// デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// ワイヤーフレームの影響受けないためここで設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

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
// メッシュで使用する更新処理
//=============================================================================
void MeshList::Update(void)
{
	// 現在のモードの取得
	CManager::MODE mode = CManager::GetMode();

	// モード取得
	if (mode == CManager::MODE_GAME)
	{
		// Imguiウィンドウ生成
		ImGui::Begin(u8"ツールウィンドウ");

		// メッシュ処理
		MeshList::MeshInfo();

		// Imguiウィンドウ生成完了
		ImGui::End();
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
		// デモウィンドウの表示切替
		m_bDemo = m_bDemo ? false : true;
	}

	// trueの場合のみデモウィンドウ表示
	if (m_bDemo)
	{
		// デモウィンドウの生成
		ImGui::ShowDemoWindow();
	}

	// 同じラインに間隔をあける
	ImGui::SameLine(0.0f, 20.0f);

	// 説明書の表示切替Button
	if (ImGui::Button(u8"説明書"))
	{
		// 説明書の表示切替
		m_bExplanation = m_bExplanation ? false : true;
	}

	// trueの場合のみ説明書表示
	if (m_bExplanation)
	{
		// Imguiウィンドウ生成
		ImGui::Begin(u8"カメラやライトetc");

		// 設定
		Option::OperationExplanation();

		// Imguiウィンドウ生成完了
		ImGui::End();
	}

	// 空白の行を生成
	ImGui::Spacing();

	// メニュー欄の生成
	if (ImGui::CollapsingHeader(u8"メッシュポリゴンリスト"))
		{
			// メッシュの生成
			CreateMeshMenu();

			// メニュー欄の生成
			if (ImGui::CollapsingHeader(u8"表現方法"))
			{
				// 波の表現
				MeshList::MeshWave();

				// 空白の行を生成
				ImGui::Spacing(), ImGui::Spacing(), ImGui::Spacing();

				// 回転表現
				MeshList::MeshCycle();
			}

			// メニュー欄の生成
			if (ImGui::CollapsingHeader(u8"その他詳細"))
			{
				// メッシュの説明文
				MeshOptionMenu::MeshOption();
			}
		}
}

//=============================================================================
// メッシュポリゴンによる波の表現設定
//=============================================================================
void MeshList::MeshWave(void)
{
	// 波の表現の情報
	if (ImGui::TreeNode(u8"---波の表現---"))
	{
		// カウント開始・停止の切り替えボタン
		if (ImGui::Button(u8"波:[再生]・[停止]"))
		{
			// 押されたら再生と停止のどちらかにする
			m_bWaveStart = m_bWaveStart ? false : true;
		}

		// フレームカウント開始されていたら
		if (m_bWaveStart)
		{
			// カウントアップ
			m_nTime++;

			// 生成されていたら波の表現開始
			if (m_pMesh != NULL)
			{
				// 波の表現の開始
				m_pMesh->MeshWave(m_WaveCenter, m_nTime, m_fHeight, m_nCycle);
			}
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

		// ノード生成終了
		ImGui::TreePop();
	}
}

//=============================================================================
// メッシュポリゴンによる回転処理
//=============================================================================
void MeshList::MeshCycle(void)
{
	// 回転の表現
	if (ImGui::TreeNode(u8"---回転の表現---"))
	{
		// 回転開始・停止の切り替えボタン
		if (ImGui::Button(u8"回転:[再生]・[停止]"))
		{
			// true false の切り替え
			m_bCycleStart = m_bCycleStart ? false : true;
		}

		// 回転開始の合図が出ていたら
		if (m_bCycleStart)
		{
			// 生成されていたら波の表現開始
			if (m_pMesh != NULL)
			{
				// 回転運動
				m_pMesh->MeshCycleMove();
			}
		}

		// ノードの生成終了
		ImGui::TreePop();
	}
}

//=============================================================================
// メッシュ生成するための詳細設定
//=============================================================================
void MeshList::CreateMeshMenu(void)
{
	// 横に並べるための空白の準備
	float SpacingButton = ImGui::GetStyle().ItemInnerSpacing.x + 10.0f;

	// Buttonを改行せず生成
	ImGui::PushButtonRepeat(true);

	// メッシュの生成Button
	if (ImGui::Button(u8"[生成]"))
	{
		// NULLであれば生成する
		if (m_pMesh == NULL)
		{
			// （｛四角の中に入る｝縦の本数・横の本数・位置・サイズ）
			m_pMesh = CMesh3D::Create(m_nVertical, m_nSide, m_pos, m_size);
		}

		// NULLであれば生成する
		if (m_pTextureList == NULL)
		{
			m_pTextureList = new CTextureScene;
		}
	}

	// 空白の生成
	ImGui::SameLine(0.0f, SpacingButton);

	// メッシュの削除ボタン
	if (ImGui::Button(u8"[削除]"))
	{
		// NULLでなければ削除する
		if (m_pMesh != NULL)
		{
			// メッシュの開放
			m_pMesh->Uninit();

			// NULL代入
			m_pMesh = NULL;
		}

		m_nVertical = 0;							// 縦線の本数
		m_nSide = 0;								// 横線の本数
		m_pos = ZeroVector3;						// 位置
		m_size = D3DXVECTOR3(100.0f, 0.0f, 100.0f); // サイズ
	}

	// 空白の生成
	ImGui::SameLine(0.0f, SpacingButton);

	// メッシュの変更ボタン
	if (ImGui::Button(u8"[変更]"))
	{
		// NULLでなければ削除する
		if (m_pMesh != NULL)
		{
			// メッシュの開放
			m_pMesh->Uninit();

			// NULL代入
			m_pMesh = NULL;

			// （｛四角の中に入る｝縦の本数・横の本数・位置・サイズ）
			m_pMesh = CMesh3D::Create(m_nVertical, m_nSide, m_pos, m_size);
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

	// 線の追加ノード
	if (ImGui::TreeNode(u8"線の追加"))
	{
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

		// ノードの生成終了
		ImGui::TreePop();
	}

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
}

//=============================================================================
// テクスチャのロード
//=============================================================================
void TextureMake::TextureLoad(void)
{
	// キー入力クラスのポインタ
	CKey *pKey = CManager::GetKey();

	if (ImGui::TreeNode(u8"テクスチャ生成"))
	{
		if (ImGui::InputText(u8"テクスチャ名", cName, IM_ARRAYSIZE(cName)))
		{

		}
		else if (pKey->GetState(CKey::STATE_TRIGGER,DIK_RETURN))
		{
			MeshList::m_pTextureList->CreateTexture(&cName[0]);
		}

		ImGui::Text(u8"リンク先：%s", cLink);

		// ノードの終了
		ImGui::TreePop();
	}
}

//=============================================================================
// テクスチャの適用
//=============================================================================
void TextureMake::TextureBind(void)
{
}

//=============================================================================
// ライトの移動
//=============================================================================
void LightOption::LightMove(void)
{
	// ライトの情報取得
	CLight *pLight = CManager::GetLight();
}

//=============================================================================
// カメラの操作説明
//=============================================================================
void Option::OperationExplanation(void)
{
	// カメラ操作説明用ノード生成
	if (ImGui::TreeNode(u8"カメラ操作"))
	{
		// 操作説明テキストの表示
		ImGui::Text(u8"移動方法");
		ImGui::Text(u8"W/前 : S/後 : A/右 : D/左");
		ImGui::Text(u8"T/注視点の上移動：B/注視点の下移動：Y/視点の上移動：N/視点の下移動");

		// 空白の行を生成
		ImGui::Spacing(), ImGui::Spacing(), ImGui::Spacing();

		// 操作説明テキストの表示
		ImGui::Text(u8"旋回");
		ImGui::Text(u8"Q/視点の左旋回 : E/視点の右旋回");
		ImGui::Text(u8"Z/注視点の左旋回 : C/注視点の右旋回");

		// ノードの終了
		ImGui::TreePop();
	}
}

//=============================================================================
// メッシュポリゴンの詳細設定
//=============================================================================
void MeshOptionMenu::MeshOption(void)
{
	// フレームモードの変更
	if (ImGui::Button(u8"フレームモード"))
	{
		// フレームの種類
		switch (MeshList::m_fillmode)
		{
		case D3DFILL_SOLID:

			// ワイヤーフレームでの表示
			MeshList::m_fillmode = D3DFILL_WIREFRAME;
			break;
		case D3DFILL_WIREFRAME:
		
			// ポリゴンとして描画
			MeshList::m_fillmode = D3DFILL_SOLID;
			break;

		default:
			break;
		}
	}
	
	// 同じ列に間隔を空ける
	ImGui::SameLine(0.0f, 10.0f);

	// テキスト表示
	if (MeshList::m_fillmode == D3DFILL_WIREFRAME) { ImGui::Text(u8"ワイヤーフレーム"); }
	else if (MeshList::m_fillmode == D3DFILL_SOLID) { ImGui::Text(u8"デフォルトフレーム"); }

	// カリングモードの変更
	if (ImGui::Button(u8"カリングモード"))
	{
		// カリング方法の種類
		switch (MeshList::m_d3dcullmode)
		{
		case D3DCULL_CCW:

			// カリングOFF（裏面も描画）
			MeshList::m_d3dcullmode = D3DCULL_NONE;
			break;

		case D3DCULL_NONE:

			// カリングON（裏面描画なし)
			MeshList::m_d3dcullmode = D3DCULL_CCW;
			break;

		default:
			break;
		}
	}

	// 同じ列に間隔を空ける
	ImGui::SameLine(0.0f, 10.0f);

	// テキスト表示
	if (MeshList::m_d3dcullmode == D3DCULL_NONE) { ImGui::Text(u8"カリングOFF"); }
	else if (MeshList::m_d3dcullmode == D3DCULL_CCW) { ImGui::Text(u8"カリングON"); }

	// カウント開始・停止の切り替えボタン
	if (ImGui::Button(u8"法線:[有り]・[無し]"))
	{
		// 押されたら再生と停止のどちらかにする
		MeshList::m_bNorSeting = MeshList::m_bNorSeting ? false : true;

		// 生成されていたら
		if (MeshList::m_pMesh != NULL)
		{
			// 法線の計算をするかしないか設定する
			MeshList::m_pMesh->SetingbNor(MeshList::m_bNorSeting);
		}
	}

	// 同じ列に間隔を空ける
	ImGui::SameLine(0.0f, 10.0f);

	// テキスト表示
	if (MeshList::m_bNorSeting) { ImGui::Text(u8"法線自由"); }
	else if (!MeshList::m_bNorSeting) { ImGui::Text(u8"法線固定"); }

	// テクスチャの設定
	TextureMake::TextureLoad();
}
