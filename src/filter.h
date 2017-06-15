#include <windows.h>			//ウィンドウを使うプログラムには必須


#define WIDTH 1300
#define HEIGHT 700

//ボタン
#define ID_BUTTON0 100


//#作業用(バッファ)画像を作成
void MakeBuffDDB(HWND hWnd,  HDC *hBuffer,  BITMAP *bitmap,  int x,  int y);
//#画像をバッファ画像に合成
void ComposeDDB(HWND hWnd,  HDC hBuffer,  LPCTSTR szFileName,  int x,  int y);
//#画面に表示する
int LookBMP(HWND hWnd,HDC hBuffer,BITMAP bitmap,int x,int y);
//画像をビットマップに保存
void SaveDDB(HWND hWnd,  HDC hBuffer,  LPCTSTR szFileName,  LPCTSTR strSaveFileName,  int x,  int y);
//#開放処理
bool ExitApp(HDC hBuffer);

//フィルタ演算
void Space_filter(HWND hWnd,  HDC hBuffer,BITMAP bitmap);

