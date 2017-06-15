/**************************************************************************

	○このファイルでは4つの関数を作成します．

○このファイルではDIB形式でBMPデータを保存する関数を作成します．
○このファイルではDDB形式でBMPファイルを保存する関数を作成します．
○このファイルではDDB形式でBMPファイルを保存するためにビットマップ
　ファイルのデータを読み込む関数を作成します．
○このファイルではアプリケーションの終了時の後かたづけをする関数を作成します．
***************************************************************************/



#include <windows.h>					//ウィンドウを使うプログラムには必須

//---保存用ビットマップ構造体---
BITMAPFILEHEADER bmpFileHeader_min;//BITMAPFILEHEADER構造体
BITMAPINFO *bmpInfo_min;//BITMAPINFO構造体
BYTE *bPixelBits_min;//ビット配列

int SaveDIB(LPCTSTR,BITMAPFILEHEADER*,BITMAPINFO*,BYTE*);//#ビットマップを保存する
int ReadDIB2(LPCTSTR);//#ビットマップを読み込む

//
void SaveDDB(HWND hWnd,HDC hBuffer,LPCTSTR szFileName,LPCTSTR strSaveFileName,int x, int y){

	//-----変数宣言
	static HBITMAP	hBitmap_save;//保存用ビットマップのハンドル
	static BITMAP	bitmap_save;//保存用ビットマップバッファ
	static HDC		hBuffer_save;//保存用ビットマップのメモリデバイスコンテキスト

	//-----デバイスコンテキストのハンドルを取得
	HDC hdc;
	hdc = GetDC(hWnd);
	//-----

	//----保存用ビットマップのDDBを作成
	ReadDIB2(szFileName);
	hBitmap_save = CreateDIBitmap(//デバイス独立 (DIB) からデバイス依存(DDB) を作成
		hdc ,//デバイスコンテキストのハンドルを指定
		&bmpInfo_min->bmiHeader ,//DIB 情報ヘッダへのポインタを指定
		CBM_INIT ,//初期化の有無,CBM_INITは初期化
		bPixelBits_min ,//ピクセルビットへのポインタを指定
		bmpInfo_min ,//情報ヘッダへのポインタを指定
		DIB_RGB_COLORS//カラー情報フラグを指定
		);				
	//指定されたデバイスと互換性のあるメモリデバイスコンテキストを作成
	hBuffer_save = CreateCompatibleDC(hdc);
	//指定されたデバイスコンテキストに、指定されたオブジェクトを選択
	SelectObject(hBuffer_save, hBitmap_save);
	GetObject(hBitmap_save,sizeof(BITMAP),&bitmap_save);	

	//作業用ビットマップを保存用ビットマップにコピー
	BitBlt(	
			hBuffer_save,
			0,
			0,
			bitmap_save.bmWidth,
			bitmap_save.bmHeight,
			hBuffer,
			0+x,
			0+y,
			SRCCOPY//そのままコピー
			);
	//保存用ビットマップをDDBからDIBへ変換
	GetDIBits(
			   hBuffer_save,
			   hBitmap_save,
			   0,
			   bitmap_save.bmHeight,
			   bPixelBits_min,
			   bmpInfo_min,
			   DIB_RGB_COLORS
			   );
	//保存用ビットマップを保存
	SaveDIB(strSaveFileName,
		&bmpFileHeader_min,
		bmpInfo_min,
		bPixelBits_min
		);//#

	//-----

	//ビットマップのハンドルを開放
	DeleteObject(hBitmap_save);
	//デバイスコンテキストのハンドルを開放
	if (hBuffer_save){DeleteDC(hBuffer_save);}
	ReleaseDC(hWnd,hdc);//デバイスコンテキストを解放

}

//////////////////////////////////

