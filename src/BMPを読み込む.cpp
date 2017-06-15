/**************************************************************************

	○このファイルでは2つの関数を定義します．

○このファイルではもと画像のビットマップの読み込みDDB関数を作成します．
○このファイルではもと画像を作業用(バッファ)画像に合成する関数を作成します．

***************************************************************************/

#include <windows.h>					//ウィンドウを使うプログラムには必須


int ReadDIB_MakeDDB(HDC, LPCTSTR,BITMAP *,HDC *);//#

//hdc<-デバイスコンテキスト
//FileName<-読み込むファイル名
//*bitmap_original->もと画像用ビットマップバッファ
//*hBuffer_original->もと画像用ビットマップのメモリデバイスコンテキスト
int ReadDIB_MakeDDB(HDC hdc, LPCTSTR FileName, 
					BITMAP *bitmap_original,HDC *hBuffer_original)
{

	HBITMAP hBitmap_original;			//もと画像ビットマップのハンドル
	//---保存用ビットマップ構造体---
	BITMAPFILEHEADER bmpFileHeader;//BITMAPFILEHEADER構造体
	BITMAPINFO *bmpInfo;//BITMAPINFO構造体
	BYTE *bPixelBits;//ビット配列

    HANDLE hFile;//ファイル用ハンドル
	DWORD dwBytes;//実際に読み取られたバイト数が格納されるDWORD 型の変数

	//----保存用ビットマップの構造を読み込む
	hFile = CreateFile(							//オブジェクトを作成またはオープン
		FileName,								//作成するオブジェクトの名前が入ったポインタを指定
		GENERIC_READ,							//GENERIC_READ ->読み取りアクセス
		0,										//オブジェクトの共有方法を指定,'０'を指定すると共有されない
		NULL,									//取得したハンドルの子プロセスへの継承を許可,NULLは継承なし
		OPEN_EXISTING,							//OPEN_EXISTING ->ファイルをオープン 
		FILE_ATTRIBUTE_NORMAL,					//ファイルの属性およびフラグを指定
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE){			//関数が失敗すると
//		MessageBox(NULL , TEXT("ファイルが開けません") , NULL , MB_OK);
		return 1;
	}
	//+BITMAPFILEHEADER構造体を読み込む
	ReadFile(									//ファイルからデータを読み取る
		hFile,									//ファイルのハンドルを指定
		&(bmpFileHeader),						//ファイルから読み取ったデータが格納される
		sizeof(BITMAPFILEHEADER),				//読み取るバイト数を指定
		&dwBytes,								//実際に読み取られたバイト数が格納
		NULL);
	if ((bmpFileHeader).bfType != 0x4D42) {		//0x4D42は"BM"
		MessageBox(NULL , TEXT("ビットマップファイルではありません") , NULL , MB_OK);
		return 1;
	}
	//+
	//++BITMAPINFO構造体を読み込む
	//メモリーを確保
	(bmpInfo) = (BITMAPINFO *) malloc ((bmpFileHeader).bfOffBits - dwBytes);
	//データを読み取る
	ReadFile(hFile,(bmpInfo),
		(bmpFileHeader).bfOffBits - dwBytes,&dwBytes,NULL);
	//++
	//+++BITMAPINFO構造体を読み込む
	//メモリーを確保
	(bPixelBits) = (BYTE *) malloc ((bmpFileHeader).bfSize - (bmpFileHeader).bfOffBits);
	//データを読み取る
	ReadFile(hFile,(bPixelBits),
		(bmpFileHeader).bfSize - (bmpFileHeader).bfOffBits,&dwBytes,NULL);
	//+++
	CloseHandle(hFile);//CreateFile 関数が返したハンドルをクローズ
	//----

	hBitmap_original = CreateDIBitmap(hdc ,&bmpInfo->bmiHeader,CBM_INIT,
		bPixelBits ,bmpInfo ,DIB_RGB_COLORS);

	//指定されたデバイスと互換性のあるメモリデバイスコンテキストを作成
	(*hBuffer_original) = CreateCompatibleDC(hdc);
	//指定されたデバイスコンテキストに、指定されたオブジェクトを選択
	SelectObject((*hBuffer_original) , hBitmap_original);
	GetObject(hBitmap_original,sizeof(BITMAP),&(*bitmap_original));

	//-----開放
	free(bPixelBits);//ビット配列
	free(bmpInfo);//BITMAPINFO構造体
	DeleteObject(hBitmap_original);//ビットマップのハンドル
	//-----

    return 0;
}


//////////////////////////////////
//#もと画像を作業用(バッファ)画像に合成
//
void ComposeDDB(HWND hWnd,HDC hBuffer,LPCTSTR szFileName,int x, int y){

	HDC hdc;
	hdc = GetDC(hWnd);//デバイスコンテキストのハンドルを取得

	static BITMAP	bitmap_original;		//もと画像用ビットマップバッファ
	static HDC		hBuffer_original;		//もと画像用ビットマップのメモリデバイスコンテキスト


	ReadDIB_MakeDDB(hdc,szFileName,&bitmap_original,&hBuffer_original);
	//----もと画像用ビットマップを作業用ビットマップに合成
	BitBlt(	
		hBuffer,							//コピー先デバイスコンテキストのハンドル
		x,									//コピー先の X 座標
		y,									//コピー先のY 座標
		bitmap_original.bmWidth,			//幅
		bitmap_original.bmHeight,			//高さ
		hBuffer_original,					//コピー元のデバイスコンテキストのハンドル
		0,									//コピー元の X 座標
		0,									//コピー元の Y 座標
		//NOTSRCCOPY
		//SRCAND//加算合成
		SRCCOPY//そのままコピー
		);
	//-----

	//デバイスコンテキストのハンドルを開放
	if (hBuffer_original){DeleteDC(hBuffer_original);}
	ReleaseDC(hWnd,hdc);					//デバイスコンテキストを解放

}