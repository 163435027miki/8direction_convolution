/**************************************************************************

	○このファイルではBMPを画面に表示する関数を作成します．

***************************************************************************/



#include <windows.h>					//ウィンドウを使うプログラムには必須

//////////////////////////////////
//#画面に表示
//hWnd->ウィンドウのハンドル
int LookBMP(HWND hWnd,HDC hBuffer,BITMAP bitmap,int x,int y)
{
    RECT rt;
	HDC hdc;

	hdc = GetDC(hWnd);//デバイスコンテキストのハンドルを取得
    GetClientRect(hWnd, &rt);//クライアント領域の座標を取得

	StretchBlt(	hdc , rt.left+x , rt.top+y, rt.right , rt.bottom ,
		hBuffer , 0+x, 0+y, bitmap.bmWidth , bitmap.bmHeight , SRCCOPY);

	ReleaseDC(hWnd,hdc);//デバイスコンテキストを解放	
    return 0;
}