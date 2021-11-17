//##################################################################################################################################################################//
//
// プレイヤー処理 [player.cpp]
// Author : SUZUKI FUUTA
//
//##################################################################################################################################################################//
#include "main.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "tutorial.h"
#include "ranking.h"


//=====================================================================================================================================================================//
//マクロ定義
//=====================================================================================================================================================================//
#define ID_TIMER		(121)
#define TIMER_INTERVAL	(1000/60)
#define VERTEX_THREE	(4)

//=====================================================================================================================================================================//
//グローバル宣言
//=====================================================================================================================================================================//
LPDIRECT3D9 g_pD3D = NULL;							//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;				//Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;							//フォントへのポインタ
int g_nCountFPS;									//FPSカウンタ
MODE g_mode = MODE_TITLE;							//モードの種類

//=====================================================================================================================================================================//
//プロトタイプ宣言
//=====================================================================================================================================================================//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //情報
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//=====================================================================================================================================================================//
//メイン関数
//=====================================================================================================================================================================//
int WINAPI WinMain
(HINSTANCE hInstance,		//インスタンスハンドル
	HINSTANCE hPrevInstance,	//常にNULL
	LPSTR lpCmdLine,			//コマンドラインから受け取った引数へのポインタ
	int nCmdShow)				//初期ウィンドウの表示方法の指定
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		"windows.suzuki", //ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),
	};
	HWND hWnd;				//ウィンドウハンドル（識別子）
	MSG msg;				//Message

	//処理のはじめ
	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);	//登録
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの作成
	hWnd = CreateWindowEx
	(0,
		"windows.suzuki",		//見えない・ウィンドウクラスの名前
		"start",				//見える・ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	//ウィンドウスタイル
		CW_USEDEFAULT,			//ウィンドウの左上X座標
		CW_USEDEFAULT,			//ウィンドウの左上Y座標
		rect.right - rect.left,				//右の座標
		rect.bottom - rect.top,			//下の座標
		NULL,
		NULL,
		hInstance,
		NULL);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, FALSE)))
	{
		return -1;
	}

	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	DWORD dwExecLastTime;	//処理実行最終時刻
	DWORD dwFPSLastTime;	//FPS計測最終時刻
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwFrameCount;		//フレームカウント

	dwExecLastTime = 
	dwFPSLastTime = 
	dwCurrentTime = timeGetTime();	//現在時刻を設定
	dwFrameCount = 0;

	//Messageのループ（ウィンドウズ専用の処理）
	while (1)
	{//Messageの翻訳と送出
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理

			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	//終了処理
	Uninit();

	//ウィンドウクラスの登録解除
	UnregisterClass("windows.suzuki", wcex.hInstance);

	return(int)msg.wParam;	//返り値
}

//=====================================================================================================================================================================//
//ウィンドウプロシージャ
//=====================================================================================================================================================================//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{//メッセージのやり取り
	case WM_DESTROY:			//ウィンドウを破棄する
		PostQuitMessage(0);		//WM_QUITメッセージを返す
		break;

	case WM_CLOSE:				//×ボタンが押された場合
		nID = MessageBox(NULL, "確認です。", "確認", MB_YESNOCANCEL | MB_ICONHAND);
		if (nID == IDYES)
		{//YESが押された場合
			DestroyWindow(hWnd);//メッセージを送る
		}
		else
		{
			return 0;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);	//返り値
}

//=====================================================================================================================================================================//
//初期化処理
//=====================================================================================================================================================================//
HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//ディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;		//ゲーム画面の幅
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		//高さ
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dオブジェクトの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//フォントオブジェクトの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				//1
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//2
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//3
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//4
																			/*  1.カリングの設定（　　　,裏面をカリング）
																				2.αブレンド（α値の合成）の設定＊重要
																				3.ソース（描画元）の合成方法の設定
																				4.デスティネーション（描画先）の合成方法の設定
																			　　　							画像の透明度が反映　*/
	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//縮小時　補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	//拡大時　補間
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//U値　繰り返し
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//V値　繰り返し

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化処理
	InitKeyboard(hInstance, hWnd);

	//サウンドの初期化処理
	InitSound(hWnd);

	//モードの設定
	SetMode(g_mode);

	// フェードモード
	InitFade(g_mode);



	return S_OK;
}

