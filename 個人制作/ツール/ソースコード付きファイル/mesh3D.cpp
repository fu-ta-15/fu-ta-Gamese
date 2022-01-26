//#############################################################################
//
// メッシュ処理 [mesh.cpp]
// Author : SUZUKI FUUTA
//
//#############################################################################

//-----------------------------------------------------------------------------
//　インクルードファイル
//-----------------------------------------------------------------------------
#include "mesh3D.h"
#include "Dinput.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMesh3D::CMesh3D(Priority type) : CScene(type)
{
	// メンバ変数の初期化
	m_pVtxBuff = NULL;
	m_pTexture = NULL;
	m_pIdxBuff = NULL;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveRot = D3DXVECTOR3(180, 180, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CMesh3D::~CMesh3D()
{

}

//=============================================================================
// メッシュの生成
//=============================================================================
CMesh3D * CMesh3D::Create(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	// ポインタ変数
	CMesh3D *pMesh = NULL;

	// NULLチェック
	if (pMesh == NULL)
	{
		pMesh = new CMesh3D(OBJ_NONE);	 // インスタンス生成
		pMesh->SetPos(pos);				 // 位置
		pMesh->SetSize(size);			 // サイズ
		pMesh->SetSide(nSide);			 // 横線
		pMesh->SetVertical(nVertical);	 // 縦線
		pMesh->Init();					 // 初期化
	}

	return pMesh;
}

//=============================================================================
// メッシュの生成
//=============================================================================
CMesh3D * CMesh3D::Create(const int nVertical, const int nSide, const D3DXVECTOR3 pos, const D3DXVECTOR3 size, Priority nPriority)
{
	// ポインタ変数
	CMesh3D *pMesh = NULL;

	// 情報を代入
	if (pMesh == NULL)
	{// NULLチェック
		pMesh = new CMesh3D(nPriority);
		pMesh->SetPos(pos);
		pMesh->SetSize(size);
		pMesh->SetSide(nSide);
		pMesh->SetVertical(nVertical);
		pMesh->Init();
	}

	return pMesh;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
HRESULT CMesh3D::CreateTexture(const LPCSTR pSrcFile)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pSrcFile, &m_pTexture);

	return S_OK;
}

//=============================================================================
// メッシュポリゴンの初期化処理
//=============================================================================
HRESULT CMesh3D::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// 総合頂点数
	m_nVtx = VertexCreate(m_nVertical, m_nSide);			

	// 総合インデックス
	m_nIdx = IndexCreate(m_nVertical, m_nSide);				

	// 一列の縦線の数
	int nVertical = (m_nVertical + 2);

	// 一列の横線の数
	int nSide = (m_nSide + 2);

	// 配列を動的に確保
	m_Vector = new D3DXVECTOR3*[nVertical];

	// 配列を動的に確保
	for (int nSize = 0; nSize < nVertical; nSize++)
	{
		m_Vector[nSize] = new D3DXVECTOR3[nSide];
	}

	// 配列を二次元配列に確保
	m_Nor = new D3DXVECTOR3[(((1 + m_nVertical) * 2) * (1 + m_nSide))];

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer((sizeof(VERTEX_3D) * m_nVtx),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL))) {
		return E_FAIL;
	}

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 位置の設定
	MeshSetPos(m_nVertical, m_nSide, m_pVtx);		

	// 除算数 1.0fで固定								
	MeshSetNor(m_pVtx);							

	// 色の設定
	MeshSetCol(m_pVtx);							

	// テクスチャの頂点座標の設定
	MeshSetTex(m_nVertical, m_nSide, m_pVtx);		

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nIdx-2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックス情報へのポインタ
	WORD *pIdx;	

	// インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// メッシュインデックス
	MeshCreate(m_nVertical, m_nSide, pIdx);

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();

	return S_OK;	
}

//=============================================================================
// メッシュポリゴンの終了処理
//=============================================================================
void CMesh3D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// インデックスバッファの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 削除
	Release();
}

//=============================================================================
// メッシュポリゴンの更新処理
//=============================================================================
void CMesh3D::Update(void)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	if (m_bNor)
	{
		// 法線ベクトルの計算と反映
		MeshNor();
	}
	else
	{
		// 法線の初期化1.0固定
		MeshSetNor(m_pVtx);
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// メッシュポリゴンの描画処理
//=============================================================================
void CMesh3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;	

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0, 0,
		m_nVtx, 0, (m_nIdx - 2));
}

