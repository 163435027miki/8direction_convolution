/**************************************************************************

	○このファイルでは使用するウィンドウをWindowsのシステムに登録します．
	WNDCLASSEXで設定し，RegisterClassEx()で登録します．
	
***************************************************************************/
#include <windows.h>					//ウィンドウを使うプログラムには必須

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//ウィンドウプロシージャ
													 //の宣言
/////////////////////////////
//ウィンドウ・クラスの登録
////
ATOM InitApp(HINSTANCE hInst, char szClassName[])
{
    WNDCLASSEX wc;	     		//新しく作るウィンドウクラス用の変数wcを定義
    wc.cbSize = sizeof(WNDCLASSEX);					//WNDCLASSEXの大きさ(容量の)
    wc.style = CS_HREDRAW | CS_VREDRAW;				//ウィンドウの動き方の設定
    wc.lpfnWndProc = WndProc;						//このクラスが持つプロシージャ名
    wc.cbClsExtra = 0;							
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;									//インスタンスハンドル
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//アイコンの指定
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			//小さなアイコンの指定
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);				//カーソルの指定
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	//背景の色の指定
    wc.lpszMenuName = "MYMENU";								//メニュー名
    wc.lpszClassName = (LPCSTR)szClassName;					//ウィンドウクラスの名前
    return (RegisterClassEx(&wc));							//ウィンドウクラスの登録
}
/////////////////////