//=====================================================================================================================================================================//
//終了処理
//=====================================================================================================================================================================//
void Uninit(void)
{

	//キーボードの終了処理
	UninitKeyboard();

	//フェードの終了処理
	UninitFade();

	//各モードの終了処理
	switch (g_mode)
	{
		// タイトル
	case MODE_TITLE:
		UninitTitle();
		break;

		// ゲーム
	case MODE_GAME:
		UninitGame();
		break;

		// リザルト
	case MODE_RESULT:
		UninitResult();
		break;

		// チュートリアル
	case MODE_TUTORIAL:
		UninitTutorial();
		break;

		// ランキング
	case MODE_RANKING:
		UninitRanking();
		break;

	default:
		break;
	}

	//サウンドの終了処理
	UninitSound();

	//Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	//Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	//フォントの開放
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
}

//=====================================================================================================================================================================//
//更新処理
//=====================================================================================================================================================================//
void Update(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//各モードの更新処理
	switch (g_mode)
	{
		// タイトル
	case MODE_TITLE:
		UpdateTitle();
		break;

		// ゲーム
	case MODE_GAME:
		UpdateGame();
		break;

		// リザルト
	case MODE_RESULT:
		UpdateResult();
		break;

		// チュートリアル
	case MODE_TUTORIAL:
		UpdateTutorial();
		break;

		// ランキング
	case MODE_RANKING:
		UpdateRanking();
		break;

	default:
		break;
	}

	//フェードの更新処理
	UpdateFade();
}

//=====================================================================================================================================================================//
//描画処理
//=====================================================================================================================================================================//
void Draw(void)
{
	//バックバッファ＆Zバッファのクリア（画面のクリア）
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(225, 225, 225, 0), 1.0f, 0);

	//描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//各モードの描画処理
		switch (g_mode)
		{
			// タイトル
		case MODE_TITLE:
			DrawTitle();
			break;

			// ゲーム
		case MODE_GAME:
			DrawGame();
			break;

			// リザルト
		case MODE_RESULT:
			DrawResult();
			break;

			// チュートリアル
		case MODE_TUTORIAL:
			DrawTutorial();
			break;

			// ランキング
		case MODE_RANKING:
			DrawRanking();
			break;

		default:
			break;
		}

		//フェードの描画処理
		DrawFade();

#ifdef _DEBUG

		//FPSの描画処理
		DrawFPS();

#endif // _DEBUG


		//描画の終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=====================================================================================================================================================================//
//FPSの描画処理
//=====================================================================================================================================================================//
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);
	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

void SetMode(MODE mode)
{
	switch (g_mode)
	{
		// タイトル
	case MODE_TITLE:
		UninitTitle();
		break;

		// ゲーム
	case MODE_GAME:
		UninitGame();
		break;

		// リザルト
	case MODE_RESULT:
		UninitResult();
		break;

		// チュートリアル
	case MODE_TUTORIAL:
		UninitTutorial();
		break;

		// ランキング
	case MODE_RANKING:
		UninitRanking();
		break;

	default:
		break;
	}

	g_mode = mode;

	switch (mode)
	{
		// タイトル
	case MODE_TITLE:
		InitTitle();
		break;

		// ゲーム
	case MODE_GAME:
		InitGame(mode);
		break;

		// リザルト
	case MODE_RESULT:
		InitResult();
		break;

		// チュートリアル
	case MODE_TUTORIAL:
		InitTutorial(mode);
		break;

		// ランキング
	case MODE_RANKING:
		InitRanking();
		break;

	default:
		break;
	}


}


//=====================================================================================================================================================================//
//デバイスの取得
//=====================================================================================================================================================================//
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=====================================================================================================================================================================//
//モードの取得
//=====================================================================================================================================================================//
MODE GetMode(void)
{
	return g_mode;
}
