/**************************************************************************

	○このファイルでは使用するウィンドウを生成し，表示します．
	
***************************************************************************/

#include "filter.h"

//ウィンドウの生成
//
BOOL InitInstance(HINSTANCE hInst, int nCmdShow, char szClassName[])
{


	int width;
	int height;
	// ウインドウ時はウインドウの外枠のサイズを考慮する
	width = WIDTH + GetSystemMetrics(SM_CXSIZEFRAME/*枠の幅*/) * 2;
	height = HEIGHT + GetSystemMetrics(SM_CYSIZEFRAME/*枠の高さ*/) * 2 
		+ GetSystemMetrics(SM_CYCAPTION/*タイトルバーの高さ*/)
		//+ GetSystemMetrics(SM_CYMENU/*メニューバーの高さ*/)
		;
    HWND hWnd = CreateWindow(szClassName,
						"フィルタ演算",	//タイトルバーにこの名前が表示されます
						WS_OVERLAPPEDWINDOW	,//ウィンドウの種類
						120,		//Ｘ座標
						0,		//Ｙ座標
						width,				//幅
						height,				//高さ
						HWND_DESKTOP,		//親ウィンドウのハンドル、親を作るときはNULL
						NULL,		//メニューハンドル、クラスメニューを使うときはNULL
						hInst,		//インスタンスハンドル
						NULL);

	//ボタン
	CreateWindow( "BUTTON","実行",WS_CHILD | WS_VISIBLE  |  BS_PUSHBUTTON,
		300, 10, 60, 20, hWnd, (HMENU)ID_BUTTON0, hInst, NULL );




    if (!hWnd)	return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    return TRUE;
}