//=============================================================================
// メッシュを揺らす処理
//=============================================================================
void CMesh3D::MeshWave(const D3DXVECTOR3& center, int ntime, float fHeight, int nCycle)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 各頂点の座標
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		// 二つの点の距離を計算
		float Dicetan = getDistance(center.x, center.z, m_pVtx[nCnt].pos.x, m_pVtx[nCnt].pos.z);

		// 滑らかになるように高さをそれぞれ更新
		m_pVtx[nCnt].pos.y = (fHeight) * sinf((2.0f * D3DX_PI) / nCycle * (ntime - Dicetan));
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// メッシュの回転
//=============================================================================
void CMesh3D::MeshCycleMove(void)
{
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 半径の保存変数
	float fRadiusX = 0.0f, fRadiusY = 0.0f;

	// 加算される値を保存する変数
	float fAddX = 0.0f, fAddY = 0.0f;

	// カウント用変数
	int nCntVertical = 0, nCntSide = 0;

	// 一列の最後の頂点の番号
	int nNumVertical = (1 + m_nVertical), nNumSide = (1 + m_nSide);
		
	// 各頂点の位置の更新
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		// 半径の計算
		fRadiusX = m_moveRot.x * D3DX_PI / 360;
		fRadiusY = m_moveRot.y * D3DX_PI / 360;

		// 回転させるためのSIN・COSの計算
		fAddX = sinf(fRadiusX) * ((m_size.x / (nNumVertical)) * nCntVertical);
		fAddY = cosf(fRadiusY) * ((m_size.x / (nNumVertical)) * nCntVertical);

		// 位置の更新
		m_pVtx[nCnt].pos = D3DXVECTOR3(m_pos.x + fAddX, fAddY, (m_pos.z - ((m_size.z / nNumSide) * nCntSide)));

		// 端っこまで来たら
		if (nCntVertical == nNumVertical)
		{
			// 下に一つ進む
			nCntSide += 1;

			// ０からのスタート
			nCntVertical = 0;
		}
		else
		{
			// 横に進める
			nCntVertical += 1;
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	// 角度を加算
	m_moveRot.x--, m_moveRot.y--;
}

//=============================================================================
// メッシュポリゴンの生成準備　頂点に番号振り分け
//=============================================================================
HRESULT CMesh3D::MeshCreate(int nVertical, int nSide, WORD * pIdx)
{
	// カウント用変数
	int nT = 0, nCnt = 0;

	// 頂点のカウント変数
	int nCntVertical = (2 + nVertical), nCntSide = (2 + nSide);

	// 折り返しの数値
	int nWrapBack = 2 + nVertical;

	for (nCnt = 0; nCnt < m_nIdx; nCnt++)
	{
		if ((nCnt % 2) == 0)
		{
			if (nCntVertical != 0)
			{// 四角の上底の番号
				pIdx[nCnt] = nT + nWrapBack;
			}
			else
			{// 縮退ポリゴン用の番号
				pIdx[nCnt] = nT - 1;
			}
		}
		else if ((nCnt % 2) == 1)
		{
			if (nCntVertical != 0)
			{// 四角の下底の番号
				pIdx[nCnt] = nT;

				// カウントアップ
				nT += 1;

				// 横に一つ進む
				nCntVertical -= 1;
			}
			else
			{// 縮退ポリゴン用の番号
				pIdx[nCnt] = nT + nWrapBack;

				// 縦に一つ進む
				nCntSide -= 1;

				// カウントを初期化
				nCntVertical = ((2 + nVertical));
			}
		}
	}

	return S_OK;	
}

//=============================================================================
// メッシュポリゴンのテクスチャ座標設定
//=============================================================================
void CMesh3D::MeshSetTex(int nVertical, int nSide, VERTEX_3D * pVtx)
{
	// 一列の最後の値のナンバー
	float nNumVertical = (1.0f + (float)nVertical), nNumSide = (1.0f + (float)nSide);

	// カウント用変数
	float nCntVertical = 0.0f, nCntSide = 0.0f;

	// テクスチャ座標振り分け
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++, pVtx++)
	{	// 座標の代入
		pVtx[0].tex = D3DXVECTOR2((float)(nCntVertical / (nNumVertical)), (float)(nCntSide / nNumSide));

		// 端っこまで来たら
		if (nCntVertical == nNumVertical)
		{
			// 下に一つ進む
			nCntSide += 1;

			// ０からのスタート
			nCntVertical = 0;
		}
		else
		{
			// 横に進める
			nCntVertical += 1;
		}
	}
}

