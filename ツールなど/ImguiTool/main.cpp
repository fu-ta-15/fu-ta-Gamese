//*****************************************************************************
//
// メイン処理 [main.cpp]
// Author : SUZUKI FUUTA
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "keyinput.h"
#include "scene2D.h"
#include "manager.h"
#include "imguimanager.h"
//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define CLASS_NAME		"AppClass"								// ウインドウのクラス名
#define WINDOW_NAME		"メッシュポリゴンシュミレーター"		// ウインドウのキャプション名

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------
// グローバル変数:
//-----------------------------------------------------------------------------
#ifdef _DEBUG
int	g_nCountFPS;			// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

#ifdef _DEBUG
	// コンソールを作成する
	AllocConsole();
	// 標準入出力に割り当てる
	FILE* fp = NULL;
	// 現在のコード
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("%s", "知りたい情報\n");
#endif

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// managerのインスタンスを作成
	CManager *pManager = NULL;
	pManager = new CManager;

	if (pManager != NULL)
	{
		// 初期化処理
		if (FAILED(pManager->Init(hInstance, hWnd, true)))
		{
			return -1;
		}
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
		dwFrameCount = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 現在の時間を取得
			dwCurrentTime = timeGetTime();	

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
#ifdef _DEBUG
				// FPSを算出
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				// 現在の時間を保存
				dwFPSLastTime = dwCurrentTime;	

				// フレームカウントを初期化
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 1/60秒経過したら現在の時間を保存
				dwExecLastTime = dwCurrentTime;	

				// 更新処理
				pManager->Update();

				// 描画処理
				pManager->Draw();

				// フレームのカウントアップ
				dwFrameCount++;
			}
		}
	}

	if (pManager != NULL)
	{
		// マネージャーの開放
		pManager->Uninit();

		// マネージャーを削除
		delete pManager;

		// NULLを代入
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:			
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// FPSのゲット関数
//=============================================================================
#ifdef _DEBUG
int GetFps(void)
{
	return g_nCountFPS;
}
#endif