//////////////////////////
//#ビットマップを保存する
//FileName<-保存するファイル名
//*bmpFileHeader<-保存するBITMAPFILEHEADER構造体のポインタ
//*bmpInfo<-保存するBITMAPINFO構造体のポインタ
//*bPixelBits<-保存するビット配列のポインタ
int SaveDIB(LPCTSTR FileName,
			BITMAPFILEHEADER *bmpFileHeader,BITMAPINFO *bmpInfo,BYTE *bPixelBits)
{

    HANDLE hFile;
	DWORD dwBytes;//実際に書き込まれるバイト数が格納されるDWORD 型の変数
		hFile = CreateFile(//オブジェクトを作成またはオープン
			FileName,//作成するオブジェクトの名前が入ったポインタを指定
			GENERIC_WRITE,//GENERIC_WRITE ->書き込みアクセス 
			0,//オブジェクトの共有方法を指定,'０'を指定すると共有されない
			NULL,//取得したハンドルの子プロセスへの継承を許可,NULLは継承なし
			CREATE_ALWAYS,//新しいファイルを作成,上書き
			FILE_ATTRIBUTE_NORMAL,//ファイルの属性およびフラグを指定
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE){//関数が失敗すると
			return 0;
		}

	    //BITMAPFILEHEADER構造体を書きこむ
		WriteFile(//ファイルにデータを書き込み
			hFile,//ファイルのハンドルを指定
			bmpFileHeader,//書き込むデータが入ったバッファへのポインタを指定
			sizeof(BITMAPFILEHEADER),//書き込むバイト数を指定
			&dwBytes,//実際に書き込まれたバイト数が格納
			NULL
			);
		//BITMAPINFO構造体を書きこむ
		WriteFile(
			hFile,
			bmpInfo,
			bmpFileHeader_min.bfOffBits - sizeof(BITMAPFILEHEADER),
			&dwBytes,
			NULL
			);
		//ビット配列を書きこむ
		WriteFile(
			hFile,
			bPixelBits,
			bmpFileHeader_min.bfSize - bmpFileHeader_min.bfOffBits,
			&dwBytes,
			NULL);
		CloseHandle(hFile);//CreateFile 関数が返したハンドルをクローズ
    return 0;
}

////////////////////////////////////////////////////////////

//////////////////////////
//#ビットマップを読み込む
//FileName<-読み込むファイル名
int ReadDIB2(LPCTSTR FileName)
{
    HANDLE hFile;//ファイル用ハンドル
	DWORD dwBytes;//実際に読み取られたバイト数が格納されるDWORD 型の変数
	//----保存用ビットマップの構造を読み込む
	hFile = CreateFile(//オブジェクトを作成またはオープン
		FileName,//作成するオブジェクトの名前が入ったポインタを指定
		GENERIC_READ,//GENERIC_READ ->読み取りアクセス
		0,//オブジェクトの共有方法を指定,'０'を指定すると共有されない
		NULL,//取得したハンドルの子プロセスへの継承を許可,NULLは継承なし
		OPEN_EXISTING,//OPEN_EXISTING ->ファイルをオープン 
		FILE_ATTRIBUTE_NORMAL,//ファイルの属性およびフラグを指定
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE){//関数が失敗すると
		MessageBox(NULL , TEXT("画像ファイルが開けません") , NULL , MB_OK);
		return 1;
	}
	//+BITMAPFILEHEADER構造体を読み込む
	ReadFile(//ファイルからデータを読み取る
		hFile,//ファイルのハンドルを指定
		&(bmpFileHeader_min),//ファイルから読み取ったデータが格納される
		sizeof(BITMAPFILEHEADER),//読み取るバイト数を指定
		&dwBytes,//実際に読み取られたバイト数が格納
		NULL);
	if ((bmpFileHeader_min).bfType != 0x4D42) {//0x4D42は"BM"
		MessageBox(NULL , TEXT("ビットマップファイルではありません") , NULL , MB_OK);
		return 1;
	}
	//+
	//++BITMAPINFO構造体を読み込む
	//メモリーを確保
	(bmpInfo_min) = (BITMAPINFO *) malloc ((bmpFileHeader_min).bfOffBits - dwBytes);
	//データを読み取る
	ReadFile(hFile,(bmpInfo_min),
		(bmpFileHeader_min).bfOffBits - dwBytes,&dwBytes,NULL);
	//++
	//+++BITMAPINFO構造体を読み込む
	//メモリーを確保
	(bPixelBits_min) = (BYTE *) malloc ((bmpFileHeader_min).bfSize - (bmpFileHeader_min).bfOffBits);
	//データを読み取る
	ReadFile(hFile,(bPixelBits_min),
		(bmpFileHeader_min).bfSize - (bmpFileHeader_min).bfOffBits,&dwBytes,NULL);
	//+++
	CloseHandle(hFile);//CreateFile 関数が返したハンドルをクローズ
	//----
    return 0;
}


//#アプリケーションの終了時の後かたづけをする関数
//
bool ExitApp(HDC hBuffer){

	free(bPixelBits_min);
	free(bmpInfo_min);

	if (hBuffer) {DeleteDC(hBuffer);}

	return TRUE;
}


/////////////////////////////////////////////

