/**************************************************************************
	◎注意：このプログラムを使うプロジェクトは，Win32 Console Application
	ではなくWin32 Application で作る．
***************************************************************************/

#include <windows.h>					//ウィンドウを使うプログラムには必須
#include <stdio.h>

ATOM InitApp(HINSTANCE, char szClassName[]);	//ウィンドウ・クラスを登録す
												//る関数の宣言

BOOL InitInstance(HINSTANCE, int, char szClassName[]);//ウィンドウを生成する
													  //関数の宣言

///////////////////////////////////////////////////////////
//WINAPIメイン
//
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
                   LPSTR lpsCmdLine, int nCmdShow)
{
	char szClassName[1024] = "フィルタ演算";        //ウィンドウクラス

	::AllocConsole();								//コマンドプロント画面の表示
	freopen( "CON", "r", stdin  );  // 標準入力を割り当てる
	freopen( "CON", "w", stdout );  // 標準出力を割り当てる

    MSG msg;

    if (!InitApp(hCurInst,szClassName))
        return FALSE;
    if (!InitInstance(hCurInst, nCmdShow,szClassName)) 
        return FALSE;


    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

