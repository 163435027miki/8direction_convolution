/**************************************************************************

	○このファイルではウィンドウからのメッセージ（ボタンをクリックしたなど）
	を受け取って，実行する関数を呼び出します．
	
***************************************************************************/
#include "filter.h"

//メモリ確保とファイル名の結合を行うためのヘッダ
#define ANSI				
#include "nrutil.h"				
#include<stdio.h>


/////////////////////////
//ウィンドウプロシージャ
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    static PAINTSTRUCT ps;	//APIの型
	static BITMAP bitmap;	//APIの型
	static HDC hBuffer;		//APIの型


	switch (msg) {
		case WM_CREATE:

			//#作業用(バッファ)画像を作成(ウィンドウの画素値をメモリhBufferに確保)
			MakeBuffDDB(hWnd, &hBuffer, &bitmap,0,50);		//ユーザ定義の関数							
			//指定範囲を白く塗りつぶす
			PatBlt(hBuffer , 0 , 0 ,WIDTH ,HEIGHT , WHITENESS);//APIの関数
			//hBufferの値を画面に表示
			LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);//ユーザ定義の関数	
		break;


		case WM_COMMAND:
			switch (LOWORD(wp)) {

				case ID_BUTTON0://フィルタ演算

					PatBlt(hBuffer,	 0,  50,  WIDTH,  HEIGHT,  WHITENESS);//APIの関数
					Space_filter( hWnd, hBuffer, bitmap );	//ユーザ定義の関数//フィルタ演算

				break;

			}
		break;

		case WM_PAINT:
			//画面に表示
			BeginPaint(hWnd, &ps);//APIの関数
				//テキストを表示
				TextOut(ps.hdc,30,10,"bmpを読み込んでフィルタ演算",strlen("bmpを読み込んでフィルタ演算"));//APIの関数
				LookBMP(hWnd,  hBuffer,  bitmap,  0,  50);	//hBufferの値を画面に表示//ユーザ定義の関数	
			EndPaint(hWnd, &ps);//APIの関数
		break;
			
		case WM_CLOSE:

            DestroyWindow(hWnd);//Windowを破棄//APIの関数

		break;

		case WM_DESTROY:
			//アプリケーションの終了時の後かたづけ
			if(!ExitApp(hBuffer)){return FALSE;}	//ユーザ定義の関数
			PostQuitMessage(0);//APIの関数
		break;
		default:
		return (DefWindowProc(hWnd, msg, wp, lp));//APIの関数
	}
    return 0;
}

