/**************************************************************************

	○このファイルでは作業用(バッファ)画像を作成します．
	
***************************************************************************/


#include "filter.h"



void MakeBuffDDB(HWND hWnd,HDC *hBuffer,BITMAP *bitmap,int x,int y){

	HDC hdc;
	hdc = GetDC(hWnd);//デバイスコンテキストのハンドルを取得

	HDC hBuf;
	BITMAP bitm;

	hBuf=*hBuffer;
	bitm=*bitmap;

	static HBITMAP	hBitmap;//作業用ビットマップのハンドル
	
	//----作業用のDDBを作成
	hBitmap = CreateCompatibleBitmap(hdc , WIDTH ,HEIGHT);
	//指定されたデバイスと互換性のあるメモリデバイスコンテキストを作成
	hBuf = CreateCompatibleDC(hdc);
	//指定されたデバイスコンテキストに、指定されたオブジェクトを選択
	SelectObject(hBuf , hBitmap);
	//ペンを指定
	SelectObject(hBuf , GetStockObject(NULL_PEN));
	//指定された長方形を現在のブラシでペイント
	PatBlt(hBuf , 0 +x, 0+y ,WIDTH -x,HEIGHT -y, WHITENESS);
	GetObject(//指定されたグラフィックオブジェクトの情報を取得
		hBitmap,//グラフィックオブジェクトのハンドルを指定
		sizeof(BITMAP),//バッファのサイズを指定
		&bitm//バッファへのポインタを指定,このバッファにオブジェクトの情報が格納
		);
	//----

	//ビットマップのハンドルを開放
	DeleteObject(hBitmap);

	*hBuffer=hBuf;
	*bitmap=bitm;

}