//=============================================================================
// メッシュポリゴンの位置情報の設定
//=============================================================================
void CMesh3D::MeshSetPos(int nVertical, int nSide, VERTEX_3D * pVtx)
{
	// 一列の最後の値のナンバー
	int nNumVertical = (1 + nVertical), nNumSide = (1 + nSide);

	// カウント用変数
	int nCntVertical = 0, nCntSide = 0;

	// 各頂点の座標
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		// 各頂点に座標を代入
		pVtx[nCnt].pos = D3DXVECTOR3((m_pos.x + ((m_size.x / nNumVertical) * nCntVertical)),0.0f, (m_pos.z - ((m_size.z / nNumSide) * nCntSide)));

		// 端っこまで来たら
		if (nCntVertical == nNumVertical)
		{
			// 下に一つ進む
			nCntSide += 1;

			// ０からのスタート
			nCntVertical = 0;
		}
		else
		{
			// 横に進める
			nCntVertical += 1;
		}
	}
}

//=============================================================================
// メッシュポリゴンの法線の向き
//=============================================================================
void CMesh3D::MeshSetNor(VERTEX_3D * pVtx)
{
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++, pVtx++)
	{
		// 各頂点に法線ベクトル代入
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
}

//=============================================================================
// メッシュポリゴンの色設定
//=============================================================================
void CMesh3D::MeshSetCol(VERTEX_3D * pVtx)
{
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++, pVtx++)
	{
		// 各頂点にカラー・赤・緑・青・アルファを反映
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
}

//=============================================================================
// メッシュポリゴンの頂点の計算
//=============================================================================
int CMesh3D::VertexCreate(int nVertical, int nSide)
{
	// ポリゴンの総合頂点数
	return ((nVertical + 2) * (nSide + 2));
}

//=============================================================================
// メッシュポリゴンの描画番号の計算
//=============================================================================
int CMesh3D::IndexCreate(int nVertical, int nSide)
{
	// 描画に必要な総合の数
	return ((((nVertical + 2) * (nSide + 1)) * 2) + ((nSide + 1) * 2));
}

//=============================================================================
// メッシュポリゴンの外積による法線ベクトルの計算と合成
//=============================================================================
void CMesh3D::MeshNor(void)
{
	// 一列の最大頂点
	int nNumVertical = (1 + m_nVertical), nNumSide = (1 + m_nSide);

	// カウント用変数
	int	nCntVertical = 0, nCntSide = 0;

	// 法線の情報が必要になる頂点の数
	int	nNumNor = ((nNumVertical * 2) * nNumSide);

	// 座標の保存
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		// 座標を二次元配列に保存
		m_Vector[nCntVertical][nCntSide] = m_pVtx[nCnt].pos;

		// 端っこまで来たら
		if (nCntVertical == nNumVertical)
		{
			// 下に一つ進む
			nCntSide += 1;

			// ０からのスタート
			nCntVertical = 0;
		}
		else
		{
			// 横に進める
			nCntVertical += 1;
		}
	}

	// 数値リセット
	nCntSide = 0, nCntVertical = 0;

	// 各ポイントの法線を計算
	for (int nCntNor = 0; nCntNor < nNumNor; nCntNor += 1)
	{
		if ((nCntNor % 2) == 0)
		{
			m_Nor[nCntNor] = PolygonNormal(m_Vector[nCntVertical][nCntSide + 1], m_Vector[nCntVertical][nCntSide], m_Vector[nCntVertical + 1][nCntSide + 1]);
		}
		else if ((nCntNor % 2) == 1)
		{
			m_Nor[nCntNor] = PolygonNormal(m_Vector[nCntVertical + 1][nCntSide], m_Vector[nCntVertical + 1][nCntSide + 1], m_Vector[nCntVertical][nCntSide]);

			if (nCntVertical == (nNumVertical - 1))
			{
				nCntSide += 1;
				nCntVertical = 0;
			}
			else
			{
				nCntVertical += 1;
			}
		}
	}

	// 数値リセット
	nCntSide = 0, nCntVertical = 0;

	// 法線ベクトルの合成
	for (int nCnt = 0; nCnt < m_nVtx; nCnt++)
	{
		if (nCntSide == 0)
		{// 一番上のパターン
			if (nCntVertical == 0)
			{
				m_pVtx[nCnt].nor = m_Nor[0] + m_Nor[1];
				m_pVtx[nCnt].nor = m_pVtx[nCnt].nor / 2;
			}
			else if (nCntVertical == nNumVertical)
			{
				m_pVtx[nCnt].nor = m_Nor[(nCntVertical * 2) - 1];
			}
			else
			{
				m_pVtx[nCnt].nor = m_Nor[((nNumVertical * 2) * nCntSide) + (nCntVertical * 2) - 1] + m_Nor[((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)] + m_Nor[((nNumVertical * 2) * nCntSide) + (nCntVertical * 2) + 1];
				m_pVtx[nCnt].nor = m_pVtx[nCnt].nor / 3;
			}
		}
		else if (nCntSide != 0 && nCntSide != nNumSide)
		{// 真ん中のパターン
			if (nCntVertical == 0)
			{
				m_pVtx[nCnt].nor = m_Nor[(((nNumVertical * 2) * nCntSide) - (nNumVertical * 2))] + m_Nor[((nNumVertical * 2) * nCntSide)] + m_Nor[(((nNumVertical * 2) * nCntSide) + 1)];
				m_pVtx[nCnt].nor = m_pVtx[nCnt].nor / 3;
			}
			else if (nCntVertical == nNumVertical)
			{
				m_pVtx[nCnt].nor = m_Nor[(((nNumVertical * 2) * nCntSide) + (nNumVertical * 2) - 1)] + m_Nor[(((nNumVertical * 2) * nCntSide) - 1)] + m_Nor[(((nNumVertical * 2) * nCntSide) - 2)];
				m_pVtx[nCnt].nor = m_pVtx[nCnt].nor / 3;
			}
			else
			{
				m_pVtx[nCnt].nor = m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) - ((nNumVertical * 2) + 2))] +
					m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) - ((nNumVertical * 2) + 1))] +
					m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) - ((nNumVertical * 2)))] +
					m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) - 1)] +
					m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)))] +
					m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) + 1)];

				m_pVtx[nCnt].nor = m_pVtx[nCnt].nor / 6;
			}
		}
		else if (nCntSide == nNumSide)
		{// 一番下のパターン
			if (nCntVertical == 0)
			{
				m_pVtx[nCnt].nor = m_Nor[(((nNumVertical * 2) * nCntSide) - (nNumVertical * 2))];
			}
			else if (nCntVertical == nNumVertical)
			{
				m_pVtx[nCnt].nor = m_Nor[(((nNumVertical * 2) * nCntSide) - 1)] + m_Nor[(((nNumVertical * 2) * nCntSide) - 2)];
				m_pVtx[nCnt].nor = m_pVtx[nCnt].nor / 2;
			}
			else
			{
				m_pVtx[nCnt].nor = m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) - ((nNumVertical * 2)))] +
					m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) - ((nNumVertical * 2) + 1))] +
					m_Nor[((((nNumVertical * 2) * nCntSide) + (nCntVertical * 2)) - ((nNumVertical * 2) + 2))];
				m_pVtx[nCnt].nor = m_pVtx[nCnt].nor / 3;
			}
		}

		// 端っこまで来たら
		if (nCntVertical == nNumVertical)
		{
			// 下に一つ進む
			nCntSide += 1;

			// ０からのスタート
			nCntVertical = 0;
		}
		else
		{
			// 横に進める
			nCntVertical += 1;
		}
	}
}

//=============================================================================
// ポリゴンにある△の垂直ベクトルを外積による計算で求める処理
//=============================================================================
D3DXVECTOR3 CMesh3D::PolygonNormal(const D3DXVECTOR3 & pos0, const D3DXVECTOR3 & pos1, const D3DXVECTOR3 & pos2)
{
	// 計算保管変数
	D3DXVECTOR3 Pos01, Pos12, Normal;

	// ベクトルを計算
	Pos01 = (pos1 - pos0);
	Pos12 = (pos2 - pos0);

	// 各座標のベクトルを計算
	Normal.x = Pos01.y  * Pos12.z - Pos01.z * Pos12.y;
	Normal.y = Pos01.z  * Pos12.x - Pos01.x * Pos12.z;
	Normal.z = Pos01.x  * Pos12.y - Pos01.y * Pos12.x;

	// 計算結果を正規化する
	D3DXVec3Normalize(&Normal, &Normal);

	return Normal;
}

//=============================================================================
// ポイント１とポイント２の距離
//=============================================================================
float CMesh3D::getDistance(float x, float y, float x2, float y2)
{
	return sqrtf((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
}
